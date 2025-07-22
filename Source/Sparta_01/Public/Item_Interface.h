// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItem_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPARTA_01_API IItem_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,  //자기자신 컴포넌트
		AActor* OtherActor,						//부딪히는 상대방엑터 ex=플레이어
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	virtual void ActivateItem(AActor* OverlapActor) = 0;
	virtual FName GetItemType() const = 0;



};
