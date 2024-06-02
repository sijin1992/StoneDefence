//全局代理
#pragma once

#include "CoreMinimal.h"
/*
#include "GlobalTutoriaProxy.generated.h"

//非动态多播
DECLARE_MULTICAST_DELEGATE_OneParam(FSimpleTutorialMulticastDelegate, int32);
FSimpleTutorialMulticastDelegate SimpleTutoriaMulticastDelegate;
*/

//非动态单播
DECLARE_DELEGATE_RetVal_OneParam(bool, FSimpleTutorialDelegate, int32)
FSimpleTutorialDelegate SimpleTutoriaDelegate;

DECLARE_DELEGATE_RetVal_OneParam(bool, FSimpleTutorialPathDelegate, const FString &)
FSimpleTutorialPathDelegate SimpleTutoriaPathDelegate;