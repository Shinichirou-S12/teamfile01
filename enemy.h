//=============================================================================
//
// 敵処理 [enemy.h]
// Author : kitade mayumi
//
//=============================================================================
#pragma once


// マクロ定義
#define ENEMY_MOVE_SPEED	(1.2f)

#define ENEMY_TEXTURE_SIZE_X	(50/2)		// テクスチャサイズ
#define ENEMY_TEXTURE_BB_SIZE_X	(50/2.5)	// テクスチャサイズ

#define ENEMY_TEXTURE_SIZE_Y	(50/2)		// 同上

// SNIPER型アニメーションテクスチャ
#define ENEMY_SNIPER_IDLE_TEXTURE			_T("data/TEXTURE/EnemyChara/ventos/idle_ventos01.png")
#define ENEMY_SNIPER_ATTACK_TEXTURE			_T("data/TEXTURE/EnemyChara/ventos/attack_ventos01.png")
#define ENEMY_SNIPER_DEATH_TEXTURE			_T("data/TEXTURE/EnemyChara/ventos/death_ventos01.png")

// TRACKER型アニメーションテクスチャ
#define ENEMY_TRACKER_IDLE_TEXTURE			_T("data/TEXTURE/EnemyChara/slime/idle_slime01.png")
#define ENEMY_TRACKER_ATTACK_TEXTURE			_T("data/TEXTURE/EnemyChara/slime/attack_slime01.png")
#define ENEMY_TRACKER_DEATH_TEXTURE			_T("data/TEXTURE/EnemyChara/slime/death_slime01.png")

// GUARDIAN型アニメーションテクスチャ
#define ENEMY_GUARDIAN_IDLE_TEXTURE			_T("data/TEXTURE/EnemyChara/slime02/slime02_idle.png")
#define ENEMY_GUARDIAN_ATTACK_TEXTURE			_T("data/TEXTURE/EnemyChara/slime02/slime02_attack.png")
#define ENEMY_GUARDIAN_DEATH_TEXTURE			_T("data/TEXTURE/EnemyChara/slime02/slime02_death.png")

#define ENEMY_TEXTURE_PATTERN_DIVIDE_X	(3)	// アニメパターンのテクスチャ内分割数（X)
#define ENEMY_TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ENEMY_ANIM_PATTERN_NUM			(ENEMY_TEXTURE_PATTERN_DIVIDE_X*ENEMY_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ENEMY_TIME_ANIMATION			(6)	// アニメーションの切り替わるカウント

#define ENEMY_MAX						(15) // 敵の最大数

enum ENEMYTYPE
{
	SNIPER,
	TRACKER,
	GUARDIAN,
	ENEMYTYPEMAX
};

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// エネミー構造体
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	D3DXVECTOR3		move;						// 移動量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	int				hp;							// 体力
	bool			damage;						// ダメージ判定
	int				countShot;					// 弾を打つまでのカウント
	int				type;

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	int				state;						// エネミーの状態管理
	int				direction;					// ENEMYの方向
	float			Radius;						// エネミーの半径
	float			BaseAngle;					// エネミーの角度
	float			dropSpeed;					// 落下速度（重力）

} ENEMY;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(void);


