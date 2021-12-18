// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SlateDelegates.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	//virtual void NativeConstruct() override;
	//UMainMenu();
	virtual bool Initialize() override;
	UFUNCTION()
	void OnClickedStartGame();
	UFUNCTION()
	void OnClickedSetting();
	UFUNCTION()
	void OnClickedQuitGame();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartGameBtn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SettingBtn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitGameBtn;
	FORCEINLINE class UButton* GetStartGameBtn() { return StartGameBtn; }
	FORCEINLINE class UButton* GetSettingBtn() { return SettingBtn; }
	FORCEINLINE class UButton* GetQuitGameBtn() { return QuitGameBtn; }
	int CurrentMode = 0;
	TArray<FString> ModeSet;
};
