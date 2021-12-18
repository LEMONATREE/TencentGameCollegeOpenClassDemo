// Fill out your copyright notice in the Description page of Project Settings.
#include "EnermyAIController.h"
#include "EnermyCharacter.h"

AEnermyAIController::AEnermyAIController()
{
	BehaviorTreeCompo = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}
void AEnermyAIController::OnPossess(APawn* InPawn)//拥有控制权->启动BehaviorTree
{
	AEnermyCharacter* Enermy = Cast<AEnermyCharacter>(InPawn);
	if (Enermy == nullptr)
	{
		return;
	}
	Super::OnPossess(InPawn);
	if (Enermy && Enermy->BehaviorTree && Enermy->BehaviorTree->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*(Enermy->BehaviorTree->BlackboardAsset)); // 初始化
		BehaviorTreeCompo->StartTree(*(Enermy->BehaviorTree)); // 运行行为树
		//定义攻击类型对应的距离
		BlackboardComp->SetValueAsFloat(FName("LongDistance"), 1800.f);
		BlackboardComp->SetValueAsFloat(FName("MiddleDistance"), 900.f);
		BlackboardComp->SetValueAsFloat(FName("ShortDistance"), 400.f);
		BlackboardComp->SetValueAsFloat(FName("CloseDistance"), 200.f);
	}
}
void AEnermyAIController::OnUnPossess()//失去对Character的控制->停止运行BehaviorTree
{
	Super::OnUnPossess();
	BehaviorTreeCompo->StopTree(EBTStopMode::Safe); 
}
void AEnermyAIController::SetPlayerCaught(FVector Location)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(FName("PlayerLocation"), Location);
	}
}
