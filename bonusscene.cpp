//! @file	bonusscene.cpp
//! @author	kitade mayumi
//! @date	2020-08-09
//! @brief	ボーナスシーンの実装

// インクルードファイル
#include "main.h"
#include "scene.h"
#include "bonusscene.h"
#include "file.h"
#include "playerTest.h"
#include "item.h"
#include "bg.h"
#include "map01.h"
#include "score.h"
#include "timer.h"
#include "life.h"
#include "effect.h"
#include "sound.h"

// シーンの初期化処理
void InitBonusScene(void)
{
	OpenBonusFile();
	InitBg(BONUS_BG);
	InitMap();
	InitItem(0);
	InitPlayer();
	InitEffect(0);
	PlaySound(SOUND_LABEL_BGM_BOSS_STAGE);
}

// シーンの終了処理
void UninitBonusScene(void)
{
	UninitEffect();
	UninitMap();
	UninitBg();
	UninitItem();
	UninitPlayer();
	StopSound(SOUND_LABEL_BGM_BOSS_STAGE);
}

// シーンの更新処理
void UpdateBonusScene(void)
{
	UpdateMap();
	UpdatePlayer();
	UpdateBg();
	UpdateItem();
	UpdateEffect();
	UpdateScore();
	UpdateLife();
	UpdateTimer();
}

// シーンの描画処理
void DrawBonusScene(void)
{
	DrawBg();
	DrawMap();
	DrawScore();
	DrawLife();
	DrawTimer();
	DrawItem();
	DrawPlayer();
	DrawEffect();
}
