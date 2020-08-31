//=============================================================================
//
// プレイヤー処理 [playerTest.h]
// Author : 
//
//=============================================================================

#pragma once
#include "main.h"

// テクスチャサイズ
#define	PLAYER_TEXTURE_BB_SIZE_TOP_X				(64.0f/5.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_TEXTURE_BB_SIZE_X					(64.0f/4.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_TEXTURE_BB_SIZE_Y					(64.0f/4.0f)		// PLAYERのサイズ(Y方向)

#define PLAYER_HP	(4)
#define PLAYER_SLIDE_MAX	(MAP_TEXTURE_SIZE_X * 2)

#define	PLAYER_TEXTURE_SIZE_X						(64.0f/2.0f)		// PLAYERのサイズ(X方向)
#define	PLAYER_TEXTURE_SIZE_Y						(64.0f/2.0f)		// PLAYERのサイズ(Y方向)
// Jump係数-----------------------------------
#define PLAYER_JUMP_HIGH							(25)				// ジャンプの高さ
#define PLAYER_ACCELE								(16.0f)				// 加速度

// Move速度-----------------------------------
//#define PLAYER_MOVE_SPEED							(8.0f)				// 移動量
#define	PLAYER_ROTATE								(D3DX_PI * 0.02f)	// 回転量

// 最大攻撃遅延速度-----------------------
#define MAX_ATK_DELAY_RESPONSE_TIME					(100)

// Idleアニメーション----------------------
#define	TEXTURE_PLAYER_PERFECT_IDLE					"data/TEXTURE/CharaTest/ユニティちゃん_待機.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_THREE_IDLE					"data/TEXTURE/CharaTest/idle_parts_three.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_TWO_IDLE						"data/TEXTURE/CharaTest/idle_parts_two.png"			// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_ONE_IDLE						"data/TEXTURE/CharaTest/idle_parts_one.png"			// 読み込むテクスチャファイル名

#define PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X		(8)										// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y		(1)										// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_IDLE_ANIM_PATTERN_NUM				(PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X*PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_IDLE_TIME_ANIMATION					(20)									// アニメーションの切り替わるカウント

// Runningアニメーション----------------------
#define	TEXTURE_PLAYER_PERFECT_RUN					"data/TEXTURE/CharaTest/run.png"				// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_THREE_RUN					"data/TEXTURE/CharaTest/run_parts_three.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_TWO_RUN						"data/TEXTURE/CharaTest/run_parts_two.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_ONE_RUN						"data/TEXTURE/CharaTest/run_parts_one.png"		// 読み込むテクスチャファイル名

#define PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X			(9)										// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y			(1)										// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_RUN_ANIM_PATTERN_NUM					(PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X*PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_RUN_TIME_ANIMATION					(8)										// アニメーションの切り替わるカウント

// Jumpアニメーション-----------------------------------
#define	TEXTURE_PLAYER_PERFECT_JUMP					"data/TEXTURE/CharaTest/jump.png"				// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_THREE_JUMP					"data/TEXTURE/CharaTest/jump_parts_three.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_TWO_JUMP						"data/TEXTURE/CharaTest/jump_parts_two.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_ONE_JUMP						"data/TEXTURE/CharaTest/jump_parts_one.png"		// 読み込むテクスチャファイル名

#define PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_X		(9)										// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_Y		(1)										// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_JUMP_ANIM_PATTERN_NUM				(PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_X*PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define PLAYER_JUMP_TIME_ANIMATION					(8)										// アニメーションの切り替わるカウント

typedef struct							// PLAYERアニメーション用属性構造体
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


typedef struct					// PLAYER構造体
{
	D3DXVECTOR3			pos;				// 位置
	D3DXVECTOR3			scrollPos;			// カメラ用の位置
	D3DXVECTOR3			rot;				// ポリゴンの向き(回転)
	D3DXVECTOR3			scl;				// ポリゴンの大きさ(スケール)
	D3DXVECTOR3			ofsPos;				// オフセット座標

	float				moveSpeed;			// 移動量
	float				radius;				// 半径
	float				baseAngle;			// 角度
	float				checkTopTexSize;	// topの当たり判定用

	bool				moveble;			// 移動可能
	bool				invincible;			// 無敵状態
	bool				superInvincible;	// スーパースターの無敵状態

	bool				use;				// 使用状態かどうか
	bool				scroll;				// スクロールしているかどうか
	bool				warpUse;			// ワープゲートを使用しているかどうか
	bool				bogUse;				// 泥沼床にいるかどうか

	int					countSuperInvincible;	// スーパースターの無敵状態のカウント

	int					countScroll;
	int					countMove;
	int					countShot;			// 撃つまでのカウント
	int					countInvincible;	// 無敵状態のカウント
	int					countBog;			// 泥沼状態のカウント
	int					hp;					// 体力
	int					jumpForce;			// ジャンプ力
	float				dropSpeed;			// 落下速度（重力）
	D3DXVECTOR2			textureSize;		// テクスチャサイズ

	int					slideCnt;			// 滑った時のカウント
	int					direction;			// PLAYERの方向
	int					state;				// PLAYERの状態
	int					partsState;			// PLAYERのパーツ状態

	PLAYER_ANIMCNT		animeCnt;			// PLAYERアニメーション用属性構造体
	LPDIRECT3DTEXTURE9	Texture;			// テクスチャ情報

	VERTEX_2D		vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク

	//D3DXMATRIX			mtxWorld;			// ワールドマトリックス
}PLAYER;

enum PLAYER_PARTS_STATE
{
	ONE,
	TWO,
	THREE,
	PERFECT,
	MAXPARTS
};


enum DIRECTION			// PLAYERの方向
{
	Right,				// 右向き
	Left,				// 左向き
	Down,				// 下向き
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool SlidePlayer(void);
bool DownSpeed(void);
PLAYER *GetPlayer(void);
