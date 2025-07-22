// Fill out your copyright notice in the Description page of Project Settings.


#include "Sparta_Character.h"
#include "Sparta_PlayerController.h"
#include "Sparta_GameState.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
// Sets default values
ASparta_Character::ASparta_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SpringArmComp		= CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_SpringArmComp->SetupAttachment(RootComponent);
	m_SpringArmComp->TargetArmLength = 300.f;
	m_SpringArmComp->bUsePawnControlRotation= true; //스피링암도 회전할때 같이회전하는 기능

	m_CameraComp		= CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_CameraComp->SetupAttachment(m_SpringArmComp, USpringArmComponent::SocketName); //카메라는 카메라암에 붙여	//그중에서 끝에
	m_CameraComp->bUsePawnControlRotation = false;//카메라는 회전안하도록 

	m_OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	m_OverHeadWidget->SetupAttachment(GetMesh());
	m_OverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	m_fSprintSpeed = m_fSprintSpeed_Multiplier * m_fNormalSpeed;
	
	GetCharacterMovement()->MaxWalkSpeed = m_fNormalSpeed;

	m_fMaxHealth = 100.f;
	m_fHealth = m_fMaxHealth;

}

float ASparta_Character::Get_Health() const
{
	return m_fHealth;
}

void ASparta_Character::AddHealth(float Amount)
{
	m_fHealth = FMath::Clamp(m_fHealth + Amount, 0.0f, m_fMaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health incresed to: %f"), m_fHealth);
	UpdateOverHeadHp();
}

float ASparta_Character::Get_Health_per()
{
	if (!m_fHealth)
		return 0.0f;
	else
		return (m_fHealth / m_fMaxHealth);
}

float ASparta_Character::Get_Speed_per()
{
	float CurrentSpeed = GetVelocity().Size(); // 현재 이동 속도 (벡터 길이)

	//float MaxSpeed = GetCharacterMovement()->GetMaxSpeed();
	float MaxSpeed = 2000.f;

	float SpeedRatio = CurrentSpeed / MaxSpeed;
	
	return SpeedRatio;
}

float ASparta_Character::Get_Health_Max() const
{
	return m_fMaxHealth;
}

float ASparta_Character::Get_Speed() const
{
	float CurrentSpeed = GetVelocity().Size(); // 현재 이동 속도 (벡터 길이)

	float MaxSpeed = GetCharacterMovement()->GetMaxSpeed();
	return CurrentSpeed;
}

float ASparta_Character::Get_Speed_Max() const
{
	float MaxSpeed = GetCharacterMovement()->GetMaxSpeed();
	return MaxSpeed;
}

void ASparta_Character::Set_SpeedTemp(float arg_speed)
{
	m_fTempSpeed = arg_speed;

	//// 현재 최대 속도 저장 (처음 호출시에만)
	//if (!GetWorld()->GetTimerManager().IsTimerActive(m_SpeedBoostTimerHandle))
	//{
	//	m_fOriginalMaxSpeed = GetCharacterMovement()->GetMaxSpeed();
	//}

	//// 새로운 속도 적용
	//GetCharacterMovement()->MaxWalkSpeed = m_fOriginalMaxSpeed + arg_speed;

	// 기존 타이머가 있다면 클리어
	GetWorld()->GetTimerManager().ClearTimer(m_SpeedBoostTimerHandle);

	// 5초 후 원래 속도로 복구하는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		m_SpeedBoostTimerHandle,
		this,
		&ASparta_Character::Reset_Speed,
		5.0f,
		false // 한 번만 실행
	);
}

void ASparta_Character::Reset_Speed()
{
	//if (GetCharacterMovement())
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = m_fOriginalMaxSpeed;
	//}

	m_fTempSpeed = 0.0f;
}

FString ASparta_Character::Get_PlayerName() const
{
	return m_PlayerName;
}

void ASparta_Character::Set_PlayerName(const FString& NewName)
{
	m_PlayerName = NewName;
}

// Called when the game starts or when spawned
void ASparta_Character::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverHeadHp();
	
}

// Called every frame
void ASparta_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ASparta_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASparta_PlayerController* PlayerController = Cast<ASparta_PlayerController>(GetController()))
		{
			if (PlayerController->m_ActiveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->m_ActiveAction,
					ETriggerEvent::Triggered,
					this,
					&ASparta_Character::Active_item
				);
			}

			if (PlayerController->m_PauseAction)
			{
				EnhancedInput->BindAction(
					PlayerController->m_PauseAction,
					ETriggerEvent::Triggered,
					this,
					&ASparta_Character::Input_Pause
				);
			}

			if (PlayerController->m_MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->m_MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASparta_Character::Move
				);
			}

			if (PlayerController->m_JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->m_JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASparta_Character::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->m_JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASparta_Character::StopJump
				);
			}

			if (PlayerController->m_LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->m_LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASparta_Character::Look
				);
		
			}

			if (PlayerController->m_SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->m_SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASparta_Character::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->m_SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASparta_Character::StopSprint
				);
			}

		}
	}
}

float ASparta_Character::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	m_fHealth = FMath::Clamp(m_fHealth - DamageAmount, 0.f, m_fMaxHealth);
	//UE_LOG(LogTemp, Warning, TEXT("Health decresed to: %f"), m_fHealth);
	if (m_fHealth < 0.f)
	{
		OnDeath();
	}
	UpdateOverHeadHp();

	return ActualDamage;
}

void ASparta_Character::Active_item(const FInputActionValue& value)
{

}

void ASparta_Character::Move(const FInputActionValue& value)
{
	if (!Controller)
		return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(),MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

}

void ASparta_Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASparta_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASparta_Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y); //Y축 지금은 반전된상태

}

void ASparta_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = m_fSprintSpeed + m_fTempSpeed;
		//GetCharacterMovement()->MaxWalkSpeed = m_fOriginalMaxSpeed
		
	}
}

void ASparta_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = m_fNormalSpeed + m_fTempSpeed;

	}
}

void ASparta_Character::Input_Pause(const FInputActionValue& value)
{
	if (APlayerController* pPLayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASparta_PlayerController* pSparta_PlayerController = Cast<ASparta_PlayerController>(pPLayerController))
		{

			pSparta_PlayerController->Show_MainMenu(true);
			pSparta_PlayerController->SetPause(true);
		}
	}
}

void ASparta_Character::OnDeath()
{
	//캐릭터 사망로직
	ASparta_GameState* pSpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASparta_GameState>() : nullptr;
	if (pSpartaGameState)
	{
		pSpartaGameState->OnGameOver();
	}
}

void ASparta_Character::UpdateOverHeadHp()
{
	if (!m_OverHeadWidget)
		return;

	UUserWidget* pOverHeadWidgetInstance = m_OverHeadWidget->GetUserWidgetObject();
	if (!pOverHeadWidgetInstance)
		return;

	if (UTextBlock* pHPText = Cast<UTextBlock>(pOverHeadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP_Text"))))
	{
		pHPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), m_fHealth, m_fMaxHealth)));
	}

}

