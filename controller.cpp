//! @file	controller.cpp
//! @author	北出真弓
//! @date	2019-01-07
//! @brief	コントローラーの設定の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "controller.h"
#include "input.h"

//=============================================================================
//コマンド処理
//=============================================================================
bool GetInput(int nCommand)
{
	switch (nCommand)
	{
	case STARTBUTTON:
		return GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_R);
		break;

	case ATTACK:
		return GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_X);
		break;

	case JUMPMOVE:
		return GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_A);
		break;

	case RIGHTMOVE:
		return GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT);
		break;

	case LEFTMOVE:
		return GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, LSTICK_LEFT);
		break;

	case UPMOVE:
		return GetKeyboardPress(DIK_UP) || IsButtonPressed(0, LSTICK_UP);
		break;

	case DOWNMOVE:
		return GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, LSTICK_DOWN);
		break;
	default:
		break;
	}
	return false;
}