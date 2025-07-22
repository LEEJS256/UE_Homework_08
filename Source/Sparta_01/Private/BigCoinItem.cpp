// Fill out your copyright notice in the Description page of Project Settings.


#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	m_ItemType		= "BigCoin";
	m_PointValue	= 50;
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	//DestoryItem();
}


 //Called every frame
void ABigCoinItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 90.f * DeltaTime;

	SetActorRotation(CurrentRotation);

}
