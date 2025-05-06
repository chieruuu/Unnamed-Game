// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameInstance.h"

#include"Kismet/GameplayStatics.h"

void UNewGameInstance::SetPlayerHP(int NewHP)
{
	PlayerHP = NewHP;
}

void UNewGameInstance::AddDiamond(int Amount)
{
	CollectedDiamondCount += Amount;
}

void UNewGameInstance::ChangeLevel(int LevelIndex)
{
	if (LevelIndex <= 0)return;

	CurrentLevelIndex = LevelIndex;

	FString LevelNameString = FString::Printf(TEXT("Level_%d"), LevelIndex);
	UGameplayStatics::OpenLevel(GetWorld(),FName(LevelNameString));
}

void UNewGameInstance::RestartGame()
{
	PlayerHP = 100;
	CollectedDiamondCount = 0;
	IsDoubleJumpUnlock = 0;

	CurrentLevelIndex = 1;
	ChangeLevel(CurrentLevelIndex);
}
