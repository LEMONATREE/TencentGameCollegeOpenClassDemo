// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Bome.generated.h"
UCLASS()
class PIC_API ABome : public AActor
{
	GENERATED_BODY()
protected:	
	/** Drop */
	UPROPERTY(EditAnywhere)
	float DropSpeed = 500.0f;
	/** Destroy */
	bool SetDestroy = false;
	float DestroyZ = 250.f;
	/** Mesh */
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MyMeshcomp;
	/** Collision & Overlap */
	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* MySphereComp;
public:
	/** Drop */
	void BomeDrop(float time);
	/** Destroy */
	void BomeDestroy();
	void BomeDelayDestroy();
	/** Collision & Overlap */
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
protected:
	virtual void BeginPlay() override;
public:
	ABome();	
	virtual void Tick(float DeltaTime) override;
};
