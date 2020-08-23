//! @file	wall.cpp
//! @author	kitade mayumi
//! @date	2020-08-07
//! @brief	壁の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "wall.h"
#include "checkhit.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_WALL	"data/TEXTURE/wall04.png"
#define WALL_SPEED	(0.1f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakevertexWall(void);
void SettextureWall(VERTEX_2D *Vtx);
void SetVertexWall(VERTEX_2D *Vtx);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;	// テクスチャへのポリゴン
static WALL g_wall;										// 壁の格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_WALL,						// ファイルの名前
		&g_pD3DTexture);					// 読み込むメモリー

	g_wall.use = true;
	g_wall.pos = D3DXVECTOR3(-(WALL_TEXTURE_SIZEX / 4.0f), WALL_TEXTURE_SIZEY / 2.0f, 0.0f);
	g_wall.speed = WALL_SPEED;
	g_wall.Texture = g_pD3DTexture;

	MakevertexWall();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
	if (g_pD3DTexture != NULL)
	{
		g_pD3DTexture->Release();
		g_pD3DTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{
	g_wall.pos.x += g_wall.speed;

	SetVertexWall(g_wall.vertexWk);

	CheckHitWall();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの作成
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_wall.use == true)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_wall.Texture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_wall.vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakevertexWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	g_wall.vertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_wall.vertexWk[1].vtx = D3DXVECTOR3(WALL_TEXTURE_SIZEX, 0.0f, 0.0f);
	g_wall.vertexWk[2].vtx = D3DXVECTOR3(0.0f, WALL_TEXTURE_SIZEY, 0.0f);
	g_wall.vertexWk[3].vtx = D3DXVECTOR3(WALL_TEXTURE_SIZEX, WALL_TEXTURE_SIZEY, 0.0f);

	// rhwの設定
	g_wall.vertexWk[0].rhw = 1.0f;
	g_wall.vertexWk[1].rhw = 1.0f;
	g_wall.vertexWk[2].rhw = 1.0f;
	g_wall.vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	g_wall.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_wall.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_wall.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_wall.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	SettextureWall(g_wall.vertexWk);
	return S_OK;
}

//=============================================================================
// テクスチャ座標設定
//=============================================================================
void SettextureWall(VERTEX_2D *Vtx)
{
	Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexWall(VERTEX_2D *Vtx)
{
	Vtx[0].vtx.x = -WALL_TEXTURE_SIZEX + g_wall.pos.x;
	Vtx[1].vtx.x = 0.0f + g_wall.pos.x;
	Vtx[2].vtx.x = -WALL_TEXTURE_SIZEX + g_wall.pos.x;
	Vtx[3].vtx.x = 0.0f + g_wall.pos.x;
}

//=============================================================================
// 壁のアドレス取得
//=============================================================================
WALL *GetWall(void)
{
	return &g_wall;
}