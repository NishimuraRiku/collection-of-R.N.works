//========================================
// 
// 矢印[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_arrow_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "ui_arrow_00.h"

//****************************************
// 列挙型の定義
//****************************************

// 矢印[00] の部品番号
typedef enum
{
	UI_ARROW_00_PARTS_TOP_ARROW = 0,	// 矢印(上)
	UI_ARROW_00_PARTS_BOTTOM_ARROW,		// 矢印(下)
	UI_ARROW_00_PARTS_MAX,
}UI_ARROW_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// 矢印[00] のポリゴン毎の情報構造体
typedef struct
{
	char			aTexturePath	// テクスチャファイルの相対パス
					[TXT_MAX];		// 
	int				nPtnMaxX;		// パターンの上限(横)
	int				nPtnMaxY;		// パターンの上限(縦)
	int				nPtnMin;		// パターンNo.の下限
	int				nPtnMax;		// パターンNo.の上限
	int				nAnimTime;		// アニメーションにかかる時間
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	int				nPolygonNum;	// 必要ポリゴン数
	float			fAngle;			// 向き
	D3DXVECTOR3		relativePos;	// 相対位置

	int				nPtn;			// パターンNo.
	int				nCounterAnim;	// アニメーションのカウンター
}Ui_arrow_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// 部品(矢印(上))の更新処理
void UpdateUi_arrow_00Parts_top_arrow(void);

// 部品(矢印(下))の更新処理
void UpdateUi_arrow_00Parts_bottom_arrow(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_arrow_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_arrow_00	// テクスチャへのポインタ
						[UI_ARROW_00_PARTS_MAX]	//
						= {};					//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_arrow_00	// 頂点バッファへのポインタ
						= NULL;					//
Ui_arrow_00				g_ui_arrow_00;			// 矢印[00] の情報

// 矢印[00] の部品毎の情報
Ui_arrow_00Parts g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_arrow_001_8x4.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 8 , PIXEL * 4 , 1 , D3DX_PI * 1.0f	, D3DXVECTOR3(PIXEL * 0, PIXEL * -7, 0.0f) },
	{ "data\\TEXTURE\\ui_arrow_001_8x4.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 8 , PIXEL * 4 , 1 , 0.0f				, D3DXVECTOR3(PIXEL * 0, PIXEL *  6, 0.0f) },
};

//========== *** 矢印[00] 情報を取得 ***
Ui_arrow_00 *GetUi_arrow_00(void)
{
	return &g_ui_arrow_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_arrow_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterUi_arrow_00(void) 
{
	g_ui_arrow_00.bUse = false;	// 使用されていない状態にする
}

//========================================
// UpdateUi_arrow_00Parts_top_arrow関数 - 部品(矢印(上))の更新処理 -
//========================================
void UpdateUi_arrow_00Parts_top_arrow(void)
{
	Ui_arrow_00Parts	*pParts							// 
						= &g_aUi_arrow_00Parts			//
						[UI_ARROW_00_PARTS_TOP_ARROW];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_arrow_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_arrow_00(UI_ARROW_00_PARTS_TOP_ARROW));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_arrow_00.pos + g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].fAngle),
			false,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].fWidth,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].fHeight,
			ANGLE_TYPE_FREE);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtn,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMin,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMax,
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nCounterAnim,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMaxX,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_arrow_00Parts_bottom_arrow関数 - 部品(矢印(下))の更新処理 -
//========================================
void UpdateUi_arrow_00Parts_bottom_arrow(void)
{
	Ui_arrow_00Parts	*pParts								// 
						= &g_aUi_arrow_00Parts				//
						[UI_ARROW_00_PARTS_BOTTOM_ARROW];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;								// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_arrow_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_arrow_00(UI_ARROW_00_PARTS_BOTTOM_ARROW));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_arrow_00.pos + g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].fAngle),
			false,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].fWidth,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].fHeight,
			ANGLE_TYPE_FREE);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtn,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMin,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMax,
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nCounterAnim,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMaxX,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMaxY,
			NULL);
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_arrow_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// 必要ポリゴン数

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_arrow_00Parts[nCntPolygon].nPolygonNum;
	}

	// 必要ポリゴン数を返す
	return nPolygonNum;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_arrow_00関数 - 矢印[00] の初期化処理 -
//========================================
void InitUi_arrow_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_arrow_00(UI_ARROW_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_arrow_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_arrow_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aUi_arrow_00Parts[nCntParts].aTexturePath, &g_aTextureUi_arrow_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_arrow_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_arrow_00Parts[nCntParts].fWidth,
				g_aUi_arrow_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx,
				0,
				g_aUi_arrow_00Parts[nCntParts].nPtnMaxX,
				g_aUi_arrow_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_arrow_00->Unlock();
}

//========================================
// UninitUi_arrow_00関数 - 矢印[00] の終了処理 -
//========================================
void UninitUi_arrow_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_arrow_00[nCntParts] != NULL)
		{
			g_aTextureUi_arrow_00[nCntParts]->Release();
			g_aTextureUi_arrow_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_arrow_00 != NULL)
	{
		g_pVtxBuffUi_arrow_00->Release();
		g_pVtxBuffUi_arrow_00 = NULL;
	}
}

//========================================
// UpdateUi_arrow_00関数 - 矢印[00] の更新処理 -
//========================================
void UpdateUi_arrow_00(void)
{
	Ui_arrow_00	*pUi				//
				= &g_ui_arrow_00;	// 矢印[00] の情報のポインタ

	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** 矢印(上) ***
		case UI_ARROW_00_PARTS_TOP_ARROW:
		{
			// 部品(矢印(上))の更新処理
			UpdateUi_arrow_00Parts_top_arrow();
		}
		break;
			//========== *** 矢印(下) ***
		case UI_ARROW_00_PARTS_BOTTOM_ARROW:
		{
			// 部品(矢印(下))の更新処理
			UpdateUi_arrow_00Parts_bottom_arrow();
		}
		break;
		}
	}
}

//========================================
// DrawUi_arrow_00関数 - 矢印[00] の描画処理 -
//========================================
void DrawUi_arrow_00(void)
{
	if (!g_ui_arrow_00.bUse) 
	{// 使用されていない状態の時、
		// 処理を折り返す
		return;
	}

	Ui_arrow_00	*pUi				//
				= &g_ui_arrow_00;	// 矢印[00] の情報のポインタ

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_arrow_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_arrow_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_arrow_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// 矢印[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}