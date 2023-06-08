//========================================
// 
// UI:注意テキスト[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_warning-text_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include "ui_warning-text_00.h"	// UI :注意テキスト	[00]

//****************************************
// マクロ定義
//****************************************
// UI:注意テキスト[00] の位置
// UI:注意テキスト[00] のテキストの相対位置
#define UI_WARNINGTEXT_00_POS				(D3DXVECTOR3((BUFFER_WIDTH*0.5f),PIXEL*64,0.0f))
#define UI_WARNINGTEXT_00_TEXT_RELATIVE_POS	(D3DXVECTOR3(0,PIXEL*-8,0.0f))

// UI:注意テキスト[00] の点滅にかかる時間
#define UI_WARNINGTEXT_00_BRINK_TIME		(60)

// UI:注意テキスト[00] の影の拡大上限
#define UI_WARNINGTEXT_00_SHADOW_SCALE_MAX	(1.1f)

//****************************************
// 列挙型の定義
//****************************************
// UI:注意テキスト[00] の部品番号
typedef enum
{
	UI_WARNINGTEXT_00_PARTS_CURTAIN,		// カーテン
	UI_WARNINGTEXT_00_PARTS_MARK,			// マーク
	UI_WARNINGTEXT_00_PARTS_SHADOW,			// 影
	UI_WARNINGTEXT_00_PARTS_MAX,
}UI_WARNINGTEXT_00_PARTS;

// UI:注意テキスト[00] のテクスチャ番号
typedef enum 
{
	UI_WARNINGTEXT_00_TEXTURE_CURTAIN,	// カーテン
	UI_WARNINGTEXT_00_TEXTURE_NORMAL,	// 通常
	UI_WARNINGTEXT_00_TEXTURE_WARNING,	// 注意
	UI_WARNINGTEXT_00_TEXTURE_MAX,
}UI_WARNINGTEXT_00_TEXTURE;

//****************************************
// 構造体の定義
//****************************************
// UI:注意テキスト[00] の部品毎の情報構造体
typedef struct
{
	float	fWidth;		// 幅
	float	fHeight;	// 高さ
}Ui_warningText_00Parts;

// UI:注意テキスト[00] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath[TXT_MAX];	// テクスチャファイルの相対パス
	int		nPtnWidth;				// パターン幅
	int		nPtnHeight;				// パターン高さ
}Ui_warningText_00Texture;

// UI:注意テキスト[00] の管理情報
typedef struct
{
	int	nCounterBlink;	// 点滅カウンター
}Ui_warningText_00Control;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:注意テキスト[00] の管理情報のパラメーター初期化処理
void InitParameterUi_warningText_00Control(void);

// UI:注意テキスト[00] の部品毎の頂点座標設定処理
void UpdatePartsUi_warningText_00(VERTEX_2D *pVtx, UI_WARNINGTEXT_00_PARTS parts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9			g_aTextureUi_warningText_00	// テクスチャへのポインタ
							[UI_WARNINGTEXT_00_TEXTURE_MAX]
							= {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffUi_warningText_00	// 頂点バッファへのポインタ
							= NULL;
Ui_warningText_00Control	g_ui_warningTextControl;	// UI:注意テキスト[00] の管理情報

// UI:注意テキスト[00] のテクスチャ毎の情報
const char c_aUi_warningText_00TexturePath[UI_WARNINGTEXT_00_TEXTURE_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_curtain_000.png",			// カーテン
	"data\\TEXTURE\\UserInterface\\ui_monster-mark_000.png",	// マーク
	"data\\TEXTURE\\UserInterface\\ui_boss-mark_000.png",		// 注意
};

// UI:注意テキスト[00] の部品毎の情報
const Ui_warningText_00Parts c_aUi_warningText_00Parts[UI_WARNINGTEXT_00_PARTS_MAX] =
{
	{ PIXEL * 64, PIXEL * 16 },	// カーテン
	{ PIXEL * 16, PIXEL * 16 },	// 注意テキスト
	{ PIXEL * 16, PIXEL * 16 },	// 影
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_warningText_00Control関数 - UI:注意テキスト[00] の管理情報のパラメーター初期化処理 -
//========================================
void InitParameterUi_warningText_00Control(void) 
{
	Ui_warningText_00Control	*pUiCtl	// UI:注意テキスト[00] の管理情報のポインタ
								= &g_ui_warningTextControl;

	pUiCtl->nCounterBlink = 0;	// 点滅カウンター
}

//========================================
// SetVertexPositionUi_warningText_00関数 - UI:注意テキスト[00] の部品毎の頂点座標設定処理 -
//========================================
void UpdatePartsUi_warningText_00(VERTEX_2D *pVtx, UI_WARNINGTEXT_00_PARTS parts) 
{
	Ui_warningText_00Control	*pUiCtl	// UI:注意テキスト[00] の管理情報のポインタ
								= &g_ui_warningTextControl;

	switch (parts)
	{
	case /*/ マーク /*/UI_WARNINGTEXT_00_PARTS_MARK: {
		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_WARNINGTEXT_00_POS + UI_WARNINGTEXT_00_TEXT_RELATIVE_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			c_aUi_warningText_00Parts[parts].fWidth,
			c_aUi_warningText_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);

		break;
	}
	case /*/ 影 /*/UI_WARNINGTEXT_00_PARTS_SHADOW: {
		float	fRate	= (float)pUiCtl->nCounterBlink / (float)UI_WARNINGTEXT_00_BRINK_TIME;
		float	fScale	= (1.0f + ((UI_WARNINGTEXT_00_SHADOW_SCALE_MAX - 1.0f) * fRate));

		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_WARNINGTEXT_00_POS + UI_WARNINGTEXT_00_TEXT_RELATIVE_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			c_aUi_warningText_00Parts[parts].fWidth * fScale,
			c_aUi_warningText_00Parts[parts].fHeight * fScale,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * (1.0f - fRate)) });

		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_warningText_00関数 - UI:注意テキスト[00] の初期化処理 -
//========================================
void InitUi_warningText_00(void)
{
	// 管理情報のパラメーターの初期化処理
	InitParameterUi_warningText_00Control();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_WARNINGTEXT_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_warningText_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_warningText_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < UI_WARNINGTEXT_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, c_aUi_warningText_00TexturePath[nCntTexture], &g_aTextureUi_warningText_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_WARNINGTEXT_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			UI_WARNINGTEXT_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			c_aUi_warningText_00Parts[nCntParts].fWidth,
			c_aUi_warningText_00Parts[nCntParts].fHeight,
			ANGLE_TYPE_FIXED);

		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			0,
			1,
			1,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_warningText_00->Unlock();
}

//========================================
// UninitUi_warningText_00関数 - UI:注意テキスト[00] の終了処理 -
//========================================
void UninitUi_warningText_00(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < UI_WARNINGTEXT_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_warningText_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_warningText_00[nCntTexture]->Release();
			g_aTextureUi_warningText_00[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_warningText_00 != NULL) 
	{
		g_pVtxBuffUi_warningText_00->Release();
		g_pVtxBuffUi_warningText_00 = NULL;
	}
}

//========================================
// UpdateUi_warningText_00関数 - UI:注意テキスト[00] の更新処理 -
//========================================
void UpdateUi_warningText_00(void) 
{
	Ui_warningText_00Control	*pUiCtl	// UI:注意テキスト[00] の管理情報のポインタ
								= &g_ui_warningTextControl;

	// 点滅カウンターにOBJ:ステージのカウンターを代入制御
	pUiCtl->nCounterBlink = GetObj_stage_00()->nCounter % UI_WARNINGTEXT_00_BRINK_TIME;

	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_warningText_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_WARNINGTEXT_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 部品毎の更新処理
		UpdatePartsUi_warningText_00(pVtx, (UI_WARNINGTEXT_00_PARTS)nCntParts);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_warningText_00->Unlock();
}

//========================================
// DrawUi_warningText_00関数 - UI:注意テキスト[00] の描画処理 -
//========================================
void DrawUi_warningText_00(void)
{
	if (GetObj_stage_00()->state != OBJ_STAGE_00_STATE_INTERVAL)
	{// OBJ:ステージ[00] の状態がインターバルでない時、
		return;	// 処理を終了する
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_warningText_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_WARNINGTEXT_00_PARTS_MAX; nCntParts++)
	{
		UI_WARNINGTEXT_00_TEXTURE tex;

		if (nCntParts != UI_WARNINGTEXT_00_PARTS_CURTAIN) 
		{// カウントの部品がカーテンでない時、
			tex = !GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
				UI_WARNINGTEXT_00_TEXTURE_NORMAL : UI_WARNINGTEXT_00_TEXTURE_WARNING;
		}
		else 
		{// カウントの部品がカーテンの時、
			tex = UI_WARNINGTEXT_00_TEXTURE_CURTAIN;
		}

		// テクスチャの設定
		pDevice->SetTexture(0,g_aTextureUi_warningText_00[tex]);

		// UI:注意テキスト[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}