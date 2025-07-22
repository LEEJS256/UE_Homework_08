 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPARTA_01_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* m_Scene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* m_ItemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent*		m_SpawningBox	;

	
	UFUNCTION(BlueprintCallable,Category="Spawning")
	AActor* SpawnRandomItem();

	FItemSpawnRow* GetRandomItem()	const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector		GetRandomPointInVolume() const;

protected:

};
