// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(m_Scene);

	m_Collision= CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	m_Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));//Collision preset �������ִ� �ڵ�
	m_Collision->SetupAttachment(m_Scene);
	

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMesh->SetupAttachment(m_Collision);

	//�̺�Ʈ ���ε� 
	m_Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	m_Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(-1,3.0f, FColor::Green, FString::Printf(TEXT("delete!!")));
		ActivateItem(OtherActor); //��Ƽ����Ʈ ��Ŵ
	}
}

void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}



void ABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* pParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_PickUpParticle, GetActorLocation());

	if (pParticle)
	{
		m_SpawnedParticle = pParticle;
		m_SpawnedParticle->bAutoDestroy = true;
	}

	if (m_PickUpParticle)
	{
		// ��ƼŬ ���� (�ڵ� �ı��� false�� �� ���� ����)
		m_SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			m_PickUpParticle,
			GetActorLocation(),
			GetActorRotation(),
			true  // ���⼱ �������� ���� �Ŵϱ� false
		);
		// ���� ���
		if (m_PickUpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				m_PickUpSound,
				GetActorLocation(),
				true
			);
		}

		// 2�� �� DestroyParticle ȣ�� (��ƼŬ ����)
		if (m_SpawnedParticle)
		{
			GetWorld()->GetTimerManager().SetTimer(
				m_DestoryParticleTimerHandle,    // FTimerHandle
				this,                             // this ������
				&ABaseItem::DestroyParticle,      // ȣ���� ��� �Լ�
				2.0f,                             // 2�� �� ����
				true                             // �ݺ� �ƴ�
			);
		}
	}



}

FName ABaseItem::GetItemType() const
{
	return m_ItemType;
}

void ABaseItem::DestoryItem()
{
	Destroy();


}

void ABaseItem::DestroyParticle()
{


	if (m_SpawnedParticle && IsValid(m_SpawnedParticle))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Destroying Particle"));
		m_SpawnedParticle->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		m_SpawnedParticle->DestroyComponent();
		m_SpawnedParticle = nullptr;
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("m_SpawnedParticle is null or invalid"));
	}
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

