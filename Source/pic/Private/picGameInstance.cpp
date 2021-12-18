// Fill out your copyright notice in the Description page of Project Settings.


#include "picGameInstance.h"
UpicGameInstance::UpicGameInstance()
{
	/** Weapon Info */
	WeaponBasicDamageSet.Push(1.f);
	WeaponBasicDamageSet.Push(10.f);
	WeaponBasicDamageSet.Push(100.f);
	WeaponDamageCoeffSet.Push(1.f);
	WeaponDamageCoeffSet.Push(1.5f);
	WeaponDamageCoeffSet.Push(2.f);
}
/** Player Info */
void UpicGameInstance::PlayerLevelUp()
{
	PlayerLevel += (CurrentMode + 1);
}