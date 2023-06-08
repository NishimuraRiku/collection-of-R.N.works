//========================================
// 
// UI:パラメーター[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_parameter_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "ui_parameter_00.h"	// UI :パラメーター	[00]

//****************************************
// マクロ定義
//****************************************
// UI:パラメーター[00] の位置
#define UI_PARAMETER_00_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*32),BUFFER_HEIGHT-(PIXEL*30),0.0f)

// UI:パラメーター[00] のLVテキストの相対位置
// UI:パラメーター[00] のLVテキストの間隔Y
// UI:パラメーター[00] のLVテキストの色
#define UI_PARAMETER_00_LV_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL*16,PIXEL*-12,0.0f)
#define UI_PARAMETER_00_LV_TEXT_SPACE_Y			(PIXEL*8)
#define UI_PARAMETER_00_LV_TEXT_COLOR			Color{255,255,255,255}

//****************************************
// 列挙型の定義
//****************************************
// UI:パラメーター[00] のポリゴン
typedef enum 
{
	UI_PARAMETER_00_POLYGON_FRAME,	// フレーム
	UI_PARAMETER_00_POLYGON_MAX,
}UI_PARAMETER_00_POLYGON;

// UI:パラメーター[00] のLVテキスト
typedef enum
{
	UI_PARAMETER_00_LV_TEXT_SWORD_ATK,	// 剣ATK
	UI_PARAMETER_00_LV_TEXT_BULLET_ATK,	// 弾ATK
	UI_PARAMETER_00_LV_TEXT_PLAYER_HP,	// プレイヤーHP
	UI_PARAMETER_00_LV_TEXT_UNIT_HP,	// ユニットHP
	UI_PARAMETER_00_LV_TEXT_MAX,
}UI_PARAMETER_00_LV_TEXT;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_parameter_00	// テクスチャへのポインタ
						[UI_PARAMETER_00_POLYGON_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_parameter_00	// 頂点バッファへのポインタ
						= NULL;

// UI:パラメーター[00] のポリゴン毎の情報
const Polygon2D g_aUi_parameter_00Polygon[UI_PARAMETER_00_POLYGON_MAX] = 
{
	{ "data\\TEXTURE\\UserInterface\\ui_parameter-frame_000.png",PIXEL * 64,PIXEL * 64,1 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_parameter_00関数 - UI:パラメーター[00] の初期化処理 -
//========================================
void InitUi_parameter_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	LoadPolygon2DTexture(pDevice, g_aTextureUi_parameter_00, g_aUi_parameter_00Polygon, UI_PARAMETER_00_POLYGON_MAX);
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * GetPolygon2DNum(g_aUi_parameter_00Polygon, UI_PARAMETER_00_POLYGON_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_parameter_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_parameter_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// ポリゴン毎の情報
						= &g_aUi_parameter_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPos2D(pVtx,
				UI_PARAMETER_00_POS,
				INITD3DXVECTOR3,
				false,
				pPoly->fWidth,
				pPoly->fHeight,
				ANGLE_TYPE_FIXED);

			// rhwの設定
			SetRHW2D(pVtx);

			switch (nCntPoly)
			{
			case/*/ フレーム /*/ UI_PARAMETER_00_POLYGON_FRAME:
				// テクスチャ座標の設定
				SetTexturePos2D(pVtx, 0, 1, 1, NULL);

				// 頂点カラーの設定
				SetVertexColor2D(pVtx, INITCOLOR);

				break;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_parameter_00->Unlock();
}

//========================================
// UninitUi_parameter_00関数 - UI:パラメーター[00] の終了処理 -
//========================================
void UninitUi_parameter_00(void)
{
	// テクスチャの破棄
	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		if (g_aTextureUi_parameter_00[nCntPoly] != NULL)
		{
			g_aTextureUi_parameter_00[nCntPoly]->Release();
			g_aTextureUi_parameter_00[nCntPoly] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_parameter_00 != NULL) 
	{
		g_pVtxBuffUi_parameter_00->Release();
		g_pVtxBuffUi_parameter_00 = NULL;
	}
}

//========================================
// UpdateUi_parameter_00関数 - UI:パラメーター[00] の更新処理 -
//========================================
void UpdateUi_parameter_00(void)
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_parameter_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly		// ポリゴン毎の情報
						= &g_aUi_parameter_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			switch (nCntPoly)
			{
			case/*/ フレーム /*/ UI_PARAMETER_00_POLYGON_FRAME:

				break;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_parameter_00->Unlock();

	for (int nCntTxt = 0; nCntTxt < UI_PARAMETER_00_LV_TEXT_MAX; nCntTxt++) 
	{
		int nLv = 0;	// LV

		switch (nCntTxt)
		{
		case /*/ 剣ATK /*/UI_PARAMETER_00_LV_TEXT_SWORD_ATK:
			nLv = GetChr_player_00()->nSwordAtkUpCount; break;
		case /*/ 弾ATK /*/UI_PARAMETER_00_LV_TEXT_BULLET_ATK:
			nLv = GetChr_player_00()->nBulletAtkUpCount; break;
		case /*/ プレイヤーHP /*/UI_PARAMETER_00_LV_TEXT_PLAYER_HP:
			nLv = GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS].nCntPriceUp; break;
		case /*/ ユニットHP /*/UI_PARAMETER_00_LV_TEXT_UNIT_HP:
			nLv = GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp; break;
		}

		char aString[TXT_MAX];	// 文字列を設定
		sprintf(aString, "LV%2d", nLv + 1);

		D3DXVECTOR3 setPos	// 設定位置を設定
			= UI_PARAMETER_00_POS + UI_PARAMETER_00_LV_TEXT_RELATIVE_POS;
		setPos.y += UI_PARAMETER_00_LV_TEXT_SPACE_Y * nCntTxt;
		SetText2D(
			aString,
			FONT_000,
			DISPLAY_RIGHT,
			setPos,
			UI_PARAMETER_00_LV_TEXT_COLOR, 1.0f, 1.0f);
	}
}

//========================================
// DrawUi_parameter_00関数 - UI:パラメーター[00] の描画処理 -
//========================================
void DrawUi_parameter_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_parameter_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// ポリゴン毎の情報
			= &g_aUi_parameter_00Polygon[nCntPoly];

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_parameter_00[nCntPoly]);

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++)
		{
			// UI:パラメーター[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (GetPolygon2DNum(g_aUi_parameter_00Polygon, nCntPoly) + nCntPolyIdx), 2);
		}
	}
}