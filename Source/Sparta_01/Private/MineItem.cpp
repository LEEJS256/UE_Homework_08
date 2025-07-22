// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	m_ItemType = "Mine";
	m_ExplosionDelay = 5.0f;
	m_ExplosionDamage = 30.f;
	m_ExplosionRadius = 300.f;

	m_ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion_Collision"));
	m_ExplosionCollision->InitSphereRadius(m_ExplosionRadius);
	m_ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	m_ExplosionCollision->SetupAttachment(m_Scene);

}


void AMineItem::Explode()
{
	UParticleSystemComponent* pParticle = nullptr;
	//if (m_Explode_PickUpParticle)
	//{
	//	 pParticle = UGameplayStatics::SpawnEmitterAtLocation(
	//		GetWorld(),
	//		m_Explode_PickUpParticle,
	//		GetActorLocation(),
	//		GetActorRotation(),
	//		false
	//	);

	//	 if (pParticle && IsValid(pParticle))
	//	 {
	//		 pParticle->bAutoDestroy = true;
	//	 }


	//}

	//if (m_Explode_PickUpSound)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(
	//		GetWorld(),
	//		m_Explode_PickUpSound,
	//		GetActorLocation()
	//	);
	//}
	TArray<AActor*> OverlappingActors;
	m_ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{

		if (Actor && Actor->ActorHasTag("Player"))
		{
			//UGamePlayStatics::A
			UGameplayStatics::ApplyDamage(
				Actor,
				m_ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
	
	//if (pParticle)
	//{
	//	FTimerHandle DestoryParticleTimerHandle;

	//	GetWorld()->GetTimerManager().SetTimer(
	//		DestoryParticleTimerHandle,
	//		[pParticle]()
	//		{
	//			pParticle->DestroyComponent();
	//		},
	//		2.0f,
	//		false

	//	);
	//}

	DestoryItem();
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (m_bHasExploded)
		return;
	Super::ActivateItem(Activator);
	GetWorld()->GetTimerManager().SetTimer(m_ExplosionHander
		,this
		,&AMineItem::Explode
		, m_ExplosionDelay
		,false
		);
	m_bHasExploded = true;
}