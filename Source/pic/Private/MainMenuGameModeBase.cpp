// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameModeBase.h"
#include "MainMenuHUD.h"
#include "AMainMenuPlayerController.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
	UE_LOG(LogTemp, Warning, TEXT("Mode:Menu"));//要在关卡蓝图内选择对应的游戏模式，否则默认加载项目定义的模式可能造成错误
	HUDClass = AMainMenuHUD::StaticClass();
	PlayerControllerClass = AAMainMenuPlayerController::StaticClass();
}
void AMainMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FString Sound_Path = TEXT("SoundCue'/Game/SideScrollerCPP/Sound/Into_The_Sun_Cue.Into_The_Sun_Cue'");//12.15
	Sound = LoadObj<USoundCue>(Sound_Path);
	if (Sound != nullptr) // 播放音乐
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}