// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnermyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API AEnermyAIController : public AAIController
{
	GENERATED_BODY()
	//class UBehaviorTree* BehaviorTree;
public:
	AEnermyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	class UBehaviorTreeComponent* BehaviorTreeCompo;
	class UBlackboardComponent* BlackboardComp;
	//之后再设置为private，并完成Set..comp函数
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeCompo() { return BehaviorTreeCompo; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() { return BlackboardComp; }
	void SetPlayerCaught(FVector Location);
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;

};
