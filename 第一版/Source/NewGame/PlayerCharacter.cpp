// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include"Enemy.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �������ӳ��������    ��������ת��Ϊ��ҿ�����
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// ��ȡ��ǿ���뱾�������ϵͳ
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// ����ί�кͺ���
	OnAttackOverrideEndDelegate.BindUObject(this, &APlayerCharacter::OnAttackOverrideAnimEnd);

	// ����ײ���ϰ���ײ�¼�������ײ��ĺ���
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::AttackBoxOverlapBegin);

	// ��ʼ���ù�����ײ
	EnableAttackCollisionBox(false);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�������붯����    ����������������ת��Ϊ��ǿ�����������ȡ��ǿ�������
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStarted);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnded);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &APlayerCharacter::JumpEnded);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);

	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	float MoveActionValue = Value.Get<float>();

	if (IsAlive && CanMove)
	{
		FVector Direction = FVector(1.0f, 0.0f, 0.0f);//Ĭ�Ϸ���
		AddMovementInput(Direction, MoveActionValue);
		UpdateDirection(MoveActionValue);
	}
}

void APlayerCharacter::UpdateDirection(float MoveDirection)
{
	//��ǰ��ת
	FRotator CurrentRotation= Controller->GetControlRotation();

	if (MoveDirection<0.0f)//��ת
	{
		if (CurrentRotation.Yaw != 180.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
		}
	}
	else if (MoveDirection > 0.0f)
	{
		if (CurrentRotation.Yaw != 0.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
		}
	}
}

void APlayerCharacter::JumpStarted(const FInputActionValue& Value)
{
	if (IsAlive && CanMove)
	{
		Jump();
	}
}
void APlayerCharacter::JumpEnded(const FInputActionValue& Value)
{
	StopJumping();
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (IsAlive && CanAttack)
	{
		CanAttack = 0;
		CanMove = 0;

		//EnableAttackCollisionBox(true);

		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("DefaultSlot"),
			1, 0,OnAttackOverrideEndDelegate);
	}
}

void APlayerCharacter::OnAttackOverrideAnimEnd(bool Completed)
{
	CanAttack = 1;
	CanMove = 1;

	//EnableAttackCollisionBox(false);
}

void APlayerCharacter::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndedx, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy)
	{
		// �Ե�������˺�
		Enemy->TakeDamage(AttackDamage,AttackStunDuration);
		//GEngine->AddOnScreenDebugMessage(-1,10,FColor::White,Enemy->GetName());
	}
}

void APlayerCharacter::EnableAttackCollisionBox(bool Enabled)
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