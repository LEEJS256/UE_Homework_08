// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sparta_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;


UCLASS()
class SPARTA_01_API ASparta_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASparta_Character();

	//UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* m_OverHeadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CAMERA")
	USpringArmComponent*	m_SpringArmComp; //스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CAMERA")
	UCameraComponent*		m_CameraComp;
	UFUNCTION(BlueprintPure, Category = "Health")
	float Get_Health() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	float Get_Health_per();
	float Get_Health_Max() const;
	float Get_Speed_per();
	float Get_Speed() const;
	float Get_Speed_Max() const;

	void Set_SpeedTemp(float arg_speed);
	void Reset_Speed();
	FString Get_PlayerName() const;
	void Set_PlayerName(const FString& NewName);
private:

	//sprint 용 변수
	float m_fNormalSpeed = 600.f;
	float m_fSprintSpeed_Multiplier = 1.7f; //600 *1.7 10
	//float m_fSprintSpeed_Multiplier = 1.7f; //600 *1.7 10
	float m_fSprintSpeed = 0.f;

	float m_fTempSpeed = 0.f;
	float m_fOriginalMaxSpeed = 0.f; // 원래 속도 저장용
	//UPROPERTY()
	//FTimerHandle m_SpeedBoostTimerHandle;

protected:

	UPROPERTY()
	FTimerHandle m_SpeedBoostTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	FString m_PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float m_fMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float m_fHealth;

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;


	UFUNCTION()
	void Active_item(const FInputActionValue& value);

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	
	UFUNCTION()
	void Input_Pause(const FInputActionValue& value);

	
	void OnDeath();
	void UpdateOverHeadHp();
public:
	

};
