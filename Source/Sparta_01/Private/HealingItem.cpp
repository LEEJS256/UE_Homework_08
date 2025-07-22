// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "Sparta_Character.h"

AHealingItem::AHealingItem()
{
	m_ItemType = "Healing";
	m_HealAmount = 20;
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
	/*	GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Green,
			FString::Printf(TEXT("Player Gained %d HP"),m_HealAmount ));
	*/
		if (ASparta_Character* PlayerCharacter = Cast<ASparta_Character>(Activator))
		{
			PlayerCharacter->AddHealth(m_HealAmount);
		}


		DestoryItem();
	}
	//DestoryItem();
}


void AHealingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector RotAxis = FVector(0, 1, 1).GetSafeNormal();

	float AngleDeg = 90.f * DeltaTime;

	FQuat DeltaRotation = FQuat(RotAxis, FMath::DegreesToRadians(AngleDeg));
	FQuat NewRotation = DeltaRotation * GetActorQuat();

	/*FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Roll += 90.f * DeltaTime;
	CurrentRotation.Roll  += 45.f * DeltaTime;*/


	SetActorRotation(NewRotation);

}