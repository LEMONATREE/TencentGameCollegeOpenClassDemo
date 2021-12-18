// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyBTTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"

UEnermyBTTaskNode::UEnermyBTTaskNode()
{
	bCreateNodeInstance = true;//每个Enermy产生一个实例
}
EBTNodeResult::Type UEnermyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//距离大于一定阈值时，Enermy就会跑到player身边
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner()); // 获得行为树的所属control
	if (AIController != nullptr)
	{
		FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerLocation")); // 获得黑板上的数据
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn()); // 获取所控制的AI
		if (AI != nullptr) // 让AI移动到targetlocation
		{
			FVector AILocation = AI->GetActorLocation();
			FRotator Turn = UKismetMathLibrary::FindLookAtRotation(AILocation, PlayerLocation);
			FRotator AIRotation(0.f, 0.f, 0.f);
			AIRotation.Yaw = Turn.Yaw;
			AI->SetActorRotation(AIRotation);
			if (AI->GetEnermyClass() == 0)//近程
			{
				AIController->MoveToLocation(PlayerLocation);
			}
			else//远程
			{
				//到达AI->player距离20%的地方
				FVector Target = AILocation - 0.2 * (AILocation - PlayerLocation);
				AIController->MoveToLocation(Target);
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}