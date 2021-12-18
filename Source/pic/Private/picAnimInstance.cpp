// Fill out your copyright notice in the Description page of Project Settings.


#include "picAnimInstance.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
void UpicAnimInstance::UpdateAnimInfo(float dt)
{
	APawn* Owner = TryGetPawnOwner(); // 得到所属的pawn
	ApicCharacter* Player = Cast<ApicCharacter>(Owner);
	if (Owner != nullptr)
	{
		Speed = Owner->GetVelocity().Size(); // 获得移动速度
		Direction = CalculateDirection(Owner->GetVelocity(), Owner->GetBaseAimRotation()); // 通过速度和旋转计算方向 (-180~180)
		bInAir = Player->GetMovementComponent()->IsFalling();
	}
}