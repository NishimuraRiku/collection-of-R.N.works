//========================================
// 
// UI:TIPS[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_tips_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_tips_00.h"	// UI :TIPS	[00]

#include "obj_stage_00.h"
#include "obj_turret_00.h"
#include "obj_discharger_00.h"
#include "chr_enemy_00.h"
#include "chr_player_00.h"

#include "ui_menu_00.h"

#include "md_tutorial_00.h"

//****************************************
// マクロ定義
//****************************************
// UI:TIPS[00] の種類の最大数
#define UI_TIPS_00_TYPE_MAX (2)
// UI:TIPS[00] の幅
#define UI_TIPS_00_WIDTH (PIXEL*256)
// UI:TIPS[00] の高さ
#define UI_TIPS_00_HEIGHT (PIXEL*256)
// UI:TIPS[00] の出現にかかる時間
#define UI_TIPS_00_IN_POP_TIME (10)
// UI:TIPS[00] の消えるのにかかる時間
#define UI_TIPS_00_IN_CLEAR_TIME (10)
// UI:TIPS[00] の切り替えにかかる時間
#define UI_TIPS_00_SWITCH_TIME (5)
// UI:TIPS[00] の切り替え時の高さの下限
#define UI_TIPS_00_SWITCH_HEIGHT_MIN (0.95f)
// UI:TIPS[00] の切り替え時の高さの上限
#define UI_TIPS_00_SWITCH_HEIGHT_MAX (1.0f)
// UI:TIPS[00] の切り替えSE
#define UI_TIPS_00_SWITCH_SE (SOUND_LABEL_SE_SWITCH_000)
// UI:TIPS[00] の設定SE
#define UI_TIPS_00_SET_SE (SOUND_LABEL_SE_DETERMINATION_001)
// UI:TIPS[00] のステップ最大数
#define UI_TIPS_00_STEP_MAX (2)
// UI:TIPS[00] の位置
#define UI_TIPS_00_POS D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),BUFFER_HEIGHT*0.5f,0.0f)
// TIPSメニューの位置
#define TIPS_MENU_POS D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),BUFFER_HEIGHT-PIXEL*32,0.0f)

//****************************************
// 構造体の定義
//****************************************
// TIPSメニュー
typedef enum
{
	TIPS_MENU_OK,	// OK
	TIPS_MENU_MAX,
}TIPS_MENU;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:TIPS[00] のパラメーターの初期化処理
void InitParameterUi_tips_00(void);
// UI:TIPS[00] の状態処理
void StateProcessUi_tips_00(void);
// UI:TIPS[00] のキー入力処理
void KeyInputUi_tips_00(void);

//****************************************
// グローバル宣言
//****************************************
// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_aTextureUi_tips_00[UI_TIPS_00_TYPE_MAX][UI_TIPS_00_STEP_MAX] = {};
// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_tips_00 = NULL;
Ui_tips_00 g_ui_tips_00;	// UI:TIPS[00] の情報

// UI:TIPS[00] の種類毎のステップ数
const int g_aStepNumUi_tips_00[UI_TIPS_00_TYPE_MAX] = {
	2,
	1,
};

// UI:TIPS[00] のステップ毎のテクスチャの相対パス
const char g_aTexturePathUi_tips_00[UI_TIPS_00_TYPE_MAX][UI_TIPS_00_STEP_MAX][TXT_MAX] =
{
	{
		"data\\TEXTURE\\UserInterface\\ui_tips_000\\00.png",
		"data\\TEXTURE\\UserInterface\\ui_tips_000\\01.png",
	},
	{
		"data\\TEXTURE\\UserInterface\\ui_tips_000\\02.png",
	}
};


// TIPSメニュー設定情報
Ui_menu_00Set g_aTipsMenuSet[TIPS_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"OK",true },
};

//========== *** UI:TIPS[00] の情報を取得 ***
Ui_tips_00 *GetUi_tips_00(void)
{
	return &g_ui_tips_00;
}

//========== *** UI:TIPS[00] の切替を取得 ***
bool GetSwitchUi_tips_00(void)
{
	return (!g_ui_tips_00.bStepViewAllOld && g_ui_tips_00.bStepViewAll);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_tips_00関数 - UI:TIPS[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_tips_00(void)
{
	Ui_tips_00	*pUi	// UI:TIPS[00] の情報のポインタ 
				= &g_ui_tips_00;

	pUi->nType				= 0;						// 種類
	pUi->nStep				= (ACTIVE_INPUT_TYPE)0;		// ステップ
	pUi->bStepViewAll		= false;					// ステップを全て閲覧したフラグ
	pUi->bStepViewAllOld	= false;					// ステップを全て閲覧したフラグ(過去)
	pUi->state				= UI_TIPS_00_STATE_CLEAR;	// 状態
	pUi->nCounterState		= 0;						// 状態カウンター
	pUi->fAlpha				= 0.0f;						// 全体の透明度
	pUi->scale				= { 1.0f,0.0f,0.0f };		// 拡大倍率
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcessUi_tips_00(void)
{
	Ui_tips_00	*pUi	// UI:TIPS[00] の情報のポインタ 
				= &g_ui_tips_00;

	switch (pUi->state)
	{
	case /*/ 出現 /*/UI_TIPS_00_STATE_POP: {

		break;
	}
	case /*/ 出現中 /*/UI_TIPS_00_STATE_IN_POP: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		float	fRate	// 割合
				= (float)pUi->nCounterState / (float)UI_TIPS_00_IN_POP_TIME;

		// 透明度と高さの倍率を更新
		pUi->fAlpha			= fRate;
		pUi->scale.fHeight	= fRate;

		if (pUi->nCounterState >= UI_TIPS_00_IN_POP_TIME)
		{// 状態カウンターが出現にかかる時間に達した時、
			// 状態を出現にする
			SetStateUi_tips_00(UI_TIPS_00_STATE_POP);
		}

		break;
	}
	case /*/ 消えている /*/UI_TIPS_00_STATE_CLEAR: {

		break;
	}
	case /*/ 消え中 /*/UI_TIPS_00_STATE_IN_CLEAR: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		float	fRate	// 割合
				= (float)pUi->nCounterState / (float)UI_TIPS_00_IN_CLEAR_TIME;

		// 透明度と高さの倍率を更新
		pUi->fAlpha			= 1.0f - fRate;
		pUi->scale.fHeight	= 1.0f - fRate;

		if (pUi->nCounterState >= UI_TIPS_00_IN_CLEAR_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateUi_tips_00(UI_TIPS_00_STATE_CLEAR);
		}

		break;
	}
	case /*/ 切り替え /*/UI_TIPS_00_STATE_SWITCH: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		float	fRate	// 割合
				= (float)pUi->nCounterState / (float)UI_TIPS_00_SWITCH_TIME;

		// 高さの倍率を更新
		pUi->scale.fHeight = 
			(UI_TIPS_00_SWITCH_HEIGHT_MAX * fRate) +
			(UI_TIPS_00_SWITCH_HEIGHT_MIN * (1.0f - fRate));

		if (pUi->nCounterState >= UI_TIPS_00_SWITCH_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 状態を出現にする
			SetStateUi_tips_00(UI_TIPS_00_STATE_POP);
		}

		break;
	}
	}
}

//========================================
// KeyInputUi_tips_00関数 - UI:TIPS[00] のキー入力処理 -
//========================================
void KeyInputUi_tips_00(void) 
{
	Ui_tips_00	*pUi	// UI:TIPS[00] の情報のポインタ 
					= &g_ui_tips_00;

	// 過去のステップを全て閲覧フラグを保存
	pUi->bStepViewAllOld = pUi->bStepViewAll;

	if (pUi->state == UI_TIPS_00_STATE_POP)
	{// 状態が出現の時、
		bool	bStepSwitch	// ステップ切り替えフラグ
				= false;

		if ((GetKeyboardTrigger(DIK_A))		||
			(GetKeyboardTrigger(DIK_LEFT))	||
			(GetButtonTrigger(BUTTON_LEFT))	||
			(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT]))
		{// 左に入力された時、ステップを減算制御
			int nStep = pUi->nStep;
			int nStepTemp = nStep;
			nStep--;
			IntControl(&nStep, g_aStepNumUi_tips_00[pUi->nType] - 1, 0);
			pUi->nStep	= (ACTIVE_INPUT_TYPE)nStep;
			bStepSwitch = nStepTemp != nStep;	// ステップ切り替えフラグを設定する
		}
		else if (
			(GetKeyboardTrigger(DIK_D))			||
			(GetKeyboardTrigger(DIK_RIGHT))		||
			(GetButtonTrigger(BUTTON_RIGHT))	||
			(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT]))
		{// 右に入力された時、ステップを加算制御
			int nStep = pUi->nStep;
			int nStepTemp = nStep;
			nStep++;
			IntControl(&nStep, g_aStepNumUi_tips_00[pUi->nType] -1, 0);
			pUi->nStep	= (ACTIVE_INPUT_TYPE)nStep;
			bStepSwitch = nStepTemp != nStep;	// ステップ切り替えフラグを設定する
		}

		if (pUi->nStep >= g_aStepNumUi_tips_00[pUi->nType] - 1)
		{// ステップがステップ数に達している時、
			pUi->bStepViewAll = true;	// ステップを全て閲覧したフラグを真にする
		}

		if (bStepSwitch) 
		{// ステップ切り替えフラグが真の時、
			SetStateUi_tips_00(	// 状態を切り替えに設定
				UI_TIPS_00_STATE_SWITCH);
			PlaySound(			// 切り替えSEを再生
				UI_TIPS_00_SWITCH_SE);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_tips_00関数 - UI:TIPS[00] の初期化処理 -
//========================================
void InitUi_tips_00(void)
{
	// パラメーターの初期化処理
	InitParameterUi_tips_00();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_tips_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_tips_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntType = 0; nCntType < UI_TIPS_00_TYPE_MAX; nCntType++)
	{
		for (int nCntStep = 0; nCntStep < UI_TIPS_00_STEP_MAX; nCntStep++)
		{
			D3DXCreateTextureFromFile(pDevice, g_aTexturePathUi_tips_00[nCntType][nCntStep], &g_aTextureUi_tips_00[nCntType][nCntStep]);
		}
	}

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_TIPS_00_WIDTH,
		UI_TIPS_00_HEIGHT,
		ANGLE_TYPE_FREE);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, { 255,255,255,255 });

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_tips_00->Unlock();
}

//========================================
// UninitUi_tips_00関数 - UI:TIPS[00] の終了処理 -
//========================================
void UninitUi_tips_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < UI_TIPS_00_TYPE_MAX; nCntType++)
	{
		for (int nCntStep = 0; nCntStep < UI_TIPS_00_STEP_MAX; nCntStep++)
		{
			if (g_aTextureUi_tips_00[nCntType][nCntStep] != NULL)
			{
				g_aTextureUi_tips_00[nCntType][nCntStep]->Release();
				g_aTextureUi_tips_00[nCntType][nCntStep] = NULL;
			}
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_tips_00 != NULL) 
	{
		g_pVtxBuffUi_tips_00->Release();
		g_pVtxBuffUi_tips_00 = NULL;
	}
}

//========================================
// UpdateUi_tips_00関数 - UI:TIPS[00] の更新処理 -
//========================================
void UpdateUi_tips_00(void) 
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_tips_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_tips_00	*pUi	// UI:TIPS[00] の情報のポインタ
				= &g_ui_tips_00;

	if (!g_ui_tips_00.bStepViewAllOld && g_ui_tips_00.bStepViewAll)
	{// 切替取得時、
		// UI:メニュー[00] の中心座標を設定
		SetUi_menu_00Pos(TIPS_MENU_POS);

		// UI:メニュー[00] の設定処理(ユーザーガイドメニュー)
		SetUi_menu_00(
			g_aTipsMenuSet,
			TIPS_MENU_MAX);
	}

	if (pUi->bStepViewAll)
	{// ステップを全て閲覧したフラグが真の時、
		// TIPSメニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ OK /*/TIPS_MENU_OK:
			// UI:TIPS[00] の状態を消え中にする
			SetStateUi_tips_00(UI_TIPS_00_STATE_IN_CLEAR);
			break;
		}

		if (GetUi_tips_00()->state == UI_TIPS_00_STATE_CLEAR)
		{// UI:TIPS[00] の状態が消えている時、
			// チュートリアルの状態を通常にする
			SetMd_tutorial_00State(MD_TUTORIAL_00_STATE_NORMAL);
			pUi->bStepViewAll = false;	// ステップを全て閲覧したフラグを偽にする
		}
	}

	// 状態処理
	StateProcessUi_tips_00();

	// キー入力処理
	KeyInputUi_tips_00();

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		UI_TIPS_00_POS,
		INITD3DXVECTOR3,
		false,
		UI_TIPS_00_WIDTH
		* pUi->scale.fWidth,
		UI_TIPS_00_HEIGHT
		* pUi->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * g_ui_tips_00.fAlpha) });
}

//========================================
// DrawUi_tips_00関数 - UI:TIPS[00] の描画処理 -
//========================================
void DrawUi_tips_00(void)
{
	Ui_tips_00	*pUi	// UI:TIPS[00] の情報のポインタ
					= &g_ui_tips_00;

	if (pUi->state == UI_TIPS_00_STATE_CLEAR)
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_tips_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureUi_tips_00[pUi->nType][pUi->nStep]);

	// UI:TIPS[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetState関数 - UI:TIPS[00] の状態設定処理 -
//========================================
void SetStateUi_tips_00(UI_TIPS_00_STATE state) 
{
	Ui_tips_00	*pUi	// UI:TIPS[00] の情報のポインタ
				= &g_ui_tips_00;

	// 状態を代入
	pUi->state = state;

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}

//========================================
// SetUi_tips_00関数 - UI:TIPS[00] の設定処理 -
//========================================
void SetUi_tips_00(int nType)
{
	// UI:TIPS[00] の情報のポインタ 
	Ui_tips_00 *pUi = &g_ui_tips_00;

	if (pUi->state != UI_TIPS_00_STATE_CLEAR)
	{// 消えている状態でない時、
		// 処理を終了する
		return;
	}

	// セットSEを再生
	PlaySound(UI_TIPS_00_SET_SE);

	pUi->nStep = 0;				// ステップを初期化
	pUi->nType = nType;			// 種類を代入
	pUi->bStepViewAll = false;	// ステップを全て閲覧したフラグを偽にする

	// 出現中の状態にする
	SetStateUi_tips_00(UI_TIPS_00_STATE_IN_POP);

	// チュートリアルの状態をTIPSにする
	SetMd_tutorial_00State(MD_TUTORIAL_00_STATE_TIPS);
}