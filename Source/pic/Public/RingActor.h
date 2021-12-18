// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RingActor.generated.h"

UCLASS()
class PIC_API ARingActor : public AActor
{
	GENERATED_BODY()
protected:	
	/** Scale */
	FVector NewScale3D = FVector(1.f);
	/** Mesh */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	/** Collision & Overlap */
	UPROPERTY(EditAnywhere, Category = Collision)
	UBoxComponent* RingCollisionComp;
	TArray<UBoxComponent*> MyCollisionSet;
	bool IsDamageApplied = false;	
	bool IsStabAttack = false;//判断是否由BossStabAttack产生
public:
	/** Collision & Overlap */
	UFUNCTION()
	void OverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	FORCEINLINE bool GetIsStabAttack() const { return IsStabAttack; }
	FORCEINLINE void SetIsStabAttack(bool value) { IsStabAttack = value; }
	/** Destroy */
	void RingDestroy();

protected:
	virtual void BeginPlay() override;	
public:	
	ARingActor();
	virtual void Tick(float DeltaTime) override;
};
