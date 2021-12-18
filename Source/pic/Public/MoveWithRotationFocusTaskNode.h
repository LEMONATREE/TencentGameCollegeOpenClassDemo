// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "MoveWithRotationFocusTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UMoveWithRotationFocusTaskNode : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	UMoveWithRotationFocusTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

};
