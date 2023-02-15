//========================================
// 
// UI:入力ガイド[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_input-guid_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "color.h"
#include "input.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:プレイヤー			[00]
#include "ui_dialog-box_00.h"	// UI :ダイアログボックス	[00]
#include "ui_input-guid_00.h"	// UI :入力ガイド			[00]
#include "ui_item-guid_00.h"	// UI :商品ガイド			[00]

//****************************************
// マクロ定義
//****************************************
// UI:入力ガイド[00] の位置(購入時)
// UI:入力ガイド[00] の位置(スキップ時)
// UI:入力ガイド[00] のテキストの相対位置(購入時)
// UI:入力ガイド[00] のテキストの相対位置(スキップ時)
// UI:入力ガイド[00] のダイアログボックスとの相対位置
#define UI_INPUTGUID_00_POS_BUY						(D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*33)+(PIXEL*6*3),(SCREEN_HEIGHT*0.5f)-(PIXEL*14)+(PIXEL*8*4),0.0f))
#define UI_INPUTGUID_00_POS_SKIP					(D3DXVECTOR3((SCREEN_WIDTH*0.5f),(SCREEN_HEIGHT*0.5f)+(PIXEL*12),0.0f))
#define UI_INPUTGUID_00_TEXT_RELATIVE_POS_BUY		(D3DXVECTOR3(PIXEL*-24,0.0f,0.0f))
#define UI_INPUTGUID_00_TEXT_RELATIVE_POS_SKIP		(D3DXVECTOR3(0.0f,PIXEL*-16,0.0f))
#define UI_INPUTGUID_00_RELATIVE_POS_WITH_DIALOGBOX	(D3DXVECTOR3(PIXEL*113,PIXEL*21,0.0f))

// UI:入力ガイド[00] のアニメーションにかかる時間
// UI:入力ガイド[00] のテキストの点滅カウンター
#define UI_INPUTGUID_00_ANIM_TIME			(20)
#define UI_INPUTGUID_00_TEXT_BRINK_COLOR	Color{243,189,63,255}

//****************************************
// 列挙型の定義
//****************************************
// UI:入力ガイド[00] の部品番号
typedef enum
{
	UI_INPUTGUID_00_PARTS_BUTTON,	// ボタン
	UI_INPUTGUID_00_PARTS_MAX,
}UI_INPUTGUID_00_PARTS;

// UI:入力ガイド[00] のテクスチャ番号
typedef enum 
{
	UI_INPUTGUID_00_TEXTURE_BUTTON_ENTER,	// ボタン(ENTER)
	UI_INPUTGUID_00_TEXTURE_BUTTON_A,		// ボタン(A)
	UI_INPUTGUID_00_TEXTURE_MAX,
}UI_INPUTGUID_00_TEXTURE;

//****************************************
// 構造体の定義
//****************************************
// UI:入力ガイド[00] の部品毎の情報構造体
typedef struct
{
	float	fWidth;		// 幅
	float	fHeight;	// 高さ
}Ui_inputGuid_00Parts;

// UI:入力ガイド[00] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath[TXT_MAX];	// テクスチャファイルの相対パス
	int		nPtnWidth;				// パターン幅
	int		nPtnHeight;				// パターン高さ
}Ui_inputGuid_00Texture;

// UI:入力ガイド[00] の管理情報構造体
typedef struct
{
	int						nPattern;		// パターンNo.
	int						nCounterAnim;	// アニメーションカウンター
	UI_INPUTGUID_00_TEXTURE btnTex;			// ボタンのテクスチャ
	D3DXVECTOR3				setPos;			// 設定位置
}Ui_inputGuid_00Control;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:入力ガイド[00] の管理情報のパラメーターを初期化
void InitParameterUi_inputGuid_00Control(void);

// UI:入力ガイド[00] の部品毎の頂点座標設定処理
void UpdatePartsUi_inputGuid_00(VERTEX_2D *pVtx, UI_INPUTGUID_00_PARTS parts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_inputGuid_00	// テクスチャへのポインタ
						[UI_INPUTGUID_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_inputGuid_00	// 頂点バッファへのポインタ
						= NULL;
Ui_inputGuid_00Control	g_ui_inputGuid_00Control;	// UI:入力ガイド[00] の管理情報

// UI:入力ガイド[00] のテクスチャ毎の情報
const Ui_inputGuid_00Texture g_aUi_inputGuid_00Texture[UI_INPUTGUID_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_button-enter_000.png" , 2, 1 },	// ボタン(ENTER)
	{ "data\\TEXTURE\\UserInterface\\ui_button-a_000.png"     , 2, 1 },	// ボタン(A)
};

// UI:入力ガイド[00] の部品毎の情報
const Ui_inputGuid_00Parts g_aUi_inputGuid_00Parts[UI_INPUTGUID_00_PARTS_MAX] =
{
	{ PIXEL * 16, PIXEL * 16 },	// ボタン
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_inputGuid_00関数 - UI:入力ガイド[00] の部品毎の頂点座標設定処理 -
//========================================
void UpdatePartsUi_inputGuid_00(VERTEX_2D *pVtx, UI_INPUTGUID_00_PARTS parts) 
{
	Ui_inputGuid_00Control	*pUiCtl	// UI:入力ガイド[00] の管理情報のポインタ
							= &g_ui_inputGuid_00Control;

	switch (parts)
	{
	case /*/ ボタン /*/UI_INPUTGUID_00_PARTS_BUTTON: {
		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			pUiCtl->setPos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_inputGuid_00Parts[parts].fWidth,
			g_aUi_inputGuid_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			Count(
				&pUiCtl->nPattern,
				0,
				(g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnWidth * g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnHeight) - 1,
				&pUiCtl->nCounterAnim,
				UI_INPUTGUID_00_ANIM_TIME,
				COUNT_TYPE_NORMAL),
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnWidth,
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnHeight,
			NULL);
	}
	break;
	}
}

//========================================
// InitParameterUi_inputGuid_00Control関数 - UI:入力ガイド[00] の管理情報のパラメーターを初期化 -
//========================================
void InitParameterUi_inputGuid_00Control(void) 
{
	Ui_inputGuid_00Control	*pUiCtl	// UI:入力ガイド[00] の管理情報のポインタ
							= &g_ui_inputGuid_00Control;

	pUiCtl->nPattern		= 0;									// パターンNo.
	pUiCtl->nCounterAnim	= 0;									// アニメーションカウンター
	pUiCtl->btnTex			= UI_INPUTGUID_00_TEXTURE_BUTTON_ENTER;	// ボタンのテクスチャ
	pUiCtl->setPos			= INITD3DXVECTOR3;						// 設定位置
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_inputGuid_00関数 - UI:入力ガイド[00] の初期化処理 -
//========================================
void InitUi_inputGuid_00(void)
{
	// 管理情報のパラメーターの初期化
	InitParameterUi_inputGuid_00Control();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_INPUTGUID_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_inputGuid_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_inputGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < UI_INPUTGUID_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_inputGuid_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_inputGuid_00[nCntTexture]);
	}

	Ui_inputGuid_00Control	*pUiCtl	// UI:入力ガイド[00] の管理情報のポインタ
							= &g_ui_inputGuid_00Control;

	for (int nCntParts = 0; nCntParts < UI_INPUTGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			0,
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnWidth,
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnHeight,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_inputGuid_00->Unlock();
}

//========================================
// UninitUi_inputGuid_00関数 - UI:入力ガイド[00] の終了処理 -
//========================================
void UninitUi_inputGuid_00(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < UI_INPUTGUID_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_inputGuid_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_inputGuid_00[nCntTexture]->Release();
			g_aTextureUi_inputGuid_00[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_inputGuid_00 != NULL) 
	{
		g_pVtxBuffUi_inputGuid_00->Release();
		g_pVtxBuffUi_inputGuid_00 = NULL;
	}
}

//========================================
// UpdateUi_inputGuid_00関数 - UI:入力ガイド[00] の更新処理 -
//========================================
void UpdateUi_inputGuid_00(void) 
{
	Ui_inputGuid_00Control	*pUiCtl	// UI:入力ガイド[00] の管理情報のポインタ
							= &g_ui_inputGuid_00Control;

	if (GetActiveInputType() == ACTIVE_INPUT_TYPE_KEYBOARD) 
	{// キーボードが動的な時、テクスチャをボタン(ENTER)に設定
		pUiCtl->btnTex = UI_INPUTGUID_00_TEXTURE_BUTTON_ENTER;
	}
	else if (GetActiveInputType() == ACTIVE_INPUT_TYPE_CONTROLLER)
	{// コントローラーが動的な時、テクスチャをボタン(A)に設定
		pUiCtl->btnTex = UI_INPUTGUID_00_TEXTURE_BUTTON_A;
	}

	float	fAlpha;	// 不透明度

	if (GetUi_dialogBox_00()->state == UI_DIALOGBOX_00_STATE_POP) 
	{// UI:ダイアログボックス[00] が出現している状態の時、
		fAlpha = 1.0f;	// 不透明度を設定

		// 設定位置をUI:ダイアログボックス[00]との相対位置で設定
		pUiCtl->setPos = UI_DIALOGBOX_00_POS + UI_INPUTGUID_00_RELATIVE_POS_WITH_DIALOGBOX;
	}
	else if (GetChr_player_00()->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00].bHit) 
	{// CHR:プレイヤー[00] のOBJ:スイッチ[00] との当たりフラグが真の時、
		fAlpha = 1.0f;	// 不透明度を設定

		// 設定位置をスキップ時のものに設定
		pUiCtl->setPos = UI_INPUTGUID_00_POS_SKIP;

		// テキストの設定
		SetText2D(
			"ARE YOU READY?",
			FONT_002, DISPLAY_CENTER,
			UI_INPUTGUID_00_POS_SKIP + UI_INPUTGUID_00_TEXT_RELATIVE_POS_SKIP,
			!pUiCtl->nPattern ?
			Color{ 255,255,255,(int)(255 * fAlpha) } :
			ColorMix(UI_INPUTGUID_00_TEXT_BRINK_COLOR, { 255,255,255,(int)(255 * fAlpha) }), 1.0f, 1.0f);
	}
	else
	{// CHR:プレイヤー[00] のOBJ:スイッチ[00] との当たりフラグが偽 & OBJ:台座[00] との時、
		// UI:商品ガイドの到着カウンターに応じて不透明度を設定
		fAlpha = (float)GetUi_itemGuid_00()->nCounterArrival / (float)UI_ITEMGUID_00_ARRIVAL_TIME;

		// 設定位置を購入時のものに設定
		pUiCtl->setPos = UI_INPUTGUID_00_POS_BUY;

		// テキストの設定
		SetText2D(
			"BUY?",
			FONT_002, DISPLAY_CENTER,
			UI_INPUTGUID_00_POS_BUY + UI_INPUTGUID_00_TEXT_RELATIVE_POS_BUY,
			!pUiCtl->nPattern ?
			Color{ 255,255,255,(int)(255 * fAlpha) } :
			ColorMix(UI_INPUTGUID_00_TEXT_BRINK_COLOR, { 255,255,255,(int)(255 * fAlpha) }), 1.0f, 1.0f);
	}

	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_inputGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_INPUTGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * fAlpha) });

		// 部品毎の更新処理
		UpdatePartsUi_inputGuid_00(pVtx, (UI_INPUTGUID_00_PARTS)nCntParts);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_inputGuid_00->Unlock();
}

//========================================
// DrawUi_inputGuid_00関数 - UI:入力ガイド[00] の描画処理 -
//========================================
void DrawUi_inputGuid_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_inputGuid_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Ui_inputGuid_00Control	*pUiCtl	// UI:入力ガイド[00] の管理情報のポインタ
							= &g_ui_inputGuid_00Control;

	for (int nCntParts = 0; nCntParts < UI_INPUTGUID_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_inputGuid_00[pUiCtl->btnTex]);

		// UI:入力ガイド[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}