// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LoadResource.h"
#include "MainMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API AMainMenuGameModeBase : public AGameModeBase, public LoadResource
{
	GENERATED_BODY()
	AMainMenuGameModeBase();
private:
	class USoundCue* Sound;
protected:
	virtual void BeginPlay() override;
};
