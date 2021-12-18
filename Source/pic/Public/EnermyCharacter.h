// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "BaseCharacter.h"
#include "WeaponActor.h"
#include "CharacterSkill.h"
#include "BaseCharacterInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnermyCharacter.generated.h"

UENUM(BlueprintType)
enum class EAttackDistanceType:uint8
{
	Close,
	Short,
	Middle,
	Long,
	Far
};
UCLASS()
class PIC_API AEnermyCharacter : public ABaseCharacter,public CharacterSkill,public IBaseCharacterInterface
{
	GENERATED_BODY()
protected:
	/** Enermy Info */
	int EnermyClass;
	/** Skill */
	bool BeginThrow = false;
	void InitialThrow();
	void Throw(float deltaT);
	void FarAttack();
	FVector WeaponLocation;
	FVector WeaponSpeed;
	FVector WeaponA;
	FQuat WeaponRotationQuat= FQuat(FRotator(0.f, 0.f, 20.f));
	float ThrowTime = 2.5f;
	float ThrowTime_update;
	bool IsJumpBack = false;
	/** Overlap */
	UFUNCTION(BluePrintCallable)
	void OnOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	/** Sense */
	UPROPERTY(EditAnywhere, Category = Sense)
	UPawnSensingComponent* Sight;
	UFUNCTION()//Add to interact with blueprint
	void HaveSightOfPlayer(APawn* Pawn);
	/** BehaviorTree */
	FString BehaviorTree_Path;
	UBehaviorTree* BehaviorTree;
	/** Enermy Info */
	FORCEINLINE int GetEnermyClass() const { return EnermyClass; }
	FORCEINLINE void SetEnermyClass(int value) { EnermyClass = value; }
	/** Skill */
	FORCEINLINE bool GetIsJumpBack() const { return IsJumpBack; }
	FORCEINLINE void SetIsJumpBack(bool value) { IsJumpBack = value; }
	float RandomAttack();
	/** AI */
	void StopBehaviorTree();
protected:
	virtual void BeginPlay() override;
public:	
	AEnermyCharacter();	
	virtual void Tick(float DeltaTime) override;
};
