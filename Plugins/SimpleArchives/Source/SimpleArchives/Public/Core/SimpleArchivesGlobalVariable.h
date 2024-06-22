// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SimpleArchivesInterface.h"

extern int32 SimpleSlotIndex;

namespace SimpleArchivesGlobalVariable
{
	//只能内部调用
	TArray<ISimpleArchivesInterface*>& GetSimpleArchivesArray();

	//暴露给外部调用
	SIMPLEARCHIVES_API void ClearSimpleArchivesArray();

	void AddArchivesInterface(ISimpleArchivesInterface* NewArchivesInterface);

	void RemoveArchivesInterface(ISimpleArchivesInterface* NewArchivesInterface);
}
