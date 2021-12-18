// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadResourceInterface.h"
/** Abandon */
// Add default functionality here for any ILoadResourceInterface functions that are not pure virtual.
UAnimMontage* ILoadResourceInterface::LoadMontage(const FString& Path)
{
	return LoadObj<UAnimMontage>(Path);
}

