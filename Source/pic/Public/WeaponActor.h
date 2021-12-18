// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LoadResource.h"
#include "WeaponActor.generated.h"

UCLASS()
class PIC_API AWeaponActor : public AActor, public LoadResource//似乎会菱形继承，TODO:换成Interface
{
	GENERATED_BODY()
protected:	
	/** Mesh */
	FString WeaponMesh_Path;
	UStaticMesh* WeaponMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = mesh)
	UStaticMeshComponent* WeaponMeshComp = nullptr;
	/** Collision & Overlap */
	UPROPERTY(EditAnywhere, Category = Collision)
	UBoxComponent* WeaponCollisionComp;
	/** Damage */
	float BasicDamage = 0.f;
	float DamageCoeff = 0.f;
	/** Weapon Info */
	bool IsPlayer = false;
public:
	/** Mesh */
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() { return WeaponMeshComp; }
	/** Collision & Overlap */
	FORCEINLINE UBoxComponent* GetWeaponCollision() { return WeaponCollisionComp; }
	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
	/** Damage */
	FORCEINLINE float GetBasicDamage() const { return BasicDamage; }
	FORCEINLINE float GetDamageCoeff() const { return DamageCoeff; }
	FORCEINLINE void SetBasicDamage(float value) { BasicDamage = value; }
	FORCEINLINE void SetDamageCoeff(float value) { DamageCoeff = value; }
	/** Weapon Info */
	FORCEINLINE bool GetIsPlayer() const { return IsPlayer; }
	FORCEINLINE void SetIsPlayer(bool value) { IsPlayer = value; }

protected:
	virtual void BeginPlay() override;	
public:	
	AWeaponActor();
};
