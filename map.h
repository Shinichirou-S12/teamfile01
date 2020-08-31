//=============================================================================
//
// マップファイル処理 [map.h]
// Author : kitade mayumi
//
//=============================================================================

#include "main.h"

#define SIZE_X	(40)
#define SIZE_Y	(15)
#define MAP_MAXDATA	(4)
#define MAP_BONUS_MAXDATA	(4)

//#define MAP_TEXTURE_SIZE_BB_X			(16.0f/2.0f)		    // テクスチャサイズ
//#define MAP_TEXTURE_SIZE_BB_Y			(16.0f/2.0f)		    // 同上

#define MAP_TEXTURE_SIZE_X			(16.0f)		    // テクスチャサイズ
#define MAP_TEXTURE_SIZE_Y			(16.0f)		    // 同上
#define MAP_TEXTURE_PATTERN_DIVIDE_X	(2)			// アニメパターンのテクスチャ内分割数（X)
#define MAP_TEXTURE_PATTERN_DIVIDE_Y	(2)			// アニメパターンのテクスチャ内分割数（Y)

#define MAP_TEXTURE_PATTERN_BOG_DIVIDE_X	(4)			// 泥床のテクスチャ内分割数（X)
#define MAP_TEXTURE_PATTERN_BOG_DIVIDE_Y	(4)			// 泥床のテクスチャ内分割数（Y)
enum BLOCKTYPE
{
	BLOCK0,
	BLOCK1,
	BLOCK2,
	BLOCK3,
	BLOCK4,
	BLOCK5,
	BLOCK6,
	BLOCK7,
	BLOCK8,
	BLOCK9,
	BLOCK10,
	BLOCK11,
	BLOCK12,
	BLOCK13,
	GLASS14,
	GLASS15,
	BLOCK16,
	ITEM17,
	ITEM18,
	BOSS1,
	BOSS2,
	BOSS3,
	MAXTYPE
};

typedef struct
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	int				type;
	int				popCnt;						// 次にポップするまでのカウント
	float			countPos;
	D3DXVECTOR3		move;						// ポリゴンの移動量

	bool			scroll;						// スクロールの判定
	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
}MAP;

MAP *GetMapData(void);
