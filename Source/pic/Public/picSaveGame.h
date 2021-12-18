// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "picSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UpicSaveGame : public USaveGame
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)//interact with blueprint
	float PlayerScore = 0.f;
public:
	FORCEINLINE float GetPlayerScore() const { return PlayerScore; }
	FORCEINLINE void SetPlayerScore(float value) { PlayerScore = value; }

};
