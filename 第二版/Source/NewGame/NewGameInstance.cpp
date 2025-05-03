// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameInstance.h"

void UNewGameInstance::SetPlayerHP(int NewHP)
{
	PlayerHP = NewHP;
}

void UNewGameInstance::AddDiamond(int Amount)
{
	CollectedDiamondCount += Amount;
}