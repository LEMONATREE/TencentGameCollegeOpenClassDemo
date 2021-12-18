// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadResource.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "BlackHoleActor.h"
#include "RotationActor.generated.h"

UCLASS()
class PIC_API ARotationActor : public AActor, public LoadResource
{
	GENERATED_BODY()
private:
	/** Base param */
	float FloatSpeed = 20.0f;
	float RotationSpeed = 0.f;
	bool IntoBossGame = true;
	/** Mesh */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp = nullptr;
	/** Collision & Overlap */
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionComp = nullptr;
	/** Move */	
	float MoveTime = 8.f;
	bool MoveDown = false;
	/** Timer */
	FTimerHandle EndHandle;
	FTimerHandle BeginHandle;
public:
	/** Base param */
	FORCEINLINE float GetFloatSpeed() const { return FloatSpeed; }
	FORCEINLINE float GetRotationSpeed() const { return RotationSpeed; }
	FORCEINLINE bool GetIntoBossGame() const { return IntoBossGame; }
	FORCEINLINE void SetFloatSpeed(float value) { FloatSpeed = value; }
	FORCEINLINE void SetRotationSpeed(float value) { RotationSpeed = value; }
	FORCEINLINE void SetIntoBossGame(bool value) { IntoBossGame = value; }
	/** Collision & Overlap */
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	/** Move */
	void MoveTo(FVector TargetLocation, float Time);
	void ThreeStepMove(FVector TargetLocation, float DeltaTime);
	void DelayMove(ARotationActor* tmp, FVector TargetLocation, float LoopTime);
	/** Timer */
	void StopTimerHandle(FTimerHandle &InHandle);
	/** Spawn Blackhole */
	void SpawnBlackHole();
	void DelaySpawnBlackHole(float DelayTime);
	ABlackHoleActor* MyBlackHole = nullptr;
protected:
	virtual void BeginPlay() override;
public:	
	ARotationActor();
	virtual void Tick(float DeltaTime) override;

	void SetViewOnBlackHole();
	void DelaySetViewOnBlackHole();
	FTimerHandle ViewHandle;
};
