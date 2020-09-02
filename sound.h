//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_TITLE,			// タイトルの音	
	SOUND_LABEL_BGM_GAMESTAGE,		// ゲーム中の音
	SOUND_LABEL_BGM_BOSS_STAGE,		// ボス戦の音
	SOUND_LABEL_BGM_RESULT,			// リザルトの音
	SOUND_LABEL_SE_SHOT01,	 		// 弾発射音
	SOUND_LABEL_SE_ITEM,	 		// アイテム音
	SOUND_LABEL_SE_JUMP,		 	// ジャンプ音
	SOUND_LABEL_SE_STARTBUTTON,	 	// スタートボタン音
	SOUND_LABEL_SE_WARP,			// ワープ音
	SOUND_LABEL_SE_SHOT02,			// 弾発射音
	SOUND_LABEL_SE_HIT,				// ヒット音
	SOUND_LABEL_MAX,				// サウンドラベルの数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

#endif
