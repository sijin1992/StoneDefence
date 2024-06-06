// 存放全局UI临时数据

#include "Global/UI_Datas.h"
#include "Character/Core/RuleOfTheCharacter.h"

FGuid TowerIconGUID;
AActor* TowerDoll;
bool bLockGUID = false;

ARuleOfTheCharacter* ClickedTargetTower = nullptr;
ARuleOfTheCharacter* ClickedTargetMonster = nullptr;