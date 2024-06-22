#include "Data/SimpleArchivesList.h"

#define LOCTEXT_NAMESPACE "SaveSlot"

FSaveSlot::FSaveSlot()
{
	Init();
}

void FSaveSlot::Init()
{
	GameThumbnail = nullptr;
	DateText = LOCTEXT("SaveSlot", "Save Slot hello world~~");
	bSave = false;
}

FSaveSlotList::FSaveSlotList()
{

}

void FSaveSlotList::InitSlot()
{
	for (int32 i = 0; i < 20; i++)
	{
		Slots.Add(i, FSaveSlot());
	}

	for (int32 i = 0; i < 5; i++)
	{
		DegreeOfCompletion.Add(i + 1, 0.0f);
	}
}

int32 FSaveSlotList::GetSerialNumber()
{
	for (auto& Temp : Slots)
	{
		if (!Temp.Value.bSave)
		{
			return Temp.Key;
		}
	}
	return INDEX_NONE;
}

bool FSaveSlotList::RemoveAtGameData(int32 SlotNumber)
{
	if (Slots.Contains(SlotNumber))
	{
		Slots[SlotNumber].bSave = false;
		return true;
	}
	return false;
}

bool FSaveSlotList::AddGameDataByNumber(int32 SlotNumber)
{
	if (Slots.Contains(SlotNumber))
	{
		Slots[SlotNumber].bSave = true;
		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE