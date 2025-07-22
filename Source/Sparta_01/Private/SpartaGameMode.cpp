// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameMode.h"
#include "Sparta_Character.h"
#include "Sparta_PlayerController.h"
#include "Sparta_GameState.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASparta_Character::StaticClass();

	PlayerControllerClass = ASparta_PlayerController::StaticClass();

	GameStateClass = ASparta_GameState::StaticClass();


}
