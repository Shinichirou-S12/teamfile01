//! @file	bg.cpp
//! @author	まよ
//! @date	2020-06-19
//! @brief	タイトルの実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "bg.h"
#include "scene.h"
#include "controller.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "playerTest.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BG_TITLE		_T("data/TEXTURE/background/title01.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_BG_GAME		_T("data/TEXTURE/background/bg01.png")				// 読み込むテクスチャファイル名
#define	TEXTURE_BG_BONUS		_T("data/TEXTURE/background/bg03.png")			// 読み込むテクスチャファイル名

#define	TEXTURE_TITLE_LOGO	_T("data/TEXTURE/titlelogo.png")					// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO	_T("data/TEXTURE/ResultTexture/result_logo.png")	// 読み込むテクスチャファイル名
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBg(void);		// タイトルの頂点情報作成
void SetTextureBg(void);		// タイトルのテクスチャの設定

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBG = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureBGLogo = NULL;	// テクスチャへのポインタ

VERTEX_2D				g_vertexWkTitle[NUM_VERTEX];	// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTitleLogo[NUM_VERTEX];// 頂点情報格納ワーク
static float			g_scrollspeed;					// テクスチャのスクロール速度

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBg(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	switch(type)
	{
	case TITLE_BG:
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_BG_TITLE,							// ファイルの名前
			&g_pD3DTextureBG);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_LOGO,					// ファイルの名前
			&g_pD3DTextureBGLogo);			// 読み込むメモリー
		break;

	case GAME_BG:
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_BG_GAME,							// ファイルの名前
			&g_pD3DTextureBG);				// 読み込むメモリー
		break;

	case BONUS_BG:
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_BG_BONUS,					// ファイルの名前
			&g_pD3DTextureBG);				// 読み込むメモリー
		break;

	case RESULT_BG:
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_BG_BONUS,					// ファイルの名前
			&g_pD3DTextureBG);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_RESULT_LOGO,				// ファイルの名前
			&g_pD3DTextureBGLogo);				// 読み込むメモリー
		break;
	}

	// 頂点情報の作成
	MakeVertexBg();

	g_scrollspeed = 0.0f;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBg(void)
{
	if(g_pD3DTextureBG != NULL)
	{// テクスチャの開放
		g_pD3DTextureBG->Release();
		g_pD3DTextureBG = NULL;
	}

	if(g_pD3DTextureBGLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureBGLogo->Release();
		g_pD3DTextureBGLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBg(void)
{
	PLAYER *player = GetPlayer();
	int scene = GetScene();

	if (scene != SCENE_GAME)
	{
		g_scrollspeed += 0.002f;
	}
	else
	{
		if (player->direction == Right && (GetInput(RIGHTMOVE)))
		{
			g_scrollspeed += player->moveSpeed / 1000.0f;
		}
		else if(player->direction == Left && (GetInput(LEFTMOVE)))
		{
			g_scrollspeed -= player->moveSpeed / 1000.0f;
		}
	}

	SetTextureBg();

	if (GetInput(STARTBUTTON)&& scene == SCENE_TITLE)
	{
		SetFade(FADE_OUT, SCENE_GAME, SOUND_LABEL_BGM_TITLE);
		PlaySound(SOUND_LABEL_SE_STARTBUTTON);
		player->warpUse = false;
		player->scrollPos = D3DXVECTOR3(/*i*200.0f + */200.0f, 300.0f, 0.0f);// 座標データを初期化
		player->partsState = PERFECT;
		player->hp = PLAYER_HP;									// HPの初期化
	}
	if (scene == SCENE_RESULT)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_BOSS_STAGE);

		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_BOSS_STAGE);
		}
	}

	else
	{
		return;
	}

	//if(GetKeyboardTrigger(DIK_RETURN))
	//{// Enter押したら、ステージを切り替える
	//	SetStage(STAGE_GAME);
	//}
	//// ゲームパッドでで移動処理
	//else if (IsButtonTriggered(0, BUTTON_START))
	//{
	//	SetStage(STAGE_GAME);
	//}
	//else if (IsButtonTriggered(0, BUTTON_B))
	//{
	//	SetStage(STAGE_GAME);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int scene = GetScene();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureBG);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle, sizeof(VERTEX_2D));

	if (scene == SCENE_TITLE 
		|| scene == SCENE_RESULT)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureBGLogo);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleLogo, sizeof(VERTEX_2D));
	}
}

/**************************************************************************//**
	@brief		MakeVertexBg
	@brief		タイトルの頂点座標作成
	@param		なし
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT MakeVertexBg(void)
{
	int scene = GetScene();
	// 頂点座標の設定
	g_vertexWkTitle[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[1].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);
	g_vertexWkTitle[3].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitle[0].rhw =
	g_vertexWkTitle[1].rhw =
	g_vertexWkTitle[2].rhw =
	g_vertexWkTitle[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	if (scene == SCENE_TITLE)
	{
		// 頂点座標の設定
		g_vertexWkTitleLogo[0].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y, 0.0f);
		g_vertexWkTitleLogo[1].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y, 0.0f);
		g_vertexWkTitleLogo[2].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);
		g_vertexWkTitleLogo[3].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);
	}

	if (scene == SCENE_RESULT)
	{
		// 頂点座標の設定
		g_vertexWkTitleLogo[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
		g_vertexWkTitleLogo[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
		g_vertexWkTitleLogo[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
		g_vertexWkTitleLogo[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	}

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleLogo[0].rhw =
	g_vertexWkTitleLogo[1].rhw =
	g_vertexWkTitleLogo[2].rhw =
	g_vertexWkTitleLogo[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitleLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

/**************************************************************************//**
	@brief		SetTextureBg
	@brief		タイトルテクスチャの座標設定
	@param		なし
	@return		なし
*//***************************************************************************/
void SetTextureBg(void)
{
	g_vertexWkTitle[0].tex = D3DXVECTOR2(0.0f + g_scrollspeed, 0.0f);
	g_vertexWkTitle[1].tex = D3DXVECTOR2(0.5f + g_scrollspeed, 0.0f);
	g_vertexWkTitle[2].tex = D3DXVECTOR2(0.0f + g_scrollspeed, 1.0f);
	g_vertexWkTitle[3].tex = D3DXVECTOR2(0.5f + g_scrollspeed, 1.0f);
}