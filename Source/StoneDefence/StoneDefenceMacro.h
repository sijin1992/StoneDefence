#include "StoneDefence.h"
//自定义宏

//宏打印
#define SD_print_s(Format,...) \
{const FString Msg_s = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, Msg_s);}

#define SD_print(Type,Format,...) \
{const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
UE_LOG(LogStoneDefence, Type, TEXT("%s"), *Msg);}

#if WITH_EDITOR
#define SD_print_r(Type,Format,...) \
{const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, Msg); \
UE_LOG(LogStoneDefence, Type, TEXT("%s"), *Msg);}
#else
#define SD_print_r(Type,Format,...) \
SD_print(Type,Format, ##__VA_ARGS__)
#endif

#define SKILL_TABLE_ACQUISITION(ValueBase,SkillAttr) \
float Value = ValueBase; \
for (const auto& Temp : AdditionalSkillData) \
{ \
	switch (Temp.Value.SkillType.SkillEffectType) \
	{ \
		case ESkillEffectType::ADD: \
			Value += Temp.Value.SkillAttr; \
			break; \
		case ESkillEffectType::SUBTRACT: \
			Value -= Temp.Value.SkillAttr; \
			break; \
	} \
} \
return Value;

#define SKILL_TABLE_ACQUISITION_CD(InCD) \
float Value = InCD; \
for (const auto& Temp : AdditionalSkillData) \
{ \
	Value -= Temp.Value.InCD; \
} \
return Value;