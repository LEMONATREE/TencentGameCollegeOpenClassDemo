// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LoadResource.h"
/**
 * 
 */
class PIC_API CharacterSkill: public LoadResource//继承Load方法
{
public:
	CharacterSkill();
	~CharacterSkill();

	class UAnimMontage* Skill_Q_Montage;
	class UAnimMontage* Skill_E_Montage;
	class UAnimMontage* Skill_R_Montage;
	TArray<UAnimMontage*> SkillSet;
	int BasicSkillNum;
	//UAnimMontage* LoadSkillMontage(const FString& Path);
};
