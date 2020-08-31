//=============================================================================
//
// 身代わりアイテムの処理 [subtitute.h]
// Author : kitade mayumi
//
//=============================================================================

//=============================================================================
//
// [substitute.cpp]
// Author : kitade mayumi
//! @date	2020-08-24
//! @brief	身代わりアイテムの実装
//=============================================================================

#pragma once

// テクスチャサイズ
#define	SUBSTITUTE_TEXTURE_SIZE_X						(32.0f)		// SUBSTITUTEのサイズ(X方向)
#define	SUBSTITUTE_TEXTURE_SIZE_Y						(32.0f)		// SUBSTITUTEのサイズ(Y方向)

#define	SUBSTITUTE_TEXTURE_BB_SIZE_TOP_X				(SUBSTITUTE_TEXTURE_SIZE_X / 2.5f)		// SUBSTITUTEのサイズ(X方向)
#define	SUBSTITUTE_TEXTURE_BB_SIZE_X					(SUBSTITUTE_TEXTURE_SIZE_X / 2.0f)		// SUBSTITUTEのサイズ(X方向)
#define	SUBSTITUTE_TEXTURE_BB_SIZE_Y					(SUBSTITUTE_TEXTURE_SIZE_Y / 2.0f)		// SUBSTITUTEのサイズ(Y方向)

#define SUBSTITUTE_HP	(4)

// Jump係数-----------------------------------
#define SUBSTITUTE_JUMP_HIGH							(25)				// ジャンプの高さ
#define SUBSTITUTE_ACCELE								(16.0f)				// 加速度

// Move速度-----------------------------------
//#define SUBSTITUTE_MOVE_SPEED							(8.0f)				// 移動量
#define	SUBSTITUTE_ROTATE								(D3DX_PI * 0.02f)	// 回転量

// Idleアニメーション----------------------
#define	TEXTURE_SUBSTITUTE_IDLE					"data/TEXTURE/substitute/substitute_idle.png"	// 読み込むテクスチャファイル名

#define SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_X		(4)										// アニメパターンのテクスチャ内分割数（X)
#define SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_Y		(1)										// アニメパターンのテクスチャ内分割数（Y)
#define SUBSTITUTE_IDLE_ANIM_PATTERN_NUM				(SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_X*SUBSTITUTE_IDLE_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define SUBSTITUTE_IDLE_TIME_ANIMATION					(20)									// アニメーションの切り替わるカウント

// Runningアニメーション----------------------
#define	TEXTURE_SUBSTITUTE_RUN					"data/TEXTURE/substitute/substitute_run.png"	// 読み込むテクスチャファイル名

#define SUBSTITUTE_RUN_TEXTURE_PATTERN_DIVIDE_X			(6)										// アニメパターンのテクスチャ内分割数（X)
#define SUBSTITUTE_RUN_TEXTURE_PATTERN_DIVIDE_Y			(1)										// アニメパターンのテクスチャ内分割数（Y)
#define SUBSTITUTE_RUN_ANIM_PATTERN_NUM					(SUBSTITUTE_RUN_TEXTURE_PATTERN_DIVIDE_X*SUBSTITUTE_RUN_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define SUBSTITUTE_RUN_TIME_ANIMATION					(8)										// アニメーションの切り替わるカウント

// Jumpアニメーション-----------------------------------
#define	TEXTURE_SUBSTITUTE_JUMP					"data/TEXTURE/substitute/substitute_jump.png"	// 読み込むテクスチャファイル名

#define SUBSTITUTE_JUMP_TEXTURE_PATTERN_DIVIDE_X		(2)										// アニメパターンのテクスチャ内分割数（X)
#define SUBSTITUTE_JUMP_TEXTURE_PATTERN_DIVIDE_Y		(1)										// アニメパターンのテクスチャ内分割数（Y)
#define SUBSTITUTE_JUMP_ANIM_PATTERN_NUM				(SUBSTITUTE_JUMP_TEXTURE_PATTERN_DIVIDE_X*SUBSTITUTE_JUMP_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define SUBSTITUTE_JUMP_TIME_ANIMATION					(8)										// アニメーションの切り替わるカウント

typedef struct							// SUBSTITUTEアニメーション用属性構造体
{
	int		PatDivX;					// アニメパターンのテクスチャ内分割数（X)
	int		PatDivY;					// アニメパターンのテクスチャ内分割数（Y)
	int		AnimPatNum;					// アニメーションパターン数
	int		TimeAnim;					// アニメーションの切り替わるカウント
	int		PatternAnim;				// アニメーションパターンナンバー
	int		CountAnim;					// アニメーションカウント
}SUBSTITUTE_ANIMCNT;

typedef struct
{
	int		PatDivX;					// アニメパターンのテクスチャ内分割数（X)
	int		PatDivY;					// アニメパターンのテクスチャ内分割数（Y)
	int		AnimPatNum;					// アニメーションパターン数
	int		TimeAnim;					// アニメーションの切り替わるカウント
}SUBSTITUTE_ANIM_DATE;


typedef struct					// SUBSTITUTE構造体
{
	D3DXVECTOR3			pos;				// 位置
	D3DXVECTOR3			scrollPos;			// カメラ用の位置
	D3DXVECTOR3			rot;				// ポリゴンの向き(回転)
	D3DXVECTOR3			scl;				// ポリゴンの大きさ(スケール)
	D3DXVECTOR3			ofsPos;				// オフセット座標

	float				moveSpeed;			// 移動量
	float				radius;				// 半径
	float				baseAngle;			// 角度
	float				lenght;				// 距離

	bool				moveble;			// 移動可能
	bool				use;				// 使用状態かどうか
	bool				releaseUse;			// 離れているかどうか
	bool				sticking;			// 一度くっついたかどうか
	bool				attackUse;			// 攻撃しているかどうか

	int					moveCount;			// 移動カウント
	int					countReleaseTime;	// プレイヤーから離れている時間
	int					jumpForce;			// ジャンプ力
	float				dropSpeed;			// 落下速度（重力）
	D3DXVECTOR2			textureSize;		// テクスチャサイズ

	int					direction;			// SUBSTITUTEの方向
	int					state;				// SUBSTITUTEの状態

	SUBSTITUTE_ANIMCNT	animeCnt;			// SUBSTITUTEアニメーション用属性構造体
	LPDIRECT3DTEXTURE9	Texture;			// テクスチャ情報

	VERTEX_2D		vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク

}SUBSTITUTE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSubstitute(void);
void UninitSubstitute(void);
void UpdateSubstitute(void);
void DrawSubstitute(void);
SUBSTITUTE *GetSubstitute(void);
void SetPosSubstitute(void);
