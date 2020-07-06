//=============================================================================
//
// player.cpp
// Author : 
//
//=============================================================================
#include "main.h"
#include "checkhit.h"
#include "controller.h"
#include "file.h"
#include "player.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COLLISON_SIZE_Y			(PLAYER_SIZE_STAND_Y - 15)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetTexturePlayer(VERTEX_2D *Vtx, int cntPattern);
void SetVertexPlayer(D3DXVECTOR2 * textureSize);

void PlayerAnimeUpdate(void);
void PlayerFalling(void);					//プレイヤー落下
void PlayerJump(void);						//メインキャラジャンプ
void PlayerMoving(void);					//メインキャラ移動
void PlayerIdling(void);					//メインキャラIDLE
void PlayerAttack(void);					//メインキャラ攻撃
void SetPlayerAnimation(void);				//アニメーション設定
void CheckCameraFollowPlayer(void);			//カメラついて

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3D_Texture[STATE_MAX] = { NULL };	// テクスチャへのポインタ

static LPDIRECT3DTEXTURE9		g_pD3D_AtkTexture[Atk_Max] = { NULL };	// テクスチャへのポインタ
static PLAYER_ANIM_DATE			g_AtkAniDate[Atk_Max] = {				// テクスチャへのポインタ
	{PLAYER_ATK1_TEXTURE_PATTERN_DIVIDE_X, PLAYER_ATK1_TEXTURE_PATTERN_DIVIDE_Y, PLAYER_ATK1_ANIM_PATTERN_NUM, PLAYER_ATK1_TIME_ANIMATION},
	{PLAYER_ATK2_TEXTURE_PATTERN_DIVIDE_X, PLAYER_ATK2_TEXTURE_PATTERN_DIVIDE_Y, PLAYER_ATK2_ANIM_PATTERN_NUM, PLAYER_ATK2_TIME_ANIMATION},
	{PLAYER_ATK3_TEXTURE_PATTERN_DIVIDE_X, PLAYER_ATK3_TEXTURE_PATTERN_DIVIDE_Y, PLAYER_ATK3_ANIM_PATTERN_NUM, PLAYER_ATK3_TIME_ANIMATION},
	{PLAYER_ATK4_TEXTURE_PATTERN_DIVIDE_X, PLAYER_ATK4_TEXTURE_PATTERN_DIVIDE_Y, PLAYER_ATK4_ANIM_PATTERN_NUM, PLAYER_ATK4_TIME_ANIMATION},
	{PLAYER_ATK5_TEXTURE_PATTERN_DIVIDE_X, PLAYER_ATK5_TEXTURE_PATTERN_DIVIDE_Y, PLAYER_ATK5_ANIM_PATTERN_NUM, PLAYER_ATK5_TIME_ANIMATION},
};

static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff = NULL;					// 頂点バッファへのポインタ
static PLAYER				g_player;								// ポリゴンデータ

//*****************************************************************************
// 関数
//*****************************************************************************
void SetVertexManChara(void);

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// ひま
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_IDLE,				// ファイルの名前
			&g_pD3D_Texture[IDLE]);				// 読み込むメモリー

		// 移動
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_RUN,					// ファイルの名前
			&g_pD3D_Texture[RUN]);				// 読み込むメモリー

		// ジャンプ
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_JUMP,				// ファイルの名前
			&g_pD3D_Texture[JUMP]);				// 読み込むメモリー

		// 落ちる
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_FALLING,				// ファイルの名前
			&g_pD3D_Texture[FALLING]);			// 読み込むメモリー

		// 攻撃-------------------------
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_ATK1,				// ファイルの名前
			&g_pD3D_Texture[ATTACK1]);			// 読み込むメモリー

		// 攻撃-------------------------
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_ATK1,				// ファイルの名前
			&g_pD3D_AtkTexture[Atk_Pat1]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_ATK2,				// ファイルの名前
			&g_pD3D_AtkTexture[Atk_Pat2]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_ATK3,				// ファイルの名前
			&g_pD3D_AtkTexture[Atk_Pat3]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_ATK4,				// ファイルの名前
			&g_pD3D_AtkTexture[Atk_Pat4]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PLAYER_ATK5,				// ファイルの名前
			&g_pD3D_AtkTexture[Atk_Pat5]);		// 読み込むメモリー
		// -------------------------------------
	}

	// 位置・回転・スケールの初期設定
	g_player.pos = D3DXVECTOR3(300.0f, 100.0f, 0.0f);
	g_player.oldpos = D3DXVECTOR3(300.0f, 100.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_player.moveSpeed = 0.0f;
	g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_STAND_X, PLAYER_SIZE_STAND_Y);

	g_player.moveble = true;
	g_player.selected = true;
	g_player.keyPressing = false;
	g_player.direction = Left;
	g_player.jumpForce = PLAYER_JUMP_HIGH;

	// キャラ状態
	g_player.state.idle = true;
	g_player.state.running = false;
	g_player.state.jumping = false;
	g_player.state.falling = true;
	g_player.state.Grounded = false;

	// 攻撃
	g_player.AtkPat = NULL;
	g_player.NextAtkPat = NULL;
	g_player.AtkDeReSwi = false;
	g_player.AtkDeRespTime = 0;

	// PLAYERアニメーションの初期化
	g_player.Texture = g_pD3D_Texture[IDLE];
	g_player.animeCnt.PatDivX = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの内分割数Xを初期化
	g_player.animeCnt.PatDivY = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの内分割数Yを初期化
	g_player.animeCnt.AnimPatNum = PLAYER_IDLE_ANIM_PATTERN_NUM;		// アニメーションパターン数を初期化
	g_player.animeCnt.TimeAnim = PLAYER_IDLE_TIME_ANIMATION;			// アニメーションの切り替わるカウントを初期化
	g_player.animeCnt.PatternAnim = 0;									// アニメパターン番号初期化
	g_player.animeCnt.CountAnim = 0;									// アニメカウントを初期化

	// 頂点情報の作成
	MakeVertexPlayer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < STATE_MAX; i++)
	{
		if (&g_pD3D_Texture[i] != NULL)
		{// テクスチャの開放
			g_pD3D_Texture[i]->Release();
			g_pD3D_Texture[i] = NULL;
		}
	}

	for (int i = 0; i < Atk_Max; i++)
	{
		if (&g_pD3D_AtkTexture[i] != NULL)
		{// テクスチャの開放
			g_pD3D_AtkTexture[i]->Release();
			g_pD3D_AtkTexture[i] = NULL;
		}
	}
	if (g_pD3DVtxBuff != NULL)
	{// テクスチャの開放
		g_pD3DVtxBuff->Release();
		g_pD3DVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	 //アニメーション更新----------------------------------------
	PlayerAnimeUpdate();
	g_player.oldpos = g_player.pos;

	 //スウィッチ---------------------------------
	if (GetInput(DOWNMOVE))
	{
		g_player.keyPressing = false;

		if (g_player.selected == true)
		{
			g_player.selected = false;
		}
		else
		{
			g_player.selected = true;
		}
	}
	/*-------------------------------------*/
	PlayerFalling();		// 落ちる
	PlayerIdling();			// 暇

	//キャラが操作されてる----------------------------
	if (g_player.selected == true)
	{
		// キャラが移動できる状態
		if (g_player.moveble == true)
		{
			PlayerAttack();						// 攻撃
			PlayerJump();						// ジャンプ

			if (g_player.state.Attack == false)
			{
				PlayerMoving();					// 移動			
			}

			CheckCameraFollowPlayer();			// カメラ

		}
	}
	g_player.pos.x += g_player.moveSpeed;		// キャラ座標更新

	// キャラアニメーションモード------------------------------------
	SetPlayerAnimation();
	// テクスチャを更新--------------------------------
	SetVertexManChara();

	 //Debugメッセージ---------------------------------

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定  
	pDevice->SetTexture( 0, g_player.Texture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PLAYER_POLYGON, g_player.vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点座標の設定
	SetVertexPlayer(&g_player.textureSize);

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

	// テクスチャ座標の設定
	SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(D3DXVECTOR2 * textureSize)
{

	// 頂点座標の設定 
	g_player.vertexWk[0].vtx.x = g_player.pos.x - textureSize->x;
	g_player.vertexWk[0].vtx.y = g_player.pos.y - textureSize->y;
	g_player.vertexWk[0].vtx.z = 0.0f;

	g_player.vertexWk[1].vtx.x = g_player.pos.x + textureSize->x;
	g_player.vertexWk[1].vtx.y = g_player.pos.y - textureSize->y;
	g_player.vertexWk[1].vtx.z = 0.0f;

	g_player.vertexWk[2].vtx.x = g_player.pos.x - textureSize->x;
	g_player.vertexWk[2].vtx.y = g_player.pos.y + textureSize->y;
	g_player.vertexWk[2].vtx.z = 0.0f;

	g_player.vertexWk[3].vtx.x = g_player.pos.x + textureSize->x;
	g_player.vertexWk[3].vtx.y = g_player.pos.y + textureSize->y;
	g_player.vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// テクスチャの更新
//=============================================================================
void SetVertexManChara(void)
{
	// テクスチャ座標の設定
	SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
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

}
//=============================================================================
// ManChara取得関数
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}
//=============================================================================
// プレイヤージャンプ
//=============================================================================
void PlayerAnimeUpdate(void)
{
	//攻撃すれば
	if (g_player.state.Attack == true)
	{
		// アニメーションのパターンを更新------------------
		g_player.animeCnt.CountAnim++;

		// 今のフレームが時間切れたら
		if ((g_player.animeCnt.CountAnim % g_player.animeCnt.TimeAnim) == 0)
		{
			// cntリセット
			g_player.animeCnt.CountAnim = 0;
			// 次のフレームに進む
			g_player.animeCnt.PatternAnim = (g_player.animeCnt.PatternAnim + 1) % g_player.animeCnt.AnimPatNum;

			// 今の攻撃最後のフレーム
			if (g_player.animeCnt.PatternAnim % g_player.animeCnt.AnimPatNum == 0)
			{
				// 次の攻撃モードあれば
				if (g_player.NextAtkPat > g_player.AtkPat)
				{
					g_player.AtkPat = g_player.NextAtkPat;
					g_player.NextAtkPat = NULL;
				}
				// 次の攻撃モードなくなったら終わり
				else
				{
					g_player.state.Attack = false;
					g_player.AtkDeReSwi = true;
					g_player.AtkDeRespTime = 0;

					//g_player.AtkPat++;
					//if (g_player.AtkPat > Atk_Pat5)
					//{
					//	g_player.AtkPat = 0;
					//}
				}
			}
		}
	}
	else
	{
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
	}
}
//=============================================================================
// プレイヤージャンプ
//=============================================================================
void PlayerJump(void)
{
	//ジャンプ---------------------------------------
	if (GetKeyboardPress(DIK_SPACE) && g_player.state.Grounded == true && g_player.state.Attack == false)	// もし地面に立っている状態でスペースキーを押したら
	{
		g_player.state.jumping = true;
		g_player.state.Grounded = false;
	}
	if (g_player.state.jumping == true)		// プレイヤーがジャンプしているなら
	{	// Ｙ座標移動計算

		g_player.pos.y -= (PLAYER_ACCELE * g_player.jumpForce);
		g_player.jumpForce -= 3.0f;

		if (GetKeyboardPress(DIK_SPACE) && g_player.jumpForce <= PLAYER_JUMP_HIGH)
		{
			g_player.jumpForce += 2.0f;
		}
		if (g_player.jumpForce <= 0)
		{
			g_player.state.jumping = false;
			g_player.state.falling = true;
		}
	}
}
//=============================================================================
// プレイヤー移動
//=============================================================================
void PlayerMoving(void)
{
	// 右移動----------------------------------------
	if (GetKeyboardPress(DIK_RIGHT))
	{
		if (g_player.direction == Left)
		{
			g_player.moveSpeed = 0.0f;
		}
		g_player.direction = Right;					// 
		g_player.keyPressing = true;				//
		g_player.state.running = true;				// ランニングtrue
		g_player.state.idle = false;				// false
		g_player.Texture = g_pD3D_Texture[RUN];		// テキスチャー

		// キャラ移動スピード++
		g_player.moveSpeed += PLAYER_MOVE_SPEED / 10.0f;
		
		// 移動スピードが最大値超えた
		if (g_player.moveSpeed > PLAYER_MOVE_SPEED)
		{
			g_player.moveSpeed = PLAYER_MOVE_SPEED;
		}
	}
	// 左移動-----------------------------------------
	else if (GetKeyboardPress(DIK_LEFT))
	{
		if (g_player.direction == Right)
		{
			g_player.moveSpeed = 0.0f;
		}
		g_player.direction = Left;					// 左移動
		g_player.keyPressing = true;				// 
		g_player.state.running = true;				// ランニングtrue
		g_player.state.idle = false;				// false
		g_player.Texture = g_pD3D_Texture[RUN];		// テキスチャー

		// キャラ移動スピード++
		g_player.moveSpeed -= PLAYER_MOVE_SPEED / 20;

		// 移動スピードが最大値超えた
		if (g_player.moveSpeed < -PLAYER_MOVE_SPEED)
		{
			g_player.moveSpeed = -PLAYER_MOVE_SPEED;
		}
	}
}
//=============================================================================
// プレイヤー攻撃
//=============================================================================
void PlayerAttack(void)
{
	//普通攻撃
	if (GetKeyboardTrigger(DIK_A))
	{
		if (g_player.state.Attack == false)
		{
			g_player.state.Attack = true;
			g_player.moveSpeed = 0.0f;
			g_player.state.running = false;
			g_player.animeCnt.PatternAnim = 0;
			g_player.AtkPat = Atk_Pat1;
		}
		else
		{
			g_player.NextAtkPat = g_player.AtkPat + 1;

			if (g_player.NextAtkPat >= Atk_Max)
			{
				g_player.NextAtkPat = NULL;
			}
		}
	}

}
//=============================================================================
// アニメーションの設置
//=============================================================================
void SetPlayerAnimation(void)
{
	// キャラ攻撃アニメーション(最優先)
	if (g_player.state.Attack == true)
	{
		g_player.Texture = g_pD3D_AtkTexture[g_player.AtkPat];
		g_player.animeCnt.PatDivX = g_AtkAniDate[g_player.AtkPat].PatDivX;
		g_player.animeCnt.PatDivY = g_AtkAniDate[g_player.AtkPat].PatDivY;
		g_player.animeCnt.AnimPatNum = g_AtkAniDate[g_player.AtkPat].AnimPatNum;
		g_player.animeCnt.TimeAnim = g_AtkAniDate[g_player.AtkPat].TimeAnim;

		//テキスチャー大きさ調整---------------------------
		switch (g_player.AtkPat)
		{
			// 攻撃モード1--------------------------------------------
		case Atk_Pat1:
		{
			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_ATK1_X, PLAYER_SIZE_ATK1_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
		}
		break;
		// 攻撃モード2---------------------------------------------
		case Atk_Pat2:
		{
			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_ATK2_X, PLAYER_SIZE_ATK2_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
		}
		break;
		// 攻撃モード3---------------------------------------------
		case Atk_Pat3:
		{
			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_ATK3_X, PLAYER_SIZE_ATK3_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
		}
		break;
		// 攻撃モード4---------------------------------------------
		case Atk_Pat4:
		{
			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_ATK4_X, PLAYER_SIZE_ATK4_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
		}
		break;
		// 攻撃モード5---------------------------------------------
		case Atk_Pat5:
		{
			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_ATK5_X, PLAYER_SIZE_ATK5_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
		}
		break;
		default:
			break;
		}
	}
	// 攻撃してないない
	else
	{
		// ジャンプアニメーション---------------------------------------
		if (g_player.state.jumping == true)
		{
			g_player.Texture = g_pD3D_Texture[JUMP];
			g_player.animeCnt.PatDivX = PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_X;
			g_player.animeCnt.PatDivY = PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_Y;
			g_player.animeCnt.AnimPatNum = PLAYER_JUMP_ANIM_PATTERN_NUM;
			g_player.animeCnt.TimeAnim = PLAYER_JUMP_TIME_ANIMATION;

			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_STAND_X, PLAYER_SIZE_STAND_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
			
		}
		// 落ちるアニメーション----------------------------------------
		else if (g_player.state.falling == true)
		{
			g_player.Texture = g_pD3D_Texture[FALLING];
			g_player.animeCnt.PatDivX = PLAYER_FALLING_TEXTURE_PATTERN_DIVIDE_X;
			g_player.animeCnt.PatDivY = PLAYER_FALLING_TEXTURE_PATTERN_DIVIDE_Y;
			g_player.animeCnt.AnimPatNum = PLAYER_FALLING_ANIM_PATTERN_NUM;
			g_player.animeCnt.TimeAnim = PLAYER_FALLING_TIME_ANIMATION;

			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_STAND_X, PLAYER_SIZE_STAND_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);

			
		}
		// ランニングアニメーション---------------------------------------
		else if (g_player.state.running == true)
		{
			// Runningアニメーション
			g_player.animeCnt.PatDivX = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X;
			g_player.animeCnt.PatDivY = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y;
			g_player.animeCnt.AnimPatNum = PLAYER_RUN_ANIM_PATTERN_NUM;
			g_player.animeCnt.TimeAnim = PLAYER_RUN_TIME_ANIMATION;

			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_RUNNING_X, PLAYER_SIZE_RUNNING_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
			
		}
		// アニメーション---------------------------------------
		else if (g_player.state.idle == true)
		{
			// Idleアニメーション
			g_player.Texture = g_pD3D_Texture[IDLE];
			g_player.animeCnt.PatDivX = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;
			g_player.animeCnt.PatDivY = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y;
			g_player.animeCnt.AnimPatNum = PLAYER_IDLE_ANIM_PATTERN_NUM;
			g_player.animeCnt.TimeAnim = PLAYER_IDLE_TIME_ANIMATION;

			g_player.textureSize = D3DXVECTOR2(PLAYER_SIZE_STAND_X, PLAYER_SIZE_STAND_Y);
			SetVertexPlayer(&g_player.textureSize);

			// テクスチャ座標の設定
			SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
			
		}
	}
}
//=============================================================================
// プレイヤー落ちる
//=============================================================================
void PlayerFalling(void)
{
	MAP  *map = GetMapData();
	g_player.state.falling = true;

	// 落ちているなら---------------------------------
	if (g_player.state.falling == true && g_player.state.jumping == false)
	{
		g_player.pos.y += (PLAYER_ACCELE * g_player.jumpForce);

		if (g_player.jumpForce <= PLAYER_JUMP_HIGH * 2)
		{
			g_player.jumpForce += 2.0f;
		}
		for (int i = 0; i < SIZE_Y; i++)
		{
			for (int j = 0; j < SIZE_X; j++)
			{
				if (map->type != -1 && g_player.state.Attack == true)
				{
					// 床と当たる-----------------
					if (CheckHitBB(g_player.pos, map->pos,
						D3DXVECTOR2(PLAYER_SIZE_STAND_X - 15, PLAYER_SIZE_ATK1_Y + 5),
						D3DXVECTOR2(MAP_TEXTURE_SIZE_X * 2.0f, MAP_TEXTURE_SIZE_Y * 2.0f)))
					{
						g_player.pos = g_player.oldpos;
						g_player.state.falling = false;
						g_player.state.Grounded = true;
						g_player.jumpForce = PLAYER_JUMP_HIGH;
						break;
					}
				}

				else if(map->type != -1 && g_player.state.Attack == false)
				{
					// 床と当たる-----------------
					if (CheckHitBB(g_player.pos, map->pos,
						D3DXVECTOR2(PLAYER_SIZE_STAND_X - 15, PLAYER_SIZE_STAND_Y - 5),
						D3DXVECTOR2(MAP_TEXTURE_SIZE_X * 2.0f, MAP_TEXTURE_SIZE_Y * 2.0f)))
					{
						g_player.pos = g_player.oldpos;
						g_player.state.falling = false;
						g_player.state.Grounded = true;
						g_player.jumpForce = PLAYER_JUMP_HIGH;
						break;
					}
				}
				map++;
			}
		}
	}

}
//=============================================================================
// プレイヤー暇
//=============================================================================
void PlayerIdling(void)
{
	// 操作していない場合------------------------------
	if (GetKeyboardRelease(DIK_RIGHT) || GetKeyboardRelease(DIK_LEFT))
	{
		g_player.keyPressing = false;
	}
	if (g_player.keyPressing == false)
	{
		if (g_player.moveSpeed > 0)
		{
			g_player.moveSpeed -= PLAYER_MOVE_SPEED / 20;
			if (g_player.moveSpeed < 0) {
				g_player.moveSpeed = 0.0f;
				g_player.state.idle = true;
				g_player.state.running = false;
			}
		}
		else if (g_player.moveSpeed < 0)
		{
			g_player.moveSpeed += PLAYER_MOVE_SPEED / 20;
			if (g_player.moveSpeed > 0) {
				g_player.moveSpeed = 0.0f;
				g_player.state.idle = true;
				g_player.state.running = false;
			}
		}
		else g_player.state.idle = true;
	}
}
//=============================================================================
// カメラがキャラをついてる
//=============================================================================
void CheckCameraFollowPlayer(void)
{
	//CAMERA *cam = GetCamera();
	//if (cam->follow == false)
	//{
	//	// キャラが移動を超えたら
	//	if (g_player.pos.x >= cam->pos.x + CHARACTOR_MAX_MOVEBLE_RANGE_X ||
	//		g_player.pos.x <= cam->pos.x - CHARACTOR_MAX_MOVEBLE_RANGE_X ||
	//		g_player.pos.y >= cam->pos.y + CHARACTOR_MAX_MOVEBLE_RANGE_Y ||
	//		g_player.pos.y <= cam->pos.y - CHARACTOR_MAX_MOVEBLE_RANGE_Y)
	//	{
	//		cam->follow = true;
	//		cam->Target = FollowManChara;
	//	}
	//}
}
