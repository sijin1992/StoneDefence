#include "Tool/ScreenMove.h"

bool FScreenMoveUnits::ListenScreenMove(APlayerController* PlayerController, const float& ScreenMoveSpeed)
{
	return MoveDirection(PlayerController, CursorMove(PlayerController), ScreenMoveSpeed);
}

EScreenMoveState FScreenMoveUnits::CursorMove(const APlayerController* PlayerController)
{
	if (PlayerController)
	{
		//屏幕尺寸
		int32 SizeX = INDEX_NONE;
		int32 SizeY = INDEX_NONE;

		//鼠标的位置
		float MousePositionX = -107374176.f;
		float MousePositionY = -107374176.f;

		//获取尺寸
		PlayerController->GetViewportSize(SizeX, SizeY);

		//获取鼠标的位置
		PlayerController->GetMousePosition(MousePositionX, MousePositionY);

		if (MousePositionX >= 0 && MousePositionX <= SizeX &&	//X轴范围
			MousePositionY >= 0 && MousePositionY <= SizeY)		//Y轴范围
		{
			if (FMath::IsNearlyEqual(MousePositionX,0.0f,5.f) && FMath::IsNearlyEqual(MousePositionY,0.0f,5.f))
			{
				return EScreenMoveState::Screen_LeftAndUp;		//左上
			}
			else if (FMath::IsNearlyEqual(MousePositionX, SizeX, 5.f) && FMath::IsNearlyEqual(MousePositionY, SizeY, 5.f))
			{
				return EScreenMoveState::Screen_RightAndDown;	//右下
			}
			else if (FMath::IsNearlyEqual(MousePositionX, SizeX, 5.f) && FMath::IsNearlyEqual(MousePositionY, 0.0f, 5.f))
			{
				return EScreenMoveState::Screen_RightAndUp;		//右上
			}
			else if (FMath::IsNearlyEqual(MousePositionX, 0.0f, 5.f) && FMath::IsNearlyEqual(MousePositionY, SizeY, 5.f))
			{
				return EScreenMoveState::Screen_LeftAndDown;	//左下
			}
			else if (FMath::IsNearlyEqual(MousePositionX, 0.0f, 5.f))
			{
				return EScreenMoveState::Screen_Left;			//左
			}
			else if (FMath::IsNearlyEqual(MousePositionY, 0.0f, 5.f))
			{
				return EScreenMoveState::Screen_Up;				//上
			}
			else if (FMath::IsNearlyEqual(MousePositionY, SizeY, 5.f))
			{
				return EScreenMoveState::Screen_Down;			//下
			}
			else if (FMath::IsNearlyEqual(MousePositionX, SizeX, 5.f))
			{
				return EScreenMoveState::Screen_Right;			//右
			}
		}
	}

	return EScreenMoveState::Screen_None;
}

bool FScreenMoveUnits::MoveDirection(APlayerController* PlayerController, EScreenMoveState ScreenMoveState, const float& ScreenMoveSpeed)
{
	FVector OffsetValue = FVector::ZeroVector;

	if (PlayerController &&
		PlayerController->GetPawn())
	{
		switch (ScreenMoveState)
		{
		case EScreenMoveState::Screen_Up:
			OffsetValue = FVector(ScreenMoveSpeed, 0.0f, 0.0f);
			break;
		case EScreenMoveState::Screen_Down:
			OffsetValue = FVector(-ScreenMoveSpeed, 0.0f, 0.0f);
			break;
		case EScreenMoveState::Screen_Right:
			OffsetValue = FVector(0.0f, ScreenMoveSpeed, 0.0f);
			break;
		case EScreenMoveState::Screen_Left:
			OffsetValue = FVector(0.0f, -ScreenMoveSpeed, 0.0f);
			break;
		case EScreenMoveState::Screen_RightAndUp:
			OffsetValue = FVector(ScreenMoveSpeed, ScreenMoveSpeed, 0.0f);
			break;
		case EScreenMoveState::Screen_RightAndDown:
			OffsetValue = FVector(-ScreenMoveSpeed, ScreenMoveSpeed, 0.0f);
			break;
		case EScreenMoveState::Screen_LeftAndUp:
			OffsetValue = FVector(ScreenMoveSpeed, -ScreenMoveSpeed, 0.0f);
			break;
		case EScreenMoveState::Screen_LeftAndDown:
			OffsetValue = FVector(-ScreenMoveSpeed, -ScreenMoveSpeed, 0.0f);
			break;
		}

		PlayerController->GetPawn()->AddActorWorldOffset(OffsetValue);
	}
	return OffsetValue != FVector::ZeroVector;
}