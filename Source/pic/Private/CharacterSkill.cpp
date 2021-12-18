// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkill.h"

CharacterSkill::CharacterSkill()
{
	SkillSet.Push(LoadMontage("/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Move_achilly_Montage"));
	SkillSet.Push(LoadMontage("/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Move_fast_Montage"));
	SkillSet.Push(LoadMontage("/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Move_fast_Montage_2"));
	SkillSet.Push(LoadMontage("/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Move_fast_Montage_3"));
	SkillSet.Push(LoadMontage("/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Move_Med_half_Montage"));
	SkillSet.Push(LoadMontage("/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Place_fast_down_Montage"));
	
	BasicSkillNum = SkillSet.Num();

	FString Skill_Q = "/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Attack_Montage1.Attack_Montage1";
	Skill_Q_Montage = LoadMontage(Skill_Q);
	FString Skill_E = "/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Attack_Jump_Montage";
	Skill_E_Montage = LoadMontage(Skill_E);
	FString Skill_R = "/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Attack_Montage2.Attack_Montage2";
	Skill_R_Montage = LoadMontage(Skill_R);
	
	SkillSet.Push(Skill_Q_Montage);
	SkillSet.Push(Skill_E_Montage);
	SkillSet.Push(Skill_R_Montage);
}

CharacterSkill::~CharacterSkill()
{
}
