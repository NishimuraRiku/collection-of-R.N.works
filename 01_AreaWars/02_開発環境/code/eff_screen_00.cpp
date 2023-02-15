//========================================
// 
// スクリーン[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** eff_screen_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon.h"
#include "eff_screen_00.h"
#include <limits.h>

//****************************************
// マクロ定義
//****************************************

// スクリーン[00] のテクスチャファイルの相対パス
// スクリーン[00] の幅
// スクリーン[00] の高さ
#define EFF_SCREEN_00_TEXTURE_PATH	"data\\TEXTURE\\eff_screen_000_960x720.png"
#define EFF_SCREEN_00_WIDTH			(960)
#define EFF_SCREEN_00_HEIGHT		(720)

//****************************************
// 構造体の定義
//****************************************

// スクリーン[00] の情報構造体
typedef struct 
{
	Color	col;				// 色
	int		nCounterAlpha;		// 透明度カウンター
	int		nCounterAlphaMax;	// 透明度カウンターの最大値(開始値)
}Eff_screen_00;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterEff_screen_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_screen_00 //
						= NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_screen_00	//
						= NULL;					// 頂点バッファへのポインタ
Eff_screen_00			g_eff_screen_00;		// スクリーン[00] の情報構造体

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_screen_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterEff_screen_00(void) 
{
	g_eff_screen_00.col					= { 0,0,0,0 };	// 色
	g_eff_screen_00.nCounterAlpha		= 0;			// 透明度カウンター
	g_eff_screen_00.nCounterAlphaMax	= 0;			// 透明度カウンターの最大値(開始値)
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitEff_screen_00関数 - スクリーン[00] の初期化処理 -
//========================================
void InitEff_screen_00(void)
{
	// パラメーターの初期化処理
	InitParameterEff_screen_00();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, EFF_SCREEN_00_TEXTURE_PATH, &g_aTextureEff_screen_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_screen_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_screen_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	SetVertexPosition(pVtx,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		EFF_SCREEN_00_WIDTH,
		EFF_SCREEN_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhwの設定
	SetRHW(pVtx);

	// 頂点カラーの設定
	SetVertexColor(pVtx, g_eff_screen_00.col);

	// テクスチャ座標の設定
	SetTexturePosition(pVtx, 0, 1, 1, NULL);

	// 頂点座標をアンロックする
	g_pVtxBuffEff_screen_00->Unlock();
}

//========================================
// UninitEff_screen_00関数 - スクリーン[00] の終了処理 -
//========================================
void UninitEff_screen_00(void)
{
	// テクスチャの破棄
	if (g_aTextureEff_screen_00 != NULL)
	{
		g_aTextureEff_screen_00->Release();
		g_aTextureEff_screen_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEff_screen_00 != NULL) 
	{
		g_pVtxBuffEff_screen_00->Release();
		g_pVtxBuffEff_screen_00 = NULL;
	}
}

//========================================
// UpdateEff_screen_00関数 - スクリーン[00] の更新処理 -
//========================================
void UpdateEff_screen_00(void)
{
	Eff_screen_00	*pEff	// スクリーン[00] の情報のポインタ
					= &g_eff_screen_00;

	if (pEff->nCounterAlpha > 0)
	{// 透明度カウンターが0を上回っている時、
		// 透明度カウンターを減算
		pEff->nCounterAlpha--;

		// 透明度を設定
		pEff->col.nA = 255 * ((float)pEff->nCounterAlpha / (float)pEff->nCounterAlphaMax);

		VERTEX_2D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffEff_screen_00->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		SetVertexColor(pVtx, g_eff_screen_00.col);

		// 頂点座標をアンロックする
		g_pVtxBuffEff_screen_00->Unlock();
	}
}

//========================================
// DrawEff_screen_00関数 - スクリーン[00] の描画処理 -
//========================================
void DrawEff_screen_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEff_screen_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureEff_screen_00);

	// スクリーン[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetEff_screen_00関数 - スクリーン[00] の設定処理 -
//========================================
void SetEff_screen_00(int nTime, Color col) 
{
	// 色を代入
	g_eff_screen_00.col = col;

	// 透明度カウンターに時間を代入
	g_eff_screen_00.nCounterAlpha = nTime;
	g_eff_screen_00.nCounterAlphaMax = nTime;

	// 透明度カウンターの最大値を制御
	IntControl(&g_eff_screen_00.nCounterAlphaMax, INT_MAX, 1);
}