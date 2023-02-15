//========================================
// 
// タイトル画面[00]の処理
// Author:西村 吏功
// 
//========================================
// *** md_title_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "text.h"
#include "sound.h"
#include "setting.h"
#include "bg_color.h"
#include "bg_space_01.h"
#include "md_game_00.h"
#include "md_title_00.h"
#include "img_fighter_00.h"
#include "chr_fighter_00.h"
#include "ui_frame_02.h"
#include "ui_menu_00.h"
#include "ui_operation_00.h"
#include "ui_team_logo_00.h"
#include "ui_title_logo_00.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************

// タイトル画面[00] の初期の背景色
// タイトル画面[00] の通常時の背景色
// タイトル画面[00] のフラッシュ時の背景色
#define MD_TITLE_00_INIT_BG_COLOR	{0,0,0,255}
#define MD_TITLE_00_NORMAL_BG_COLOR	{36,36,56,255}
#define MD_TITLE_00_FLASH_BG_COLOR	{255,255,255,255}

// タイトル画面[00] のフラッシュにかかる時間
#define MD_TITLE_00_FLASH_TIME	(20)

// タイトル画面[00] のメインメニューの位置
// タイトル画面[00] の操作説明メニューの位置
// タイトル画面[00] のランキングメニューの位置
#define MD_TITLE_00_MAIN_MENU_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))
#define MD_TITLE_00_USER_GUIDE_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 32, 0.0f))
#define MD_TITLE_00_RANKING_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 32, 0.0f))

// ランキングテキストの基準位置
// ランキングテキストの配置間隔Y
// ランキングフレームの位置
#define RANKING_TEXT_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (PIXEL * 8 * 9), PIXEL * 180, 0.0f))
#define RANKING_TEXT_SPACE_Y	(PIXEL * 16)
#define RANKING_FRAME_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 8 * 7), 0.0f))

//****************************************
// 列挙型の定義
//****************************************

// タイトル画面[00] のメニュー
typedef enum
{
	MD_TITLE_00_MENU_MAIN = 0,		// メイン
	MD_TITLE_00_MENU_USER_GUIDE,	// 操作説明
	MD_TITLE_00_MENU_RANKING,		// ランキング
	MD_TITLE_00_MENU_MAX,
}MD_TITLE_00_MENU;

// タイトル画面[00] のメインメニュー
typedef enum
{
	MD_TITLE_00_MAIN_MENU_START = 0,	// 開始
	MD_TITLE_00_MAIN_MENU_USER_GUIDE,	// 操作説明
	MD_TITLE_00_MAIN_MENU_RANKING,		// ランキング
	MD_TITLE_00_MAIN_MENU_SETTINGS,		// 設定
	MD_TITLE_00_MAIN_MENU_EXIT,			// 終了
	MD_TITLE_00_MAIN_MENU_MAX
}MD_TITLE_00_MAIN_MENU;

// タイトル画面[00] の操作説明メニュー
typedef enum
{
	MD_TITLE_00_USER_GUIDE_MENU_OK = 0,	// 了解
	MD_TITLE_00_USER_GUIDE_MENU_MAX
}MD_TITLE_00_USER_GUIDE_MENU;

// タイトル画面[00] のランキングメニュー
typedef enum
{
	MD_TITLE_00_RANKING_MENU_OK = 0,	// 了解
	MD_TITLE_00_RANKING_MENU_MAX
}MD_TITLE_00_RANKING_MENU;

//****************************************
// プロトタイプ宣言
//****************************************

// タイトル画面[00] のパラメーターの初期化処理
void InitParameterMd_title_00(void);

// タイトル画面[00] の状態に応じた更新処理
void UpdateMd_title_00State(void);

// タイトル画面[00] の状態に応じた開始処理
void StartMd_title_00State(void);

// タイトル画面[00] の状態に応じた終了処理
void EndMd_title_00State(void);

//****************************************
// グローバル宣言
//****************************************
Md_title_00	g_md_title_00;	// タイトル画面[00] の情報

// メインメニューの設定情報
Ui_menu_00_set g_aUi_menu_00_set[MD_TITLE_00_MENU_MAX][MD_TITLE_00_MAIN_MENU_MAX] =
{
	{
		{ UI_MENU_00_TYPE_NORMAL	,"START" },
		{ UI_MENU_00_TYPE_NORMAL	,"USER GUIDE" },
		{ UI_MENU_00_TYPE_NORMAL	,"RANKING" },
		{ UI_MENU_00_TYPE_NORMAL	,"SETTINGS" },
		{ UI_MENU_00_TYPE_NORMAL	,"QUIT" },
	},
	{
		{ UI_MENU_00_TYPE_NORMAL	,"OK" },
	},
	{
		{ UI_MENU_00_TYPE_NORMAL	,"OK" },
	},
};

// タイトル画面[00] のテキストの情報
Text g_aMd_title_00Text[MD_TITLE_00_TEXT_MAX] =
{
	{ "(C) 2022 STUDIO.D0DO"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - (PIXEL * 12), 0.0f)						,{ 255, 255, 255, 255 } , FONT_000 },	// 著作権表示
	{ "CREDIT : %02d"			, D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + PIXEL * 8 * 20, SCREEN_HEIGHT - (PIXEL * 12), 0.0f)	,{ 231, 0  , 91 , 255 } , FONT_000 },	// クレジット
	{ " %s  %-3s %8d"			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)																,{ 255, 255, 255, 255 } , FONT_000 },	// ランキングスコア
};

//========== *** タイトル画面[00] の情報を取得 ***
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
// InitParameterMd_title_00関数 - タイトル画面[00] のパラメーターの初期化処理 -
//========================================
void InitParameterMd_title_00(void)
{
	g_md_title_00.bPlayBGM		= false;	// BGM再生フラグ
	g_md_title_00.bShowRanking	= false;	// ランキング表示フラグ
}

//========================================
// UpdateTxtMd_title_00関数 - テキスト内容の更新処理 -
//========================================
void UpdateTxtMd_title_00(void)
{
	char			aString					//
					[TXT_MAX];				// 文字列代入用
	Chr_fighter_00	*pChr					//
					= GetChr_fighter_00();	// 機体[00] の情報のポインタ

	//========== *** テキスト[著作権表示] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_TITLE_00_TEXT_COPYRIGHT,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].aDisplay,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].pos,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].col,
			TEXT_MODE_CENTER,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[クレジット] ***
	{
		sprintf(aString, g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].aDisplay, INIT_CREDIT_NUM);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_TITLE_00_TEXT_CREDIT,
			aString,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].pos,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].col,
			TEXT_MODE_CENTER,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ランキング] ***
	{
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			Sys_ranking_00	*pSys					//
							= GetSys_ranking_00();	// ランキング[00] の情報のポインタ
			char			aName					//
							[RANKING_NAME_NUM + 1];	// 名前
			D3DXVECTOR3		pos						//
							= RANKING_TEXT_POS;		// 位置

			// Y座標をランキングのカウントに応じてずらす
			pos.y += nCntRanking * RANKING_TEXT_SPACE_Y;

			sprintf(aString, g_aMd_title_00Text[MD_TITLE_00_TEXT_RANKING].aDisplay, GetRankText(nCntRanking), pSys[nCntRanking].aName, pSys[nCntRanking].nScore);
			UpdateTxt_00(
				TEXT_FIELD_UI,
				MD_TITLE_00_TEXT_RANKING + nCntRanking,
				aString,
				pos,
				g_aMd_title_00Text[MD_TITLE_00_TEXT_RANKING].col,
				TEXT_MODE_LEFT,
				g_aMd_title_00Text[MD_TITLE_00_TEXT_RANKING].font,
				1.0f,
				1.0f);
		}
	}
}

//========================================
// StartMd_title_00State関数 - タイトル画面[00] の状態に応じた開始処理 -
//========================================
void StartMd_title_00State(void)
{
	// 状態カウンターを初期化
	g_md_title_00.nCounterState = 0;

	switch (g_md_title_00.state)
	{
		//========== *** チームロゴ ***
	case MD_TITLE_00_STATE_TEAM_LOGO:
	{
		// 背景色を初期のものに設定
		SetBg_color(MD_TITLE_00_INIT_BG_COLOR, 0);

		// チームロゴ[00] を出現中にする
		SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_IN_POP);
	}
	break;
		//========== *** タイトルアニメーション ***
	case MD_TITLE_00_STATE_TITLE_ANIM:
	{
		// タイトルロゴ[00] をプラズマウォールの移動待ち状態にする
		SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT);
	}
		break;
		//========== *** フラッシュ ***
	case MD_TITLE_00_STATE_FLASH:
	{
		// 背景色をフラッシュ時のものに設定
		SetBg_color(MD_TITLE_00_FLASH_BG_COLOR, 0);

		// 背景色を通常時のものに推移設定
		SetBg_color(MD_TITLE_00_NORMAL_BG_COLOR, MD_TITLE_00_FLASH_TIME);
	}
		break;
		//========== *** 通常 ***
	case MD_TITLE_00_STATE_NORMAL:
	{
		// タイトルロゴ[00] を出現にする
		SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_POP);

		// 背景色を通常時のものに設定
		SetBg_color(MD_TITLE_00_NORMAL_BG_COLOR, 0);

		// チームロゴ[00] を消えている状態にする
		SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_CLEAR);

		if (!g_md_title_00.bPlayBGM) 
		{// BGM再生フラグが偽の時、
			// タイトルBGM[000] を再生
			PlaySound(SOUND_LABEL_BGM_TITLE_000);

			// BGM再生フラグを真にする
			g_md_title_00.bPlayBGM = true;
		}

		// メニューの中心座標を設定
		SetUi_menu_00Pos(MD_TITLE_00_MAIN_MENU_POS);

		// メニュー[00] の設定処理
		SetUi_menu_00(
			g_aUi_menu_00_set[MD_TITLE_00_MENU_MAIN],
			MD_TITLE_00_MAIN_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);

		// 選択番号を開始に設定
		*GetSelect() = MD_TITLE_00_MAIN_MENU_START;

		// テキスト[著作権表示] を表示
		// テキスト[クレジット] を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_COPYRIGHT, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_CREDIT, true);
	}
		break;
		//========== *** 操作説明 ***
	case MD_TITLE_00_STATE_USER_GUIDE:
	{
		// メニューの中心座標を設定
		SetUi_menu_00Pos(MD_TITLE_00_USER_GUIDE_MENU_POS);

		// メニュー[00] の設定処理
		SetUi_menu_00(
			g_aUi_menu_00_set[MD_TITLE_00_MENU_USER_GUIDE],
			MD_TITLE_00_USER_GUIDE_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);

		// 選択番号を了解に設定
		*GetSelect() = MD_TITLE_00_USER_GUIDE_MENU_OK;

		// 操作説明[00] を出現中の状態にする
		SetStateUi_operation_00(UI_OPERATION_00_STATE_IN_POP);
	}
		break;
		//========== *** ランキング ***
	case MD_TITLE_00_STATE_RANKING:
	{
		// メニューの中心座標を設定
		SetUi_menu_00Pos(MD_TITLE_00_RANKING_MENU_POS);

		// メニュー[00] の設定処理
		SetUi_menu_00(
			g_aUi_menu_00_set[MD_TITLE_00_MENU_RANKING],
			MD_TITLE_00_RANKING_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);

		// 選択番号を了解に設定
		*GetSelect() = MD_TITLE_00_RANKING_MENU_OK;

		// フレーム[02] の設定処理
		SetUi_frame_02(RANKING_FRAME_POS);
	}
		break;
		//========== *** 設定 ***
	case MD_TITLE_00_STATE_SETTING:
	{
		// 設定メニューの設定処理
		SetSettingMenu(MODE_TITLE_00);
	}
	break;
	}
}

//========================================
// EndMd_title_00State関数 - タイトル画面[00] の状態に応じた終了処理 -
//========================================
void EndMd_title_00State(void) 
{
	switch (g_md_title_00.state)
	{
		//========== *** チームロゴ ***
	case MD_TITLE_00_STATE_TEAM_LOGO:
	{

	}
	break;
		//========== *** タイトルアニメーション ***
	case MD_TITLE_00_STATE_TITLE_ANIM:
	{
		
	}
	break;
		//========== *** フラッシュ ***
	case MD_TITLE_00_STATE_FLASH:
	{

	}
		//========== *** 通常 ***
	case MD_TITLE_00_STATE_NORMAL:
	{
		// テキスト[著作権表示] を非表示
		// テキスト[クレジット] を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_COPYRIGHT, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_CREDIT, false);
	}
		break;
		//========== *** 操作説明 ***
	case MD_TITLE_00_STATE_USER_GUIDE:
	{
		// 操作説明[00] を消え中の状態にする
		SetStateUi_operation_00(UI_OPERATION_00_STATE_IN_CLEAR);
	}
		break;
		//========== *** ランキング ***
	case MD_TITLE_00_STATE_RANKING:
	{
		// テキスト[ランキング] を全て非表示する
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_RANKING + nCntRanking, false);
		}

		// 消え中にする
		SetStateUi_frame_02(UI_FRAME_02_STATE_IN_CLEAR);

		// ランキング表示フラグを偽にする
		g_md_title_00.bShowRanking = false;
	}
		break;
		//========== *** 設定 ***
	case MD_TITLE_00_STATE_SETTING:
	{
		
	}
		break;
	}
}

//========================================
// UpdateMd_title_00State関数 - タイトル画面[00] の状態に応じた更新処理 -
//========================================
void UpdateMd_title_00State(void)
{
	switch (g_md_title_00.state)
	{
	case MD_TITLE_00_STATE_TEAM_LOGO:
	case MD_TITLE_00_STATE_TITLE_ANIM:
	case MD_TITLE_00_STATE_FLASH:
		// 状態がチームロゴ/タイトルアニメーション/フラッシュのいずれかの時、

		if ((GetKeyboardTrigger(DIK_RETURN))
			|| 
			(GetButtonTrigger(BUTTON_A))
			||
			(GetButtonTrigger(BUTTON_START)))
		{// スキップの入力時、
			// 状態を通常に設定
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);
		}

		break;
	}

	switch (g_md_title_00.state)
	{
		//========== *** チームロゴ ***
	case MD_TITLE_00_STATE_TEAM_LOGO:
	{
		if (GetUi_team_logo_00()->state == UI_TEAM_LOGO_00_STATE_CLEAR) 
		{// チームロゴ[00] が消えている状態の時、
			// 状態をタイトルアニメーションに設定
			SetMd_title_00State(MD_TITLE_00_STATE_TITLE_ANIM);
		}
	}
	break;
		//========== *** タイトルアニメーション ***
	case MD_TITLE_00_STATE_TITLE_ANIM:
	{
		if (++g_md_title_00.nCounterState == MD_TITLE_00_TITLE_ANIM_TIME)
		{// 状態カウンターを加算した結果タイトルアニメーションにかかる時間に達した時、
			// 状態をフラッシュにする
			SetMd_title_00State(MD_TITLE_00_STATE_FLASH);
		}
	}
	break;
		//========== *** フラッシュ ***
	case MD_TITLE_00_STATE_FLASH:
	{
		if (++g_md_title_00.nCounterState == MD_TITLE_00_FLASH_TIME)
		{// 状態カウンターを加算した結果フラッシュにかかる時間に達した時、
			// 状態を通常にする
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);
		}
	}
		//========== *** 通常 ***
	case MD_TITLE_00_STATE_NORMAL:
	{
		// メニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** 開始 ***
		case MD_TITLE_00_MAIN_MENU_START:

			// 効果音を再生
			PlaySound(SOUND_LABEL_SE_DECITION_000);

			// 画面をゲーム画面[00] に設定
			SetFade(MODE_GAME_00);

			break;
			//========== *** 操作説明 ***
		case MD_TITLE_00_MAIN_MENU_USER_GUIDE:

			// 状態を操作説明に設定
			SetMd_title_00State(MD_TITLE_00_STATE_USER_GUIDE);

			break;
			//========== *** ランキング ***
		case MD_TITLE_00_MAIN_MENU_RANKING:

			// 状態をランキングに設定
			SetMd_title_00State(MD_TITLE_00_STATE_RANKING);

			break;
			//========== *** 設定 ***
		case MD_TITLE_00_MAIN_MENU_SETTINGS:

			// 状態を設定に設定
			SetMd_title_00State(MD_TITLE_00_STATE_SETTING);

			break;
			//========== *** 終了 ***
		case MD_TITLE_00_MAIN_MENU_EXIT:

			// ウインドウを破棄する
			DestroyWindow(*GetWindowHandle());

			break;
		}
	}
		break;
		//========== *** 操作説明 ***
	case MD_TITLE_00_STATE_USER_GUIDE:
	{
		// メニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** 了解 ***
		case MD_TITLE_00_USER_GUIDE_MENU_OK:

			// 状態を通常に設定
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);

			break;
		}
	}
		break;
		//========== *** ランキング ***
	case MD_TITLE_00_STATE_RANKING:
	{
		if ((GetUi_frame_02()->state == UI_FRAME_02_STATE_POP)
			&& (!g_md_title_00.bShowRanking))
		{// フレーム[02] の状態が出現中の時、かつランキング表示フラグが偽の時、
			// ランキング表示フラグを真にする
			g_md_title_00.bShowRanking = true;

			// テキスト[ランキング] を全て表示する
			for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
			{
				ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_RANKING + nCntRanking, true);
			}

			// 着地SE[000] を再生
			PlaySound(SOUND_LABEL_SE_LANDING_000);
		}

		// メニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** 了解 ***
		case MD_TITLE_00_USER_GUIDE_MENU_OK:

			// 状態を通常に設定
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);

			break;
		}
	}
		break;
		//========== *** 設定 ***
	case MD_TITLE_00_STATE_SETTING:
	{
		// 設定メニューの更新処理
		UpdateSettingMenu();
	}
		break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_title_00関数 - タイトル画面[00]の初期化処理 -
//========================================
void InitMd_title_00(void)
{
	// パラメーターの初期化処理
	InitParameterMd_title_00();

	// 初期化処理
	InitTxt_00();			// テキスト
	InitBg_space_01();		// BG ;宇宙			[01]
	InitImg_fighter_00();	// IMG:機体			[00]
	InitUi_frame_02();		// UI :フレーム		[00]
	InitUi_menu_00();		// UI :メニュー		[00]
	InitUi_operation_00();	// UI :操作説明		[00]
	InitUi_team_logo_00();	// UI :チームロゴ	[00]
	InitUi_title_logo_00();	// UI :タイトルロゴ	[00]

	// 状態をチームロゴに設定
	SetMd_title_00State(MD_TITLE_00_STATE_TEAM_LOGO);
}

//========================================
// UninitMd_title_00関数 - タイトル画面[00]の終了処理 -
//========================================
void UninitMd_title_00(void)
{
	// 終了処理
	UninitTxt_00();				// テキスト
	UninitBg_space_01();		// BG ;宇宙			[01]
	UninitImg_fighter_00();		// IMG:機体			[00]
	UninitUi_frame_02();		// UI :フレーム		[00]
	UninitUi_menu_00();			// UI :メニュー		[00]
	UninitUi_operation_00();	// UI :操作説明		[00]
	UninitUi_team_logo_00();	// UI :チームロゴ	[00]
	UninitUi_title_logo_00();	// UI :タイトルロゴ	[00]
}

//========================================
// UpdateMd_title_00関数 - タイトル画面[00]の更新処理 -
//========================================
void UpdateMd_title_00(void)
{
	// 更新処理
	UpdateBg_space_01();		// BG ;宇宙			[01]
	UpdateImg_fighter_00();		// IMG:機体			[00]
	UpdateUi_frame_02();		// UI :フレーム		[00]
	UpdateUi_menu_00();			// UI :メニュー		[00]
	UpdateUi_operation_00();	// UI :操作説明		[00]
	UpdateUi_team_logo_00();	// UI :チームロゴ	[00]
	UpdateUi_title_logo_00();	// UI :タイトルロゴ	[00]

	// タイトル画面[00] の状態に応じた更新処理
	UpdateMd_title_00State();

	// テキスト内容の更新処理
	UpdateTxtMd_title_00();
}

//========================================
// DrawMd_title_00関数 - タイトル画面[00]の描画処理 -
//========================================
void DrawMd_title_00(void)
{
	// 描画処理
	DrawBg_space_01();		// BG ;宇宙			[01]
	DrawImg_fighter_00();	// IMG:機体			[00]
	DrawUi_frame_02();		// UI :フレーム		[00]
	DrawUi_menu_00();		// UI :メニュー		[00]
	DrawTxt_00();			// テキスト
	DrawUi_operation_00();	// UI :操作説明		[00]
	DrawUi_title_logo_00();	// UI :タイトルロゴ	[00]
	DrawUi_team_logo_00();	// UI :チームロゴ	[00]
}

//========================================
// SetMd_title_00State関数 - タイトル画面[00] の状態を設定 -
//========================================
void SetMd_title_00State(MD_TITLE_00_STATE state)
{
	// タイトル画面[00] の状態に応じた終了処理
	EndMd_title_00State();

	// タイトル画面[00] の状態を代入
	g_md_title_00.state = state;

	// タイトル画面[00] の状態に応じた開始処理
	StartMd_title_00State();
}