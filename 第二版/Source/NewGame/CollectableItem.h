// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include"Components/CapsuleComponent.h"
#include"PaperFlipbookComponent.h"
#include"PaperFlipbook.h"

#include "CollectableItem.generated.h"

UENUM(BlueprintType)
enum class CollectableType :uint8
{
	Diamond,
	Heart,
	DoubleJumpUpgrade
};


UCLASS()
class NEWGAME_API ACollectableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* ItemFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	CollectableType Type;

	// Sets default values for this actor's properties
	ACollectableItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndedx, bool bFromSweep, const FHitResult& SweepResult);

};
