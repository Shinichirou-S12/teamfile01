//! @file	resultscene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	リザルトシーンの実装

// インクルードファイル
#include "main.h"
#include "savefile.h"
#include "resultscene.h"
#include "result_score.h"
#include "score.h"
#include "sound.h"
#include "bg.h"

// マクロ定義

// 初期化処理
void InitResultScene(void)
{
	// ロゴと背景の初期化処理
	InitBg(RESULT_BG);

	//セーブデータからランキングを引っ張り出す
	LoadData();

	// スコア
	InitResult_Score(0);

	// サウンドの開始
	PlaySound(SOUND_LABEL_BGM_RESULT);
}

// 終了処理
void UninitResultScene(void)
{
	// ロゴと背景の終了処理
	UninitBg();

	UninitResult_Score();

	UninitScore();

	// サウンドの終了処理
	StopSound(SOUND_LABEL_BGM_RESULT);

}

// 更新処理
void UpdateResultScene(void)
{
	// ロゴと背景の更新処理
	UpdateBg();

	// リザルトスコアの更新処理
	UpdateResult_Score();
}

// 描画処理
void DrawResultScene(void)
{
	// ロゴと背景の描画処理
	DrawBg();

	// リザルトスコアの描画処理
	DrawResult_Score();
}