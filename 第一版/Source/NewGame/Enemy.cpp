// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	PlayerDetectorSphere->SetupAttachment(RootComponent);

	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HPText"));
	HPText->SetupAttachment(RootComponent);

	// 更新生命值
	UpdateHP(HitPoints);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);


}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && FollowTarget&&!IsStunned)
	{
		float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0 ? 1 : -1;
		UpdateDirection(MoveDirection);
		if(ShouldMoveToTarget())// 判断距离是否接近
		{
			if (CanMove)
			{
				FVector WorldDirection = FVector(1, 0, 0);
				AddMovementInput(WorldDirection, MoveDirection);
			}
		}
		else
		{


		}
	}
}

void AEnemy::UpdateDirection(float MoveDirection)
{
	FRotator CurrentRotation = GetActorRotation();

	if (MoveDirection < 0)
	{
		if (CurrentRotation.Yaw != 180)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 180, CurrentRotation.Roll));
		}
	}
	else if (MoveDirection > 0)
	{
		if (CurrentRotation.Yaw != 0)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 0, CurrentRotation.Roll));
		}
	}
}

bool AEnemy::ShouldMoveToTarget()
{
	bool Result=0;

	if (FollowTarget)
	{
		float DisToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
		Result=DisToTarget > StopDistanceToTaget;
	}

	return Result;
}

// 设置跟随
void AEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndedx, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		FollowTarget = Player;
	}
}


void AEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndedx)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		FollowTarget = NULL;
	}
}


void AEnemy::UpdateHP(int NewHP)
{
	HitPoints = NewHP;

	// 更新生命文本
	FString Str=FString::Printf(TEXT("HP:%d"), HitPoints);
	HPText->SetText(FText::FromString(Str));
}

void AEnemy::TakeDamage(int DamageAmount, float StunDuration)
{
	if (!IsAlive)return;

	UpdateHP(HitPoints - DamageAmount);

	if (HitPoints <= 0)
	{
		// 敌人死亡
		UpdateHP(0);
		HPText->SetHiddenInGame(true);

		IsAlive = 0;
		CanMove = 0;

		// 播放死亡动画
		GetAnimInstance()->JumpToNode(FName("JumpDie"),FName("EnemyStateMachine"));
		
	}
	else
	{
		// 播放受击动画
		GetAnimInstance()->JumpToNode(FName("JumpDTakeHit"), FName("EnemyStateMachine"));
		Stun(StunDuration);
	}
}

void AEnemy::Stun(float DurationInSeconds)
{
	IsStunned = 1;

	bool IsTimerAlreadyActive=GetWorldTimerManager().IsTimerActive(StunTimer);
	// 如果已经存在，清空计时器
	if (IsTimerAlreadyActive)
	{
		GetWorldTimerManager().ClearTimer(StunTimer);
	}

	// 设置定时器
	GetWorldTimerManager().SetTimer(StunTimer, this, &AEnemy::OnStaunTimeout, 1, false, DurationInSeconds);
	
	// 停止所有动画覆盖
	GetAnimInstance()->StopAllAnimationOverrides();
}

void AEnemy::OnStaunTimeout()
{
	IsStunned = 0;
}