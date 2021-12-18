// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnermyBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UEnermyBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UEnermyBTTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
