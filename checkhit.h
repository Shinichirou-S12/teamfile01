//! @file	checkhit.h
//! @author	
//! @date	2020-06-18
//! @brief	衝突判定の定義

#ifndef ___CHECKHIT_H___
#define ___CHECKHIT_H___

enum CHECKBLOCK
{
	TOP,
	UNDER,
	LEFT_MAPCHIP,
	RIGHT_MAPCHIP,
	NOTTOUCHED,
	MAXCHECKBLOCK
};

/**************************************************************************//**
	@brief		CheckHitBB_MAP
	@brief		BBMAPの衝突判定
	@param[in]	pos1		pos1の座標
	@param[in]	pos2		pos2の座標
	@param[in]	size1		size1の大きさ
	@param[in]	size2		size2の大きさ
	@param[in]	moveSpeed	移動速度
	@return		true	当たっている
				false	当たっていない
*//***************************************************************************/
int CheckHitBB_MAP(D3DXVECTOR3 pos1, D3DXVECTOR3 pos_mapchip, D3DXVECTOR2 size1, D3DXVECTOR2 size_mapchip, float moveSpeed);

/**************************************************************************//**
	@brief		CheckHitBB
	@brief		BBの衝突判定
	@param[in]	pos1	pos1の座標
	@param[in]	pos2	pos2の座標
	@param[in]	size1	size1の大きさ
	@param[in]	size2	size2の大きさ
	@return		true	当たっている
				false	当たっていない
*//***************************************************************************/
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);

/**************************************************************************//**
	@brief		CheckHitBC
	@brief		BCの当たり判定
	@param[in]	pos1	pos1の座標
	@param[in]	pos2	pos2の座標
	@param[in]	size1	size1の大きさ
	@param[in]	size2	size2の大きさ
	@return		true	pos1とpos2が当たっている
				false	pos1とpos2が当たっていない
*//***************************************************************************/
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

/**************************************************************************//**
	@brief		FallCheckHitPlayer
	@brief		プレイヤーの落下処理の時の当たり判定
	@param		なし
	@return		なし
*//***************************************************************************/
void FallCheckHitPlayer(void);

/**************************************************************************//**
	@brief		Restriction
	@brief		プレイヤーの横移動で画面外やブロックを貫通しないための処理
	@param		なし
	@return		なし
*//***************************************************************************/
void Restriction(void);

#endif