// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFarAttackTaskNode.h"
#include "BossCharacter.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
EBTNodeResult::Type UBossFarAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner()); // 获得行为树的所属control
	if (AIController != nullptr)
	{
		ABossCharacter* Boss = Cast<ABossCharacter>(AIController->GetPawn()); // 获取所控制的AI
		if (Boss && !Boss->GetIsAttack())
		{
			Boss->BossSkill2();
			return EBTNodeResult::Succeeded;	
		}
	}
	return EBTNodeResult::Failed;
}