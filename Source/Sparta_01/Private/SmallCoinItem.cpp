// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallCoinItem.h"


ASmallCoinItem::ASmallCoinItem()
{
	m_ItemType = "SmallCoin";
	m_PointValue = 10;
}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	// DestoryItem();
}

void ASmallCoinItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 90.f * DeltaTime;

	SetActorRotation(CurrentRotation);

}