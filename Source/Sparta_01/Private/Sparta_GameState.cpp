// Fill out your copyright notice in the Description page of Project Settings.


#include "Sparta_GameState.h"
#include "Sparta_GameInstance.h"
#include "Sparta_Character.h"
#include "Sparta_PlayerController.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "U_WBP_HUD.h"
#include "Kismet/GamePlayStatics.h"
// Fill out your copyright notice in the Description page of Project Settings.



ASparta_GameState::ASparta_GameState()
{
	m_Score = 0.f;
	m_SpawnedCoinCount = 0;
	m_CollectedCoinCount = 0;
	m_LevelDuration = 20.f;
	m_CurrentLevelIndex = 0; 
	m_MaxLevels = 3;
	m_MaxWaves = 3;
	m_WaveDuration = 20.f;
	m_ItemToSpawnPerWave  ={20, 30, 40};
}


void ASparta_GameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(m_HUDUpdate_TimerHandle,
		this,
		&ASparta_GameState::Update_HUD,
		0.1f,
		true);
}


int32 ASparta_GameState::Get_Level() const
{
	return m_CurrentLevelIndex;
}

int32 ASparta_GameState::Get_Score() const
{
	return m_Score;
}

void ASparta_GameState::Set_Score(int32 Amount)
{
	m_Score = Amount;
}

void ASparta_GameState::Add_Score(int32 Amount)
{
	if (UGameInstance* pGameInstance = GetGameInstance())
	{
		USparta_GameInstance* pSparata_GameInstance = Cast<USparta_GameInstance>(pGameInstance);
		if (pSparata_GameInstance)
		{
			pSparata_GameInstance->AddToScore(Amount);
		UE_LOG(LogTemp, Warning, TEXT("Score : %d"), m_Score);
		}
	}
	//m_Score += Amount;
}

void ASparta_GameState::OnCoinCollected()
{
	m_CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		m_CollectedCoinCount,
		m_SpawnedCoinCount);

	if (m_SpawnedCoinCount > 0 && m_CollectedCoinCount >= m_SpawnedCoinCount)
	{
		EndLevel();
	}
	//Add_Score()
}

void ASparta_GameState::StartLevel()
{
	if (APlayerController* pPLayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASparta_PlayerController* pSparta_PlayerController = Cast<ASparta_PlayerController>(pPLayerController))
		{
			pSparta_PlayerController->Show_GameHUD();
		}
	}


	if (UGameInstance* pGameInstance = GetGameInstance())
	{
		USparta_GameInstance* pSparata_GameInstance = Cast<USparta_GameInstance>(pGameInstance);
		if (pSparata_GameInstance)
		{
			m_CurrentLevelIndex = pSparata_GameInstance->m_CurrentLevelIndex;
		}
	}



	// TArray<AActor*> FoundVolumes;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(),FoundVolumes);
	//
	// const int32 I_ItemtoSpawn = 40;
	//
	// for (int32 i = 0; i < I_ItemtoSpawn; i++)
	// {
	// 	if (FoundVolumes.Num() > 0)
	// 	{
	//
	// 		ASpawnVolume* pSpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
	// 		if (pSpawnVolume)
	// 		{
	// 			AActor* SpawnedActor = pSpawnVolume->SpawnRandomItem();
	// 			if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
	// 			{
	// 				m_SpawnedCoinCount++;
	// 			}
	// 		}
	// 	}
	// }


	GetWorldTimerManager().SetTimer(
		m_LevelTimerHandle,
		this,
		&ASparta_GameState::OnLevelTimeUp,
		m_LevelDuration,
		false
	);

	m_CurrentWaveIndex = 0;
	StartWave();

}

void ASparta_GameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASparta_GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(m_LevelTimerHandle);
	m_CurrentLevelIndex++;


	if (UGameInstance* pGameInstance = GetGameInstance())
	{
		USparta_GameInstance* pSparata_GameInstance = Cast<USparta_GameInstance>(pGameInstance);
		if (pSparata_GameInstance)
		{
			Add_Score(m_Score);
			//m_CurrentLevelIndex++;
			pSparata_GameInstance->m_CurrentLevelIndex = m_CurrentLevelIndex;
		}
	}

	if (m_CurrentLevelIndex >= m_MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (m_LevelMapNames.IsValidIndex(m_CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), m_LevelMapNames[m_CurrentLevelIndex]);
		
	}
	else
	{
		OnGameOver();
	}

}

void ASparta_GameState::Update_HUD()
{
	if (APlayerController* pPlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASparta_PlayerController* pSprataPlayerController = Cast<ASparta_PlayerController>(pPlayerController))
		{
			if (UUserWidget* pHUDWidget = pSprataPlayerController->GetHUDWidget())
			{
				if (UTextBlock* pTimeText = Cast<UTextBlock>(pHUDWidget->GetWidgetFromName(TEXT("Time_Box"))))
				{
					float fRemainTime = GetWorldTimerManager().GetTimerRemaining(m_LevelTimerHandle);
					pTimeText->SetText(FText::FromString(FString::Printf(TEXT("Time :%.1f"), fRemainTime)));

					

				}
				Update_HUD_ProgressBar_Time(pHUDWidget);
				Update_HUD_ProgressBar_HP(pHUDWidget, pSprataPlayerController);
				Update_HUD_ProgressBar_Speed(pHUDWidget, pSprataPlayerController);

				if (UTextBlock* pScoreText = Cast<UTextBlock>(pHUDWidget->GetWidgetFromName(TEXT("Score_Box"))))
				{
					if (UGameInstance* pGameInstance = GetGameInstance())
					{
						USparta_GameInstance* pSpartaGameInstance = Cast<USparta_GameInstance>(pGameInstance);
						if (pSpartaGameInstance)
						{
							pScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score :%d"), pSpartaGameInstance->m_TotalScore)));

						}
					}
				}
				if (UTextBlock* pLevelText = Cast<UTextBlock>(pHUDWidget->GetWidgetFromName(TEXT("Level_Box"))))
				{
					pLevelText->SetText(FText::FromString(FString::Printf(TEXT("Level :%d"), m_CurrentLevelIndex + 1)));
					Update_HUD_Notice(pHUDWidget);
				}
			}
		}
	}
}

void ASparta_GameState::StartWave()
{
	OnWaveStarted.Broadcast(m_CurrentLevelIndex);

	m_SpawnedCoinCount =0;
	m_CollectedCoinCount =0;

	for (AActor* Item : m_WaveItem)
	{
		if (Item && Item ->IsValidLowLevelFast())
		{
			Item->Destroy();
		}
	}
	m_WaveItem.Empty();

	int32 ItemToSpawn = (m_ItemToSpawnPerWave.IsValidIndex(m_CurrentWaveIndex)) ? m_ItemToSpawnPerWave[m_CurrentWaveIndex] : 20;

	if (ASpawnVolume* SpawnVolume = GetSpawnVolume())
	{
		for (int32 i = 0; i < ItemToSpawn; i++)
		{
			if (AActor* SpawnedActor = SpawnVolume->SpawnRandomItem())
			{
				if (SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					m_SpawnedCoinCount++;
				}

				m_WaveItem.Add(SpawnedActor);
			}
		}
	}



	GetWorldTimerManager().SetTimer(
	m_WaveTimerHandle,
	this,
	&ThisClass::OnWaveTimeUp,
	m_WaveDuration,
	false
);
	
}

void ASparta_GameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(m_WaveTimerHandle);

	
}

void ASparta_GameState::OnWaveTimeUp()
{
	EndWave();

	++m_CurrentWaveIndex;

	if (m_CurrentWaveIndex >= m_MaxWaves)
	{
		EndLevel();
	}
	else
	{
		StartWave();
	}
	
}

void ASparta_GameState::Update_HUD_ProgressBar_Time(UUserWidget* pHUDWidget)
{

	// UU_WBP_HUD�� ĳ�����Ͽ� SetProgressPercentage �Լ��� ����
	if (UU_WBP_HUD* pCustomHUD = Cast<UU_WBP_HUD>(pHUDWidget))
	{
		// ����� ��� �� ����
		float fRemainTime = GetWorldTimerManager().GetTimerRemaining(m_LevelTimerHandle);

		float fProgress = (fRemainTime / m_LevelDuration); // 0.0 ~ 1.0

		// ������� ��Ƽ���� �Ķ���Ϳ� ����
		pCustomHUD->Set_ProgressBar_Percentage(fProgress);
	}


	
}

void ASparta_GameState::Update_HUD_ProgressBar_HP(UUserWidget* pHUDWidget,ASparta_PlayerController* pPlayerController)
{
	if (UU_WBP_HUD* pCustomHUD = Cast<UU_WBP_HUD>(pHUDWidget))
	{
		if (pPlayerController )
		{
			if (APawn* PlayerPawn = pPlayerController->GetPawn())
			{
				ASparta_Character* pPlayer = Cast<ASparta_Character>(PlayerPawn);

			
				float f_HP = pPlayer->Get_Health_per();
				// �Ǵ� ���� �ۼ�Ʈ�� ����
				pCustomHUD->Set_PlayerHP_Percent(f_HP); // 75% HP
				pCustomHUD->Update_Text_HP(pPlayer->Get_Health(), pPlayer->Get_Health_Max()); // 75% HP\
												
			}
		}


	}

}

void ASparta_GameState::Update_HUD_ProgressBar_Speed(UUserWidget* pHUDWidget, ASparta_PlayerController* pPlayerController)
{
	if (UU_WBP_HUD* pCustomHUD = Cast<UU_WBP_HUD>(pHUDWidget))
	{
		if (pPlayerController)
		{
			if (APawn* PlayerPawn = pPlayerController->GetPawn())
			{
				ASparta_Character* pPlayer = Cast<ASparta_Character>(PlayerPawn);


				float f_Speed = pPlayer->Get_Speed_per();
				// �Ǵ� ���� �ۼ�Ʈ�� ����
				pCustomHUD->Set_ProgressBar_Percentage_Speed(f_Speed); // 75% HP
				pCustomHUD->Update_Text_Speed(pPlayer->Get_Speed(), 2000.f); // 75% HP\
								
			}
		}


	}

}

void ASparta_GameState::Update_HUD_Notice(UUserWidget* pHUDWidget)
{
	if (UU_WBP_HUD* pCustomHUD = Cast<UU_WBP_HUD>(pHUDWidget))
	{
		//pLevelText->SetText(FText::FromString(FString::Printf(TEXT("Level :%d"), m_CurrentLevelIndex + 1)));

		int32 i_level = m_CurrentLevelIndex + 1;
		FString NewWave = FString::Printf(TEXT("Wave %d Start"), i_level);

		pCustomHUD->Set_TextBox_Text("Notice_Text_1", NewWave); // 75% HP\
												
	}

}



ASpawnVolume* ASparta_GameState::GetSpawnVolume() const
{
	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), SpawnVolumes);
	return (SpawnVolumes.Num() > 0) ? Cast<ASpawnVolume>(SpawnVolumes[0]) : nullptr;
}

void ASparta_GameState::OnGameOver()
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
