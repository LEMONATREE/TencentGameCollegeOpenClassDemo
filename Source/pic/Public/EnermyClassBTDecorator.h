// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "EnermyClassBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UEnermyClassBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere, Category = EnermyClass)
	int EnermyClass = 0;
	
};
