// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "picGameInstance.h"

ASaveGameActor::ASaveGameActor()
{
}
void ASaveGameActor::BeginPlay()
{
	Super::BeginPlay();
    /** Receive KeyInput */
	EnableInput(GetWorld()->GetFirstPlayerController());
    UInputComponent* myInputComp = this->InputComponent;  
    if (myInputComp)
    {
        SetupActorInputComponent(myInputComp);
    }
    bool SaveFileExist = UGameplayStatics::DoesSaveGameExist(TEXT("SavedGame"), 0);
    /** Load or create savefile */
    if (SaveFileExist)
    {
        SaveGame = Cast<UpicSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SavedGame"), 0));
    }
    else
    {
        SaveGame = Cast<UpicSaveGame>(UGameplayStatics::CreateSaveGameObject(UpicSaveGame::StaticClass()));
        SaveGame->SetPlayerScore(InitScore);
        UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("SavedGame"), 0);
    }
}
void ASaveGameActor::LoadGameFile()//读取测试
{
    SaveGame = Cast<UpicSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SavedGame"), 0));
    GEngine->AddOnScreenDebugMessage(0, 2.f, FColor(23, 233, 4, 255), FString::Printf(TEXT("读取击杀分数:%.0f"), SaveGame->GetPlayerScore()));
}
void ASaveGameActor::SaveGameFile()
{
    if (SaveGame)
    {
        //从gameinstance获取score再保存
        UpicGameInstance* GI = Cast<UpicGameInstance>(GetWorld()->GetGameInstance());
        if (GI)
        {
            SaveGame->SetPlayerScore(GI->GetPlayerScore());
            UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("SavedGame"), 0);
            GEngine->AddOnScreenDebugMessage(0, 2.f, FColor(23, 233, 4, 255), FString::Printf(TEXT("记录击杀分数:%.0f"), SaveGame->GetPlayerScore()));
        }
    }
}
void ASaveGameActor::SetupActorInputComponent(UInputComponent* myInputComponent)
{
    myInputComponent->BindAction("LoadFile", IE_Pressed, this, &ASaveGameActor::LoadGameFile);//key L
    myInputComponent->BindAction("SaveFile", IE_Pressed, this, &ASaveGameActor::SaveGameFile);//key P
}
