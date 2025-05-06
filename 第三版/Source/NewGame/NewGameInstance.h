// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include"Sound/SoundBase.h"

#include "NewGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API UNewGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PlayerHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CollectedDiamondCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDoubleJumpUnlock=false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentLevelIndex = 1;



	void SetPlayerHP(int NewHP);
	void AddDiamond(int Amount);

	void ChangeLevel(int LevelIndex);

	UFUNCTION(BlueprintCallable)
	void RestartGame();


};
