//! @file	checkhit.cpp
//! @author	まよ
//! @date	2020-06-18
//! @brief	衝突判定の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "checkhit.h"
#include "player.h"
#include "file.h"
#include "map.h"
#include "playerTest.h"

//=============================================================================
// マップチップとの当たり判定
//=============================================================================
int CheckHitBB_MAP(D3DXVECTOR3 pos1, D3DXVECTOR3 pos_mapchip, D3DXVECTOR2 size1, D3DXVECTOR2 size_mapchip, float moveSpeed)
{
	if ((pos1.x + size1.x > pos_mapchip.x - size_mapchip.x) && (pos1.x - size1.x < pos_mapchip.x + size_mapchip.x) &&
		(pos1.y + size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y + size1.y < pos_mapchip.y + size_mapchip.y))		// 対象の下辺がマップチップに触れているか
	{
		return TOP;	// マップチップの上に触れている
	}

	if ((pos1.x + size1.x > pos_mapchip.x - size_mapchip.x + moveSpeed) && (pos1.x - size1.x < pos_mapchip.x + size_mapchip.x - moveSpeed) &&	// PLAYER_MOVEを加減しているのは、縦につながったブロックに対して「横入力をしながらジャンプ」ができなくなるバグを抑制するため。
		(pos1.y - size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y - size1.y < pos_mapchip.y + size_mapchip.y))		// 対象の上辺がマップチップに触れているか
	{
		return UNDER;	// マップチップの下に触れている
	}

	if ((pos1.x + size1.x > pos_mapchip.x - size_mapchip.x) && (pos1.x + size1.x < pos_mapchip.x + size_mapchip.x) &&
		(pos1.y + size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y - size1.y < pos_mapchip.y + size_mapchip.y)) 	// 対象の右辺がマップチップに触れているか
	{
		return LEFT_MAPCHIP;	// マップチップの左に触れている
	}

	if ((pos1.x - size1.x > pos_mapchip.x - size_mapchip.x) && (pos1.x - size1.x < pos_mapchip.x + size_mapchip.x) &&
		(pos1.y + size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y - size1.y < pos_mapchip.y + size_mapchip.y))	// 対象の左辺がマップチップに触れているか
	{
		return RIGHT_MAPCHIP;	// マップチップの右に触れている
	}

	return NOTTOUCHED;
}


//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	size1 /= 2.0f;	// 半サイズにする
	size2 /= 2.0f;	// 同上

	if (pos2.x + size2.x > pos1.x - size1.x && pos1.x + size1.x > pos2.x - size2.x &&
		pos2.y + size2.y > pos1.y - size1.y && pos1.y + size1.y > pos2.y - size2.y)
	{
		return true;
	}

	return false;
}

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	if ((size1 + size2) * (size1 + size2) >
		(pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y))
	{
		return true;
	}

	return false;
}

//=============================================================================
// プライヤーの落下処理の時の当たり判定
//=============================================================================
void FallCheckHitPlayer (void)
{
	MAP *mapchip = GetMapData();
	PLAYER *player = GetPlayer();

	for (int i = 0; i < (SIZE_Y * SIZE_X * MAP_MAXDATA); i++)
	{
		if ((player->jumpForce < 1) || player->dropSpeed >= PLAYER_ACCELE)	// ブロック横でジャンプするとブロック上辺に張り付くバグを抑制する処理
		{
			if (mapchip->type == 1 || mapchip->type == 10)
			{
				if (CheckHitBB_MAP(player->pos, mapchip->pos, D3DXVECTOR2(PLAYER_TEXTURE_BB_SIZE_TOP_X, PLAYER_TEXTURE_SIZE_Y),
					D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), player->moveSpeed) == TOP)	// ブロックの上に立っているとき
				{
					player->dropSpeed = 0;		// 重力加速度をリセット

					// 上からブロックに突っ込むと、ブロックの上に戻す
					// テクスチャサイズに合わせて高さの変更を行う
					if (player->partsState <= TWO)
					{
						player->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y -
							(PLAYER_TEXTURE_SIZE_X / (MAXPARTS - player->partsState - (0.5f * player->partsState)
								+ (5.0f * (TWO - player->partsState))));
					}
					else
					{
						player->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y - PLAYER_TEXTURE_SIZE_X;
					}
					player->jumpForce = 0;		// ジャンプ回数をリセット
					player->rot.z = 0;			// 回転ジャンプの回転リセット
					break;
				}
			}

			if (mapchip->type == 0)
			{
				if (CheckHitBB_MAP(player->pos, mapchip->pos, D3DXVECTOR2(PLAYER_TEXTURE_BB_SIZE_X, PLAYER_TEXTURE_SIZE_Y),
					D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), player->moveSpeed) == TOP)	// ブロックの上に立っているとき
				{
					player->dropSpeed = 0;		// 重力加速度をリセット

					// 上からブロックに突っ込むと、ブロックの上に戻す
					// テクスチャサイズに合わせて高さの変更を行う
					if (player->partsState <= TWO)
					{
						player->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y -
							(PLAYER_TEXTURE_SIZE_X / (MAXPARTS - player->partsState - (0.5f * player->partsState)
								+ (5.0f * (TWO - player->partsState))));
					}
					else
					{
						player->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y - PLAYER_TEXTURE_SIZE_X;
					}
					player->jumpForce = 0;		// ジャンプ回数をリセット
					player->rot.z = 0;			// 回転ジャンプの回転リセット
					break;
				}
			}
		}
		mapchip++;
	}
}

//=============================================================================
// プレイヤーの横移動で画面外やブロックを貫通しないための処理
//=============================================================================
void Restriction(void)
{
	MAP *mapchip = GetMapData();
	PLAYER *player = GetPlayer();

	for (int j = 0; j < SIZE_X * SIZE_Y * MAP_MAXDATA; j++)
	{
		if (mapchip->type != -1 && mapchip->type != 15)
		{
			switch (CheckHitBB_MAP(player->pos, mapchip->pos, D3DXVECTOR2(PLAYER_TEXTURE_BB_SIZE_X, PLAYER_TEXTURE_SIZE_Y),
				D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), player->moveSpeed))	// ブロックのどこに触れているか
			{
			case LEFT_MAPCHIP:
				player->pos.x = mapchip->pos.x - MAP_TEXTURE_SIZE_X - PLAYER_TEXTURE_SIZE_X;	// 左からブロックに突っ込むとブロックの左に戻す
				break;
			case RIGHT_MAPCHIP:
				player->pos.x = mapchip->pos.x + MAP_TEXTURE_SIZE_X + PLAYER_TEXTURE_SIZE_X;	// 右からブロックに突っ込むとブロックの右に戻す
				break;
			case UNDER:
				player->pos.y = mapchip->pos.y + MAP_TEXTURE_SIZE_Y + PLAYER_TEXTURE_SIZE_Y;	// 下からブロックに突っ込むとブロックの下に戻す
				break;
			}
		}
		mapchip++;
	}

	if (player->pos.x <= MAP_TEXTURE_SIZE_X)	//画面左より左に行けないようにする
	{
		player->pos.x = MAP_TEXTURE_SIZE_X;
	}

	if (player->pos.x == SCREEN_WIDTH)	//画面中央より右に行けないようにする
	{
		player->scroll = true;
		player->countScroll++;
	}
}

