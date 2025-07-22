// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Speed_Item.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class SPARTA_01_API ASpeed_Item : public ABaseItem
{
	GENERATED_BODY()
public:
	ASpeed_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 m_SpeedAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* m_OverHeadWidget;
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void ActivateItem(AActor* Activator) override;
};
