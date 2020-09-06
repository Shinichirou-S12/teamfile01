//! @file	bullet.cpp
//! @author	kitade mayumi
//! @date	2020-06-18
//! @brief	弾の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "bullet.h"
//#include "sound.h"
#include "playerTest.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet( int no );					// 弾の頂点情報作成
void SetTextureBullet(int no, int cntPattern);		// 弾の頂点座標の設定
void SetVertexBullet(int no);						// 弾のテクスチャの設定

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;		// テクスチャへのポリゴン

BULLET					bulletWk[BULLET_MAX];			// 弾の構造体

//LPDIRECTSOUNDBUFFER8	g_pSE;							// SE用バッファ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = &bulletWk[0];		// エネミーのポインターを初期化

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_BULLET,			// ファイルの名前
			&g_pD3DTextureBullet);			// 読み込むメモリのポインタ
		
		//g_pSE = LoadSound(SE_00);

	}

	// バレットの初期化処理
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		bullet->use = false;									// 未使用（発射されていない弾）
		bullet->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);			// 座標データを初期化
		bullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		bullet->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		bullet->CountAnim = 0;									// アニメカウントを初期化
		bullet->speed = 5.0f;

		bullet->Texture = g_pD3DTextureBullet;					// テクスチャ情報
		MakeVertexBullet(i);									// 頂点情報の作成
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	if (g_pD3DTextureBullet != NULL)
	{	// テクスチャの開放
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}

	//if (g_pSE != NULL)
	//{	// テクスチャの開放
	//	g_pSE->Release();
	//	g_pSE = NULL;
	//}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = bulletWk;				// バレットのポインターを初期化
	PLAYER *player = GetPlayer();

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == true)			// 使用している状態なら更新する
		{
			// アニメーション  
			bullet->CountAnim++;
			if ((bullet->CountAnim % TIME_ANIMATION_BULLET) == 0)
			{
				// パターンの切り替え
				bullet->PatternAnim = (bullet->PatternAnim + 1) % ANIM_PATTERN_NUM_BULLET;
			}
			SetTextureBullet(i, bullet->PatternAnim);	// アニメーション後のテクスチャの設定

			// バレットの移動処理
			bullet->pos.x += bullet->move.x;

			// 画面外まで進んだ？
			if (bullet->pos.x < 0.0f
				|| bullet->pos.x >SCREEN_WIDTH)	// 自分の大きさを考慮して画面外か判定している
			{
				bullet->use = false;
			}

			SetVertexBullet(i);							// 移動後の座標で頂点を設定
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = bulletWk;				// バレットのポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == true)			// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, bullet->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BULLET, bullet->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

/**************************************************************************//**
	@brief		MakeVertexBullet
	@brief		弾の頂点座標作成
	@param[in]	no		弾の番号
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT MakeVertexBullet( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = &bulletWk[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	SetVertexBullet( no );

	// rhwの設定
	bullet->vertexWk[0].rhw =
	bullet->vertexWk[1].rhw =
	bullet->vertexWk[2].rhw =
	bullet->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	bullet->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	bullet->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	bullet->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET, 0.0f);
	bullet->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET);
	bullet->vertexWk[3].tex = D3DXVECTOR2( 1.0f/ TEXTURE_PATTERN_DIVIDE_X_BULLET, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_BULLET);

	return S_OK;
}

/**************************************************************************//**
	@brief		SetTextureBullet
	@brief		弾のテクスチャの座標設定
	@param[in]	no				弾の番号
	@param[in]	cntPattern		弾のアニメーションカウントパターン
	@return		なし
*//***************************************************************************/
void SetTextureBullet( int no, int cntPattern )
{
	BULLET *bullet = &bulletWk[no];			// バレットのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_BULLET;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
	
	bullet->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	bullet->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	bullet->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	bullet->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

/**************************************************************************//**
	@brief		SetVertexBullet
	@brief		弾の頂点座標設定
	@param[in]	no		弾の番号
	@return		なし
*//***************************************************************************/
void SetVertexBullet( int no )
{
	BULLET *bullet = &bulletWk[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	bullet->vertexWk[0].vtx.x = bullet->pos.x - TEXTURE_BULLET_SIZE_X;
	bullet->vertexWk[0].vtx.y = bullet->pos.y - TEXTURE_BULLET_SIZE_Y;
	bullet->vertexWk[0].vtx.z = 0.0f;

	bullet->vertexWk[1].vtx.x = bullet->pos.x + TEXTURE_BULLET_SIZE_X;
	bullet->vertexWk[1].vtx.y = bullet->pos.y - TEXTURE_BULLET_SIZE_Y;
	bullet->vertexWk[1].vtx.z = 0.0f;

	bullet->vertexWk[2].vtx.x = bullet->pos.x - TEXTURE_BULLET_SIZE_X;
	bullet->vertexWk[2].vtx.y = bullet->pos.y + TEXTURE_BULLET_SIZE_Y;
	bullet->vertexWk[2].vtx.z = 0.0f;

	bullet->vertexWk[3].vtx.x = bullet->pos.x + TEXTURE_BULLET_SIZE_X;
	bullet->vertexWk[3].vtx.y = bullet->pos.y + TEXTURE_BULLET_SIZE_Y;
	bullet->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// プレイヤーの弾の発射設定
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, int type)
{
	BULLET *bullet = &bulletWk[0];			// バレットのポインターを初期化

	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet[i].use == false)
		{
			bullet->use = true;
			bullet->pos = pos;

			// 弾の種類に応じて飛ばす方向を変えてみる処理
			switch (type)
			{
			case Right:
				bullet->move = D3DXVECTOR3(bullet->speed, 0.0f, 0.0f);			// 移動量を初期化
				bullet->rot.z = 0.0f;												// 右
				break;

			case Left:
				bullet->move = D3DXVECTOR3(-bullet->speed, 0.0f, 0.0f);			// 移動量を初期化
				bullet->rot.z = -D3DX_PI;											// 左
				break;
			}
			//PlaySound(SOUND_LABEL_SE_shot000);
			return;

		}
	}
}


//=============================================================================
// 弾の取得関数
//=============================================================================
BULLET *GetBullet(int no)
{
	return(&bulletWk[no]);
}

