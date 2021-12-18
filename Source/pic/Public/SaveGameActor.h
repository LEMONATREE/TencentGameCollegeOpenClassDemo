// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "picSaveGame.h"
#include "LoadResource.h"
#include "SaveGameActor.generated.h"

UCLASS()
class PIC_API ASaveGameActor : public AActor, public LoadResource
{
	GENERATED_BODY()
protected:
	/** SaveGame */
	UPROPERTY(EditAnywhere)
    float InitScore = 0.f;
	UPROPERTY()//Interact with blueprint
	UpicSaveGame* SaveGame;
public:
	void LoadGameFile();
	void SaveGameFile();
	void SetupActorInputComponent(UInputComponent* myInputComponent);

protected:
	virtual void BeginPlay() override;	
public:	
	ASaveGameActor();
	//virtual void Tick(float DeltaTime) override;
};
