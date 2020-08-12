//=============================================================================
//
// フェード処理 [fade.h]
// Author : 
//
//=============================================================================
#pragma once

#include "scene.h"

//*************************************
// フェードの状態
//*************************************
enum FADE
{
	FADE_NONE = 0,		// 何もない状態
	FADE_IN,			// フェードイン処理
	FADE_MUSIC_STOP,
	FADE_OUT,			// フェードアウト処理
	FADE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(FADE fade);
void SetFade(FADE fade, SCENE next, int sno );
FADE GetFade(void);

