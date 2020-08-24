//! @file	spear.cpp
//! @author	kitade mayumi
//! @date	2020-08-20
//! @brief	針の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "spear.h"
#include "map.h"
#include "checkhit.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_SPEAR		_T("data/TEXTURE/spears/spear01.png")			// 針の画像
#define POP_COUNT_SPEAR	(60 * 3)

// 針の向いている方向
enum SPEAR_TYPE
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSpear( int no );						// 針の頂点情報作成
void SetTextureSpear(int no, int cntPattern, int direction);// 針の頂点座標の設定
void SetVertexSpear(int no);							// 針のテクスチャzの設定
void MovingDirectionSpear(int no, D3DXVECTOR3 *vec);	// 針の移動処理
void SetSpear(void);
void MoveSpear(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureSpear = NULL;		// テクスチャへのポリゴン

static SPEAR					g_spear[SPEAR_MAX];			// 針の構造体
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSpear(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPEAR *spear = &g_spear[0];		// 針のポインターを初期化

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_SPEAR,			// ファイルの名前
			&g_pD3DTextureSpear);			// 読み込むメモリのポインタ
		
	}

	// バレットの初期化処理
	for (int i = 0; i < SPEAR_MAX; i++, spear++)
	{
		spear->use = false;									// 未使用（発射されていない弾）
		spear->downUse = false;
		spear->popUse = false;
		spear->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 座標データを初期化
		spear->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		spear->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量のデータを初期化
		spear->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ベクトルの初期化

		spear->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		spear->CountAnim = 0;									// アニメカウントを初期化
		spear->speed = 0.2f;									// 移動スピード
		spear->direction = LEFT;
		spear->MoveCnt = 0;
		spear->PopCnt = 0;

		spear->Texture = g_pD3DTextureSpear;				// テクスチャ情報
		MakeVertexSpear(i);									// 頂点情報の作成
	}

	SetSpear();

	for (int i = 0; i < SPEAR_MAX; i++, spear++)
	{
		MovingDirectionSpear(i, &spear[i].vec);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSpear(void)
{
	SPEAR *spear = g_spear;				// 針のポインターを初期化

	if (g_pD3DTextureSpear != NULL)
	{	// テクスチャの開放
		g_pD3DTextureSpear->Release();
		g_pD3DTextureSpear = NULL;
	}
	for (int i = 0; i < SPEAR_MAX; i++, spear++)
	{
		spear->use = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSpear(void)
{
	SPEAR *spear = g_spear;				// 針のポインターを初期化
	int scene = GetScene();

	for (int i = 0; i < SPEAR_MAX; i++, spear++)
	{
		if (spear->use == true)			// 使用している状態なら更新する
		{
			// アニメーション  
			spear->CountAnim++;
			if ((spear->CountAnim % TIME_ANIMATION_SPEAR) == 0)
			{
				// パターンの切り替え
				spear->PatternAnim = (spear->PatternAnim + 1) % ANIM_PATTERN_NUM_SPEAR;
			}

			MoveSpear(i);
			// 画面外まで進んだ？
			if (spear->pos.x < 0.0f
				|| spear->pos.x >=SCREEN_WIDTH)	// 自分の大きさを考慮して画面外か判定している
			{
				spear->use = false;
			}

			SetTextureSpear(i, spear->PatternAnim, spear->direction);	// アニメーション後のテクスチャの設定

			SetVertexSpear(i);							// 移動後の座標で頂点を設定
		}

	}
}

//=============================================================================
// 針の移動処理
//=============================================================================
void MoveSpear(int no)
{
	SPEAR *spear = &g_spear[no];		// 針のポインターを初期化

	// 針の向いているベクトルの方向に対して、移動する
	if (!spear->downUse && !spear->popUse)
	{
		spear->move += spear->vec * spear->speed;
		spear->MoveCnt++;

		if (spear->MoveCnt == MAP_TEXTURE_SIZE_X)
		{
			spear->MoveCnt = 0;
			spear->downUse = true;
			spear->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量のデータを初期化
		}
	}

	else if(spear->downUse && !spear->popUse)
	{
		spear->move += spear->vec * spear->speed * -1;
		spear->MoveCnt++;

		if (spear->MoveCnt == MAP_TEXTURE_SIZE_X)
		{
			spear->MoveCnt = 0;
			spear->popUse = true;
			spear->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量のデータを初期化
		}

	}

	if (spear->popUse)
	{
		spear->PopCnt++;

		if (spear->PopCnt == POP_COUNT_SPEAR)
		{
			spear->downUse = false;
			spear->popUse = false;
			spear->PopCnt = 0;
		}
	}


	D3DXVec3Add(&spear->pos, &spear->pos, &spear->move);

}

//=============================================================================
// 移動ベクトル方向処理
//=============================================================================
void MovingDirectionSpear(int no, D3DXVECTOR3 *vec)
{
	SPEAR *spear = &g_spear[no];		// 針のポインターを初期化
	D3DXVECTOR3 vec2;

	// 針の向いている方向にあわせてベクトルの向きを合わせる
	switch (spear->direction)
	{
	case UP:
		vec2 = D3DXVECTOR3(spear->pos.x, 0.0f, 0.0f);
		D3DXVec3Subtract(&spear->vec, &vec2, &spear->pos);
		break;

	case DOWN:
		vec2 = D3DXVECTOR3(spear->pos.x, SCREEN_HEIGHT, 0.0f);
		D3DXVec3Subtract(&spear->vec, &vec2, &spear->pos);
		break;

	case RIGHT:
		vec2 = D3DXVECTOR3(SCREEN_WIDTH, spear->pos.y, 0.0f);
		D3DXVec3Subtract(&spear->vec, &vec2, &spear->pos);
		break;

	case LEFT:
		vec2 = D3DXVECTOR3(0.0f, spear->pos.y, 0.0f);
		D3DXVec3Subtract(&spear->vec, &vec2, &spear->pos);
		break;
	default:
		break;
	}
	D3DXVec3Normalize(&spear->vec, &spear->vec);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSpear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPEAR *spear = &g_spear[0];			// 針のポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SPEAR_MAX; i++, spear++)
	{
		if (spear->use == true)			// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, spear->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SPEAR, spear->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

/**************************************************************************//**
	@brief		MakeVertexSpear
	@brief		針の頂点座標作成
	@param[in]	no		針の番号
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT MakeVertexSpear( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPEAR *spear = &g_spear[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	SetVertexSpear( no );

	// rhwの設定
	spear->vertexWk[0].rhw =
	spear->vertexWk[1].rhw =
	spear->vertexWk[2].rhw =
	spear->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	spear->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spear->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spear->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spear->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	spear->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	spear->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SPEAR, 0.0f);
	spear->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SPEAR);
	spear->vertexWk[3].tex = D3DXVECTOR2( 1.0f/ TEXTURE_PATTERN_DIVIDE_X_SPEAR, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_SPEAR);

	return S_OK;
}

/**************************************************************************//**
	@brief		SetTextureSpear
	@brief		針のテクスチャの座標設定
	@param[in]	no				針の番号
	@param[in]	cntPattern		針のアニメーションカウントパターン
	@param[in]	direction		針の向き
	@return		なし
*//***************************************************************************/
void SetTextureSpear( int no, int cntPattern, int direction)
{
	SPEAR *spear = &g_spear[no];			// バレットのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_SPEAR;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_SPEAR;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_SPEAR;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SPEAR;
	
	switch (direction)
	{
	case UP:
		spear->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		spear->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		spear->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		spear->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case DOWN:
		spear->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		spear->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		spear->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		spear->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case RIGHT:
		spear->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		spear->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		spear->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		spear->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case LEFT:
		spear->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		spear->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		spear->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		spear->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;
	}
}

/**************************************************************************//**
	@brief		SetVertexSpear
	@brief		針の頂点座標設定
	@param[in]	no		針の番号
	@return		なし
*//***************************************************************************/
void SetVertexSpear( int no )
{
	SPEAR *spear = &g_spear[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	spear->vertexWk[0].vtx.x = spear->pos.x - TEXTURE_SPEAR_SIZE_X;
	spear->vertexWk[0].vtx.y = spear->pos.y - TEXTURE_SPEAR_SIZE_Y;
	spear->vertexWk[0].vtx.z = 0.0f;

	spear->vertexWk[1].vtx.x = spear->pos.x + TEXTURE_SPEAR_SIZE_X;
	spear->vertexWk[1].vtx.y = spear->pos.y - TEXTURE_SPEAR_SIZE_Y;
	spear->vertexWk[1].vtx.z = 0.0f;

	spear->vertexWk[2].vtx.x = spear->pos.x - TEXTURE_SPEAR_SIZE_X;
	spear->vertexWk[2].vtx.y = spear->pos.y + TEXTURE_SPEAR_SIZE_Y;
	spear->vertexWk[2].vtx.z = 0.0f;

	spear->vertexWk[3].vtx.x = spear->pos.x + TEXTURE_SPEAR_SIZE_X;
	spear->vertexWk[3].vtx.y = spear->pos.y + TEXTURE_SPEAR_SIZE_Y;
	spear->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 針の発射設定
//=============================================================================
void SetSpear(void)
{
	SPEAR *spear = &g_spear[0];			// 針のポインターを初期化
	MAP *map = GetMapData();			// マップのポインターを取得

	for (int p = 0; p < MAP_MAXDATA * SIZE_X * SIZE_Y; p++, map++)
	{
		// 針の種類に応じて飛ばす方向を変えてみる処理
		switch (map->type)
		{
		case BLOCK7:
			spear->direction = UP;
			spear->pos = map->pos;
			spear->pos.y += MAP_TEXTURE_SIZE_Y * 2.0f;
			break;

		case BLOCK8:
			spear->direction = DOWN;
			spear->pos = map->pos;
			spear->pos.y -= MAP_TEXTURE_SIZE_Y * 2.0f;
			break;

		case BLOCK9:
			spear->direction = RIGHT;
			spear->pos = map->pos;
			spear->pos.x -= MAP_TEXTURE_SIZE_X * 2.0f;
			break;

		case BLOCK11:
			spear->direction = LEFT;
			spear->pos = map->pos;
			spear->pos.x += MAP_TEXTURE_SIZE_X * 2.0f;
			break;

		}

		if (map->type == BLOCK7
			|| map->type == BLOCK8
			|| map->type == BLOCK9
			|| map->type == BLOCK11)
		{
			spear->use = true;
			spear++;
		}
	}
}

//=============================================================================
// 針の取得関数
//=============================================================================
SPEAR *GetSpear(int no)
{
	return(&g_spear[no]);
}

