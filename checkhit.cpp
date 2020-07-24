//! @file	checkhit.cpp
//! @author	まよ
//! @date	2020-06-18
//! @brief	衝突判定の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "checkhit.h"
#include "playerTest.h"
#include "file.h"
#include "map.h"
#include "playerTest.h"
#include "item.h"
#include "life.h"
#include "score.h"
#include "enemy.h"
#include "bullet.h"
#include "controller.h"
#include "effect.h"

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

	for (int i = 0; i < (SIZE_Y * SIZE_X * MAP_MAXDATA); i++, mapchip++)
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
					// 地面と動くブロックの位置が一緒になった場合落ちてしまうのを防ぐための複合条件
					if (player->partsState <= TWO && mapchip->pos.y < (SCREEN_HEIGHT - (MAP_TEXTURE_SIZE_Y * 3.0f + PLAYER_TEXTURE_BB_SIZE_TOP_X)))
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
	}
}

//=============================================================================
// プレイヤーの横移動で画面外やブロックを貫通しないための処理
//=============================================================================
void Restriction(void)
{
	MAP *mapchip = GetMapData();
	PLAYER *player = GetPlayer();

	for (int j = 0; j < SIZE_X * SIZE_Y * MAP_MAXDATA; j++, mapchip++)
	{
		if (mapchip->type != -1 && mapchip->type != 15 && mapchip->type != 2)
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

//=============================================================================
// アイテムとプレイヤーとの衝突判定
//=============================================================================
void CheckHitItem(void)
{
	// アイテムとプレイヤー(BC)
	ITEM *item = GetItem(0);
	PLAYER *player = GetPlayer();


	if (player->use)
	{
		for (int j = 0; j < ITEM_MAX; j++, item++)
		{
			if (item->use == false)
			{
				continue;		//未使用なら処理なし
			}
			if (CheckHitBC(player->pos, item->pos, PLAYER_TEXTURE_BB_SIZE_X, TEXTURE_ITEM_SIZE_X))
			{
				item->use = false;		 // アイテムの消滅処理を行い
				item->delete_use = true; // 

				if (item->grape_use == true)
				{
					//体力回復処理
					ChangeLife(1);
				}

				ChangeScore(item->point * 10);		// スコア加算
			}
		}
	}
}

//=============================================================================
// エネミーとプレイヤーとの衝突判定
//=============================================================================
void CheckHitEnemy(void)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	CHANGE_LIFE *life = GetLifeState();

	bool damage = false;
	if (player->use)
	{
		for (int i = 0; i < ENEMY_MAX; i++, enemy++)
		{
			if (enemy->use == false)
			{
				continue;
			}

			// エネミーとの衝突判定を行う
			if (CheckHitBC(player->pos, enemy->pos, PLAYER_TEXTURE_BB_SIZE_X, ENEMY_TEXTURE_SIZE_X))
			{
				damage = true;
				break;
			}
		}

		if (damage && player->invincible == false)
		{
			// プレイヤーのHPが減少する
			player->hp--;
			player->invincible = true;
			life--;
			ChangeLife(-1);
			// ライフの減少
			ChangeScore(-100);
			damage = false;
		}
	}
}

//=============================================================================
// バレットとの衝突判定
//=============================================================================
void CheckBullet(void)
{
	ENEMY *enemy = GetEnemy();
	CHANGE_LIFE *life = GetLifeState();
	BULLET *bullet = GetBullet(0);

	for (int j = 0; j < BULLET_MAX; j++, bullet++)
	{
		if (bullet[j].use == false) continue;

		for (int i = 0; i < ENEMY_MAX; i++, enemy++)
		{
			if (enemy->use == false) continue;

			if (CheckHitBB(enemy->pos, bullet->pos, 
				D3DXVECTOR2(TEXTURE_BULLET_SIZE_X, TEXTURE_BULLET_SIZE_Y), 
				D3DXVECTOR2(ENEMY_TEXTURE_SIZE_X, ENEMY_TEXTURE_SIZE_Y)))
			{
 				enemy->damage = true;
				bullet[j].use = false;
				ChangeScore(100);
				enemy->use = false;
				SetEffect(enemy->pos.x, enemy->pos.y, 15);
			}
		}
	}


}

