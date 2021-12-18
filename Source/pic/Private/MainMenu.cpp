// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "picGameInstance.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	ModeSet.Push(FString(TEXT("当前难度:Easy")));
	ModeSet.Push(FString(TEXT("当前难度:Middle")));
	ModeSet.Push(FString(TEXT("当前难度:Hard")));
}
bool UMainMenu::Initialize()
{
	Super::Initialize();
	if (StartGameBtn != nullptr)
	{
		StartGameBtn->OnClicked.AddDynamic(this, &UMainMenu::OnClickedStartGame);
	}
	if (SettingBtn != nullptr)
	{
		SettingBtn->OnClicked.AddDynamic(this, &UMainMenu::OnClickedSetting);
	}
	if (QuitGameBtn != nullptr)
	{
		QuitGameBtn->OnClicked.AddDynamic(this, &UMainMenu::OnClickedQuitGame);
	}
	return true;
}

void UMainMenu::OnClickedStartGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UpicGameInstance* GI = Cast<UpicGameInstance>(World->GetGameInstance());
		if (GI)
		{
			GI->SetCurrentMode(this->CurrentMode);
			GI->SetCurrentMap(1);//mark下一关
		}
		this->RemoveFromViewport();//移除widget
		UE_LOG(LogTemp, Warning, TEXT("OnClicked StartGame"));
		FString NewMap = TEXT("/Game/SideScrollerCPP/Maps/BeginMap_0");
		World->ServerTravel(NewMap);
	}	
}
void UMainMenu::OnClickedSetting()
{
	UTextBlock* TxtMenuTitle= Cast<UTextBlock>(GetWidgetFromName("SettingText"));
	if (TxtMenuTitle)
	{
		CurrentMode = (CurrentMode + 1) % 3;
		FText ModeText = FText::FromString(ModeSet[CurrentMode]);
		TxtMenuTitle->SetText(ModeText);
	}
}
void UMainMenu::OnClickedQuitGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC != nullptr)
	{
		PC->ConsoleCommand(TEXT("quit"));
	}
}