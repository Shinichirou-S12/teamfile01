//=============================================================================
//
// タイマー処理 [timer.h]
// Author : 劉培遠
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

// 構造体定義

// timeの増減に使う構造体
struct TIME
{
	D3DXCOLOR	color;		// 色、増やすは青、減らすは赤

	int			time;		// 変化する値
	bool		use;		// 使用中かどうか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void EnableTimer(bool bEnable);
void ResetTimer(int nTime = 999);

void ChangeTimer(int value);

#endif
