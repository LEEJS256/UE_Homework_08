// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Slow_Item.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_01_API ASlow_Item : public ABaseItem
{
	GENERATED_BODY()
public:
	ASlow_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 m_SpeedAmount;


protected:
	virtual void Tick(float DeltaTime) override;
	virtual void ActivateItem(AActor* Activator) override;
};
