// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyJumpBackTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
//回跳闪避
UEnermyJumpBackTaskNode::UEnermyJumpBackTaskNode()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}
EBTNodeResult::Type UEnermyJumpBackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner()); // 获得行为树的所属control
	if (AIController != nullptr)
	{
		FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerLocation")); // 获得黑板上的数据
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn()); // 获取所控制的AI
		if (AI != nullptr && AI->GetIsJumpBack() == false)
		{
			FVector AILocation = AI->GetActorLocation();
			FRotator Turn_Z = AI->GetLookAtRotation(PlayerLocation, AILocation);//AI朝向，由于是跳到player身后，因此于当前player朝向AI方向相同
			AI->SetActorRotation(Turn_Z);
			FVector Direction = AI->GetActorForwardVector();//AI朝向
			TargetLocation = AILocation - 800.f * Direction / Direction.Size();//跳跃目的地
			FNavLocation TargetNavLocation;
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(AI);
			auto FindPath = NavSystem->FindPathToLocationSynchronously(GetWorld(), AILocation, TargetLocation);//寻找路径
			float PathCost = FindPath->GetPathCost();
			//GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("PathCost:%f!"),PathCost));
			if (PathCost > 0.f)//找到路径才执行jumpback，防止掉下平台
			{
				float AnimTime = AI->CastSkill(AI, AI->JumpBack_Montage, 1.f);
				if (AnimTime != 0.0f)
				{
					AI->SetIsJumpBack(true);
					FTimerDelegate Del;
					Del.BindUFunction(this, FName(TEXT("TaskFinish")), &OwnerComp);
					GetWorld()->GetTimerManager().SetTimer(TaskFinish_Handle, Del, AnimTime, false, AnimTime);
					return EBTNodeResult::InProgress;
				}
			}
			return EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}
void UEnermyJumpBackTaskNode::TaskFinish(UBehaviorTreeComponent* OwnerComp)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp->GetAIOwner());
	if (AIController)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn());
		if (AI)
		{
			AI->SetIsJumpBack(false);
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
void UEnermyJumpBackTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController != nullptr)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn());
		if (AI && AI->GetIsJumpBack() == true && TargetLocation.Size()<100000.f)//似乎初始化无效？判断Target是否初始化？可以修改？
		{
			FVector AILocation = AI->GetActorLocation();
			FVector NowTarget = FVector(TargetLocation);
			NowTarget.Z = AILocation.Z;
			FVector NewAILocation = FMath::VInterpTo(AILocation, NowTarget, DeltaSeconds,1.f);
			AI->SetActorLocation(NewAILocation);
		}
		
	}
}

