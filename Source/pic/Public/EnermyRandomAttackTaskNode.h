// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
#include "TimerManager.h"
#include "EnermyRandomAttackTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UEnermyRandomAttackTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UEnermyRandomAttackTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FTimerHandle TaskFinish_Handle;
	UFUNCTION()
	void TaskFinish(UBehaviorTreeComponent* OwnerComp);
	
};
