//! @file	spear.h
//! @author	kitade mayumi
//! @date	2020-08-20
//! @brief  針の定義

#ifndef _SPEAR_H_
#define _SPEAR_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_SPEAR				(2)			// ポリゴン数


#define TEXTURE_SPEAR_SIZE_X	(16.0f)		// テクスチャサイズ
#define TEXTURE_SPEAR_SIZE_Y	(16.0f)		// 同上


#define TEXTURE_PATTERN_DIVIDE_X_SPEAR	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_SPEAR	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_SPEAR			(TEXTURE_PATTERN_DIVIDE_X_SPEAR*TEXTURE_PATTERN_DIVIDE_Y_SPEAR)	// アニメーションパターン数
#define TIME_ANIMATION_SPEAR			(4)	// アニメーションの切り替わるカウント

#define SPEAR_MAX						(50) // 針の最大数
#define SPEAR_DAMAGE_SCORE				(50) // 針のスコア減算値

#define SPEAR_SPEED			(6.0f)		// 針の移動スピード

/**************************************************************************//**
	@struct		SPEAR
	@brief		針の構造体
	@par		[針の定義]
*//***************************************************************************/
typedef struct	
{
	bool			use;						// true:使用  false:未使用
	bool			downUse;					// 下に下がるやつ
	bool			popUse;

	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	D3DXVECTOR3		move;						// 移動量
	D3DXVECTOR3		vec;						// ベクトル

	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	int				type;						// タイプ（種類）
	int				direction;					// 向き
	int				MoveCnt;					// 移動カウント
	int				PopCnt;						// 停滞カウント

	float			speed;						// 移動速度

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} SPEAR;

/**************************************************************************//**
	@brief		InitSpear
	@brief		針の初期化処理
	@param[in]	type		テクスチャタイプ
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT InitSpear(int type);

/**************************************************************************//**
	@brief		UninitSpear
	@brief		針の終了処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UninitSpear(void);

/**************************************************************************//**
	@brief		UpdateSpear
	@brief		針の更新処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UpdateSpear(void);

/**************************************************************************//**
	@brief		DrawSpear
	@brief		針の描画処理
	@param		なし
	@return		なし
*//***************************************************************************/
void DrawSpear(void);

/**************************************************************************//**
	@brief		GetSpear
	@brief		針のアドレス取得
	@param[in]	no		針の番号
	@return		&g_killer[no] ブロック番号のアドレス
*//***************************************************************************/
SPEAR *GetSpear(int no);


#endif
