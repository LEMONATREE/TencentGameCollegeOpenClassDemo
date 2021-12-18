// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"
UCLASS()
class PIC_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	float Direction; 
	UPROPERTY(BlueprintReadOnly)
	bool bInAir;
	virtual void UpdateAnimInfo(float dt) {};
};
