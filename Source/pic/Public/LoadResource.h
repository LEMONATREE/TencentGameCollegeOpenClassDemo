// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
/**
 * 
 */
class PIC_API LoadResource
{
public:
	LoadResource();
	~LoadResource();
	//UObject* LoadFromName(const FString& Path);
    static UAnimMontage* LoadMontage(const FString& Path);//静态方法 只保留一个备份

    template<typename ObjClass>
    static FORCEINLINE ObjClass* LoadObj(const FString& Path)//加载音频等非蓝图资源
    {
        if (Path.IsEmpty())
        {
			return nullptr;
        }
        //TODO:将staticload换成load试一下
        ObjClass* Load_Obj = Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *(Path)));
        return Load_Obj;
    }
    template<typename ObjClass>
    static FORCEINLINE UClass* LoadBP(const FString& Path)//加载蓝图资源
    {
        //ObjClass为蓝图父类
        //GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("LoadBP1")));
        if (Path.IsEmpty())
        {
            //GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("LoadBP2")));
            return nullptr;
        }
        //GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("LoadBP3")));
        UClass* Load_BP = LoadClass<ObjClass>(NULL, *(Path));
        return Load_BP;
    }

};
