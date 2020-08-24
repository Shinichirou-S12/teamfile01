//=============================================================================
//
// [substitute.h]
// Author : kitade mayumi
//! @date	2020-08-24
//! @brief	身代わりアイテムの定義
//=============================================================================
#pragma once
// マクロ定義
#define	NUM_SUBSTITUTE		(2)									// ポリゴン数

#define TEXTURE_GAME_SUBSTITUTE	_T("data/TEXTURE/egg.png")		// 画像
#define TEXTURE_SUBSTITUTE_SIZE_X		(48/2)					// テクスチャサイズ
#define TEXTURE_SUBSTITUTE_SIZE_Y		(48/2)					// 同上

#define TEXTURE_SUBSTITUTE_PATTERN_X	(5)						// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_SUBSTITUTE_PATTERN_Y	(3)						// アニメパターンのテクスチャ内分割数（Y)
//
//#define TIME_ANIMATION				(5)					// アニメーションの切り替わるカウント
//
//#define MOVE_ANIM_PATTERN			(4)
//
//#define MOVE_TYPE_TRAVERSE			(0)					// バレットテキスチャーのY軸0番
//
//#define MOVE_SPEED					(5.0f)				// 移動速度

#define SUBSTITUTE_JUMP_VAL				(6.0f)					// バレットのジャンプ時の高さ補正値
#define SUBSTITUTE_JUMP_SUM				(0.2f)					// バレットのジャンプ時の高さ値の補正値
#define SUBSTITUTE_DOWN_SUM				(0.18f)

#define SUBSTITUTE_HITANIM_VAL			(5.0f)					// バレットのジャンプ時の高さ補正値
#define SUBSTITUTE_HITANIM_SUM			(0.3f)					// バレットのジャンプ時の高さ値の補正値
//#define SUBSTITUTE_HITDOWN_SUM				(0.18f)


#define SUBSTITUTE_INTERVAL_MINIMUM		(48.0f)
#define SUBSTITUTE_INTERVAL_MAXIMUM		(64.0f)
#define SUBSTITUTE_INTERVAL_BETWEEN		(36.0f)

#define SUBSTITUTE_INITPOS_X			(PLAYER_INITPOS_X - SUBSTITUTE_INTERVAL_MINIMUM)
#define SUBSTITUTE_INITPOS_Y			(410.0f)				// バレットのY軸の初期値

#define SUBSTITUTE_DISCHARGE_SPEED		(11.0f)					// バレットの発射速度

#define SUBSTITUTE_LIMIT_POS_X_MIN		(-100.0f)				// バレット発射時の左限界値
#define SUBSTITUTE_LIMIT_POS_X_MAX		(SCREEN_WIDTH + TEXTURE_SUBSTITUTE_SIZE_X)	// ベレット発射時の右限界値


#define SUBSTITUTE_POSX_MIN				(3.0f)					// バレット座標の左限界値

#define DROP_WAVE_VAL				(5)						// ドロップ時の頂点座標の揺れ範囲

typedef enum
{// バレットのテキスチャーX軸の番号
	SUBSTITUTE_ANIM_WAIT,
	SUBSTITUTE_ANIM_WALK_A,
	SUBSTITUTE_ANIM_WALK_B,
	SUBSTITUTE_ANIM_WALK_C,
	SUBSTITUTE_ANIM_DMG,
}SUBSTITUTE_ANIM_NUM;

typedef enum
{// バレットの各アクションのラベル
	SUBSTITUTE_ACT_MOVE,
	SUBSTITUTE_ACT_JUMP,
	SUBSTITUTE_ACT_DISCHARGE,
	SUBSTITUTE_ACT_HIT,
	SUBSTITUTE_ACT_WAIT,
	SUBSTITUTE_ACT_DROP
}SUBSTITUTE_ACTION;

typedef enum
{// バレット番号
	SUBSTITUTE_PATTERN_DROP,
	SUBSTITUTE_PATTERN_USE,
	SUBSTITUTE_PATTERN_BORN
}SUBSTITUTE_ANIM_TYPE;

typedef enum
{// バレット番号
	SUBSTITUTE_1ST,
	SUBSTITUTE_2ND,
	//SUBSTITUTE_3RD,
	SUBSTITUTE_MAX //= 10
}SUBSTITUTE_NUM;

typedef enum
{
	SUBSTITUTE_OPTIMAL_DISTANCE,
	SUBSTITUTE_TOO_NEAR,
	SUBSTITUTE_TOO_FAR,
	SUBSTITUTE_DISUSE
}SUBSTITUTE_DISTANCE;



//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// バレット構造体
{
	bool				use;					// true:使用		false:未使用
	bool				useJump;				// true:ジャンプ中	false:ジャンプ未使用
	bool				useDischarge;			// true:発射中		flase:未発射
	bool				useHitAnim;				// true:着弾後		false:着弾前

	int					patternAnim;			// アニメーションパターンナンバー
	int					countAnim;				// アニメーションカウント
	int					state;					// バレットの向き状態
	int					animType;				// バレットテキスチャーのｙ軸の値

	D3DXVECTOR3			pos;					// ポリゴンの移動量
	D3DXVECTOR3			rot;					// ポリゴンの回転量

	float				turn;					// バレットの向き
	float				dirDischarge;			// 発射の向き +1.0fで右方向、-1.0ｆで左方向

	float				yTemp;					// ジャンプ時のｙ値を臨時保存
	float				yPrev;					// ジャンプ時の前回のｙ値を保存

	float				radius;					// バレットの半径
	float				baseAngle;				// バレットの角度

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D			vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク

} SUBSTITUTE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSubstitute(int type);
void	UninitSubstitute(void);
void	UpdateSubstitute(void);
void	DrawSubstitute(void);
SUBSTITUTE	*GetSubstitute(int no);

void	SetMoveSubstitute(float val, float dir);
void	SetSubstituteDischarge(float direction);
void	SetHitSubstitute(int no);
void	UpdateJumpSubstitute(void);
void	SetJumpSubstitute(void);



