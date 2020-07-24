//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


// マクロ定義
#define ENEMY_TEXTURE			_T("data/TEXTURE/EnemyChara/eyes.png")	// 画像
#define ENEMY_TEXTURE_SIZE_X	(50/2)		// テクスチャサイズ
#define ENEMY_TEXTURE_SIZE_Y	(50/2)		// 同上

#define ENEMY_TEXTURE_PATTERN_DIVIDE_X	(1)	// アニメパターンのテクスチャ内分割数（X)
#define ENEMY_TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ENEMY_ANIM_PATTERN_NUM			(ENEMY_TEXTURE_PATTERN_DIVIDE_X*ENEMY_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ENEMY_TIME_ANIMATION			(4)	// アニメーションの切り替わるカウント

#define ENEMY_MAX						(15) // 敵の最大数

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

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// エネミーの半径
	float			BaseAngle;					// エネミーの角度

} ENEMY;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(void);


