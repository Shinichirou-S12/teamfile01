//=============================================================================
//
// プレイヤー処理 [playerTest.cpp]
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
#define PLAYER_INVINCIVLE	(10)
#define PLAYER_MOVE_SPEED	(2.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetTexturePlayer(VERTEX_2D *Vtx, int cntPattern);
void SetVertexPlayer(VERTEX_2D *Vtx);
void animPlayerState(int * animState);

void Invincible(void);

void FallPlayer(void);
void JumpPlayer(void);
//void IdlePlayer(void);
void AttackPlayer(void);
void PlayerMoving(void);

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
	g_player.moveSpeed = PLAYER_MOVE_SPEED;

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

		Invincible();
		if (g_player.jumpForce == 0)	// プレイヤーがジャンプしていないと立ち状態になる
		{
			g_player.state = IDLE;
		}

		// キー入力で移動 
		if (GetInput(JUMPMOVE))	// ジャンプ関係
		{
			if (g_player.dropSpeed > 1 && g_player.jumpForce <= 1)	//「地上からジャンプしてからの一回だけ空中ジャンプ」と「足場から自由落下からの一回だけ空中ジャンプ」をするための判定
			{
				g_player.dropSpeed = 0;	//重力加速度を0にすることで、再びプレイヤーが上昇をはじめる
				g_player.jumpForce++;	// すぐ下のjump++と合わせてjumpが2以上になるので、どのようなタイミングでも「空中ジャンプ後はジャンプできない」
										// SE再生
				//PlaySound(SOUND_LABEL_SE_jump2);
				//playerWk[1].use = true;
			}
			else
			{
				//PlaySound(SOUND_LABEL_SE_jump);
			}

			g_player.jumpForce++;		// ジャンプの切り替え、1以上でジャンプ状態になる
			g_player.state = JUMP;		// テクスチャは「ジャンプ」
		}

		PlayerMoving();

		g_player.mapPos += g_player.scrollPos;	// どれだけスクロールしたかを保持、イベントに使用

		Restriction();
		JumpPlayer();
		FallPlayer();

		animPlayerState(&g_player.state);
		AttackPlayer();

		SetVertexPlayer(g_player.vertexWk);
		SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
	}
}

//=============================================================================
// プレイヤーの無敵状態の確認
//=============================================================================
void Invincible(void)
{
	if (g_player.invincible == true)
	{
		g_player.countInvincible++;

		if (g_player.countInvincible >= PLAYER_INVINCIVLE)
		{
			g_player.invincible = false;
			g_player.countInvincible = 0;
		}
	}
}

//=============================================================================
// プレイヤー移動
//=============================================================================
void PlayerMoving(void)
{
	// 右移動----------------------------------------
	if (GetInput(RIGHTMOVE))
	{
		g_player.pos.x += g_player.moveSpeed;
		if (g_player.pos.x >= SCREEN_WIDTH / 2)	// プレイヤーが画面中央辺りに来たらステージ自体がスクロールする
		{
			//if (playerWk[0].pos_MAP.x < boss->initialpos.x - SCREEN_CENTER_X - MAPCHIP_TEXTURE_SIZE_X - 3.0f)
			{
				g_player.scrollPos.x += g_player.moveSpeed;
			}
		}

		g_player.direction = Right;					// 左移動
		g_player.keyPressing = true;				// 
		g_player.state = RUN;						// ランニングtrue

	}
	// 左移動-----------------------------------------
	else if (GetInput(LEFTMOVE))
	{
		g_player.pos.x -= g_player.moveSpeed;
		if (g_player.pos.x <= MAP_TEXTURE_SIZE_X * (SIZE_X / 2))	// プレイヤーが画面中央辺りに来たらステージ自体がスクロールする
		{
			//if (playerWk[0].pos_MAP.x < boss->initialpos.x - SCREEN_CENTER_X - MAPCHIP_TEXTURE_SIZE_X - 3.0f)
			{
				g_player.scrollPos.x -= g_player.moveSpeed;
			}
		}

		g_player.direction = Left;					// 左移動
		g_player.keyPressing = true;				// 
		g_player.state = RUN;						// ランニングtrue

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
// プレイヤーの攻撃
//=============================================================================
void AttackPlayer(void)
{
	if (GetInput(ATTACK))
	{
		D3DXVECTOR3 pos = g_player.pos;
		if (g_player.countShot >= PLAYER_TIME_SHOT || g_player.jumpForce > 1)	// 連射用のカウントが規定値を超えているか、二段ジャンプ中なら弾発射
		{
			//SetBullet(pos, g_player.direction, g_player.jumpForce);		// 
			g_player.countShot = 0;	// 

			//PlaySound(SOUND_LABEL_SE_shot2);
		}
	}
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

//=============================================================================
// プレイヤーの横移動で画面外やブロックを貫通しないための処理
//=============================================================================
void Restriction(void)
{
	MAP *mapchip = GetMapData();
	//int *scene = GetScene();

	for (int j = 0; j < SIZE_X * SIZE_Y; j++)
	{

		switch (CheckHitBB_MAP(g_player.pos, mapchip->pos, D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y),
			D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), g_player.moveSpeed))	// ブロックのどこに触れているか
		{
		case LEFT_MAPCHIP:
			g_player.pos.x = mapchip[j].pos.x - MAP_TEXTURE_SIZE_X - PLAYER_TEXTURE_SIZE_X;	// 左からブロックに突っ込むとブロックの左に戻す
			break;
		case RIGHT_MAPCHIP:
			g_player.pos.x = mapchip[j].pos.x + MAP_TEXTURE_SIZE_X + PLAYER_TEXTURE_SIZE_X;	// 右からブロックに突っ込むとブロックの右に戻す
			break;
		case UNDER:
			g_player.pos.y = mapchip[j].pos.y + MAP_TEXTURE_SIZE_Y + PLAYER_TEXTURE_SIZE_Y;	// 下からブロックに突っ込むとブロックの下に戻す
			break;
		}
		mapchip++;
	}

	//if (*scene == SCENE_TUTORIAL || playerWk[0].pos_MAP.x >= boss->initialpos.x - SCREEN_CENTER_X - MAPCHIP_TEXTURE_SIZE_X - 3.0f)
	//{
	//	if (g_player.pos.x <= PLAYER_TEXTURE_SIZE_X)	//チュートリアル中&ボス戦は画面いっぱいに動かせる
	//	{
	//		g_player.pos.x = PLAYER_TEXTURE_SIZE_X;
	//	}

	//	if (g_player.pos.x >= SCREEN_WIDTH - PLAYER_TEXTURE_SIZE_X)	//チュートリアル中&ボス戦は画面いっぱいに動かせる
	//	{
	//		g_player.pos.x = SCREEN_WIDTH - PLAYER_TEXTURE_SIZE_X;
	//	}

	//}
	/*else*/
	{
		if (g_player.pos.x <= MAP_TEXTURE_SIZE_X * (SIZE_X / 2))	//画面左より左に行けないようにする
		{
			g_player.pos.x = MAP_TEXTURE_SIZE_X * (SIZE_X / 2);
		}

		if (g_player.pos.x >= SCREEN_WIDTH / 2)	//画面中央より右に行けないようにする
		{
			g_player.pos.x = SCREEN_WIDTH / 2;
		}
	}
}

