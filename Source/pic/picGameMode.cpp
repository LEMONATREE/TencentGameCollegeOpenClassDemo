// Copyright Epic Games, Inc. All Rights Reserved.

#include "picGameMode.h"
#include "picCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "picController.h"

ApicGameMode::ApicGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = ApicController::StaticClass();
}
void ApicGameMode::BeginPlay()
{
	Super::BeginPlay();
	FString Sound_Path = TEXT("SoundCue'/Game/SideScrollerCPP/Sound/LittleAdventure_Cue.LittleAdventure_Cue'");//12.15
	Sound = LoadObj<USoundCue>(Sound_Path);
	if (Sound != nullptr) // 播放音乐
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}
