// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LoadResourceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULoadResourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIC_API ILoadResourceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    static UAnimMontage* LoadMontage(const FString& Path);//静态方法 只保留一个备份

public:
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
        if (Path.IsEmpty())
        {
            return nullptr;
        }
        UClass* Load_BP = LoadClass<ObjClass>(NULL, *(Path));
        return Load_BP;
    }
};
