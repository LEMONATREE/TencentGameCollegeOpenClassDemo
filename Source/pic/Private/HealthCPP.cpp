// Fill out your copyright notice in the Description page of Project Settings.
#include "HealthCPP.h"

void UHealthCPP::SetTextBlock(UTextBlock* TextBlock, float Value)
{
	if (TextBlock != nullptr)
	{
		FText Text = FText::AsNumber((int)Value);
		TextBlock->SetText(Text);
	}
}
void UHealthCPP::SetProgressBar(UProgressBar* ProgressBar, float Percentage)
{
	if (ProgressBar != nullptr)
	{
		ProgressBar->SetPercent(Percentage);
	}
}
void UHealthCPP::UpdateHealth(float CurrentHealth, float MaxHealth,float CurrentEnergy,float MaxEnergy)
{
	SetTextBlock(CurrentHealth_Text, CurrentHealth);
	SetTextBlock(MaxHealth_Text, MaxHealth);
	SetProgressBar(Health_Display, CurrentHealth / MaxHealth);

	SetTextBlock(CurrentEnergy_Text, CurrentEnergy);
	SetTextBlock(MaxEnergy_Text, MaxEnergy);
	SetProgressBar(Energy_Display, CurrentEnergy / MaxEnergy);
}
