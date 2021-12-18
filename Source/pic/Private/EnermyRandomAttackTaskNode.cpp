// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyRandomAttackTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
//RandomAttack
UEnermyRandomAttackTaskNode::UEnermyRandomAttackTaskNode()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}
EBTNodeResult::Type UEnermyRandomAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner()); // 获得行为树的所属control
	if (AIController != nullptr)
	{
		FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerLocation")); // 获得黑板上的数据
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn()); // 获取所控制的AI
		if (AI != nullptr)
		{
			//AI朝向Player进行攻击
			FVector AILocation = AI->GetActorLocation();
			FRotator AIRotation = AI->GetLookAtRotation(AILocation, PlayerLocation);
			AI->SetActorRotation(AIRotation);
			float AnimTime = AI->RandomAttack();
			if (AnimTime != 0.0f)
			{
				FTimerDelegate Del;
				Del.BindUFunction(this, FName(TEXT("TaskFinish")), &OwnerComp);
				GetWorld()->GetTimerManager().SetTimer(TaskFinish_Handle, Del, AnimTime, false, AnimTime);
				return EBTNodeResult::InProgress;
			}
			//没有成功执行CastSkill时返回InProgress会出错
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
void UEnermyRandomAttackTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
//do something
}
void UEnermyRandomAttackTaskNode::TaskFinish(UBehaviorTreeComponent* OwnerComp)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackTask Finish!!!"));
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}