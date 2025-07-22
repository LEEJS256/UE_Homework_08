// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_01_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()
	

public:
	AHealingItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 m_HealAmount;


protected:

	virtual void ActivateItem(AActor* Activator) override;
	virtual void Tick(float DeltaTime) override;


};
