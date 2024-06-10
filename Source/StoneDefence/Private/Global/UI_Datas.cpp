// 存放全局UI临时数据

#include "Global/UI_Datas.h"
#include "Character/Core/RuleOfTheCharacter.h"


int32 PlayerSkillNumber = 0;

FGuid TowerIconGUID;
AActor* TowerDoll;
bool bLockGUID = false;

ARuleOfTheCharacter* ClickedTargetTower = nullptr;
ARuleOfTheCharacter* ClickedTargetMonster = nullptr;