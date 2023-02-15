//========================================
// 
// HPバー[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_hp-bar_00.cpp ***
//========================================
#include "main.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"	// CHR:プレイヤー	[00]
#include "ui_hp-bar_00.h"	// UI :HPバー		[00]

//****************************************
// マクロ定義
//****************************************
// HPバー[00] の位置
// HPバー[00] のバー部分の相対位置
// HPバー[00] のテキストの相対位置
#define UI_HPBAR_00_POS					(D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*64),PIXEL*16,0.0f))
#define UI_HPBAR_00_BAR_RELATIVE_POS	(D3DXVECTOR3(PIXEL*-55,PIXEL*-4,0.0f))
#define UI_HPBAR_00_TEXT_RELATIVE_POS	(D3DXVECTOR3(PIXEL*13,PIXEL*11,0.0f))

// HPバー[00] のバー部分の0%カラー
// HPバー[00] のバー部分の100%カラー
#define UI_HPBAR_00_BAR_COLOR_0		Color{88,248,152,255}
#define UI_HPBAR_00_BAR_COLOR_100	Color{88,248,152,255}

//****************************************
// 列挙型の定義
//****************************************
// HPバー[00] の部品番号
typedef enum
{
	UI_HPBAR_00_PARTS_FRAME,		// フレーム
	UI_HPBAR_00_PARTS_BAR,			// バー
	UI_HPBAR_00_PARTS_BAR_RIGHT,	// バーの右端
	UI_HPBAR_00_PARTS_MAX,
}UI_HPBAR_00_PARTS;

// HPバー[00] のテクスチャ番号
typedef enum 
{
	UI_HPBAR_00_TEXTURE_FRAME,		// フレーム
	UI_HPBAR_00_TEXTURE_BAR,		// バー
	UI_HPBAR_00_TEXTURE_MAX,
}UI_HPBAR_00_TEXTURE;

//****************************************
// 構造体の定義
//****************************************
// HPバー[00] の部品毎の情報構造体
typedef struct
{
	UI_HPBAR_00_TEXTURE texture;	// テクスチャ
	int					nPattern;	// パターンNo.
	float				fWidth;		// 幅
	float				fHeight;	// 高さ
}Ui_hpBar_00Parts;

// HPバー[00] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath[TXT_MAX];	// テクスチャファイルの相対パス
	int		nPtnWidth;				// パターン幅
	int		nPtnHeight;				// パターン高さ
}Ui_hpBar_00Texture;

//****************************************
// プロトタイプ宣言
//****************************************
// HPバー[00] の部品毎の頂点座標設定処理
void UpdatePartsUi_hpBar_00(VERTEX_2D *pVtx, UI_HPBAR_00_PARTS parts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_hpBar_00	// テクスチャへのポインタ
						[UI_HPBAR_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_hpBar_00	// 頂点バッファへのポインタ
						= NULL;

// HPバー[00] のテクスチャ毎の情報
const Ui_hpBar_00Texture g_aUi_hpBar_00Texture[UI_HPBAR_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_hp-bar_000\\frame.png" , 1, 1 },	// フレーム
	{ "data\\TEXTURE\\UserInterface\\ui_hp-bar_000\\bar.png"   , 2, 1 },	// バー
};

// HPバー[00] の部品毎の情報
const Ui_hpBar_00Parts g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_MAX] =
{
	{ UI_HPBAR_00_TEXTURE_FRAME, 0, PIXEL * 128, PIXEL * 32 },	// フレーム
	{ UI_HPBAR_00_TEXTURE_BAR  , 0, PIXEL * 108, PIXEL * 16 },	// バー
	{ UI_HPBAR_00_TEXTURE_BAR  , 1, PIXEL * 1  , PIXEL * 16 },	// バーの右端
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_hpBar_00関数 - HPバー[00] の部品毎の頂点座標設定処理 -
//========================================
void UpdatePartsUi_hpBar_00(VERTEX_2D *pVtx, UI_HPBAR_00_PARTS parts) 
{
	float	fBarLengthRate	// バーの長さの倍率
			= ((float)GetChr_player_00()->nHP / (float)GetChr_player_00()->nHPMax);

	switch (parts)
	{
		//========== *** フレーム ***
	case UI_HPBAR_00_PARTS_FRAME:
	{
		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_HPBAR_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_hpBar_00Parts[parts].fWidth,
			g_aUi_hpBar_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
	break;
		//========== *** バー ***
	case UI_HPBAR_00_PARTS_BAR:
	{
		D3DXVECTOR3 basePos	// 基準位置
			= UI_HPBAR_00_POS + UI_HPBAR_00_BAR_RELATIVE_POS;

		// 頂点座標を設定
		pVtx[0].pos.x = basePos.x;
		pVtx[0].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[1].pos.x = basePos.x + (g_aUi_hpBar_00Parts[parts].fWidth * fBarLengthRate);
		pVtx[1].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[2].pos.x = basePos.x;
		pVtx[2].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[3].pos.x = basePos.x + (g_aUi_hpBar_00Parts[parts].fWidth * fBarLengthRate);
		pVtx[3].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, 
		{ 
			(int)((UI_HPBAR_00_BAR_COLOR_100.r * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.r * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.g * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.g * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.b * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.b * (1.0f - fBarLengthRate))),
			255
		});
	}
	break;
		//========== *** バーの右端 ***
	case UI_HPBAR_00_PARTS_BAR_RIGHT:
	{
		D3DXVECTOR3 basePos	// 基準位置
			= UI_HPBAR_00_POS + UI_HPBAR_00_BAR_RELATIVE_POS;

		// 頂点座標を設定
		pVtx[0].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate);
		pVtx[0].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[1].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate) + g_aUi_hpBar_00Parts[parts].fWidth;
		pVtx[1].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[2].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate);
		pVtx[2].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[3].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate) + g_aUi_hpBar_00Parts[parts].fWidth;
		pVtx[3].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx,
		{
			(int)((UI_HPBAR_00_BAR_COLOR_100.r * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.r * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.g * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.g * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.b * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.b * (1.0f - fBarLengthRate))),
			255
		});
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
// InitUi_hpBar_00関数 - HPバー[00] の初期化処理 -
//========================================
void InitUi_hpBar_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_HPBAR_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_hpBar_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_hpBar_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < UI_HPBAR_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_hpBar_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_hpBar_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_HPBAR_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			g_aUi_hpBar_00Parts[nCntParts].nPattern,
			g_aUi_hpBar_00Texture[g_aUi_hpBar_00Parts[nCntParts].texture].nPtnWidth,
			g_aUi_hpBar_00Texture[g_aUi_hpBar_00Parts[nCntParts].texture].nPtnHeight,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_hpBar_00->Unlock();
}

//========================================
// UninitUi_hpBar_00関数 - HPバー[00] の終了処理 -
//========================================
void UninitUi_hpBar_00(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < UI_HPBAR_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_hpBar_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_hpBar_00[nCntTexture]->Release();
			g_aTextureUi_hpBar_00[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_hpBar_00 != NULL) 
	{
		g_pVtxBuffUi_hpBar_00->Release();
		g_pVtxBuffUi_hpBar_00 = NULL;
	}
}

//========================================
// UpdateUi_hpBar_00関数 - HPバー[00] の更新処理 -
//========================================
void UpdateUi_hpBar_00(void) 
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_hpBar_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 部品毎の更新処理
		UpdatePartsUi_hpBar_00(pVtx, (UI_HPBAR_00_PARTS)nCntParts);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_hpBar_00->Unlock();

	{// テキストの設定
		char aString[TXT_MAX];	// 文字列
		sprintf(aString, "%3d/%3d", GetChr_player_00()->nHP, GetChr_player_00()->nHPMax);
		SetText2D(aString, FONT_000, DISPLAY_LEFT, UI_HPBAR_00_POS + UI_HPBAR_00_TEXT_RELATIVE_POS, { 255,255,255,255 }, 1.0f, 1.0f);
	}
}

//========================================
// DrawUi_hpBar_00関数 - HPバー[00] の描画処理 -
//========================================
void DrawUi_hpBar_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_hpBar_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_hpBar_00[g_aUi_hpBar_00Parts[nCntParts].texture]);

		// HPバー[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}