// Fill out your copyright notice in the Description page of Project Settings.


#include "Speed_Item.h"
#include "Sparta_Character.h"

ASpeed_Item::ASpeed_Item()
{
	m_ItemType = "SpeedUPDown";
	m_SpeedAmount = 400;

}

void ASpeed_Item::ActivateItem(AActor* Activator)
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

void ASpeed_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 90.f * DeltaTime;

	SetActorRotation(CurrentRotation);

}

