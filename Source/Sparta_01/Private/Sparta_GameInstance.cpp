// Fill out your copyright notice in the Description page of Project Settings.


#include "Sparta_GameInstance.h"

USparta_GameInstance::USparta_GameInstance()
{
	m_TotalScore		= 0.f;
	m_CurrentLevelIndex	= 0.f;


}
void USparta_GameInstance::AddToScore(int32 Amount)
{
	m_TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated : %d"), m_TotalScore);
}
