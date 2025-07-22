// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "U_WBP_HUD.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_01_API UU_WBP_HUD : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// ������ ������ �� ȣ��
	virtual void NativeConstruct() override;

	UPROPERTY()
	UImage* m_ProgressBar_Time;
	
	UPROPERTY()
	UProgressBar* m_ProgressBar_HP;
	UPROPERTY()
	UProgressBar* m_ProgressBar_Speed;

	UPROPERTY()
	class UMaterialInstanceDynamic* m_DynamicMaterialInstance =nullptr;
public:

	// Percentage �Ķ���� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void Set_ProgressBar_Percentage(float arg_Percentage);





	float Get_PlayerHP_Percent() const;
	void Set_PlayerHP_Percent(float arg_Percentage);

	void Set_ProgressBar_Percentage_Speed(float arg_Percentage);

	void Update_Text_HP(float hp, float hp_max);
	void Update_Text_Speed(float speed, float speed_max);
	void Update_Text_Name(FString& name);

	// Percentage �Ķ���� �������� �Լ�
	UFUNCTION(BlueprintCallable, Category = "HUD")
	float GetProgressPercentage() const;

	// ��Ƽ���� �ν��Ͻ� �ʱ�ȭ �Լ�
	void InitializeMaterialInstance();
	

	void PlayTextBlink(const FName& TextBlockName, const FName& AnimationName);
	void Set_TextBox_Text(const FName& TextBlockName, const FString& Text_value);
	void Update_TextBox_Level();
	UWidgetAnimation* FindAnimationByName(FName AnimationName) ;
private:

};
