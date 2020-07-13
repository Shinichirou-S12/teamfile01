//! @file	resultscene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	リザルトシーンの実装

// インクルードファイル
#include "main.h"
#include "result.h"
#include "resultscene.h"

// マクロ定義

// 初期化処理
void InitResultScene(void)
{
	// ロゴと背景の初期化処理
	InitResult(0);
}

// 終了処理
void UninitResultScene(void)
{
	// ロゴと背景の終了処理
	UninitResult();
}

// 更新処理
void UpdateResultScene(void)
{
	// ロゴと背景の更新処理
	UpdateResult();
}

// 描画処理
void DrawResultScene(void)
{
	// ロゴと背景の描画処理
	DrawResult();
}