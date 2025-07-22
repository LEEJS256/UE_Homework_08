// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "Engine/World.h"
#include "Sparta_GameState.h"
// Sets default values
ACoinItem::ACoinItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PointValue = 0;
	m_ItemType = "DefaultCoin";


}

// Called when the game starts or when spawned
void ACoinItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoinItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(
		//	-1,
		//	3.0f, 
		//	FColor::Green, 
		//	FString::Printf(TEXT("Player Gained %d points"),m_PointValue));
		if (UWorld* pWorld = GetWorld())
		{
			if(ASparta_GameState* GameState =  pWorld->GetGameState<ASparta_GameState>())
			{
				GameState->Add_Score(m_PointValue);

				GameState->OnCoinCollected();
			}
		}

		DestoryItem();
	}
}


