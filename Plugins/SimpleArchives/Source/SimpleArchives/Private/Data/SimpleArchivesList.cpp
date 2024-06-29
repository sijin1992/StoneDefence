#include "Data/SimpleArchivesList.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapper.h"

#define LOCTEXT_NAMESPACE "SaveSlot"

FGameArchivesThumbnail::FGameArchivesThumbnail()
	:GameThumbnail(nullptr)
{

}


void FGameArchivesThumbnail::InitResource()
{
	if (!ScrPath.IsEmpty())
	{
		if (!GameThumbnail)
		{
			LoadTexture2D(ScrPath);
		}
	}
}

void FGameArchivesThumbnail::LoadTexture2D(const FString& ImagePath)
{
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		TArray<uint8> CompressedData;
		if (FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
		{
			//加载专门处理图片的模块
			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
			//创建JPG
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);

			if (ImageWrapper.IsValid() &&
				ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
			{
				TArray<uint8> UnCompressedRGBA;
				if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UnCompressedRGBA))
				{
					GameThumbnail = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
					if (GameThumbnail != nullptr)
					{
						GameThumbnail->UpdateResource();
					}
				}
			}
			//释放图片处理工具
			ImageWrapper.Reset();
		}
	}
}

FSaveSlot::FSaveSlot()
{
	Init();
}

void FSaveSlot::Init()
{
	/*GameThumbnail = nullptr;*/
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