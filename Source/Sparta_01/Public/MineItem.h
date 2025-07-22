// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_01_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();


	//Sound
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
	//UParticleSystem* m_Explode_PickUpParticle;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
	//USoundBase* m_Explode_PickUpSound;



	USphereComponent* m_ExplosionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 m_ExplosionDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float m_ExplosionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float m_ExplosionRadius;

	FTimerHandle			m_ExplosionHander;
	void Explode();

	bool m_bHasExploded = false;
protected:

	
	virtual void ActivateItem(AActor* Activator) override;


	
};
