//! @file	item.cpp
//! @author	まよ
//! @date	2020-06-19
//! @brief	アイテムの実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "item.h"
#include "bullet.h"
#include "map.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_ITEM_WATERMELON	_T("data/TEXTURE/Item/watermelon.png")	// スイカの画像
#define TEXTURE_GAME_ITEM_APPLE			_T("data/TEXTURE/Item/apple.png")		// リンゴの画像
#define	TEXTURE_GAME_ITEM_GRAPE			_T("data/TEXTURE/Item/grape.png")		// ブドウの画像
#define	TEXTURE_GAME_ITEM_STAR			_T("data/TEXTURE/Item/star02.png")		// スターの画像

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexItem( int no );				// アイテムの頂点情報作成
void SetTextureItem(int no, int cntPattern);	// アイテムの頂点座標の設定
void SetVertexItem(int no);						// アイテムのテクスチャの設定
void SetItem(float speed);
void DeathItem(void);							// アイテムの破棄処理

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureItem[ITEM_KIND_MAX];		// テクスチャへのポリゴン

ITEM					itemWk[ITEM_MAX];			// アイテム構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[0];

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_ITEM_WATERMELON,		// ファイルの名前
			&g_pD3DTextureItem[WATERMELON]);	// 読み込むメモリのポインタ
		
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_ITEM_APPLE,			// ファイルの名前
			&g_pD3DTextureItem[APPLE]);			// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_ITEM_GRAPE,			// ファイルの名前
			&g_pD3DTextureItem[GRAPE]);			// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_ITEM_STAR,				// ファイルの名前
			&g_pD3DTextureItem[STAR]);			// 読み込むメモリのポインタ

	}

	// アイテムの初期化処理
	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		item->use = false;									// 使用
		item->delete_use = false;							// 未使用			
		item->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		item->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		item->CountAnim = 0;								// アニメカウントを初期化
		item->Texturenum = rand() % 3;						// テクスチャー種類をランダムで初期化
		item->speed = 0;									// アイテムスピードの初期化
		item->type = 0;										// アイテムの種類
		item->point = ITEM_POINT_HERB;
		
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ITEM_SIZE_X, TEXTURE_ITEM_SIZE_Y);
		item->Radius = D3DXVec2Length(&temp);				// アイテムの半径を初期化
		item->BaseAngle = atan2f(TEXTURE_ITEM_SIZE_Y, TEXTURE_ITEM_SIZE_X);	// アイテムの角度を初期化

		if (item->Texturenum != STAR)
		{
			item->Texture = g_pD3DTextureItem[item->Texturenum];		// テクスチャ情報
		}
		MakeVertexItem(i);												// 頂点情報の作成
	}

	 SetItem(0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	ITEM *item = &itemWk[0];

	for (int i = 0; i < ITEM_KIND_MAX; i++)
	{
		if (g_pD3DTextureItem[i] != NULL)
		{	// テクスチャの開放
			g_pD3DTextureItem[i]->Release(); // 問題がある項目
			g_pD3DTextureItem[i] = NULL;
		}
	}
	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		item->use = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	ITEM *item = &itemWk[0];

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->delete_use == true)
		{
			if (item->Radius < 0.0f)
			{
				item->delete_use = false;
			}

			item->rot.z += 1.0f;
			item->Radius -= 1.0f;

			// テクスチャ座標を設定
			SetTextureItem(i, item->PatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexItem(i);
		}
	

		if (item->use == true)			// 使用している状態なら更新する
		{
			item->pos.x += item->speed;
			// テクスチャ座標を設定
			SetTextureItem(i, item->PatternAnim);

			// 移動後の座標で頂点を設定
			SetVertexItem(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use == true || item->delete_use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, item->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_ITEM, item->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// アイテムの配置関数
//=============================================================================
void SetItem(D3DXVECTOR3 pos)
{
	MAP * map = GetMapData();
	ITEM *item = &itemWk[0];

	for (int i = 0; i < MAP_MAXDATA * SIZE_X * SIZE_Y; i++, map++)
	{
		for (int j = 0; j < ITEM_MAX; j++, item++)
		{
			if (map->type == BLOCK2)
			{
				item->pos = pos;
			}
		}
	}
}

//=============================================================================
// アイテムの破棄設定
//=============================================================================
void DeathItem(void)
{
	ITEM *item = &itemWk[0];
	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->pos.x <= 0.0f)
		{
			item->use = false;
		}
	}
}

/**************************************************************************//**
	@brief		MakeVertexItem
	@brief		アイテムの頂点座標作成
	@param[in]	no		アイテムの番号
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT MakeVertexItem(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[no];

	// 頂点座標の設定
	SetVertexItem(no);

	// rhwの設定
	item->vertexWk[0].rhw =
	item->vertexWk[1].rhw =
	item->vertexWk[2].rhw =
	item->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	item->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	if (item->type != STAR)
	{
		// テクスチャ座標の設定
		item->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		item->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 0.0f);
		item->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
		item->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	}
	else
	{
		item->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		item->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		item->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		item->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

/**************************************************************************//**
	@brief		SetTextureItem
	@brief		アイテムのテクスチャ設定
	@param[in]	no		アイテムの番号
	@return		なし
*//***************************************************************************/
void SetTextureItem( int no, int cntPattern )
{
	ITEM *item = &itemWk[no];
	
	// テクスチャ座標の設定
	int x = cntPattern ;
	int y = cntPattern ;
	float sizeX = 1.0f ;
	float sizeY = 1.0f ;
	
	item->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	item->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	item->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	item->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}
/**************************************************************************//**
	@brief		SetVertexItem
	@brief		アイテムの座標設定
	@param[in]	no		アイテムの番号
	@return		なし
*//***************************************************************************/
void SetVertexItem(int no)
{
	ITEM *item = &itemWk[no];
	
	// 頂点座標の設定
	item->vertexWk[0].vtx.x = item->pos.x - cosf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[0].vtx.y = item->pos.y - sinf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[0].vtx.z = 0.0f;

	item->vertexWk[1].vtx.x = item->pos.x + cosf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[1].vtx.y = item->pos.y - sinf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[1].vtx.z = 0.0f;

	item->vertexWk[2].vtx.x = item->pos.x - cosf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[2].vtx.y = item->pos.y + sinf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[2].vtx.z = 0.0f;

	item->vertexWk[3].vtx.x = item->pos.x + cosf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[3].vtx.y = item->pos.y + sinf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// アイテム取得関数
//=============================================================================
ITEM *GetItem(int no)
{
	return(&itemWk[no]);
}

//============================================================================
// アイテムの設定
//============================================================================
void SetItem(float speed)
{
	ITEM *item = GetItem(0);
	MAP *map = GetMapData();

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (!item->use)
		{
			item->Texturenum = rand() % 3;						//テクスチャー種類をランダムで初期化

			if (item->Texturenum == 2)							//テクスチャーナンバーが２なら
			{
				item->grape_use = true;							//ブドウを使用に
			}
			D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ITEM_SIZE_X, TEXTURE_ITEM_SIZE_Y);
			item->Radius = D3DXVec2Length(&temp);				// アイテムの半径を初期化
			item->rot.z = 0.0f;
			
			item->speed = speed;
			item->delete_use = false;
		}
	}

	ITEM *items = GetItem(0);

	for (int j = 0; j < MAP_MAXDATA * SIZE_X * SIZE_Y; j++, map++)
	{
		if (map->type == BLOCK2)
		{
			items->use = true;
			items->pos = map->pos;
			items++;
		}
		if (map->type == ITEM17)
		{
			items->use = true;
			items->pos = map->pos;
			items->type = STAR;
			items->Texture = g_pD3DTextureItem[STAR];
			items++;
		}
	}

}
