//========================================
// 
// 警告[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_warning_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_warning_00.h"

//****************************************
// マクロ定義
//****************************************

// 警告[00] の出現にかかる時間
// 警告[00] の出現する時間
// 警告[00] の消えるのにかかる時間
// 警告[00] の透明度の最大値
#define UI_WARNING_00_IN_POP_TIME		(40)
#define UI_WARNING_00_POP_TIME			(80)
#define UI_WARNING_00_IN_CLEAR_TIME		(40)
#define UI_WARNING_00_ALPHA_MAX			(0.8f)

// 警告[00] の文字の位置
#define UI_WARNING_00_TEXT_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))

// 警告[00] の帯上の基準位置
// 警告[00] の帯下の基準位置
// 警告[00] の帯の配置間隔X
// 警告[00] の帯の移動にかかる時間
#define UI_WARNING_00_BELT_TOP_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - PIXEL * 24 * 2, 0.0f))
#define UI_WARNING_00_BELT_BOTTOM_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + PIXEL * 24 * 2, 0.0f))
#define UI_WARNING_00_BELT_SPACE_X		(PIXEL * 24 * 2)
#define UI_WARNING_00_MOVE_TIME			(12)

//****************************************
// 列挙型の定義
//****************************************

// 警告[00] の部品番号
typedef enum
{
	UI_WARNING_00_PARTS_TEXT = 0,		// 文字
	UI_WARNING_00_PARTS_BELT_TOP,		// 帯上
	UI_WARNING_00_PARTS_BELT_BOTTOM,	// 帯下
	UI_WARNING_00_PARTS_MAX,
}UI_WARNING_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// 警告[00] の情報構造体
typedef struct 
{
	UI_WARNING_00_STATE state;				// 状態
	int					nCounterState;		// 状態カウンター
	int					nCounterBeltMove;	// 帯の移動カウンター
	float				fAlpha;				// 全体の透明度
}Ui_warning_00;

// 警告[00] のポリゴン毎の情報構造体
typedef struct
{
	char	aTexturePath	// テクスチャファイルの相対パス
			[TXT_MAX];		// 
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	int		nPolygonNum;	// 必要ポリゴン数
}Ui_warning_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterUi_warning_00(void);

// 部品(文字)の更新処理
void UpdateUi_warning_00Parts_text(void);

// 部品(帯上)の更新処理
void UpdateUi_warning_00Parts_belt_top(void);

// 部品(帯下)の更新処理
void UpdateUi_warning_00Parts_belt_bottom(void);

// 状態処理
void StateProcessUi_warning_00(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_warning_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_warning_00		// テクスチャへのポインタ
						[UI_WARNING_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_warning_00		// 頂点バッファへのポインタ
						= NULL;						//
Ui_warning_00			g_ui_warning_00;			// 警告[00] の情報

// 警告[00] の部品毎の情報
Ui_warning_00Parts g_aUi_warning_00Parts[UI_WARNING_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_warning_000_text.png", PIXEL * 128 * 2, PIXEL * 32 * 2, 1  },
	{ "data\\TEXTURE\\ui_warning_000_belt.png", PIXEL * 32  * 2, PIXEL * 8  * 2, 16 },
	{ "data\\TEXTURE\\ui_warning_000_belt.png", PIXEL * 32  * 2, PIXEL * 8  * 2, 16 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_warning_00Parts_text関数 - 部品(文字)の更新処理 -
//========================================
void UpdateUi_warning_00Parts_text(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_TEXT));

	for (int nCntPolygon = 0; nCntPolygon < g_aUi_warning_00Parts[UI_WARNING_00_PARTS_TEXT].nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_WARNING_00_TEXT_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_warning_00Parts[UI_WARNING_00_PARTS_TEXT].fWidth,
			g_aUi_warning_00Parts[UI_WARNING_00_PARTS_TEXT].fHeight,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_warning_00.fAlpha) });
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_warning_00->Unlock();
}

//========================================
// UpdateUi_warning_00Parts_belt_top関数 - 部品(帯上)の更新処理 -
//========================================
void UpdateUi_warning_00Parts_belt_top(void)
{
	Ui_warning_00Parts	*pParts							// 部品の情報のポインタ 
						= &g_aUi_warning_00Parts		//
						[UI_WARNING_00_PARTS_BELT_TOP];	//
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_BELT_TOP));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			D3DXVECTOR3(
			(UI_WARNING_00_BELT_TOP_POS.x - ((UI_WARNING_00_BELT_SPACE_X * pParts->nPolygonNum) * 0.5f))
				+ (UI_WARNING_00_BELT_SPACE_X * nCntPolygon)
				- (((float)g_ui_warning_00.nCounterBeltMove / (float)UI_WARNING_00_MOVE_TIME) * UI_WARNING_00_BELT_SPACE_X),
				UI_WARNING_00_BELT_TOP_POS.y,
				0.0f),
			D3DXVECTOR3(0.0f,0.0f,D3DX_PI),
			false,
			pParts->fWidth,
			pParts->fHeight,
			ANGLE_TYPE_FREE);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_warning_00.fAlpha) });
	}
}

//========================================
// UpdateUi_warning_00Parts_belt_bottom関数 - 部品(帯下)の更新処理 -
//========================================
void UpdateUi_warning_00Parts_belt_bottom(void)
{
	Ui_warning_00Parts	*pParts								// 部品の情報のポインタ 
						= &g_aUi_warning_00Parts			//
						[UI_WARNING_00_PARTS_BELT_BOTTOM];	//
	VERTEX_2D			*pVtx;								// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_BELT_BOTTOM));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			D3DXVECTOR3(
			(UI_WARNING_00_BELT_BOTTOM_POS.x + ((UI_WARNING_00_BELT_SPACE_X * pParts->nPolygonNum) * 0.5f))
				- (UI_WARNING_00_BELT_SPACE_X * nCntPolygon)
				+ (((float)g_ui_warning_00.nCounterBeltMove / (float)UI_WARNING_00_MOVE_TIME) * UI_WARNING_00_BELT_SPACE_X),
				UI_WARNING_00_BELT_BOTTOM_POS.y,
				0.0f),
			D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
			false,
			pParts->fWidth,
			pParts->fHeight,
			ANGLE_TYPE_FREE);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_warning_00.fAlpha) });
	}
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcessUi_warning_00(void)
{
	Ui_warning_00	*pUi	// 警告[00] の情報のポインタ 
					= &g_ui_warning_00;

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_WARNING_00_STATE_POP:

		// 状態カウンターを加算
		pUi->nCounterState++;

		if (pUi->nCounterState >= UI_WARNING_00_POP_TIME)
		{// 状態カウンターが出現する時間に達した時、
			// 消え中状態にする
			SetStateUi_warning_00(UI_WARNING_00_STATE_IN_CLEAR);
		}
		
		break;
		//========== *** 出現中 ***
	case UI_WARNING_00_STATE_IN_POP:

		// 状態カウンターを加算
		pUi->nCounterState++;

		// 透明度を更新
		pUi->fAlpha = UI_WARNING_00_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_WARNING_00_IN_POP_TIME);

		if (pUi->nCounterState >= UI_WARNING_00_IN_POP_TIME) 
		{// 状態カウンターが出現にかかる時間に達した時、
			// 出現状態にする
			SetStateUi_warning_00(UI_WARNING_00_STATE_POP);

			// 透明度を更新
			pUi->fAlpha = UI_WARNING_00_ALPHA_MAX;
		}
		
		break;
		//========== *** 消えている ***
	case UI_WARNING_00_STATE_CLEAR:
		
		break;
		//========== *** 消え中 ***
	case UI_WARNING_00_STATE_IN_CLEAR:

		// 状態カウンターを加算
		pUi->nCounterState++;

		// 透明度を更新
		pUi->fAlpha = UI_WARNING_00_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_WARNING_00_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_WARNING_00_IN_CLEAR_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateUi_warning_00(UI_WARNING_00_STATE_CLEAR);

			// ゲームの状態を通常に設定
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}

		break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_warning_00(int nParts) 
{
	int nPolygonNum	// 必要ポリゴン数
		= 0;		//

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_warning_00Parts[nCntPolygon].nPolygonNum;
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
// InitParameterUi_warning_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterUi_warning_00(void)
{
	g_ui_warning_00.state				= UI_WARNING_00_STATE_CLEAR;	// 状態
	g_ui_warning_00.nCounterState		= 0;							// 状態カウンター
	g_ui_warning_00.nCounterBeltMove	= 0;							// 帯の移動カウンター
	g_ui_warning_00.fAlpha				= 0.0f;							// 全体の透明度
}

//========================================
// InitUi_warning_00関数 - 警告[00] の初期化処理 -
//========================================
void InitUi_warning_00(void)
{
	// パラメーターの初期化処理
	InitParameterUi_warning_00();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_warning_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aUi_warning_00Parts[nCntParts].aTexturePath, &g_aTextureUi_warning_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_warning_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_warning_00Parts[nCntParts].fWidth,
				g_aUi_warning_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx, 0, 1, 1, NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_warning_00->Unlock();
}

//========================================
// UninitUi_warning_00関数 - 警告[00] の終了処理 -
//=====================================d===
void UninitUi_warning_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_warning_00[nCntParts] != NULL)
		{
			g_aTextureUi_warning_00[nCntParts]->Release();
			g_aTextureUi_warning_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_warning_00 != NULL) 
	{
		g_pVtxBuffUi_warning_00->Release();
		g_pVtxBuffUi_warning_00 = NULL;
	}
}

//========================================
// UpdateUi_warning_00関数 - 警告[00] の更新処理 -
//========================================
void UpdateUi_warning_00(void) 
{
	Ui_warning_00	*pUi	// 警告[00] の情報のポインタ 
					= &g_ui_warning_00;

	// 状態処理
	StateProcessUi_warning_00();

	if (pUi->state == UI_WARNING_00_STATE_CLEAR) 
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	// 帯の移動カウンターを更新
	pUi->nCounterBeltMove = ++pUi->nCounterBeltMove % UI_WARNING_00_MOVE_TIME;

	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++) 
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** 文字 ***
		case UI_WARNING_00_PARTS_TEXT:

			// 部品(文字)の更新処理
			UpdateUi_warning_00Parts_text();

			break;
			//========== *** 帯上 ***
		case UI_WARNING_00_PARTS_BELT_TOP:

			// 部品(帯上)の更新処理
			UpdateUi_warning_00Parts_belt_top();

			break;
			//========== *** 帯下 ***
		case UI_WARNING_00_PARTS_BELT_BOTTOM:

			// 部品(帯下)の更新処理
			UpdateUi_warning_00Parts_belt_bottom();

			break;
		}
	}
}

//========================================
// DrawUi_warning_00関数 - 警告[00] の描画処理 -
//========================================
void DrawUi_warning_00(void)
{
	Ui_warning_00	*pUi	// 警告[00] の情報のポインタ 
					= &g_ui_warning_00;

	if (pUi->state == UI_WARNING_00_STATE_CLEAR)
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_warning_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	// ポリゴン番号
		= 0;

	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_warning_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_warning_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// 警告[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState関数 - 警告[00] の状態設定処理 -
//========================================
void SetStateUi_warning_00(UI_WARNING_00_STATE state) 
{
	Ui_warning_00	*pUi	// 警告[00] の情報のポインタ 
					= &g_ui_warning_00;

	// 状態を代入
	pUi->state = state;

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}