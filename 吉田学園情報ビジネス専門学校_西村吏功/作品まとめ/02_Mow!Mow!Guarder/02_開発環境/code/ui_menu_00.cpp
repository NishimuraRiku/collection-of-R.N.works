//========================================
// 
// UI:メニュー[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_menu_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_menu_00.h"	// UI:メニュー	[00]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// マクロ定義
//****************************************
// UI:メニュー[00] の最大数
// UI:メニュー[00] の幅
// UI:メニュー[00] の高さ
// UI:メニュー[00] のテクスチャのパターン数(横)
// UI:メニュー[00] のテクスチャのパターン数(縦)
// UI:メニュー[00] の選択時の色
#define UI_MENU_00_MAX			(16)
#define UI_MENU_00_WIDTH		(PIXEL * 256)
#define UI_MENU_00_HEIGHT		(PIXEL * 16)
#define UI_MENU_00_SELECT_COLOR	(Color{243, 189, 63, 255})

// UI:メニュー[00] の配置間隔X
// UI:メニュー[00] の配置間隔Y
// UI:メニュー[00] が消えるのにかかる時間
#define UI_MENU_00_SPACE_X		(PIXEL * 0)
#define UI_MENU_00_SPACE_Y		(PIXEL * 16)
#define UI_MENU_00_CLEAR_TIME	(4)

// UI:メニュー[00] の出現にかかる時間
// UI:メニュー[00] の出現している時の大きさにかかる倍率
// UI:メニュー[00] の消えている時の大きさにかかる倍率
#define UI_MENU_00_APPEAR_TIME		(10)
#define UI_MENU_00_APPEAR_SCALE		Scale{1.0f,1.0f,0.0f}
#define UI_MENU_00_DISAPPEAR_SCALE	Scale{1.0f,0.0f,0.0f}

// UI:メニュー[00] の選択SE
// UI:メニュー[00] の決定SE
#define UI_MENU_00_SELECT_SE		(SOUND_LABEL_SE_SELECT_000)
#define UI_MENU_00_DETERMINATION_SE	(SOUND_LABEL_SE_DETERMINATION_001)

//****************************************
// 構造体の定義
//****************************************
// UI:メニュー[00] の予約情報
typedef struct 
{
	bool			bReservation;	// 予約フラグ
	Ui_menu_00Set	*pSet;			// 設定情報のポインタ
	int				nNum;			// メニュー数
}Ui_menu_00Reservation;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:メニュー[00] のパラメーター初期化処理
// UI:メニュー[00] の管理情報のパラメーター初期化処理
// UI:メニュー[00] の予約情報のパラメーター初期化処理
void InitParameterUi_menu_00(Ui_menu_00 *pUi);
void InitParameterUi_menu_00Control(void);
void InitParameterUi_menu_00Reservation(void);

// UI:メニュー[00] の状態処理
// UI:メニュー[00] の作成処理
void StateProcessUi_menu_00(void);
void CreateUi_menu_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_menu_00		// テクスチャへのポインタ
						[UI_MENU_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_menu_00		// 頂点バッファへのポインタ
						= NULL;
Ui_menu_00				g_aUi_menu_00				// UI:メニュー[00] の情報
						[UI_MENU_00_MAX];
Ui_menu_00Control		g_ui_menu_00Control;		// UI:メニュー[00] の管理情報
Ui_menu_00Reservation	g_ui_menu_00Reservation;	// UI:メニュー[00] の予約情報

// UI:メニュー[00] のテクスチャファイルの相対パス
char g_aTexturePath[UI_MENU_00_TYPE_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_menu_000\\normal.png",
	"data\\TEXTURE\\UserInterface\\ui_menu_000\\select.png",
};

//========== *** UI:メニュー[00] の情報を取得 ***
Ui_menu_00 *GetUi_menu_00(void) 
{
	return g_aUi_menu_00;
}

//========== *** UI:メニュー[00] の全体管理の情報を取得 ***
Ui_menu_00Control *GetUi_menu_00Control(void) 
{
	return &g_ui_menu_00Control;
}

//========== *** UI:メニュー[00] の現在の選択番号を取得 ***
int *GetSelect(void) 
{
	return &g_ui_menu_00Control.nSelect;
}


//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_menu_00関数 - UI:メニュー[00] のパラメーター初期化処理 -
//========================================
void InitParameterUi_menu_00(Ui_menu_00 *pUi)
{
	pUi->pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pUi->type		= (UI_MENU_00_TYPE)0;				// 種類
	pUi->bUse		= false;							// 使用されているかフラグ
	pUi->bSelect	= false;							// 選択されているかフラグ
	sprintf(pUi->aString, "");							// 表示する文字列
	pUi->nSelectNum = 0;								// 現在の選択肢の番号
}

//========================================
// InitParameterUi_menu_00Control関数 - UI:メニュー[00] の管理情報のパラメーター初期化処理 -
//========================================
void InitParameterUi_menu_00Control(void)
{
	Ui_menu_00Control	*pUiCtl	// UI:メニュー[00] の管理情報のポインタ
						= &g_ui_menu_00Control;

	pUiCtl->state			= UI_MENU_00_STATE_NONE;			// 状態 
	pUiCtl->nCounterState	= 0;								// 状態のカウンター 
	pUiCtl->scale			= { 0.0f,0.0f,0.0f };				// 拡大率 
	pUiCtl->nNum			= 0;								// 現在の使用数 
	pUiCtl->nSelect			= 0;								// 現在の選択番号 
	pUiCtl->centerPos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心座標 
}

//========================================
// InitParameterUi_menu_00Reservation関数 - UI:メニュー[00] の予約情報のパラメーター初期化処理 -
//========================================
void InitParameterUi_menu_00Reservation(void)
{
	Ui_menu_00Reservation	*pUiRsv	// UI:メニュー[00] の予約情報のポインタ
							= &g_ui_menu_00Reservation;

	pUiRsv->bReservation	= false;	// 予約フラグ
	pUiRsv->pSet			= NULL;		// 設定情報のポインタ
	pUiRsv->nNum			= 0;		// メニュー数
}

//========================================
// StateProcessUi_menu_00関数 - UI:メニュー[00] の状態処理 -
//========================================
void StateProcessUi_menu_00(void)
{
	Ui_menu_00Control	*pUiCtl	// UI:メニュー[00] の管理情報のポインタ
						= &g_ui_menu_00Control;

	switch (pUiCtl->state)
	{
		//========== *** 出現中 ***
	case UI_MENU_00_STATE_IN_POP: {
		if (--pUiCtl->nCounterState <= 0)
		{// 状態のカウンターを減算した結果0以下の時、
			// 出現状態にする
			pUiCtl->state = UI_MENU_00_STATE_POP;
		}

		float	fRate	// 割合 
				= (float)pUiCtl->nCounterState / (float)UI_MENU_00_APPEAR_TIME;

		// 大きさにかかる倍率を更新
		pUiCtl->scale.fWidth =
			(UI_MENU_00_APPEAR_SCALE.fWidth * (1.0f - fRate)) +
			(UI_MENU_00_DISAPPEAR_SCALE.fWidth * fRate);
		pUiCtl->scale.fHeight =
			(UI_MENU_00_APPEAR_SCALE.fHeight * (1.0f - fRate)) +
			(UI_MENU_00_DISAPPEAR_SCALE.fHeight * fRate);
	}
		break;
		//========== *** 出現 ***
	case UI_MENU_00_STATE_POP: {
	}
		break;
		//========== *** 消え中 ***
	case UI_MENU_00_STATE_IN_CLEAR: {
		if (--pUiCtl->nCounterState <= 0)
		{// 状態のカウンターを減算した結果0以下の時、
			// 状態を無しにする
			pUiCtl->state = UI_MENU_00_STATE_NONE;

			// UI:メニュー[00] を全て使用していない状態にする
			for (int nCntUi = 0; nCntUi < UI_MENU_00_MAX; nCntUi++) {
				g_aUi_menu_00[nCntUi].bUse = false;
			}
		}

		float	fRate	// 割合 
				= (float)pUiCtl->nCounterState / (float)UI_MENU_00_CLEAR_TIME;

		// 大きさにかかる倍率を更新
		pUiCtl->scale.fWidth =
			(UI_MENU_00_APPEAR_SCALE.fWidth * fRate) +
			(UI_MENU_00_DISAPPEAR_SCALE.fWidth * (1.0f - fRate));
		pUiCtl->scale.fHeight =
			(UI_MENU_00_APPEAR_SCALE.fHeight * fRate) +
			(UI_MENU_00_DISAPPEAR_SCALE.fHeight * (1.0f - fRate));
	}
		break;
	}
}

//========================================
// CreateUi_menu_00関数 - UI:メニュー[00] の作成処理 -
//========================================
void CreateUi_menu_00(void)
{
	Ui_menu_00Control		*pUiCtl	// UI:メニュー[00] の管理情報のポインタ
							= &g_ui_menu_00Control;
	Ui_menu_00Reservation	*pUiRsv	// UI:メニュー[00] の予約情報のポインタ
							= &g_ui_menu_00Reservation;

	if ((!pUiRsv->bReservation)
		||
		(pUiCtl->state != UI_MENU_00_STATE_NONE))
	{// メニューの予約が無い、もしくは状態が無しでない時、
		// 処理を終了する
		return;
	}

	pUiRsv->bReservation	= false;					// 予約フラグを偽にする
	pUiCtl->state			= UI_MENU_00_STATE_IN_POP;	// 出現中にする
	pUiCtl->nCounterState	= UI_MENU_00_APPEAR_TIME;	// 状態カウンターを設定
	pUiCtl->nSelect			= 0;						// 現在の選択番号を初期化する
	pUiCtl->nNum			= pUiRsv->nNum;				// 現在の使用数を予約情報から代入

	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_menu_00	*pUi	// UI:メニュー[00] の情報のポインタ 
				= g_aUi_menu_00;

	for (int nCntUi = 0; nCntUi < pUiRsv->nNum; nCntUi++, pVtx += 4, pUi++)
	{
		Ui_menu_00Set	*pSet	// UI:メニュー[00] の設定情報のポインタ
						= &pUiRsv->pSet[nCntUi];

		pUi->type		= pSet->type;			// 種類を代入
		pUi->bUse		= true;					// 使用されている状態にする
		pUi->bSelect	= false;				// 選択されていない状態にする
		sprintf(pUi->aString, pSet->aString);	// 文字列を代入

		// 位置を設定
		pUi->pos =
			D3DXVECTOR3(
				pUiCtl->centerPos.x - (((pUiRsv->nNum - 1) * UI_MENU_00_SPACE_X) * 0.5f) + (nCntUi * UI_MENU_00_SPACE_X),
				pUiCtl->centerPos.y - (((pUiRsv->nNum - 1) * UI_MENU_00_SPACE_Y) * 0.5f) + (nCntUi * UI_MENU_00_SPACE_Y), 0.0f);

		// 頂点座標の設定
		SetVertexPos2D(pVtx,
			pUi->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_MENU_00_WIDTH * pUiCtl->scale.fWidth,
			UI_MENU_00_HEIGHT * pUiCtl->scale.fHeight,
			ANGLE_TYPE_FIXED);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_menu_00->Unlock();
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_menu_00関数 - UI:メニュー[00] の初期化処理 -
//========================================
void InitUi_menu_00(void)
{
	InitParameterUi_menu_00Control();		// 管理情報のパラメーター初期化
	InitParameterUi_menu_00Reservation();	// 予約情報のパラメーター初期化

	LPDIRECT3DDEVICE9	pDevice	// デバイスへのポインタを取得
						= GetDevice();
	VERTEX_2D			*pVtx;	// 頂点情報へのポインタ
	
	for (int nCntType = 0; nCntType < UI_MENU_00_TYPE_MAX; nCntType++) 
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aTexturePath[nCntType], &g_aTextureUi_menu_00[nCntType]);
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

	Ui_menu_00			*pUi	// UI:メニュー[00] の情報のポインタ
						= g_aUi_menu_00;
	Ui_menu_00Control	*pUiCtl	// UI:メニュー[00] の管理情報のポインタ
						= &g_ui_menu_00Control;

	for (int nCntUi = 0; nCntUi < UI_MENU_00_MAX; nCntUi++, pVtx += 4, pUi++)
	{
		InitParameterUi_menu_00(pUi);	// パラメーターの初期化処理

		// 頂点座標の設定
		SetVertexPos2D(pVtx,
			pUi->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_MENU_00_WIDTH * pUiCtl->scale.fWidth,
			UI_MENU_00_HEIGHT * pUiCtl->scale.fHeight,
			ANGLE_TYPE_FIXED);

		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx, 0, 1, 1, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// UninitUi_menu_00関数 - UI:メニュー[00] の終了処理 -
//========================================
void UninitUi_menu_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < UI_MENU_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureUi_menu_00[nCntType] != NULL)
		{
			g_aTextureUi_menu_00[nCntType]->Release();
			g_aTextureUi_menu_00[nCntType] = NULL;
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
// UpdateUi_menu_00関数 - UI:メニュー[00] の更新処理 -
//========================================
void UpdateUi_menu_00(void)
{
	StateProcessUi_menu_00();	// 状態処理
	CreateUi_menu_00();			// 作成処理

	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_menu_00			*pUi	// UI:メニュー[00] の情報のポインタ
						= g_aUi_menu_00;
	Ui_menu_00Control	*pUiCtl	// UI:メニュー[00] の管理情報のポインタ
						= &g_ui_menu_00Control;

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pVtx += 4, pUi++)
	{
		if (!pUi->bUse)
		{// 使用されていない時、
			// 繰り返し処理をリセット
			continue;
		}

		// 頂点座標の設定
		SetVertexPos2D(pVtx,
			pUi->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_MENU_00_WIDTH * pUiCtl->scale.fWidth,
			UI_MENU_00_HEIGHT * pUiCtl->scale.fHeight,
			ANGLE_TYPE_FIXED);

		// rhwの設定
		SetRHW2D(pVtx);

		Color setCol =	// 設定色
			nCntUi_menu_00 == g_ui_menu_00Control.nSelect ?
			UI_MENU_00_SELECT_COLOR :
			Color{ 255,255,255,255 };

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, setCol);

		// テキストの設定
		SetText2D(
			pUi->aString,
			FONT_002, DISPLAY_CENTER,
			pUi->pos, setCol,
			pUiCtl->scale.fWidth,
			pUiCtl->scale.fHeight);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// Ui_menu_00Input関数 - UI:メニュー[00] の入力処理 -
//========================================
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode)
{
	int					nSelected	// 選択した番号(-1は選択無し)
						= -1;
	Ui_menu_00Control	*pUiCtl		// UI:メニュー[00] の管理情報のポインタ
						= &g_ui_menu_00Control;

	if ((pUiCtl->state != UI_MENU_00_STATE_POP)
		||
		(GetFade() != FADE_NONE))
	{// 出現状態でない or フェード中の時、
		// 処理を終了する
		return nSelected;
	}

	{
		bool	bInput	// 入力フラグ
				= false;

		if (
			(((GetKeyboardRepeat(DIK_UP))
			|| (GetKeyboardRepeat(DIK_W))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
			|| (GetButtonRepeat(BUTTON_UP)))
			&& (mode == UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
			||
			(((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
			&& (mode == UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT)))
		{// 上入力時、
			g_ui_menu_00Control.nSelect--;	// 現在の選択番号を減算
			bInput = true;					// 入力フラグを真にする
		}
		else if (
			(((GetKeyboardRepeat(DIK_DOWN))
			|| (GetKeyboardRepeat(DIK_S))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
			|| (GetButtonRepeat(BUTTON_DOWN)))
			&& (mode == UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
			||
			(((GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
			&& (mode == UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT)))
		{// 下/右入力時、
			g_ui_menu_00Control.nSelect++;	// 現在の選択番号を加算
			bInput = true;					// 入力フラグを真にする
		}

		if ((g_ui_menu_00Reservation.nNum != 1) && (bInput))
		{// メニューの数が1で無い & 入力フラグが真の時、
			PlaySound(UI_MENU_00_SELECT_SE);	// 選択SEを再生
		}
	}

	// 選択番号をループ制御
	IntLoopControl(&pUiCtl->nSelect, pUiCtl->nNum, 0);

	if (g_aUi_menu_00[g_ui_menu_00Control.nSelect].type == UI_MENU_00_TYPE_SELECTION)
	{// 種類が選択の時、
		if (
			(GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// 左/右入力時、
			nSelected = g_ui_menu_00Control.nSelect;	// 選択した番号に現在の選択番号を代入
			PlaySound(UI_MENU_00_SELECT_SE);			// 選択SEを再生
		}
	}
	else if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A) || (GetMouseTrigger(MOUSE_LEFT))))
	{// ENTERキー or Aボタン or マウス左ボタンが入力された時、
		nSelected = g_ui_menu_00Control.nSelect;					// 選択した番号に現在の選択番号を代入
		g_ui_menu_00Control.state = UI_MENU_00_STATE_IN_CLEAR;		// 消え中にする
		g_ui_menu_00Control.nCounterState = UI_MENU_00_CLEAR_TIME;	// 状態カウンターを設定する
		if (g_ui_menu_00Reservation.pSet[nSelected].bDecisionSE)
		{// 決定SEフラグが真の時、決定SEを再生
			PlaySound(UI_MENU_00_DETERMINATION_SE);
		}
	}
	
	// 選択した番号を返す
	return nSelected;
}

//========================================
// DrawUi_menu_00関数 - UI:メニュー[00] の描画処理 -
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

	Ui_menu_00	*pUi	// UI:メニュー[00] の情報のポインタ
				= g_aUi_menu_00;

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pUi++)
	{
		if (!pUi->bUse)
		{// 使用されていない時、
			continue;	// 繰り返し処理を折り返す
		}

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_menu_00[pUi->type]);

		// UI:メニュー[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_menu_00 * 4, 2);
	}
}

//========================================
// SetUi_menu_00Pos関数 - UI:メニュー[00] の中心座標を設定 -
//========================================
void SetUi_menu_00Pos(D3DXVECTOR3 pos)
{
	Ui_menu_00Control	*pUiCtl		// UI:メニュー[00] の管理情報のポインタ
						= &g_ui_menu_00Control;

	// 中心座標を代入
	pUiCtl->centerPos = pos;
}

//========================================
// SetUi_menu_00関数 - UI:メニュー[00] の設定処理 -
//========================================
void SetUi_menu_00(Ui_menu_00Set *pSet, int nNum)
{
	g_ui_menu_00Reservation.bReservation	= true;	// 予約フラグを真にする
	g_ui_menu_00Reservation.pSet			= pSet;	// 設定情報のポインタを代入
	g_ui_menu_00Reservation.nNum			= nNum;	// メニュー数を代入
}