//! @file	block.h
//! @author	まよ
//! @date	2020-06-18
//! @brief	弾の定義

#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_BULLET				(2)			// ポリゴン数

#define TEXTURE_GAME_BULLET		_T("data/TEXTURE/lightning_ball.png")			// 画像

#define TEXTURE_BULLET_SIZE_X	(50)		// テクスチャサイズ
#define TEXTURE_BULLET_SIZE_Y	(50)		// 同上


#define TEXTURE_PATTERN_DIVIDE_X_BULLET	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLET	(4)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_BULLET			(TEXTURE_PATTERN_DIVIDE_X_BULLET*TEXTURE_PATTERN_DIVIDE_Y_BULLET)	// アニメーションパターン数
#define TIME_ANIMATION_BULLET			(4)	// アニメーションの切り替わるカウント

#define BULLET_MAX						(6) // 弾の最大数

#define BULLET_SPEED			(6.0f)		// 弾の移動スピード

/**************************************************************************//**
	@struct		BULLET
	@brief		弾の構造体
	@par		[弾の定義]
*//***************************************************************************/
typedef struct	
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	D3DXVECTOR3		move;						// 移動量
	float			speed;						// 移動速度

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} BULLET;

/**************************************************************************//**
	@brief		InitBullet
	@brief		弾の初期化処理
	@param[in]	type		テクスチャタイプ
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT InitBullet(int type);

/**************************************************************************//**
	@brief		UninitBullet
	@brief		弾の終了処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UninitBullet(void);

/**************************************************************************//**
	@brief		UpdateBullet
	@brief		弾の更新処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UpdateBullet(void);

/**************************************************************************//**
	@brief		DrawBullet
	@brief		弾の描画処理
	@param		なし
	@return		なし
*//***************************************************************************/
void DrawBullet(void);

/**************************************************************************//**
	@brief		SetBullet
	@brief		弾の発射設定
	@param[in]	pos		弾の座標
	@param[in]	type	弾の向き
	@return		ptr		弾の先頭アドレス
*//***************************************************************************/
void SetBullet(D3DXVECTOR3 pos, int type);

/**************************************************************************//**
	@brief		GetBullet
	@brief		弾のアドレス取得
	@param[in]	no		弾の番号
	@return		&blockWk[no] ブロック番号のアドレス
*//***************************************************************************/
BULLET *GetBullet(int no);


#endif
