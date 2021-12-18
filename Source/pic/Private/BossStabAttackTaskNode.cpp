// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStabAttackTaskNode.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
#include "BossCharacter.h"

UBossStabAttackTaskNode::UBossStabAttackTaskNode()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}
EBTNodeResult::Type UBossStabAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("StabTask!!!"));
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner()); // 获得行为树的所属control
	if (AIController != nullptr)
	{
		ABossCharacter* Boss = Cast<ABossCharacter>(AIController->GetPawn()); // 获取所控制的AI
		if (Boss && !Boss->GetIsStab()) // 还未执行stab
		{
			Boss->SetIsStab(true);
			Boss->GetWeapon()->SetActorHiddenInGame(false);
			Boss->DelayHideWeapon();
			Boss->BossSkill();
			float AnimTime = Boss->CastSkill(Boss, Boss->GetStabAttack_Montage(), 1.f);//TODO:Stab_Montage Finished	
			if (AnimTime != 0.0f)
			{
				//委托del执行tick更新
				FTimerDelegate Del;
				Del.BindUFunction(this, FName(TEXT("TaskFinish")), &OwnerComp);
				GetWorld()->GetTimerManager().SetTimer(TaskFinish_Handle, Del, AnimTime, false, AnimTime);
				return EBTNodeResult::InProgress;
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
void UBossStabAttackTaskNode::TaskFinish(UBehaviorTreeComponent* OwnerComp)
{
	//UE_LOG(LogTemp, Warning, TEXT("StabTask Finish!!!"));
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp->GetAIOwner());
	if (AIController)
	{
		ABossCharacter* Boss = Cast<ABossCharacter>(AIController->GetPawn());
		if (Boss)
		{
			//完成stab，重置
			Boss->SetIsStab(false);
			Boss->SetIsStabHit(false);
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
void UBossStabAttackTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController != nullptr)
	{
		ABossCharacter* Boss = Cast<ABossCharacter>(AIController->GetPawn());
		FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerLocation")); // 获得黑板上的数据
		if (Boss && Boss->GetIsStab() && !Boss->GetIsStabHit() && PlayerLocation.Size() < 100000.f)//防止未初始化
		{
			FVector BossLocation = Boss->GetActorLocation();
			float Dis = (BossLocation - PlayerLocation).Size();
			if (Dis < 100)
			{
				Boss->SetIsStabHit(true);
			}
			FRotator Turn = UKismetMathLibrary::FindLookAtRotation(BossLocation, PlayerLocation);
			FRotator Turn_Z(0.f, Turn.Yaw, 0.f);
			Boss->SetActorRotation(Turn_Z);
			FVector Direction = Turn_Z.Vector();
			FVector TargetLocation = PlayerLocation - 50.f * Direction;//跳跃目的地
			FVector NewBossLocation = FMath::VInterpTo(BossLocation, TargetLocation, DeltaSeconds, 8.f);//插值到newlocation
			Boss->SetActorLocation(NewBossLocation);
			//GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, FString::Printf(TEXT("%.3f %.3f %.3f|%.3f %.3f %.3f"), AILocation.X, AILocation.Y, AILocation.Z, NowTarget.X, NowTarget.Y, NowTarget.Z));
		}
	}
}