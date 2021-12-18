// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuHUD.h"
#include "UObject/ConstructorHelpers.h"
AMainMenuHUD::AMainMenuHUD(const FObjectInitializer& Initailizer): Super(Initailizer)
{
	MainMenuBP_Path = TEXT("WidgetBlueprint'/Game/SideScrollerCPP/UI/MainMenu_BP.MainMenu_BP_C'");
	MainMenu_Class = LoadBP<UUserWidget>(MainMenuBP_Path);
	//TODO:替换constructorhelper Finished
	//static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetBP(TEXT("/Game/SideScrollerCPP/UI/MainMenu_BP")); // 找到蓝图类
	//if (MainMenuWidgetBP.Class!=nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Get MainMenuClass"));
	//	MainMenu_Class = MainMenuWidgetBP.Class; // 得到class
	//}
}
void AMainMenuHUD::BeginPlay()
{
	if (MainMenu_Class != nullptr)
	{
		myMenu = CreateWidget<UMainMenu>(GetWorld()->GetGameInstance(), MainMenu_Class);
		if (myMenu != nullptr)
		{
			myMenu->AddToViewport();
		}
	}
}


