// Fill out your copyright notice in the Description page of Project Settings.
#include "EnermyAnimInstance.h"
#include "EnermyCharacter.h"
void UEnermyAnimInstance::UpdateAnimInfo(float dt)
{
	APawn* Owner = TryGetPawnOwner();
	AEnermyCharacter* Player = Cast<AEnermyCharacter>(Owner);
	if (Owner != nullptr)
	{
		Speed = Owner->GetVelocity().Size();
		Direction = CalculateDirection(Owner->GetVelocity(), Owner->GetBaseAimRotation());
		bInAir = Player->GetMovementComponent()->IsFalling();
	}
}

