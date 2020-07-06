//=============================================================================
//
// スコア処理 [score.h]
// Author : 
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

// 構造体定義

// scoreの増減に使う構造体
struct SCORE
{
	D3DXCOLOR	color;		// 色、増やすは青、減らすは赤

	int			score;		// 変化する値
	bool		use;		// 使用中かどうか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void ChangeScore(int value);

#endif
