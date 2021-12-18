// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LoadResource.h"
/**
 * 
 */
class PIC_API BaseSkill: public LoadResource
{
public:
	BaseSkill();
	~BaseSkill();

	class UAnimMontage* Death_Montage;
	class UAnimMontage* BeAttacked_Montage;
	class UAnimMontage* JumpBack_Montage;
	//UAnimMontage* LoadSkillMontage(const FString& Path);
};
