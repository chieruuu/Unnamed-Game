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
	//球体组件指针
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USphereComponent* PlayerDetectorSphere;

	// 显示敌人血量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextRenderComponent* HPText;

	//追踪玩家
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayerCharacter* FollowTarget;

	//追踪半径
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

	// 是否向目标移动
	bool ShouldMoveToTarget();
	// 更新方向
	void UpdateDirection(float MoveDirection);

	// 更新HP
	void UpdateHP(int NewHP);
	// 受到伤害
	void TakeDamage(int DamageAmount,float StunDuration);
	// 受击后麻痹
	void Stun(float DurationInSeconds);
	// 麻痹计时器调用
	void OnStaunTimeout();
};
