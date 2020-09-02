//=============================================================================
//
// ボスの処理 [boss.cpp]
// Author : kitade mayumi
//! @date	2020-08-28
//! @brief	ボスの実装
//=============================================================================
#include "main.h"
#include "boss.h"
#include "playerTest.h"
#include "map.h"
#include "checkhit.h"
#include "enemyBullet.h"
#include "effect.h"
#include <math.h>
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSS_TIME_SHOT	(100)
#define BOSS_DIRECTION_ATTACK (250)
#define BOSS_LIMIT_WALL	(100)
#define BOSS_MAX_HP	(10)
#define BOSS_MAX_ATTACKTYPE	(4)
#define BOSS_MAX_COLOR_ALPHA	(255)

#define BOSS_ALPHA_CHANGE_TIME	(5)
#define BOSS_DAMAGE_TIME	(2 * 60)

// #define BOSS_STAGE_APPEAR	(MAP_MAXDATA * player->scrollCount)
enum BOSS_STATE_ANIME
{
	IDLE,
	ATTACK,
	RUN,
	DEATH,
	STATE_MAX,
};

enum BOSSTYPE
{
	SINGLE,
	TWIN,
	BOSSTYPEMAX
};

enum ATTACKTYPE
{
	TWINSLASH,
	DOWNSLASH,
	TWINFAIR,
	SINGLESLASH,
	MAX_ATTACKTYPE
};

enum TWINTYPE
{
	ZEROTYPE,
	ONETYPE,
	TWOTYPE,
	MAX_TWINTYPE
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ReadBossTexture(void);
HRESULT MakeVertexBoss( int no );
void SetTextureBoss( int no, int cntPattern );
void SetVertexBoss( int no );
void SetBoss(int i);

void AttackBoss(int i);
void DamageBoss(int i);

void UpdateSingleBoss(int i);
void UpdateTwinBoss(int i);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy[BOSSTYPEMAX][STATE_MAX] = { NULL };		// テクスチャへのポリゴン

static BOSS					g_boss[BOSS_MAX];				// エネミー構造体
static int					g_countAttack;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(void)
{
	// ボスのテクスチャの読み込み
	ReadBossTexture();
	g_countAttack = 0;
	// ボスの初期化処理
	for (int i = 0; i < BOSS_MAX; i++)
	{
		g_boss[i].use = false;									// 使用
		g_boss[i].vecUse = false;								// 方向の初期設定
		g_boss[i].downUse = false;
		g_boss[i].start = false;								// スタート位置
		g_boss[i].lenghtUse = false;							// 距離判定初期化
		g_boss[i].attackUse = false;
		g_boss[i].attackUseBullet = false;


		g_boss[i].hp = BOSS_MAX_HP;								// 体力
		g_boss[i].damage = false;								// ダメージ判定
		g_boss[i].type = SINGLE;								// エネミータイプの初期化
		g_boss[i].countShot = 0;								// カウントショットの初期化
		g_boss[i].colorCnt = 0.0f;								// α値の色の変更カウント
		g_boss[i].direction = Left;								// ボスの向いている方向
		g_boss[i].state = IDLE;									// ボスの状態初期化

		g_boss[i].attackPtn = 0;
		g_boss[i].damageCnt = 0;
		g_boss[i].lenght = 0.0f;

		g_boss[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		g_boss[i].move = D3DXVECTOR3(BOSS_MOVE_SPEED, 0.0f, 0.0f);// 移動量を初期化
		g_boss[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転量を初期化
		g_boss[i].PatternAnim = 0;									// アニメパターン番号をランダムで初期化
		g_boss[i].CountAnim = 0;									// アニメカウントを初期化
		
		D3DXVECTOR2 temp = D3DXVECTOR2(BOSS_TEXTURE_SIZE_X, BOSS_TEXTURE_SIZE_Y);
		g_boss[i].Radius = D3DXVec2Length(&temp);					// ボスの半径を初期化
		g_boss[i].BaseAngle = atan2f(BOSS_TEXTURE_SIZE_Y, BOSS_TEXTURE_SIZE_X);	// ボスの角度を初期化
		// 頂点情報の作成
		MakeVertexBoss(i);
	}
	for (int i = 0; i < BOSS_MAX; i++)
	{
		SetBoss(i);
	}

	return S_OK;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void ReadBossTexture(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	for (int i = 0; i < BOSSTYPEMAX; i++)
	{
		for (int j = 0; j < STATE_MAX; j++)
		{
			// スナイパー型のテクスチャ読み込み
			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_SINGLE_IDLE_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[SINGLE][IDLE]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_SINGLE_ATTACK_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[SINGLE][ATTACK]);			// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_SINGLE_RUN_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[SINGLE][RUN]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_SINGLE_DEATH_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[SINGLE][DEATH]);			// 読み込むメモリのポインタ

			// トラッカー型のテクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_TWIN_IDLE_TEXTURE,							// ファイルの名前
				&g_pD3DTextureEnemy[TWIN][IDLE]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_TWIN_ATTACK_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[TWIN][ATTACK]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_TWIN_RUN_TEXTURE,							// ファイルの名前
				&g_pD3DTextureEnemy[TWIN][RUN]);				// 読み込むメモリのポインタ

			D3DXCreateTextureFromFile(pDevice,					// デバイスのポインタ
				BOSS_TWIN_DEATH_TEXTURE,						// ファイルの名前
				&g_pD3DTextureEnemy[TWIN][DEATH]);				// 読み込むメモリのポインタ

		}
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	for (int i = 0; i < BOSSTYPEMAX; i++)
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

	for (int i = 0; i < BOSS_MAX; i++)
	{
		g_boss[i].use = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	PLAYER *player = GetPlayer();
	for (int i = 0; i < BOSS_MAX; i++)
	{
		D3DXVECTOR3 vec = player->pos - g_boss[i].pos;
		vec.x =fabsf(vec.x);
		if (g_boss[i].use == true)
		{
			// アニメーション
			g_boss[i].CountAnim++;

			if ((g_boss[i].CountAnim % BOSS_TIME_ANIMATION) == 0)
			{
				// パターンの切り替え
				g_boss[i].PatternAnim = (g_boss[i].PatternAnim + 1) % BOSS_ANIM_PATTERN_NUM;
			}

			if (player->pos.x >= SCREEN_CENTER_X
				&& player->pos.x <= SCREEN_WIDTH 
				&& player->countScroll >= (2 * MAP_MAXDATA - 1))
			{
				for (int j = 0; j < BOSS_MAX; j++)
				{
					g_boss[j].start = true;
				}
			}

		}
		if (g_boss[i].start)
		{
			// 攻撃パターンの更新
			g_boss[i].attackPtn = g_countAttack;

			// それぞれのタイプ別ごとのボスの更新処理
			UpdateSingleBoss(i);
			UpdateTwinBoss(i);

			AttackBoss(i);

			DamageBoss(i);

			CheckEnemyBullet(); // ボスのやつに変える + プレイヤーとボスとの判定も作る
		}

		if (g_boss[i].use == true)
		{
			// 移動後の座標で頂点を設定
			SetVertexBoss(i);
			// テクスチャ座標を設定
			SetTextureBoss(i, g_boss[i].PatternAnim);
		}
	}
}

//=============================================================================
// boss[1][2]ツインタイプの更新処理
//=============================================================================
void UpdateTwinBoss(int i)
{
	// 0のタイプ以外の攻撃更新
	if (g_boss[i].type != SINGLE && i != SINGLE)					// 使用している状態なら更新する
	{
		g_boss[i].countShot++;
		// 攻撃パターンが0の時更新
		if (g_boss[i].attackPtn == TWINSLASH)
		{
			if (!g_boss[i].lenghtUse)
			{
				g_boss[i].lenght = GetPlayer()->pos.x - g_boss[i].pos.x;
				g_boss[i].lenghtUse = true;
				g_boss[i].use = true;
			}

			if (g_boss[i].lenght <= 0)
			{
				g_boss[i].direction = Left;
				g_boss[i].pos.x -= g_boss[i].move.x * 5.0f;
				if (g_boss[i].pos.x <= ((float)SCREEN_CENTER_X + (BOSS_TEXTURE_SIZE_X * 2)))
				{
					SetEffect(g_boss[i].pos.x, g_boss[i].pos.y, BOSS_MAX_HP);
					SetBoss(i);
					g_boss[i].use = false;
				}
			}
			else if(g_boss[i].lenght > 0)
			{
				g_boss[i].direction = Right;
				g_boss[i].pos.x += g_boss[i].move.x * 5.0f;
				if (g_boss[i].pos.x >= ((float)SCREEN_CENTER_X - (BOSS_TEXTURE_SIZE_X * 2)))
				{
					SetEffect(g_boss[i].pos.x, g_boss[i].pos.y, BOSS_MAX_HP);
					SetBoss(i);
					g_boss[i].use = false;
				}
			}
			if (!g_boss[1].use && !g_boss[2].use)
			{
				g_countAttack++;
				for (int j = 1; j < BOSS_MAX; j++)
				{
					g_boss[j].lenghtUse = false;
				}
			}
		}
		// 攻撃パターンが2の時更新
		else if (g_boss[i].attackPtn == TWINFAIR)
		{
			if (!g_boss[i].vecUse)
			{
				g_boss[i].use = true;
				g_boss[i].pos.y -= SCREEN_CENTER_Y;
				D3DXVECTOR3 pos = D3DXVECTOR3(GetPlayer()->pos.x, 
					(SCREEN_WIDTH - BOSS_TEXTURE_SIZE_X), 0.0f) - g_boss[i].pos;
				// 方向処理
				if (pos.x > 0)
				{
					g_boss[i].direction = Right;
				}
				else if(pos.x < 0)
				{
					g_boss[i].direction = Left;
				}
				else
				{
					g_boss[i].direction = Down;
				}

				// 回転処理
				if (g_boss[i].direction == Right)
				{
					g_boss[i].rot.z = atan2f(pos.y, pos.x);
				}
				else if(g_boss[i].direction == Left)
				{
					g_boss[i].rot.z = atan2f(pos.y, pos.x) - D3DX_PI;
				}
				else
				{
					g_boss[i].rot.z = 0.0f;
				}

				// 移動処理
				float rot = atan2f(pos.y, pos.x);
				g_boss[i].vecUse = true;
				if (g_boss[i].direction == Right)
				{
					g_boss[i].pos.x += cosf(rot) * g_boss[i].move.x * 2.0f;
					g_boss[i].pos.y += sinf(rot) * g_boss[i].move.x * 2.0f;
				}
				else if (g_boss[i].direction == Left)
				{
					g_boss[i].pos.x -= cosf(rot) * g_boss[i].move.x * 2.0f;
					g_boss[i].pos.y += sinf(rot) * g_boss[i].move.x * 2.0f;
				}
				else
				{
					g_boss[i].pos.x += g_boss[i].move.x * 2.0f;
					g_boss[i].pos.y += g_boss[i].move.x * 2.0f;
				}
			}
			if(g_boss[i].pos.y < ((float)SCREEN_HEIGHT - (MAP_TEXTURE_SIZE_Y * 4)))
			{
				// 弾が着弾し終わったら、次の攻撃に移行する
				if (FallEnemyBullet())
				{
					for (int j = 1; j < BOSS_MAX; j++)
					{
						g_boss[j].attackUseBullet = true;
					}
				}
				if (g_boss[i].attackUse && g_boss[i].attackUseBullet)
				{
					if (g_boss[i].direction == Right)
					{
						g_boss[i].pos.x += g_boss[i].move.x * 3.0f;
					}
					else
					{
						g_boss[i].pos.x -= g_boss[i].move.x * 3.0f;
					}
					g_boss[i].pos.y += g_boss[i].move.x * 3.0f;
				}
			}
			else if(g_boss[i].pos.y >= ((float)SCREEN_HEIGHT - (MAP_TEXTURE_SIZE_Y * 4)))
			{
				SetBoss(i);
				g_boss[i].use = false;
			}
			if (!g_boss[1].use && !g_boss[2].use)
			{
				g_countAttack++;
				for (int j = 1; j < BOSS_MAX; j++)
				{
					g_boss[j].vecUse = false;
					g_boss[j].rot.z = 0;
					g_boss[j].attackUse = false;
					g_boss[j].attackUseBullet = false;
				}
			}
		}
		else
		{
			return;
		}
	}
}

//=============================================================================
// boss[0]シングルタイプの更新処理
//=============================================================================
void UpdateSingleBoss(int i)
{
	float vec;
	// 0のタイプの攻撃更新
	if (g_boss[i].type == SINGLE && i == SINGLE)					// 使用している状態なら更新する
	{
		// 攻撃パターン1の時の更新
		if (g_boss[i].attackPtn == DOWNSLASH)
		{
			g_boss[i].use = true;
			if (!g_boss[i].downUse)
			{
				g_boss[i].pos.x = GetPlayer()->pos.x;
				g_boss[i].downUse = true;
				g_boss[i].direction = UNDER;
			}
			if (g_boss[i].pos.y <= ((float)SCREEN_HEIGHT - (MAP_TEXTURE_SIZE_Y * 4)))
			{
				g_boss[i].pos.y += g_boss[i].move.x * 5.0f;
			}
			else
			{
				g_countAttack++;
				g_boss[i].downUse = false;
				SetBoss(i);
				g_boss[i].use = false;
				g_boss[i].rot.z = 0.0f;

			}
		}
		// 攻撃パターン3の時の更新
		if (g_boss[i].attackPtn == SINGLESLASH)
		{
			g_boss[i].use = true;

			if (!g_boss[i].downUse)
			{
				g_boss[i].pos.y = ((float)SCREEN_HEIGHT - (MAP_TEXTURE_SIZE_Y * 6));
				g_boss[i].pos.x = (float)SCREEN_WIDTH - (MAP_TEXTURE_SIZE_Y * 2);
				vec = GetPlayer()->pos.x - g_boss[i].pos.x;
				g_boss[i].downUse = true;
				if (vec < 0)
				{
					g_boss[i].direction = Left;
				}
				else
				{
					g_boss[i].direction = Right;
				}
			}
			if (g_boss[i].direction == Left && g_boss[i].pos.x >= (0.0f - BOSS_TEXTURE_SIZE_X))
			{
				g_boss[i].pos.x -= g_boss[i].move.x * 5.0f;
			}
			else if (g_boss[i].direction == Right && g_boss[i].pos.x <= (SCREEN_WIDTH + BOSS_TEXTURE_SIZE_X))
			{
				g_boss[i].pos.x += g_boss[i].move.x * 5.0f;
			}
			else
			{
				g_countAttack = 0;
				SetBoss(i);
				g_boss[i].downUse = false;
				g_boss[i].use = false;
			}
		}
	}
}

//=============================================================================
// ボスのダメージの状態遷移
//=============================================================================
void DamageBoss(int i)
{
	if (g_boss[i].damage == true)
	{
		g_boss[i].damageCnt++;

		if (g_boss[i].damageCnt >= BOSS_DAMAGE_TIME)
		{
			g_boss[i].damage = false;
			g_boss[i].damageCnt = 0;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < BOSS_MAX; i++)
	{
		if (g_boss[i].use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_boss[i].Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_boss[i].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBoss( int i )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexBoss( i );

	// rhwの設定
	g_boss[i].vertexWk[0].rhw =
	g_boss[i].vertexWk[1].rhw =
	g_boss[i].vertexWk[2].rhw =
	g_boss[i].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	g_boss[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_boss[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_boss[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_boss[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_boss[i].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_boss[i].vertexWk[1].tex = D3DXVECTOR2(1.0f / BOSS_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	g_boss[i].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / BOSS_TEXTURE_PATTERN_DIVIDE_Y);
	g_boss[i].vertexWk[3].tex = D3DXVECTOR2( 1.0f/ BOSS_TEXTURE_PATTERN_DIVIDE_X, 1.0f/BOSS_TEXTURE_PATTERN_DIVIDE_Y);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBoss( int i, int cntPattern )
{
	// テクスチャ座標の設定
	int x = cntPattern % BOSS_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / BOSS_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / BOSS_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / BOSS_TEXTURE_PATTERN_DIVIDE_Y;
	
	if (g_boss[i].direction == Left)
	{
		g_boss[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		g_boss[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		g_boss[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		g_boss[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else if(g_boss[i].direction == Right)
	{
		g_boss[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		g_boss[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		g_boss[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		g_boss[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else if(g_boss[i].direction == UNDER)
	{
		g_boss[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		g_boss[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		g_boss[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		g_boss[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}

	int flashing = (g_boss[i].damageCnt + 1) % 2;

	if (g_boss[i].start)
	{
		if (g_boss[i].colorCnt <= 1.0f)
		{
			g_boss[i].colorCnt += (1.0f - 0.1f) / (float)BOSS_ALPHA_CHANGE_TIME;
		}

		// 反射光の設定
		g_boss[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * flashing));
		g_boss[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * flashing));
		g_boss[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * flashing));
		g_boss[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * flashing));
	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBoss( int i )
{
	// 頂点座標の設定
	g_boss[i].vertexWk[0].vtx.x = g_boss[i].pos.x - cosf(g_boss[i].BaseAngle + g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[0].vtx.y = g_boss[i].pos.y - sinf(g_boss[i].BaseAngle + g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[0].vtx.z = 0.0f;

	g_boss[i].vertexWk[1].vtx.x = g_boss[i].pos.x + cosf(g_boss[i].BaseAngle - g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[1].vtx.y = g_boss[i].pos.y - sinf(g_boss[i].BaseAngle - g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[1].vtx.z = 0.0f;

	g_boss[i].vertexWk[2].vtx.x = g_boss[i].pos.x - cosf(g_boss[i].BaseAngle - g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[2].vtx.y = g_boss[i].pos.y + sinf(g_boss[i].BaseAngle - g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[2].vtx.z = 0.0f;

	g_boss[i].vertexWk[3].vtx.x = g_boss[i].pos.x + cosf(g_boss[i].BaseAngle + g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[3].vtx.y = g_boss[i].pos.y + sinf(g_boss[i].BaseAngle + g_boss[i].rot.z) * g_boss[i].Radius;
	g_boss[i].vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ボスの位置設定関数
//=============================================================================
void SetBoss(int i)
{
	MAP *map = GetMapData();

	for (int p = 0; p < MAP_MAXDATA * SIZE_X * SIZE_Y; p++, map++)
	{
		switch(map->type)
		{
		case BOSS1:
			if (i == SINGLE)
			{
				g_boss[i].type = SINGLE;
				g_boss[i].Texture = g_pD3DTextureEnemy[SINGLE][IDLE];
				g_boss[i].pos = map->pos;
				g_boss[i].use = true;
				g_boss[i].rot.z = (D3DX_PI / 2.0f) * 3.0f;

			}
			break;

		case BOSS2:
			if (i == ONETYPE)
			{
				g_boss[i].type = TWIN;
				g_boss[i].Texture = g_pD3DTextureEnemy[TWIN][IDLE];
				g_boss[i].pos = map->pos;
				g_boss[i].use = true;
				break;
			}
		case BOSS3:
			if (i == TWOTYPE)
			{
				g_boss[i].type = TWIN;
				g_boss[i].Texture = g_pD3DTextureEnemy[TWIN][IDLE];
				g_boss[i].pos = map->pos;
				g_boss[i].use = true;
				break;
			}

		default:
			break;
		}
	}
}

//=============================================================================
// ボスの攻撃設定
//=============================================================================
void AttackBoss(int i)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 vec;
	float lenght = 0.0f;

	switch (g_boss[i].type)
	{
	case ZEROTYPE:
		// プレイヤーとボスの距離を計算する。
		D3DXVec3Subtract(&vec, &player->pos, &g_boss[i].pos);

		if (g_boss[i].attackPtn == DOWNSLASH)
		{
			lenght = fabsf(vec.x);
		}
		else if (g_boss[i].attackPtn == SINGLESLASH)
		{
			lenght = fabsf(vec.y);
		}

		// 距離を絶対値とし、距離が一定になったら攻撃モーションを開始する
		if (lenght <= BOSS_DIRECTION_ATTACK)
		{
			g_boss[i].state = ATTACK;
			g_boss[i].Texture = g_pD3DTextureEnemy[g_boss[i].type][g_boss[i].state];
		}
		else
		{
			g_boss[i].state = RUN;  // ここを走ってるモーションにする
			g_boss[i].Texture = g_pD3DTextureEnemy[g_boss[i].type][g_boss[i].state];
		}
		break;

	case ONETYPE:
	case TWOTYPE:
		if (g_countAttack == TWINFAIR && !g_boss[i].attackUse)
		{
			SetEnemyBullet(g_boss[i].pos,GetPlayer()->pos,
				g_boss[i].type, &g_boss[i].countShot);
			PlaySound(SOUND_LABEL_SE_SHOT01);

			g_boss[i].state = ATTACK;
			g_boss[i].Texture = g_pD3DTextureEnemy[g_boss[i].type][g_boss[i].state];
			g_boss[i].attackUse = true;
		}

		if (g_boss[i].attackUse || g_countAttack == TWINSLASH)
		{
			// プレイヤーとボスの距離を計算する。
			D3DXVec3Subtract(&vec, &player->pos, &g_boss[i].pos);
			lenght = fabsf(vec.x);

			// 距離を絶対値とし、距離が一定になったら攻撃モーションを開始する
			if (lenght <= BOSS_DIRECTION_ATTACK)
			{
				g_boss[i].state = ATTACK;
				g_boss[i].Texture = g_pD3DTextureEnemy[g_boss[i].type][g_boss[i].state];
			}
			else
			{
				g_boss[i].state = RUN; // ここを走ってるモーションにする
				g_boss[i].Texture = g_pD3DTextureEnemy[g_boss[i].type][g_boss[i].state];
			}
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// ボスのアドレス取得関数
//=============================================================================
BOSS *GetBoss(void)
{
	return(&g_boss[0]);
}


