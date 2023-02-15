//========================================
// 
// UI:スクリーン[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_screen_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "ui_screen_00.h"	// UI:スクリーン	[00]

//****************************************
// マクロ定義
//****************************************
// UI:スクリーン[00] のテクスチャファイルの相対パス
// UI:スクリーン[00] の幅
// UI:スクリーン[00] の高さ
#define UI_SCREEN_00_TEXTURE_PATH	"data\\TEXTURE\\UserInterface\\ui_screen_000.png"
#define UI_SCREEN_00_WIDTH			(1280)
#define UI_SCREEN_00_HEIGHT			(720)

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_screen_00	// テクスチャへのポインタ
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_screen_00	// 頂点バッファへのポインタ
						= NULL;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_screen_00関数 - UI:スクリーン[00] の初期化処理 -
//========================================
void InitUi_screen_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, UI_SCREEN_00_TEXTURE_PATH, &g_aTextureUi_screen_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_screen_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_screen_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_SCREEN_00_WIDTH,
		UI_SCREEN_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, INITCOLOR);

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_screen_00->Unlock();
}

//========================================
// UninitUi_screen_00関数 - UI:スクリーン[00] の終了処理 -
//========================================
void UninitUi_screen_00(void)
{
	// テクスチャの破棄
	if (g_aTextureUi_screen_00 != NULL)
	{
		g_aTextureUi_screen_00->Release();
		g_aTextureUi_screen_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_screen_00 != NULL) 
	{
		g_pVtxBuffUi_screen_00->Release();
		g_pVtxBuffUi_screen_00 = NULL;
	}
}

//========================================
// UpdateUi_screen_00関数 - UI:スクリーン[00] の更新処理 -
//========================================
void UpdateUi_screen_00(void)
{
	
}

//========================================
// DrawUi_screen_00関数 - UI:スクリーン[00] の描画処理 -
//========================================
void DrawUi_screen_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_screen_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureUi_screen_00);

	// UI:スクリーン[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}