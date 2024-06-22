// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SimpleArchivesGlobalVariable.h"

TArray<ISimpleArchivesInterface*> SimpleArchivesArray;//全局的，做收集的工作

int32 SimpleSlotIndex = INDEX_NONE;

TArray<ISimpleArchivesInterface*>& SimpleArchivesGlobalVariable::GetSimpleArchivesArray()
{
	return SimpleArchivesArray;
}

void SimpleArchivesGlobalVariable::ClearSimpleArchivesArray()
{
	SimpleArchivesArray.Empty();
}

void SimpleArchivesGlobalVariable::AddArchivesInterface(ISimpleArchivesInterface* NewArchivesInterface)
{
	SimpleArchivesArray.AddUnique(NewArchivesInterface);
}

void SimpleArchivesGlobalVariable::RemoveArchivesInterface(ISimpleArchivesInterface* NewArchivesInterface)
{
	SimpleArchivesArray.Remove(NewArchivesInterface);
}
