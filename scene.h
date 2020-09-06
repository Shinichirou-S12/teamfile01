//! @file	scene.h
//! @author	まよ
//! @date	2020-07-13
//! @brief	シーンの定義
#pragma once

// インクルードファイル
#include "main.h"

// シーン遷移用列挙型
enum SCENE
{
	SCENE_TITLE,
	SCENE_TUTRIAL,
	SCENE_GAME,
	SCENE_BONUS,
	SCENE_RESULT,
	SCENE_MAX
};

// プロトタイプ宣言

// シーンの初期化
void InitScene(void);

// シーンの終了処理
void UninitScene(void);

// シーンの更新処理
void UpdateScene(void);

// シーンの描画処理
void DrawScene(void);

// シーン切り替え
void SetScene(int scene);

// シーンの取得
int GetScene(void);