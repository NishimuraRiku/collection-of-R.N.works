//========================================
// 
// MD:タイトル画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_title_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "setting.h"
#include "sound.h"
#include "text.h"
#include "bg_mountain_00.h"			// BG :山					[00]
#include "bg_space_00.h"			// BG :宇宙					[00]
#include "md_title_00.h"			// MD :タイトル画面			[00]
#include "obj_stage_00.h"			// OBJ:ステージ				[00]
#include "ui_menu_00.h"				// UI :メニュー				[00]
#include "ui_ranking-frame_00.h"	// UI :ランキングフレーム	[00]
#include "ui_title-logo_00.h"		// UI :タイトルロゴ			[00]
#include "ui_user-guid_00.h"		// UI :ユーザーガイド		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// タイトル画面[00] のメインメニューの位置
// タイトル画面[00] のユーザーガイドメニューの位置
// タイトル画面[00] のユーザーガイドの位置
// タイトル画面[00] のランキングメニューの位置
// タイトル画面[00] のランキングの位置
// タイトル画面[00] の設定メニューの位置
#define MD_TITLE_00_MAIN_MENU_POS		D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*64,0.0f)
#define MD_TITLE_00_USERGUID_MENU_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*32,0.0f)
#define MD_TITLE_00_USERGUID_POS		D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT*0.5f,0.0f)
#define MD_TITLE_00_RANKING_MENU_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*32,0.0f)
#define MD_TITLE_00_RANKING_POS			D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT*0.5f,0.0f)
#define MD_TITLE_00_SETTING_MENU_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*64,0.0f)

// タイトル画面[00] の著作権の位置
// タイトル画面[00] の著作権の表示にかかる時間
#define MD_TITLE_00_COPYRIGHT_POS		D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_HEIGHT-PIXEL*8,0.0f)
#define MD_TITLE_00_COPYRIGHT_SHOW_TIME	(10)

// タイトル画面[00] の決定SE
#define MD_TITLE_00_DETERMINATION_SE	(SOUND_LABEL_SE_DETERMINATION_000)

//****************************************
// 列挙型の定義
//****************************************
// タイトル画面[00] のメインメニュー
typedef enum
{
	MD_TITLE_00_MAIN_MENU_START,	// 開始
	MD_TITLE_00_MAIN_MENU_USERGUID,	// ユーザーガイド
	MD_TITLE_00_MAIN_MENU_RANKING,	// ランキング
	MD_TITLE_00_MAIN_MENU_SETTINGS,	// 設定
	MD_TITLE_00_MAIN_MENU_EXIT,		// 終了
	MD_TITLE_00_MAIN_MENU_MAX,
}MD_TITLE_00_MAIN_MENU;

// タイトル画面[00] のユーザーガイドメニュー
typedef enum
{
	MD_TITLE_00_USERGUID_MENU_OK,	// OK
	MD_TITLE_00_USERGUID_MENU_MAX,
}MD_TITLE_00_USERGUID_MENU;

// タイトル画面[00] のランキングメニュー
typedef enum
{
	MD_TITLE_00_RANKING_MENU_OK,	// OK
	MD_TITLE_00_RANKING_MENU_MAX,
}MD_TITLE_00_RANKING_MENU;

//****************************************
// プロトタイプ宣言
//****************************************
// MD:タイトル画面[00] のパラメーターの初期化処理
// MD:タイトル画面[00] の状態に応じた更新処理
// MD:タイトル画面[00] の状態に応じた開始処理
// MD:タイトル画面[00] の状態に応じた終了処理
void InitParameterMd_title_00(void);
void UpdateMd_title_00State(void);
void StartMd_title_00State(void);
void EndMd_title_00State(void);

//****************************************
// グローバル宣言
//****************************************
Md_title_00	g_md_title_00;	// MD:タイトル画面[00] の情報

// MD:タイトル画面[00] のメインメニュー設定情報
Ui_menu_00Set g_aMd_title_00MainMenuSet[MD_TITLE_00_MAIN_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"START"    ,false },
	{ UI_MENU_00_TYPE_NORMAL,"USER GUID",true  },
	{ UI_MENU_00_TYPE_NORMAL,"RANKING"  ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"SETTINGS" ,true },
	{ UI_MENU_00_TYPE_NORMAL,"QUIT"     ,false },
};

// MD:タイトル画面[00] のユーザーガイドメニュー設定情報
Ui_menu_00Set g_aMd_title_00UserGuidSet[MD_TITLE_00_USERGUID_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"OK",true },
};

// MD:タイトル画面[00] のランキングメニュー設定情報
Ui_menu_00Set g_aMd_title_00RankingMenuSet[MD_TITLE_00_RANKING_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"OK",true },
};

//========== *** MD:タイトル画面[00] の情報を取得 ***
Md_title_00 *GetMd_title_00(void) 
{
	return &g_md_title_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterMd_title_00関数 - MD:タイトル画面[00] のパラメーターの初期化処理 -
//========================================
void InitParameterMd_title_00(void)
{
	Md_title_00	*pMd =	// MD:タイトル画面[00] の情報
				&g_md_title_00;

	pMd->state				= MD_TITLE_00_STATE_NONE;	// 状態
	pMd->nCounterState		= 0;						// 状態カウンター
	pMd->bShowCopyRight		= false;					// 著作権表示フラグ
	pMd->nCounterCopyRight	= 0;						// 著作権表示カウンター
}

//========================================
// StartMd_title_00State関数 - MD:タイトル画面[00] の状態に応じた開始処理 -
//========================================
void StartMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	// 状態カウンターを初期化
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TITLE_00_STATE_NORMAL: {
		// UI:メニュー[00] の中心座標を設定
		SetUi_menu_00Pos(MD_TITLE_00_MAIN_MENU_POS);

		// UI:メニュー[00] の設定処理(メインメニュー)
		SetUi_menu_00(
			g_aMd_title_00MainMenuSet,
			MD_TITLE_00_MAIN_MENU_MAX);

		// 著作権表示フラグを真にする
		pMd->bShowCopyRight = true;

		break;
	}
	case /*/ ユーザーガイド /*/MD_TITLE_00_STATE_USERGUID: {
		// UI:メニュー[00] の中心座標を設定
		SetUi_menu_00Pos(MD_TITLE_00_USERGUID_MENU_POS);

		// UI:メニュー[00] の設定処理(ユーザーガイドメニュー)
		SetUi_menu_00(
			g_aMd_title_00RankingMenuSet,
			MD_TITLE_00_USERGUID_MENU_MAX);

		break;
	}
	case /*/ ランキング /*/MD_TITLE_00_STATE_RANKING: {
		// UI:メニュー[00] の中心座標を設定
		SetUi_menu_00Pos(MD_TITLE_00_RANKING_MENU_POS);

		// UI:メニュー[00] の設定処理(ランキングメニュー)
		SetUi_menu_00(
			g_aMd_title_00RankingMenuSet,
			MD_TITLE_00_RANKING_MENU_MAX);

		break;
	}
	}
}

//========================================
// EndMd_title_00State関数 - MD:タイトル画面[00] の状態に応じた終了処理 -
//========================================
void EndMd_title_00State(void) 
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TITLE_00_STATE_NORMAL: {
		break;
	}
	case /*/ ユーザーガイド /*/MD_TITLE_00_STATE_USERGUID: {
		break;
	}
	case /*/ ランキング /*/MD_TITLE_00_STATE_RANKING: {
		break;
	}
	}
}

//========================================
// UpdateMd_title_00State関数 - MD:タイトル画面[00] の状態に応じた更新処理 -
//========================================
void UpdateMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TITLE_00_STATE_NORMAL: {
		// メインメニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ 開始 /*/MD_TITLE_00_MAIN_MENU_START: {
			// 画面をゲーム画面[00] に設定
			SetFade(MODE_GAME_00);

			// 決定SEを再生
			PlaySound(MD_TITLE_00_DETERMINATION_SE);

			// 著作権表示フラグを偽にする
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ ユーザーガイド /*/MD_TITLE_00_MAIN_MENU_USERGUID: {
			// UI:ユーザーガイド[00] の設定処理
			SetUi_userGuid_00(MD_TITLE_00_USERGUID_POS, GetActiveInputType());

			// 状態をユーザーガイドにする
			SetMd_title_00State(MD_TITLE_00_STATE_USERGUID);

			// 著作権表示フラグを偽にする
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ ランキング /*/MD_TITLE_00_MAIN_MENU_RANKING: {
			// UI:ランキングフレーム[00] の設定処理
			SetUi_rankingFrame_00(MD_TITLE_00_RANKING_POS);

			// 著作権表示フラグを偽にする
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ 設定 /*/MD_TITLE_00_MAIN_MENU_SETTINGS: {
			// UI:メニュー[00] の中心座標を設定
			SetUi_menu_00Pos(MD_TITLE_00_SETTING_MENU_POS);

			// 設定メニューの設定処理
			SetSettingMenu();

			// 状態を設定にする
			SetMd_title_00State(MD_TITLE_00_STATE_SETTINGS);

			// 著作権表示フラグを偽にする
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ 終了 /*/MD_TITLE_00_MAIN_MENU_EXIT: {
			// ウインドウを破棄する
			DestroyWindow(*GetWindowHandle());
			break;
		}
		}

		if (GetUi_rankingFrame_00()->state == UI_RANKINGFRAME_00_STATE_POP) 
		{// UI:ランキングフレーム[00] の状態が出現の時、
			SetMd_title_00State(MD_TITLE_00_STATE_RANKING);	// 状態をランキングにする
		}

		break;
	}
	case /*/ ユーザーガイド /*/MD_TITLE_00_STATE_USERGUID: {
		// ユーザーガイドメニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ OK /*/MD_TITLE_00_USERGUID_MENU_OK: {
			// UI:ユーザーガイド[00] の状態を消え中にする
			SetStateUi_userGuid_00(UI_USERGUID_00_STATE_IN_CLEAR);

			break;
		}
		}

		if (GetUi_userGuid_00()->state == UI_USERGUID_00_STATE_CLEAR)
		{// UI:ユーザーガイド[00] の状態が消えている時、
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// 状態を通常にする
		}

		break;
	}
	case /*/ ランキング /*/MD_TITLE_00_STATE_RANKING: {
		// ランキングメニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ OK /*/MD_TITLE_00_RANKING_MENU_OK: {
			// UI:ランキングフレーム[00] の状態を消え中にする
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_IN_CLEAR);

			break;
		}
		}

		if (GetUi_rankingFrame_00()->state == UI_RANKINGFRAME_00_STATE_CLEAR)
		{// UI:ランキングフレーム[00] の状態が消えている時、
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// 状態を通常にする
		}

		break;
	}
	case /*/ 設定 /*/MD_TITLE_00_STATE_SETTINGS: {
		// 設定メニューの更新処理
		UpdateSettingMenu();

		if (!*GetSettingNow())
		{// 設定中フラグが偽の時、
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// 状態を通常にする
		}

		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_title_00関数 - MD:タイトル画面[00]の初期化処理 -
//========================================
void InitMd_title_00(void)
{
	// パラメーターの初期化処理
	InitParameterMd_title_00();

	// 初期化処理
	InitParameterCamera3D();	// カメラ(3D)のパラメーター
	InitBg_mountain_00();		// BG :山					[00]
	InitBg_space_00();			// BG :宇宙					[00]
	InitObj_stage_00();			// OBJ:ステージ				[00]
	InitUi_menu_00();			// UI :メニュー				[00]
	InitUi_rankingFrame_00();	// UI :ランキングフレーム	[00]
	InitUi_titleLogo_00();		// UI :タイトルロゴ			[00]
	InitUi_userGuid_00();		// UI :ユーザーガイド		[00]

	// 状態を通常に設定
	SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);

	// カメラ(3D)の注視点をOBJ:ステージ[00] に設定
	SetCamera3DPosR(
		GetObj_stage_00()->pos +
		D3DXVECTOR3(0.0f, MD_TITLE_00_CAMERA3D_POS_V_HEIGHT, 0.0f));

	// カメラ(3D)の高さ/距離を設定
	GetCamera3D()->fHeight = MD_TITLE_00_CAMERA3D_POS_R_HEIGHT;
	GetCamera3D()->fLength = MD_TITLE_00_CAMERA3D_LENGTH;

	// タイトルBGMを再生
	PlaySound(SOUND_LABEL_BGM_TITLE_000);
}

//========================================
// UninitMd_title_00関数 - MD:タイトル画面[00]の終了処理 -
//========================================
void UninitMd_title_00(void)
{
	// 終了処理
	UninitBg_mountain_00();		// BG :山					[00]
	UninitBg_space_00();		// BG :宇宙					[00]
	UninitObj_stage_00();		// OBJ:ステージ				[00]
	UninitUi_menu_00();			// UI :メニュー				[00]
	UninitUi_rankingFrame_00();	// UI :ランキングフレーム	[00]
	UninitUi_titleLogo_00();	// UI :タイトルロゴ			[00]
	UninitUi_userGuid_00();		// UI :ユーザーガイド		[00]
}

//========================================
// UpdateMd_title_00関数 - MD:タイトル画面[00]の更新処理 -
//========================================
void UpdateMd_title_00(void)
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	// MD:タイトル画面[00] の状態に応じた更新処理
	UpdateMd_title_00State();

	// 更新処理
	UpdateBg_mountain_00();		// BG :山					[00]
	UpdateBg_space_00();		// BG :宇宙					[00]
	UpdateObj_stage_00();		// OBJ:ステージ				[00]
	UpdateUi_menu_00();			// UI :メニュー				[00]
	UpdateUi_rankingFrame_00();	// UI :ランキングフレーム	[00]
	UpdateUi_titleLogo_00();	// UI :タイトルロゴ			[00]
	UpdateUi_userGuid_00();		// UI :ユーザーガイド		[00]

	if (pMd->bShowCopyRight) {		// 著作権表示フラグが真の時、
		pMd->nCounterCopyRight++;	// 著作権表示カウンターを加算
	}
	else {							// 著作権表示フラグが偽の時、
		pMd->nCounterCopyRight--;	// 著作権表示カウンターを減算
	}
	// 著作権表示カウンターを制御
	IntControl(&pMd->nCounterCopyRight, MD_TITLE_00_COPYRIGHT_SHOW_TIME, 0);
	{
		float	fRate	// カウンターの進行率
				= (float)pMd->nCounterCopyRight / (float)MD_TITLE_00_COPYRIGHT_SHOW_TIME;

		// 著作権表示のテキスト設定
		SetText2D(
			"(C) 2022 STUDIO.D0DO",
			FONT_002,
			DISPLAY_CENTER,
			MD_TITLE_00_COPYRIGHT_POS,
			{ 255,255,255,(int)(255 * fRate) },
			fRate,
			fRate);
	}
}

//========================================
// DrawMd_title_00関数 - MD:タイトル画面[00]の描画処理 -
//========================================
void DrawMd_title_00(void)
{
	// 描画処理
	DrawBg_mountain_00();		// BG :山					[00]
	DrawBg_space_00();			// BG :宇宙					[00]
	DrawObj_stage_00();			// OBJ:ステージ				[00]
	DrawUi_menu_00();			// UI :メニュー				[00]
	DrawUi_rankingFrame_00();	// UI :ランキングフレーム	[00]
	DrawUi_titleLogo_00();		// UI :タイトルロゴ			[00]
	DrawUi_userGuid_00();		// UI :ユーザーガイド		[00]
	DrawText_();				// テキスト
}

//========================================
// SetMd_title_00State関数 - MD:タイトル画面[00] の状態を設定 -
//========================================
void SetMd_title_00State(MD_TITLE_00_STATE state)
{
	// MD:タイトル画面[00] の状態に応じた終了処理
	EndMd_title_00State();

	// MD:タイトル画面[00] の状態を代入
	g_md_title_00.state = state;

	// MD:タイトル画面[00] の状態に応じた開始処理
	StartMd_title_00State();
}