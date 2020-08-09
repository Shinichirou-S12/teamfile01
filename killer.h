//! @file	killer.h
//! @author	kitade mayumi
//! @date	2020-08-07
//! @brief キラーの定義

#ifndef _KILLER_H_
#define _KILLER_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KILLER				(2)			// ポリゴン数


#define TEXTURE_KILLER_SIZE_X	(50)		// テクスチャサイズ
#define TEXTURE_KILLER_SIZE_Y	(50)		// 同上


#define TEXTURE_PATTERN_DIVIDE_X_KILLER	(3)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_KILLER	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_KILLER			(TEXTURE_PATTERN_DIVIDE_X_KILLER*TEXTURE_PATTERN_DIVIDE_Y_KILLER)	// アニメーションパターン数
#define TIME_ANIMATION_KILLER			(4)	// アニメーションの切り替わるカウント

#define KILLER_MAX						(6) // キラーの最大数

#define KILLER_SPEED			(6.0f)		// キラーの移動スピード

/**************************************************************************//**
	@struct		KILLER
	@brief		キラーの構造体
	@par		[キラーの定義]
*//***************************************************************************/
typedef struct	
{
	bool			use;						// true:使用  false:未使用
	bool			vecUse;						// ベクトルタイプの使用状態
	bool			dead;						// 死んだかどうか

	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	D3DXVECTOR3		move;						// 移動量
	D3DXVECTOR3		vec;						// ベクトル

	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	int				type;						// タイプ（種類）
	int				popCnt;						// リスボーンカウント
	int				direction;					// 向き

	float			speed;						// 移動速度

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} KILLER;

/**************************************************************************//**
	@brief		InitKiller
	@brief		キラーの初期化処理
	@param[in]	type		テクスチャタイプ
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT InitKiller(int type);

/**************************************************************************//**
	@brief		UninitKiller
	@brief		キラーの終了処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UninitKiller(void);

/**************************************************************************//**
	@brief		UpdateKiller
	@brief		キラーの更新処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UpdateKiller(void);

/**************************************************************************//**
	@brief		DrawKiller
	@brief		キラーの描画処理
	@param		なし
	@return		なし
*//***************************************************************************/
void DrawKiller(void);

/**************************************************************************//**
	@brief		SetKiller
	@brief		キラーの発射設定
	@param[in]	pos		キラーの座標
	@param[in]	type	キラーの向き
	@return		ptr		キラーの先頭アドレス
*//***************************************************************************/
void SetKiller(D3DXVECTOR3 pos, int type);

/**************************************************************************//**
	@brief		GetKiller
	@brief		キラーのアドレス取得
	@param[in]	no		キラーの番号
	@return		&g_killer[no] ブロック番号のアドレス
*//***************************************************************************/
KILLER *GetKiller(int no);


#endif
