//========================================
// 
// UI:インベントリ[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_inventory_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "main.h"
#include "input.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "ui_inventory_00.h"	// UI :インベントリ	[00]

//****************************************
// マクロ定義
//****************************************
// UI:インベントリ[00] の幅
// UI:インベントリ[00] の高さ
// UI:インベントリ[00] の配置間隔X
// UI:インベントリ[00] の基準位置
#define UI_INVENTORY_00_WIDTH		(PIXEL * 32)
#define UI_INVENTORY_00_HEIGHT		(PIXEL * 32)
#define UI_INVENTORY_00_SPACE_X		(PIXEL * 32)
#define UI_INVENTORY_00_BASE_POS	(D3DXVECTOR3(BUFFER_WIDTH * 0.5f,BUFFER_HEIGHT - (PIXEL*21),0.0f))

// UI:インベントリ[00] の選択時の色
// UI:インベントリ[00] の非選択時の色
#define UI_INVENTORY_00_SELECTED_COLOR		{255,255,255,255}
#define UI_INVENTORY_00_UNSELECTED_COLOR	{100,100,100,200}

// UI:インベントリ[00] のカウントテキストの相対位置
// UI:インベントリ[00] のカウントテキストの選択時の色
// UI:インベントリ[00] のカウントテキストの非選択時の色
#define UI_INVENTORY_00_COUNT_TEXT_RELATIVE_POS			(D3DXVECTOR3(0.0f,PIXEL*12,0.0f))
#define UI_INVENTORY_00_COUNT_TEXT_SELECTED_COLOR		{255,255,255,255}
#define UI_INVENTORY_00_COUNT_TEXT_UNSELECTED_COLOR		{100,100,100,255}

// UI:インベントリ[00] の選択時の移動にかかる時間
// UI:インベントリ[00] の選択時の移動量Y
#define UI_INVENTORY_00_SELECTED_MOVE_TIME	(5)
#define UI_INVENTORY_00_SELECTED_MOVE_Y		(PIXEL*-4)

// UI:インベントリ[00] のアイテム名テキストの相対位置
#define UI_INVENTORY_00_ITEMNAME_TEXT_RELATIVE_POS	D3DXVECTOR3(0.0f,PIXEL * -25,0.0f)

// UI:インベントリ[00] の左選択テキストの相対位置
// UI:インベントリ[00] の右選択テキストの相対位置
// UI:インベントリ[00] の入力結果テキストの相対位置
#define UI_INVENTORY_00_LEFTSELECT_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL * -42,PIXEL * -25,0.0f)
#define UI_INVENTORY_00_RIGHTSELECT_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL * 42,PIXEL * -25,0.0f)
#define UI_INVENTORY_00_INPUTRESULT_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL * 30,PIXEL *6,0.0f)

//****************************************
// 構造体の定義
//****************************************
// UI:インベントリ[00] の情報構造体
typedef struct 
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置

	// 状態関連
	int	nCounterSelect;	// 選択カウンター
}Ui_inventory_00;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_inventory_00	// テクスチャへのポインタ
						[CHR_PLAYER_00_ITEM_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_inventory_00	// 頂点バッファへのポインタ
						= NULL;
Ui_inventory_00			g_aUi_inventory_00			// UI:インベントリ[00] の情報
						[CHR_PLAYER_00_ITEM_MAX];

// UI:インベントリ[00] のテクスチャの相対パス
const char g_aTexturePathUi_inventory_00[CHR_PLAYER_00_ITEM_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\sword.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\gun.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\block.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\turret.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\reflector.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\discharger.png",
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_inventory_00関数 - UI:インベントリ[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_inventory_00(Ui_inventory_00 *pUi)
{
	pUi->nCounterSelect = 0;	// 選択カウンター
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_inventory_00関数 - UI:インベントリ[00] の初期化処理 -
//========================================
void InitUi_inventory_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイスを取得
						= GetDevice();
	VERTEX_2D			*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_PLAYER_00_ITEM_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_inventory_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_inventory_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < CHR_PLAYER_00_ITEM_MAX; nCntUi++, pVtx += 4)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aTexturePathUi_inventory_00[nCntUi], &g_aTextureUi_inventory_00[nCntUi]);

		// 頂点座標を設定
		{
			D3DXVECTOR3 setPos = UI_INVENTORY_00_BASE_POS;	// 設定位置を設定
			setPos.x = setPos.x - (CHR_PLAYER_00_ITEM_MAX * UI_INVENTORY_00_SPACE_X * 0.5f) + (((float)nCntUi + 0.5f) * UI_INVENTORY_00_SPACE_X);
			SetVertexPos2D(pVtx,
				setPos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				UI_INVENTORY_00_WIDTH,
				UI_INVENTORY_00_HEIGHT,
				ANGLE_TYPE_FIXED);
		}

		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			0,
			1,
			1,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_inventory_00->Unlock();
}

//========================================
// UninitUi_inventory_00関数 - UI:インベントリ[00] の終了処理 -
//========================================
void UninitUi_inventory_00(void)
{
	// テクスチャの破棄
	for (int nCntUi = 0; nCntUi < CHR_PLAYER_00_ITEM_MAX; nCntUi++)
	{
		if (g_aTextureUi_inventory_00[nCntUi] != NULL)
		{
			g_aTextureUi_inventory_00[nCntUi]->Release();
			g_aTextureUi_inventory_00[nCntUi] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_inventory_00 != NULL)
	{
		g_pVtxBuffUi_inventory_00->Release();
		g_pVtxBuffUi_inventory_00 = NULL;
	}
}

//========================================
// UpdateUi_inventory_00関数 - UI:インベントリ[00] の更新処理 -
//========================================
void UpdateUi_inventory_00(void)
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_inventory_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_inventory_00	*pUi	// UI:インベントリ[00] の情報のポインタ
					= g_aUi_inventory_00;

	for (int nCntItem = 0; nCntItem < CHR_PLAYER_00_ITEM_MAX; nCntItem++, pVtx += 4, pUi++)
	{
		// 選択アイテム番号と一致しているかに応じて選択カウンターを加算or減算
		nCntItem == GetChr_player_00()->nSelectItem ?
			pUi->nCounterSelect++ :
			pUi->nCounterSelect--;

		// 選択カウンターの制御
		IntControl(&pUi->nCounterSelect, UI_INVENTORY_00_SELECTED_MOVE_TIME, 0);

		// 頂点座標を設定
		{
			D3DXVECTOR3 setPos = UI_INVENTORY_00_BASE_POS;	// 設定位置を設定
			setPos.x = setPos.x - (CHR_PLAYER_00_ITEM_MAX * UI_INVENTORY_00_SPACE_X * 0.5f) + (((float)nCntItem + 0.5f) * UI_INVENTORY_00_SPACE_X);
			setPos.y += UI_INVENTORY_00_SELECTED_MOVE_Y * ((float)pUi->nCounterSelect / (float)(UI_INVENTORY_00_SELECTED_MOVE_TIME));
			SetVertexPos2D(pVtx,
				setPos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				UI_INVENTORY_00_WIDTH,
				UI_INVENTORY_00_HEIGHT,
				ANGLE_TYPE_FIXED);

			if(GetChr_player_00Item()[nCntItem].bCount)
			{// アイテムのカウントフラグが真の時、
				// カウントテキスト表示
				char aString[TXT_MAX];
				sprintf(aString, "*%02d", GetChr_player_00()->aItemCount[nCntItem]);
				Color SetColor;
				nCntItem == GetChr_player_00()->nSelectItem ?
					SetColor = UI_INVENTORY_00_COUNT_TEXT_SELECTED_COLOR :
					SetColor = UI_INVENTORY_00_COUNT_TEXT_UNSELECTED_COLOR;
				SetText2D(aString,
					FONT_002,
					DISPLAY_LEFT,
					setPos + UI_INVENTORY_00_COUNT_TEXT_RELATIVE_POS,
					SetColor,
					1.0f, 1.0f);
			}
		}

		// 頂点カラーの設定
		{
			Color setColor;	// 設定色を選択アイテム番号と一致しているかに応じて設定
			nCntItem == GetChr_player_00()->nSelectItem ?
				setColor = UI_INVENTORY_00_SELECTED_COLOR :
				setColor = UI_INVENTORY_00_UNSELECTED_COLOR;

			SetVertexColor2D(pVtx, setColor);
		}

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			0,
			1,
			1,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_inventory_00->Unlock();

	// アイテム名テキスト表示
	{
		char aString[TXT_MAX];
		sprintf(aString, GetChr_player_00Item()[GetChr_player_00()->nSelectItem].aName);
		Color SetColor = { 255,255,255,255 };
		SetText2D(
			aString,
			FONT_002,
			DISPLAY_CENTER,
			UI_INVENTORY_00_BASE_POS + UI_INVENTORY_00_ITEMNAME_TEXT_RELATIVE_POS,
			SetColor,
			1.0f, 1.0f);
	}

	// 左選択テキスト表示
	{
		char aString[TXT_MAX];
		if (GetActiveInputType() == ACTIVE_INPUT_TYPE_KEYBOARD)
		{// 動的な入力の種類がキーボードの時、
			sprintf(aString, "<[Q]");
		}
		else if (GetActiveInputType() == ACTIVE_INPUT_TYPE_CONTROLLER)
		{// 動的な入力の種類がコントローラーの時、
			sprintf(aString, "<[LB]");
		}
		Color SetColor = { 255,255,255,255 };
		SetText2D(
			aString,
			FONT_002,
			DISPLAY_RIGHT,
			UI_INVENTORY_00_BASE_POS + UI_INVENTORY_00_LEFTSELECT_TEXT_RELATIVE_POS,
			SetColor,
			1.0f, 1.0f);
	}

	// 右選択テキスト表示
	{
		char aString[TXT_MAX];
		if (GetActiveInputType() == ACTIVE_INPUT_TYPE_KEYBOARD)
		{// 動的な入力の種類がキーボードの時、
			sprintf(aString, "[E]>");
		}
		else if (GetActiveInputType() == ACTIVE_INPUT_TYPE_CONTROLLER)
		{// 動的な入力の種類がコントローラーの時、
			sprintf(aString, "[RB]>");
		}
		Color SetColor = { 255,255,255,255 };
		SetText2D(
			aString,
			FONT_002,
			DISPLAY_LEFT,
			UI_INVENTORY_00_BASE_POS + UI_INVENTORY_00_RIGHTSELECT_TEXT_RELATIVE_POS,
			SetColor,
			1.0f, 1.0f);
	}
}

//========================================
// DrawUi_inventory_00関数 - UI:インベントリ[00] の描画処理 -
//========================================
void DrawUi_inventory_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// デバイスを取得
						= GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_inventory_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUi = 0; nCntUi < CHR_PLAYER_00_ITEM_MAX; nCntUi++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_inventory_00[nCntUi]);

		// UI:インベントリ[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);
	}
}