// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnermyCharacter.h"
#include "RingActor.h"
#include "BossCharacter.generated.h"

UCLASS()
class PIC_API ABossCharacter : public AEnermyCharacter
{
	GENERATED_BODY()
protected:
	/** Skill */
	bool IsAttack = false;
	bool HaveRing = false;
	ARingActor* Ring;
	int RingClass = 0;
	bool IsStab = false;
	bool IsStabHit = false;
	/** Resource */
	UAnimMontage* StabAttack_Montage;
	FString RingBP_Path;
	FString FireBallBP_Path;
	FString ThunderBallBP_Path;
public:
	/** Skill */
	void BossSkill();
	void BossSkill2();
	FORCEINLINE bool GetIsAttack() const { return IsAttack; }
	FORCEINLINE bool GetHaveRing() const { return HaveRing; }
	FORCEINLINE bool GetIsStab() const { return IsStab; }
	FORCEINLINE bool GetIsStabHit() const { return IsStabHit; }
	FORCEINLINE void SetIsAttack(bool value) { IsAttack = value; }
	FORCEINLINE void SetHaveRing(bool value) { HaveRing = value; }
	FORCEINLINE void SetIsStab(bool value) { IsStab = value; }
	FORCEINLINE void SetIsStabHit(bool value) { IsStabHit = value; }
	/** Resource */
	FORCEINLINE UAnimMontage* GetStabAttack_Montage() const { return StabAttack_Montage; }
	/** Weapon */
	void HideWeapon();
	void DelayHideWeapon();
protected:
	virtual void BeginPlay() override;
public:
	ABossCharacter();
	virtual void Tick(float DeltaTime) override;
};
