//========================================
// 
// チームロゴ[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_team_logo_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_team_logo_00.h"

//****************************************
// マクロ定義
//****************************************

// チームロゴ[00] の出現にかかる時間
// チームロゴ[00] の出現する時間
// チームロゴ[00] の消えるのにかかる時間
// チームロゴ[00] の透明度の最大値
#define UI_TEAM_LOGO_00_IN_POP_TIME		(40)
#define UI_TEAM_LOGO_00_POP_TIME		(80)
#define UI_TEAM_LOGO_00_IN_CLEAR_TIME	(40)
#define UI_TEAM_LOGO_00_ALPHA_MAX		(1.0f)

// チームロゴ[00] の位置
#define UI_TEAM_LOGO_00_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))

//****************************************
// 列挙型の定義
//****************************************

// チームロゴ[00] の部品番号
typedef enum
{
	UI_TEAM_LOGO_00_PARTS_TEAM_LOGO = 0,		// チームロゴ
	UI_TEAM_LOGO_00_PARTS_MAX,
}UI_TEAM_LOGO_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// チームロゴ[00] のポリゴン毎の情報構造体
typedef struct
{
	char	aTexturePath	// テクスチャファイルの相対パス
			[TXT_MAX];		// 
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	int		nPolygonNum;	// 必要ポリゴン数
}Ui_team_logo_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterUi_team_logo_00(void);

// 部品(チームロゴ)の更新処理
void UpdateUi_team_logo_00Parts_team_logo(void);

// 状態処理
void StateProcessUi_team_logo_00(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_team_logo_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_team_logo_00	// テクスチャへのポインタ
						[UI_TEAM_LOGO_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_team_logo_00	// 頂点バッファへのポインタ
						= NULL;						//
Ui_team_logo_00			g_ui_team_logo_00;			// チームロゴ[00] の情報

// チームロゴ[00] の部品毎の情報
Ui_team_logo_00Parts g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\tmlg_STUDIO.D0DO.png", PIXEL * 512, PIXEL * 128, 1 },
};

//========== *** チームロゴ[00] の情報を取得 ***
Ui_team_logo_00 *GetUi_team_logo_00(void) 
{
	return &g_ui_team_logo_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_team_logo_00Parts_team_logo関数 - 部品(チームロゴ)の更新処理 -
//========================================
void UpdateUi_team_logo_00Parts_team_logo(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_team_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_team_logo_00(UI_TEAM_LOGO_00_PARTS_TEAM_LOGO));

	for (int nCntPolygon = 0; nCntPolygon < g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_TEAM_LOGO].nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_TEAM_LOGO_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_TEAM_LOGO].fWidth,
			g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_TEAM_LOGO].fHeight,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_team_logo_00.fAlpha) });
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_team_logo_00->Unlock();
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcessUi_team_logo_00(void)
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// チームロゴ[00] の情報のポインタ 

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_TEAM_LOGO_00_STATE_POP:

		// 状態カウンターを加算
		pUi->nCounterState++;

		if (pUi->nCounterState >= UI_TEAM_LOGO_00_POP_TIME)
		{// 状態カウンターが出現する時間に達した時、
			// 消え中状態にする
			SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_IN_CLEAR);
		}
		
		break;
		//========== *** 出現中 ***
	case UI_TEAM_LOGO_00_STATE_IN_POP:

		// 状態カウンターを加算
		pUi->nCounterState++;

		// 透明度を更新
		pUi->fAlpha = UI_TEAM_LOGO_00_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_TEAM_LOGO_00_IN_POP_TIME);

		if (pUi->nCounterState >= UI_TEAM_LOGO_00_IN_POP_TIME) 
		{// 状態カウンターが出現にかかる時間に達した時、
			// 出現状態にする
			SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_POP);

			// 透明度を更新
			pUi->fAlpha = UI_TEAM_LOGO_00_ALPHA_MAX;
		}
		
		break;
		//========== *** 消えている ***
	case UI_TEAM_LOGO_00_STATE_CLEAR:
		
		break;
		//========== *** 消え中 ***
	case UI_TEAM_LOGO_00_STATE_IN_CLEAR:

		// 状態カウンターを加算
		pUi->nCounterState++;

		// 透明度を更新
		pUi->fAlpha = UI_TEAM_LOGO_00_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_TEAM_LOGO_00_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_TEAM_LOGO_00_IN_CLEAR_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_CLEAR);

			// ゲームの状態を通常に設定
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}

		break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_team_logo_00(int nParts) 
{
	int nPolygonNum	// 必要ポリゴン数
		= 0;		//

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_team_logo_00Parts[nCntPolygon].nPolygonNum;
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
// InitParameterUi_team_logo_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterUi_team_logo_00(void)
{
	g_ui_team_logo_00.state				= UI_TEAM_LOGO_00_STATE_CLEAR;	// 状態
	g_ui_team_logo_00.nCounterState		= 0;							// 状態カウンター
	g_ui_team_logo_00.fAlpha			= 0.0f;							// 全体の透明度
}

//========================================
// InitUi_team_logo_00関数 - チームロゴ[00] の初期化処理 -
//========================================
void InitUi_team_logo_00(void)
{
	// パラメーターの初期化処理
	InitParameterUi_team_logo_00();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_team_logo_00(UI_TEAM_LOGO_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_team_logo_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_team_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aUi_team_logo_00Parts[nCntParts].aTexturePath, &g_aTextureUi_team_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_team_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_team_logo_00Parts[nCntParts].fWidth,
				g_aUi_team_logo_00Parts[nCntParts].fHeight,
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
	g_pVtxBuffUi_team_logo_00->Unlock();
}

//========================================
// UninitUi_team_logo_00関数 - チームロゴ[00] の終了処理 -
//=====================================d===
void UninitUi_team_logo_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_team_logo_00[nCntParts] != NULL)
		{
			g_aTextureUi_team_logo_00[nCntParts]->Release();
			g_aTextureUi_team_logo_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_team_logo_00 != NULL) 
	{
		g_pVtxBuffUi_team_logo_00->Release();
		g_pVtxBuffUi_team_logo_00 = NULL;
	}
}

//========================================
// UpdateUi_team_logo_00関数 - チームロゴ[00] の更新処理 -
//========================================
void UpdateUi_team_logo_00(void) 
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// チームロゴ[00] の情報のポインタ 

	// 状態処理
	StateProcessUi_team_logo_00();

	if (pUi->state == UI_TEAM_LOGO_00_STATE_CLEAR) 
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++) 
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** チームロゴ ***
		case UI_TEAM_LOGO_00_PARTS_TEAM_LOGO:

			// 部品(チームロゴ)の更新処理
			UpdateUi_team_logo_00Parts_team_logo();

			break;
		}
	}
}

//========================================
// DrawUi_team_logo_00関数 - チームロゴ[00] の描画処理 -
//========================================
void DrawUi_team_logo_00(void)
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// チームロゴ[00] の情報のポインタ 

	if (pUi->state == UI_TEAM_LOGO_00_STATE_CLEAR)
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_team_logo_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	// ポリゴン番号
		= 0;

	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_team_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_team_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// チームロゴ[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState関数 - チームロゴ[00] の状態設定処理 -
//========================================
void SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE state) 
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// チームロゴ[00] の情報のポインタ 

	// 状態を代入
	pUi->state = state;

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}