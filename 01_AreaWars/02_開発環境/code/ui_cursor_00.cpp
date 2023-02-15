//========================================
// 
// カーソル[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_cursor_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "setting.h"
#include "polygon.h"
#include "md_title_00.h"
#include "ui_cursor_00.h"

//****************************************
// マクロの定義
//****************************************

// カーソル[00] の非クリック時のパターンNo.
// カーソル[00] のクリック時のパターンNo.
#define UI_CURSOR_00_NOT_CLICK_PATTERN	(0)
#define UI_CURSOR_00_CLICK_PATTERN		(1)

//****************************************
// 列挙型の定義
//****************************************

// カーソル[00] の部品番号
typedef enum
{
	UI_CURSOR_00_PARTS_CURSOR = 0,	// カーソル
	UI_CURSOR_00_PARTS_MAX,
}UI_CURSOR_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// カーソル[00] のポリゴン毎の情報構造体
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

	int				nPtn;			// パターンNo.
	int				nCounterAnim;	// アニメーションのカウンター
}Ui_cursor_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// 部品(カーソル)の更新処理
void UpdateUi_cursor_00Parts_cursor(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_cursor_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_cursor_00		// テクスチャへのポインタ
						[UI_CURSOR_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_cursor_00		// 頂点バッファへのポインタ
						= NULL;						//
Ui_cursor_00			g_ui_cursor_00;				// カーソル[00] の情報

// カーソル[00] の部品毎の情報
Ui_cursor_00Parts g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_cursor_000.png" , 2 , 1 , 0 , 0 , -1 , PIXEL * 16 , PIXEL * 16 , 1 },
};

//========== *** カーソル[00] 情報を取得 ***
Ui_cursor_00 *GetUi_cursor_00(void)
{
	return &g_ui_cursor_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_cursor_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterUi_cursor_00(void) 
{
	
}

//========================================
// UpdateUi_cursor_00Parts_cursor関数 - 部品(カーソル)の更新処理 -
//========================================
void UpdateUi_cursor_00Parts_cursor(void)
{
	Ui_cursor_00Parts	*pParts							// 
						= &g_aUi_cursor_00Parts			//
						[UI_CURSOR_00_PARTS_CURSOR];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_cursor_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_cursor_00(UI_CURSOR_00_PARTS_CURSOR));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			GetCursorPosOnScreen(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].fWidth,
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].fHeight,
			ANGLE_TYPE_FIXED);

		if ((GetMousePress(MOUSE_LEFT))
			||
			(GetMousePress(MOUSE_RIGHT))) 
		{// クリック時、
			// パターンNo.をクリック時のものにする
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtn = UI_CURSOR_00_CLICK_PATTERN;
		}
		else 
		{// 非クリック時、
			// パターンNo.を非クリック時のものにする
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtn = UI_CURSOR_00_NOT_CLICK_PATTERN;
		}

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtn,
				g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMin,
				g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMax,
				&g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nCounterAnim,
				g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMaxX,
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMaxY,
			NULL);
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_cursor_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// 必要ポリゴン数

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_cursor_00Parts[nCntPolygon].nPolygonNum;
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
// InitUi_cursor_00関数 - カーソル[00] の初期化処理 -
//========================================
void InitUi_cursor_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_cursor_00(UI_CURSOR_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_cursor_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_cursor_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aUi_cursor_00Parts[nCntParts].aTexturePath, &g_aTextureUi_cursor_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_cursor_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_cursor_00Parts[nCntParts].fWidth,
				g_aUi_cursor_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FIXED);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx,
				0,
				g_aUi_cursor_00Parts[nCntParts].nPtnMaxX,
				g_aUi_cursor_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_cursor_00->Unlock();
}

//========================================
// UninitUi_cursor_00関数 - カーソル[00] の終了処理 -
//========================================
void UninitUi_cursor_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_cursor_00[nCntParts] != NULL)
		{
			g_aTextureUi_cursor_00[nCntParts]->Release();
			g_aTextureUi_cursor_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_cursor_00 != NULL)
	{
		g_pVtxBuffUi_cursor_00->Release();
		g_pVtxBuffUi_cursor_00 = NULL;
	}
}

//========================================
// UpdateUi_cursor_00関数 - カーソル[00] の更新処理 -
//========================================
void UpdateUi_cursor_00(void)
{
	Ui_cursor_00	*pUi				//
					= &g_ui_cursor_00;	// カーソル[00] の情報のポインタ

	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
		case UI_CURSOR_00_PARTS_CURSOR:UpdateUi_cursor_00Parts_cursor(); break; // 部品(カーソル)の更新処理
		}
	}
}

//========================================
// DrawUi_cursor_00関数 - カーソル[00] の描画処理 -
//========================================
void DrawUi_cursor_00(void)
{
	if ((GetActiveInputType() != ACTIVE_INPUT_TYPE_KEYBOARD) ||
		(!GetSetting()->bFullScreen)) 
	{// 動的な入力の種類がキーボードで無い or 全画面フラグが真の時、
		// 処理を終了
		return;
	}

	Ui_cursor_00	*pUi				//
					= &g_ui_cursor_00;	// カーソル[00] の情報のポインタ

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_cursor_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_cursor_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_cursor_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// カーソル[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}