// Fill out your copyright notice in the Description page of Project Settings.
#include "EnermyCloseEnoughBTDecorator.h"
#include "EnermyAIController.h"
#include "EnermyCharacter.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
/**Abandon*/
bool UEnermyCloseEnoughBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AEnermyAIController* AIController = Cast<AEnermyAIController>(OwnerComp.GetAIOwner());
	//UE_LOG(LogTemp, Warning, TEXT("NDSAASD AIIIIII!!!"));
	if (AIController != nullptr)
	{
		FVector AILocation = Cast<AEnermyCharacter>(AIController->GetPawn())->GetActorLocation();
		ApicCharacter* Player = Cast<ApicCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()); // 获得玩家
		if (Player != nullptr)
		{
			FVector PlayerLocation = Player->GetActorLocation();
			// 计算距离
			float Dis = (PlayerLocation - AILocation).Size();
			//GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("AIopp Distance: %.3f %.3f %.3f %.3f"), PlayerLocation.X,PlayerLocation.Y,PlayerLocation.Z,Dis));
			if (Dis > MinDistance)
			{
				return true; // 不够近，继续移动
			}
		}
	}
	return false;

}

