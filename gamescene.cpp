//! @file	gamescene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	ゲームシーンの実装

// インクルードファイル
#include "main.h"
#include "bullet.h"
#include "playerTest.h"
#include "file.h"
#include "map01.h"
#include "life.h"
#include "score.h"
#include "timer.h"
#include "enemy.h"
#include "item.h"
#include "bg.h"
#include "effect.h"
#include "enemyBullet.h"

// マクロ定義

// シーンの初期化処理
void InitGameScene(void)
{
	// 弾の初期化処理
	InitBullet(0);

	// エネミー用の弾の初期化処理
	InitEnemyBullet(0);

	// プレイヤーの初期化処理
	InitPlayer();

	// ファイルの読み込み
	OpenFile();

	// マップの初期化処理
	InitMap();

	// 背景の初期化処理
	InitBg();

	// アイテムの初期化処理
	InitItem(0);

	// エネミーの初期化処理
	InitEnemy();

	// ライフの初期化処理
	InitLife();

	// スコアの初期化処理
	InitScore();

	// タイマーの初期化処理
	InitTimer();

	// エフェクトの初期化処理
	InitEffect(0);
}

// シーンの終了処理
void UninitGameScene(void)
{
	// エフェクトの終了処理
	UninitEffect();

	// エネミー用の弾の終了処理
	UninitEnemyBullet();

	// 弾の終了処理
	UninitBullet();
	
	// プレイヤーの終了処理
	UninitPlayer();

	// エネミーの終了処理
	UninitEnemy();

	// 背景の終了処理
	UninitBg();

	// マップの終了処理
	UninitMap();

	// ライフの終了処理
	UninitLife();

	// アイテムの終了処理
	UninitItem();
	//// スコアの終了処理
	//UninitScore();

	// タイマーの終了処理
	UninitTimer();
}

// シーンの更新処理
void UpdateGameScene(void)
{
	// マップの更新処理
	UpdateMap();

	// 背景の更新処理
	UpdateBg();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	// アイテムの更新処理
	UpdateItem();

	// 弾の更新処理
	UpdateBullet();

	// エネミー用の弾の更新処理
	UpdateEnemyBullet();

	// エフェクトの更新処理
	UpdateEffect();

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
	// 背景の描画処理
	DrawBg();

	// マップの描画処理
	DrawMap();

	// スコアの描画処理
	DrawScore();

	// タイマーの描画処理
	DrawTimer();

	// ライフの描画処理
	DrawLife();

	// アイテムの描画処理
	DrawItem();

	// エネミーの描画処理
	DrawEnemy();

	// プレイヤーの描画処理
	DrawPlayer();

	// バレットの描画処理
	DrawBullet();

	// エネミー用の弾の描画処理
	DrawEnemyBullet();

	// エフェクトの描画処理
	DrawEffect();
}