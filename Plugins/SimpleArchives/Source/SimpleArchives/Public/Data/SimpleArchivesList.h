#pragma once

#include "CoreMinimal.h"
#include "SimpleArchivesList.generated.h"

//游戏储存缩略图
USTRUCT()
struct SIMPLEARCHIVES_API FGameArchivesThumbnail
{
	GENERATED_USTRUCT_BODY()

	FGameArchivesThumbnail();

	//存档缩略图,Transient表示当前数据没法保存
	UPROPERTY(Transient)
	UTexture2D* GameThumbnail;

	UPROPERTY(SaveGame)
	FString ScrPath;

	void InitResource();

protected:
	//读取图片
	void LoadTexture2D(const FString& ImagePath);
};

USTRUCT()
struct SIMPLEARCHIVES_API FSaveSlot
{
	GENERATED_USTRUCT_BODY()

	FSaveSlot();

	//存档缩略图
	UPROPERTY(SaveGame)
	FGameArchivesThumbnail GameThumbnail;

	//存档关卡名
	UPROPERTY(SaveGame)
	FText LevelName;

	//存档章节名
	UPROPERTY(SaveGame)
	FText ChapterName;

	//存档日期
	UPROPERTY(SaveGame)
	FText DateText;

	//是否有存档
	UPROPERTY(SaveGame)
	bool bSave;

	void Init();

};


USTRUCT()
struct SIMPLEARCHIVES_API FSaveSlotList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame)
	TMap<int32, FSaveSlot> Slots;//存档列表
	UPROPERTY(SaveGame)
	TMap<int32, float> DegreeOfCompletion;//关卡完成度

	FSaveSlotList();

	//由外部调用
	void InitSlot();

	//获得空闲存档的下标
	int32 GetSerialNumber();
	//删除存档
	bool RemoveAtGameData(int32 SlotNumber);
	//添加存档
	bool AddGameDataByNumber(int32 SlotNumber);
};