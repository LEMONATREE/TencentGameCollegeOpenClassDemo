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
public:
	/** Collision & Overlap */
	UFUNCTION()
	void OverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** Destroy */
	void RingDestroy();

protected:
	virtual void BeginPlay() override;	
public:	
	ARingActor();
	virtual void Tick(float DeltaTime) override;
};
