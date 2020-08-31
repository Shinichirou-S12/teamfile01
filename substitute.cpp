//=============================================================================
//
// 身代わりアイテム処理 [substitute.cpp]
// Author : kitade mayumi
//
//=============================================================================
#include "main.h"
#include "scene.h"
#include "playerTest.h"
#include "controller.h"
#include "input.h"
#include "map.h"
#include "bullet.h"
#include "checkhit.h"
#include "sound.h"
#include "substitute.h"
#include "fade.h"
#include <math.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIFFUSE	(255)
#define SUBSTITUTE_MAP_MOVE_SPEED	(10.0f)

#define SUBSTITUTE_MOVE_SPEED	(2.0f)
#define SUBSTITUTE_INIT_POSX	(30.0f)

#define SUBSTITUTE_RELEASE_TIME	(60 * 10)
#define SUBSTITUTE_MOVE_TIME	(60)
#define SUBSTITUTE_MOVE_ATTACK_SPEED	(3.5f)
enum SUBSTITUTE_STATE_ANIME
{
	IDLE,
	RUN,
	JUMP,
	RELEASE,
	STATE_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ReadTextureSubstitute(void);
void SetSubstitute(void);

HRESULT MakeVertexSubstitute(void);
void SetTextureSubstitute(VERTEX_2D *Vtx, int cntPattern);
void SetVertexSubstitute(VERTEX_2D *Vtx);
void animSubstituteState(int * animState);

void UpdateStickingSubstitute(void);
void UpdateAttackSubstitute(void);

void SlideSubstitute(void);
void ReleaseSubstitute(void);
void LenghtSubstitute(void);

void FallSubstitute(void);
void JumpSubstitute(void);
void AttackSubstitute(void);
void SubstituteMoving(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture[STATE_MAX] = { NULL };	// テクスチャへのポリゴン

static SUBSTITUTE				g_substitute;		// 身代わりアイテム構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSubstitute(void)
{
	ReadTextureSubstitute();
	int scene = GetScene();
	PLAYER *player = GetPlayer();
	g_substitute.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 元の座標データを返す

	g_substitute.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 回転データを初期化

	g_substitute.moveSpeed = SUBSTITUTE_MOVE_SPEED;
	g_substitute.use = true;
	g_substitute.releaseUse = false;
	g_substitute.sticking = false;
	g_substitute.attackUse = false;

	// アニメパターン番号をランダムで初期化
	g_substitute.animeCnt.PatternAnim = rand() % SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_X;

	g_substitute.textureSize = D3DXVECTOR2(SUBSTITUTE_TEXTURE_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y);

	g_substitute.state = IDLE;
	g_substitute.Texture = g_pD3DTexture[g_substitute.state];
	g_substitute.lenght = 0.0f;		// プレイヤーとの位置関係

	g_substitute.animeCnt.PatDivX = SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの内分割数Xを初期化
	g_substitute.animeCnt.PatDivY = SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの内分割数Yを初期化
	g_substitute.animeCnt.AnimPatNum = SUBSTITUTE_IDLE_ANIM_PATTERN_NUM;		// アニメーションパターン数を初期化
	g_substitute.animeCnt.TimeAnim = SUBSTITUTE_IDLE_TIME_ANIMATION;			// アニメーションの切り替わるカウントを初期化
	g_substitute.animeCnt.PatternAnim = 0;									// アニメパターン番号初期化

	g_substitute.animeCnt.CountAnim = 0;								// アニメカウントを初期化
	g_substitute.countReleaseTime = 0;
	g_substitute.moveCount = 0;											// 移動量のカウント初期化

	D3DXVECTOR2 temp = D3DXVECTOR2(SUBSTITUTE_TEXTURE_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y);
	g_substitute.radius = D3DXVec2Length(&temp);									// 半径を初期化
	g_substitute.baseAngle = atan2f(SUBSTITUTE_TEXTURE_SIZE_Y, SUBSTITUTE_TEXTURE_SIZE_X);	// 角度を初期化

	g_substitute.jumpForce = 0;	// ジャンプしていない
	g_substitute.dropSpeed = 0;	// 重力加速度初期化

	g_substitute.ofsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// オフセット座標を初期化
	g_substitute.direction = player->direction;				// 身代わりアイテムはプレイヤーと同じ向き

	MakeVertexSubstitute();

	SetSubstitute();

	if (FAILED(MakeVertexSubstitute()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSubstitute(void)
{
	for (int i = 0; i < (STATE_MAX - 1); i++)
	{
		if (g_pD3DTexture[i] != NULL)
		{	// テクスチャの開放
			g_pD3DTexture[i]->Release();
			g_pD3DTexture[i] = NULL;
		}
	}
}

//=============================================================================
// 身代わりアイテムのくっついているときの更新処理
//=============================================================================
void UpdateStickingSubstitute(void)
{
	if (g_substitute.sticking)
	{
		// キー入力で移動 
		if (GetInput(JUMPMOVE))	// ジャンプ関係
		{
			if (g_substitute.dropSpeed > 1 && g_substitute.jumpForce <= 1)	//「地上からジャンプしてからの一回だけ空中ジャンプ」と「足場から自由落下からの一回だけ空中ジャンプ」をするための判定
			{
				g_substitute.dropSpeed = 0;	//重力加速度を0にすることで、再び身代わりアイテムが上昇をはじめる
				g_substitute.jumpForce++;	// すぐ下のjump++と合わせてjumpが2以上になるので、どのようなタイミングでも「空中ジャンプ後はジャンプできない」
										// SE再生
				//PlaySound(SOUND_LABEL_SE_jump2);
			}
			else
			{
				//PlaySound(SOUND_LABEL_SE_jump);
			}

			g_substitute.jumpForce++;		// ジャンプの切り替え、1以上でジャンプ状態になる
			g_substitute.state = JUMP;		// テクスチャは「ジャンプ」
		}
		SubstituteMoving();
		//SlideSubstitute();

		JumpSubstitute();

		AttackSubstitute();

	}
}

//=============================================================================
// 身代わりアイテムの攻撃している時の更新処理
//=============================================================================
void UpdateAttackSubstitute(void)
{
	// sticking = false; Attackによって離れた状態になった
	if (g_substitute.attackUse && !g_substitute.sticking)
	{
		if (g_substitute.direction == Right)
		{
			g_substitute.pos.x += g_substitute.moveSpeed * SUBSTITUTE_MOVE_ATTACK_SPEED;
		}
		else
		{
			g_substitute.pos.x -= g_substitute.moveSpeed * SUBSTITUTE_MOVE_ATTACK_SPEED;
		}
		g_substitute.moveCount++;
	}

	// 最大移動量に達した時攻撃状態でなくなる（それ以上の飛距離がない状態）
	if (g_substitute.moveCount >= SUBSTITUTE_MOVE_TIME)
	{
		g_substitute.moveCount = 0;
		g_substitute.attackUse = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSubstitute(void)
{
	int scene = GetScene();
	PLAYER *player = GetPlayer();

	if (g_substitute.use)
	{
		// アニメーションのパターンを更新------------------
		g_substitute.animeCnt.CountAnim++;

		// 今のフレームが時間切れたら
		if ((g_substitute.animeCnt.CountAnim % g_substitute.animeCnt.TimeAnim) == 0)
		{
			//cntリセット
			g_substitute.animeCnt.CountAnim = 0;
			// 次のフレームに進む
			g_substitute.animeCnt.PatternAnim = (g_substitute.animeCnt.PatternAnim + 1) % g_substitute.animeCnt.AnimPatNum;
		}

		if (g_substitute.jumpForce == 0)	// 身代わりアイテムがジャンプしていないと立ち状態になる
		{
			g_substitute.state = IDLE;
		}

		ReleaseSubstitute();
		UpdateStickingSubstitute();
		RestrictionSubstitute();
		FallSubstitute();
		LenghtSubstitute();

		UpdateAttackSubstitute();
		CheckHitEnemySubstitute();

		animSubstituteState(&g_substitute.state);

		SetVertexSubstitute(g_substitute.vertexWk);
		SetTextureSubstitute(g_substitute.vertexWk, g_substitute.animeCnt.PatternAnim);
	}
}

//=============================================================================
// 身代わりアイテムのテクスチャ読み込み
//=============================================================================
void ReadTextureSubstitute(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// アイドル状態
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_SUBSTITUTE_IDLE,		// ファイルの名前
		&g_pD3DTexture[IDLE]);			// 読み込むメモリー
	// 歩行状態
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_SUBSTITUTE_RUN,			// ファイルの名前
		&g_pD3DTexture[RUN]);			// 読み込むメモリー
	// ジャンプ状態
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_SUBSTITUTE_JUMP,		// ファイルの名前
		&g_pD3DTexture[JUMP]);			// 読み込むメモリー

}

//=============================================================================
// 身代わりアイテム移動
//=============================================================================
void SubstituteMoving(void)
{
	// 右移動----------------------------------------
	if (GetInput(RIGHTMOVE))
	{
		if (DownSpeed())
		{
			g_substitute.pos.x += g_substitute.moveSpeed / 2.0f;
		}
		else
		{
			g_substitute.pos.x += g_substitute.moveSpeed;
		}

		g_substitute.direction = Right;					// 左移動
		g_substitute.state = RUN;						// ランニングtrue

	}
	// 左移動-----------------------------------------
	else if (GetInput(LEFTMOVE))
	{
		if (DownSpeed())
		{
			g_substitute.pos.x -= g_substitute.moveSpeed / 2.0f;
		}
		else
		{
			g_substitute.pos.x -= g_substitute.moveSpeed;
		}
		g_substitute.direction = Left;					// 左移動
		g_substitute.state = RUN;						// ランニングtrue

	}
}

//=============================================================================
// 身代わりアイテムの滑る処理
//=============================================================================
void SlideSubstitute(void)
{
	PLAYER *player = GetPlayer();
	if (SlidePlayer())
	{
		if (g_substitute.direction == Right)
		{
			g_substitute.pos.x += (PLAYER_SLIDE_MAX - (player->slideCnt * 1.0f));
		}
		else
		{
			g_substitute.pos.x -= (PLAYER_SLIDE_MAX - (player->slideCnt * 1.0f));
		}
	}
}

//=============================================================================
// 身代わりアイテムの落下処理
//=============================================================================
void FallSubstitute(void)
{
	g_substitute.pos.y += g_substitute.dropSpeed * 0.7f;	// 加速度的に下へ移動、重力加速度
	g_substitute.dropSpeed++;

	FallCheckHitSubstitute();
}

//=============================================================================
// 身代わりアイテムのジャンプ処理
//=============================================================================
void JumpSubstitute(void)
{
	if (g_substitute.jumpForce >= 1)	//ジャンプしたら等速で上昇。落下速度が加速度的に増加するので時間経過で落下する
	{
		if (DownSpeed())
		{
			g_substitute.pos.y -= SUBSTITUTE_ACCELE / 2;
		}
		else
		{
			g_substitute.pos.y -= SUBSTITUTE_ACCELE;
		}
		g_substitute.state = JUMP;		// テクスチャは「ジャンプ」

		g_substitute.textureSize = D3DXVECTOR2(SUBSTITUTE_TEXTURE_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y);

	}
	/* 着地したらジャンプを０にしてジャンプ状態を解除する。その処理はFallSubstitute内にある */
}

//=============================================================================
// 身代わりアイテムの攻撃
//=============================================================================
void AttackSubstitute(void)
{
	if (GetInput(ATTACK))
	{
		D3DXVECTOR3 pos = g_substitute.pos;
		//PlaySound(SOUND_LABEL_SE_shot2);
		g_substitute.releaseUse = true;
		g_substitute.sticking = false;
		g_substitute.attackUse = true;
	}
}

//=============================================================================
// 身代わりアイテムのリリース状態の時間変化
//=============================================================================
void ReleaseSubstitute(void)
{
	if (g_substitute.releaseUse == true
		&& !g_substitute.attackUse)
	{
		g_substitute.countReleaseTime++;

		if (g_substitute.countReleaseTime >= SUBSTITUTE_RELEASE_TIME)
		{
			if (!g_substitute.sticking)
			{
				g_substitute.use = false;
			}
		}
	}
	else
	{
		g_substitute.countReleaseTime = 0;
	}
}

//=============================================================================
// 身代わりアイテムのステータスによるテクスチャ状態変化
//=============================================================================
void animSubstituteState(int * animState)
{
	switch (*animState)
	{
	case IDLE:
	case RELEASE:
		// Idleアニメーション
		g_substitute.Texture = g_pD3DTexture[*animState];
		g_substitute.animeCnt.PatDivX = SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_X;
		g_substitute.animeCnt.PatDivY = SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_Y;
		g_substitute.animeCnt.AnimPatNum = SUBSTITUTE_IDLE_ANIM_PATTERN_NUM;
		g_substitute.animeCnt.TimeAnim = SUBSTITUTE_IDLE_TIME_ANIMATION;

		g_substitute.textureSize = D3DXVECTOR2(SUBSTITUTE_TEXTURE_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y);
		break;

	case RUN:
		// Runアニメーション
		g_substitute.Texture = g_pD3DTexture[*animState];
		g_substitute.animeCnt.PatDivX = SUBSTITUTE_RUN_TEXTURE_PATTERN_DIVIDE_X;
		g_substitute.animeCnt.PatDivY = SUBSTITUTE_RUN_TEXTURE_PATTERN_DIVIDE_Y;
		g_substitute.animeCnt.AnimPatNum = SUBSTITUTE_RUN_ANIM_PATTERN_NUM;
		g_substitute.animeCnt.TimeAnim = SUBSTITUTE_RUN_TIME_ANIMATION;

		g_substitute.textureSize = D3DXVECTOR2(SUBSTITUTE_TEXTURE_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y);
		break;

	case JUMP:
		// Jumpアニメーション
		g_substitute.Texture = g_pD3DTexture[*animState];
		g_substitute.animeCnt.PatDivX = SUBSTITUTE_JUMP_TEXTURE_PATTERN_DIVIDE_X;
		g_substitute.animeCnt.PatDivY = SUBSTITUTE_JUMP_TEXTURE_PATTERN_DIVIDE_Y;
		g_substitute.animeCnt.AnimPatNum = SUBSTITUTE_JUMP_ANIM_PATTERN_NUM;
		g_substitute.animeCnt.TimeAnim = SUBSTITUTE_JUMP_TIME_ANIMATION;

		break;

	default:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSubstitute(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_substitute.use == true)			// 使用している状態なら描画する
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_substitute.Texture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_substitute.vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSubstitute(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	g_substitute.vertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_substitute.vertexWk[1].vtx = D3DXVECTOR3(0.0f + SUBSTITUTE_TEXTURE_SIZE_X, 0.0f, 0.0f);
	g_substitute.vertexWk[2].vtx = D3DXVECTOR3(0.0f, 0.0f + SUBSTITUTE_TEXTURE_SIZE_Y, 0.0f);
	g_substitute.vertexWk[3].vtx = D3DXVECTOR3(0.0f + SUBSTITUTE_TEXTURE_SIZE_X, 0.0f + SUBSTITUTE_TEXTURE_SIZE_Y, 0.0f);

	// rhwの設定
	g_substitute.vertexWk[0].rhw =
		g_substitute.vertexWk[1].rhw =
		g_substitute.vertexWk[2].rhw =
		g_substitute.vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	g_substitute.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_substitute.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_substitute.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_substitute.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	SetTextureSubstitute(g_substitute.vertexWk, g_substitute.animeCnt.PatternAnim);

	return S_OK;
}

//=============================================================================
// テクスチャ座標設定
//=============================================================================
void SetTextureSubstitute(VERTEX_2D *Vtx, int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % g_substitute.animeCnt.PatDivX;
	int y = cntPattern / g_substitute.animeCnt.PatDivX;
	float sizeX = 1.0f / g_substitute.animeCnt.PatDivX;
	float sizeY = 1.0f / g_substitute.animeCnt.PatDivY;

	if (g_substitute.direction == Right)
	{
		Vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		Vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		Vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		Vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else if (g_substitute.direction == Left)
	{
		Vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		Vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		Vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		Vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}

	int flashing = (g_substitute.countReleaseTime + 1) % 2;

	if (g_substitute.countReleaseTime > 0)
	{
		Vtx[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, flashing * 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, flashing * 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, flashing * 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, flashing * 255);
	}

	else
	{
		Vtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexSubstitute(VERTEX_2D *Vtx)
{
	//D3DXVECTOR3 pos = g_substitute.pos + g_substitute.ofsPos;

	// 頂点座標の設定

	Vtx[0].vtx.x = g_substitute.pos.x - cosf(g_substitute.baseAngle + g_substitute.rot.z) * g_substitute.radius;
	Vtx[0].vtx.y = g_substitute.pos.y - sinf(g_substitute.baseAngle + g_substitute.rot.z) * g_substitute.radius;
	Vtx[0].vtx.z = 0.0f;

	Vtx[1].vtx.x = g_substitute.pos.x + cosf(g_substitute.baseAngle - g_substitute.rot.z) * g_substitute.radius;
	Vtx[1].vtx.y = g_substitute.pos.y - sinf(g_substitute.baseAngle - g_substitute.rot.z) * g_substitute.radius;
	Vtx[1].vtx.z = 0.0f;

	Vtx[2].vtx.x = g_substitute.pos.x - cosf(g_substitute.baseAngle - g_substitute.rot.z) * g_substitute.radius;
	Vtx[2].vtx.y = g_substitute.pos.y + sinf(g_substitute.baseAngle - g_substitute.rot.z) * g_substitute.radius;
	Vtx[2].vtx.z = 0.0f;

	Vtx[3].vtx.x = g_substitute.pos.x + cosf(g_substitute.baseAngle + g_substitute.rot.z) * g_substitute.radius;
	Vtx[3].vtx.y = g_substitute.pos.y + sinf(g_substitute.baseAngle + g_substitute.rot.z) * g_substitute.radius;
	Vtx[3].vtx.z = 0.0f;
}

//=============================================================================
// 身代わりアイテム取得関数
//=============================================================================
SUBSTITUTE *GetSubstitute(void)
{
	return(&g_substitute);
}

//=============================================================================
// 身代わりアイテムのセット処理（位置）
//=============================================================================
void SetSubstitute(void)
{
	MAP *map = GetMapData();

	for (int p = 0; p < MAP_MAXDATA * SIZE_X * SIZE_Y; p++, map++)
	{
		if (map->type == ITEM18)
		{
			g_substitute.Texture = g_pD3DTexture[IDLE];
			g_substitute.pos = map->pos;
			g_substitute.use = true;
			break;
		}
	}
}

//=============================================================================
// 身代わりアイテムのプレイヤーとの距離の処理（プレイヤーの位置に合わせる）
//=============================================================================
void LenghtSubstitute(void)
{
	PLAYER *player = GetPlayer();
	float vecLenghtX = 0.0f;
	float vecLenghtY = 0.0f;

	vecLenghtX = g_substitute.pos.x - player->pos.x;
	vecLenghtY = fabsf(g_substitute.pos.y - player->pos.y);
	g_substitute.lenght = fabsf(g_substitute.pos.x - player->pos.x);
	if (g_substitute.sticking && g_substitute.attackUse == false)
	{
		if (g_substitute.lenght > SUBSTITUTE_INIT_POSX
			&& vecLenghtY <= 0.0f)
		{
			g_substitute.state = RUN;

			if (vecLenghtX < 0)
			{
				g_substitute.pos.x += g_substitute.moveSpeed;
			}
			else
			{
				g_substitute.pos.x -= g_substitute.moveSpeed;
			}
		}
		else if(vecLenghtY > 0.0f)
		{
			g_substitute.sticking = false;
			g_substitute.releaseUse = true;
		}
	}
}

//=============================================================================
// 身代わりアイテムのセット処理（プレイヤーの位置に合わせる）
//=============================================================================
void SetPosSubstitute(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(SUBSTITUTE_INIT_POSX, 0.0f, 0.0f);
	PLAYER *player = GetPlayer();
	if (player->direction == Right)
	{
		D3DXVec3Subtract(&g_substitute.pos, &player->pos, &pos);

	}
	else if (player->direction == Left)
	{
		D3DXVec3Add(&g_substitute.pos, &player->pos, &pos);

	}
}
