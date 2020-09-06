//=============================================================================
//
// プレイヤー処理 [playerTest.cpp]
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
#include "life.h"
#include "enemy.h"
#include "item.h"
#include "wall.h"
#include "fade.h"
#include "sound.h"
#include "enemyBullet.h"
#include "spear.h"
#include "substitute.h"
#include "boss.h"
#include "killer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_TIME_SHOT	(CHANGE_TIME)
#define MAX_DIFFUSE	(255)
#define PLAYER_STAR_INVINCIVLE	(60 * 5)
#define PLAYER_INVINCIVLE	(60 * 3)
#define PLAYER_DOWNSTATE	(60 * 5)
#define PLAYER_MAP_MOVE_SPEED	(10.0f)

#define PLAYER_MOVE_SPEED	(2.0f)

enum PLAYER_STATE_ANIME
{
	IDLE,
	RUN,
	JUMP,
	STATE_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ReadTexturePlayer(void);

HRESULT MakeVertexPlayer(void);
void SetTexturePlayer(VERTEX_2D *Vtx, int cntPattern);
void SetVertexPlayer(VERTEX_2D *Vtx);
void animPlayerState(int * animState, int * partsState);

void Invincible(void);
void StarInvincible(void);
void FallPlayer(void);
void JumpPlayer(void);
void AttackPlayer(void);
void PlayerMoving(void);

void Restriction(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture[STATE_MAX][MAXPARTS] = { NULL };	// テクスチャへのポリゴン

static PLAYER					g_player;		// プレイヤー構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	ReadTexturePlayer();
	int scene = GetScene();

	g_player.invincible = false;									// 無敵状態ではない

	if (scene == SCENE_TUTRIAL)
	{
		g_player.pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
	}

	if (scene == SCENE_GAME)
	{
		g_player.pos = g_player.scrollPos;							// 座標データを初期化
		//g_player.ofsPos = g_player.pos;
	}
	else if(scene == SCENE_BONUS)
	{
		g_player.scrollPos = g_player.pos;							// 元の座標データを保持
		//g_player.pos = g_player.ofsPos;							// 座標データを初期化
		g_player.pos = g_player.scrollPos;							// 元の座標データを返す
	}

	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 回転データを初期化

	g_player.moveSpeed = PLAYER_MOVE_SPEED;
	g_player.use = true;
	g_player.scroll = false;
	g_player.bogUse = false;
	g_player.superInvincible = false;
	g_player.countMove = 0;
	g_player.countScroll = 0;

	// アニメパターン番号をランダムで初期化
	g_player.animeCnt.PatternAnim = rand() % PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;	

	g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
	g_player.checkTopTexSize = PLAYER_TEXTURE_BB_SIZE_TOP_X;

	g_player.state = IDLE;
	g_player.Texture = g_pD3DTexture[g_player.state][g_player.partsState];

	g_player.animeCnt.PatDivX = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの内分割数Xを初期化
	g_player.animeCnt.PatDivY = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの内分割数Yを初期化
	g_player.animeCnt.AnimPatNum = PLAYER_IDLE_ANIM_PATTERN_NUM;		// アニメーションパターン数を初期化
	g_player.animeCnt.TimeAnim = PLAYER_IDLE_TIME_ANIMATION;			// アニメーションの切り替わるカウントを初期化
	g_player.animeCnt.PatternAnim = 0;									// アニメパターン番号初期化

	g_player.animeCnt.CountAnim = 0;								// アニメカウントを初期化
	g_player.countShot = PLAYER_TIME_SHOT;							// 最初の一発はすぐ撃てるように
	g_player.countInvincible = 0;									// 無敵カウントを初期化
	g_player.countSuperInvincible = 0;								// スター無敵カウントを初期化

	D3DXVECTOR2 temp = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
	g_player.radius = D3DXVec2Length(&temp);									// 半径を初期化
	g_player.baseAngle = atan2f(PLAYER_TEXTURE_SIZE_Y, PLAYER_TEXTURE_SIZE_X);	// 角度を初期化

	g_player.slideCnt = 0;	// 滑ったカウントの初期化
	g_player.jumpForce = 0;	// ジャンプしていない
	g_player.dropSpeed = 0;	// 重力加速度初期化
	g_player.countBog = 0;	// 泥状態の初期化

	g_player.ofsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// オフセット座標を初期化
	g_player.direction = Right;							// プレイヤーは右向き

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
		for (int j = 0; j < MAXPARTS; j++)
		{
			if (g_pD3DTexture[i][j] != NULL)
			{	// テクスチャの開放
				g_pD3DTexture[i][j]->Release();
				g_pD3DTexture[i][j] = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	int scene = GetScene();
	if (!g_player.use)
	{
		SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_GAMESTAGE);
		return;
	}
	
	if (g_player.warpUse && GetInput(DOWNMOVE))
	{
		if (scene == SCENE_GAME)
		{
			SetFade(FADE_OUT, SCENE_BONUS, SOUND_LABEL_BGM_GAMESTAGE);
			PlaySound(SOUND_LABEL_SE_WARP);
			g_player.warpUse = false;
		}
		else
		{
			SetFade(FADE_OUT, SCENE_GAME, SOUND_LABEL_BGM_BOSS_STAGE);
			PlaySound(SOUND_LABEL_SE_WARP);
		}
		return;
	}

	// アニメーションのパターンを更新------------------
	g_player.animeCnt.CountAnim++;

	// 今のフレームが時間切れたら
	if ((g_player.animeCnt.CountAnim % g_player.animeCnt.TimeAnim) == 0)
	{
		//cntリセット
		g_player.animeCnt.CountAnim = 0;
		// 次のフレームに進む
		g_player.animeCnt.PatternAnim = (g_player.animeCnt.PatternAnim + 1) % g_player.animeCnt.AnimPatNum;
	}


	g_player.countShot++;			// 連射のカウント用

	Invincible();
	StarInvincible();

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
			PlaySound(SOUND_LABEL_SE_JUMP);
			//playerWk[1].use = true;
		}
		else
		{
			// SE再生
			PlaySound(SOUND_LABEL_SE_JUMP);
		}

		g_player.jumpForce++;		// ジャンプの切り替え、1以上でジャンプ状態になる
		g_player.state = JUMP;		// テクスチャは「ジャンプ」
	}

	// 画面の端に行ったらスクロール処理を開始する
	if (g_player.scroll)
	{
		MAP *mapchip = GetMapData();
		ENEMY *enemy = GetEnemy();
		ITEM * item = GetItem(0);
		WALL *wall = GetWall();
		SPEAR *spear = GetSpear(0);
		ENEMYBULLET *enemBullet = GetEnemyBullet(0);
		SUBSTITUTE *substitute = GetSubstitute();
		BOSS *boss = GetBoss();
		KILLER *killer = GetKiller(0);

		if(g_player.countMove != (SCREEN_WIDTH / (int)PLAYER_MAP_MOVE_SPEED))
		{
			g_player.pos.x -= PLAYER_MAP_MOVE_SPEED;
			substitute->pos.x -= PLAYER_MAP_MOVE_SPEED;

			if (scene == SCENE_GAME || scene == SCENE_TUTRIAL)
			{
				wall->pos.x -= PLAYER_MAP_MOVE_SPEED;
				for (int k = 0; k < ENEMY_MAX; k++, enemy++)
				{
					enemy->pos.x -= PLAYER_MAP_MOVE_SPEED;
				}

				for (int i = 0; i < BOSS_MAX; i++, boss++)
				{
					boss->pos.x -= PLAYER_MAP_MOVE_SPEED;
				}
			}
			for (int i = 0; i < SPEAR_MAX; i++, spear++)
			{
				spear->pos.x -= PLAYER_MAP_MOVE_SPEED;
			}

			for (int a = 0; a < BULLET_MAX; a++, enemBullet++)
			{
				enemBullet->use = false;
			}

			for (int s = 0; s < ITEM_MAX; s++, item++)
			{
				item->pos.x -= PLAYER_MAP_MOVE_SPEED;
			}

			for (int l = 0; l < KILLER_MAX; l++,killer++)
			{
				killer->pos.x -= PLAYER_MAP_MOVE_SPEED;
			}


			for (int j = 0; j < (SIZE_X * SIZE_Y * MAP_MAXDATA); j++)
			{
				mapchip->pos.x -= PLAYER_MAP_MOVE_SPEED;
				mapchip++;
			}

		}
		g_player.countMove++;

		if (g_player.countMove == (SCREEN_WIDTH / (int)PLAYER_MAP_MOVE_SPEED))
		{
			g_player.scroll = false;
			g_player.countMove = 0;
			if (scene == SCENE_GAME)
			{
				g_player.countScroll++;
			}
		}

	}

	PlayerMoving();

	//g_player.mapPos += g_player.scrollPos;	// どれだけスクロールしたかを保持、イベントに使用

	Restriction();
	JumpPlayer();
	FallPlayer();

	if (g_player.hp > 1)
	{
		AttackPlayer();
	}

	CheckHitItem();
	CheckHitEnemy();
	CheckHitBoss();
	CheckPlayerBullet();
	CheckSpear();
	CheckHitPlayerSubstitute();
	CheckHitGoal();

	g_player.warpUse = CheckHitWarp();

	if (g_player.hp <= 0 || g_player.pos.y > SCREEN_HEIGHT + PLAYER_TEXTURE_SIZE_Y * 5)
	{
		g_player.use = false;
	}

	animPlayerState(&g_player.state, &g_player.partsState);

	SetVertexPlayer(g_player.vertexWk);
	SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
}

//=============================================================================
// プレイヤーのテクスチャ読み込み
//=============================================================================
void ReadTexturePlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// アイドル状態
	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_PERFECT_IDLE,		// ファイルの名前
		&g_pD3DTexture[IDLE][PERFECT]);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_THREE_IDLE,			// ファイルの名前
		&g_pD3DTexture[IDLE][THREE]);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_TWO_IDLE,			// ファイルの名前
		&g_pD3DTexture[IDLE][TWO]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_ONE_IDLE,			// ファイルの名前
		&g_pD3DTexture[IDLE][ONE]);			// 読み込むメモリー


	// 移動
	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_PERFECT_RUN,			// ファイルの名前
		&g_pD3DTexture[RUN][PERFECT]);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_THREE_RUN,			// ファイルの名前
		&g_pD3DTexture[RUN][THREE]);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_TWO_RUN,				// ファイルの名前
		&g_pD3DTexture[RUN][TWO]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_ONE_RUN,				// ファイルの名前
		&g_pD3DTexture[RUN][ONE]);			// 読み込むメモリー

	// ジャンプ
	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_PERFECT_JUMP,		// ファイルの名前
		&g_pD3DTexture[JUMP][PERFECT]);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_THREE_JUMP,			// ファイルの名前
		&g_pD3DTexture[JUMP][THREE]);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_TWO_JUMP,			// ファイルの名前
		&g_pD3DTexture[JUMP][TWO]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_PLAYER_ONE_JUMP,			// ファイルの名前
		&g_pD3DTexture[JUMP][ONE]);			// 読み込むメモリー

}

//=============================================================================
// プレイヤーのステータスダウン状態
//=============================================================================
bool DownSpeed(void)
{
	if (g_player.bogUse == true)
	{
		g_player.countBog++;

		if (g_player.countBog >= PLAYER_DOWNSTATE)
		{
			g_player.bogUse = false;
			g_player.countBog = 0;
		}
		if (!g_player.superInvincible)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// プレイヤーのスターの無敵状態の確認
//=============================================================================
void StarInvincible(void)
{
	if (g_player.superInvincible == true)
	{
		g_player.countSuperInvincible++;

		if (g_player.countSuperInvincible >= PLAYER_STAR_INVINCIVLE)
		{
			g_player.superInvincible = false;
			g_player.countSuperInvincible = 0;
		}
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
		if (DownSpeed())
		{
			g_player.pos.x += g_player.moveSpeed / 2.0f;
		}
		else
		{
			g_player.pos.x += g_player.moveSpeed;
		}

		g_player.direction = Right;					// 左移動
		g_player.state = RUN;						// ランニングtrue

	}
	// 左移動-----------------------------------------
	else if (GetInput(LEFTMOVE))
	{
		if (DownSpeed())
		{
			g_player.pos.x -= g_player.moveSpeed / 2.0f;
		}
		else
		{
			g_player.pos.x -= g_player.moveSpeed;
		}
		g_player.direction = Left;					// 左移動
		g_player.state = RUN;						// ランニングtrue

	}
}

//=============================================================================
// プレイヤーの滑る処理
//=============================================================================
bool SlidePlayer(void)
{
	if (g_player.slideCnt <= PLAYER_SLIDE_MAX)
	{
		if (g_player.direction == Right)
		{
			g_player.pos.x += (PLAYER_SLIDE_MAX - (g_player.slideCnt * 1.0f));
		}
		else
		{
			g_player.pos.x -= (PLAYER_SLIDE_MAX - (g_player.slideCnt * 1.0f));
		}
		g_player.slideCnt++;
		return true;
	}
	else
	{
		g_player.slideCnt = 0;
		return false;
	}
}

//=============================================================================
// プレイヤーの落下処理
//=============================================================================
void FallPlayer(void)
{
	g_player.pos.y += g_player.dropSpeed * 1.0f;	// 加速度的に下へ移動、重力加速度
	g_player.dropSpeed++;

	FallCheckHitPlayer();
}

//=============================================================================
// プレイヤーのジャンプ処理
//=============================================================================
void JumpPlayer(void)
{
	if (g_player.jumpForce >= 1)	//ジャンプしたら等速で上昇。落下速度が加速度的に増加するので時間経過で落下する
	{
		if (DownSpeed())
		{
			g_player.pos.y -= PLAYER_ACCELE / 2;
		}
		else
		{
			g_player.pos.y -= PLAYER_ACCELE;
		}
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
	CHANGE_LIFE *life = GetLifeState();
	SUBSTITUTE *substitute = GetSubstitute();
	if (GetInput(ATTACK) && !substitute->sticking)
	{
		D3DXVECTOR3 pos = g_player.pos;
		if (g_player.countShot >= PLAYER_TIME_SHOT || g_player.jumpForce > 1)	// 連射用のカウントが規定値を超えているか、二段ジャンプ中なら弾発射
		{
			SetBullet(g_player.pos, g_player.direction);
			g_player.countShot = 0;	// 
			g_player.hp--;
			g_player.partsState--;
			ChangeLife(-1);
			PlaySound(SOUND_LABEL_SE_SHOT01);
		}
	}
}

//=============================================================================
// プレイヤーのステータスによるテクスチャ状態変化
//=============================================================================
void animPlayerState(int * animState, int * partsState)
{
	switch (*animState)
	{
	case IDLE:
		// Idleアニメーション
		g_player.Texture = g_pD3DTexture[*animState][*partsState];
		g_player.animeCnt.PatDivX = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_IDLE_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_IDLE_TIME_ANIMATION;

		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
		break;

	case RUN:
		// Runアニメーション
		g_player.Texture = g_pD3DTexture[*animState][*partsState];
		g_player.animeCnt.PatDivX = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_RUN_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_RUN_TIME_ANIMATION;

		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
		break;

	case JUMP:
		// Jumpアニメーション
		g_player.Texture = g_pD3DTexture[*animState][*partsState];
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

	if (g_player.use == true || g_player.hp > 0)			// 使用している状態なら描画する
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

	if (!g_player.superInvincible)
	{
		Vtx[0].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
	}
	else
	{
		Vtx[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	}

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(VERTEX_2D *Vtx)
{
	//D3DXVECTOR3 pos = g_player.pos + g_player.ofsPos;

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
