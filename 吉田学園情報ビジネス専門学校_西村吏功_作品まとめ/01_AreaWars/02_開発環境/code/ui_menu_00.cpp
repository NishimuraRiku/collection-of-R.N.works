//========================================
// 
// メニュー[00]の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_menu_00.cpp ***
//========================================
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "text.h"
#include "ui_menu_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// マクロ定義
//****************************************
#define UI_MENU_00_MAX						(16)						// メニュー[00] の最大数
#define UI_MENU_00_WIDTH					(PIXEL * 256)				// メニュー[00] の幅
#define UI_MENU_00_HEIGHT					(PIXEL * 16)				// メニュー[00] の高さ
#define UI_MENU_00_TEXTURE_X_PATTERN_MAX	(1)							// メニュー[00] のテクスチャのパターン数(横)
#define UI_MENU_00_TEXTURE_Y_PATTERN_MAX	(1)							// メニュー[00] のテクスチャのパターン数(縦)
#define UI_MENU_00_SELECT_COLOR				(Color{243, 189, 63, 255})	// メニュー[00] の選択時の色

#define UI_MENU_00_POP_TIME					(10)						// メニュー[00] の出現にかかる時間
#define UI_MENU_00_POP_SCALE				(ScaleDiameter{1.0f, 1.0f})	// メニュー[00] の出現している時の大きさにかかる倍率

#define UI_MENU_00_CLEAR_SCALE				(ScaleDiameter{1.0f, 0.0f})	// メニュー[00] の消えている時の大きさにかかる倍率

//****************************************
// 構造体の定義
//****************************************

// メニューの予約情報
typedef struct 
{
	bool bReservation;
	Ui_menu_00_set *set;
	int nMax; 
	float fSpaceX; 
	float fSpaceY;
}MenuReservation;

//****************************************
// プロトタイプ宣言
//****************************************
void StateProcessUi_menu_00(void);	// メニュー[00] の状態処理
void CreateUi_menu_00(void);		// メニュー[00] の作成処理

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_menu_00[UI_MENU_00_TYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_menu_00 = NULL;					// 頂点バッファへのポインタ
Ui_menu_00				g_aUi_menu_00[UI_MENU_00_MAX];					// メニュー[00] の情報
int						g_nUi_menu_00Num;								// メニュー[00] の現在の使用数
int						g_nSelect;										// 現在の選択番号
D3DXVECTOR3				g_centerPos;									// 中心座標
Ui_menu_00Control		g_control;										// 全体管理
MenuReservation			g_menuReservation;								// メニューの予約情報

// メニュー[00] のテクスチャファイルの相対パス
char g_aTexturePath[UI_MENU_00_TYPE_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\ui_menu_000.png",
	"data\\TEXTURE\\ui_menu_000_select.png",
};

//========== *** メニュー[00] の情報を取得 ***
Ui_menu_00 *GetUi_menu_00(void) 
{
	return &g_aUi_menu_00[0];
}

//========== *** メニュー[00] の全体管理の情報を取得 ***
Ui_menu_00Control *GetUi_menu_00Control(void) 
{
	return &g_control;
}

//========== *** メニュー[00] の現在の選択番号を取得 ***
int *GetSelect(void) 
{
	return &g_nSelect;
}

//========================================
// CreateUi_menu_00関数 - メニュー[00] の作成処理 -
//========================================
void CreateUi_menu_00(void)
{
	if ((!g_menuReservation.bReservation)
		|| (g_control.state != UI_MENU_00_STATE_NONE))
	{// メニューの予約が無い、もしくは状態が無しでない時、
		// 処理を終了する
		return;
	}

	// 予約フラグを偽にする
	g_menuReservation.bReservation = false;

	// メニュー[00] を出現中にする
	g_control.state = UI_MENU_00_STATE_IN_POP;
	g_control.nCounterState = UI_MENU_00_POP_TIME;

	// 現在の選択番号を初期化する
	g_nSelect = 0;

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

								// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	// 現在の使用数を代入
	g_nUi_menu_00Num = g_menuReservation.nMax;

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < g_menuReservation.nMax; nCntUi_menu_00++, g_menuReservation.set++, pVtx += 4)
	{
		// 種類を代入
		g_aUi_menu_00[nCntUi_menu_00].type = g_menuReservation.set->type;

		// 文字列を代入
		sprintf(g_aUi_menu_00[nCntUi_menu_00].aString, g_menuReservation.set->aString);

		// 使用されている状態にする
		g_aUi_menu_00[nCntUi_menu_00].bUse = true;

		// 選択されていない状態にする
		g_aUi_menu_00[nCntUi_menu_00].bSelect = false;

		// 位置を設定
		g_aUi_menu_00[nCntUi_menu_00].pos = D3DXVECTOR3(g_centerPos.x - (((g_menuReservation.nMax - 1) * g_menuReservation.fSpaceX) * 0.5f) + (nCntUi_menu_00 * g_menuReservation.fSpaceX),
			g_centerPos.y - (((g_menuReservation.nMax - 1) * g_menuReservation.fSpaceY) * 0.5f) + (nCntUi_menu_00 * g_menuReservation.fSpaceY),
			0.0f);

		{
			D3DXVECTOR3 pos = g_aUi_menu_00[nCntUi_menu_00].pos;						// 位置
			D3DXVECTOR3 rot = g_aUi_menu_00[nCntUi_menu_00].rot;						// 向き
			float fWidth = g_aUi_menu_00[nCntUi_menu_00].fWidth;						// 幅
			float fHeight = g_aUi_menu_00[nCntUi_menu_00].fHeight;					// 高さ
			float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle = atan2f(fWidth, fHeight);									// 対角線の角度

																					// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// InitUi_menu_00関数 - メニュー[00] の初期化処理 -
//========================================
void InitUi_menu_00(void)
{
	// メニューの予約フラグを初期化
	g_menuReservation.bReservation = false;

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	
	// 中心座標を初期化
	g_centerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 現在の使用数を初期化
	g_nUi_menu_00Num = 0;

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntUi_menu_00_type = 0; nCntUi_menu_00_type < UI_MENU_00_TYPE_MAX; nCntUi_menu_00_type++) 
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aTexturePath[nCntUi_menu_00_type], &g_aTextureUi_menu_00[nCntUi_menu_00_type]);
	}
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_MENU_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_menu_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pVtx += 4)
	{
		g_aUi_menu_00[nCntUi_menu_00].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置を初期化
		g_aUi_menu_00[nCntUi_menu_00].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きを初期化
		g_aUi_menu_00[nCntUi_menu_00].fWidth	= UI_MENU_00_WIDTH;					// 幅を初期化
		g_aUi_menu_00[nCntUi_menu_00].fHeight	= UI_MENU_00_HEIGHT;				// 高さを初期化
		g_aUi_menu_00[nCntUi_menu_00].nPattern	= 0;								// パターンNo.を初期化
		g_aUi_menu_00[nCntUi_menu_00].bUse		= false;							// 使用されていない状態にする
		g_aUi_menu_00[nCntUi_menu_00].bSelect	= false;							// 選択されていない状態にする

		{
			D3DXVECTOR3 pos = g_aUi_menu_00[nCntUi_menu_00].pos;						// 位置
			D3DXVECTOR3 rot = g_aUi_menu_00[nCntUi_menu_00].rot;						// 向き
			float fWidth	= g_aUi_menu_00[nCntUi_menu_00].fWidth;						// 幅
			float fHeight	= g_aUi_menu_00[nCntUi_menu_00].fHeight;					// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		{
			int nPtn		= g_aUi_menu_00[nCntUi_menu_00].nPattern;	// テクスチャのパターンNo.
			int nPtnMaxX	= UI_MENU_00_TEXTURE_X_PATTERN_MAX;			// テクスチャのパターン数(横)
			int nPtnMaxY	= UI_MENU_00_TEXTURE_Y_PATTERN_MAX;			// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// UninitUi_menu_00関数 - メニュー[00] の終了処理 -
//========================================
void UninitUi_menu_00(void)
{
	for (int nCntUi_menu_00_type = 0; nCntUi_menu_00_type < UI_MENU_00_TYPE_MAX; nCntUi_menu_00_type++)
	{
		// テクスチャの破棄
		if (g_aTextureUi_menu_00[nCntUi_menu_00_type] != NULL)
		{
			g_aTextureUi_menu_00[nCntUi_menu_00_type]->Release();
			g_aTextureUi_menu_00[nCntUi_menu_00_type] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_menu_00 != NULL) 
	{
		g_pVtxBuffUi_menu_00->Release();
		g_pVtxBuffUi_menu_00 = NULL;
	}
}

//========================================
// UpdateUi_menu_00関数 - メニュー[00] の更新処理 -
//========================================
void UpdateUi_menu_00(void)
{
	// 状態処理
	StateProcessUi_menu_00();

	// 作成処理
	CreateUi_menu_00();

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	Ui_menu_00 *pUi
		= g_aUi_menu_00;		// メニュー[00] の情報のポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pVtx += 4, pUi++)
	{
		if (!pUi->bUse)
		{// 使用されていない時、
			// 繰り返し処理をリセット
			continue;
		}

		{
			float fWidth	= pUi->fWidth * g_control.scale.fWidth;						// 幅
			float fHeight	= pUi->fHeight * g_control.scale.fHeight;					// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

			// 頂点座標を設定
			pVtx[0].pos.x = pUi->pos.x + sinf(pUi->rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pUi->pos.y + cosf(pUi->rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pUi->pos.x + sinf(pUi->rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pUi->pos.y + cosf(pUi->rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pUi->pos.x + sinf(pUi->rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pUi->pos.y + cosf(pUi->rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pUi->pos.x + sinf(pUi->rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pUi->pos.y + cosf(pUi->rot.z + fAngle) * fLength;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		{
			Color col = { 255, 255, 255, 255 };	// 色

			if (nCntUi_menu_00 == g_nSelect) 
			{// カウントが選択番号と一致していた時、
				// 色を選択時の色にする
				col = UI_MENU_00_SELECT_COLOR;
			}

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
			pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
			pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
			pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);

			// テキスト[メニュー] を表示
			ShowTxt_00(TEXT_FIELD_MENU, nCntUi_menu_00, true);

			// テキスト[メニュー] の更新処理
			UpdateTxt_00(TEXT_FIELD_MENU, nCntUi_menu_00, pUi->aString, pUi->pos, col, TEXT_MODE_CENTER, FONT_000, g_control.scale.fWidth, g_control.scale.fHeight);
		}

		{
			int nPtn = pUi->nPattern;	// テクスチャのパターンNo.
			int nPtnMaxX = UI_MENU_00_TEXTURE_X_PATTERN_MAX;	// テクスチャのパターン数(横)
			int nPtnMaxY = UI_MENU_00_TEXTURE_Y_PATTERN_MAX;	// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// StateProcessUi_menu_00関数 - メニュー[00] の状態処理 -
//========================================
void StateProcessUi_menu_00(void)
{
	switch (g_control.state)
	{
		//========== *** 出現中 ***
	case UI_MENU_00_STATE_IN_POP:

		// 状態のカウンターを減算
		g_control.nCounterState--;

		if (g_control.nCounterState <= 0) 
		{// 状態のカウンターが0以下の時、
			// 出現状態にする
			g_control.state = UI_MENU_00_STATE_POP;
		}

		// 大きさにかかる倍率を更新
		g_control.scale.fWidth = (UI_MENU_00_POP_SCALE.fWidth * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME)))
			+ (UI_MENU_00_CLEAR_SCALE.fWidth * ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME));
		g_control.scale.fHeight = (UI_MENU_00_POP_SCALE.fHeight * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME)))
			+ (UI_MENU_00_CLEAR_SCALE.fHeight * ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME));

		break;
		//========== *** 出現 ***
	case UI_MENU_00_STATE_POP:

		break;
		//========== *** 消え中 ***
	case UI_MENU_00_STATE_IN_CLEAR:

		// 状態のカウンターを減算
		g_control.nCounterState--;

		if (g_control.nCounterState <= 0)
		{// 状態のカウンターが0以下の時、
			// 状態を無しにする
			g_control.state = UI_MENU_00_STATE_NONE;

			Ui_menu_00 *pUi
				= g_aUi_menu_00;	// メニュー[00] の情報のポインタ

			for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pUi++)
			{
				// 使用していない状態にする
				pUi->bUse = false;

				// テキスト[メニュー] を非表示
				ShowTxt_00(TEXT_FIELD_MENU, nCntUi_menu_00, false);
			}
		}

		// 大きさにかかる倍率を更新
		g_control.scale.fWidth = (UI_MENU_00_CLEAR_SCALE.fWidth * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME)))
			+ (UI_MENU_00_POP_SCALE.fWidth * ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME));
		g_control.scale.fHeight = (UI_MENU_00_CLEAR_SCALE.fHeight * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME)))
			+ (UI_MENU_00_POP_SCALE.fHeight * ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME));

		break;
	}
}

//========================================
// Ui_menu_00Input関数 - メニュー[00] の入力処理 -
//========================================
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode)
{
	int nSelected = -1;	// 選択した番号(-1は選択無し)

	if ((g_control.state != UI_MENU_00_STATE_POP)
		||(GetFade() != FADE_NONE))
	{// 出現状態でない時、もしくはフェード中の時、
		// 処理を終了する
		return nSelected;
	}

	switch (mode)
	{
	//========== *** 上下 ***
	case UI_MENU_00_INPUT_MODE_UP_AND_DOWN:

		if ((GetKeyboardRepeat(DIK_UP)) 
			|| (GetKeyboardRepeat(DIK_W))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
			|| (GetButtonRepeat(BUTTON_UP)))
		{// 上キーが押された時、
			// 現在の選択番号を減算
			g_nSelect--;

			if (g_menuReservation.nMax != 1)
			{// メニューの数が1で無い時、
			 // 効果音[選択] を再生
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}
		else if ((GetKeyboardRepeat(DIK_DOWN))
			|| (GetKeyboardRepeat(DIK_S))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
			|| (GetButtonRepeat(BUTTON_DOWN)))
		{// 下キーが押された時、
			// 現在の選択番号を加算
			g_nSelect++;

			if (g_menuReservation.nMax != 1)
			{// メニューの数が1で無い時、
			 // 効果音[選択] を再生
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}

		break;
	//========== *** 左右 ***
	case UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT:

		if ((GetKeyboardRepeat(DIK_LEFT)) 
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
		{// 左キーが押された時、
			// 現在の選択番号を減算
			g_nSelect--;

			if (g_menuReservation.nMax != 1)
			{// メニューの数が1で無い時、
				// 効果音[選択] を再生
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}
		else if ((GetKeyboardRepeat(DIK_RIGHT)) 
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// 右キーが押された時、
			// 現在の選択番号を加算
			g_nSelect++;

			if (g_menuReservation.nMax != 1)
			{// メニューの数が1で無い時、
				// 効果音[選択] を再生
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}

		break;
	}

	if (g_nUi_menu_00Num > 0)
	{// 現在使用数が0を上回っている時、
		// 選択番号を制御
		g_nSelect %= g_nUi_menu_00Num;
	}

	if (g_nSelect < 0) 
	{// 現在の選択番号が0を下回っている時

		g_nSelect = g_nUi_menu_00Num - 1;
	}

	if (g_aUi_menu_00[g_nSelect].type == UI_MENU_00_TYPE_SELECTION)
	{// 種類が選択だった時

		if ((GetKeyboardRepeat(DIK_LEFT)) || (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_RIGHT)) || (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// 左キーか右キーが押された時、
			// 選択した番号に現在の選択番号を代入
			nSelected = g_nSelect;

			// 選択音[000] を再生
			PlaySound(SOUND_LABEL_SE_SELECT_000);
		}
	}
	else if ((GetKeyboardTrigger(DIK_RETURN))
		|| (GetButtonTrigger(BUTTON_A)))
	{// エンターキーが押された時、
		// 選択した番号に現在の選択番号を代入
		nSelected = g_nSelect;

		// メニュー[00] を消え中にする
		g_control.state = UI_MENU_00_STATE_IN_CLEAR;
		g_control.nCounterState = UI_MENU_00_CLEAR_TIME;
	}
	
	return nSelected;
}

//========================================
// DrawUi_menu_00関数 - メニュー[00] の描画処理 -
//========================================
void DrawUi_menu_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_menu_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++)
	{
		if (g_aUi_menu_00[nCntUi_menu_00].bUse) 
		{// 使用されている時、
			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureUi_menu_00[g_aUi_menu_00[nCntUi_menu_00].type]);

			// メニュー[00]の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_menu_00 * 4, 2);
		}
	}
}

//========================================
// SetUi_menu_00Pos関数 - メニュー[00] の中心座標を設定 -
//========================================
void SetUi_menu_00Pos(D3DXVECTOR3 pos)
{
	// 中心座標を代入
	g_centerPos = pos;
}

//========================================
// SetUi_menu_00関数 - メニュー[00] の設定処理 -
//========================================
void SetUi_menu_00(Ui_menu_00_set *set, int nMax, float fSpaceX, float fSpaceY)
{
	// 予約フラグを真にする
	g_menuReservation.bReservation = true;

	g_menuReservation.set = set;
	g_menuReservation.nMax = nMax;
	g_menuReservation.fSpaceX = fSpaceX;
	g_menuReservation.fSpaceY = fSpaceY;
}