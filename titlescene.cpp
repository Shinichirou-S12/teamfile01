//! @file	titlescene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	タイトルシーンの実装

// インクルードファイル
#include "main.h"
#include "bg.h"
#include "savefile.h"
#include "sound.h"

// マクロ定義

//グローバル変数

// 初期化処理
void InitTitleScene(void)
{
	InitBg(TITLE_BG);
	SaveData();
	// BGM再生
	PlaySound(SOUND_LABEL_BGM_TITLE);
}

// 終了処理
void UninitTitleScene(void)
{
	UninitBg();
	// BGM停止
	StopSound(SOUND_LABEL_BGM_TITLE);

}

// 更新処理
void UpdateTitleScene(void)
{
	UpdateBg();
}

// 描画処理
void DrawTitleScene(void)
{
	DrawBg();
}
