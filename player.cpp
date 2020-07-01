//=============================================================================
//
// ポリゴン処理 [Player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "controller.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetTexturePlayer( int cntPattern );	
void SetVertexPlayer(void);					
void PlayerController(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer = NULL;		// テクスチャへのポリゴン 

PLAYER g_player;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_player.pos = D3DXVECTOR3(300.0f, 100.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.oldPos = D3DXVECTOR3(300.0f, 100.0f, 0.0f);

	g_player.countAnim = 0;
	g_player.patternAnim = 0;

	// テクスチャの読み込み  
	if(type == 0)	// 初回のみ読み込む
	{
		D3DXCreateTextureFromFile( pDevice,	// デバイスのポインタ
			PLAYER_TEXTURE,					// ファイルの名前
			&g_pD3DTexturePlayer );			// 読み込むメモリのポインタ
	}

	// 頂点情報の作成
	MakeVertexPlayer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if( g_pD3DTexturePlayer != NULL )	// 
	{	// テクスチャの開放
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	// アニメーション  
	g_player.countAnim++;
	if( ( g_player.countAnim % PLAYER_TIME_ANIMATION ) == 0 )
	{
		// パターンの切り替え
		g_player.patternAnim = ( g_player.patternAnim + 1 ) % PLAYER_ANIM_PATTERN_NUM;

		// テクスチャ座標を設定
		SetTexturePlayer( g_player.patternAnim );
	}
	g_player.oldPos = g_player.pos;

	PlayerController();

	SetVertexPlayer();	// 移動後の座標で頂点を設定
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定  
	pDevice->SetTexture( 0, g_pD3DTexturePlayer );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PLAYER, g_player.vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexPlayer();

	// rhwの設定
	g_player.vertexWk[0].rhw =
	g_player.vertexWk[1].rhw =
	g_player.vertexWk[2].rhw =
	g_player.vertexWk[3].rhw = 1.0f;

	// 反射光の設定  
	g_player.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定  
	g_player.vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	g_player.vertexWk[1].tex = D3DXVECTOR2( 1.0f/PLAYER_TEXTURE_PATTERN_DIVIDE_X, 0.0f );
	g_player.vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/PLAYER_TEXTURE_PATTERN_DIVIDE_Y );
	g_player.vertexWk[3].tex = D3DXVECTOR2( 1.0f/PLAYER_TEXTURE_PATTERN_DIVIDE_X, 1.0f/PLAYER_TEXTURE_PATTERN_DIVIDE_Y );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePlayer( int cntPattern )
{
	// テクスチャ座標の設定
	int x = cntPattern % PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / PLAYER_TEXTURE_PATTERN_DIVIDE_Y;
	g_player.vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	g_player.vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	g_player.vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	g_player.vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(void)
{
	// 頂点座標の設定 
	g_player.vertexWk[0].vtx.x = g_player.pos.x - PLAYER_TEXTURE_SIZE_X;
	g_player.vertexWk[0].vtx.y = g_player.pos.y - PLAYER_TEXTURE_SIZE_Y;
	g_player.vertexWk[0].vtx.z = 0.0f;

	g_player.vertexWk[1].vtx.x = g_player.pos.x + PLAYER_TEXTURE_SIZE_X;
	g_player.vertexWk[1].vtx.y = g_player.pos.y - PLAYER_TEXTURE_SIZE_Y;
	g_player.vertexWk[1].vtx.z = 0.0f;

	g_player.vertexWk[2].vtx.x = g_player.pos.x - PLAYER_TEXTURE_SIZE_X;
	g_player.vertexWk[2].vtx.y = g_player.pos.y + PLAYER_TEXTURE_SIZE_Y;
	g_player.vertexWk[2].vtx.z = 0.0f;

	g_player.vertexWk[3].vtx.x = g_player.pos.x + PLAYER_TEXTURE_SIZE_X;
	g_player.vertexWk[3].vtx.y = g_player.pos.y + PLAYER_TEXTURE_SIZE_Y;
	g_player.vertexWk[3].vtx.z = 0.0f;
}

/**************************************************************************//**
	@brief		入力コマンド処理
	@param		なし
	@return		なし
*//***************************************************************************/
void PlayerController(void)
{
	// コマンド入力、コントローラー入力操作
	if (GetInput(DOWNMOVE))
	{
		g_player.pos.y += 2.0f;
	}
	else if (GetInput(RIGHTMOVE))
	{
		g_player.pos.y -= 2.0f;
	}
	else if (GetInput(LEFTMOVE))
	{
		g_player.pos.x -= 2.0f;
	}
	else if (GetInput(UPMOVE))
	{
		g_player.pos.y -= 2.0f;
	}

}

PLAYER *GetPlayer(void)
{
	return &g_player;
}