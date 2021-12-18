// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.h"
#include "picAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
//class PIC_API UpicAnimInstance : public UAnimInstance
class PIC_API UpicAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	//UPROPERTY(BlueprintReadOnly)
	//float Speed; // 移动速度
	//UPROPERTY(BlueprintReadOnly)
	//float Direction; // 方向(-180~180)
	//UPROPERTY(BlueprintReadOnly)
	//bool bInAir; // 是否在空中
	UFUNCTION(BlueprintCallable)
	//virtual void UpdateAnimInfo(float dt) override;
	virtual void UpdateAnimInfo(float dt) override;
};
