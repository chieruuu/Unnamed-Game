// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include"Components/SphereComponent.h"
#include"Components/TextRenderComponent.h"

#include"PaperZDAnimInstance.h"

#include"Engine/TimerHandle.h"

#include"PlayerCharacter.h"


#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	//�������ָ��
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USphereComponent* PlayerDetectorSphere;

	// ��ʾ����Ѫ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextRenderComponent* HPText;

	//׷�����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayerCharacter* FollowTarget;

	//׷�ٰ뾶
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistanceToTaget = 70;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitPoints = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	FTimerHandle StunTimer;

	AEnemy();

	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent*OverlappedComponent,
		AActor*OtherActor,UPrimitiveComponent*OtherComp,
		int32 OtherBodyIndedx,bool bFromSweep,const FHitResult&SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndedx);

	// �Ƿ���Ŀ���ƶ�
	bool ShouldMoveToTarget();
	// ���·���
	void UpdateDirection(float MoveDirection);

	// ����HP
	void UpdateHP(int NewHP);
	// �ܵ��˺�
	void TakeDamage(int DamageAmount,float StunDuration);
	// �ܻ������
	void Stun(float DurationInSeconds);
	// ��Լ�ʱ������
	void OnStaunTimeout();
};
