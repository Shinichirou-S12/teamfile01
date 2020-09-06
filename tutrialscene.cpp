//! @file	tutrialscene.cpp
//! @author	kitade mayumi
//! @date	2020-09-05
//! @brief	チュートリアルシーンの実装

// インクルードファイル
#include "main.h"
#include "scene.h"
#include "tutrialscene.h"
#include "bullet.h"
#include "file.h"
#include "playerTest.h"
#include "bullet.h"
#include "item.h"
#include "bg.h"
#include "map01.h"
#include "enemy.h"
#include "enemyBullet.h"
#include "substitute.h"
#include "score.h"
#include "timer.h"
#include "life.h"
#include "effect.h"
#include "sound.h"

// マクロ定義

//グローバル変数

// 初期化処理
void InitTutrialScene(void)
{
	OpenTutrialFile();
	InitBg(TUTRIAL_BG);
	InitMap();
	InitItem(0);
	InitPlayer();
	InitEffect(0);
	InitBullet(0);
	InitEnemy();
	InitEnemyBullet(0);
	InitSubstitute();

	InitLife();
	InitScore();
	InitTimer();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_TUTRIAL);
}

// 終了処理
void UninitTutrialScene(void)
{
	UninitLife();
	UninitScore();
	UninitTimer();

	UninitSubstitute();
	UninitEnemy();
	UninitEnemyBullet();
	UninitBullet();
	UninitEffect();
	UninitMap();
	UninitBg();
	UninitItem();
	UninitPlayer();
	// BGM停止
	StopSound(SOUND_LABEL_BGM_TUTRIAL);

}

// 更新処理
void UpdateTutrialScene(void)
{
	UpdateLife();
	UpdateTimer();
	UpdateScore();

	UpdateSubstitute();
	UpdateEnemyBullet();
	UpdateEnemy();
	UpdateBullet();
	UpdateMap();
	UpdatePlayer();
	UpdateBg();
	UpdateItem();
	UpdateEffect();
}

// 描画処理
void DrawTutrialScene(void)
{
	DrawBg();
	DrawMap();
	DrawScore();
	DrawTimer();
	DrawLife();

	DrawItem();
	DrawEnemy();
	DrawSubstitute();
	DrawPlayer();
	DrawBullet();
	DrawEnemyBullet();
	DrawEffect();
}
