//=============================================================================
//
// player.h
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <d3dx9.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_PLAYER_POLYGON		(2)					// ポリゴン数
#define	NUM_PLAYER_VERTEX		(4)					// その頂点数

// Idleアニメーション------------------------
#define	TEXTURE_PLAYER_IDLE							"data/TEXTURE/Charactor/stand.png"						// 読み込むテクスチャファイル名
#define PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X			(4)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_IDLE_ANIM_PATTERN_NUM					(PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X*PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_IDLE_TIME_ANIMATION					(20)		// アニメーションの切り替わるカウント

// Runningアニメーション----------------------
#define	TEXTURE_PLAYER_RUN							"data/TEXTURE/Charactor/WALK.png"						// 読み込むテクスチャファイル名
#define PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X			(6)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_RUN_ANIM_PATTERN_NUM					(PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X*PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_RUN_TIME_ANIMATION					(8)			// アニメーションの切り替わるカウント

// Jumpアニメーション-----------------------------------
#define	TEXTURE_PLAYER_JUMP							"data/TEXTURE/Charactor/$erzaDark_3.png"				// 読み込むテクスチャファイル名
#define PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_X			(6)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_JUMP_ANIM_PATTERN_NUM					(PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_X*PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_JUMP_TIME_ANIMATION					(8)			// アニメーションの切り替わるカウント

// Fallアニメーション-----------------------------------
#define	TEXTURE_PLAYER_FALLING						"data/TEXTURE/Charactor/$erzaSword_3.png"					// 読み込むテクスチャファイル名
#define PLAYER_FALLING_TEXTURE_PATTERN_DIVIDE_X		(4)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_FALLING_TEXTURE_PATTERN_DIVIDE_Y		(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_FALLING_ANIM_PATTERN_NUM				(PLAYER_FALLING_TEXTURE_PATTERN_DIVIDE_X*PLAYER_FALLING_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_FALLING_TIME_ANIMATION				(8)			// アニメーションの切り替わるカウント

// Attackアニメーション-----------------------------------
// Attack1
#define	TEXTURE_PLAYER_ATK1							"data/TEXTURE/Charactor/attack1.png"		// 読み込むテクスチャファイル名
#define PLAYER_ATK1_TEXTURE_PATTERN_DIVIDE_X			(5)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_ATK1_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_ATK1_ANIM_PATTERN_NUM					(PLAYER_ATK1_TEXTURE_PATTERN_DIVIDE_X*PLAYER_ATK1_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_ATK1_TIME_ANIMATION					(4)			// アニメーションの切り替わるカウント
// Attack2
#define	TEXTURE_PLAYER_ATK2							"data/TEXTURE/Charactor/attack2.png"		// 読み込むテクスチャファイル名
#define PLAYER_ATK2_TEXTURE_PATTERN_DIVIDE_X			(5)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_ATK2_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_ATK2_ANIM_PATTERN_NUM					(PLAYER_ATK2_TEXTURE_PATTERN_DIVIDE_X*PLAYER_ATK2_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_ATK2_TIME_ANIMATION					(4)			// アニメーションの切り替わるカウント
// Attack3
#define	TEXTURE_PLAYER_ATK3							"data/TEXTURE/Charactor/attack3.png"		// 読み込むテクスチャファイル名
#define PLAYER_ATK3_TEXTURE_PATTERN_DIVIDE_X			(5)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_ATK3_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_ATK3_ANIM_PATTERN_NUM					(PLAYER_ATK3_TEXTURE_PATTERN_DIVIDE_X*PLAYER_ATK3_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_ATK3_TIME_ANIMATION					(4)			// アニメーションの切り替わるカウント
// Attack4
#define	TEXTURE_PLAYER_ATK4							"data/TEXTURE/Charactor/attack4.png"		// 読み込むテクスチャファイル名
#define PLAYER_ATK4_TEXTURE_PATTERN_DIVIDE_X			(5)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_ATK4_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_ATK4_ANIM_PATTERN_NUM					(PLAYER_ATK4_TEXTURE_PATTERN_DIVIDE_X*PLAYER_ATK4_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_ATK4_TIME_ANIMATION					(4)			// アニメーションの切り替わるカウント
// Attack5
#define	TEXTURE_PLAYER_ATK5							"data/TEXTURE/Charactor/attack5.png"		// 読み込むテクスチャファイル名
#define PLAYER_ATK5_TEXTURE_PATTERN_DIVIDE_X			(5)			// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_ATK5_TEXTURE_PATTERN_DIVIDE_Y			(1)			// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_ATK5_ANIM_PATTERN_NUM					(PLAYER_ATK5_TEXTURE_PATTERN_DIVIDE_X*PLAYER_ATK5_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_ATK5_TIME_ANIMATION					(4)			// アニメーションの切り替わるカウント

#define PLAYER_TEXTURE_SIZE_X		(50/2) // テクスチャサイズ  
#define PLAYER_TEXTURE_SIZE_Y		(100/2) // 同上  


// テクスチャサイズ-----------------------------------
#define	PLAYER_SIZE_STAND_X							(50.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_SIZE_STAND_Y							(80.0f/2.0f)		// PLAYERのサイズ(Y方向)
#define	PLAYER_SIZE_RUNNING_X						(85.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_SIZE_RUNNING_Y						(70.0f/2.0f)		// PLAYERのサイズ(Y方向)
#define	PLAYER_SIZE_ATK1_X							(115.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_SIZE_ATK1_Y							(115.0f/2.0f)		// PLAYERのサイズ(Y方向)
#define	PLAYER_SIZE_ATK2_X							(115.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_SIZE_ATK2_Y							(115.0f/2.0f)		// PLAYERのサイズ(Y方向)
#define	PLAYER_SIZE_ATK3_X							(165.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_SIZE_ATK3_Y							(115.0f/2.0f)		// PLAYERのサイズ(Y方向)
#define	PLAYER_SIZE_ATK4_X							(125.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_SIZE_ATK4_Y							(115.0f/2.0f)		// PLAYERのサイズ(Y方向)
#define	PLAYER_SIZE_ATK5_X							(145.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_SIZE_ATK5_Y							(115.0f/2.0f)		// PLAYERのサイズ(Y方向)

// Jump係数-----------------------------------
#define PLAYER_JUMP_HIGH								(25)		// ジャンプの高さ
#define PLAYER_ACCELE								(0.8f)			// 加速度

// Move速度-----------------------------------
#define PLAYER_MOVE_SPEED							(8.0f)				// 移動量
#define	PLAYER_ROTATE								(D3DX_PI * 0.02f)	// 回転量

// 最大攻撃遅延速度-----------------------
#define MAX_ATK_DELAY_RESPONSE_TIME						(100)

typedef struct					// PLAYERアニメーション用属性構造体
{
	int		PatDivX;					// アニメパターンのテクスチャ内分割数（X)
	int		PatDivY;					// アニメパターンのテクスチャ内分割数（Y)
	int		AnimPatNum;					// アニメーションパターン数
	int		TimeAnim;					// アニメーションの切り替わるカウント
	int		PatternAnim;				// アニメーションパターンナンバー
	int		CountAnim;					// アニメーションカウント
}PLAYER_ANIMCNT;

typedef struct
{
	int		PatDivX;					// アニメパターンのテクスチャ内分割数（X)
	int		PatDivY;					// アニメパターンのテクスチャ内分割数（Y)
	int		AnimPatNum;					// アニメーションパターン数
	int		TimeAnim;					// アニメーションの切り替わるカウント
}PLAYER_ANIM_DATE;

typedef struct 					// PLAYERの状態
{
	bool idle;							// 操作していない
	bool running;						// 走っている
	bool jumping;						// ジャンプしている
	bool falling;						// 落ちている	
	bool Grounded;						// 地面上に立っている
	bool Attack;						// 攻撃
}PLAYER_STATE;

typedef struct					// PLAYER構造体
{
	D3DXVECTOR3			pos;				// ポリゴンの位置
	D3DXVECTOR3			oldpos;				// ポリゴンの位置
	D3DXVECTOR3			rot;				// ポリゴンの向き(回転)
	D3DXVECTOR3			scl;				// ポリゴンの大きさ(スケール)
	float				moveSpeed;			// 移動量

	bool				moveble;			// 移動可能
	bool				keyPressing;
	bool				selected;
	float				jumpForce;			// ジャンプ力
	int					AtkPat;				// 攻撃タイプ
	int					NextAtkPat;
	bool				AtkDeReSwi;			// (Delay response switch)
	int					AtkDeRespTime;		// (Delay response time)
	D3DXVECTOR2			textureSize;		// テクスチャサイズ

	int					direction;			// PLAYERの方向
	PLAYER_STATE		state;				// PLAYERの状態

	PLAYER_ANIMCNT		animeCnt;			// PLAYERアニメーション用属性構造体
	LPDIRECT3DTEXTURE9	Texture;			// テクスチャ情報

	VERTEX_2D		vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
}PLAYER;


enum PLAYER_DIRECTION			// PLAYERの方向
{
	Right,				// 右向き
	Left,				// 左向き
	Down,				// 下向き
};

enum PLAYER_ATK_ANIME
{
	Atk_Pat1,
	Atk_Pat2,
	Atk_Pat3,
	Atk_Pat4,
	Atk_Pat5,
	Atk_Max,
};

enum PLAYER_STATE_ANIME
{
	IDLE,
	RUN,
	JUMP,
	FALLING,
	ATTACK1,
	STATE_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
