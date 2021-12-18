// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadResource.h"

LoadResource::LoadResource()
{
}

LoadResource::~LoadResource()
{
}
UAnimMontage* LoadResource::LoadMontage(const FString& Path)
{
	return LoadObj<UAnimMontage>(Path);
}


