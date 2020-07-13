//! @file	gamescene.h
//! @author	まよ
//! @date	2020-07-13
//! @brief	ゲームシーンの定義

#pragma once

#include "main.h"

// プロトタイプ宣言

// ゲームシーンの初期化処理
void InitGameScene(void);

// ゲームシーンの終了処理
void UninitGameScene(void);

// ゲームシーンの更新処理
void UpdateGameScene(void);

// ゲームシーンの描画処理
void DrawGameScene(void);