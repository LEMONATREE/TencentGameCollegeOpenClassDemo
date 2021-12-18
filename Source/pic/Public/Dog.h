// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bome.h"
#include "Dog.generated.h"

UCLASS()
class PIC_API ADog : public AActor
{
	GENERATED_BODY()
protected:
	/** Mesh */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* MyMeshcomp;	
	/** Move */
	UPROPERTY(EditAnywhere)
	float Speed = 50.0f;//移动速度
	UFUNCTION(BluePrintCallable)
	virtual void Dogmove(float time);
	/** Spawn */
	UPROPERTY(EditAnywhere)
	float BoomGap = 1.5f;
	UPROPERTY(EditAnywhere)
	int BoomBoomGap = 30;//每30帧
	int Now = 0;//当前帧
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABome> UsefulActorBP;
	UPROPERTY(EditAnywhere)
	float TimeToSpawn = 10.f;
	UFUNCTION(BluePrintCallable)
	virtual float GetBoomTime();
	UFUNCTION()
	void SpawnUsefulActor();
	/** SlowTimeFlow */
	UFUNCTION(BluePrintCallable)
	virtual void BulletTime();

protected:
	virtual void BeginPlay() override;

public:	
	ADog();
	virtual void Tick(float DeltaTime) override;

};
