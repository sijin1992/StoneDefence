// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI_ArchivesWidget.h"
#include "Interface/SimpleArchivesInterface.h"
#include "Core/SimpleArchivesGlobalVariable.h"

ISimpleArchivesInterface* UUI_ArchivesWidget::GetCorrectArchivesInterface()
{
	TArray<ISimpleArchivesInterface*>& MyArchives = SimpleArchivesGlobalVariable::GetSimpleArchivesArray();
	for (const auto& Temp : MyArchives)
	{
		if (Temp->GetSimpleWorld())
		{
			return Temp;
		}
	}
	return nullptr;
}
