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

// 初期化処理
void InitGameScene(void)
{
	InitBullet(0);
	InitPlayer();
	InitFile();
}