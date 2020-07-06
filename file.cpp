/*******************************************************************************
* scanf のwarning防止
*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <stdio.h>
#include "main.h"
#include "file.h"
#include "checkhit.h"
#include "player.h"
#include "input.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define MAP_TEXTURE                 _T("data/TEXTURE/maptip.jpg")    // マップチップ画像

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexMap(int x, int y);
void SetTexture(int x, int y);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static LPDIRECT3DTEXTURE9		g_pD3DTextureMap = NULL;		// テクスチャへのポリゴン
int data[SIZE_Y][SIZE_X];
MAP mapBlock[SIZE_Y][SIZE_X];

/*******************************************************************************
 関数名:	HRESULT InitFile(void)
 引数:		無し
 戻り値:	
 説明:		
*******************************************************************************/
HRESULT InitFile(void)
{
	data[SIZE_Y][SIZE_X] = {0};
	FILE *fp;
	char buff[SIZE_Y * SIZE_X], *pbuff;

	fp = fopen("data/DATA/maptest01.csv", "r");

	if (fp == NULL)
	{
		return S_FALSE;
	}

	for (int i = 0; i < SIZE_Y; i++)
	{
		fgets(buff, SIZE_Y * SIZE_X, fp);
		pbuff = buff;
		for (int j = 0; j < SIZE_X; j++)
		{
			data[i][j] = (int)strtod(pbuff, &pbuff);
			pbuff++;
		}
	}

	fclose(fp);

	return S_OK;
}


// 初期化処理
HRESULT InitMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
		MAP_TEXTURE,										// ファイルの名前
		&g_pD3DTextureMap);	// 読み込むメモリのポインタ

	for (int i = 0; i < SIZE_Y; i++)
	{
		for (int j = 0; j < SIZE_X; j++)
		{
			mapBlock[i][j].use = true;
			mapBlock[i][j].Texture = g_pD3DTextureMap;
			mapBlock[i][j].pos = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (j * MAP_TEXTURE_SIZE_X * 2),
									MAP_TEXTURE_SIZE_Y + (i * MAP_TEXTURE_SIZE_Y * 2), 0.0f);

			mapBlock[i][j].type = data[i][j];

			MakeVertexMap(j,i);
		}
	}
	return S_OK;
}


// 終了処理
void UninitMap(void)
{
	if (g_pD3DTextureMap != NULL)
	{
		// テクスチャの開放
		g_pD3DTextureMap->Release();
		g_pD3DTextureMap = NULL;
	}
}

// 更新処理
void UpdateMap(void)
{
	//PLAYER *player = GetPlayer();
	//for (int i = 0; i < SIZE_Y; i++)
	//{
	//	for (int j = 0; j < SIZE_X; j++)
	//	{
	//		if (mapBlock[i][j].type != -1)
	//		{
	//			if (CheckHitBB(player->pos, mapBlock[i][j].pos,
	//				D3DXVECTOR2(PLAYER_SIZE_STAND_X, PLAYER_SIZE_STAND_Y),
	//				D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y )) == 1)
	//			{
	//				player->state.falling = false;
	//				player->state.Grounded = true;
	//				player->jumpForce = PLAYER_JUMP_HIGH;

	//				player->pos.y = player->oldpos.y;
	//			}
	//		}
	//	}
	//}
}

// 描画処理
void DrawMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i <SIZE_Y; i++)
	{
		for (int j = 0; j <SIZE_X; j++)
		{
			if (mapBlock[i][j].use == true)		// 使用している状態なら描画する
			{
				// テクスチャの設定
				pDevice->SetTexture(0, mapBlock[i][j].Texture);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, mapBlock[i][j].vertexWk, sizeof(VERTEX_2D));

			}
		}
	}
}

HRESULT MakeVertexMap(int x, int y)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	mapBlock[y][x].vertexWk[0].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) - MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) - MAP_TEXTURE_SIZE_Y, 0.0f);
	mapBlock[y][x].vertexWk[1].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) + MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) - MAP_TEXTURE_SIZE_Y, 0.0f);
	mapBlock[y][x].vertexWk[2].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) - MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) + MAP_TEXTURE_SIZE_Y, 0.0f);
	mapBlock[y][x].vertexWk[3].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) + MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) + MAP_TEXTURE_SIZE_Y, 0.0f);

	// rhwの設定
	mapBlock[y][x].vertexWk[0].rhw =
	mapBlock[y][x].vertexWk[1].rhw =
	mapBlock[y][x].vertexWk[2].rhw =
	mapBlock[y][x].vertexWk[3].rhw = 1.0f;

	if (mapBlock[y][x].type != -1)
	{
		// 反射光の設定
		mapBlock[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		mapBlock[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		mapBlock[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		mapBlock[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	
	else
	{
		// 反射光の設定
		mapBlock[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		mapBlock[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		mapBlock[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		mapBlock[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	}

	if (mapBlock[y][x].type != BLOCK1 && mapBlock[y][x].type != -1)
	{
		// テクスチャ座標の設定
		mapBlock[y][x].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		mapBlock[y][x].vertexWk[1].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
		mapBlock[y][x].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[y][x].vertexWk[3].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
	}
	
	else if(mapBlock[y][x].type == BLOCK1)
	{

		// テクスチャ座標の設定
		mapBlock[y][x].vertexWk[0].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 0.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[y][x].vertexWk[1].tex = D3DXVECTOR2(2.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 0.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[y][x].vertexWk[2].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[y][x].vertexWk[3].tex = D3DXVECTOR2(2.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);

	}
	//else
	//{
	//	// テクスチャ座標の設定
	//	mapBlock[y][x].vertexWk[0].tex = D3DXVECTOR2(2.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 2.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
	//	mapBlock[y][x].vertexWk[1].tex = D3DXVECTOR2(3.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 2.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
	//	mapBlock[y][x].vertexWk[2].tex = D3DXVECTOR2(2.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 3.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
	//	mapBlock[y][x].vertexWk[3].tex = D3DXVECTOR2(3.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 3.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
	//}

	return S_OK;
}

void SetTexture(int x, int y)
{
	// 反射光の設定
	mapBlock[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	mapBlock[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	mapBlock[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	mapBlock[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
}

MAP *GetMapData(void)
{
	return &mapBlock[0][0];
}