//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "playerTest.h"
#include "controller.h"
#include "input.h"
#include "file.h"
#include "bullet.h"
#include "checkhit.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_HP	(6)
#define PLAYER_TIME_SHOT	(5)
#define MAX_DIFFUSE	(255)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetTexturePlayer(VERTEX_2D *Vtx, int cntPattern);
void SetVertexPlayer(VERTEX_2D *Vtx);
void animPlayerState(int * animState);

void FallPlayer(void);
void JumpPlayer(void);
void IdlePlayer(void);
void AttackPlayer(void);

void Restriction(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture[STATE_MAX] = { NULL };	// テクスチャへのポリゴン

static PLAYER					g_player;		// プレイヤー構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	{
		// アイドル状態
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_IDLE,				// ファイルの名前
			&g_pD3DTexture[IDLE]);				// 読み込むメモリー

		// 移動
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_RUN,					// ファイルの名前
			&g_pD3DTexture[RUN]);				// 読み込むメモリー

		// ジャンプ
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_JUMP,				// ファイルの名前
			&g_pD3DTexture[JUMP]);				// 読み込むメモリー
	}

	g_player.invincible = false;									// 無敵状態ではない
	g_player.pos = D3DXVECTOR3(/*i*200.0f + */200.0f, 300.0f, 0.0f);// 座標データを初期化
	g_player.mapPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// マップ座標データを初期化
	g_player.scrollPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// スクロール座標データを初期化
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 回転データを初期化

	// アニメパターン番号をランダムで初期化
	g_player.animeCnt.PatternAnim = rand() % PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;	

	g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
	g_player.state = IDLE;
	g_player.Texture = g_pD3DTexture[IDLE];
	g_player.animeCnt.CountAnim = 0;								// アニメカウントを初期化
	g_player.countShot = PLAYER_TIME_SHOT;							// 最初の一発はすぐ撃てるように
	g_player.countInvincible = 0;									// 無敵カウントを初期化
	g_player.hp = PLAYER_HP;										// HPの初期化
	D3DXVECTOR2 temp = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
	g_player.radius = D3DXVec2Length(&temp);									// 半径を初期化
	g_player.baseAngle = atan2f(PLAYER_TEXTURE_SIZE_Y, PLAYER_TEXTURE_SIZE_X);	// 角度を初期化

	g_player.jumpForce = 0;										// ジャンプしていない
	g_player.dropSpeed = 0;	// 重力加速度初期化
	g_player.ofsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// オフセット座標を初期化
	g_player.direction = Right;										// プレイヤーは右向き

	MakeVertexPlayer();

	if (FAILED(MakeVertexPlayer()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < STATE_MAX; i++)
	{
		if (g_pD3DTexture[i] != NULL)
		{	// テクスチャの開放
			g_pD3DTexture[i]->Release();
			g_pD3DTexture[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (g_player.use)
	{
		if (g_player.hp <= 0 || g_player.pos.y > SCREEN_HEIGHT + PLAYER_TEXTURE_SIZE_Y * 5)
		{
			g_player.use = false;
		}

		g_player.countShot++;			// 連射のカウント用
		g_player.scrollPos.x = 0.0f;	// 画面スクロール用の変数


	}
}

//=============================================================================
// プレイヤーの落下処理
//=============================================================================
void FallPlayer(void)
{
	MAP *mapchip = GetMapData();

	g_player.pos.y += g_player.dropSpeed * 1.0f;	// 加速度的に下へ移動、重力加速度
	g_player.dropSpeed++;

	for (int i = 0; i < (SIZE_X * SIZE_Y); i++)
	{
		if ((g_player.jumpForce < 1) || g_player.dropSpeed >= PLAYER_ACCELE)	// ブロック横でジャンプするとブロック上辺に張り付くバグを抑制する処理
		{
			if (CheckHitBB_MAP(g_player.pos, mapchip->pos, D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y),
				D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), g_player.moveSpeed) == TOP)	// ブロックの上に立っているとき
			{
				g_player.dropSpeed = 0;		// 重力加速度をリセット
				g_player.pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y - PLAYER_TEXTURE_SIZE_Y;	// 上からブロックに突っ込むと、ブロックの上に戻す
				g_player.jumpForce = 0;		// ジャンプ回数をリセット
				g_player.rot.z = 0;			// 回転ジャンプの回転リセット
				break;
			}
		}
		mapchip++;
	}
}

//=============================================================================
// プレイヤーのジャンプ処理
//=============================================================================
void JumpPlayer(void)
{
	if (g_player.jumpForce >= 1)	//ジャンプしたら等速で上昇。落下速度が加速度的に増加するので時間経過で落下する
	{
		g_player.pos.y -= PLAYER_ACCELE;
		g_player.state = JUMP;		// テクスチャは「ジャンプ」


		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);

	}

	/* 着地したらジャンプを０にしてジャンプ状態を解除する。その処理はFallPlayer内にある */
}

//=============================================================================
// プレイヤーのステータスによるテクスチャ状態変化
//=============================================================================
void animPlayerState(int * animState)
{
	switch (*animState)
	{
	case IDLE:
		// Idleアニメーション
		g_player.Texture = g_pD3DTexture[IDLE];
		g_player.animeCnt.PatDivX = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_IDLE_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_IDLE_TIME_ANIMATION;

		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
		break;

	case RUN:
		// Runアニメーション
		g_player.Texture = g_pD3DTexture[RUN];
		g_player.animeCnt.PatDivX = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_RUN_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_RUN_TIME_ANIMATION;

		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
		break;

	case JUMP:
		// Jumpアニメーション
		g_player.Texture = g_pD3DTexture[JUMP];
		g_player.animeCnt.PatDivX = PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_JUMP_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_JUMP_TIME_ANIMATION;

		break;

	default:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.use == true)			// 使用している状態なら描画する
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_player.Texture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_player.vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	g_player.vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_player.vertexWk[1].vtx = D3DXVECTOR3(100.0f + PLAYER_TEXTURE_SIZE_X, 100.0f, 0.0f);
	g_player.vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + PLAYER_TEXTURE_SIZE_Y, 0.0f);
	g_player.vertexWk[3].vtx = D3DXVECTOR3(100.0f + PLAYER_TEXTURE_SIZE_X, 100.0f + PLAYER_TEXTURE_SIZE_Y, 0.0f);

	// rhwの設定
	g_player.vertexWk[0].rhw =
		g_player.vertexWk[1].rhw =
		g_player.vertexWk[2].rhw =
		g_player.vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	g_player.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);

	return S_OK;
}

//=============================================================================
// テクスチャ座標設定
//=============================================================================
void SetTexturePlayer(VERTEX_2D *Vtx, int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % g_player.animeCnt.PatDivX;
	int y = cntPattern / g_player.animeCnt.PatDivX;
	float sizeX = 1.0f / g_player.animeCnt.PatDivX;
	float sizeY = 1.0f / g_player.animeCnt.PatDivY;

	if (g_player.direction == Right)
	{
		Vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		Vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		Vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		Vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else if (g_player.direction == Left)
	{
		Vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		Vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		Vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		Vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}

	// 残りHPに応じてプレイヤーが赤くなる && 無敵時間中の点滅
	int nokoriHP = MAX_DIFFUSE / PLAYER_HP;
	int amari = MAX_DIFFUSE % PLAYER_HP;

	int flashing = (g_player.countInvincible + 1) % 2;

	Vtx[0].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
	Vtx[1].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
	Vtx[2].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
	Vtx[3].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(VERTEX_2D *Vtx)
{
	D3DXVECTOR3 pos = g_player.pos + g_player.ofsPos;

	// 頂点座標の設定

	Vtx[0].vtx.x = g_player.pos.x - cosf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[0].vtx.y = g_player.pos.y - sinf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[0].vtx.z = 0.0f;

	Vtx[1].vtx.x = g_player.pos.x + cosf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[1].vtx.y = g_player.pos.y - sinf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[1].vtx.z = 0.0f;

	Vtx[2].vtx.x = g_player.pos.x - cosf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[2].vtx.y = g_player.pos.y + sinf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[2].vtx.z = 0.0f;

	Vtx[3].vtx.x = g_player.pos.x + cosf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[3].vtx.y = g_player.pos.y + sinf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[3].vtx.z = 0.0f;
}

//=============================================================================
// プレイヤー取得関数
//=============================================================================
PLAYER *GetPlayer(void)
{
	return(&g_player);
}
