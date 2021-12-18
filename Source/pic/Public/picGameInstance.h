// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveGameActor.h"
#include "picGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UpicGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	/** Game Info */
	int CurrentMode = 0;
	int CurrentMap = 0;
	double GameBeginTime = 0.0;
	bool RotationActorIsSpawn = false;
	int TotalEnermyNum = 20;
	/** Player Info */
	int PlayerLevel = 0;
	float PlayerScore = 0.f;
	/** Weapon Info */
	TArray<float> WeaponBasicDamageSet;
	TArray<float> WeaponDamageCoeffSet;
public:
	/** Game Info */
	FORCEINLINE int GetCurrentMode() const { return CurrentMode; }
	FORCEINLINE int GetCurrentMap() const { return CurrentMap; }
	FORCEINLINE double GetGameBeginTime() const { return GameBeginTime; }
	FORCEINLINE bool GetRotationActorIsSpawn() const { return RotationActorIsSpawn; }
	FORCEINLINE int GetTotalEnermyNum() const { return TotalEnermyNum; }
	FORCEINLINE void SetCurrentMode(int value) { CurrentMode = value; }
	FORCEINLINE void SetCurrentMap(int value) { CurrentMap = value; }
	FORCEINLINE void SetGameBeginTime(double value) { GameBeginTime = value; }
	FORCEINLINE void SetRotationActorIsSpawn(bool value) { RotationActorIsSpawn = value; }
	FORCEINLINE void SetTotalEnermyNum(int value) { TotalEnermyNum = value; }
	/** Player Info */
	FORCEINLINE int GetPlayerLevel() const { return PlayerLevel; }
	FORCEINLINE float GetPlayerScore() const { return PlayerScore; }
	FORCEINLINE void SetPlayerLevel(int value) { PlayerLevel = value; }
	FORCEINLINE void SetPlayerScore(float value) { PlayerScore = value; }
	void PlayerLevelUp();
	/** Weapon Info */
	FORCEINLINE TArray<float> GetWeaponBasicDamageSet() const { return WeaponBasicDamageSet; }
	FORCEINLINE TArray<float> GetWeaponDamageCoeffSet() const { return WeaponDamageCoeffSet; }
public:
	UpicGameInstance();
};

