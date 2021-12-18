// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "MainMenu.h"
#include "LoadResourceInterface.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API AMainMenuHUD : public AHUD, public ILoadResourceInterface
{
	GENERATED_BODY()
public:
	AMainMenuHUD(const FObjectInitializer& Initailizer);
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;
	FString MainMenuBP_Path;
	TSubclassOf<class UUserWidget> MainMenu_Class;
private:
	UMainMenu* myMenu;
};
