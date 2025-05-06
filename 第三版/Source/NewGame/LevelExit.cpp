// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelExit.h"

#include"Kismet/GameplayStatics.h"

#include"PlayerCharacter.h"
#include"NewGameInstance.h"

// Sets default values
ALevelExit::ALevelExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
	DoorFlipbook->SetupAttachment(GetRootComponent());

	DoorFlipbook->SetPlayRate(0);
	DoorFlipbook->SetLooping(0);

}

// Called when the game starts or when spawned
void ALevelExit::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OverlapBegin);

	// 设置播放位置为0
	DoorFlipbook->SetPlaybackPosition(0, false);
	
}

// Called every frame
void ALevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelExit::OverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndedx, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player && (Player->IsAlive))
	{
		if (IsActive)
		{
			Player->Deactivate();

			IsActive = 0;

			DoorFlipbook->SetPlayRate(1);
			DoorFlipbook->PlayFromStart();

			UGameplayStatics::PlaySound2D(GetWorld(), PlayerEnterSound);

			GetWorldTimerManager().SetTimer(WaitTimer, this, 
				&ALevelExit::OnWaitTimerTimeout, 1, false, WaitTimeInSecond);

		}
	}
}

void ALevelExit::OnWaitTimerTimeout()
{
	UNewGameInstance*MyGameInstance=Cast<UNewGameInstance>(GetGameInstance());
	if (MyGameInstance)
	{
		MyGameInstance->ChangeLevel(LevelIndex);
	}
}

