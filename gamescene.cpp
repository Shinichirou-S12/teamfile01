//! @file	gamescene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	ゲームシーンの実装

// インクルードファイル
#include "main.h"
#include "bullet.h"
#include "playerTest.h"
#include "file.h"
#include "life.h"
#include "score.h"
#include "timer.h"

// マクロ定義

// シーンの初期化処理
void InitGameScene(void)
{
	// 弾の初期化処理
	InitBullet(0);

	// プレイヤーの初期化処理
	InitPlayer();

	// ファイルの読み込み
	InitFile();

	// マップの初期化処理
	InitMap();

	// ライフの初期化処理
	InitLife();

	// スコアの初期化処理
	InitScore();

	// タイマーの初期化処理
	InitTimer();
}

// シーンの終了処理
void UninitGameScene(void)
{
	// 弾の終了処理
	UninitBullet();

	// プレイヤーの終了処理
	UninitPlayer();

	// マップの終了処理
	UninitMap();

	// ライフの終了処理
	UninitLife();

	// スコアの終了処理
	UninitScore();

	// タイマーの終了処理
	UninitTimer();
}

// シーンの更新処理
void UpdateGameScene(void)
{
	// マップの更新処理
	UpdateMap();

	// プレイヤーの更新処理
	UpdatePlayer();

	// 弾の更新処理
	UpdateBullet();

	// ライフの更新処理
	UpdateLife();

	// タイマーの更新処理
	UpdateTimer();

	// スコアの更新処理
	UpdateScore();
}

// シーンの描画処理
void DrawGameScene(void)
{
	// マップの描画処理
	DrawMap();

	// スコアの描画処理
	DrawScore();

	// タイマーの描画処理
	DrawTimer();

	// ライフの描画処理
	DrawLife();

	// プレイヤーの描画処理
	DrawPlayer();

	// バレットの描画処理
	DrawBullet();
}