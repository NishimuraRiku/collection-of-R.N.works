//========================================
// 
// UI:ダイアログボックス[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_dialog-box_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_dialog-box_00.h"	// UI :ダイアログボックス	[00]

//****************************************
// マクロ定義
//****************************************
// UI:ダイアログボックス[00] の幅
// UI:ダイアログボックス[00] の高さ
#define UI_DIALOGBOX_00_WIDTH	(PIXEL*256)
#define UI_DIALOGBOX_00_HEIGHT	(PIXEL*64)

// UI:ダイアログボックス[00] の出現にかかる時間
// UI:ダイアログボックス[00] の消えるのにかかる時間
#define UI_DIALOGBOX_00_IN_POP_TIME		(10)
#define UI_DIALOGBOX_00_IN_CLEAR_TIME	(10)

// UI:ダイアログボックス[00] の設定SE
#define UI_DIALOGBOX_00_SET_SE	(SOUND_LABEL_SE_CRY_001)

//****************************************
// プロトタイプ宣言
//****************************************
// UI:ダイアログボックス[00] のパラメーターの初期化処理
void InitParameterUi_dialogBox_00(void);

// UI:ダイアログボックス[00] の状態処理
void StateProcessUi_dialogBox_00(void);

// UI:ダイアログボックス[00] のキー入力処理
void KeyInputUi_dialogBox_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_dialogBox_00	// テクスチャへのポインタ
						[UI_DIALOGBOX_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_dialogBox_00	// 頂点バッファへのポインタ
						= NULL;
Ui_dialogBox_00			g_ui_dialogBox_00;			// UI:ダイアログボックス[00] の情報

// UI:ダイアログボックス[00] の種類毎のテクスチャの相対パス
const char c_aTexturePathUi_dialogBox_00[UI_DIALOGBOX_00_TYPE_MAX][TXT_MAX] = 
{
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\00.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\01.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\02.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\03.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\04.png",
};

//========== *** UI:ダイアログボックス[00] の情報を取得 ***
Ui_dialogBox_00 *GetUi_dialogBox_00(void)
{
	return &g_ui_dialogBox_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_dialogBox_00関数 - UI:ダイアログボックス[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_dialogBox_00(void)
{
	Ui_dialogBox_00	*pUi	// UI:ダイアログボックス[00] の情報のポインタ 
						= &g_ui_dialogBox_00;

	pUi->type			= UI_DIALOGBOX_00_TYPE_NONE;	// 種類
	pUi->state			= UI_DIALOGBOX_00_STATE_CLEAR;	// 状態
	pUi->nCounterState	= 0;							// 状態カウンター
	pUi->fAlpha			= 0.0f;							// 全体の透明度
	pUi->scale			= { 1.0f,0.0f,0.0f };			// 拡大倍率
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcessUi_dialogBox_00(void)
{
	Ui_dialogBox_00	*pUi	// UI:ダイアログボックス[00] の情報のポインタ 
					= &g_ui_dialogBox_00;

	switch (pUi->state)
	{
	case /*/ 出現 /*/UI_DIALOGBOX_00_STATE_POP: {
		break;
	}
	case /*/ 出現中 /*/UI_DIALOGBOX_00_STATE_IN_POP: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		float	fRate	// 割合
				= (float)pUi->nCounterState / (float)UI_DIALOGBOX_00_IN_POP_TIME;

		// 透明度と高さの倍率を更新
		pUi->fAlpha			= fRate;
		pUi->scale.fHeight	= fRate;

		if (pUi->nCounterState >= UI_DIALOGBOX_00_IN_POP_TIME)
		{// 状態カウンターが出現にかかる時間に達した時、
			// 状態を出現にする
			SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_POP);
		}

		break;
	}
	case /*/ 消えている /*/UI_DIALOGBOX_00_STATE_CLEAR: {
		break;
	}
	case /*/ 消え中 /*/UI_DIALOGBOX_00_STATE_IN_CLEAR: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		float	fRate	// 割合
				= (float)pUi->nCounterState / (float)UI_DIALOGBOX_00_IN_POP_TIME;

		// 透明度と高さの倍率を更新
		pUi->fAlpha			= 1.0f - fRate;
		pUi->scale.fHeight	= 1.0f - fRate;

		if (pUi->nCounterState >= UI_DIALOGBOX_00_IN_CLEAR_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_CLEAR);
		}

		break;
	}
	}
}

//========================================
// KeyInputUi_dialogBox_00関数 - UI:ダイアログボックス[00] のキー入力処理 -
//========================================
void KeyInputUi_dialogBox_00(void) 
{
	Ui_dialogBox_00	*pUi	// UI:ダイアログボックス[00] の情報のポインタ 
					= &g_ui_dialogBox_00;

	if (pUi->state == UI_DIALOGBOX_00_STATE_POP)
	{// 状態が出現の時、
		if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A) || (GetMouseTrigger(MOUSE_LEFT))))
		{// ENTERキー or Aボタン or マウス左ボタンが入力された時、
			// 状態を消え中にする
			SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_IN_CLEAR);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_dialogBox_00関数 - UI:ダイアログボックス[00] の初期化処理 -
//========================================
void InitUi_dialogBox_00(void)
{
	// パラメーターの初期化処理
	InitParameterUi_dialogBox_00();

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
		&g_pVtxBuffUi_dialogBox_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_dialogBox_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntType = 0; nCntType < UI_DIALOGBOX_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, c_aTexturePathUi_dialogBox_00[nCntType], &g_aTextureUi_dialogBox_00[nCntType]);
	}

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_DIALOGBOX_00_WIDTH,
		UI_DIALOGBOX_00_HEIGHT,
		ANGLE_TYPE_FREE);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, { 255,255,255,255 });

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_dialogBox_00->Unlock();
}

//========================================
// UninitUi_dialogBox_00関数 - UI:ダイアログボックス[00] の終了処理 -
//========================================
void UninitUi_dialogBox_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < UI_DIALOGBOX_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureUi_dialogBox_00[nCntType] != NULL)
		{
			g_aTextureUi_dialogBox_00[nCntType]->Release();
			g_aTextureUi_dialogBox_00[nCntType] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_dialogBox_00 != NULL) 
	{
		g_pVtxBuffUi_dialogBox_00->Release();
		g_pVtxBuffUi_dialogBox_00 = NULL;
	}
}

//========================================
// UpdateUi_dialogBox_00関数 - UI:ダイアログボックス[00] の更新処理 -
//========================================
void UpdateUi_dialogBox_00(void) 
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_dialogBox_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_dialogBox_00	*pUi	// UI:ダイアログボックス[00] の情報のポインタ
						= &g_ui_dialogBox_00;

	// 状態処理
	StateProcessUi_dialogBox_00();

	// キー入力処理
	KeyInputUi_dialogBox_00();

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		UI_DIALOGBOX_00_POS,
		INITD3DXVECTOR3,
		false,
		UI_DIALOGBOX_00_WIDTH
		* pUi->scale.fWidth,
		UI_DIALOGBOX_00_HEIGHT
		* pUi->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * g_ui_dialogBox_00.fAlpha) });
}

//========================================
// DrawUi_dialogBox_00関数 - UI:ダイアログボックス[00] の描画処理 -
//========================================
void DrawUi_dialogBox_00(void)
{
	Ui_dialogBox_00	*pUi	// UI:ダイアログボックス[00] の情報のポインタ
					= &g_ui_dialogBox_00;

	if (pUi->state == UI_DIALOGBOX_00_STATE_CLEAR)
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_dialogBox_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureUi_dialogBox_00[pUi->type]);

	// UI:ダイアログボックス[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetState関数 - UI:ダイアログボックス[00] の状態設定処理 -
//========================================
void SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE state) 
{
	Ui_dialogBox_00	*pUi	// UI:ダイアログボックス[00] の情報のポインタ
					= &g_ui_dialogBox_00;

	// 状態を代入
	pUi->state = state;

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}

//========================================
// SetUi_dialogBox_00関数 - UI:ダイアログボックス[00] の設定処理 -
//========================================
void SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE type)
{
	Ui_dialogBox_00	*pUi	// UI:ダイアログボックス[00] の情報のポインタ 
					= &g_ui_dialogBox_00;

	if (pUi->state != UI_DIALOGBOX_00_STATE_CLEAR) 
	{// 消えている状態でない時、
		// 処理を終了する
		return;
	}

	// 設定SEを再生
	PlaySound(UI_DIALOGBOX_00_SET_SE);

	// 種類を代入
	pUi->type = type;

	// 出現中の状態にする
	SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_IN_POP);
}