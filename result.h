//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT		_T("data/TEXTURE/ResultTexture/bg001.jpg")				// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO	_T("data/TEXTURE/ResultTexture/result_logo.png")		// 読み込むテクスチャファイル名
#define	RESULT_POS_X			(0)							// タイトルの表示位置
#define	RESULT_POS_Y			(0)							// タイトルの表示位置
#define	RESULT_SIZE_X			(800)						// タイトルの幅
#define	RESULT_SIZE_Y			(600)						// タイトルの高さ
#define	RESULTLOGO_POS_X		(160)						// タイトルロゴの表示位置
#define	RESULTLOGO_POS_Y		(200)						// タイトルロゴの表示位置
#define	RESULTLOGO_SIZE_X		(480)						// タイトルロゴの幅
#define	RESULTLOGO_SIZE_Y		(80)						// タイトルロゴの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

