//! @file	bg.h
//! @author	まよ
//! @date	2020-07-13
//! @brief	タイトル背景の定義

#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TITLE_POS_X			(0)								// タイトルの表示位置
#define	TITLE_POS_Y			(-100)							// タイトルの表示位置
#define	TITLE_SIZE_X		(800)							// タイトルの幅
#define	TITLE_SIZE_Y		(600)							// タイトルの高さ
#define	TITLELOGO_POS_X		(100)							// タイトルロゴの表示位置
#define	TITLELOGO_POS_Y		(100)							// タイトルロゴの表示位置
#define	TITLELOGO_SIZE_X	(480)							// タイトルロゴの幅
#define	TITLELOGO_SIZE_Y	(300)							// タイトルロゴの高さ


#define	RESULTLOGO_POS_X		(80)						// タイトルロゴの表示位置
#define	RESULTLOGO_POS_Y		(60)						// タイトルロゴの表示位置
#define	RESULTLOGO_SIZE_X		(480)						// タイトルロゴの幅
#define	RESULTLOGO_SIZE_Y		(360)						// タイトルロゴの高さ

enum BG_TEXTURE_TYPE
{
	TITLE_BG,
	TUTRIAL_BG,
	GAME_BG,
	BONUS_BG,
	RESULT_BG,
	MAX_TEXTURE_TYPE
};

/**************************************************************************//**
	@brief		InitBg
	@brief		タイトルの初期化処理
	@param[in]	type
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT InitBg(int type);

/**************************************************************************//**
	@brief		UninitBg
	@brief		タイトルの終了処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UninitBg(void);

/**************************************************************************//**
	@brief		UpdateBg
	@brief		タイトルの更新処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UpdateBg(void);

/**************************************************************************//**
	@brief		DrawBg
	@brief		タイトルの描画処理
	@param		なし
	@return		なし
*//***************************************************************************/
void DrawBg(void);

#endif
