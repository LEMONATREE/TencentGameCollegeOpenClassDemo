// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnermyJumpBackTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UEnermyJumpBackTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UEnermyJumpBackTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FVector TargetLocation = FVector(0.f);
	FTimerHandle TaskFinish_Handle;
	UFUNCTION()
	void TaskFinish(UBehaviorTreeComponent* OwnerComp);
};
