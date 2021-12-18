// Fill out your copyright notice in the Description page of Project Settings.


#include "GetPatrolPointTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
#include "NavigationSystem.h"
//Get reachable patrol location for AI
EBTNodeResult::Type UGetPatrolPointTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController != nullptr)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn()); // 获取所控制的AI
		if (AI != nullptr)
		{
			FVector AILocation = AI->GetActorLocation();
			FNavLocation TargetNavLocation;
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(AIController);
			if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(AILocation, Radius, TargetNavLocation))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("PatrolLocation"), TargetNavLocation.Location);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
