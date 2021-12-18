// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SlateDelegates.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Components/WidgetComponent.h"
#include "ComboWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIC_API UComboWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Combo_Text;
	void UpdateCombo(int Count);
	void HideCombo();

};
