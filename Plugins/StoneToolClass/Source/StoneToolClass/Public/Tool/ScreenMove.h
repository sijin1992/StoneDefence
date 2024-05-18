#pragma once

#include "CoreMinimal.h"

enum EScreenMoveState
{
	Screen_None,
	Screen_Up,				//上
	Screen_Down,			//下
	Screen_Right,			//右
	Screen_Left,			//左
	Screen_RightAndUp,		//右上
	Screen_RightAndDown,	//右下
	Screen_LeftAndUp,		//左上
	Screen_LeftAndDown,		//左下
	Screen_Max
};

class APlayerController;

struct STONETOOLCLASS_API FScreenMoveUnits
{
	/**
	* 监听屏幕移动
	* PlayerController:	玩家控制器
	* ScreenMoveSpeed:	屏幕移动速度
	* @return:			返回是否移动成功
	*/
	bool ListenScreenMove(APlayerController* PlayerController, const float& ScreenMoveSpeed);
	/**
	* 获取当前移动状态
	* PlayerController:	玩家的控制
	* @return:			返回当前捕捉到的状态
	*/
	EScreenMoveState CursorMove(const APlayerController* PlayerController);

	/**
	* 使当前的Pawn随着鼠标移动
	* PlayerController:	玩家的控制
	* ScreenMoveState:	移动状态
	* ScreenMoveSpeed:	屏幕移动速度
	* @return:			
	*/
	bool MoveDirection(APlayerController* PlayerController, EScreenMoveState ScreenMoveState, const float &ScreenMoveSpeed);
};