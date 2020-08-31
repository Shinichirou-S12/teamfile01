//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : 劉培遠
//
//=============================================================================
#include "life.h"
#include "input.h"
#include "playerTest.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_LIFE		"data/TEXTURE/life000.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_LIFE	"data/TEXTURE/frame_life.png"	// 読み込むテクスチャファイル名
#define	LIFE_SIZE_X			(35.0f)							// ライフの数字の幅
#define	LIFE_SIZE_Y			(45.0f)							// ライフの数字の高さ
#define	LIFE_INTERVAL_X		(0.0f)							// ライフの数字の表示間隔

#define	MAX_LIFE			(4)								// ライフの最大値

#define	LIFE_POS_X			(20.0f)							// ライフの表示基準位置Ｘ座標
#define	LIFE_POS_Y			(25.0f)							// ライフの表示基準位置Ｙ座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
void SetTextureLife(int nIdx,float y,float alpha);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureLife[2] = { NULL };		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLife = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posLife;						// 位置
D3DXVECTOR3				g_rotLife;						// 向き
int						g_nLife;						// ライフ
int						lifeBuff;

CHANGE_LIFE				g_lifeStatus;					// ライフの増減状態

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posLife = D3DXVECTOR3(LIFE_POS_X + LIFE_SIZE_X/2, LIFE_POS_Y + LIFE_SIZE_Y/2, 0.0f);
	g_rotLife = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ライフの初期化
	g_nLife = lifeBuff = MAX_LIFE;

	// ライフ状態の初期化
	g_lifeStatus = NONE;

	// 頂点情報の作成
	MakeVertexLife(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_LIFE,			// ファイルの名前
								&g_pD3DTextureLife[0]);	// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FRAME_LIFE,			// ファイルの名前
								&g_pD3DTextureLife[1]);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	if(g_pD3DTextureLife[0] != NULL)
	{// テクスチャの開放
		g_pD3DTextureLife[0]->Release();
		g_pD3DTextureLife[0] = NULL;
	}
	if(g_pD3DTextureLife[1] != NULL)
	{// テクスチャの開放
		g_pD3DTextureLife[1]->Release();
		g_pD3DTextureLife[1] = NULL;
	}

	if(g_pD3DVtxBuffLife != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffLife->Release();
		g_pD3DVtxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
	static int count=0;				// 線形補間するためのカウント
	float radious, alpha, posY;		// 補間の角度、透明度、高さ
	
	PLAYER *player = GetPlayer();

	if (!player->use)
	{
		return;
	}

	// 線形補間する
	if (g_lifeStatus != NONE)
	{
		count++;
		radious = D3DX_PI / CHANGE_TIME * count;
		alpha = 1.0f / CHANGE_TIME * count;
		posY =sinf(radious)*LIFE_SIZE_Y;
	}

	for (int i = 0; i < lifeBuff-g_nLife; i++)
	{
		// 状態によって処理を分ける
		if (g_lifeStatus == ADD)
		{
			SetTextureLife(g_nLife + i, posY, alpha);
		}
		else if (g_lifeStatus == MINUS)
		{
			SetTextureLife(g_nLife + i, posY, 1.0f - alpha);
		}

	}

	if (count >= CHANGE_TIME)
	{
		if (g_lifeStatus == ADD)g_nLife = lifeBuff;
		if (g_lifeStatus == MINUS)lifeBuff = g_nLife;
		if (g_nLife < 0)g_nLife = 0;

		count = 0;
		g_lifeStatus = NONE;
	}

#ifdef _DEBUG
	// DEBUG中、１を押すとlife+1する、２を押すとlife-1する
	// また、playerとの連動はまだしていない
	if (GetKeyboardTrigger(DIK_1))ChangeLife(-1);
	if (GetKeyboardTrigger(DIK_2))ChangeLife(1);


	//強制result行き
	if (GetKeyboardTrigger(DIK_0))SetScene(SCENE_RESULT);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pD3DVtxBuffLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureLife[0]);

	// ポリゴンの描画
	for(int nCntPlace = 0; nCntPlace < lifeBuff; nCntPlace++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
	}

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureLife[1]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (MAX_LIFE * 4), NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * MAX_LIFE + 4),		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_2D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffLife,							// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < MAX_LIFE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X - 15, LIFE_POS_Y - 25, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * MAX_LIFE + 15, LIFE_POS_Y - 25, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X - 15, LIFE_POS_Y + 55, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * MAX_LIFE + 15, LIFE_POS_Y + 55, 0.0f);

		// rhwの設定
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffLife->Unlock();
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
// 引数:何枚目のlifeの値-1、テクスチャーのｙ値、テクスチャーのα値
// nIdexは0から始まる為、ｎ番目のlifeのidex値はn-1
//=============================================================================
void SetTextureLife(int nIdx, float y, float alpha)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X + nIdx * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y - y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + nIdx * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y - y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X + nIdx * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y + LIFE_SIZE_Y - y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + nIdx * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y + LIFE_SIZE_Y - y, 0.0f);

	// 反射光の設定
	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffLife->Unlock();
}

//=============================================================================
// ライフの変更
//=============================================================================
void ChangeLife(int nValue)
{
	// もし変化中なら変化しない
	//if (g_lifeStatus != NONE)return;

	// もしlifeが+されるなら
	if (nValue > 0)
	{
		// +処理に入る
		lifeBuff++;			// 画像表示の為先にlifeを+する
		// lifeの最大値に達したら
		if (lifeBuff > MAX_LIFE)
		{
			lifeBuff--;
			g_lifeStatus = NONE;
		}
		if (g_lifeStatus == MINUS)lifeBuff= g_nLife+1;
		g_lifeStatus = ADD;
	}
	// もしlifeが-されるなら
	else if (nValue < 0)
	{
		g_nLife--;
		if (g_lifeStatus == ADD)g_nLife= lifeBuff-1;
		g_lifeStatus = MINUS;
	}

}

CHANGE_LIFE *GetLifeState(void)
{
	return &g_lifeStatus;
}

