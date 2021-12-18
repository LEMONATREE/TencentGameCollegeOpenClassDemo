// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "EnermyCloseEnoughBTDecorator.generated.h"
/**Abandon*/
UCLASS()
class PIC_API UEnermyCloseEnoughBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere, Category = MinDistance)
	float MinDistance=10.0f;
};
