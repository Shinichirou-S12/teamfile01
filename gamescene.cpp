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
#include "wall.h"
#include "killer.h"
#include "sound.h"
#include "spear.h"
#include "substitute.h"
#include "boss.h"

// マクロ定義

// シーンの初期化処理
void InitGameScene(void)
{
	PLAYER *player = GetPlayer();

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
	InitBg(GAME_BG);

	// ボスの初期化処理
	InitBoss();

	// アイテムの初期化処理
	InitItem(0);

	// エネミーの初期化処理
	InitEnemy();

	// 身代わりアイテムの初期化処理
	InitSubstitute();

	// 壁の初期化処理
	InitWall();

	if (!player->warpUse)
	{
		// ライフの初期化処理
		InitLife();

		// スコアの初期化処理
		InitScore();

		// タイマーの初期化処理
		InitTimer();
	}

	// 針の初期化処理
	InitSpear(0);

	// エフェクトの初期化処理
	InitEffect(0);

	// キラーの初期化処理
	InitKiller(0);

	// サウンドの開始
	PlaySound(SOUND_LABEL_BGM_GAMESTAGE);
}

// シーンの終了処理
void UninitGameScene(void)
{
	PLAYER *player = GetPlayer();

	// キラーの終了処理
	UninitKiller();

	// エフェクトの終了処理
	UninitEffect();

	// 針の終了処理
	UninitSpear();

	// エネミー用の弾の終了処理
	UninitEnemyBullet();

	// 弾の終了処理
	UninitBullet();
	
	// プレイヤーの終了処理
	UninitPlayer();

	// 身代わりアイテムの終了処理
	UninitSubstitute();

	// エネミーの終了処理
	UninitEnemy();

	// 背景の終了処理
	UninitBg();

	// マップの終了処理
	UninitMap();

	// 壁の終了処理
	UninitWall();

	// アイテムの終了処理
	UninitItem();

	// ボスの終了処理
	UninitBoss();

	if (!player->warpUse)
	{
		// ライフの終了処理
		UninitLife();

		//// スコアの終了処理
		//UninitScore();

		// タイマーの終了処理
		UninitTimer();
	}

	// サウンドの終了処理
	StopSound(SOUND_LABEL_BGM_GAMESTAGE);

}

// シーンの更新処理
void UpdateGameScene(void)
{
	// マップの更新処理
	UpdateMap();

	// 背景の更新処理
	UpdateBg();

	// キラーの更新処理
	UpdateKiller();

	// プレイヤーの更新処理
	UpdatePlayer();

	// 身代わりアイテムの更新処理
	UpdateSubstitute();

	// 壁の更新処理
	UpdateWall();

	// 針の更新処理
	UpdateSpear();

	// エネミーの更新処理
	UpdateEnemy();

	// エネミー用の弾の更新処理
	UpdateEnemyBullet();

	// ボスの更新処理
	UpdateBoss();

	// アイテムの更新処理
	UpdateItem();

	// 弾の更新処理
	UpdateBullet();

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

	// 針の描画処理
	DrawSpear();

	// マップの描画処理
	DrawMap();

	// キラーの描画処理
	DrawKiller();

	// 壁の描画処理
	DrawWall();

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

	// ボスの描画処理
	DrawBoss();

	// 身代わりアイテム描画処理
	DrawSubstitute();

	// プレイヤーの描画処理
	DrawPlayer();

	// バレットの描画処理
	DrawBullet();

	// エネミー用の弾の描画処理
	DrawEnemyBullet();

	// エフェクトの描画処理
	DrawEffect();
}