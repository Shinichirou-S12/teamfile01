//=============================================================================
//
// スコア処理 [score.h]
// Author : 劉培遠
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

struct LANKING_SCORE
{
	int num;
};


typedef struct					// セーブデータの構造体
{
	// スコアデータを保存
	LANKING_SCORE r_score;	// ランキングスコア
	int		  score;	// プレイ後のスコア
} SAVEDATA;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

LANKING_SCORE *GetResult_Score(void);
void ChangeScore(int value);
int GetScore(void);
#endif
