//! @file	item.h
//! @author	kitade mayumi
//! @date	2020-06-19
//! @brief	アイテムの定義

#ifndef _ITEM_H_
#define _ITEM_H_

enum KIND_FRUIT
{
	WATERMELON,
	APPLE,
	GRAPE,
	STAR,
	MAX_KIND
};

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_ITEM		(2)					// ポリゴン数

#define TEXTURE_ITEM_SIZE_X	(50/2) // テクスチャサイズ
#define TEXTURE_ITEM_SIZE_Y	(50/2) // 同上

#define TEXTURE_PATTERN_DIVIDE_X	(12)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION				(MAX_KIND)	// アニメーションの切り替わるカウント

#define ITEM_MAX					(50) // アイテムの最大数
#define ITEM_KIND_MAX				(4)	// アイテムの種類の最大数
#define ITEM_POINT_ICE				(10) // ポイント
#define ITEM_POINT_SALT_PEPPER		(10) // ポイント
#define ITEM_POINT_HERB				(20) // ポイント


/**************************************************************************//**
	@struct		ITEM
	@brief		アイテムの構造体
	@par		[アイテムの定義]
*//***************************************************************************/
typedef struct	
{
	bool			use;						// true:使用  false:未使用
	bool			grape_use;					// ブドウが使用されているか
	bool			delete_use;					// 使用していたものを消す
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	int				type;						// アイテムの種類

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// アイテムの半径
	float			BaseAngle;					// アイテムの角度
	int				Texturenum;					//テクスチャー番号
	int				point;						//アイテムの点数
	float			speed;						//アイテムの速度

} ITEM;

/**************************************************************************//**
	@brief		InitItem
	@brief		アイテムの初期化処理
	@param[in]	type		テクスチャタイプ
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT InitItem(int type);

/**************************************************************************//**
	@brief		UninitItem
	@brief		アイテムの終了処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UninitItem(void);

/**************************************************************************//**
	@brief		UpdateItem
	@brief		アイテムの更新処理
	@param		なし
	@return		なし
*//***************************************************************************/
void UpdateItem(void);

/**************************************************************************//**
	@brief		DrawItem
	@brief		アイテムの描画処理
	@param		なし
	@return		なし
*//***************************************************************************/
void DrawItem(void);

/**************************************************************************//**
	@brief		DrawItem
	@brief		アイテムのアドレス取得
	@param[in]	no		アイテムの番号
	@return		&itemWk[no] アイテムの番号のアドレス
*//***************************************************************************/
ITEM *GetItem(int no);

/**************************************************************************//**
	@brief		SetItem
	@brief		アイテムの座標設定
	@param[in]	pos		アイテムの座標
	@param[in]	speed	アイテムの移動スピード
	@return		item	アイテムの先頭アドレス
*//***************************************************************************/
//ITEM *SetItem(D3DXVECTOR3 pos, float speed);

#endif
