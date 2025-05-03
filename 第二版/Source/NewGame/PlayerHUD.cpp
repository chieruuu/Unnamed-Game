// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::SetHP(int NewHP)
{
	FString Str = FString::Printf(TEXT("HP:%d"), NewHP);
	HPText->SetText(FText::FromString(Str));
}
void UPlayerHUD::SetDiamonds(int Amount)
{
	FString Str = FString::Printf(TEXT("Diamonds:%d"), Amount);
	DiamondsText->SetText(FText::FromString(Str));
}
void UPlayerHUD::SetLevel(int index)
{
	FString Str = FString::Printf(TEXT("Level:%d"), index);
	LevelText->SetText(FText::FromString(Str));
}