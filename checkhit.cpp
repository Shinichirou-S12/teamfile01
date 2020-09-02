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
#include "enemyBullet.h"
#include "wall.h"
#include "killer.h"
#include "spear.h"
#include "substitute.h"
#include "sound.h"
#include "boss.h"
#include "fade.h"

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
// プレイヤーの落下処理の時の当たり判定
//=============================================================================
void FallCheckHitPlayer (void)
{
	MAP *mapchip = GetMapData();
	PLAYER *player = GetPlayer();

	for (int i = 0; i < (SIZE_Y * SIZE_X * MAP_MAXDATA); i++, mapchip++)
	{
		if ((player->jumpForce < 1) || player->dropSpeed >= PLAYER_ACCELE)	// ブロック横でジャンプするとブロック上辺に張り付くバグを抑制する処理
		{
			if (mapchip->use &&(mapchip->type == BLOCK1 || mapchip->type == BLOCK10 || mapchip->type == BLOCK12))
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

			if (mapchip->type == BLOCK0 || mapchip->type == BLOCK3 || mapchip->type == BLOCK16)
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

					if (mapchip->type == BLOCK3)
					{
						SlidePlayer();
					}
					if (mapchip->type == BLOCK16)
					{
						player->bogUse = true;
					}
					break;
				}
			}
		}
	}
}

//=============================================================================
// 身代わりアイテムの落下処理の時の当たり判定
//=============================================================================
void FallCheckHitSubstitute(void)
{
	MAP *mapchip = GetMapData();
	SUBSTITUTE *substitute = GetSubstitute();
	PLAYER *player = GetPlayer();

	for (int i = 0; i < (SIZE_Y * SIZE_X * MAP_MAXDATA); i++, mapchip++)
	{
		if ((substitute->jumpForce < 1) || substitute->dropSpeed >= PLAYER_ACCELE)	// ブロック横でジャンプするとブロック上辺に張り付くバグを抑制する処理
		{
			if (mapchip->use && (mapchip->type == BLOCK1 || mapchip->type == BLOCK10 || mapchip->type == BLOCK12))
			{
				if (CheckHitBB_MAP(substitute->pos, mapchip->pos, D3DXVECTOR2(SUBSTITUTE_TEXTURE_BB_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y),
					D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), player->moveSpeed) == TOP)	// ブロックの上に立っているとき
				{
					substitute->dropSpeed = 0;		// 重力加速度をリセット

					// 上からブロックに突っ込むと、ブロックの上に戻す
					substitute->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y - SUBSTITUTE_TEXTURE_SIZE_X;

					substitute->jumpForce = 0;		// ジャンプ回数をリセット
					substitute->rot.z = 0;			// 回転ジャンプの回転リセット
					break;
				}
			}

			if (mapchip->type == BLOCK0 || mapchip->type == BLOCK3 || mapchip->type == BLOCK16)
			{
				if (CheckHitBB_MAP(substitute->pos, mapchip->pos, D3DXVECTOR2(SUBSTITUTE_TEXTURE_BB_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y),
					D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), player->moveSpeed) == TOP)	// ブロックの上に立っているとき
				{
					substitute->dropSpeed = 0;		// 重力加速度をリセット

					// 上からブロックに突っ込むと、ブロックの上に戻す
					// テクスチャサイズに合わせて高さの変更を行う
					substitute->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y - SUBSTITUTE_TEXTURE_SIZE_X;
					substitute->jumpForce = 0;		// ジャンプ回数をリセット
					substitute->rot.z = 0;			// 回転ジャンプの回転リセット

					break;
				}
			}
		}
	}
}

//=============================================================================
// エネミーの落下処理の時の当たり判定
//=============================================================================
void FallCheckHitEnemy(int i)
{
	MAP *mapchip = GetMapData();
	ENEMY *enemy = GetEnemy();

	enemy += i;
	for (int j = 0; j < (SIZE_Y * SIZE_X * MAP_MAXDATA); j++, mapchip++)
	{
		if (mapchip->use && (mapchip->type == BLOCK1 || mapchip->type == BLOCK10 || mapchip->type == BLOCK12))
		{
			if (CheckHitBB_MAP(enemy->pos, mapchip->pos, D3DXVECTOR2(ENEMY_TEXTURE_BB_SIZE_X, ENEMY_TEXTURE_SIZE_Y),
				D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), ENEMY_MOVE_SPEED) == TOP)	// ブロックの上に立っているとき
			{
				enemy->dropSpeed = 0;		// 重力加速度をリセット

				// 上からブロックに突っ込むと、ブロックの上に戻す
				enemy->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y - ENEMY_TEXTURE_SIZE_X;
				break;
			}
		}

		if (mapchip->type == BLOCK0 || mapchip->type == BLOCK3 || mapchip->type == BLOCK16)
		{
			if (CheckHitBB_MAP(enemy->pos, mapchip->pos, D3DXVECTOR2(ENEMY_TEXTURE_SIZE_X, ENEMY_TEXTURE_SIZE_Y),
				D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), ENEMY_MOVE_SPEED) == TOP)	// ブロックの上に立っているとき
			{
				enemy->dropSpeed = 0;		// 重力加速度をリセット

				// 上からブロックに突っ込むと、ブロックの上に戻す
				enemy->pos.y = mapchip->pos.y - MAP_TEXTURE_SIZE_Y - ENEMY_TEXTURE_SIZE_X;
				break;
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
		if (mapchip->type != -1 && mapchip->use &&
			(mapchip->type == BLOCK0 || mapchip->type == BLOCK1 
			|| mapchip->type == BLOCK3 || mapchip->type == BLOCK10
			|| mapchip->type == BLOCK12 || mapchip->type == BLOCK16))
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

	if (player->countScroll < (2 * MAP_MAXDATA - 1))
	{
		if (player->pos.x >= SCREEN_WIDTH)	//画面中央より右に行けないようにする
		{
			player->scroll = true;
		}
	}
	else if(player->countScroll >= (2 * MAP_MAXDATA - 1)
		&&player->pos.x >= SCREEN_WIDTH - MAP_TEXTURE_SIZE_X)
	{
		player->pos.x = SCREEN_WIDTH - MAP_TEXTURE_SIZE_X;
	}
}

//=============================================================================
// 身代わりアイテムの横移動で画面外やブロックを貫通しないための処理
//=============================================================================
void RestrictionSubstitute(void)
{
	MAP *mapchip = GetMapData();
	SUBSTITUTE *substitute = GetSubstitute();
	PLAYER *player = GetPlayer();

	for (int j = 0; j < SIZE_X * SIZE_Y * MAP_MAXDATA; j++, mapchip++)
	{
		if (mapchip->type != -1 &&
			(mapchip->type == BLOCK0 || mapchip->type == BLOCK1
				|| mapchip->type == BLOCK3 || mapchip->type == BLOCK10
				|| mapchip->type == BLOCK12 || mapchip->type == BLOCK16))
		{
			switch (CheckHitBB_MAP(substitute->pos, mapchip->pos, D3DXVECTOR2(SUBSTITUTE_TEXTURE_BB_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_Y),
				D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), 0.0f))		// ブロックのどこに触れているか
			{
			case LEFT_MAPCHIP:
				substitute->pos.x = mapchip->pos.x - MAP_TEXTURE_SIZE_X - SUBSTITUTE_TEXTURE_SIZE_X;	// 左からブロックに突っ込むとブロックの左に戻す
				break;
			case RIGHT_MAPCHIP:
				substitute->pos.x = mapchip->pos.x + MAP_TEXTURE_SIZE_X + SUBSTITUTE_TEXTURE_SIZE_X;	// 右からブロックに突っ込むとブロックの右に戻す
				break;
			case UNDER:
				substitute->pos.y = mapchip->pos.y + MAP_TEXTURE_SIZE_Y + SUBSTITUTE_TEXTURE_SIZE_Y;	// 下からブロックに突っ込むとブロックの下に戻す
				break;
			}
		}
	}
}

//=============================================================================
// エネミーの横移動で画面外やブロックを貫通しないための処理
//=============================================================================
void RestrictionEnemy(int i)
{
	MAP *mapchip = GetMapData();
	ENEMY *enemy = GetEnemy();
	PLAYER *player = GetPlayer();

	enemy += i;
	for (int j = 0; j < SIZE_X * SIZE_Y * MAP_MAXDATA; j++, mapchip++)
	{
		if (mapchip->type != -1 && mapchip->use &&
			(mapchip->type == BLOCK0 || mapchip->type == BLOCK1
			|| mapchip->type == BLOCK3 || mapchip->type == BLOCK10
			|| mapchip->type == BLOCK12 || mapchip->type == BLOCK16))
		{
			switch (CheckHitBB_MAP(enemy->pos, mapchip->pos, D3DXVECTOR2(ENEMY_TEXTURE_SIZE_X, ENEMY_TEXTURE_SIZE_Y),
				D3DXVECTOR2(MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y), ENEMY_MOVE_SPEED))	// ブロックのどこに触れているか
			{
			case LEFT_MAPCHIP:
				enemy->pos.x = mapchip->pos.x - MAP_TEXTURE_SIZE_X - ENEMY_TEXTURE_SIZE_X;	// 左からブロックに突っ込むとブロックの左に戻す
				break;
			case RIGHT_MAPCHIP:
				enemy->pos.x = mapchip->pos.x + MAP_TEXTURE_SIZE_X + ENEMY_TEXTURE_SIZE_X;	// 右からブロックに突っ込むとブロックの右に戻す
				break;
			case UNDER:
				enemy->pos.y = mapchip->pos.y + MAP_TEXTURE_SIZE_Y + ENEMY_TEXTURE_SIZE_Y;	// 下からブロックに突っ込むとブロックの下に戻す
				break;
			}
		}
	}

	D3DXVECTOR3 vec;
	D3DXVec3Subtract(&vec, &player->pos, &enemy->pos);
	fabs(vec.x);

	if (player->scroll)	// 画面右に行けないようにする
	{
		if (vec.x <= (float)(SCREEN_WIDTH / 8) && vec.x >= 0.0f)
		{
			enemy->use = false;
		}
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

				//体力回復処理
				ChangeLife(1);

				if (player->partsState != PERFECT)
				{
					player->partsState++;
				}

				if (player->hp != PLAYER_HP)
				{
					player->hp++;
				}
				if (item->type == STAR)
				{
					player->superInvincible = true;
					item->use = false;
				}
				ChangeScore(item->point * 10);		// スコア加算
				//SetEffect(player->pos.x, player->pos.y, EFFECT_LIFE_TIME);
				PlaySound(SOUND_LABEL_SE_ITEM);

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
			if (!player->superInvincible)
			{
				player->invincible = true;
				// プレイヤーのHPが減少する
				player->hp--;
				life--;
				ChangeLife(-1);
				// ライフの減少
				ChangeScore(-SCORE_SNIPER_ENEMY);
				damage = false;
				SetEffect(player->pos.x, player->pos.y, EFFECT_LIFE_TIME);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
	}
}

//=============================================================================
// ボスとプレイヤーとの衝突判定
//=============================================================================
void CheckHitBoss(void)
{
	PLAYER *player = GetPlayer();
	BOSS *boss = GetBoss();
	CHANGE_LIFE *life = GetLifeState();

	bool damage = false;
	if (player->use)
	{
		for (int i = 0; i < BOSS_MAX; i++, boss++)
		{
			if (boss->use == false)
			{
				continue;
			}

			// エネミーとの衝突判定を行う
			if (CheckHitBC(player->pos, boss->pos, PLAYER_TEXTURE_BB_SIZE_X, BOSS_TEXTURE_SIZE_X))
			{
				damage = true;
				break;
			}
		}

		if (damage && player->invincible == false)
		{
			if (!player->superInvincible)
			{
				player->invincible = true;
				// プレイヤーのHPが減少する
				player->hp--;
				life--;
				ChangeLife(-1);
				// ライフの減少
				ChangeScore(-SCORE_SNIPER_ENEMY);
				damage = false;
				SetEffect(player->pos.x, player->pos.y, EFFECT_LIFE_TIME);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
	}
}


//=============================================================================
// 針とプレイヤーとの衝突判定
//=============================================================================
void CheckSpear(void)
{
	PLAYER *player = GetPlayer();
	SPEAR *spear = GetSpear(0);
	CHANGE_LIFE *life = GetLifeState();

	bool damage = false;

	for (int i = 0; i < SPEAR_MAX; i++, spear++)
	{
		if(spear->use)
		{ 
			if (CheckHitBC(player->pos, spear->pos, PLAYER_TEXTURE_BB_SIZE_X, TEXTURE_SPEAR_SIZE_X))
			{
				damage = true;
			}
		}
	}
	if (damage)
	{
		if (player->invincible == false)
		{
			// プレイヤーのHPが減少する
			player->hp--;
			player->invincible = true;
			life--;
			ChangeLife(-1);
			// ライフの減少
			ChangeScore(-SPEAR_DAMAGE_SCORE);
			PlaySound(SOUND_LABEL_SE_HIT);
		}
		damage = false;
		SetEffect(spear->pos.x, spear->pos.y, EFFECT_LIFE_TIME);
	}
}

//=============================================================================
// エネミーとバレットとの衝突判定
//=============================================================================
void CheckEnemyBullet(void)
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
				ChangeScore(SCORE_SNIPER_ENEMY);
				enemy->use = false;
				SetEffect(enemy->pos.x, enemy->pos.y, EFFECT_LIFE_TIME);
				PlaySound(SOUND_LABEL_SE_HIT);

			}
		}
	}
}

//=============================================================================
// プレイヤーとエネミーバレットとの衝突判定
//=============================================================================
void CheckPlayerBullet(void)
{
	PLAYER *player = GetPlayer();
	CHANGE_LIFE *life = GetLifeState();
	ENEMYBULLET *bullet = GetEnemyBullet(0);

	for (int j = 0; j < BULLET_MAX; j++, bullet++)
	{
		if (bullet[j].use == false) continue;

		if (player->use == false) continue;

		if (CheckHitBB(player->pos, bullet->pos,
			D3DXVECTOR2(TEXTURE_BULLET_SIZE_X, TEXTURE_BULLET_SIZE_Y),
			D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y)))
		{
			//player->hp -= 1;
			bullet[j].use = false;
			ChangeScore(-SCORE_SNIPER_ENEMY);
			SetEffect(player->pos.x, player->pos.y, EFFECT_LIFE_TIME);
			PlaySound(SOUND_LABEL_SE_HIT);

		}
	}
}

//=============================================================================
// 壁との衝突判定
//=============================================================================
void CheckHitWall(void)
{
	PLAYER *player = GetPlayer();
	WALL *wall = GetWall();
	ENEMY *enemy = GetEnemy();
	ITEM *item = GetItem(0);

	if (player->use)
	{
		if (wall->pos.x >= player->pos.x)
		{
			SetEffect(player->pos.x, player->pos.y, EFFECT_LIFE_TIME);
			player->use = false;
		}
	}

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == false) continue;

		if (wall->pos.x >= enemy->pos.x)
		{
			enemy->damage = true;
			enemy->use = false;
			SetEffect(enemy->pos.x, enemy->pos.y, EFFECT_LIFE_TIME);
		}
	}

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use == false) continue;

		if (wall->pos.x >= item->pos.x)
		{
			item->use = false;
			SetEffect(item->pos.x, item->pos.y, EFFECT_LIFE_TIME);
			PlaySound(SOUND_LABEL_SE_HIT);

		}
	}
}


//=============================================================================
// キラーとプレイヤーとの衝突判定
//=============================================================================
void CheckHitKiller(void)
{
	PLAYER *player = GetPlayer();
	KILLER *killer = GetKiller(0);
	CHANGE_LIFE *life = GetLifeState();

	bool damage = false;
	if (player->use)
	{
		for (int i = 0; i < KILLER_MAX; i++, killer++)
		{
			if (killer->use == false)
			{
				continue;
			}

			// キラーとの衝突判定を行う
			if (CheckHitBC(player->pos, killer->pos, PLAYER_TEXTURE_BB_SIZE_X, TEXTURE_KILLER_SIZE_X))
			{
				damage = true;
				break;
			}
		}

		if (damage)
		{
			if (player->invincible == false)
			{
				// プレイヤーのHPが減少する
				player->hp--;
				player->invincible = true;
				life--;
				ChangeLife(-1);
				// ライフの減少
				ChangeScore(-SCORE_SNIPER_ENEMY);
				PlaySound(SOUND_LABEL_SE_HIT);

			}
			damage = false;
			SetEffect(killer->pos.x, killer->pos.y, EFFECT_LIFE_TIME);
			killer->use = false;
			killer->dead = true;
		}
	}
}

//=============================================================================
// ワープゲートとプレイヤーとの衝突判定
//=============================================================================
bool CheckHitWarp(void)
{
	PLAYER *player = GetPlayer();
	MAP * map = GetMapData();

	if (player->use)
	{
		for (int i = 0; i < SIZE_X * SIZE_Y * MAP_MAXDATA; i++, map++)
		{
			if (map->type == BLOCK6)
			{
				// ワープエリアとの衝突判定を行う
				if (CheckHitBC(player->pos, map->pos, PLAYER_TEXTURE_BB_SIZE_X, SIZE_X))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// ゴールとプレイヤーとの衝突判定
//=============================================================================
void CheckHitGoal(void)
{
	PLAYER *player = GetPlayer();
	MAP * map = GetMapData();

	if (player->use)
	{
		for (int i = 0; i < SIZE_X * SIZE_Y * MAP_MAXDATA; i++, map++)
		{
			if (map->type == GOAL)
			{
				// ワープエリアとの衝突判定を行う
				if (CheckHitBC(player->pos, map->pos, PLAYER_TEXTURE_BB_SIZE_TOP_X, SIZE_GOAL_X))
				{
					PlaySound(SOUND_LABEL_SE_WARP);
					SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_GAMESTAGE);
					player->use = false;
				}
			}
		}
	}
}

//=============================================================================
// プレイヤーと身代わりアイテムの衝突判定
//=============================================================================
void CheckHitPlayerSubstitute(void)
{
	PLAYER *player = GetPlayer();
	SUBSTITUTE *substitute = GetSubstitute();

	if (player->use)
	{
		// 身代わりアイテムとの衝突判定を行う
		if (CheckHitBC(player->pos, substitute->pos, PLAYER_TEXTURE_BB_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_X))
		{
			if (!substitute->attackUse)
			{
				substitute->sticking = true;
				substitute->releaseUse = false;
				SetPosSubstitute();
				PlaySound(SOUND_LABEL_SE_ITEM);
			}
		}
	}
}

//=============================================================================
// エネミーと身代わりアイテムの衝突判定
//=============================================================================
void CheckHitEnemySubstitute(void)
{
	ENEMY *enemy = GetEnemy();
	SUBSTITUTE *substitute = GetSubstitute();

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == false)
		{
			continue;
		}
		// 身代わりアイテムとの衝突判定を行う
		if (CheckHitBC(enemy->pos, substitute->pos, ENEMY_TEXTURE_SIZE_X, SUBSTITUTE_TEXTURE_SIZE_X))
		{
			if (substitute->attackUse)
			{
				SetEffect(substitute->pos.x, substitute->pos.y, EFFECT_LIFE_TIME);
				substitute->use = false;
				enemy->use = false;
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
	}
}





