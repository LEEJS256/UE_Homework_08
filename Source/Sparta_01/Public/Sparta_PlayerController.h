// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Sparta_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction; 


UCLASS()
class SPARTA_01_API ASparta_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASparta_PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* m_InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_PauseAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_ActiveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> m_HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* m_HUDWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	
	//munu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget>	m_MainMenu_WidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget*				m_MainMenu_WidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void Show_GameHUD();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void TurnOff_GameHUD();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void Show_MainMenu(bool bIsReStart =false);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void EndGame();
	
	

protected:
	virtual void BeginPlay() override;



};
