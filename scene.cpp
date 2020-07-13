//! @file	scene.cpp
//! @author	まよ
//! @date	2020-07-13
//! @brief	シーンの実装

// インクルードファイル
#include "main.h"
#include "scene.h"

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
		break;
	case SCENE_GAME:
		break;
	case SCENE_RESULT:
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
		break;
	case SCENE_GAME:
		break;
	case SCENE_RESULT:
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
		break;
	case SCENE_GAME:
		break;
	case SCENE_RESULT:
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
		break;
	case SCENE_GAME:
		break;
	case SCENE_RESULT:
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

