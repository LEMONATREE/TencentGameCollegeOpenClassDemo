// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyClassBTDecorator.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
//在BehaviorTree中判断Enermy类型
bool UEnermyClassBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	if (AIController != nullptr)
	{
		AEnermyCharacter* AI = Cast<AEnermyCharacter>(AIController->GetPawn());
		if (AI->GetEnermyClass() == this->EnermyClass)
		{
			return true;
		}
	}
	return false;
}

