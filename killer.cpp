//! @file	killer.cpp
//! @author	kitade mayumi
//! @date	2020-08-07
//! @brief	キラーの実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "killer.h"
#include "playerTest.h"
#include "map.h"
#include "checkhit.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_KILLER		_T("data/TEXTURE/EnemyChara/killer01.png")			// 画像
#define POP_COUNT_KILLER	(60 * 3)

// キラーのタイプの列挙型
enum KILLER_TYPE
{
	TRACKER,
	VECTOR,
	KILLER_TYPE_MAX
};

// キラーの向いている方向
enum KILLER_DIRECTION
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexKiller( int no );					// キラーの頂点情報作成
void SetTextureKiller(int no, int cntPattern, int direction);// キラーの頂点座標の設定
void SetVertexKiller(int no);						// キラーのテクスチャzの設定
void MovingKiller(void);							// キラーの移動処理
void SetKiller(void);
void DirectionKiller(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureKiller = NULL;		// テクスチャへのポリゴン

KILLER					g_killer[KILLER_MAX];			// キラーの構造体
static int g_count;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitKiller(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	KILLER *killer = &g_killer[0];		// キラーのポインターを初期化
	g_count = 0;

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_KILLER,			// ファイルの名前
			&g_pD3DTextureKiller);			// 読み込むメモリのポインタ
		
	}

	// バレットの初期化処理
	for (int i = 0; i < KILLER_MAX; i++, killer++)
	{
		killer->use = false;									// 未使用（発射されていない弾）
		killer->dead = false;
		killer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 座標データを初期化
		killer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		killer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量のデータを初期化
		killer->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ベクトルの初期化

		killer->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		killer->CountAnim = 0;									// アニメカウントを初期化
		killer->speed = 2.0f;									// 移動スピード
		killer->type = TRACKER;
		killer->vecUse = false;
		killer->popCnt = 0;
		killer->direction = LEFT;

		killer->Texture = g_pD3DTextureKiller;					// テクスチャ情報
		MakeVertexKiller(i);									// 頂点情報の作成
	}

	SetKiller();

	g_count++;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitKiller(void)
{
	KILLER *killer = g_killer;				// キラーのポインターを初期化

	if (g_pD3DTextureKiller != NULL)
	{	// テクスチャの開放
		g_pD3DTextureKiller->Release();
		g_pD3DTextureKiller = NULL;
	}
	for (int i = 0; i < KILLER_MAX; i++, killer++)
	{
		killer->use = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateKiller(void)
{
	KILLER *killer = g_killer;				// キラーのポインターを初期化
	int scene = GetScene();

	CheckHitKiller();

	for (int i = 0; i < KILLER_MAX; i++, killer++)
	{
		if (killer->use == true)			// 使用している状態なら更新する
		{
			// アニメーション  
			killer->CountAnim++;
			if ((killer->CountAnim % TIME_ANIMATION_KILLER) == 0)
			{
				// パターンの切り替え
				killer->PatternAnim = (killer->PatternAnim + 1) % ANIM_PATTERN_NUM_KILLER;
			}
			// キラーの移動処理
			MovingKiller();

			killer->move = killer->vec * killer->speed;

			D3DXVec3Add(&killer->pos, &killer->pos, &killer->move);

			DirectionKiller(i);

			// 画面外まで進んだ？
			if (killer->pos.x < 0.0f
				|| killer->pos.x >=SCREEN_WIDTH
				|| killer->pos.y <0.0f
				|| killer->pos.y >=SCREEN_HEIGHT)	// 自分の大きさを考慮して画面外か判定している
			{
				killer->use = false;
			}

		}
		else if (!killer->use)
		{
			if (scene == SCENE_GAME)
			{
				if (killer->dead)
				{
					// 再ポップまでのカウント
					killer->popCnt++;
				}

				// 再ポップまでのカウント条件を満たしたらもう一度セット
				if (killer->popCnt == POP_COUNT_KILLER)
				{
					SetKiller();
				}
			}

		}
		SetTextureKiller(i, killer->PatternAnim, killer->direction);	// アニメーション後のテクスチャの設定

		SetVertexKiller(i);							// 移動後の座標で頂点を設定
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void MovingKiller(void)
{
	KILLER *killer = &g_killer[0];			// キラーのポインターを初期化
	PLAYER *player = GetPlayer();			// プレイヤーのポインタを取得
	D3DXVECTOR3 vec, vec2;					// 仮のベクトルの作成

	for (int i = 0; i < KILLER_MAX; i++, killer++)
	{
		if (killer->type == TRACKER)
		{
			D3DXVec3Subtract(&vec, &player->pos, &killer->pos);
			D3DXVec3Normalize(&vec, &vec);
			D3DXVec3Scale(&vec, &vec, killer->speed);
			//// 回転処理
			//D3DXVECTOR3 pos = GetPlayer()->pos - killer->pos;
			//killer->rot.z = atan2f(pos.y, pos.x) - D3DX_PI / 2;
			killer->vec = vec;
		}

		else if(killer->type == VECTOR)
		{
			if (!killer->vecUse)
			{
				D3DXVec3Subtract(&vec2, &player->pos, &killer->pos);
				D3DXVec3Normalize(&vec2, &vec2);
				D3DXVec3Scale(&vec2, &vec2, killer->speed * 3.0f);
				killer->vec = vec2;
				killer->vecUse = true;
			}
		}
	}
}

//=============================================================================
// 向いている方向の設定
//=============================================================================
void DirectionKiller(int no)
{
	KILLER *killer = &g_killer[no];			// キラーのポインターを初期化
	PLAYER *player = GetPlayer();

	// キラーの向いている方向 = キラーのX軸とY軸のベクトルの大きさを比べてより大きいほうを優先にする
	if (killer->vec.y <= killer->vec.x)
	{
		if (killer->vec.x <= 0)
		{
			killer->direction = LEFT;
		}
		else
		{
			killer->direction = RIGHT;
		}
	}
	else
	{
		if (killer->vec.y <= 0)
		{
			killer->direction = UP;
		}
		else
		{
			killer->direction = DOWN;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawKiller(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	KILLER *killer = &g_killer[0];			// キラーのポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < KILLER_MAX; i++, killer++)
	{
		if (killer->use == true)			// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, killer->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_KILLER, killer->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

/**************************************************************************//**
	@brief		MakeVertexKiller
	@brief		キラーの頂点座標作成
	@param[in]	no		キラーの番号
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT MakeVertexKiller( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	KILLER *killer = &g_killer[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	SetVertexKiller( no );

	// rhwの設定
	killer->vertexWk[0].rhw =
	killer->vertexWk[1].rhw =
	killer->vertexWk[2].rhw =
	killer->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	killer->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	killer->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	killer->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	killer->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	killer->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	killer->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_KILLER, 0.0f);
	killer->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_KILLER);
	killer->vertexWk[3].tex = D3DXVECTOR2( 1.0f/ TEXTURE_PATTERN_DIVIDE_X_KILLER, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_KILLER);

	return S_OK;
}

/**************************************************************************//**
	@brief		SetTextureKiller
	@brief		キラーのテクスチャの座標設定
	@param[in]	no				キラーの番号
	@param[in]	cntPattern		キラーのアニメーションカウントパターン
	@param[in]	direction		キラーの向き
	@return		なし
*//***************************************************************************/
void SetTextureKiller( int no, int cntPattern, int direction)
{
	KILLER *killer = &g_killer[no];			// バレットのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_KILLER;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_KILLER;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_KILLER;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_KILLER;
	
	switch (direction)
	{
	case UP:
		killer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		killer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		killer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		killer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case DOWN:
		killer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		killer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		killer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		killer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case RIGHT:
		killer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		killer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		killer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		killer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case LEFT:
		killer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		killer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		killer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		killer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;
	}
}

/**************************************************************************//**
	@brief		SetVertexKiller
	@brief		キラーの頂点座標設定
	@param[in]	no		キラーの番号
	@return		なし
*//***************************************************************************/
void SetVertexKiller( int no )
{
	KILLER *killer = &g_killer[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	killer->vertexWk[0].vtx.x = killer->pos.x - TEXTURE_KILLER_SIZE_X;
	killer->vertexWk[0].vtx.y = killer->pos.y - TEXTURE_KILLER_SIZE_Y;
	killer->vertexWk[0].vtx.z = 0.0f;

	killer->vertexWk[1].vtx.x = killer->pos.x + TEXTURE_KILLER_SIZE_X;
	killer->vertexWk[1].vtx.y = killer->pos.y - TEXTURE_KILLER_SIZE_Y;
	killer->vertexWk[1].vtx.z = 0.0f;

	killer->vertexWk[2].vtx.x = killer->pos.x - TEXTURE_KILLER_SIZE_X;
	killer->vertexWk[2].vtx.y = killer->pos.y + TEXTURE_KILLER_SIZE_Y;
	killer->vertexWk[2].vtx.z = 0.0f;

	killer->vertexWk[3].vtx.x = killer->pos.x + TEXTURE_KILLER_SIZE_X;
	killer->vertexWk[3].vtx.y = killer->pos.y + TEXTURE_KILLER_SIZE_Y;
	killer->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// キラーの発射設定
//=============================================================================
void SetKiller(void)
{
	KILLER *killer = &g_killer[0];			// キラーのポインターを初期化
	MAP *map = GetMapData();				// マップのポインターを取得

	for (int p = 0; p < MAP_MAXDATA * SIZE_X * SIZE_Y; p++, map++)
	{
		if (killer->dead || g_count == 0)
		{
			// キラーの種類に応じて飛ばす方向を変えてみる処理
			switch (map->type)
			{
			case BLOCK4:
			case BLOCK5:
				if (map->type == BLOCK4)
				{
					killer->type = TRACKER;
				}
				else
				{
					killer->type = VECTOR;
				}
				killer->use = true;
				killer->pos = map->pos;
				killer->popCnt = 0;
				killer->dead = false;
				killer->vecUse = false;
				killer++;
				break;
			}
		}
	}
}

//=============================================================================
// キラーの取得関数
//=============================================================================
KILLER *GetKiller(int no)
{
	return(&g_killer[no]);
}

