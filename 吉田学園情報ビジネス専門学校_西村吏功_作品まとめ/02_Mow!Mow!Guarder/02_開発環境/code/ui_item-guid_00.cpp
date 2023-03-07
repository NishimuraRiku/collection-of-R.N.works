//========================================
// 
// UI:商品ガイド[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_item-guid_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "item.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include "ui_item-guid_00.h"	// UI :商品ガイド	[00]

//****************************************
// マクロ定義
//****************************************
// UI:商品ガイド[00] の位置
#define UI_ITEMGUID_00_POS	(D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*64),(BUFFER_HEIGHT*0.5f),0.0f))

//****************************************
// 列挙型の定義
//****************************************
// UI:商品ガイド[00] の部品番号
typedef enum
{
	UI_ITEMGUID_00_PARTS_ITEM_GUID,	// 商品説明
	UI_ITEMGUID_00_PARTS_MAX,
}UI_ITEMGUID_00_PARTS;

//****************************************
// 構造体の定義
//****************************************
// UI:商品ガイド[00] の部品毎の情報構造体
typedef struct
{
	float	fWidth;		// 幅
	float	fHeight;	// 高さ
}Ui_itemGuid_00Parts;

// UI:商品ガイド[00] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath	// テクスチャファイルの相対パス
			[TXT_MAX];
}Ui_itemGuid_00Texture;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:商品ガイド[00] のパラメーター初期化処理
void InitParameterUi_itemGuid_00(void);

// UI:商品ガイド[00] の部品毎の頂点座標設定処理
void UpdatePartsUi_itemGuid_00(VERTEX_2D *pVtx, UI_ITEMGUID_00_PARTS parts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_itemGuid_00	// テクスチャへのポインタ
						[OBJ_PEDESTAL_00_ITEM_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_itemGuid_00	// 頂点バッファへのポインタ
						= NULL;
Ui_itemGuid_00			g_ui_itemGuid_00;			// UI:商品説明[00] の情報

// UI:商品ガイド[00] のテクスチャ毎の情報
const Ui_itemGuid_00Texture g_aUi_itemGuid_00Texture[OBJ_PEDESTAL_00_ITEM_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\block.png" },			// ブロック
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\turret.png" },			// タレット
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\reflector.png" },		// 反射装置
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\discharger.png" },		// 放電装置
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\repair-you.png" },		// プレイヤーのHPを回復
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\repair-unit.png" },		// ユニットのHPを回復
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\slash-atk-up.png" },		// 剣の攻撃力アップ
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\bullet-atk-up.png" },	// 弾の攻撃力アップ
};

// UI:商品ガイド[00] の部品毎の情報
const Ui_itemGuid_00Parts g_aUi_itemGuid_00Parts[UI_ITEMGUID_00_PARTS_MAX] =
{
	{ PIXEL * 128, PIXEL * 64 },	// 商品説明
};

//========== *** UI:商品ガイド[00] の情報を取得 ***
Ui_itemGuid_00 *GetUi_itemGuid_00(void) 
{
	return &g_ui_itemGuid_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_itemGuid_00関数 - UI:商品ガイド[00] のパラメーター初期化処理 -
//========================================
void InitParameterUi_itemGuid_00(void) 
{
	Ui_itemGuid_00	*pUi	// UI:商品ガイド[00] の情報のポインタ
					= &g_ui_itemGuid_00;
	
	pUi->nTex				= 0;	// テクスチャ番号
	pUi->nCounterArrival	= 0;	// 到着カウンター
}

//========================================
// SetVertexPositionUi_itemGuid_00関数 - UI:商品ガイド[00] の部品毎の頂点座標設定処理 -
//========================================
void UpdatePartsUi_itemGuid_00(VERTEX_2D *pVtx, UI_ITEMGUID_00_PARTS parts) 
{
	Ui_itemGuid_00	*pUi	// UI:商品ガイド[00] の情報のポインタ
					= &g_ui_itemGuid_00;

	switch (parts)
	{
	case /*/ 商品説明 /*/UI_ITEMGUID_00_PARTS_ITEM_GUID: {
		float	fRate	// カウンターの進行率
				= (float)pUi->nCounterArrival / (float)UI_ITEMGUID_00_ARRIVAL_TIME;

		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_ITEMGUID_00_POS + D3DXVECTOR3(0.0f, (UI_ITEMGUID_00_NON_ARRIVAL_RELATIVE_POS_Y * fRate), 0),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_itemGuid_00Parts[parts].fWidth,
			g_aUi_itemGuid_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * fRate) });
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
// InitUi_itemGuid_00関数 - UI:商品ガイド[00] の初期化処理 -
//========================================
void InitUi_itemGuid_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_ITEMGUID_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_itemGuid_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_itemGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < OBJ_PEDESTAL_00_ITEM_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_itemGuid_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_itemGuid_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_ITEMGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_ITEMGUID_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_itemGuid_00Parts[nCntParts].fWidth,
			g_aUi_itemGuid_00Parts[nCntParts].fHeight,
			ANGLE_TYPE_FIXED);

		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx, 0, 1, 1, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_itemGuid_00->Unlock();

	// パラメーターの初期化処理
	InitParameterUi_itemGuid_00();
}

//========================================
// UninitUi_itemGuid_00関数 - UI:商品ガイド[00] の終了処理 -
//========================================
void UninitUi_itemGuid_00(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < UI_ITEMGUID_00_PARTS_MAX; nCntTexture++)
	{
		if (g_aTextureUi_itemGuid_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_itemGuid_00[nCntTexture]->Release();
			g_aTextureUi_itemGuid_00[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_itemGuid_00 != NULL) 
	{
		g_pVtxBuffUi_itemGuid_00->Release();
		g_pVtxBuffUi_itemGuid_00 = NULL;
	}
}

//========================================
// UpdateUi_itemGuid_00関数 - UI:商品ガイド[00] の更新処理 -
//========================================
void UpdateUi_itemGuid_00(void) 
{
	Ui_itemGuid_00	*pUi	// UI:商品ガイド[00] の情報のポインタ
					= &g_ui_itemGuid_00;

	if (GetChr_player_00()->nPedestalIndex != -1)
	{// CHR:プレイヤー[00] の台座の番号が-1でない時、
		pUi->nCounterArrival++;	// 到着カウンターを加算
		pUi->nTex				// テクスチャ番号を設定
			= GetObj_stage_00Type()[GetObj_stage_00()->nType].itemsSet.aItems[GetObj_pedestal_00Control()->nCntItems].aItem[GetChr_player_00()->nPedestalIndex];
	}
	else
	{// CHR:プレイヤー[00] の台座の番号が-1の時、
		pUi->nCounterArrival--;	// 到着カウンターを減算
	}

	// 到着カウンターを制御
	IntControl(&pUi->nCounterArrival, UI_ITEMGUID_00_ARRIVAL_TIME, 0);

	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_itemGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_ITEMGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 部品毎の更新処理
		UpdatePartsUi_itemGuid_00(pVtx, (UI_ITEMGUID_00_PARTS)nCntParts);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_itemGuid_00->Unlock();
}

//========================================
// DrawUi_itemGuid_00関数 - UI:商品ガイド[00] の描画処理 -
//========================================
void DrawUi_itemGuid_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_itemGuid_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Ui_itemGuid_00	*pUi	// UI:商品ガイド[00] の情報のポインタ
					= &g_ui_itemGuid_00;

	for (int nCntParts = 0; nCntParts < UI_ITEMGUID_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_itemGuid_00[pUi->nTex]);

		// UI:商品ガイド[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}