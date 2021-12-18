// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
#include "EnermyAttackDistanceTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UEnermyAttackDistanceTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackDistanceTypeKey;
	//UPROPERTY(EditAnywhere)
	//FBlackboardKeySelector Target;
	//UPROPERTY(EditAnywhere)
	//FBlackboardKeySelector LongDistance;
	//UPROPERTY(EditAnywhere)
	//FBlackboardKeySelector MiddleDistance;
	//UPROPERTY(EditAnywhere)
	//FBlackboardKeySelector ShortDistance;
	//UPROPERTY(EditAnywhere)
	//FBlackboardKeySelector CloseDistance;

	EAttackDistanceType AttackDistanceType = EAttackDistanceType::Far;
};
