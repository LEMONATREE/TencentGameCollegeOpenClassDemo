// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkill.h"

BaseSkill::BaseSkill()
{
	FString Death = "/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Death_Front_Montage";
	Death_Montage = LoadMontage(Death);
	FString BeAttacked = "/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/BeAttack_Dodge_Montage";
	BeAttacked_Montage = LoadMontage(BeAttacked);
	FString JumpBack = "/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/Jump_Back_AnimMontage";
	JumpBack_Montage = LoadMontage(JumpBack);
}
BaseSkill::~BaseSkill()
{
}

