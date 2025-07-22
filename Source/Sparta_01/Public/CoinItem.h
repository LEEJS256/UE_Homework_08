// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class SPARTA_01_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 m_PointValue; 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ActivateItem(AActor* Activator)			override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
