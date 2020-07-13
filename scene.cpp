//! @file	scene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	シーンの実装

// インクルードファイル
#include "main.h"
#include "scene.h"
#include "titlescene.h"
#include "gamescene.h"
#include "resultscene.h"

// マクロ定義
#define SCENE_START	(SCENE_TITLE)
 
// グローバル変数
int g_scene = SCENE_START;

// 初期化処理
void InitScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		InitTitleScene();
		break;
	case SCENE_GAME:
		InitGameScene();
		break;
	case SCENE_RESULT:
		InitResultScene();
		break;
	default:
		break;
	}
}

// 終了処理
void UninitScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		UninitTitleScene();
		break;
	case SCENE_GAME:
		UninitGameScene();
		break;
	case SCENE_RESULT:
		UninitResultScene();
		break;
	default:
		break;
	}
}

// 更新処理
void UpdateScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		UpdateTitleScene();
		break;
	case SCENE_GAME:
		UpdateGameScene();
		break;
	case SCENE_RESULT:
		UpdateResultScene();
		break;
	default:
		break;
	}
}

// 描画処理
void DrawScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		DrawTitleScene();
		break;
	case SCENE_GAME:
		DrawGameScene();
		break;
	case SCENE_RESULT:
		DrawResultScene();
		break;
	default:
		break;
	}
}

// シーン切り替え
void SetScene(int scene)
{
	UninitScene();

	g_scene = scene;
	
	InitScene();
}

// シーンの取得
int GetScene(void)
{
	return g_scene;
}

