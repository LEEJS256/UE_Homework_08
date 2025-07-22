// Fill out your copyright notice in the Description page of Project Settings.


#include "Slow_Item.h"
#include "Sparta_Character.h"

ASlow_Item::ASlow_Item()
{
	m_ItemType = "SpeedUPDown";
	m_SpeedAmount = -300;

}

void ASlow_Item::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{

		if (ASparta_Character* PlayerCharacter = Cast<ASparta_Character>(Activator))
		{
			PlayerCharacter->Set_SpeedTemp(m_SpeedAmount);
		}


		DestoryItem();
	}
	//DestoryItem();
}

void ASlow_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 90.f * DeltaTime;

	SetActorRotation(CurrentRotation);

}

