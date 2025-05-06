// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	PlayerDetectorSphere->SetupAttachment(RootComponent);

	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HPText"));
	HPText->SetupAttachment(RootComponent);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);

	// ��������ֵ
	UpdateHP(HitPoints);

	// �����������ӵ�ί��
	OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::OnAttackOverrideAnimEnd);

	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(0);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && FollowTarget&&!IsStunned)
	{
		float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0 ? 1 : -1;
		UpdateDirection(MoveDirection);
		if(ShouldMoveToTarget())// �жϾ����Ƿ�ӽ�
		{
			if (CanMove)
			{
				FVector WorldDirection = FVector(1, 0, 0);
				AddMovementInput(WorldDirection, MoveDirection);
			}
		}
		else // �������
		{
			if (FollowTarget->IsAlive)
			{
				Attack();
			}
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

// ���ø���
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

	// ���������ı�
	FString Str=FString::Printf(TEXT("HP:%d"), HitPoints);
	HPText->SetText(FText::FromString(Str));
}

void AEnemy::TakeDamage(int DamageAmount, float StunDuration)
{
	if (!IsAlive)return;

	UpdateHP(HitPoints - DamageAmount);

	if (HitPoints <= 0)
	{
		// ��������
		UpdateHP(0);
		HPText->SetHiddenInGame(true);

		IsAlive = 0;
		CanMove = 0;
		CanAttack = 0;

		// ������������
		GetAnimInstance()->JumpToNode(FName("JumpDie"),FName("EnemyStateMachine"));

		EnableAttackCollisionBox(0);
		
	}
	else
	{
		// �����ܻ�����
		GetAnimInstance()->JumpToNode(FName("JumpDTakeHit"), FName("EnemyStateMachine"));
		Stun(StunDuration);
	}
}

void AEnemy::Stun(float DurationInSeconds)
{
	IsStunned = 1;

	bool IsTimerAlreadyActive=GetWorldTimerManager().IsTimerActive(StunTimer);
	// ����Ѿ����ڣ���ռ�ʱ��
	if (IsTimerAlreadyActive)
	{
		GetWorldTimerManager().ClearTimer(StunTimer);
	}

	// ���ö�ʱ��
	GetWorldTimerManager().SetTimer(StunTimer, this, &AEnemy::OnStaunTimeout, 1, false, DurationInSeconds);
	
	// ֹͣ���ж�������
	GetAnimInstance()->StopAllAnimationOverrides();
}

void AEnemy::OnStaunTimeout()
{
	IsStunned = 0;
}

void AEnemy::Attack()
{
	if (IsAlive && CanAttack && !IsStunned)
	{
		CanAttack = 0;
		CanMove = 0;

		// ���Ź�������
		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("DefaultSlot"),
			1, 0, OnAttackOverrideEndDelegate);

		// ����ȴ��ʱ��
		GetWorldTimerManager().SetTimer(AttackCooldownTimer, this,
			&AEnemy::OnAttackCooldownTimerTimeout, 1, false, AttackCooldownInSeconds);
	}
}

void AEnemy::OnAttackOverrideAnimEnd(bool Completed)
{
	if (IsAlive)
	{
		CanMove = 1;
	}
}

void AEnemy::OnAttackCooldownTimerTimeout()
{
	if (IsAlive)// ��ʱ����ȴ������ٴι���
	{
		CanAttack = 1;
	}
}

void AEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndedx, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		Player->TakeDamage(AttackDamage, AttackStunDuration);

		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::White, TEXT("Player Take Damage"));

	}
}

void AEnemy::EnableAttackCollisionBox(bool Enabled)
{
	if (Enabled)
	{
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// ������ײ��Ӧ
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
			ECollisionResponse::ECR_Overlap);
	}
	else
	{
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// ������ײ��Ӧ
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
			ECollisionResponse::ECR_Ignore);
	}
}