// Fill out your copyright notice in the Description page of Project Settings.
#include "ComboWidget.h"
void UComboWidget::UpdateCombo(int Count)
{
	//将ComboCount转化成Text显示在Widget中
	Combo_Text->SetVisibility(ESlateVisibility::Visible);
	FText Text = FText::FromString(FString::FromInt(Count) + " x Combo!");
	Combo_Text->SetText(Text);
}
void UComboWidget::HideCombo()
{
	Combo_Text->SetVisibility(ESlateVisibility::Hidden);
}