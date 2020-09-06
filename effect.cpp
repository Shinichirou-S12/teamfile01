//! @file	effect.cpp
//! @author	kitade mayumi
//! @date	2020-08-24
//! @brief	エフェクト処理の実装

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "effect.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define EMISSION_FULL 0		// 全体追加フラグon
#define EMISSION_RATE 1		// 全体追加フラグoff

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect( int nCount, LPDIRECT3DDEVICE9 pDevice);	 // エフェクトの頂点情報作成
void SetTextureEffect( int nCount, int nNum, int nCntPattern );		 // テクスチャ座標の設定
void SetVertexEffect( int nCount, int nNum);						 // 頂点座標の設定
void ResetParticle(int nCount, int nNum);							 // パーティクルのリセット

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureEffect[MAX_KIND_EFFECT] = { NULL };		// テクスチャへのポリゴン

static EFFECT					effectWk[EFFECT_NUM_EFFECTS];	// エネミー構造体
static int				g_effectTexDevideX;				// テクスチャのX分割数
static int				g_effectTexDevideY;				// テクスチャのY分割数
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int type)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// テクスチャーの初期化を行う
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
			EFFECT_TEXTURE_PLAYER_BLOOD,						// ファイルの名前
			&g_pD3DTextureEffect[PLAYER_BLOOD]);				// 読み込むメモリのポインタ

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
			EFFECT_TEXTURE_ENEMY_BLOOD,							// ファイルの名前
			&g_pD3DTextureEffect[ENEMY_BLOOD]);					// 読み込むメモリのポインタ

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
			EFFECT_TEXTURE_ITEM_HEAL,							// ファイルの名前
			&g_pD3DTextureEffect[ITEM_HEAL]);					// 読み込むメモリのポインタ

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
			EFFECT_TEXTURE_ITEM_GET,							// ファイルの名前
			&g_pD3DTextureEffect[ITEM_GET]);					// 読み込むメモリのポインタ

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
			EFFECT_TEXTURE_KILLER_APPEAR,							// ファイルの名前
			&g_pD3DTextureEffect[KILLER_APPEAR]);				// 読み込むメモリのポインタ
	}


	g_effectTexDevideX = EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_X;
	g_effectTexDevideY = EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_Y;

	// 初期化処理
	for (int nCount = 0; nCount < EFFECT_NUM_EFFECTS; nCount++)
	{
		effectWk[nCount].nUse = false;
		effectWk[nCount].nElapsed = 0;

		MakeVertexEffect(nCount, pDevice);										// 頂点情報の作成

		for (int nNum = 0; nNum < EFFECT_NUM_PARTS; nNum++)
		{
			effectWk[nCount].pParticle[nNum].Texture = g_pD3DTextureEffect[PLAYER_BLOOD];
			ResetParticle(nCount, nNum);
		}
	}

	return S_OK;
}

/**************************************************************************//**
	@brief			パーティクルのリセット
	@param[in,out]	nCount		エフェクトの数のカウント
	@param[in]		nNum		パーティクルの数
	@return			なし
*//***************************************************************************/
void ResetParticle(int nCount, int nNum) 
{
	// 座標データを初期化
	effectWk[nCount].pParticle[nNum].pos = 
								D3DXVECTOR3(effectWk[nCount].pos.x + rand() % 30 - 15,
									effectWk[nCount].pos.y + rand() % 30 - 15, 0.0f);
	effectWk[nCount].pParticle[nNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量

	effectWk[nCount].pParticle[nNum].nPatternAnim = 0;							// アニメパターン番号をランダムで初期化
	effectWk[nCount].pParticle[nNum].nCountAnim = 0;								// アニメカウントを初期化
	effectWk[nCount].pParticle[nNum].nLiveTime = 30;								// パーティクルの生存時間
	effectWk[nCount].pParticle[nNum].bIsFinish = 0;								// パーティクルの終了
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	for (int i = 0; i < MAX_KIND_EFFECT; i++)
	{
		if (g_pD3DTextureEffect[i] != NULL)
		{	// テクスチャの開放
			g_pD3DTextureEffect[i]->Release();
			g_pD3DTextureEffect[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	for (int nCount = 0; nCount < EFFECT_NUM_EFFECTS; nCount++)
	{
		if (effectWk[nCount].nUse)
		{
			if (effectWk[nCount].nIsEnding){
				effectWk[nCount].nUse = FALSE;
				continue;
			}

			//エミットが有効であればエフェクト作成処理を行う
			if (effectWk[nCount].bIsRemoveOnFinish == FALSE)
			{

				//エフェクト作成レートの増加処理
				if (effectWk[nCount].nEffectCount < EFFECT_NUM_PARTS)
					effectWk[nCount].nEmitCounter++;

				//バッファに空きがあり、追加タイミングが来ていれば新たなエフェクトを追加する
				while ((effectWk[nCount].nEffectCount < EFFECT_NUM_PARTS) && (effectWk[nCount].nEmitCounter > EMISSION_RATE))
				{
					//全体追加フラグがONであれば空き領域全てに新たなエフェクトを追加する
					if (EMISSION_FULL)
						effectWk[nCount].nEffectCount = EFFECT_NUM_PARTS;
					else
						effectWk[nCount].nEffectCount++;

					//エフェクト作成レートの初期化
					effectWk[nCount].nEmitCounter = 0;
				}

				effectWk[nCount].nElapsed++;

				//時間超過
				if ((effectWk[nCount].nDuration != -1) && (effectWk[nCount].nDuration < effectWk[nCount].nElapsed)){
					effectWk[nCount].bIsRemoveOnFinish = TRUE;
				}
			}

			int effectIndex = 0;

			//エフェクトの更新処理
			while (effectIndex < effectWk[nCount].nEffectCount)
			{
				if (effectWk[nCount].pParticle[effectIndex].nLiveTime > 0)
				{
					//生存フレーム減少
					effectWk[nCount].pParticle[effectIndex].nLiveTime--;

					//アニメパターン進行
					if (++effectWk[nCount].pParticle[effectIndex].nCountAnim > EFFECT_TIME_ANIMATION) {
						//アニメパターンが最大値に達した場合でも終了
						if (++effectWk[nCount].pParticle[effectIndex].nPatternAnim >= ((g_effectTexDevideX * g_effectTexDevideY) -1))
						{
							effectWk[nCount].pParticle[effectIndex].nPatternAnim = (g_effectTexDevideX * g_effectTexDevideY) - 1;
							effectWk[nCount].pParticle[effectIndex].nLiveTime = 0;
						}

						effectWk[nCount].pParticle[effectIndex].nCountAnim = 0;
					}

					SetTextureEffect(nCount, effectIndex, effectWk[nCount].pParticle[effectIndex].nPatternAnim);

					SetVertexEffect(nCount, effectIndex);						// 移動後の座標で頂点を設定

					effectIndex++;
				}
				else{
					if (effectWk[nCount].bIsRemoveOnFinish){
						if (effectWk[nCount].pParticle[effectIndex].bIsFinish == 0)
						{
							effectWk[nCount].pParticle[effectIndex].bIsFinish = 1;
							effectWk[nCount].nNumFinish++;
						}
						//終了処理
						if (effectWk[nCount].nNumFinish == effectWk[nCount].nEffectCount)
						{
							effectWk[nCount].nIsEnding = TRUE;
							break;
						}
						effectIndex++;
					}
					else{
						//バッファを初期化する
						ResetParticle(nCount, effectIndex);

						//末尾でなければインデックスを詰める
						if (effectIndex != (effectWk[nCount].nEffectCount - 1))
						{
							effectWk[nCount].pParticle[effectIndex] = effectWk[nCount].pParticle[effectWk[nCount].nEffectCount - 1];
							ResetParticle(nCount, (effectWk[nCount].nEffectCount - 1));
						}
						effectWk[nCount].nEffectCount--;
					}
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < EFFECT_NUM_EFFECTS; nCount++)
	{
		if (effectWk[nCount].nUse)
		{
			for (int nNum = 0; nNum < effectWk[nCount].nEffectCount; nNum++)
			{
				if (effectWk[nCount].pParticle[nNum].bIsFinish == 0) {
					// テクスチャの設定
					pDevice->SetTexture(0, effectWk[nCount].pParticle[nNum].Texture);
					// ポリゴンの描画
					pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, effectWk[nCount].pParticle[nNum].vertexWk, sizeof(VERTEX_2D));
				}
			}
		}
	}
}

/**************************************************************************//**
	@brief			エフェクトの頂点情報作成
	@param[in,out]	pDevice	デバイスのポインタ
	@param[in]		nCnt	エフェクトの数のカウント
	@return			S_OK	処理の成功
					E-FAIL	処理の失敗
*//***************************************************************************/
HRESULT MakeVertexEffect( int nCount,LPDIRECT3DDEVICE9 pDevice)
{
	for (int nNum = 0; nNum < EFFECT_NUM_PARTS; nNum++){

		// 頂点座標の設定
		SetVertexEffect(nCount,nNum);
		// 頂点座標の設定
		SetTextureEffect(nCount, nNum, 0);

		// rhwの設定
		effectWk[nCount].pParticle[nNum].vertexWk[0].rhw =
		effectWk[nCount].pParticle[nNum].vertexWk[1].rhw =
		effectWk[nCount].pParticle[nNum].vertexWk[2].rhw =
		effectWk[nCount].pParticle[nNum].vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		effectWk[nCount].pParticle[nNum].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		effectWk[nCount].pParticle[nNum].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		effectWk[nCount].pParticle[nNum].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		effectWk[nCount].pParticle[nNum].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	return S_OK;
}

/**************************************************************************//**
	@brief			テクスチャ座標の設定
	@param[in,out]	nCount		エフェクトの数のカウント
	@param[in]		nNum		パーティクルの数
	@param[in]		nCntPattern	エフェクトパターンのカウント
	@return			なし
*//***************************************************************************/
void SetTextureEffect( int nCount, int nNum, int nCntPattern )
{
	// テクスチャ座標の設定
	int x = nCntPattern % g_effectTexDevideX;
	int y = nCntPattern / g_effectTexDevideX;
	float sizeX = 1.0f / g_effectTexDevideX;
	float sizeY = 1.0f / g_effectTexDevideY;

	effectWk[nCount].pParticle[nNum].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y) * sizeY );
	effectWk[nCount].pParticle[nNum].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	effectWk[nCount].pParticle[nNum].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	effectWk[nCount].pParticle[nNum].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/**************************************************************************//**
	@brief			頂点座標の設定
	@param[in,out]	nCount		エフェクトの数のカウント
	@param[in]		nNum		パーティクルの数
	@return			なし
*//***************************************************************************/
void SetVertexEffect( int nCount, int nNum )
{
	int scene = GetScene();
	if (scene == SCENE_RESULT)
	{
		// 頂点座標の設定
		effectWk[nCount].pParticle[nNum].vertexWk[0].vtx.x = effectWk[nCount].pParticle[nNum].pos.x - EFFECT_TEXTURE_SIZE_FALRE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[0].vtx.y = effectWk[nCount].pParticle[nNum].pos.y - EFFECT_TEXTURE_SIZE_FALRE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[0].vtx.z = 0.0f;

		effectWk[nCount].pParticle[nNum].vertexWk[1].vtx.x = effectWk[nCount].pParticle[nNum].pos.x + EFFECT_TEXTURE_SIZE_FALRE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[1].vtx.y = effectWk[nCount].pParticle[nNum].pos.y - EFFECT_TEXTURE_SIZE_FALRE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[1].vtx.z = 0.0f;

		effectWk[nCount].pParticle[nNum].vertexWk[2].vtx.x = effectWk[nCount].pParticle[nNum].pos.x - EFFECT_TEXTURE_SIZE_FALRE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[2].vtx.y = effectWk[nCount].pParticle[nNum].pos.y + EFFECT_TEXTURE_SIZE_FALRE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[2].vtx.z = 0.0f;

		effectWk[nCount].pParticle[nNum].vertexWk[3].vtx.x = effectWk[nCount].pParticle[nNum].pos.x + EFFECT_TEXTURE_SIZE_FALRE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[3].vtx.y = effectWk[nCount].pParticle[nNum].pos.y + EFFECT_TEXTURE_SIZE_FALRE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[3].vtx.z = 0.0f;
	}
	else
	{
		// 頂点座標の設定
		effectWk[nCount].pParticle[nNum].vertexWk[0].vtx.x = effectWk[nCount].pParticle[nNum].pos.x - EFFECT_TEXTURE_SIZE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[0].vtx.y = effectWk[nCount].pParticle[nNum].pos.y - EFFECT_TEXTURE_SIZE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[0].vtx.z = 0.0f;

		effectWk[nCount].pParticle[nNum].vertexWk[1].vtx.x = effectWk[nCount].pParticle[nNum].pos.x + EFFECT_TEXTURE_SIZE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[1].vtx.y = effectWk[nCount].pParticle[nNum].pos.y - EFFECT_TEXTURE_SIZE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[1].vtx.z = 0.0f;

		effectWk[nCount].pParticle[nNum].vertexWk[2].vtx.x = effectWk[nCount].pParticle[nNum].pos.x - EFFECT_TEXTURE_SIZE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[2].vtx.y = effectWk[nCount].pParticle[nNum].pos.y + EFFECT_TEXTURE_SIZE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[2].vtx.z = 0.0f;

		effectWk[nCount].pParticle[nNum].vertexWk[3].vtx.x = effectWk[nCount].pParticle[nNum].pos.x + EFFECT_TEXTURE_SIZE_X;
		effectWk[nCount].pParticle[nNum].vertexWk[3].vtx.y = effectWk[nCount].pParticle[nNum].pos.y + EFFECT_TEXTURE_SIZE_Y;
		effectWk[nCount].pParticle[nNum].vertexWk[3].vtx.z = 0.0f;
	}
}

//=============================================================================
// エフェクトのセット
//=============================================================================
void SetEffect(float fX, float fY, int nDuration, int type)
{
	switch (type)
	{
	case PLAYER_BLOOD:
		g_effectTexDevideX = EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_X;
		g_effectTexDevideY = EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_Y;
		break;

	case ENEMY_BLOOD:
		g_effectTexDevideX = EFFECT_TEXTURE_ENEMY_BLOOD_PATTERN_DIVIDE_X;
		g_effectTexDevideY = EFFECT_TEXTURE_ENEMY_BLOOD_PATTERN_DIVIDE_Y;
		break;

	case ITEM_HEAL:
		g_effectTexDevideX = EFFECT_TEXTURE_ITEM_HEAL_PATTERN_DIVIDE_X;
		g_effectTexDevideY = EFFECT_TEXTURE_ITEM_HEAL_PATTERN_DIVIDE_Y;
		break;

	case ITEM_GET:
		g_effectTexDevideX = EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_X;
		g_effectTexDevideY = EFFECT_TEXTURE_PLAYER_BLOOD_PATTERN_DIVIDE_Y;
		break;

	case KILLER_APPEAR:
		g_effectTexDevideX = EFFECT_TEXTURE_KILLER_APPEAR_PATTERN_DIVIDE_X;
		g_effectTexDevideY = EFFECT_TEXTURE_KILLER_APPEAR_PATTERN_DIVIDE_Y;
		break;

	default:
		break;
	}

	// もし未使用のエフェクトが無かったら実行しない( =これ以上表示できないって事 )
	for (int nCount = 0; nCount < EFFECT_NUM_EFFECTS; nCount++)
	{
		if (effectWk[nCount].nUse == false)		// 未使用状態のエフェクトを見つける
		{
			effectWk[nCount].nUse = true;
			effectWk[nCount].nIsEnding = false;
			effectWk[nCount].bIsRemoveOnFinish = FALSE;

			effectWk[nCount].nDuration = nDuration;
			effectWk[nCount].pos.x = fX;			// 座標をセット
			effectWk[nCount].pos.y = fY;			// 座標をセット

			effectWk[nCount].nEffectCount = 0;
			effectWk[nCount].nElapsed = 0;
			effectWk[nCount].nEmitCounter = 0;
			effectWk[nCount].nNumFinish = 0;

			//パーティクルの初期化
			for (int nNum = 0; nNum < EFFECT_NUM_PARTS; nNum++)
			{
				effectWk[nCount].pParticle[nNum].Texture = g_pD3DTextureEffect[type];
				ResetParticle(nCount, nNum);
			}

			return;							// 1個セットしたので終了する
		}
	}
}
