//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "playerTest.h"
#include "map.h"
#include "checkhit.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemy( int no );
void SetTextureEnemy( int no, int cntPattern );
void SetVertexEnemy( int no );
void SetEnemy(void);
void DeathEnemy(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;		// テクスチャへのポリゴン

static ENEMY					enemyWk[ENEMY_MAX];				// エネミー構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
			ENEMY_TEXTURE,										// ファイルの名前
			&g_pD3DTextureEnemy);								// 読み込むメモリのポインタ
	}


	// エネミーの初期化処理
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemyWk[i].use = true;										// 使用
		enemyWk[i].hp = 2;											// 体力
		enemyWk[i].damage = false;									// ダメージ判定

		enemyWk[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		enemyWk[i].move = D3DXVECTOR3(2.0f, 0.0f, 0.0f);			// 移動量を初期化
		enemyWk[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転量を初期化
		enemyWk[i].PatternAnim = 0;									// アニメパターン番号をランダムで初期化
		enemyWk[i].CountAnim = 0;									// アニメカウントを初期化
		
		D3DXVECTOR2 temp = D3DXVECTOR2(ENEMY_TEXTURE_SIZE_X, ENEMY_TEXTURE_SIZE_Y);
		enemyWk[i].Radius = D3DXVec2Length(&temp);					// エネミーの半径を初期化
		enemyWk[i].BaseAngle = atan2f(ENEMY_TEXTURE_SIZE_Y, ENEMY_TEXTURE_SIZE_X);	// エネミーの角度を初期化
		enemyWk[i].Texture = g_pD3DTextureEnemy;

		MakeVertexEnemy(i);											// 頂点情報の作成
	}

	SetEnemy();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_pD3DTextureEnemy != NULL)
	{	// テクスチャの開放
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemyWk[i].use == true)					// 使用している状態なら更新する
		{
			// アニメーション
			enemyWk[i].CountAnim++;
			if( ( enemyWk[i].CountAnim % ENEMY_TIME_ANIMATION ) == 0 )
			{
				// パターンの切り替え
				enemyWk[i].PatternAnim = ( enemyWk[i].PatternAnim + 1 ) % ENEMY_ANIM_PATTERN_NUM;

				// テクスチャ座標を設定
				SetTextureEnemy( i, enemyWk[i].PatternAnim );
			}
			
			// 回転処理
			if (i & 1)
			{
				enemyWk[i].rot.z += 0.05f;
			}
			else
			{
				D3DXVECTOR3 pos = GetPlayer()->pos - enemyWk[i].pos;
				enemyWk[i].rot.z = atan2f(pos.y, pos.x) - D3DX_PI / 2;
			}

			// 移動処理
			if (i == 0)
			{	// ０番だけプレイヤーをホーミングするようにしてみる
				D3DXVECTOR3 pos = GetPlayer()->pos - enemyWk[i].pos;
				float rot = atan2f(pos.y, pos.x);

				enemyWk[i].pos.x += cosf(rot) * enemyWk[i].move.x/3;
				enemyWk[i].pos.y += sinf(rot) * enemyWk[i].move.x/3;

			}
			else
			{	// 他のは横に動いているだけ
				enemyWk[i].pos += enemyWk[i].move;		// 移動させる
				if ((enemyWk[i].pos.x < 0) || (enemyWk[i].pos.x > SCREEN_WIDTH))
				{										// 横画面外へ出たら
					enemyWk[i].move.x *= -1;			// 反対方向へ移動させる
				}
			}

			CheckBullet();
			DeathEnemy();

			SetVertexEnemy(i);						// 移動後の座標で頂点を設定
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemyWk[i].use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, enemyWk[i].Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, enemyWk[i].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemy( int i )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexEnemy( i );

	// rhwの設定
	enemyWk[i].vertexWk[0].rhw =
	enemyWk[i].vertexWk[1].rhw =
	enemyWk[i].vertexWk[2].rhw =
	enemyWk[i].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	if (i & 1)
	{
		enemyWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		enemyWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		enemyWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		enemyWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	else
	{
		enemyWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(64, 255, 64, 255);
		enemyWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(64, 255, 64, 255);
		enemyWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(64, 255, 64, 255);
		enemyWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(64, 255, 64, 255);
	}

	// テクスチャ座標の設定
	enemyWk[i].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemyWk[i].vertexWk[1].tex = D3DXVECTOR2(1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	enemyWk[i].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_Y);
	enemyWk[i].vertexWk[3].tex = D3DXVECTOR2( 1.0f/ ENEMY_TEXTURE_PATTERN_DIVIDE_X, 1.0f/ENEMY_TEXTURE_PATTERN_DIVIDE_Y);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy( int i, int cntPattern )
{
	// テクスチャ座標の設定
	int x = cntPattern % ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_Y;
	
	enemyWk[i].vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	enemyWk[i].vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	enemyWk[i].vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	enemyWk[i].vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy( int i )
{
	// 頂点座標の設定
	enemyWk[i].vertexWk[0].vtx.x = enemyWk[i].pos.x - cosf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[0].vtx.y = enemyWk[i].pos.y - sinf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[0].vtx.z = 0.0f;

	enemyWk[i].vertexWk[1].vtx.x = enemyWk[i].pos.x + cosf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[1].vtx.y = enemyWk[i].pos.y - sinf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[1].vtx.z = 0.0f;

	enemyWk[i].vertexWk[2].vtx.x = enemyWk[i].pos.x - cosf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[2].vtx.y = enemyWk[i].pos.y + sinf(enemyWk[i].BaseAngle - enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[2].vtx.z = 0.0f;

	enemyWk[i].vertexWk[3].vtx.x = enemyWk[i].pos.x + cosf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[3].vtx.y = enemyWk[i].pos.y + sinf(enemyWk[i].BaseAngle + enemyWk[i].rot.z) * enemyWk[i].Radius;
	enemyWk[i].vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// エネミーの位置設定関数
//=============================================================================
void SetEnemy(void)
{
	MAP *map = GetMapData();
	ENEMY *enemy = GetEnemy();

	for (int p = 0; p < MAP_MAXDATA * SIZE_X * SIZE_Y; p++, map++)
	{
		if (map->type == GLASS15)
		{
			enemy->pos = map->pos;
			enemy++;
		}
	}
}

//=============================================================================
// エネミーの破棄設定
//=============================================================================
void DeathEnemy(void)
{
	ENEMY *enemy = GetEnemy();
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->pos.x <= 0.0f )
		{
			enemy->use = false;
		}
	}
}


//=============================================================================
// エネミー取得関数
//=============================================================================
ENEMY *GetEnemy(void)
{
	return(&enemyWk[0]);
}


