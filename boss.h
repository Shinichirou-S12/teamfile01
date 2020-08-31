//=============================================================================
//
// ボスの定義 [boss.h]
// Author : kitade mayumi
//! @date	2020-08-28
//=============================================================================
#pragma once


// マクロ定義
#define BOSS_MOVE_SPEED	(1.2f)

#define BOSS_TEXTURE_SIZE_X	(50/2)		// テクスチャサイズ
#define BOSS_TEXTURE_BB_SIZE_X	(50/2.5)	// テクスチャサイズ

#define BOSS_TEXTURE_SIZE_Y	(50/2)		// 同上

// SINGLE型アニメーションテクスチャ
#define BOSS_SINGLE_IDLE_TEXTURE			_T("data/TEXTURE/EnemyChara/ventos/idle_ventos01.png")
#define BOSS_SINGLE_ATTACK_TEXTURE			_T("data/TEXTURE/EnemyChara/ventos/attack_ventos01.png")
#define BOSS_SINGLE_RUN_TEXTURE			_T("data/TEXTURE/EnemyChara/ventos/attack_ventos01.png")
#define BOSS_SINGLE_DEATH_TEXTURE			_T("data/TEXTURE/EnemyChara/ventos/death_ventos01.png")

// TWIN型アニメーションテクスチャ
#define BOSS_TWIN_IDLE_TEXTURE			_T("data/TEXTURE/EnemyChara/slime/idle_slime01.png")
#define BOSS_TWIN_ATTACK_TEXTURE			_T("data/TEXTURE/EnemyChara/slime/attack_slime01.png")
#define BOSS_TWIN_RUN_TEXTURE			_T("data/TEXTURE/EnemyChara/slime/idle_slime01.png")
#define BOSS_TWIN_DEATH_TEXTURE			_T("data/TEXTURE/EnemyChara/slime/death_slime01.png")

#define BOSS_TEXTURE_PATTERN_DIVIDE_X	(3)	// アニメパターンのテクスチャ内分割数（X)
#define BOSS_TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define BOSS_ANIM_PATTERN_NUM			(BOSS_TEXTURE_PATTERN_DIVIDE_X*BOSS_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define BOSS_TIME_ANIMATION			(16)	// アニメーションの切り替わるカウント

#define BOSS_MAX						(3) // 敵の最大数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// ボス構造体
{
	bool			use;						// true:使用  false:未使用
	bool			vecUse;						// プレイヤーのいる方向に対しての向き
	bool			damage;						// ダメージ判定
	bool			downUse;					// 落ちる設定
	bool			start;						// プレイヤーがスタート位置に立った時の判定
	bool			lenghtUse;
	bool			attackUse;					// 攻撃判定

	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	D3DXVECTOR3		move;						// 移動量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	int				hp;							// 体力
	int				countShot;					// 弾を打つまでのカウント
	int				type;
	int				attackPtn;					// 攻撃パターン
	float			colorCnt;					// α値の変更カウント
	float			lenght;						// 長さ
	int				damageCnt;					// ダメージカウント

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	int				state;						// ボスの状態管理
	int				direction;					// BOSSの方向
	float			Radius;						// ボスの半径
	float			BaseAngle;					// ボスの角度

} BOSS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
BOSS *GetBoss(void);


