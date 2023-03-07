//========================================
// 
// UI:メニューコメント[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_menu-comment_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "md_title_00.h"		// UI:タイトル画面		[00]
#include "ui_menu_00.h"			// UI:メニュー			[00]
#include "ui_menu-comment_00.h"	// UI:メニューコメント	[00]

//****************************************
// マクロ定義
//****************************************
// UI:メニューコメント[00] の幅
#define UI_MENUCOMMENT_00_WIDTH (PIXEL*128)
// UI:メニューコメント[00] の高さ
#define UI_MENUCOMMENT_00_HEIGHT (PIXEL*16)
// UI:メニューコメント[00] の開始相対位置
#define UI_MENUCOMMENT_00_START_RELATIVE_POS (D3DXVECTOR3(PIXEL*64,0.0f,0.0f))
// UI:メニューコメント[00] の終了相対位置
#define UI_MENUCOMMENT_00_END_RELATIVE_POS (D3DXVECTOR3(PIXEL*128,0.0f,0.0f))
// UI:メニューコメント[00] のスライドにかかる時間
#define UI_MENUCOMMENT_00_SLIDE_TIME (10)

//****************************************
// 構造体の定義
//****************************************
// UI:メニューコメント[00] の情報構造体
typedef struct 
{
	int nCounterSlide;	// スライドカウンター
	int nSelectTemp;	// 選択番号を保存
}Ui_menuComment_00;

//****************************************
// グローバル宣言
//****************************************
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_aTextureUi_menuComment_00[MD_TITLE_00_MAIN_MENU_MAX] = {};
// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_menuComment_00 = NULL;
Ui_menuComment_00 g_ui_menuComment_00;	// UI:メニューコメント[00] の情報構造体

// テクスチャの相対パス
const char g_aTexturePathUi_menuComment_00[MD_TITLE_00_MAIN_MENU_MAX][TXT_MAX] = {
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\00.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\01.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\02.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\03.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\04.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\05.png",
};

// 配置間隔X
const float g_aSpaceXUi_menuComment_00[MD_TITLE_00_MAIN_MENU_MAX] = {
	PIXEL*-16,
	PIXEL*-8,
	PIXEL*-24,
	PIXEL*-16,
	PIXEL*-40,
	PIXEL*-32,
};// 12 13 11 12 8 10

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_menuComment_00関数 - UI:メニューコメント[00] の初期化処理 -
//========================================
void InitUi_menuComment_00(void)
{
	// スライドカウンターを初期化
	g_ui_menuComment_00.nCounterSlide = 0;
	// 選択番号の保存を初期化
	g_ui_menuComment_00.nSelectTemp = 0;

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntMenu = 0; nCntMenu < MD_TITLE_00_MAIN_MENU_MAX; nCntMenu++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aTexturePathUi_menuComment_00[nCntMenu], &g_aTextureUi_menuComment_00[nCntMenu]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_menuComment_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_menuComment_00->Lock(0, 0, (void**)&pVtx, 0);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, INITCOLOR);

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_menuComment_00->Unlock();
}

//========================================
// UninitUi_menuComment_00関数 - UI:メニューコメント[00] の終了処理 -
//========================================
void UninitUi_menuComment_00(void)
{
	// テクスチャの破棄
	for (int nCntMenu = 0; nCntMenu < MD_TITLE_00_MAIN_MENU_MAX; nCntMenu++)
	{
		if (g_aTextureUi_menuComment_00[nCntMenu] != NULL)
		{
			g_aTextureUi_menuComment_00[nCntMenu]->Release();
			g_aTextureUi_menuComment_00[nCntMenu] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_menuComment_00 != NULL) 
	{
		g_pVtxBuffUi_menuComment_00->Release();
		g_pVtxBuffUi_menuComment_00 = NULL;
	}
}

//========================================
// UpdateUi_menuComment_00関数 - UI:メニューコメント[00] の更新処理 -
//========================================
void UpdateUi_menuComment_00(void)
{
	if ((GetMode() != MODE_TITLE_00) || (GetMd_title_00()->state != MD_TITLE_00_STATE_NORMAL))
	{// モードがタイトル画面でない or 状態が通常でない時、
		return;	// 処理を終了する
	}

	// スライドカウンターを加算
	g_ui_menuComment_00.nCounterSlide++;

	// スライドカウンターの値を制御
	IntControl(&g_ui_menuComment_00.nCounterSlide, UI_MENUCOMMENT_00_SLIDE_TIME, 0);

	// 選択番号が切り替わった時、スライドカウンターを初期化
	g_ui_menuComment_00.nCounterSlide *= !(GetUi_menu_00Control()->nSelect != g_ui_menuComment_00.nSelectTemp);

	// 選択番号を保存
	g_ui_menuComment_00.nSelectTemp = GetUi_menu_00Control()->nSelect;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_menuComment_00->Lock(0, 0, (void**)&pVtx, 0);

	float fRate = (float)g_ui_menuComment_00.nCounterSlide / (float)UI_MENUCOMMENT_00_SLIDE_TIME;
	D3DXVECTOR3 addPos =
		(UI_MENUCOMMENT_00_START_RELATIVE_POS * (1.0f - fRate)) +
		(UI_MENUCOMMENT_00_END_RELATIVE_POS * fRate);
	addPos.x += g_aSpaceXUi_menuComment_00[GetUi_menu_00Control()->nSelect];

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		GetUi_menu_00()[GetUi_menu_00Control()->nSelect].pos + addPos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_MENUCOMMENT_00_WIDTH,
		UI_MENUCOMMENT_00_HEIGHT * GetUi_menu_00Control()->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_menuComment_00->Unlock();
}

//========================================
// DrawUi_menuComment_00関数 - UI:メニューコメント[00] の描画処理 -
//========================================
void DrawUi_menuComment_00(void)
{
	if ((GetMode() != MODE_TITLE_00) || (GetMd_title_00()->state != MD_TITLE_00_STATE_NORMAL))
	{// モードがタイトル画面でない or 状態が通常でない時、
		return;	// 処理を終了する
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_menuComment_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureUi_menuComment_00[GetUi_menu_00Control()->nSelect]);

	// UI:メニューコメント[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}