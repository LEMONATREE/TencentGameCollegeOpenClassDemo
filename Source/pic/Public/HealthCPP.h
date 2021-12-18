// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SlateDelegates.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "HealthCPP.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UHealthCPP : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentHealth_Text;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MaxHealth_Text;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Health_Display;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentEnergy_Text;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MaxEnergy_Text;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Energy_Display;
	void UpdateHealth(float CurrentHealth, float MaxHealth,float CurrentEnergy, float MaxEnergy);
	void SetTextBlock(UTextBlock* TextBlock, float Value);
	void SetProgressBar(UProgressBar* ProgressBar, float Percentage);
};
