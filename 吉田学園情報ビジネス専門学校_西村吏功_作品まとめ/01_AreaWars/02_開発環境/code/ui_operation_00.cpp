//========================================
// 
// 操作説明[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_operation_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_operation_00.h"

//****************************************
// マクロ定義
//****************************************

// 操作説明[00] の出現にかかる時間
// 操作説明[00] の消えるのにかかる時間
// 操作説明[00] の透明度の最大値
#define UI_OPERATION_00_IN_POP_TIME		(10)
#define UI_OPERATION_00_IN_CLEAR_TIME	(10)
#define UI_OPERATION_00_ALPHA_MAX		(1.0f)

// 操作説明[00] のキーボードの説明の開始位置
// 操作説明[00] のキーボードの説明の終了位置
#define UI_OPERATION_00_KEYBOARD_START_POS	(D3DXVECTOR3((SCREEN_WIDTH * 0.3f), (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))
#define UI_OPERATION_00_KEYBOARD_END_POS	(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - PIXEL * 76, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))

// 操作説明[00] のコントローラーの説明の開始位置
// 操作説明[00] のコントローラーの説明の終了位置
#define UI_OPERATION_00_CONTROLLER_START_POS	(D3DXVECTOR3((SCREEN_WIDTH * 0.7f), (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))
#define UI_OPERATION_00_CONTROLLER_END_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + PIXEL * 76, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))

//****************************************
// 列挙型の定義
//****************************************

// 操作説明[00] の部品番号
typedef enum
{
	UI_OPERATION_00_PARTS_KEYBOARD = 0,	// キーボードの説明
	UI_OPERATION_00_PARTS_CONTROLLER,	// コントローラーの説明
	UI_OPERATION_00_PARTS_MAX,
}UI_OPERATION_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// 操作説明[00] の情報構造体
typedef struct 
{
	UI_OPERATION_00_STATE	state;				// 状態
	int						nCounterState;		// 状態カウンター
	float					fAlpha;				// 全体の透明度
}Ui_operation_00;

// 操作説明[00] のポリゴン毎の情報構造体
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

	D3DXVECTOR3		pos;			// 位置
	int				nPtn;			// パターンNo.
	int				nCounterAnim;	// アニメーションのカウンター
}Ui_operation_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterUi_operation_00(void);

// 部品(キーボードの説明)の更新処理
void UpdateUi_operation_00Parts_keyboard(void);

// 部品(コントローラーの説明)の更新処理
void UpdateUi_operation_00Parts_controller(void);

// 状態処理
void StateProcessUi_operation_00(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_operation_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_operation_00	// テクスチャへのポインタ
						[UI_OPERATION_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_operation_00	// 頂点バッファへのポインタ
						= NULL;						//
Ui_operation_00			g_ui_operation_00;			// 操作説明[00] の情報

// 操作説明[00] の部品毎の情報
Ui_operation_00Parts g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_operation_000_keyboard.png"   , 4 , 4 , 0 , 15 , 10 , PIXEL * 256 , PIXEL * 256 , 1 },
	{ "data\\TEXTURE\\ui_operation_000_controller.png" , 4 , 4 , 0 , 15 , 10 , PIXEL * 256 , PIXEL * 256 , 1 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_operation_00Parts_keyboard関数 - 部品(キーボードの説明)の更新処理 -
//========================================
void UpdateUi_operation_00Parts_keyboard(void)
{
	Ui_operation_00Parts	*pParts								// 部品の情報のポインタ 
							= &g_aUi_operation_00Parts			//
							[UI_OPERATION_00_PARTS_KEYBOARD];	//
	VERTEX_2D				*pVtx;								// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_operation_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_operation_00(UI_OPERATION_00_PARTS_KEYBOARD));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].fWidth,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtn,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMin,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMax,
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nCounterAnim,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMaxX,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMaxY,
			NULL);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_operation_00.fAlpha) });
	}
}

//========================================
// UpdateUi_operation_00Parts_controller関数 - 部品(コントローラー)の更新処理 -
//========================================
void UpdateUi_operation_00Parts_controller(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_operation_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_operation_00(UI_OPERATION_00_PARTS_CONTROLLER));

	for (int nCntPolygon = 0; nCntPolygon < g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].fWidth,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtn,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMin,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMax,
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nCounterAnim,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMaxX,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMaxY,
			NULL);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_operation_00.fAlpha) });
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_operation_00->Unlock();
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcessUi_operation_00(void)
{
	Ui_operation_00	*pUi	// 操作説明[00] の情報のポインタ 
					= &g_ui_operation_00;

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_OPERATION_00_STATE_POP:

		break;
		//========== *** 出現中 ***
	case UI_OPERATION_00_STATE_IN_POP:

		// 状態カウンターを加算
		pUi->nCounterState++;

		// キーボードの説明の位置を更新
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].pos
			= (UI_OPERATION_00_KEYBOARD_START_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME)))
			+ (UI_OPERATION_00_KEYBOARD_END_POS * ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME));

		// コントローラーの説明の位置を更新
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].pos
			= (UI_OPERATION_00_CONTROLLER_START_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME)))
			+ (UI_OPERATION_00_CONTROLLER_END_POS * ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME));

		// 透明度を更新
		pUi->fAlpha = UI_OPERATION_00_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_OPERATION_00_IN_POP_TIME);

		if (pUi->nCounterState >= UI_OPERATION_00_IN_POP_TIME) 
		{// 状態カウンターが出現にかかる時間に達した時、
			// 出現状態にする
			SetStateUi_operation_00(UI_OPERATION_00_STATE_POP);

			// 透明度を更新
			pUi->fAlpha = UI_OPERATION_00_ALPHA_MAX;

			// 着地SE[000] を再生
			PlaySound(SOUND_LABEL_SE_LANDING_000);
		}
		
		break;
		//========== *** 消えている ***
	case UI_OPERATION_00_STATE_CLEAR:
		
		break;
		//========== *** 消え中 ***
	case UI_OPERATION_00_STATE_IN_CLEAR:

		// 状態カウンターを加算
		pUi->nCounterState++;

		// キーボードの説明の位置を更新
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].pos
			= (UI_OPERATION_00_KEYBOARD_START_POS * ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME))
			+ (UI_OPERATION_00_KEYBOARD_END_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME)));

		// コントローラーの説明の位置を更新
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].pos
			= (UI_OPERATION_00_CONTROLLER_START_POS * (((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME)))
			+ (UI_OPERATION_00_CONTROLLER_END_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME)));

		// 透明度を更新
		pUi->fAlpha = UI_OPERATION_00_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_OPERATION_00_IN_CLEAR_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateUi_operation_00(UI_OPERATION_00_STATE_CLEAR);

			// ゲームの状態を通常に設定
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}

		break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_operation_00(int nParts) 
{
	int nPolygonNum	// 必要ポリゴン数
		= 0;		//

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_operation_00Parts[nCntPolygon].nPolygonNum;
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
// InitParameterUi_operation_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterUi_operation_00(void)
{
	g_ui_operation_00.state				= UI_OPERATION_00_STATE_CLEAR;	// 状態
	g_ui_operation_00.nCounterState		= 0;							// 状態カウンター
	g_ui_operation_00.fAlpha			= 0.0f;							// 全体の透明度
}

//========================================
// InitUi_operation_00関数 - 操作説明[00] の初期化処理 -
//========================================
void InitUi_operation_00(void)
{
	// パラメーターの初期化処理
	InitParameterUi_operation_00();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_operation_00(UI_OPERATION_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_operation_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_operation_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aUi_operation_00Parts[nCntParts].aTexturePath, &g_aTextureUi_operation_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_operation_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_operation_00Parts[nCntParts].fWidth,
				g_aUi_operation_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx, 
				0, 
				g_aUi_operation_00Parts[nCntParts].nPtnMaxX, 
				g_aUi_operation_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_operation_00->Unlock();
}

//========================================
// UninitUi_operation_00関数 - 操作説明[00] の終了処理 -
//========================================
void UninitUi_operation_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_operation_00[nCntParts] != NULL)
		{
			g_aTextureUi_operation_00[nCntParts]->Release();
			g_aTextureUi_operation_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_operation_00 != NULL) 
	{
		g_pVtxBuffUi_operation_00->Release();
		g_pVtxBuffUi_operation_00 = NULL;
	}
}

//========================================
// UpdateUi_operation_00関数 - 操作説明[00] の更新処理 -
//========================================
void UpdateUi_operation_00(void) 
{
	Ui_operation_00	*pUi	// 操作説明[00] の情報のポインタ 
					= &g_ui_operation_00;

	// 状態処理
	StateProcessUi_operation_00();

	if (pUi->state == UI_OPERATION_00_STATE_CLEAR) 
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++) 
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** キーボードの説明 ***
		case UI_OPERATION_00_PARTS_KEYBOARD:

			// 部品(キーボードの説明)の更新処理
			UpdateUi_operation_00Parts_keyboard();

			break;
			//========== *** コントローラーの説明 ***
		case UI_OPERATION_00_PARTS_CONTROLLER:

			// 部品(コントローラーの説明)の更新処理
			UpdateUi_operation_00Parts_controller();

			break;
		}
	}
}

//========================================
// DrawUi_operation_00関数 - 操作説明[00] の描画処理 -
//========================================
void DrawUi_operation_00(void)
{
	Ui_operation_00	*pUi	// 操作説明[00] の情報のポインタ 
					= &g_ui_operation_00;

	if (pUi->state == UI_OPERATION_00_STATE_CLEAR)
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_operation_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	// ポリゴン番号
		= 0;

	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_operation_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_operation_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// 操作説明[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState関数 - 操作説明[00] の状態設定処理 -
//========================================
void SetStateUi_operation_00(UI_OPERATION_00_STATE state) 
{
	Ui_operation_00	*pUi	// 操作説明[00] の情報のポインタ 
					= &g_ui_operation_00;

	switch (state)
	{
		//========== *** 出現 ***
	case UI_OPERATION_00_STATE_POP:

		break;
		//========== *** 出現中 ***
	case UI_OPERATION_00_STATE_IN_POP:
		
		// 全ての部品のアニメーションのカウンターを初期化
		for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
		{
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_MAX].nCounterAnim = 0;
		}
		
		break;
		//========== *** 消えている ***
	case UI_OPERATION_00_STATE_CLEAR:

		break;
		//========== *** 消え中 ***
	case UI_OPERATION_00_STATE_IN_CLEAR:

		break;
	}

	// 状態を代入
	pUi->state = state;

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}