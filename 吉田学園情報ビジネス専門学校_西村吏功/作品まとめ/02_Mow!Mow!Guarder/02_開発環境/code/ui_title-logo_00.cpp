//========================================
// 
// UI:タイトルロゴ[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_title-logo_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "ui_title-logo_00.h"	// UI :タイトルロゴ	[00]

//****************************************
// マクロ定義
//****************************************
// UI:タイトルロゴ[00] の位置
// UI:タイトルロゴ[00] のテキストの相対位置
#define UI_TITLELOGO_00_POS					(D3DXVECTOR3((BUFFER_WIDTH*0.5f)+PIXEL*64,(BUFFER_HEIGHT*0.5f)+PIXEL*-64,0.0f))
#define UI_TITLELOGO_00_TEXT_RELATIVE_POS	(D3DXVECTOR3(0,PIXEL*-13,0.0f))

//****************************************
// 列挙型の定義
//****************************************
// UI:タイトルロゴ[00] の部品番号
typedef enum
{
	UI_TITLELOGO_00_PARTS_TITLELOGO,	// タイトルロゴ
	UI_TITLELOGO_00_PARTS_MAX,
}UI_TITLELOGO_00_PARTS;

// UI:タイトルロゴ[00] のテクスチャ番号
typedef enum 
{
	UI_TITLELOGO_00_TEXTURE_TITLELOGO,	// タイトルロゴ
	UI_TITLELOGO_00_TEXTURE_MAX,
}UI_TITLELOGO_00_TEXTURE;

//****************************************
// 構造体の定義
//****************************************
// UI:タイトルロゴ[00] の部品毎の情報構造体
typedef struct
{
	UI_TITLELOGO_00_TEXTURE	tex;		// テクスチャ
	float					fWidth;		// 幅
	float					fHeight;	// 高さ
}Ui_titleLogo_00Parts;

// UI:タイトルロゴ[00] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath[TXT_MAX];	// テクスチャファイルの相対パス
	int		nPtnWidth;				// パターン幅
	int		nPtnHeight;				// パターン高さ
}Ui_titleLogo_00Texture;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:タイトルロゴ[00] の部品毎の頂点座標設定処理
void UpdatePartsUi_titleLogo_00(VERTEX_2D *pVtx, UI_TITLELOGO_00_PARTS parts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_titleLogo_00	// テクスチャへのポインタ
						[UI_TITLELOGO_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_titleLogo_00	// 頂点バッファへのポインタ
						= NULL;

// UI:タイトルロゴ[00] のテクスチャ毎の情報
const Ui_titleLogo_00Texture g_aUi_titleLogo_00Texture[UI_TITLELOGO_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_title-logo_000.png" , 1, 1 },	// タイトルロゴ
};

// UI:タイトルロゴ[00] の部品毎の情報
const Ui_titleLogo_00Parts g_aUi_titleLogo_00Parts[UI_TITLELOGO_00_PARTS_MAX] =
{
	{ UI_TITLELOGO_00_TEXTURE_TITLELOGO, PIXEL * 256, PIXEL * 128 },	// タイトルロゴ
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_titleLogo_00関数 - UI:タイトルロゴ[00] の部品毎の頂点座標設定処理 -
//========================================
void UpdatePartsUi_titleLogo_00(VERTEX_2D *pVtx, UI_TITLELOGO_00_PARTS parts) 
{
	switch (parts)
	{
	case /*/ タイトルロゴ /*/UI_TITLELOGO_00_PARTS_TITLELOGO:
	{
		
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
// InitUi_titleLogo_00関数 - UI:タイトルロゴ[00] の初期化処理 -
//========================================
void InitUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_TITLELOGO_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_titleLogo_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < UI_TITLELOGO_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_titleLogo_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_titleLogo_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_TITLELOGO_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_TITLELOGO_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_titleLogo_00Parts[nCntParts].fWidth,
			g_aUi_titleLogo_00Parts[nCntParts].fHeight,
			ANGLE_TYPE_FIXED);

		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			0,
			g_aUi_titleLogo_00Texture[g_aUi_titleLogo_00Parts[nCntParts].tex].nPtnWidth,
			g_aUi_titleLogo_00Texture[g_aUi_titleLogo_00Parts[nCntParts].tex].nPtnHeight,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// UninitUi_titleLogo_00関数 - UI:タイトルロゴ[00] の終了処理 -
//========================================
void UninitUi_titleLogo_00(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < UI_TITLELOGO_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_titleLogo_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_titleLogo_00[nCntTexture]->Release();
			g_aTextureUi_titleLogo_00[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_titleLogo_00 != NULL) 
	{
		g_pVtxBuffUi_titleLogo_00->Release();
		g_pVtxBuffUi_titleLogo_00 = NULL;
	}
}

//========================================
// UpdateUi_titleLogo_00関数 - UI:タイトルロゴ[00] の更新処理 -
//========================================
void UpdateUi_titleLogo_00(void) 
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_TITLELOGO_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 部品毎の更新処理
		UpdatePartsUi_titleLogo_00(pVtx, (UI_TITLELOGO_00_PARTS)nCntParts);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// DrawUi_titleLogo_00関数 - UI:タイトルロゴ[00] の描画処理 -
//========================================
void DrawUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_titleLogo_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_TITLELOGO_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_titleLogo_00[g_aUi_titleLogo_00Parts[nCntParts].tex]);

		// UI:タイトルロゴ[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}