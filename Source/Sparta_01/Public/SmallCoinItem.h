// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "SmallCoinItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_01_API ASmallCoinItem : public ACoinItem
{
	GENERATED_BODY()
	
	ASmallCoinItem();

protected:

	virtual void ActivateItem(AActor* Activator) override;
	virtual void Tick(float DeltaTime) override;

};
