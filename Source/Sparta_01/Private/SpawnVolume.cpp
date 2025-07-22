// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Sparta_GameState.h"
#include "Components/BoxComponent.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(m_Scene);

	m_SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawingBox"));
	m_SpawningBox->SetupAttachment(m_Scene);

	m_ItemDataTable = nullptr;

}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = m_SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = m_SpawningBox->GetComponentLocation(); // 중심좌표


	//박스중심으로부터 길이사이의 랜덤값적용된좌표
	return BoxOrigin + FVector
	(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X ),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	)
		;
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->m_ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;;

}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if(!m_ItemDataTable)
	{
		return nullptr;
	}


	ASparta_GameState* pSparta_GameState = GetWorld() ? GetWorld()->GetGameState<ASparta_GameState>() : nullptr;
	int32 i_level = pSparta_GameState->Get_Level() + 1;


	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));


	m_ItemDataTable->GetAllRows(ContextString,AllRows);

	if (AllRows.IsEmpty())
		return nullptr;

	float TotalChance = 0.f;

	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->m_SpawnChance;
		}
	}
	const float RandomValue = FMath::FMath::FRandRange(0.0f, TotalChance);

	float AccumulateChance = 0.f;

	//누적랜덤값 뽑기
	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->m_SpawnChance;
		if (RandomValue <= AccumulateChance)
		{
			return Row;
		}
	}
	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass)
		return nullptr;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
		
	);
	return SpawnedActor;;

}


