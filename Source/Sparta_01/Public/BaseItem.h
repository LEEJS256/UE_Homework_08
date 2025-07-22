// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Interface.h"
#include "BaseItem.generated.h" //맨아래에있어야함

class USphereComponent;

UCLASS()
class SPARTA_01_API ABaseItem : public AActor ,public IItem_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,  //자기자신 컴포넌트
		AActor* OtherActor,						//부딪히는 상대방엑터 ex=플레이어
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator)			override;
	virtual FName GetItemType() const						override;
	
	virtual void DestoryItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName					m_ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USceneComponent*		m_Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent*		m_Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent*	m_StaticMesh;

	//Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
	UParticleSystem* m_PickUpParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
	USoundBase* m_PickUpSound;
	FTimerHandle m_DestoryParticleTimerHandle;
	UParticleSystemComponent* m_SpawnedParticle = nullptr;
	void DestroyParticle();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
