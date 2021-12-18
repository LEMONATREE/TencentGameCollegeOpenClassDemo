// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoadResource.h"
#include "picGameMode.generated.h"

UCLASS(minimalapi)
class ApicGameMode : public AGameModeBase, public LoadResource
{
	GENERATED_BODY()

public:
	ApicGameMode();
private:
	class USoundCue* Sound;
protected:
	virtual void BeginPlay() override;
};



