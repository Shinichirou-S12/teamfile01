//=============================================================================
//
// ライフ処理 [life.h]
// Author : 劉培遠
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"

enum CHANGE_LIFE
{
	ADD,
	MINUS,
	NONE,
	MAX_CHANGE_LIFE
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);

//void ChangeLife(int value)
//引数:０以外の整数（1や-1）
//効果:引数に合わせてlifeの増減をする（UIのみ）
void ChangeLife(int value);

#endif
