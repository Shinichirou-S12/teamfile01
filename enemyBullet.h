//! @file	enemyBullet.h
//! @author	まよ
//! @date	2020-08-04
//! @brief	エネミー用の弾の定義

#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_ENEMY_BULLET		_T("data/TEXTURE/lightning_ball.png")	// 画像

/**************************************************************************//**
	@struct		ENEMYBULLET
	@brief		エネミー用の弾の構造体
	@par		[弾の定義]
*//***************************************************************************/
typedef struct	
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	D3DXVECTOR3		vec;						// ベクトルの向き
	D3DXVECTOR3		move;						// 移動量
	float			speed;						// 移動速度

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} ENEMYBULLET;

/**************************************************************************//**
	@brief		InitEnemyBullet
	@brief		弾の初期化処理
	@param[in]	type		テクスチャタイプ
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT InitEnemyBullet(int type);

/**************************************************************************//**
	@brief		UninitEnemyBullet
	@brief		弾の終了処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UninitEnemyBullet(void);

/**************************************************************************//**
	@brief		UpdateEnemyBullet
	@brief		弾の更新処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UpdateEnemyBullet(void);

/**************************************************************************//**
	@brief		DrawEnemyBullet
	@brief		弾の描画処理
	@param		なし
	@return		なし
*//***************************************************************************/
void DrawEnemyBullet(void);

/**************************************************************************//**
	@brief		SetEnemyBullet
	@brief		弾の発射設定
	@param[in]	pos		弾の座標
	@param[in]	type	弾の向き
	@return		ptr		弾の先頭アドレス
*//***************************************************************************/
void SetEnemyBullet(D3DXVECTOR3 enemyPos, D3DXVECTOR3 playerPos, int enemyType, int *countShot);

/**************************************************************************//**
	@brief		GetEnemyBullet
	@brief		弾のアドレス取得
	@param[in]	no		弾の番号
	@return		&blockWk[no] ブロック番号のアドレス
*//***************************************************************************/
ENEMYBULLET *GetEnemyBullet(int no);


#endif
