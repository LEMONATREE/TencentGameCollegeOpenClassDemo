// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "BlackHoleActor.generated.h"

UCLASS()
class PIC_API ABlackHoleActor : public AActor
{
	GENERATED_BODY()
protected:	
	/** Info */
	FVector Speed = FVector(0.f);
	int BlackHoleClass = -1;
	float DuringTime = 10.f;
	FTimerHandle OutHandle;
	/** Scene */
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComp;
	/** Mesh */
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	/** Collision & Overlap */
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComp;
	bool IsDamageApplied = false;
	/** IntoBossMap */
	bool IntoBossGame = true;
	/** Scale */
	float ScaleSpeed = 0.f;
	FVector NewScale3D = FVector(0.f);
public:
	/** Info */
	FORCEINLINE FVector GetSpeed() const { return Speed; }
	FORCEINLINE int GetBlackHoleClass() const { return BlackHoleClass; }
	FORCEINLINE void SetSpeed(FVector value) { Speed = value; }
	FORCEINLINE void SetBlackHoleClass(int value) { BlackHoleClass = value; }
	/** IntoBossMap */
	FORCEINLINE bool GetIntoBossGame() const { return IntoBossGame; }
	FORCEINLINE void SetIntoBossGame(bool value) { IntoBossGame = value; }
	/** Scale */
	FORCEINLINE float GetScaleSpeed() const { return ScaleSpeed; }
	FORCEINLINE FVector GetNewScale3D() const { return NewScale3D; }
	FORCEINLINE void SetScaleSpeed(int value) { ScaleSpeed = value; }
	FORCEINLINE void SetNewScale3D(FVector value) { NewScale3D = value; }
	/** Collision & Overlap */
	UFUNCTION()
	void OverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** IntoBossMap */
	void AttractEnermy(float DeltaTime);
	void IntoBoss();
	/** BossSkill */
	void CastBlackHole(float DeltaTime);
	/** Scale */
	void InitScale(float a, float b);//用于在spawn的时候修改参数
	void ChangeScale(float DeltaTime);//事件触发ChangeScale
	/** Destroy */
	void BlackHoleDestroy();
protected:
	virtual void BeginPlay() override;
public:	
	ABlackHoleActor();
	virtual void Tick(float DeltaTime) override;

};
