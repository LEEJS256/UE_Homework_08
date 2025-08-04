// Fill out your copyright notice in the Description page of Project Settings.


#include "Sparta_PlayerController.h"
#include "Sparta_GameState.h"
#include "Sparta_GameInstance.h"
#include "Kismet/GamePlayStatics.h"
#include "EnhancedInputSubSystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

ASparta_PlayerController::ASparta_PlayerController()
	: m_InputMappingContext(nullptr),
	  m_MoveAction(nullptr),
	  m_JumpAction(nullptr),
	  m_LookAction(nullptr),
	  m_PauseAction(nullptr),
	  m_ActiveAction(nullptr),
	  m_SprintAction(nullptr),
	  m_HUDWidgetInstance(nullptr),
	  m_HUDWidgetClass(nullptr),
	  m_MainMenu_WidgetClass(nullptr),
	  m_MainMenu_WidgetInstance(nullptr)
{
}

void ASparta_PlayerController::Show_GameHUD()
{
	if (m_HUDWidgetInstance)
	{
		m_HUDWidgetInstance->RemoveFromParent();
		m_HUDWidgetInstance = nullptr;
	}

	if (m_MainMenu_WidgetInstance)
	{
		m_MainMenu_WidgetInstance->RemoveFromParent();
		m_MainMenu_WidgetInstance = nullptr;
	}

	if (m_HUDWidgetClass)
	{
		m_HUDWidgetInstance = CreateWidget<UUserWidget>(this, m_HUDWidgetClass);
		if (m_HUDWidgetInstance)
		{
			m_HUDWidgetInstance->AddToViewport();
			bShowMouseCursor = false; //ak
			SetInputMode(FInputModeGameOnly());
		}

		ASparta_GameState* pSparta_GameState = GetWorld() ? GetWorld()->GetGameState<ASparta_GameState>() : nullptr;
		if (pSparta_GameState)
		{
			pSparta_GameState->Update_HUD();
		}
	}
}

void ASparta_PlayerController::TurnOff_GameHUD()
{
	if (m_HUDWidgetInstance)
	{
		m_HUDWidgetInstance->RemoveFromParent();
		m_HUDWidgetInstance = nullptr;
	}

	if (m_MainMenu_WidgetInstance)
	{
		m_MainMenu_WidgetInstance->RemoveFromParent();
		m_MainMenu_WidgetInstance = nullptr;
	}


	bShowMouseCursor = false;
    
	// 입력 모드를 게임 전용으로 설정합니다.
	SetInputMode(FInputModeGameOnly());

	// 게임 일시정지를 해제해야 게임이 다시 움직입니다.
	SetPause(false);
	Show_GameHUD();
}


void ASparta_PlayerController::Show_MainMenu(bool bIsReStart)
{
	if (m_HUDWidgetInstance)
	{
		m_HUDWidgetInstance->RemoveFromParent();
		m_HUDWidgetInstance = nullptr;
	}

	if (m_MainMenu_WidgetInstance)
	{
		m_MainMenu_WidgetInstance->RemoveFromParent();
		m_MainMenu_WidgetInstance = nullptr;
	}

	if (m_MainMenu_WidgetClass)
	{
		m_MainMenu_WidgetInstance = CreateWidget<UUserWidget>(this, m_MainMenu_WidgetClass);
		if (m_MainMenu_WidgetInstance)
		{
			m_MainMenu_WidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* pBtn_Text = Cast<UTextBlock>
			(m_MainMenu_WidgetInstance->GetWidgetFromName(TEXT("Start_Btn_text"))))
		{
			if (bIsReStart)
			{
				pBtn_Text->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				pBtn_Text->SetText(FText::FromString(TEXT("Start")));
			}


			if (bIsReStart) //�ĸ����Ͱ� Ʈ���ϋ�
			{
				UFunction* pPlayAnimFunc = m_MainMenu_WidgetInstance->FindFunction(FName("Play_GameOverAnim"));
				if (pPlayAnimFunc)
				{
					m_MainMenu_WidgetInstance->ProcessEvent(pPlayAnimFunc, nullptr);
				}

				if (UTextBlock* pTotalScoreText = Cast<UTextBlock>(
					m_MainMenu_WidgetInstance->GetWidgetFromName("TotalScore_Text")))
				{
					if (USparta_GameInstance* pSparta_GameInstance = Cast<USparta_GameInstance>(
						UGameplayStatics::GetGameInstance(this)))
					{
						pTotalScoreText->SetText(FText::FromString(
							FString::Printf(TEXT("Total Score :%d"), pSparta_GameInstance->m_TotalScore)));
					}
				}
			}
		}
	}
}

void ASparta_PlayerController::StartGame()
{
	if (USparta_GameInstance* pSparta_GameInstance = Cast<
		USparta_GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		pSparta_GameInstance->m_CurrentLevelIndex = 0;
		pSparta_GameInstance->m_TotalScore = 0;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}

void ASparta_PlayerController::EndGame()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		this,
		EQuitPreference::Quit,
		false
		);
}


void ASparta_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) //�÷��̾� �������� �Լ�
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (m_InputMappingContext)
			{
				SubSystem->AddMappingContext(m_InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("Menu_Level"))
	{
		Show_MainMenu(false);
	}
}

UUserWidget* ASparta_PlayerController::GetHUDWidget() const
{
	return m_HUDWidgetInstance;
}
