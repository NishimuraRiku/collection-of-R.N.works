//========================================
// 
// UI:フレーム[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_frame_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "ui_frame_00.h"	// UI:フレーム	[00]

//****************************************
// マクロ定義
//****************************************
// UI:フレーム[00] のテクスチャファイルの相対パス
#define UI_FRAME_00_TEXTURE_PATH "data\\TEXTURE\\UserInterface\\ui_frame_000.png"
// UI:フレーム[00] の幅
#define UI_FRAME_00_WIDTH (PIXEL*128)
// UI:フレーム[00] の高さ
#define UI_FRAME_00_HEIGHT (PIXEL*16)
// UI:フレーム[00] の位置
#define UI_FRAME_00_POS D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y,0.0f)

//****************************************
// 構造体の定義
//****************************************
// UI:フレーム[00] の情報構造体
typedef struct 
{
	bool bShow;	// 表示フラグ
}Ui_frame_00;

//****************************************
// グローバル宣言
//****************************************
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_aTextureUi_frame_00 = NULL;
// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_frame_00 = NULL;
Ui_frame_00 g_ui_frame_00;	// UI:フレーム[00] の情報構造体

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_frame_00関数 - UI:フレーム[00] の初期化処理 -
//========================================
void InitUi_frame_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// 表示フラグを初期化
	g_ui_frame_00.bShow = false;

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
	SetVertexPos2D(pVtx,
		UI_FRAME_00_POS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_FRAME_00_WIDTH,
		UI_FRAME_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, INITCOLOR);

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_frame_00->Unlock();
}

//========================================
// UninitUi_frame_00関数 - UI:フレーム[00] の終了処理 -
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
// UpdateUi_frame_00関数 - UI:フレーム[00] の更新処理 -
//========================================
void UpdateUi_frame_00(void)
{
	// 表示フラグを初期化
	g_ui_frame_00.bShow = false;
}

//========================================
// DrawUi_frame_00関数 - UI:フレーム[00] の描画処理 -
//========================================
void DrawUi_frame_00(void)
{
	if (!g_ui_frame_00.bShow) 
	{// 表示フラグが偽の時、
		return;	// 処理を終了する
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_frame_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureUi_frame_00);

	// UI:フレーム[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetUi_frame_00関数 - UI:フレーム[00] の設定処理 -
//========================================
void SetUi_frame_00(void) 
{
	g_ui_frame_00.bShow = true;
}