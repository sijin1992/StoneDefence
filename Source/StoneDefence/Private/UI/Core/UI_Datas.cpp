// 存放全局UI临时数据

#include "UI/Core/UI_Datas.h"
#include "Character/Core/RuleOfTheCharacter.h"

FGuid TowerIconGUID;
AActor* TowerDoll;
bool bLockGUID = false;

ARuleOfTheCharacter* ClickedTargetTower = nullptr;
ARuleOfTheCharacter* ClickedTargetMonster = nullptr;