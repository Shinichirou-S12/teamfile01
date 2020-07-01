//=============================================================================
//
// ポリゴン処理 [Player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_


// マクロ定義
#define PLAYER_TEXTURE			_T("data/TEXTURE/runningman003.png")	// サンプル用画像  
#define PLAYER_TEXTURE_SIZE_X		(50/2) // テクスチャサイズ  
#define PLAYER_TEXTURE_SIZE_Y		(100/2) // 同上  

#define PLAYER_TEXTURE_BB_SIZE_X	(40/2) // テクスチャサイズ  
#define PLAYER_TEXTURE_BB_SIZE_Y	(80/2) // 同上  


#define PLAYER_TEXTURE_PATTERN_DIVIDE_X		(5)	// アニメパターンのテクスチャ内分割数（X)  
#define PLAYER_TEXTURE_PATTERN_DIVIDE_Y		(2)	// アニメパターンのテクスチャ内分割数（Y)  
#define PLAYER_ANIM_PATTERN_NUM				(PLAYER_TEXTURE_PATTERN_DIVIDE_X*PLAYER_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数  
#define PLAYER_TIME_ANIMATION				(4)	// アニメーションの切り替わるカウント  

typedef struct
{
	D3DXVECTOR3 oldPos;			// ポリゴンの移動量
	D3DXVECTOR3 pos;			// ポリゴンの移動量
	D3DXVECTOR3 rot;			// ポリゴンの回転量
	int			countAnim;		// アニメーションカウント 
	int			patternAnim;	// アニメーションパターンナンバー

	VERTEX_2D		vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク

}PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

#endif
