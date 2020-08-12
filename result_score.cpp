//=============================================================================
//
// スコア処理 [result_score.cpp]
// Author : 
//
//=============================================================================
#include "result_score.h"
#include "file.h"
#include "score.h"
#include "savefile.h"
#include "controller.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	RESULT_SCORE_SIZE_X			(35.0f)							// スコアの数字の幅
#define	RESULT_SCORE_SIZE_Y			(50.0f)							// スコアの数字の高さ
#define	RESULT_SCORE_INTERVAL_X		(0.0f)							// スコアの数字の表示間隔

#define	NUM_PLACE					(8)								// スコアの桁数

#define	RESULT_SCORE_POS_X			(SCREEN_WIDTH - (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE - 20.0f)	// スコアの表示基準位置Ｘ座標
#define	RESULT_SCORE_POS_Y			(25.0f)																	// スコアの表示基準位置Ｙ座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult_Score(LPDIRECT3DDEVICE9 pDevice);
void SetTextureResult_Score(int line,int idx, int number);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult_Score[RESULT_SCORE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult_Score[RESULT_SCORE_MAX] = {};		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posResult_Score;										// 位置
D3DXVECTOR3				g_rotResult_Score;										// 向き

int						g_result_score[RESULT_SCORE_MAX];						// スコア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult_Score(int type)
{
	SAVEDATA *data = GetSaveData();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posResult_Score = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotResult_Score = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (type == 0)
	{
		for (int i = 0; i < RESULT_SCORE_MAX; i++)
		{
			// スコアの初期化
			//g_result_score[i] = i * 100;
			g_result_score[i] = data[i].r_score;

			// 頂点情報の作成
			MakeVertexResult_Score(pDevice);
		}
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_RESULT_SCORE,			// ファイルの名前
		&g_pD3DTextureResult_Score[0]);	// 読み込むメモリー


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult_Score(void)
{
	if (g_pD3DTextureResult_Score[0] != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult_Score[0]->Release();
		g_pD3DTextureResult_Score[0] = NULL;
	}
	if (g_pD3DTextureResult_Score[1] != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult_Score[1]->Release();
		g_pD3DTextureResult_Score[1] = NULL;
	}
	
	for (int i = 0; i < RESULT_SCORE_MAX; i++)
	{
		if (g_pD3DVtxBuffResult_Score[i] != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffResult_Score[i]->Release();
			g_pD3DVtxBuffResult_Score[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult_Score(void)
{
	if (GetInput(STARTBUTTON))
	{
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_sample002);
		return;
	}

	for (int i = 0; i < RESULT_SCORE_MAX; i++)
	{
		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
		{

			int number;
			int Patten = i*8 + nCntPlace;

			number = (g_result_score[i] % (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace)))) / (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace - 1)));
			SetTextureResult_Score(i,nCntPlace, number);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult_Score(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < RESULT_SCORE_MAX; i++)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffResult_Score[i], 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResult_Score[0]);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult_Score(LPDIRECT3DDEVICE9 pDevice)
{	
	for (int i = 0; i < RESULT_SCORE_MAX; i++)
	{

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResult_Score[i],					// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pD3DVtxBuffResult_Score[i]->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * RESULT_SCORE_SIZE_X + RESULT_SCORE_INTERVAL_X, RESULT_SCORE_POS_Y + 50.0f*i, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) + RESULT_SCORE_SIZE_X, RESULT_SCORE_POS_Y + 50.0f*i, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * RESULT_SCORE_SIZE_X + RESULT_SCORE_INTERVAL_X, RESULT_SCORE_POS_Y + 50.0f*i + RESULT_SCORE_SIZE_Y, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) + RESULT_SCORE_SIZE_X, RESULT_SCORE_POS_Y + 50.0f*i + RESULT_SCORE_SIZE_Y, 0.0f);

				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 反射光の設定
				/*pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.1f*i, 0.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.1f*i, 0.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.1f*i, 0.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.1f*i, 0.0f, 1.0f)*/

				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
			}

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X - 15, RESULT_SCORE_POS_Y - 25, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE + 15, RESULT_SCORE_POS_Y - 25, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X - 15, RESULT_SCORE_POS_Y + 55, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE + 15, RESULT_SCORE_POS_Y + 55, 0.0f);

				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// 頂点データをアンロックする
			g_pD3DVtxBuffResult_Score[i]->Unlock();
		}
	}
	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureResult_Score(int line,int idx, int number)
{

	VERTEX_2D* pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffResult_Score[line]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffResult_Score[0]->Unlock();

}

//=============================================================================
// スコアを取得
//=============================================================================
int *GetResult_Score(void)
{
	return &g_result_score[0];
}