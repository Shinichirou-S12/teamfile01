//! @file	effect.h
//! @author	北出真弓
//! @date	2020-01-27
//! @brief	エフェクト処理の定義
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_TEXTURE			_T("data/TEXTURE/bullethit2_front.png")	// 画像
#define EFFECT_TEXTURE_FLARE	_T("data/TEXTURE/blood02.png")			// 画像

#define EFFECT_TEXTURE_SIZE_X	(128.0f / 1.5f)		// テクスチャサイズ
#define EFFECT_TEXTURE_SIZE_Y	(128.0f / 1.5f)		// 同上

#define EFFECT_TEXTURE_SIZE_FALRE_X	(1024/2)		// テクスチャサイズ
#define EFFECT_TEXTURE_SIZE_FALRE_Y	(1024/2)		// 同上


#define EFFECT_TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（fX)
#define EFFECT_TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_ANIM_PATTERN_NUM			(EFFECT_TEXTURE_PATTERN_DIVIDE_X*EFFECT_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define EFFECT_TIME_ANIMATION			(1)	// アニメーションの切り替わるカウント


#define	MAX_EXPLOSION					(128)		// ビルボード最大数
#define EFFECT_NUM_PARTS				(10)		// エフェクトの最大パターン数
#define EFFECT_NUM_EFFECTS				(5)			// エフェクトの1段の最大パターン数
#define EFFECT_LIFE_TIME				(15)		// エフェクトの最大パターン数

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
void SetEffect(float fX, float fY, int nDuration);
