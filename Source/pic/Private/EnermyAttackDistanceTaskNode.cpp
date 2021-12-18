// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyAttackDistanceTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
//根据距离确定AttackDistanceType
EBTNodeResult::Type UEnermyAttackDistanceTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController != nullptr)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn()); // 获取所控制的AI
		if (AI != nullptr)
		{
			FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerLocation"));
			FVector AILocation = AI->GetActorLocation();
			const float Distance = FVector::Distance(PlayerLocation, AILocation);//用size也一样
			const float LongDistance= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("LongDistance"));
			const float MiddleDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("MiddleDistance"));
			const float ShortDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("ShortDistance"));
			const float CloseDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("CloseDistance"));
			//GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("CheckDistance: %.3f %.3f %.3f %.3f %.3f"), LongDistance, MiddleDistance, ShortDistance, CloseDistance, Distance));
			if (Distance < CloseDistance)//0-CloseDistance
			{
				AttackDistanceType = EAttackDistanceType::Close;
			}
			else if (Distance < ShortDistance)//Close-Short
			{
				AttackDistanceType = EAttackDistanceType::Short;
			}
			else if (Distance < MiddleDistance)//Short-Middle
			{
				AttackDistanceType = EAttackDistanceType::Middle;
			}
			else if (Distance < LongDistance)//Middle-Long
			{
				AttackDistanceType = EAttackDistanceType::Long;
			}
			else//Long-inf
			{
				AttackDistanceType = EAttackDistanceType::Far;
			}
			//存入blackboard
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("AttackDistanceType"),(uint8)AttackDistanceType);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
