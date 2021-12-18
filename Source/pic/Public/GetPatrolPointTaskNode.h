// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetPatrolPointTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UGetPatrolPointTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//UPROPERTY(EditAnywhere)
	//float PatrolSpeed = 600.f;
	UPROPERTY(EditAnywhere)
	float Radius = 600.f;
};
