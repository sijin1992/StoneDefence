#include "SScreenLoading.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Notifications/SProgressBar.h"

void SScreenLoading::Construct(const FArguments &InArgs, const FString &InMapname)
{
	MapName = InMapname;
	ChildSlot
		[
			SNew(SProgressBar)
			.Percent(this, &SScreenLoading::GetProgress)
		];
}

TOptional<float> SScreenLoading::GetProgress() const
{
	//异步获取百分比进度,开始时时-1，结束后也是-1,中间是0~100
	float LoadPrecentage = GetAsyncLoadPercentage(*MapName);
	return LoadPrecentage * 0.01;
}

