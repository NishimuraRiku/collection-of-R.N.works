//========================================
// 
// フレーム[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_frame_00.cpp ***
//========================================
#include "main.h"
#include "polygon.h"
#include "ui_frame_00.h"

//****************************************
// マクロ定義
//****************************************

// フレーム[00] のテクスチャファイルの相対パス
// フレーム[00] の幅
// フレーム[00] の高さ
// フレーム[00] の位置
#define UI_FRAME_00_TEXTURE_PATH	"data\\TEXTURE\\ui_frame_000.png"
#define UI_FRAME_00_WIDTH			(SCREEN_WIDTH)
#define UI_FRAME_00_HEIGHT			(SCREEN_HEIGHT)
#define UI_FRAME_00_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f))

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_frame_00	//
						= NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_frame_00	//
						= NULL;					// 頂点バッファへのポインタ

//========================================
// InitUi_frame_00関数 - フレーム[00] の初期化処理 -
//========================================
void InitUi_frame_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, UI_FRAME_00_TEXTURE_PATH, &g_aTextureUi_frame_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_frame_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_frame_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	SetVertexPosition(pVtx,
		UI_FRAME_00_POS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_FRAME_00_WIDTH,
		UI_FRAME_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhwの設定
	SetRHW(pVtx);

	// 頂点カラーの設定
	SetVertexColor(pVtx, { 255,255,255,255 });

	// テクスチャ座標の設定
	SetTexturePosition(pVtx, 0, 1, 1, NULL);

	// 頂点座標をアンロックする
	g_pVtxBuffUi_frame_00->Unlock();
}

//========================================
// UninitUi_frame_00関数 - フレーム[00] の終了処理 -
//========================================
void UninitUi_frame_00(void)
{
	// テクスチャの破棄
	if (g_aTextureUi_frame_00 != NULL)
	{
		g_aTextureUi_frame_00->Release();
		g_aTextureUi_frame_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_frame_00 != NULL) 
	{
		g_pVtxBuffUi_frame_00->Release();
		g_pVtxBuffUi_frame_00 = NULL;
	}
}

//========================================
// DrawUi_frame_00関数 - フレーム[00] の描画処理 -
//========================================
void DrawUi_frame_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_frame_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureUi_frame_00);

	// フレーム[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}