#pragma once

#include "CoreMinimal.h"
#include "SimplePopupType.h"

class UUI_SimplePopup;
//仅供C++使用
namespace SimplePopupUtils
{
	//创建弹窗
	SIMPLEPOPUP_API FPopupHandle CreatePopup(
		TSubclassOf<UUI_SimplePopup> PopupClass,
		const FText& Content,
		ESimplePopupType SimplePopupType,
		float Life = 10.0f,
		FSimpleDelegate SureDelegate = FSimpleDelegate(),
		FSimpleDelegate CancelDelegate = FSimpleDelegate()
	);
}