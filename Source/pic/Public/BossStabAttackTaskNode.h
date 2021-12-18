// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossStabAttackTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UBossStabAttackTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBossStabAttackTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FTimerHandle TaskFinish_Handle;
	UFUNCTION()
	void TaskFinish(UBehaviorTreeComponent* OwnerComp);
	//FVector TargetLocation = FVector(0.f);
};
