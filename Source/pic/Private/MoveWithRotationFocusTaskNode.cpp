// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveWithRotationFocusTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
//Make Enermy move with sight focused on Player
UMoveWithRotationFocusTaskNode::UMoveWithRotationFocusTaskNode()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}
EBTNodeResult::Type UMoveWithRotationFocusTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type ReturnType = Super::ExecuteTask(OwnerComp, NodeMemory);//不确定要不要执行
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn());
		if (AI)
		{
			AI->bUseControllerRotationYaw = false;
			return ReturnType;
		}
	}
	return EBTNodeResult::Failed;
}
void UMoveWithRotationFocusTaskNode::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn());
		if (AI)
		{
			AI->bUseControllerRotationYaw = true;
		}
	}
}
void UMoveWithRotationFocusTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController != nullptr)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn());
		if (AI)
		{
			//AI面朝Player
			FVector AILocation = AI->GetActorLocation();
			FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerLocation"));//移动时保证始终朝向Player
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("PlayerLocation:%f %f %f"), TargetLocation.X,TargetLocation.Y,TargetLocation.Z));
			FRotator Turn_Z = AI->GetLookAtRotation(AILocation, TargetLocation);
			AI->SetActorRotation(Turn_Z);
		}
	}
}