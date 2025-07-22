// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "GameFramework/GameState.h"
#include "Sparta_PlayerController.h"
#include "Sparta_GameState.generated.h"
/**
 * 
 */
UCLASS()
class SPARTA_01_API ASparta_GameState : public AGameState
{
	GENERATED_BODY()
public:
	ASparta_GameState();


	virtual void BeginPlay() override;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 m_Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 m_SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 m_CollectedCoinCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float m_LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 m_CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 m_MaxLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> m_LevelMapNames; 

	FTimerHandle m_LevelTimerHandle;
	FTimerHandle m_HUDUpdate_TimerHandle;

public:
	int32 Get_Level() const;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 Get_Score() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void Set_Score(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Score")
	void Add_Score(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void OnCoinCollected();
	void StartLevel();
	void OnLevelTimeUp();
	void EndLevel();
	void Update_HUD();

	void Update_HUD_ProgressBar_Time(UUserWidget* pHUDWidget);
	void Update_HUD_ProgressBar_HP(UUserWidget* pHUDWidget, ASparta_PlayerController* pPlayerController);
	void Update_HUD_ProgressBar_Speed(UUserWidget* pHUDWidget, ASparta_PlayerController* pPlayerController);
	void Update_HUD_Notice(UUserWidget* pHUDWidget);
};
