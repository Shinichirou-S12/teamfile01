//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 
//
//=============================================================================
#include "score.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_SCORE	"data/TEXTURE/frame_score.png"	// 読み込むテクスチャファイル名
#define	SCORE_SIZE_X		(35.0f)							// スコアの数字の幅
#define	SCORE_SIZE_Y		(50.0f)							// スコアの数字の高さ
#define	SCORE_INTERVAL_X	(0.0f)							// スコアの数字の表示間隔
#define	SCORE_INTERVAL_Y	(10.0f)							// スコアの数字の表示間隔

#define	NUM_PLACE			(5)								// スコアの桁数

#define	SCORE_POS_X			(SCREEN_WIDTH - (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_PLACE - 20.0f)	// スコアの表示基準位置Ｘ座標
#define	SCORE_POS_Y			(25.0f)																	// スコアの表示基準位置Ｙ座標

#define CHANGE_TIME			(24)							// スコアの一桁が変化時にかかるフレーム数
#define TOTAL_TIME			(40)							// スコア全部が変化するにかかるフレーム数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);
void SetTextureScore(int idx, int number);
void SetTextureChangeScore(int idx, int number, D3DXCOLOR color,float posY);
void ResetChangeScore(void);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureScore[2] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffScore = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posScore;						// 位置
D3DXVECTOR3				g_rotScore;						// 向き

SCORE					g_changeScore;					// 値変化用スコア
int						g_score;						// スコア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// スコアの初期化
	g_score = 0;

	// 変化用スコアの初期化
	ResetChangeScore();


	// 頂点情報の作成
	MakeVertexScore(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_SCORE,			// ファイルの名前
								&g_pD3DTextureScore[0]);	// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FRAME_SCORE,		// ファイルの名前
								&g_pD3DTextureScore[1]);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	if(g_pD3DTextureScore[0] != NULL)
	{// テクスチャの開放
		g_pD3DTextureScore[0]->Release();
		g_pD3DTextureScore[0] = NULL;
	}
	if(g_pD3DTextureScore[1] != NULL)
	{// テクスチャの開放
		g_pD3DTextureScore[1]->Release();
		g_pD3DTextureScore[1] = NULL;
	}

	if(g_pD3DVtxBuffScore != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffScore->Release();
		g_pD3DVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		int number;

		number = (g_score % (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace)))) / (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace - 1)));
		SetTextureScore(nCntPlace, number);
	}

#ifdef _DEBUG
	// DEBUG中、１を押すとlife+1する、２を押すとlife-1する
	// また、playerとの連動はまだしていない
	if (GetKeyboardTrigger(DIK_3))ChangeScore(300);
	if (GetKeyboardTrigger(DIK_4))ChangeScore(-300);

#endif

	// 変化するスコア
	if (g_changeScore.use == true)
	{
		static int count = 0;				// 線形補間するためのカウント
		int bCount = count;					// 計算用カウント
		float radious, posY;				// 補間の角度、、高さ

		if (count >= TOTAL_TIME)
		{
			ResetChangeScore();
			count = 0;
			return;
		}
		count++;

		if(count<= CHANGE_TIME)radious = D3DX_PI / CHANGE_TIME * count;
		else radious = D3DX_PI;
		posY = sinf(radious)*SCORE_SIZE_Y/2;

		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
		{
			int number;

			number = (g_changeScore.score % (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace)))) / (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace - 1)));
			SetTextureChangeScore(nCntPlace, number,g_changeScore.color, posY);

			bCount-=2;
			if (bCount <= 0)bCount = 0;
			if (bCount <= CHANGE_TIME)radious = D3DX_PI / CHANGE_TIME * bCount;
			else radious = D3DX_PI;
			posY = sinf(radious)*SCORE_SIZE_Y / 2;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureScore[0]);

	// ポリゴンの描画
	for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
	}

	if (g_changeScore.use == true)
	{
		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4+ nCntPlace * 4), NUM_POLYGON);
		}
	}

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureScore[1]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 2 * 4), NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE * 2 + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
												FVF_VERTEX_2D,									// 使用する頂点フォーマット
												D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffScore,							// 頂点バッファインターフェースへのポインタ
												NULL)))											// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

		// 表示score
		for(int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		// 変化score
		for(int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y+ SCORE_INTERVAL_Y + SCORE_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y+ SCORE_INTERVAL_Y + SCORE_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y + SCORE_SIZE_Y+ SCORE_INTERVAL_Y + SCORE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y + SCORE_SIZE_Y+ SCORE_INTERVAL_Y + SCORE_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse =D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);
			pVtx[1].diffuse =D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);
			pVtx[2].diffuse =D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);
			pVtx[3].diffuse =D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(SCORE_POS_X - 15, SCORE_POS_Y-25, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_PLACE + 15, SCORE_POS_Y-25, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(SCORE_POS_X - 15, SCORE_POS_Y + 55, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_PLACE + 15, SCORE_POS_Y + 55, 0.0f);

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
		g_pD3DVtxBuffScore->Unlock();
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScore(int idx, int number)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffScore->Unlock();
}

void SetTextureChangeScore( int idx, int number, D3DXCOLOR color,float posY)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (NUM_PLACE) * 4 + (idx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(SCORE_POS_X + idx * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y+(SCORE_INTERVAL_Y+ SCORE_SIZE_Y)- posY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + idx * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y + (SCORE_INTERVAL_Y + SCORE_SIZE_Y) - posY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(SCORE_POS_X + idx * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y + SCORE_SIZE_Y + (SCORE_INTERVAL_Y + SCORE_SIZE_Y) - posY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + idx * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y + SCORE_SIZE_Y + (SCORE_INTERVAL_Y + SCORE_SIZE_Y) - posY, 0.0f);

	// 反射光の設定
	pVtx[0].diffuse = color;
	pVtx[1].diffuse = color;
	pVtx[2].diffuse = color;
	pVtx[3].diffuse = color;

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffScore->Unlock();
}

//=============================================================================
// スコアの変更
//=============================================================================
void ChangeScore(int value)
{
	g_score += value;
	if (g_score < 0)
	{
		// 最小値になるとき
		g_score = 0;
	}
	else if (g_score >= (int)(powf(10.0f, (float)(NUM_PLACE + 1))))
	{
		// 最大値になるとき
		g_score = (int)(powf(10.0f, (float)(NUM_PLACE + 1))) - 1;
	}

	ResetChangeScore();
	if (value >= 0)g_changeScore.color = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);		// 青
	else g_changeScore.color = D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);			// 赤

	g_changeScore.score = abs(value);
	g_changeScore.use = true;

}

void ResetChangeScore(void)
{
	g_changeScore.color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_changeScore.score = 0;
	g_changeScore.use = false;

}


