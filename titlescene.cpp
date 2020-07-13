//! @file	titlescene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	タイトルシーンの実装

// インクルードファイル
#include "main.h"
#include "bg.h"

// マクロ定義

//グローバル変数

// 初期化処理
void InitTitleScene(void)
{
	InitBg();
}

// 終了処理
void UninitScene(void)
{
	UninitBg();
}

// 更新処理
void UpdateScene(void)
{
	UpdateBg();
}

// 描画処理
void DrawScene(void)
{
	DrawBg();
}
