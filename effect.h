//! @file	effect.h
//! @author	北出真弓
//! @date	2020-01-27
//! @brief	エフェクト処理の定義
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_TEXTURE_PLAYER_BLOOD	_T("data/TEXTURE/effect/blood02.png")					// 画像
#define EFFECT_TEXTURE_ENEMY_BLOOD	_T("data/TEXTURE/effect/blood.png")						// 画像
#define EFFECT_TEXTURE_ITEM_HEAL	_T("data/TEXTURE/effect/healing_aurora_plus.png")		// 画像
#define EFFECT_TEXTURE_ITEM_GET	_T("data/TEXTURE/effect/getFlash01.png")					// 画像
#define EFFECT_TEXTURE_KILLER_APPEAR	_T("data/TEXTURE/effect/bullethit2_front.png")		// 画像

#define EFFECT_TEXTURE_SIZE_X	(128.0f / 3.0f)		// テクスチャサイズ
#define EFFECT_TEXTURE_SIZE_Y	(128.0f / 3.0f)		// 同上

#define EFFECT_TEXTURE_SIZE_FALRE_X	(1024 / 2)		// テクスチャサイズ
#define EFFECT_TEXTURE_SIZE_FALRE_Y	(1024 / 2)		// 同上

// プレイヤーのダメージエフェクト
#define EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（fX)
#define EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_ANIM_PLAYER_PATTERN_NUM			(EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_X*EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// エネミーのダメージエフェクト
#define EFFECT_TEXTURE_ENEMY_BLOOD_PATTERN_DIVIDE_X		(5)	// アニメパターンのテクスチャ内分割数（fX)
#define EFFECT_TEXTURE_ENEMY_BLOOD_PATTERN_DIVIDE_Y		(1)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_ANIM_ENEMY_PATTERN_NUM			(EFFECT_TEXTURE_ENEMY_BLOOD_PATTERN_DIVIDE_X*EFFECT_TEXTURE_ENEMY_BLOOD_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// アイテム取得時のエフェクト
#define EFFECT_TEXTURE_ITEM_HEAL_PATTERN_DIVIDE_X		(5)	// アニメパターンのテクスチャ内分割数（fX)
#define EFFECT_TEXTURE_ITEM_HEAL_PATTERN_DIVIDE_Y		(3)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_ANIM_ITEM_PATTERN_NUM			(EFFECT_TEXTURE_ITEM_HEAL_PATTERN_DIVIDE_X*EFFECT_TEXTURE_ITEM_HEAL_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// キラー衝突時のエフェクト
#define EFFECT_TEXTURE_KILLER_APPEAR_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（fX)
#define EFFECT_TEXTURE_KILLER_APPEAR_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_ANIM_KILLER_PATTERN_NUM			(EFFECT_TEXTURE_KILLER_APPEAR_PATTERN_DIVIDE_X*EFFECT_TEXTURE_KILLER_APPEAR_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define EFFECT_TIME_ANIMATION			(1)	// アニメーションの切り替わるカウント


#define	MAX_EXPLOSION					(128)		// ビルボード最大数
#define EFFECT_NUM_PARTS				(10)		// エフェクトの最大パターン数
#define EFFECT_NUM_EFFECTS				(5)			// エフェクトの1段の最大パターン数
#define EFFECT_LIFE_TIME				(10)		// エフェクトの最大パターン数

// エフェクトの種類の列挙型
enum EFFECT_KIND
{
	PLAYER_BLOOD,
	ENEMY_BLOOD,
	ITEM_HEAL,
	ITEM_GET,
	KILLER_APPEAR,
	MAX_KIND_EFFECT
};

/**************************************************************************//**
	@struct		PARTICLE
	@brief		エフェクト構造体
	@par		[エフェクトの定義]
*//***************************************************************************/
typedef struct	
{
	D3DXVECTOR3		pos;						//!< ポリゴンの移動量
	D3DXVECTOR3		move;						//!< 移動量
	int				nPatternAnim;				//!< アニメーションパターンナンバー
	int				nCountAnim;					//!< アニメーションカウント

	int				nLiveTime;					//!< 生存時間

	LPDIRECT3DTEXTURE9	Texture;				//!< テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		//!< 頂点情報格納ワーク

	bool			bIsFinish;
}PARTICLE;

typedef struct
{
	int				texType;
	int				texDivedX;
	int				texDivedY;
}EFECTTEX;

typedef struct	// エフェクト構造体
{
	int				nUse;						//!< 使用されているか
	int				nIsEnding;					//!< アニメーションの終了
	bool			bIsRemoveOnFinish;			//!< アニメーションの終了地点から移動

	D3DXVECTOR3		pos;						//!< ポリゴンの移動量

	int				nDuration;					//!< エフェクトの開始から終了までの時間
	int				nElapsed;					//!< エフェクトの終了までをカウント
	int				nNumFinish;					//!< エフェクトの最大数までのカウント

	int				nEffectCount;				//!< エフェクトパターンのカウント
	int				nEmitCounter;				//!< エミット数カウント

	EFECTTEX		texture;					//!< テクスチャの設定
	PARTICLE		pParticle[EFFECT_NUM_PARTS];	//!< パーティクルワーク

}EFFECT;

/**************************************************************************//**
	@brief		エフェクトの初期化処理
	@param		type	テクスチャタイプ
	@return		S_OK	処理の成功
				E_FAIL	処理の失敗
*//***************************************************************************/
HRESULT InitEffect(int type);

/**************************************************************************//**
	@brief		エフェクトの終了処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UninitEffect(void);

/**************************************************************************//**
	@brief		エフェクトの更新処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UpdateEffect(void);

/**************************************************************************//**
	@brief		エフェクトの描画処理
	@param		なし
	@return		なし
*//***************************************************************************/
void DrawEffect(void);

/**************************************************************************//**
	@brief		エフェクトの配置処理
	@param[in]	fX	X座標
	@param[in]	fY	Y座標
	@param[in]	nDuration	アニメーション時間
	@return		なし
*//***************************************************************************/
void SetEffect(float fX, float fY, int nDuration, int type);
