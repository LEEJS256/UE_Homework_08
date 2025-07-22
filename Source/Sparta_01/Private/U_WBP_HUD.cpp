// Fill out your copyright notice in the Description page of Project Settings.


#include "U_WBP_HUD.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "kismet/KismetSystemLibrary.h"
#include "Animation/WidgetAnimation.h" 
#include "Sparta_GameState.h"

#include "Engine/Engine.h"



void UU_WBP_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_ProgressBar_Time = Cast<UImage>(GetWidgetFromName(TEXT("ProgressBar_Time")));
	if (!m_ProgressBar_Time)
	{
		UE_LOG(LogTemp, Error, TEXT("ProgressBar_Time widget not found!"));
		return;
	}
	m_ProgressBar_HP = Cast<UProgressBar>(GetWidgetFromName(TEXT("Player_HP_Bar")));
	if (!m_ProgressBar_HP)
	{
		UE_LOG(LogTemp, Error, TEXT("ProgressBar_HPBar widget not found!"));
		return;
	}

	m_ProgressBar_Speed = Cast<UProgressBar>(GetWidgetFromName(TEXT("Player_Speed_Bar")));
	if (!m_ProgressBar_Speed)
	{
		UE_LOG(LogTemp, Error, TEXT("ProgressBar_SpeedBar widget not found!"));
		return;
	}

	// 머티리얼 인스턴스 초기화
	InitializeMaterialInstance();

	Update_TextBox_Level();


}


float UU_WBP_HUD::GetProgressPercentage() const
{
	if (m_DynamicMaterialInstance)
	{
		float CurrentPercentage = 0.0f;
		// "Percentage" 파라미터 값 가져오기
		if (m_DynamicMaterialInstance->GetScalarParameterValue(FName("Percentage"), CurrentPercentage))
		{
			return CurrentPercentage;
		}
	}
	return 0.0f;
}

void UU_WBP_HUD::Set_ProgressBar_Percentage(float arg_Percentage)
{
	if (m_DynamicMaterialInstance)
	{
		// "Percentage" 파라미터 설정
		m_DynamicMaterialInstance->SetScalarParameterValue(FName("Percentage"), arg_Percentage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DynamicMaterialInstance is null!"));
	}
}

void UU_WBP_HUD::Set_ProgressBar_Percentage_Speed(float arg_Percentage)
{
	if (m_ProgressBar_Speed)
	{
		// 퍼센트 값을 0.0 ~ 1.0 범위로 클램프
		float ClampedPercent = FMath::Clamp(arg_Percentage, 0.0f, 1.0f);
		m_ProgressBar_Speed->SetPercent(ClampedPercent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player_Speed_Bar is null!"));
	}

}

void UU_WBP_HUD::Update_Text_HP(float hp, float hp_max)
{
	if (UTextBlock* pText = Cast<UTextBlock>(this->GetWidgetFromName(TEXT("Player_HP_Text"))))
	{
		pText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), hp, hp_max)));
	}
}

void UU_WBP_HUD::Update_Text_Speed(float speed, float speed_max)
{
	if (UTextBlock* pText = Cast<UTextBlock>(this->GetWidgetFromName(TEXT("Player_Speed_Text"))))
	{
		pText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), speed, speed_max)));
	}
}

void UU_WBP_HUD::Update_Text_Name(FString& name)
{
	if (UTextBlock* pText = Cast<UTextBlock>(this->GetWidgetFromName(TEXT("Player_Name_Text"))))
	{
		pText->SetText(FText::FromString(FString::Printf(TEXT("%s "), *name)));
	}
}



void UU_WBP_HUD::InitializeMaterialInstance()
{
	if (m_ProgressBar_Time)
	{
		// 기존 머티리얼을 가져와서 다이나믹 머티리얼 인스턴스 생성
		UObject* ResourceObject = m_ProgressBar_Time->GetBrush().GetResourceObject();
		if (UMaterialInterface* CurrentMaterial = Cast<UMaterialInterface>(ResourceObject))
		{
			m_DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
			if (m_DynamicMaterialInstance)
			{
				// 이미지 위젯에 다이나믹 머티리얼 인스턴스 적용
				m_ProgressBar_Time->SetBrushFromMaterial(m_DynamicMaterialInstance);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ProgressBar_Time does not have a valid MaterialInterface!"));
		}
	}
}

void UU_WBP_HUD::PlayTextBlink(const FName& TextBlockName, const FName& AnimationName)
{

	if (UTextBlock* pText = Cast<UTextBlock>(GetWidgetFromName(TextBlockName)))
	{
		if (UWidgetAnimation* pAnimation = FindAnimationByName(AnimationName))
		{
			PlayAnimation(pAnimation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Animation '%s' not found"), *AnimationName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TextBlock '%s' not found"), *TextBlockName.ToString());
	}
	
}

void UU_WBP_HUD::Set_TextBox_Text(const FName& TextBlockName, const FString& Text_value)
{

	if (UWidget* FoundWidget = GetWidgetFromName(TextBlockName))
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(FoundWidget))
		{
			TextBlock->SetText(FText::FromString(Text_value));
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Set_TextBox_Text: Widget is not a TextBlock or EditableTextBox."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Set_TextBox_Text: Could not find widget with name %s"), *TextBlockName.ToString());
	}
}

void UU_WBP_HUD::Update_TextBox_Level()
{
	ASparta_GameState* pSparta_GameState = GetWorld() ? GetWorld()->GetGameState<ASparta_GameState>() : nullptr;
	if (pSparta_GameState)
	{
		Set_TextBox_Text("Notice_Text_2", "");
		int32 i_level = pSparta_GameState->Get_Level() + 1;
		FString NewWave		  = FString::Printf(TEXT("Wave %d Start"), i_level);
		Set_TextBox_Text("Notice_Text_1",NewWave);


		//const TCHAR* FormatString = TEXT("Wave %d 시작");
		//FString NewWave = FString::Printf(FormatString, i_level);

	

		PlayTextBlink("Notice_Text_1", "m_Twinkle_01");
		PlayTextBlink("Notice_Text_2", "m_Twinkle_02");


		
	}
}

UWidgetAnimation* UU_WBP_HUD::FindAnimationByName(FName AnimationName)
{
	for (TFieldIterator<FProperty> It(GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		FProperty* Property = *It;

		if (FObjectPropertyBase* ObjProp = CastField<FObjectPropertyBase>(Property))
		{
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass() &&
				Property->GetFName() == AnimationName)
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				return Cast<UWidgetAnimation>(Obj);
			}
		}
	}
	return nullptr;
}







// Player HP Bar 관련 함수들 구현
void UU_WBP_HUD::Set_PlayerHP_Percent(float HPPercent)
{
	if (m_ProgressBar_HP)
	{
		// 퍼센트 값을 0.0 ~ 1.0 범위로 클램프
		float ClampedPercent = FMath::Clamp(HPPercent, 0.0f, 1.0f);
		m_ProgressBar_HP->SetPercent(ClampedPercent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player_HP_Bar is null!"));
	}
}

float UU_WBP_HUD::Get_PlayerHP_Percent() const
{
	if (m_ProgressBar_HP)
	{
		return m_ProgressBar_HP->GetPercent();
	}
	return 0.0f;
}