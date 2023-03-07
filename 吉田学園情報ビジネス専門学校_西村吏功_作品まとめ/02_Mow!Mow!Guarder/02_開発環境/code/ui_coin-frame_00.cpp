//========================================
// 
// UI:コインフレーム[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_coin-frame_00.cpp ***
//========================================
#include "main.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:プレイヤー		[00]
#include "ui_coin-frame_00.h"	// UI :コインフレーム	[00]

//****************************************
// マクロ定義
//****************************************
// UI:コインフレーム[00] の位置
// UI:コインフレーム[00] のコインテキストの相対位置
// UI:コインフレーム[00] のスコアテキストの相対位置
#define UI_COINFRAME_00_POS						(D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*64),PIXEL*16,0.0f))
#define UI_COINFRAME_00_COIN_TEXT_RELATIVE_POS	(D3DXVECTOR3(PIXEL*-23,PIXEL*-4,0.0f))
#define UI_COINFRAME_00_SCORE_TEXT_RELATIVE_POS	(D3DXVECTOR3(PIXEL*-10,PIXEL*11,0.0f))

//****************************************
// 列挙型の定義
//****************************************
// UI:コインフレーム[00] の部品番号
typedef enum
{
	UI_COINFRAME_00_PARTS_FRAME,	// フレーム
	UI_COINFRAME_00_PARTS_MAX,
}UI_COINFRAME_00_PARTS;

// UI:コインフレーム[00] のテクスチャ番号
typedef enum 
{
	UI_COINFRAME_00_TEXTURE_FRAME,	// フレーム
	UI_COINFRAME_00_TEXTURE_MAX,
}UI_COINFRAME_00_TEXTURE;

//****************************************
// 構造体の定義
//****************************************
// UI:コインフレーム[00] の部品毎の情報構造体
typedef struct
{
	UI_COINFRAME_00_TEXTURE texture;	// テクスチャ
	int						nPattern;	// パターンNo.
	float					fWidth;		// 幅
	float					fHeight;	// 高さ
}Ui_coinFrame_00Parts;

// UI:コインフレーム[00] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath[TXT_MAX];	// テクスチャファイルの相対パス
	int		nPtnWidth;				// パターン幅
	int		nPtnHeight;				// パターン高さ
}Ui_coinFrame_00Texture;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:コインフレーム[00] の部品毎の頂点座標設定処理
void UpdatePartsUi_coinFrame_00(VERTEX_2D *pVtx, UI_COINFRAME_00_PARTS parts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_coinFrame_00	// テクスチャへのポインタ
						[UI_COINFRAME_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_coinFrame_00	// 頂点バッファへのポインタ
						= NULL;

// UI:コインフレーム[00] のテクスチャ毎の情報
const Ui_coinFrame_00Texture g_aUi_coinFrame_00Texture[UI_COINFRAME_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_coin-frame_000.png" , 1, 1 },	// フレーム
};

// UI:コインフレーム[00] の部品毎の情報
const Ui_coinFrame_00Parts g_aUi_coinFrame_00Parts[UI_COINFRAME_00_PARTS_MAX] =
{
	{ UI_COINFRAME_00_TEXTURE_FRAME, 0, PIXEL * 128, PIXEL * 32 },	// フレーム
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_coinFrame_00関数 - UI:コインフレーム[00] の部品毎の頂点座標設定処理 -
//========================================
void UpdatePartsUi_coinFrame_00(VERTEX_2D *pVtx, UI_COINFRAME_00_PARTS parts) 
{
	switch (parts)
	{
		//========== *** フレーム ***
	case UI_COINFRAME_00_PARTS_FRAME:
	{
		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_COINFRAME_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_coinFrame_00Parts[parts].fWidth,
			g_aUi_coinFrame_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
	break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_coinFrame_00関数 - UI:コインフレーム[00] の初期化処理 -
//========================================
void InitUi_coinFrame_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_COINFRAME_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_coinFrame_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_coinFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < UI_COINFRAME_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_coinFrame_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_coinFrame_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_COINFRAME_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			g_aUi_coinFrame_00Parts[nCntParts].nPattern,
			g_aUi_coinFrame_00Texture[g_aUi_coinFrame_00Parts[nCntParts].texture].nPtnWidth,
			g_aUi_coinFrame_00Texture[g_aUi_coinFrame_00Parts[nCntParts].texture].nPtnHeight,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_coinFrame_00->Unlock();
}

//========================================
// UninitUi_coinFrame_00関数 - UI:コインフレーム[00] の終了処理 -
//========================================
void UninitUi_coinFrame_00(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < UI_COINFRAME_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_coinFrame_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_coinFrame_00[nCntTexture]->Release();
			g_aTextureUi_coinFrame_00[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_coinFrame_00 != NULL) 
	{
		g_pVtxBuffUi_coinFrame_00->Release();
		g_pVtxBuffUi_coinFrame_00 = NULL;
	}
}

//========================================
// UpdateUi_coinFrame_00関数 - UI:コインフレーム[00] の更新処理 -
//========================================
void UpdateUi_coinFrame_00(void) 
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_coinFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_COINFRAME_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 部品毎の更新処理
		UpdatePartsUi_coinFrame_00(pVtx, (UI_COINFRAME_00_PARTS)nCntParts);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_coinFrame_00->Unlock();

	{// コインテキストの設定
		char aString[TXT_MAX];	// 文字列
		sprintf(aString, "%6d", GetChr_player_00()->nCoin);
		SetText2D(aString, FONT_001, DISPLAY_LEFT, UI_COINFRAME_00_POS + UI_COINFRAME_00_COIN_TEXT_RELATIVE_POS, { 255,255,255,255 }, 1.0f, 1.0f);
	}

	{// スコアテキストの設定
		char aString[TXT_MAX];	// 文字列
		if (GetMode() == MODE_TUTORIAL_00)
		{// 現在のモードがチュートリアルの時、
			sprintf(aString, "      ?????");
		}
		else
		{// 現在のモードがチュートリアルでない時、
			sprintf(aString, "%11d", GetChr_player_00()->nScore);
		}
		SetText2D(aString, FONT_000, DISPLAY_LEFT, UI_COINFRAME_00_POS + UI_COINFRAME_00_SCORE_TEXT_RELATIVE_POS, { 255,255,255,255 }, 1.0f, 1.0f);
	}
}

//========================================
// DrawUi_coinFrame_00関数 - UI:コインフレーム[00] の描画処理 -
//========================================
void DrawUi_coinFrame_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_coinFrame_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_COINFRAME_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_coinFrame_00[g_aUi_coinFrame_00Parts[nCntParts].texture]);

		// UI:コインフレーム[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}