// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include"Components/BoxComponent.h"
#include"PaperFlipbookComponent.h"

#include"Sound/SoundBase.h"
#include"Engine/TimerHandle.h"

#include "LevelExit.generated.h"

UCLASS()
class NEWGAME_API ALevelExit : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* DoorFlipbook;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundBase* PlayerEnterSound;

	FTimerHandle WaitTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeInSecond = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsActive=1;

	// Sets default values for this actor's properties
	ALevelExit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndedx, bool bFromSweep, const FHitResult& SweepResult);

	void OnWaitTimerTimeout();
};
