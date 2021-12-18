// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseCharacter.h"
#include "BaseCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBaseCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIC_API IBaseCharacterInterface
{
	GENERATED_BODY()
public:
	/** Weapon */
	void UpdateWeaponLocationRotation(ABaseCharacter* Character);
	void EnableWeaponCollision(ABaseCharacter* Character);
	void DisableWeaponCollision(ABaseCharacter* Character);
	/** Skill */
	float CastSkill(ABaseCharacter* Character, UAnimMontage* Skill_key, float PlayRate);
	void StartCastSkill(ABaseCharacter* Character);
	void FinishCastSkill(ABaseCharacter* Character);
	/** Death & Revive */
	void Revive(ABaseCharacter* Character);
	void DeathCheck(ABaseCharacter* Character);
	void DestroyCharacter(ABaseCharacter* Character);
	/** Player Info */
	void UpdateLevel(ABaseCharacter* Character);
public:
	template<typename ObjClass>
	FORCEINLINE void DestroyActorFromName(UWorld* World, FString ActorName)
	{
		TSubclassOf<ObjClass> ActorToFind = ObjClass::StaticClass();
		TArray<AActor*> FoundActor;
		UGameplayStatics::GetAllActorsOfClass(World, ActorToFind, FoundActor);
		for (auto Actor : FoundActor)
		{
			//GEngine->AddOnScreenDebugMessage(0, 2.f, FColor(23, 233, 4, 255), FString::Printf(TEXT("Actor Delete")));
			if (Actor->GetName() == ActorName)
			{		
				Actor->Destroy();
			}
		}
	}
	template<typename ObjClass>
	FORCEINLINE void DestroyAllActorFromClass(UWorld* World)
	{
		TSubclassOf<ObjClass> ActorToFind = ObjClass::StaticClass();
		TArray<AActor*> FoundActor;
		UGameplayStatics::GetAllActorsOfClass(World, ActorToFind, FoundActor);
		for (auto Actor : FoundActor)
		{
			Actor->Destroy();	
		}
	}
};
