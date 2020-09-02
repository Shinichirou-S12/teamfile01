//! @file	enemyBullet.cpp
//! @author	まよ
//! @date	2020-08-04
//! @brief	エネミー用の弾の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "bullet.h"
#include "sound.h"
#include "playerTest.h"
#include "enemy.h"
#include "enemyBullet.h"
#include "map.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemyBullet( int no );				// 弾の頂点情報作成
void SetTextureEnemyBullet(int no, int cntPattern);		// 弾の頂点座標の設定
void SetVertexEnemyBullet(int no);						// 弾のテクスチャの設定

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			g_pD3DTextureEnemyBullet = NULL;		// テクスチャへのポリゴン

ENEMYBULLET					g_enemyBullet[BULLET_MAX];			// 弾の構造体

//LPDIRECTSOUNDBUFFER8		g_pSE;							// SE用バッファ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemyBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET *bullet = &g_enemyBullet[0];		// エネミーのポインターを初期化

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_ENEMY_BULLET,		// ファイルの名前
			&g_pD3DTextureEnemyBullet);		// 読み込むメモリのポインタ
		
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
		bullet->speed = 3.0f;

		bullet->Texture = g_pD3DTextureEnemyBullet;					// テクスチャ情報
		MakeVertexEnemyBullet(i);									// 頂点情報の作成
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyBullet(void)
{
	ENEMYBULLET *bullet = g_enemyBullet;				// バレットのポインターを初期化

	if (g_pD3DTextureEnemyBullet != NULL)
	{	// テクスチャの開放
		g_pD3DTextureEnemyBullet->Release();
		g_pD3DTextureEnemyBullet = NULL;
	}

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		bullet->use = false;
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
void UpdateEnemyBullet(void)
{
	ENEMYBULLET *bullet = g_enemyBullet;				// バレットのポインターを初期化
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
			SetTextureEnemyBullet(i, bullet->PatternAnim);	// アニメーション後のテクスチャの設定

			// バレットの移動処理
			bullet->move = bullet->vec * bullet->speed;

			D3DXVec3Add(&bullet->pos, &bullet->pos, &bullet->move);

			// 画面外まで進んだ？
			if (bullet->pos.x < 0.0f
				|| bullet->pos.x >SCREEN_WIDTH
				||bullet->pos.y < 0.0f
				||bullet->pos.y > SCREEN_HEIGHT)	// 自分の大きさを考慮して画面外か判定している
			{
				bullet->use = false;
			}

			SetVertexEnemyBullet(i);				// 移動後の座標で頂点を設定
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET *bullet = g_enemyBullet;				// バレットのポインターを初期化

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
	@brief		MakeVertexEnemyBullet
	@brief		弾の頂点座標作成
	@param[in]	no		弾の番号
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT MakeVertexEnemyBullet( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET *bullet = &g_enemyBullet[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	SetVertexEnemyBullet( no );

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
	@brief		SetTextureEnemyBullet
	@brief		弾のテクスチャの座標設定
	@param[in]	no				弾の番号
	@param[in]	cntPattern		弾のアニメーションカウントパターン
	@return		なし
*//***************************************************************************/
void SetTextureEnemyBullet( int no, int cntPattern )
{
	ENEMYBULLET *bullet = &g_enemyBullet[no];			// バレットのポインターを初期化

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
	@brief		SetVertexEnemyBullet
	@brief		弾の頂点座標設定
	@param[in]	no		弾の番号
	@return		なし
*//***************************************************************************/
void SetVertexEnemyBullet( int no )
{
	ENEMYBULLET *bullet = &g_enemyBullet[no];			// バレットのポインターを初期化

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
// エネミーの弾の発射設定
//=============================================================================
void SetEnemyBullet(D3DXVECTOR3 enemyPos, D3DXVECTOR3 playerPos, int enemyType, int *countShot)
{
	ENEMYBULLET *bullet = &g_enemyBullet[0];			// バレットのポインターを初期化
	D3DXVECTOR3 vec;									// 仮ベクトル

	for (int j = 0; j < ENEMY_MAX; j++)
	{
		// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{
			D3DXVec3Subtract(&vec, &playerPos, &enemyPos);		// エネミーからプレイヤーに向けてのベクトルをvecに格納
			D3DXVec3Normalize(&vec, &vec);						// vecを正規化

 			if (bullet->use == false)
			{
				bullet->use = true;
				bullet->pos = enemyPos;
				D3DXVec3Scale(&vec, &vec, bullet->speed);	// 速度をあらかじめ早くする場合はスピードを足しておく。コメント可
				*countShot = 0;								// エネミーのカウントタイマーを初期化
				bullet->vec = vec;							// vecの値をバレットのベクトルに格納
				return;
			}
		}
	}
}

//=============================================================================
// ボスエネミーの弾の着弾設定
//=============================================================================
bool FallEnemyBullet(void)
{
	ENEMYBULLET *bullet = &g_enemyBullet[0];			// バレットのポインターを初期化
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use && bullet->pos.y >= (SCREEN_HEIGHT - (MAP_TEXTURE_SIZE_Y * 2)))
		{
			return true;
		}

	}
	return false;
}

//=============================================================================
// 弾の取得関数
//=============================================================================
ENEMYBULLET *GetEnemyBullet(int no)
{
	return(&g_enemyBullet[no]);
}

