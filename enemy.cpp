//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : kitade mayumi
//! @date	2020-08-09
//! @brief	敵の実装
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "playerTest.h"
#include "map.h"
#include "checkhit.h"
#include "enemyBullet.h"
#include <math.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_TIME_SHOT	(100)
#define ENEMY_DIRECTION_ATTACK (30)
#define ENEMY_LIMIT_WALL	(100)
#define ENEMY_DIERECTION_DISCOVERY	(200)

enum ENEMY_STATE_ANIME
{
	IDLE,
	ATTACK,
	DEATH,
	STATE_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ReadEnemyTexture(void);
HRESULT MakeVertexEnemy( int no );
void SetTextureEnemy( int no, int cntPattern );
void SetVertexEnemy( int no );
void SetEnemy(void);

bool SearchPlayerDiscovery(int i);

void MovingEnemy(int i);
void AttackEnemy(int i);
void DeathEnemy(int i);
void FallEnemy(int i);

void UpdateSniperEnemy(int i);
void UpdateTrackerEnemy(int i);
void UpdateGuardianEnemy(int i);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy[ENEMYTYPEMAX][STATE_MAX] = { NULL };		// テクスチャへのポリゴン

static ENEMY					enemyWk[ENEMY_MAX];				// エネミー構造体
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	// エネミーのテクスチャの読み込み
	ReadEnemyTexture();
	// エネミーの初期化処理
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemyWk[i].use = false;										// 使用
		enemyWk[i].hp = 2;											// 体力
		enemyWk[i].damage = false;									// ダメージ判定
		enemyWk[i].type = SNIPER;									// エネミータイプの初期化
		enemyWk[i].countShot = 0;									// カウントショットの初期化
		enemyWk[i].direction = Left;								// エネミーの向いている方向
		enemyWk[i].state = IDLE;									// エネミーの状態初期化
		enemyWk[i].dropSpeed = 0;									// エネミーの落下処理

		enemyWk[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		enemyWk[i].move = D3DXVECTOR3(ENEMY_MOVE_SPEED, 0.0f, 0.0f);// 移動量を初期化
		enemyWk[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転量を初期化
		enemyWk[i].PatternAnim = 0;									// アニメパターン番号をランダムで初期化
		enemyWk[i].CountAnim = 0;									// アニメカウントを初期化
		
		D3DXVECTOR2 temp = D3DXVECTOR2(ENEMY_TEXTURE_SIZE_X, ENEMY_TEXTURE_SIZE_Y);
		enemyWk[i].Radius = D3DXVec2Length(&temp);					// エネミーの半径を初期化
		enemyWk[i].BaseAngle = atan2f(ENEMY_TEXTURE_SIZE_Y, ENEMY_TEXTURE_SIZE_X);	// エネミーの角度を初期化
		// 頂点情報の作成
		MakeVertexEnemy(i);
	}

	SetEnemy();

	return S_OK;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void ReadEnemyTexture(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	for (int i = 0; i < ENEMYTYPEMAX; i++)
	{
		for (int j = 0; j < STATE_MAX; j++)
		{
			// スナイパー型のテクスチャ読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_SNIPER_IDLE_TEXTURE,							// ファイルの名前
				&g_pD3DTextureEnemy[SNIPER][IDLE]);					// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_SNIPER_ATTACK_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[SNIPER][ATTACK]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_SNIPER_DEATH_TEXTURE,							// ファイルの名前
				&g_pD3DTextureEnemy[SNIPER][DEATH]);				// 読み込むメモリのポインタ

			// トラッカー型のテクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_TRACKER_IDLE_TEXTURE,							// ファイルの名前
				&g_pD3DTextureEnemy[TRACKER][IDLE]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_TRACKER_ATTACK_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[TRACKER][ATTACK]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_TRACKER_DEATH_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[TRACKER][DEATH]);				// 読み込むメモリのポインタ


			// ガーディアン型のテクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_GUARDIAN_IDLE_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[GUARDIAN][IDLE]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_GUARDIAN_ATTACK_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[GUARDIAN][ATTACK]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ENEMY_GUARDIAN_DEATH_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[GUARDIAN][DEATH]);				// 読み込むメモリのポインタ

		}
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < ENEMYTYPEMAX; i++)
	{
		for (int j = 0; j < STATE_MAX; j++)
		{
			if (g_pD3DTextureEnemy[i][j] != NULL)
			{	// テクスチャの開放
				g_pD3DTextureEnemy[i][j]->Release();
				g_pD3DTextureEnemy[i][j] = NULL;
			}
		}
	}

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemyWk[i].use = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER *player = GetPlayer();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		D3DXVECTOR3 vec = player->pos - enemyWk[i].pos;
		fabsf(vec.x);
		if (enemyWk[i].use == true)
		{
			// アニメーション
			enemyWk[i].CountAnim++;

			if ((enemyWk[i].CountAnim % ENEMY_TIME_ANIMATION) == 0)
			{
				// パターンの切り替え
				enemyWk[i].PatternAnim = (enemyWk[i].PatternAnim + 1) % ENEMY_ANIM_PATTERN_NUM;
			}

			if (vec.x <= SCREEN_WIDTH && enemyWk[i].pos.x <= SCREEN_WIDTH)
			{
				// それぞれのタイプ別ごとのエネミーの更新処理
				UpdateSniperEnemy(i);
				UpdateTrackerEnemy(i);
				UpdateGuardianEnemy(i);
			}

			AttackEnemy(i);

			CheckEnemyBullet();
			DeathEnemy(i);

			// 移動後の座標で頂点を設定
			SetVertexEnemy(i);						
			// テクスチャ座標を設定
			SetTextureEnemy(i, enemyWk[i].PatternAnim);

		}
	}
}

//=============================================================================
// スナイパー型の更新処理
//=============================================================================
void UpdateSniperEnemy(int i)
{
	if (enemyWk[i].type == SNIPER)					// 使用している状態なら更新する
	{
		enemyWk[i].countShot++;

		D3DXVECTOR3 pos = GetPlayer()->pos - enemyWk[i].pos;
		// 方向処理
		if (pos.x >= 0)
		{
			enemyWk[i].direction = Right;
		}
		else
		{
			enemyWk[i].direction = Left;
		}

		// 回転処理
		if (enemyWk[i].direction == Right)
		{
			enemyWk[i].rot.z = atan2f(pos.y, pos.x);
		}
		else
		{
			enemyWk[i].rot.z = atan2f(pos.y, pos.x) - D3DX_PI;
		}

		// 移動処理
		float rot = atan2f(pos.y, pos.x);

		enemyWk[i].pos.x += cosf(rot) * enemyWk[i].move.x/3;
		enemyWk[i].pos.y += sinf(rot) * enemyWk[i].move.x/3;		

	}
}

//=============================================================================
// 追尾型の更新処理
//=============================================================================
void UpdateTrackerEnemy(int i)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 vec;

	// プレイヤーを追跡する
	if (enemyWk[i].type == TRACKER)
	{
		D3DXVec3Subtract(&vec, &player->pos, &enemyWk[i].pos);

		if (vec.x <= 0)
		{
			enemyWk[i].pos.x -= enemyWk[i].move.x;
		}
		else
		{
			enemyWk[i].pos.x += enemyWk[i].move.x;
		}
		MovingEnemy(i);
		RestrictionEnemy(i);
		FallEnemy(i);
	}
}

//=============================================================================
// 追尾型の更新処理
//=============================================================================
void UpdateGuardianEnemy(int i)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 vec;

	if (enemyWk[i].type == GUARDIAN)
	{
		// プレイヤーを発見したら、プレイヤーを追跡する
		if (SearchPlayerDiscovery(i))
		{
			D3DXVec3Subtract(&vec, &player->pos, &enemyWk[i].pos);
			enemyWk[i].move.x = 1.0f;
			if (vec.x <= 0)
			{
				enemyWk[i].pos.x -= enemyWk[i].move.x;
			}
			else
			{
				enemyWk[i].pos.x += enemyWk[i].move.x;
			}
		}
		else
		{
			//// 移動する
			//enemyWk[i].pos.x += enemyWk[i].move.x;
			//// 横画面外へ出たら
			//if ((enemyWk[i].pos.x < ENEMY_LIMIT_WALL)
			//	|| (enemyWk[i].pos.x > (SCREEN_WIDTH - ENEMY_LIMIT_WALL)))
			//{
			//	enemyWk[i].move.x *= -1;			// 反対方向へ移動させる
			//}
		}
		MovingEnemy(i);
		RestrictionEnemy(i);
		FallEnemy(i);
	}
}

//=============================================================================
// 発見処理
//=============================================================================
bool SearchPlayerDiscovery(int i)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 vec;
	float lenght;
	// プレイヤーとエネミーの距離を計算する。
	D3DXVec3Subtract(&vec, &player->pos, &enemyWk[i].pos);
	lenght = fabsf(vec.x);

	// 距離を絶対値とし、距離が一定になったらプレイヤーを発見
	if (lenght <= ENEMY_DIERECTION_DISCOVERY)
	{
		return true;
	}

	return false;
}

//=============================================================================
// エネミーの落下処理
//=============================================================================
void FallEnemy(int i)
{
	enemyWk[i].pos.y += enemyWk[i].dropSpeed * 1.0f;	// 加速度的に下へ移動、重力加速度
	enemyWk[i].dropSpeed++;

	FallCheckHitEnemy(i);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemyWk[i].use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, enemyWk[i].Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, enemyWk[i].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemy( int i )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexEnemy( i );

	// rhwの設定
	enemyWk[i].vertexWk[0].rhw =
	enemyWk[i].vertexWk[1].rhw =
	enemyWk[i].vertexWk[2].rhw =
	enemyWk[i].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	enemyWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemyWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemyWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemyWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	enemyWk[i].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemyWk[i].vertexWk[1].tex = D3DXVECTOR2(1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	enemyWk[i].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_Y);
	enemyWk[i].vertexWk[3].tex = D3DXVECTOR2( 1.0f/ ENEMY_TEXTURE_PATTERN_DIVIDE_X, 1.0f/ENEMY_TEXTURE_PATTERN_DIVIDE_Y);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy( int i, int cntPattern )
{
	// テクスチャ座標の設定
	int x = cntPattern % ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_Y;
	
	if (enemyWk[i].direction == Left)
	{
		enemyWk[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		enemyWk[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		enemyWk[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		enemyWk[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else
	{
		enemyWk[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		enemyWk[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		enemyWk[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		enemyWk[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy( int i )
{
	// 頂点座標の設定
	enemyWk[i].vertexWk[0].vtx.x = enemyWk[i].pos.x - cosf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[0].vtx.y = enemyWk[i].pos.y - sinf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[0].vtx.z = 0.0f;

	enemyWk[i].vertexWk[1].vtx.x = enemyWk[i].pos.x + cosf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[1].vtx.y = enemyWk[i].pos.y - sinf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[1].vtx.z = 0.0f;

	enemyWk[i].vertexWk[2].vtx.x = enemyWk[i].pos.x - cosf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[2].vtx.y = enemyWk[i].pos.y + sinf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[2].vtx.z = 0.0f;

	enemyWk[i].vertexWk[3].vtx.x = enemyWk[i].pos.x + cosf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[3].vtx.y = enemyWk[i].pos.y + sinf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// エネミーの位置設定関数
//=============================================================================
void SetEnemy(void)
{
	MAP *map = GetMapData();
	int i = 0;

	for (int p = 0; p < MAP_MAXDATA * SIZE_X * SIZE_Y; p++, map++)
	{
		switch(map->type)
		{
		case BLOCK13:
			enemyWk[i].type = SNIPER;
			enemyWk[i].Texture = g_pD3DTextureEnemy[SNIPER][IDLE];
			enemyWk[i].pos = map->pos;
			enemyWk[i].use = true;
			i++;
			break;

		case GLASS14:
			enemyWk[i].type = TRACKER;
			enemyWk[i].Texture = g_pD3DTextureEnemy[TRACKER][IDLE];
			enemyWk[i].pos = map->pos;
			enemyWk[i].use = true;
			i++;
			break;

		case GLASS15:
			enemyWk[i].type = GUARDIAN;
			enemyWk[i].Texture = g_pD3DTextureEnemy[GUARDIAN][IDLE];
			enemyWk[i].pos = map->pos;
			enemyWk[i].use = true;
			i++;
			break;

		default:
			break;
		}
	}
}

//=============================================================================
// エネミーの移動設定
//=============================================================================
void MovingEnemy(int i)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 vec;

	D3DXVec3Subtract(&vec, &player->pos, &enemyWk[i].pos);
	if (vec.x <= 0)
	{
		enemyWk[i].direction = Left;
	}
	else
	{
		enemyWk[i].direction = Right;
	}
}

//=============================================================================
// エネミーの攻撃設定
//=============================================================================
void AttackEnemy(int i)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 vec;
	float lenght;

	switch (enemyWk[i].type)
	{
	case SNIPER:
		if (enemyWk[i].countShot >= ENEMY_TIME_SHOT)
		{
			SetEnemyBullet(enemyWk[i].pos, player->pos, enemyWk[i].type, &enemyWk[i].countShot);
			enemyWk[i].state = ATTACK;
			enemyWk[i].Texture = g_pD3DTextureEnemy[enemyWk[i].type][enemyWk[i].state];
		}
		break;

	case TRACKER:
	case GUARDIAN:
		// プレイヤーとエネミーの距離を計算する。
		D3DXVec3Subtract(&vec, &player->pos, &enemyWk[i].pos);
		lenght = fabsf(vec.x);
		
		// 距離を絶対値とし、距離が一定になったら攻撃モーションを開始する
		if (lenght <= ENEMY_DIRECTION_ATTACK)
		{
			enemyWk[i].state = ATTACK;
			enemyWk[i].Texture = g_pD3DTextureEnemy[enemyWk[i].type][enemyWk[i].state];
		}
		else
		{
			enemyWk[i].state = IDLE;
			enemyWk[i].Texture = g_pD3DTextureEnemy[enemyWk[i].type][enemyWk[i].state];
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// エネミーの破棄設定
//=============================================================================
void DeathEnemy(int i)
{
	if (enemyWk[i].pos.x <= 0.0f )
	{
		enemyWk[i].use = false;
	}
}

//=============================================================================
// エネミー取得関数
//=============================================================================
ENEMY *GetEnemy(void)
{
	return(&enemyWk[0]);
}


