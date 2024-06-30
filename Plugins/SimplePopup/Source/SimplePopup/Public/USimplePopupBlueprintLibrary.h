// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimplePopupType.h"
#include "USimplePopupBlueprintLibrary.generated.h"

class UUI_SimplePopup;
/**
 * We can create a pop-up window and return a handle, according to which we can perform the behavior in the pop-up window. Of course, it can not be implemented.@See CreatePopup,PopupSure,PopupCancel
 */
UCLASS()
class SIMPLEPOPUP_API UUSimplePopupBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/// <summary>
	/// You can create a pop-up window based on Customization .
	/// </summary>
	/// <param name="WorldContextObject"></param>
	/// <param name="PopupClass">Need a UMG Popup class</param>
	/// <param name="Content">What to display in the pop-up window</param>
	/// <param name="SimplePopupType">Types of pop ups @See SimplePopupType.h</param>
	/// <param name="SureDelegate">Definite agent of pop-up window</param>
	/// <param name="CancelDelegate">Cancel agent for pop ups</param>
	/// <param name="Life">When the pop-up window type is ESimplePopupType::DELAY, you need a custom time to destroy it</param>
	/// <returns>Returns a pop-up handle. You can customize many pop-up behaviors .@See PopupSure(...),PopupCancel(..)</returns>
	UFUNCTION(BlueprintCallable, Category = SimplePopup, meta = (WorldContext = WorldContextObject))
	static FPopupHandle CreatePopup(
		UObject* WorldContextObject,
		TSubclassOf<UUI_SimplePopup> PopupClass,
		const FText& Content,
		ESimplePopupType SimplePopupType,
		FSimpleBlueprintDelegate SureDelegate,
		FSimpleBlueprintDelegate CancelDelegate,
		float Life = 10.0f);

	/// <summary>
	/// Determine pop-up behavior ,Pop up window will disappear after execution .
	/// </summary>
	/// <param name="PopupHandle"></param>
	/// <returns>When the behavior is executed correctly, return to ture </returns>
	UFUNCTION(BlueprintCallable, Category = SimplePopup)
	static bool PopupSure(FPopupHandle PopupHandle);

	/// <summary>
	/// Cancel pop-up behavior ,Pop up window will disappear after execution .
	/// </summary>
	/// <param name="PopupHandle"></param>
	/// <returns>When the behavior is executed correctly, return to ture .</returns>
	UFUNCTION(BlueprintCallable, Category = SimplePopup)
	static bool PopupCancel(FPopupHandle PopupHandle);
};
