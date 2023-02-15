//========================================
// 
// ゲーム画面[00]の処理
// Author:西村 吏功
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "setting.h"
#include "sound.h"
#include "text.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "atk_explosion_01.h"
#include "bg_space_00.h"
#include "chr_enemy_00.h"
#include "chr_fighter_00.h"
#include "eff_general_00.h"
#include "eff_laser_pointer_00.h"
#include "eff_light_00.h"
#include "eff_particle_00.h"
#include "eff_screen_00.h"
#include "md_game_00.h"
#include "item_bomb_00.h"
#include "item_medal_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include "sys_ranking_00.h"
#include "sys_stage_00.h"
#include "ui_area_gage_00.h"
#include "ui_arrow_00.h"
#include "ui_bomb_00.h"
#include "ui_bomb_get_00.h"
#include "ui_frame_01.h"
#include "ui_frame_02.h"
#include "ui_life_00.h"
#include "ui_life_bonus_00.h"
#include "ui_menu_00.h"
#include "ui_score_00.h"
#include "ui_warning_00.h"
#include <stdio.h>

//****************************************
// マクロ定義 
//****************************************

// 隠しコマンドの有無
#define SECRET_COMMAND	(true)

// 出現待ちの時間
// 死亡後の余韻の時間
#define POP_WAIT_TIME		(120)
#define DIED_AFTERGLOW_TIME	(120)

// ウェーブ待ちの時間
// ウェーブ待ちの余韻
// ウェーブテキストの中間までの移動にかかる時間
// ウェーブテキストの終了までの移動を開始する時間
// ウェーブテキストの初期の位置
// ウェーブテキストの中間の位置
// ウェーブテキストの最後の位置
// ウェーブテキストの大きさにかかる倍率
// ウェーブテキストの色
#define WAVE_WAIT_TIME						(60)
#define WAVE_WAIT_AFTERGLOW					(20)
#define WAVE_TEXT_INTERMEDIATE_MOVE_TIME	(20)
#define WAVE_TEXT_END_MOVE_START_TIME		(40)
#define WAVE_TEXT_FIRST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_INTERMEDIATE_POS			(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_LAST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_SCALE						(ScaleDiameter{1.6f, 1.6f})
#define WAVE_TEXT_COLOR						(Color{255, 255, 255, 255})

// スタート待ちの時間
// スタートテキストの位置
// スタートテキストの中間までにかかる時間
// スタートテキストの終了までの推移を開始する時間
// スタートテキストの初期の大きさにかかる倍率
// スタートテキストの中間の大きさにかかる倍率
// スタートテキストの最後の大きさにかかる倍率
// スタートテキストの色
#define START_WAIT_TIME					(60)
#define START_TEXT_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define START_TEXT_INTERMEDIATE_TIME	(20)
#define START_TEXT_END_START_TIME		(40)
#define START_TEXT_FIRST_SCALE			(ScaleDiameter{8.0f, 8.0f})
#define START_TEXT_INTERMEDIATE_SCALE	(ScaleDiameter{2.0f, 2.0f})
#define START_TEXT_LAST_SCALE			(ScaleDiameter{8.0f, 0.0f})
#define START_TEXT_COLOR				(Color{243, 191, 63, 255})

// リザルトスコアの加算にかかる時間
// リザルトスコアの加算回数
// リザルトスコアの表示にかかる時間
// リザルトスコアの表示完了時の色
#define RESULT_SCORE_ADD_TIME			(3)
#define RESULT_SCORE_ADD_NUM			(17)
#define RESULT_SCORE_SHOW_TIME			(8)
#define RESULT_SCORE_SHOW_COMP_COLOR	(Color{243, 191, 63, 255})

// ランキングテキストの開始位置
// ランキングテキストの終了位置
// ランキングテキストの移動にかかる時間
// ランキングテキストの配置間隔X
// ランキングテキストの点滅色
// ランキングテキストの点滅にかかる時間
// ランキングフレームの位置
#define RANKING_TEXT_FIRST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.4f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_LAST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_MOVE_TIME		(10)
#define RANKING_TEXT_SPACE_Y		(PIXEL * 16)
#define RANKING_TEXT_BRINK_COLOR	(Color{243, 191, 63, 255})
#define RANKING_TEXT_BRINK_TIME		(4)
#define RANKING_FRAME_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 16), 0.0f))

// ポーズテキストの点滅にかかる時間
#define PAUSE_TEXT_BRINK_TIME	(16)

// ポーズメニューの位置
// リザルトメニューの位置
#define MD_GAME_00_PAUSE_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 80, 0.0f))
#define MD_GAME_00_RESULT_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))

// コンティニューのカウント回数
// コンティニューのカウントにかかる時間
// コンティニューの余韻にかかる時間
// コンティニュー押下催促の点滅にかかる時間
#define CONTINUE_COUNT_NUM			(10)
#define CONTINUE_COUNT_TIME			(60)
#define CONTINUE_AFTERGLOW_TIME		(60)
#define PUSH_CONTINUE_BRINK_TIME	(30)

// ゲームクリアにかかる時間
// ゲームオーバーにかかる時間
// ゲームクリアのテキストにかかる時間
// ゲームオーバーのテキストにかかる時間
#define GAME_CLEAR_TIME				(300)
#define GAME_OVER_TIME				(300)
#define GAME_CLEAR_TEXT_TIME		(12)
#define GAME_OVER_TEXT_TIME			(12)

// ノーダメージボーナススコア
// ノーダメージボーナススコアの被弾毎の減少量
#define NO_DAMAGE_BONUS_SCORE			(100000)
#define NO_DAMAGE_BONUS_SCORE_DECREASE	(20000)

// 矢印の相対位置
#define ALLOW_RELATIVE_POSITION	(D3DXVECTOR3(PIXEL * 8 * 6.5f,PIXEL * 0,0.0f))

//****************************************
// 列挙型の定義
//****************************************

// タイトル画面[00] のリザルトメニュー
typedef enum
{
	MD_GAME_00_RESULT_MENU_RETRY = 0,		// リトライ
	MD_GAME_00_RESULT_MENU_BACK_TO_TITLE,	// タイトルに戻る
	MD_GAME_00_RESULT_MENU_MAX,
}MD_GAME_00_RESULT_MENU;

// タイトル画面[00] のポーズメニュー
typedef enum
{
	MD_GAME_00_PAUSE_MENU_RESUME = 0,		// 再開
	MD_GAME_00_PAUSE_MENU_RETRY,			// リトライ
	MD_GAME_00_PAUSE_MENU_SETTING,			// 設定
	MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE,	// タイトルに戻る
	MD_GAME_00_PAUSE_MENU_MAX,
}MD_GAME_00_PAUSE_MENU;

//****************************************
// 構造体の定義
//****************************************

// 合図テキストの情報構造体
typedef struct 
{
	char			aString				// 
					[TXT_MAX];			// 文字列
	D3DXVECTOR3		pos;				// 位置
	ScaleDiameter	scaleDiameter;		// 大きさにかかる倍率
	Color			col;				// 色
}SignalText;

// ランキングテキストの情報構造体
typedef struct 
{
	D3DXVECTOR3 pos;			// 位置
	int			nCounterMove;	// 移動カウンター
	Color		col;			// 色
}RankingText;

//****************************************
// プロトタイプ宣言
//****************************************

// テキスト内容の更新処理
void UpdateTxtMd_game_00(void);

// ゲームの状態に応じた開始処理
void StartGameState(MD_GAME_00_STATE state);

// ゲームの状態に応じた終了処理
void EndGameState(MD_GAME_00_STATE state);

// ゲームの状態に応じた更新処理
void UpdateGameState(void);

//****************************************
// グローバル宣言
//****************************************
MD_GAME_00_STATE	g_gameState					//
					= MD_GAME_00_STATE_NONE;	// ゲームの状態
int					g_nStateCounter;			// 状態のカウンター
SignalText			g_signalText;				// 合図テキストの情報
int					g_nCounterReScoreAdd;		// リザルトスコアの加算カウンター
int					g_nCntReScoreAdd;			// リザルトスコアの加算数のカウント
int					g_nCounterShow;				// 表示カウンター
bool				g_bBrink;					// 点滅フラグ
int					g_nCounterBrink;			// 点滅カウンター
RankingText			g_aRankingText				//
					[RANKING_NUM];				// ランキングの位置
int					g_nUpdateRank;				// 更新した順位
int					g_nShowRanking;				// 表示した順位
int					g_nCredit;					// クレジット
int					g_nCntContinueEnd;			// コンティニュー終了までのカウント
bool				g_bContinue;				// コンティニューフラグ
int					g_nCntInputName;			// 名前入力のカウント
int					g_nCntText;					// テキストのカウント
bool				g_bResultMenu;				// リザルトメニューの出現フラグ

// リザルトのメニュー情報
Ui_menu_00_set md_game_00_result_menu_set[MD_GAME_00_RESULT_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RETRY" },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE" }
};

// ポーズのメニュー情報
Ui_menu_00_set md_game_00_pause_menu_set[MD_GAME_00_PAUSE_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RESUME" },
	{ UI_MENU_00_TYPE_NORMAL,"RETRY" },
	{ UI_MENU_00_TYPE_NORMAL,"SETTINGS" },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE" }
};

// ゲーム画面[00] のテキストの情報
Text g_aMd_game_00Text[MD_GAME_00_TEXT_MAX] =
{
	{ "SCORE: %08d"			, D3DXVECTOR3(SCREEN_WIDTH - (PIXEL * 8 * 28), PIXEL * 20, 0.0f)				, {255, 255, 255, 255} , FONT_000 },	// スコア
	{ ""					, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_001 },	// 合図
	{ "- P A U S E -"		, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {63 , 191, 255, 255} , FONT_000 },	// ポーズ
	{ "C O N T I N U E ?"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - PIXEL * 64, 0.0f)	, {63 , 191, 255, 255} , FONT_000 },	// コンティニュー
	{ "%d"					, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {63 , 191, 255, 255} , FONT_004 },	// コンティニューのカウント
	{ ""					, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + PIXEL * 80, 0.0f)	, {231, 0  , 91 , 255} , FONT_000 },	// コンティニュー押下
	{ "CREDIT : %02d"		, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + PIXEL * 64, 0.0f)	, {63 , 191, 255, 255} , FONT_000 },	// クレジット
	{ "SCORE %08d"			, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, PIXEL * 100, 0.0f)							, {255, 255, 255, 255} , FONT_001 },	// リザルトスコア
	{ "LIFE BONUS +%d"		, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_000 },	// ライフボーナス
	{ "LIFE:"				, D3DXVECTOR3(PIXEL * 8 * 13, PIXEL * 20, 0.0f)									, {255, 255, 255, 255} , FONT_000 },	// ライフ
	{ "BOMB:"				, D3DXVECTOR3(PIXEL * 8 * 26, PIXEL * 20, 0.0f)									, {255, 255, 255, 255} , FONT_000 },	// ボム
	{ "MAX!"				, D3DXVECTOR3(PIXEL * 8 * (32 + CHR_FIGHTER_00_BOMB_MAX), PIXEL * 20, 0.0f)		, {243, 191, 63 , 255} , FONT_002 },	// ボム最大
	{ "BOMB UP!"			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_002 },	// ボム獲得
	{ "AREA TIME: %2d.%1d"	, D3DXVECTOR3(PIXEL * 8 * 11, (SCREEN_HEIGHT - PIXEL * 20), 0.0f)				, {255, 255, 255, 255} , FONT_000 },	// エリア時間
	{ "STAGE CLEAR"			, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {243, 191, 63 , 255} , FONT_001 },	// ゲームクリア
	{ "GAME OVER"			, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {88 , 248, 152, 255} , FONT_001 },	// ゲームオーバー
	{ "      %-3s"			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {243, 191, 63 , 255} , FONT_000 },	// ランキング名
	{ " %s  %-3s %8d"		, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_000 },	// ランキングスコア
};

// コンティニューの入力催促テキスト
char aContinueButtonText[ACTIVE_INPUT_TYPE_MAX][TXT_MAX] = 
{
	"- PUSH ENTER KEY -",
	"- PUSH START BUTTON -",
};

//========== *** ゲーム画面[00]のテキストの情報を取得 ***
Text GetMd_game_00Text(MD_GAME_00_TEXT text)
{
	return g_aMd_game_00Text[text];
}

//========== *** ゲームの状態の取得	***
MD_GAME_00_STATE GetGameState(void) 
{
	return g_gameState;
}

//========== *** クレジットの取得 ***
int *GetCredit(void)
{
	return &g_nCredit;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateTxtMd_game_00関数 - テキスト内容の更新処理 -
//========================================
void UpdateTxtMd_game_00(void)
{
	char			aString					//
					[TXT_MAX];				// 文字列代入用
	Chr_fighter_00	*pChr					//
					= GetChr_fighter_00();	// 機体[00] の情報のポインタ

	//========== *** テキスト[合図] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SIGNAL,
			g_signalText.aString,
			g_signalText.pos,
			g_signalText.col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SIGNAL].font,
			g_signalText.scaleDiameter.fWidth,
			g_signalText.scaleDiameter.fHeight);
	}
	//========== *** テキスト[スコア] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].aDisplay, pChr->nScore);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SCORE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ポーズ] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_PAUSE,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].aDisplay,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[コンティニュー] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_CONTINUE,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].aDisplay,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[コンティニューのカウント] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].aDisplay, g_nCntContinueEnd - 1);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_CONTINUE_COUNT,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[クレジット] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].aDisplay, g_nCredit);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_CREDIT,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[コンティニュー押下] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_PUSH_CONTINUE,
			aContinueButtonText[GetActiveInputType()],
			g_aMd_game_00Text[MD_GAME_00_TEXT_PUSH_CONTINUE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PUSH_CONTINUE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PUSH_CONTINUE].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[リザルトスコア] ***
	{
		g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col.nA = 255 * ((float)g_nCounterShow / (float)RESULT_SCORE_SHOW_TIME);
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].aDisplay, (int)(pChr->nScore * ((float)g_nCntReScoreAdd / (float)RESULT_SCORE_ADD_NUM)));
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_RESULT_SCORE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ライフ] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].aDisplay, pChr->nLife);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_LIFE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ボム] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].aDisplay, pChr->nLife);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_BOMB,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ボム最大] ***
	{
		if ((GetChr_fighter_00()->nBomb >= CHR_FIGHTER_00_BOMB_MAX)
			&&
			((g_gameState != MD_GAME_00_STATE_PAUSE)
				&& (g_gameState != MD_GAME_00_STATE_NONE)
				&& (g_gameState != MD_GAME_00_STATE_SETTING)
				&& (g_gameState != MD_GAME_00_STATE_RESULT)))
		{// 爆弾所持数が爆弾の最大所持数に達していた時、かつ状態がポーズ/無し/設定/リザルトでない時、
			// テキスト[ボム最大] を表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_MAX, true);
		}
		else 
		{// 爆弾所持数が爆弾の最大所持数に達していない時、
			// テキスト[ボム最大] を非表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_MAX, false);
		}
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].aDisplay, pChr->nLife);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_BOMB_MAX,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[エリア時間] ***
	{
		if (GetSys_stage_00()->bShowAreaTime)
		{// エリア時間の表示フラグが真の時、
			sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].aDisplay,
				(GetSys_stage_00()->nWaveEndTime - GetSys_stage_00()->nWaveTime) / 60,
				((GetSys_stage_00()->nWaveEndTime - GetSys_stage_00()->nWaveTime) / 6)
				- (((GetSys_stage_00()->nWaveEndTime - GetSys_stage_00()->nWaveTime) / 60) * 10));
		}
		else
		{// エリア時間の表示フラグが偽の時、
			sprintf(aString, "AREA TIME: --.-");
		}
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_AREA_TIME,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ゲームクリア] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay);
		int nText = (g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay), 0);
		aString[(g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_CLEAR,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ゲームオーバー] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay);
		int nText = (g_nStateCounter / GAME_OVER_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay), 0);
		aString[(g_nStateCounter / GAME_OVER_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_OVER,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ランキング名] ***
	{
		Sys_ranking_00	*pSys		//
			= GetSys_ranking_00();	// ランキング[00] の情報のポインタ
		char			aName		//
						[TXT_MAX];	// 名前

		// 名前を代入
		sprintf(aName, pSys[g_nUpdateRank].aName);

		if (g_bBrink)
		{// 点滅フラグが真の時、
 			aName[g_nCntInputName] = '\0';
		}

		g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].pos = g_aRankingText[g_nUpdateRank].pos;
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].aDisplay, aName);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_RANKING_NAME,
			aString,
			g_aRankingText[g_nUpdateRank].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ランキング] ***
	{
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			Sys_ranking_00	*pSys								//
							= GetSys_ranking_00();				// ランキング[00] の情報のポインタ
			Color			col									//
							= g_aRankingText[nCntRanking].col;	// 色
			char			aName								//
							[RANKING_NAME_NUM + 1];				// 名前

			if ((!g_bResultMenu)
				&& (nCntRanking == g_nUpdateRank))
			{// リザルトメニューが出現していない時、かつ更新順位と順位のカウントが一致した時、
				// 色を設定
				col.nR = 255;
				col.nG = 255;
				col.nB = 255;

				// 空白埋め
				strinit(aName, RANKING_NAME_NUM);
			}
			else 
			{// リザルトメニューが出現している時、
				// 名前を代入
				sprintf(aName, pSys[nCntRanking].aName);
			}

			sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].aDisplay, GetRankText(nCntRanking), aName, pSys[nCntRanking].nScore);
			UpdateTxt_00(
				TEXT_FIELD_UI,
				MD_GAME_00_TEXT_RANKING + nCntRanking,
				aString,
				g_aRankingText[nCntRanking].pos,
				col,
				TEXT_MODE_LEFT,
				g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].font,
				1.0f,
				1.0f);
		}
	}
}

//========================================
// StartGameState関数 - ゲームの状態に応じた開始処理 -
//========================================
void StartGameState(MD_GAME_00_STATE state)
{
	// 状態のカウンターを初期化する
	g_nStateCounter = 0;

	// 開始処理
	switch (g_gameState)
	{
		//========== *** ポーズ ***
	case MD_GAME_00_STATE_PAUSE:
	{
		// テキスト[スコア]		を非表示
		// テキスト[ライフ]		を非表示
		// テキスト[ボム]		を非表示
		// テキスト[エリア時間]	を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE		, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE		, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB		, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME	, false);

		// テキスト[スコア表示] を全て非表示
		for (int nCntUi = 0; nCntUi < UI_SCORE_00_MAX; nCntUi++)
		{
			if (GetUi_score_00()[nCntUi].bUse)
			{// 使用されている時、
				// 非表示にする
				ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi, false);
			}
		}

		if (state != MD_GAME_00_STATE_SETTING)
		{// 元の状態が設定で無い時、
			// テキスト[ポーズ]	を表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, true);

			// 点滅フラグと点滅カウンターを初期化
			g_bBrink = true;
			g_nCounterBrink = 0;

			// ポーズ開始SE[000] を再生
			PlaySound(SOUND_LABEL_SE_PAUSE_START_000);
		}

		// メニュー[00] の設定処理
		SetUi_menu_00Pos(MD_GAME_00_PAUSE_MENU_POS);
		SetUi_menu_00(
			md_game_00_pause_menu_set,
			MD_GAME_00_PAUSE_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);
	}
	break;
		//========== *** 設定 ***
	case MD_GAME_00_STATE_SETTING:
	{
		// 設定メニューの設定処理
		SetSettingMenu(MODE_GAME_00);
	}
	break;
		//========== *** コンティニュー ***
	case MD_GAME_00_STATE_CONTINUE:
	{
		// テキスト[コンティニュー]				を表示
		// テキスト[コンティニューのカウント]	を表示
		// テキスト[クレジット]					を表示
		// テキスト[コンティニュー押下]			を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE_COUNT, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CREDIT, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PUSH_CONTINUE, true);

		// 点滅フラグと点滅カウンターを初期化
		g_bBrink = true;
		g_nCounterBrink = 0;

		// コンティニュー終了までのカウントを初期化
		g_nCntContinueEnd = CONTINUE_COUNT_NUM;

		// カウントダウンSE[000] を再生
		PlaySound(SOUND_LABEL_SE_COUNT_DOWN_000);
	}
	break;
		//========== *** コンティニューの余韻 ***
	case MD_GAME_00_STATE_CONTINUE_AFTERGLOW:
	{
		if (!GetSys_stage_00()->bWarning) 
		{// 警告フラグが偽の時、
			// コンティニューフラグを真にする
			g_bContinue = true;
		}
	}
	break;
		//========== *** 出現待ち ***
	case MD_GAME_00_STATE_POP_WAIT:
	{
		// 機体[00] の設定処理
		SetChr_fighter_00(
			D3DXVECTOR3(
			(GetObj_plasma_block_00_square()->fTargetLeft + GetObj_plasma_block_00_square()->fTargetRight) * 0.5f,
				(GetObj_plasma_block_00_square()->fTargetTop + GetObj_plasma_block_00_square()->fTargetBottom) * 0.5f,
				0.0f),
			D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));

		// テキスト[スコア]		を表示
		// テキスト[ライフ]		を表示
		// テキスト[ボム]		を表示
		// テキスト[エリア時間]	を表示
		// テキスト[ポーズ]		を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, false);
	}
	break;
		//========== *** ウェーブ待ち ***
	case MD_GAME_00_STATE_WAVE_WAIT:
	{
		// テキスト[合図] を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SIGNAL, true);

		// 合図テキストの色を設定
		g_signalText.col = WAVE_TEXT_COLOR;

		// 透明度を0にする
		g_signalText.col.nA = 0;

		// 位置を設定
		g_signalText.pos = WAVE_TEXT_FIRST_POS;

		// 大きさにかかる倍率を設定
		g_signalText.scaleDiameter = WAVE_TEXT_SCALE;

		// 文字列を設定する
		sprintf(g_signalText.aString, "WAVE %d", GetSys_stage_00()->nDispWaveCount);
	}
	break;
		//========== *** スタート待ち ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// 合図テキストの色を設定
		g_signalText.col = START_TEXT_COLOR;

		// 透明度を0にする
		g_signalText.col.nA = 0;

		// 位置を設定
		g_signalText.pos = START_TEXT_POS;

		// 大きさにかかる倍率を設定
		g_signalText.scaleDiameter = START_TEXT_FIRST_SCALE;

		if (g_bContinue) 
		{// コンティニューフラグが真の時、
			// コンティニューフラグを偽にする
			g_bContinue = false;

			// 文字列を設定する
			sprintf(g_signalText.aString, "RESTART");
		}
		else 
		{// コンティニューフラグが偽の時、
			// 文字列を設定する
			sprintf(g_signalText.aString, "START");
		}
	}
	break;
		//========== *** 通常 ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** 警告 ***
	case MD_GAME_00_STATE_WARNING:
	{
		// 警告[00] を出現中にする
		SetStateUi_warning_00(UI_WARNING_00_STATE_IN_POP);
	}
	break;
		//========== *** ゲームクリア ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// テキスト[ゲームクリア]	を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, true);

		// ゲームクリアSE[000] を再生
		PlaySound(SOUND_LABEL_SE_GAME_CLEAR_000);
	}
	break;
		//========== *** ゲームオーバー ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// テキスト[ゲームオーバー]	を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, true);

		// ゲームオーバーSE[000] を再生
		PlaySound(SOUND_LABEL_SE_GAME_OVER_000);
	}
		break;
		//========== *** リザルト ***
	case MD_GAME_00_STATE_RESULT:
	{
		// 表示カウンターを初期化
		g_nCounterShow = 0;

		// リザルトスコアの色を初期化
		g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col = { 255,255,255,0 };

		// リザルトスコアの加算カウンターと
		// リザルトスコアの加算数のカウントを初期化する
		g_nCounterReScoreAdd = 0;
		g_nCntReScoreAdd = 0;

		if (GetSys_stage_00()->soundNow != -1)
		{// 再生中のサウンド番号が-1でない時、
			// 再生中のサウンドを停止
			StopSound((SOUND_LABEL)GetSys_stage_00()->soundNow);
		}

		// テキスト[スコア]			を非表示
		// テキスト[ライフ]			を非表示
		// テキスト[ボム]			を非表示
		// テキスト[エリア時間]		を非表示
		// テキスト[リザルトスコア]	を表示
		// テキスト[ランキング名]	を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RESULT_SCORE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RANKING_NAME, true);
		
		{
			int nNoDamageBonus				//
				= NO_DAMAGE_BONUS_SCORE;	// ノーダメージボーナス

			// ノーダメージボーナスを被弾カウントに応じて減少させる
			nNoDamageBonus -= GetChr_fighter_00()->nCntDamage * NO_DAMAGE_BONUS_SCORE_DECREASE;

			// ノーダメージボーナスを制御
			IntControl(&nNoDamageBonus, INT_MAX, 0);

			// スコアにノーダメージボーナスを加算
			GetChr_fighter_00()->nScore += nNoDamageBonus;

			if (nNoDamageBonus > 0)
			{// ノーダメージボーナスが0を上回っていた時、
				// ライフボーナス[00] の設定処理
				SetUi_life_bonus_00(nNoDamageBonus);
			}
		}

		// スコアを挿入し、更新した順位を取得
		g_nUpdateRank = SetScore(GetChr_fighter_00()->nScore);

		// その順位の名前を空白埋めする
		strinit(GetSys_ranking_00()[g_nUpdateRank].aName, RANKING_NAME_NUM);

		// ランキングテキストの情報を初期化
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			// 移動カウンターを初期化
			g_aRankingText[nCntRanking].nCounterMove = 0;

			// 位置を初期化
			g_aRankingText[nCntRanking].pos = RANKING_TEXT_FIRST_POS;
			g_aRankingText[nCntRanking].pos.y += RANKING_TEXT_SPACE_Y * nCntRanking;

			// 色を設定
			g_aRankingText[nCntRanking].col = g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].col;
			g_aRankingText[nCntRanking].col.nA = 0;

			// テキスト[ランキング] を表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RANKING + nCntRanking, true);
		}

		// 表示した順位を初期化
		g_nShowRanking = 0;

		// 点滅フラグと点滅カウンターを初期化
		g_bBrink = false;
		g_nCounterBrink = 0;
	}
	break;
		//========== *** 死亡後の余韻 ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		if (GetSys_stage_00()->soundNow != -1)
		{// 再生中のサウンド番号が-1でない時、
		 // 再生中のサウンドを停止
			StopSound((SOUND_LABEL)GetSys_stage_00()->soundNow);
		}

		// 再生中のサウンド番号を初期化
		GetSys_stage_00()->soundNow = (SOUND_LABEL)-1;
	}
	break;
		//========= *** 終了 ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
	}
}

//========================================
// EndGameState関数 - ゲームの状態に応じた終了処理 -
//========================================
void EndGameState(MD_GAME_00_STATE state)
{
	switch (g_gameState)
	{
		//========== *** ポーズ ***
	case MD_GAME_00_STATE_PAUSE:
	{
		if (state != MD_GAME_00_STATE_SETTING)
		{// 元の状態が設定で無い時、
			// テキスト[スコア]		を表示
			// テキスト[ライフ]		を表示
			// テキスト[ボム]		を表示
			// テキスト[エリア時間]	を表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, true);
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE, true);
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB, true);
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME, true);

			// テキスト[スコア表示] を全て表示
			for (int nCntUi = 0; nCntUi < UI_SCORE_00_MAX; nCntUi++)
			{
				if (GetUi_score_00()[nCntUi].bUse)
				{// 使用されている時、
					ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi, true);
				}
			}

			// テキスト[ポーズ] を非表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, false);
		}
	}
	break;
		//========== *** コンティニュー ***
	case MD_GAME_00_STATE_CONTINUE:
	{
		// テキスト[コンティニュー] を非表示
		// テキスト[コンティニューのカウント] を非表示
		// テキスト[コンティニュー押下] を非表示
		// テキスト[クレジット] を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE_COUNT, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PUSH_CONTINUE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CREDIT, false);
	}
	break;
		//========== *** コンティニューの余韻 ***
	case MD_GAME_00_STATE_CONTINUE_AFTERGLOW:
	{

	}
	break;
		//========== *** 出現待ち ***
	case MD_GAME_00_STATE_POP_WAIT:
	{

	}
	break;
		//========== *** ウェーブ待ち ***
	case MD_GAME_00_STATE_WAVE_WAIT:
	{

	}
	break;
		//========== *** スタート待ち ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// テキスト[合図] を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SIGNAL, false);
	}
	break;
		//========== *** 通常 ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** 警告 ***
	case MD_GAME_00_STATE_WARNING:
	{
		// プラズマブロック[00] の移動量を設定
		SetMoveObj_plasma_block_00();
	}
	break;
		//========== *** ゲームクリア ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// テキスト[ゲームクリア]	を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, false);
	}
	break;
		//========== *** ゲームオーバー ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// テキスト[ゲームオーバー]	を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, false);
	}
	break;
		//========== *** リザルト ***
	case MD_GAME_00_STATE_RESULT:
	{
		// テキスト[リザルトスコア] を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RESULT_SCORE, false);
	}
	break;
		//========== *** 終了 ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
		//========== *** 死亡後の余韻 ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{

	}
	break;
	}
}

//========================================
// UpdateGameState関数 - ゲームの状態に応じた更新処理 -
//========================================
void UpdateGameState(void)
{
	switch (g_gameState)
	{
		//========== *** 設定 ***
	case MD_GAME_00_STATE_SETTING:
	{
		// 設定メニューの更新処理
		UpdateSettingMenu();
	}
		//========== *** ポーズ ***
	case MD_GAME_00_STATE_PAUSE:
	{
		// 点滅カウンターを加算
		g_nCounterBrink++;

		if (g_nCounterBrink >= PAUSE_TEXT_BRINK_TIME)
		{// 点滅カウンターが点滅にかかる時間に達した時、
			// 点滅カウンターを初期化
			g_nCounterBrink = 0;

			// 点滅フラグを切り替え
			g_bBrink ^= 1;

			// テキスト[ポーズ] を表示/非表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, g_bBrink);
		}

		if ((GetKeyboardTrigger(DIK_P))
			|| (GetButtonTrigger(BUTTON_START)))
		{// ポーズの入力時、
			if (GetUi_menu_00Control()->state == UI_MENU_00_STATE_POP)
			{// 出現状態の時、
				g_gameState = MD_GAME_00_STATE_PAUSE;

				// 通常状態にする
				SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

				// ポーズ終了SE[000] を再生
				PlaySound(SOUND_LABEL_SE_PAUSE_END_000);

				// 消え中にする
				GetUi_menu_00Control()->state = UI_MENU_00_STATE_IN_CLEAR;
				GetUi_menu_00Control()->nCounterState = UI_MENU_00_CLEAR_TIME;
			}
		}

		if (g_gameState == MD_GAME_00_STATE_SETTING) 
		{// 状態が設定の時、
			// 処理を抜ける
			break;
		}

		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** ゲームに戻る ***
		case MD_GAME_00_PAUSE_MENU_RESUME:
		{
			// 通常状態にする
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

			// ポーズ終了SE[000] を再生
			PlaySound(SOUND_LABEL_SE_PAUSE_END_000);
		}
		break;
			//========== *** リトライ ***
		case MD_GAME_00_PAUSE_MENU_RETRY:
		{
			// 決定SE[000] を再生
			PlaySound(SOUND_LABEL_SE_DECITION_000);

			// 状態を無しにする
			SetMd_game_00State(MD_GAME_00_STATE_NONE);

			// モードをゲームに設定
			SetFade(MODE_GAME_00);
		}
		break;
			//========== *** 設定 ***
		case MD_GAME_00_PAUSE_MENU_SETTING:
		{
			// 状態を設定に設定
			SetMd_game_00State(MD_GAME_00_STATE_SETTING);
		}
		break;
			//========== *** タイトルに戻る ***
		case MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE:
		{
			// 状態を無しにする
			SetMd_game_00State(MD_GAME_00_STATE_NONE);

			// モードをタイトルに設定
			SetFade(MODE_TITLE_00);
		}
		break;
		}
	}
	break;
		//========== *** コンティニュー ***
	case MD_GAME_00_STATE_CONTINUE:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if ((g_nStateCounter >= CONTINUE_COUNT_TIME)
			|| (GetKeyboardTrigger(DIK_SPACE))
			|| (GetButtonTrigger(BUTTON_B)))
		{// 状態のカウンターがコンティニューのカウントにかかる時間に達した時、もしくはスキップ入力時、
			// 状態のカウンターを初期化
			g_nStateCounter = 0;

			// コンティニュー終了までのカウントを減算
			g_nCntContinueEnd--;

			if (g_nCntContinueEnd == 0)
			{// コンティニュー終了までのカウントが0になった時、
				// ゲームオーバーに移行
				SetMd_game_00State(MD_GAME_00_STATE_GAME_OVER);

				// 処理を終了
				break;
			}
			else
			{// コンティニュー終了までのカウントが0になっていない時、
				// 効果音[カウントダウン] を再生
				PlaySound(SOUND_LABEL_SE_COUNT_DOWN_000);
			}
		}

		// 点滅カウンターを加算
		g_nCounterBrink++;

		if (g_nCounterBrink >= PUSH_CONTINUE_BRINK_TIME)
		{// 点滅カウンターが点滅にかかる時間に達した時、
			// 点滅カウンターを初期化
			g_nCounterBrink = 0;

			// 点滅フラグを切り替え
			g_bBrink ^= 1;

			// テキスト[コンティニュー押下] を表示/非表示
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PUSH_CONTINUE, g_bBrink);
		}

		if ((GetKeyboardTrigger(DIK_RETURN))
			|| GetButtonTrigger(BUTTON_START))
		{// コンティニュー入力時、
			if (g_nCredit > 0)
			{// クレジットが残っている時、
				// クレジットを減算
				g_nCredit--;

				// 効果音[決定] を再生
				PlaySound(SOUND_LABEL_SE_DECITION_000);

				// コンティニュー処理
				ContinueSys_stage_00();

				// コンティニューの余韻状態にする
				SetMd_game_00State(MD_GAME_00_STATE_CONTINUE_AFTERGLOW);
			}
			else
			{// クレジットが残っていない時、
				// 効果音[衝突] を再生
				PlaySound(SOUND_LABEL_SE_COLLISION_000);
			}
		}

		if (GetKeyboardTrigger(DIK_C)
			&& GetKeyboardPress(DIK_U)
			&& GetKeyboardPress(DIK_P)
			&& SECRET_COMMAND)
		{// 隠しコマンド
			// クレジットを加算
			g_nCredit++;

			// 金属SE[000]を再生
			PlaySound(SOUND_LABEL_SE_METAL_000);
		}
	}
	break;
		//========== *** コンティニューの余韻 ***
	case MD_GAME_00_STATE_CONTINUE_AFTERGLOW:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if (g_nStateCounter >= CONTINUE_AFTERGLOW_TIME)
		{// 状態のカウンターがコンティニューの余韻にかかる時間に達した時、
			// 出現待ちの状態にする
			SetMd_game_00State(MD_GAME_00_STATE_POP_WAIT);
		}

		// プレイヤーの位置を更新
		GetChr_fighter_00()->pos
			= (D3DXVECTOR3(
			(GetObj_plasma_block_00_square()->fTargetLeft + GetObj_plasma_block_00_square()->fTargetRight) * 0.5f,
				(GetObj_plasma_block_00_square()->fTargetTop + GetObj_plasma_block_00_square()->fTargetBottom) * 0.5f, 0.0f)
				* ((float)g_nStateCounter / (float)CONTINUE_AFTERGLOW_TIME))
			+ (GetChr_fighter_00()->pos * (1.0f - ((float)g_nStateCounter / (float)CONTINUE_AFTERGLOW_TIME)));
	}
	break;
		//========== *** 出現待ち ***
	case MD_GAME_00_STATE_POP_WAIT:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if ((g_nStateCounter >= POP_WAIT_TIME))
		{//	状態のカウンターが出現待ちの時間に達した時、
			// ゲームの状態をウェーブ待ちに移行
			SetMd_game_00State(MD_GAME_00_STATE_WAVE_WAIT);
		}
	}
	break;
		//========== *** 通常 ***
	case MD_GAME_00_STATE_NORMAL:
	{
		if ((GetKeyboardTrigger(DIK_P))
			|| (GetButtonTrigger(BUTTON_START)))
		{// ポーズ入力時、
		 // ポーズ状態にする
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);
		}
	}
	break;
		//========== *** ウェーブ待ち ***
	case MD_GAME_00_STATE_WAVE_WAIT:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if (g_nStateCounter <= WAVE_TEXT_INTERMEDIATE_MOVE_TIME)
		{// 状態のカウンターが中間までの移動にかかる時間以下の時、
			// 位置を更新
			g_signalText.pos = (WAVE_TEXT_FIRST_POS * (1.0f - ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME)))
				+ (WAVE_TEXT_INTERMEDIATE_POS * ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME));

			// 透明度を更新
			g_signalText.col.nA = 255 * ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME);
		}

		if ((g_nStateCounter >= WAVE_TEXT_END_MOVE_START_TIME)
			&& (g_nStateCounter <= WAVE_WAIT_TIME))
		{// 状態のカウンターが終了までの移動を開始する時間以上の時、
			// 位置を更新
			g_signalText.pos = (WAVE_TEXT_INTERMEDIATE_POS
				* (1.0f - ((float)(g_nStateCounter - WAVE_TEXT_END_MOVE_START_TIME) / (float)(WAVE_WAIT_TIME - WAVE_TEXT_END_MOVE_START_TIME))))
				+ (WAVE_TEXT_LAST_POS * ((float)(g_nStateCounter - WAVE_TEXT_END_MOVE_START_TIME) / (float)(WAVE_WAIT_TIME - WAVE_TEXT_END_MOVE_START_TIME)));

			// 透明度を更新
			g_signalText.col.nA = 255 * (1.0f - ((float)(g_nStateCounter - WAVE_TEXT_END_MOVE_START_TIME) / (float)(WAVE_WAIT_TIME - WAVE_TEXT_END_MOVE_START_TIME)));
		}

		if ((g_nStateCounter >= WAVE_WAIT_TIME + WAVE_WAIT_AFTERGLOW))
		{//	状態のカウンターがウェーブ待ちの時間に達した時、
			// ゲームの状態をスタート待ちに移行
			SetMd_game_00State(MD_GAME_00_STATE_START_WAIT);
		}
	}
	break;
		//========== *** スタート待ち ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if (g_nStateCounter <= START_TEXT_INTERMEDIATE_TIME)
		{// 状態のカウンターが中間までにかかる時間以下の時、
			// 大きさにかかる倍率を更新
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_FIRST_SCALE.fWidth * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fWidth * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_FIRST_SCALE.fHeight * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fHeight * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));

			// 透明度を更新
			g_signalText.col.nA = 255
				* ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME);
		}

		if (g_nStateCounter >= START_TEXT_END_START_TIME)
		{// 状態のカウンターが終了までの移動を開始する時間以上の時、
			// 大きさにかかる倍率を更新
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_INTERMEDIATE_SCALE.fWidth
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fWidth * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_INTERMEDIATE_SCALE.fHeight
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fHeight * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));

			// 透明度を更新
			g_signalText.col.nA = 255
				* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
		}

		if ((g_nStateCounter >= START_WAIT_TIME))
		{//	状態のカウンターがスタート待ちの時間に達した時、
			// ゲームの状態を通常に移行
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}
	}
	break;
		//========== *** ゲームクリア ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		if (g_nStateCounter >= GAME_CLEAR_TIME) 
		{// 状態のカウンターがゲームクリアにかかる時間に達した時、
			// 状態をリザルトに移行
			SetMd_game_00State(MD_GAME_00_STATE_RESULT);
		}
		else if ((g_nStateCounter % GAME_CLEAR_TEXT_TIME == 0)
			&&((g_nStateCounter / GAME_CLEAR_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay)))
		{// 状態のカウンターがゲームクリアのテキストにかかる時間で割り切れる時、
			// テキストカウントSE[000] を再生
			PlaySound(SOUND_LABEL_SE_TEXT_COUNT_000);
		}

		// 状態のカウンターを加算
		g_nStateCounter++;
	}
	break;
		//========== *** ゲームオーバー ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		if (g_nStateCounter >= GAME_OVER_TIME)
		{// 状態のカウンターがゲームオーバーにかかる時間に達した時、
			// 状態をリザルトに移行
			SetMd_game_00State(MD_GAME_00_STATE_RESULT);
		}
		else if ((g_nStateCounter % GAME_OVER_TEXT_TIME == 0)
			&& ((g_nStateCounter / GAME_OVER_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay)))
		{// 状態のカウンターがゲームオーバーのテキストにかかる時間で割り切れる時、
			// テキストカウントSE[000] を再生
			PlaySound(SOUND_LABEL_SE_TEXT_COUNT_000);
		}

		// 状態のカウンターを加算
		g_nStateCounter++;
	}
	break;
		//========== *** リザルト ***
	case MD_GAME_00_STATE_RESULT:
	{
		if (g_nCounterShow < RESULT_SCORE_SHOW_TIME)
		{// 表示カウンターがリザルトスコアの表示にかかる時間を下回っている時、
			// 表示カウンターを加算
			g_nCounterShow++;
		}

		if (g_nCntReScoreAdd < RESULT_SCORE_ADD_NUM)
		{// リザルトスコアの加算数のカウントがリザルトスコアの加算数を下回っている時、
			// リザルトスコアの加算カウンターを加算
			g_nCounterReScoreAdd++;

			if (g_nCounterReScoreAdd >= RESULT_SCORE_ADD_TIME)
			{// リザルトスコアの加算カウンターがリザルトスコアの加算にかかる時間に達した時、
				// リザルトスコアの加算カウンターを初期化
				g_nCounterReScoreAdd = 0;

				if (GetChr_fighter_00()->nScore != 0)
				{// スコアが0でない時、
					// リザルトスコアの加算数のカウントを加算
					g_nCntReScoreAdd++;

					// スコアカウントSE[000] を再生
					PlaySound(SOUND_LABEL_SE_SCORE_COUNT_000);
				}
				else
				{// スコアが0の時、
					// リザルトスコアの加算数のカウントを最大にする
					g_nCntReScoreAdd = RESULT_SCORE_ADD_NUM;
				}

				if (g_nCntReScoreAdd >= RESULT_SCORE_ADD_NUM)
				{// リザルトスコアの加算数のカウントがリザルトスコアの加算数に達した時、
					// リザルトスコアの色を表示完了時の色に設定する
					g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col = RESULT_SCORE_SHOW_COMP_COLOR;

					// フレーム[02] の設定処理
					SetUi_frame_02(RANKING_FRAME_POS);
				}
			}
		}
		else
		{// リザルトスコアの加算数のカウントがリザルトスコアの加算数に達した時、
			if (g_nUpdateRank != -1)
			{// 順位が更新されている時、
				// 点滅のカウンターを加算
				g_nCounterBrink++;

				if (g_nCounterBrink >= RANKING_TEXT_BRINK_TIME)
				{// 点滅のカウンターが点滅にかかる時間に達した時、
					// 点滅のカウンターを初期化
					g_nCounterBrink = 0;

					// 点滅フラグを更新
					g_bBrink ^= 1;

					if (g_bBrink)
					{// 点滅フラグが真の時、
						// 点滅時の色に設定
						int nTempAlpha = g_aRankingText[g_nUpdateRank].col.nA;
						g_aRankingText[g_nUpdateRank].col = RANKING_TEXT_BRINK_COLOR;
						g_aRankingText[g_nUpdateRank].col.nA = nTempAlpha;
					}
					else
					{// 点滅フラグが偽の時、
						// 通常時の色に設定
						int nTempAlpha = g_aRankingText[g_nUpdateRank].col.nA;
						g_aRankingText[g_nUpdateRank].col = g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].col;
						g_aRankingText[g_nUpdateRank].col.nA = nTempAlpha;
					}
				}
			}

			if (g_nShowRanking < RANKING_NUM)
			{// 全てのランキングを表示し終えていない時、
				// 移動のカウンターを加算
				g_aRankingText[g_nShowRanking].nCounterMove++;

				// 透明度を設定
				g_aRankingText[g_nShowRanking].col.nA =
					255 * ((float)g_aRankingText[g_nShowRanking].nCounterMove / RANKING_TEXT_MOVE_TIME);

				// 位置を更新
				g_aRankingText[g_nShowRanking].pos =
					(RANKING_TEXT_FIRST_POS * (1.0f - ((float)g_aRankingText[g_nShowRanking].nCounterMove / (float)RANKING_TEXT_MOVE_TIME)))
					+ (RANKING_TEXT_LAST_POS * ((float)g_aRankingText[g_nShowRanking].nCounterMove / (float)RANKING_TEXT_MOVE_TIME));
				g_aRankingText[g_nShowRanking].pos.y += g_nShowRanking * RANKING_TEXT_SPACE_Y;

				if (g_aRankingText[g_nShowRanking].nCounterMove >= RANKING_TEXT_MOVE_TIME)
				{// 移動のカウンターが移動にかかる時間に達した時、
					// 表示した順位を加算
					g_nShowRanking++;

					// ランキングセットSE[000] を再生
					PlaySound(SOUND_LABEL_SE_RANKING_SET_000);
				}
			}
			else if ((g_nCntInputName < RANKING_NAME_NUM)
				&& (g_nUpdateRank != -1))
			{// 名前入力のカウントがランキング名の文字数を下回っている時、
				if ((GetKeyboardRepeat(DIK_DOWN))
					|| (GetKeyboardRepeat(DIK_S))
					|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
					|| (GetButtonRepeat(BUTTON_DOWN)))
				{// 下に入力された時、
					// テキストのカウントを減算
					g_nCntText--;

					// 選択SE[001] を再生
					PlaySound(SOUND_LABEL_SE_SELECT_001);
				}
				else if ((GetKeyboardRepeat(DIK_UP))
					|| (GetKeyboardRepeat(DIK_W))
					|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
					|| (GetButtonRepeat(BUTTON_UP)))
				{// 上に入力された時、
					// テキストのカウントを加算
					g_nCntText++;

					// 選択SE[001] を再生
					PlaySound(SOUND_LABEL_SE_SELECT_001);
				}

				// テキストのカウントをループ制御
				IntLoopControl(&g_nCntText, 'Z' + 1, '0');
				
				if (((GetKeyboardTrigger(DIK_BACKSPACE))
					||
					(GetButtonTrigger(BUTTON_B)))
					&&
					(g_nCntInputName > 0))
				{// 削除が入力された時、かつ名前入力のカウントが0を上回っている時、
					// 名前入力のカウントを減算
					g_nCntInputName--;

					// 削除SE[000] を再生
					PlaySound(SOUND_LABEL_SE_DELETION_000);

					// 現在のカウント以降の文字を空白にする
					GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName + 1] = '\0';

					// テキストのカウントを設定
					g_nCntText = GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName];
				}
				else if ((GetKeyboardTrigger(DIK_RETURN))
					|| (GetButtonTrigger(BUTTON_A)))
				{// 決定が入力された時、
					// 名前入力のカウントを加算
					g_nCntInputName++;

					// 決定SE[001] を再生
					PlaySound(SOUND_LABEL_SE_DECITION_001);

					// テキストのカウントを初期化
					g_nCntText = 'A';
				}
				else
				{// 決定が入力されていない時、

					if (g_nCntText == 'Z' + 1) 
					{// テキストのカウントがZ+だった時、
						// 名前に空白を代入
						GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName] = ' ';
					}
					else 
					{// テキストのカウントがZ+でない時、
						// 名前に文字を代入
						GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName] = g_nCntText;
					}
				}

				// 矢印[00] を使用している状態にする
				GetUi_arrow_00()->bUse = true;

				// 矢印[00] の位置を設定
				GetUi_arrow_00()->pos = D3DXVECTOR3(
					RANKING_TEXT_LAST_POS.x + g_nCntInputName * GetFont()[g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].font].fSpaceX,
					RANKING_TEXT_LAST_POS.y + g_nUpdateRank * RANKING_TEXT_SPACE_Y,
					0.0f) + ALLOW_RELATIVE_POSITION;
			}
			else if(!g_bResultMenu)
			{// リザルトメニュー出現フラグが偽の時、
				// リザルトメニュー出現フラグを真にする
				g_bResultMenu = true;

				// 矢印[00] を使用していない状態にする
				GetUi_arrow_00()->bUse = false;

				// テキスト[ランキング名] を非表示にする
				ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RANKING_NAME, false);

				// メニューの中心座標を設定
				SetUi_menu_00Pos(MD_GAME_00_RESULT_MENU_POS);

				// メニュー[00] の設定処理
				SetUi_menu_00(
					md_game_00_result_menu_set,
					MD_GAME_00_RESULT_MENU_MAX,
					UI_MENU_00_DEFAULT_SPACE_X,
					UI_MENU_00_DEFAULT_SPACE_Y);
			}
			else if (GetFade() == FADE_NONE)
			{// 全てのランキングを表示し終えていて、かつフェードが無い時、
				// メニューの入力処理
				switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
				{
					//========== *** リトライ ***
				case MD_GAME_00_RESULT_MENU_RETRY:
				{
					// 決定SE[000] を再生
					PlaySound(SOUND_LABEL_SE_DECITION_000);

					// モードをゲームに設定
					SetFade(MODE_GAME_00);
				}
				break;
					//========== *** タイトルに戻る ***
				case MD_GAME_00_RESULT_MENU_BACK_TO_TITLE:
				{
					// モードをタイトルに設定
					SetFade(MODE_TITLE_00);
				}
				break;
				}
			}
		}
	}
	break;
		//========== *** 死亡後の余韻 ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if (g_nStateCounter >= DIED_AFTERGLOW_TIME)
		{// 状態のカウンターが死亡後の余韻にかかる時間に達した時、
		 // コンティニューに移行
			SetMd_game_00State(MD_GAME_00_STATE_CONTINUE);
		}
	}
	break;
		//========== *** 終了 ***
	case MD_GAME_00_STATE_END:
	{

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
// InitMd_game_00関数 - ゲーム画面[00]の初期化処理 -
//========================================
void InitMd_game_00(void)
{
	g_gameState		= MD_GAME_00_STATE_NONE;	// ゲームの状態を初期化
	g_nCredit		= INIT_CREDIT_NUM;			// クレジットを初期化
	g_bContinue		= false;					// コンティニューフラグを偽にする
	g_nCntInputName = 0;						// 名前入力のカウントを初期化
	g_nCntText		= 'A';						// テキストのカウントを初期化
	g_bResultMenu	= false;					// リザルトメニュー出現フラグを偽にする

	// 初期化処理
	InitTxt_00();				// テキスト
	InitAtk_bullet_00();		// ATK:弾					[00]
	InitAtk_explosion_00();		// ATK:爆発					[00]
	InitAtk_explosion_01();		// ATK:爆発					[01]
	InitBg_space_00();			// BG :宇宙					[00]
	InitChr_enemy_00();			// CHR:敵					[00]
	InitChr_fighter_00();		// CHR:機体					[00]
	InitEff_general_00();		// EFF:エフェクト全般		[00]
	InitEff_laser_pointer_00();	// EFF:レーザーポインタ―	[00]
	InitEff_light_00();			// EFF:ライト				[00]
	InitEff_particle_00();		// EFF:パーティクル			[00]
	InitEff_screen_00();		// EFF:スクリーン			[00]
	InitItem_bomb_00();			// ITM:爆弾					[00]
	InitItem_medal_00();		// ITM:メダル				[00]
	InitObj_plasma_block_00();	// OBJ:プラズマブロック		[00]
	InitSys_stage_00(0);		// SYS;ステージ				[00]
	InitUi_area_gage_00();		// UI :エリアゲージ			[00]
	InitUi_arrow_00();			// UI :矢印					[00]
	InitUi_bomb_00();			// UI :ボム					[00]
	InitUi_bomb_get_00();		// UI :ボム獲得				[00]
	InitUi_frame_01();			// UI :フレーム				[01]
	InitUi_frame_02();			// UI :フレーム				[02]
	InitUi_life_00();			// UI :体力					[00]
	InitUi_life_bonus_00();		// UI :ライフボーナス		[00]
	InitUi_menu_00();			// UI :メニュー				[00]
	InitUi_score_00();			// UI :スコア				[00]
	InitUi_warning_00();		// UI :警告					[00]
	InitObj_plasma_wall_00();	// OBJ:プラズマウォール		[00]

	// ゲームの状態を出現待ちに設定
	SetMd_game_00State(MD_GAME_00_STATE_POP_WAIT);
}

//========================================
// UninitMd_game_00関数 - ゲーム画面[00]の終了処理 -
//========================================
void UninitMd_game_00(void)
{
	// 終了処理
	UninitTxt_00();					// テキスト
	UninitAtk_bullet_00();			// ATK:弾					[00]
	UninitAtk_explosion_00();		// ATK:爆発					[00]
	UninitAtk_explosion_01();		// ATK:爆発					[01]
	UninitBg_space_00();			// BG :宇宙					[00]
	UninitChr_enemy_00();			// CHR:敵					[00]
	UninitChr_fighter_00();			// CHR:機体					[00]
	UninitEff_general_00();			// EFF:エフェクト全般		[00]
	UninitEff_laser_pointer_00();	// EFF:レーザーポインタ―	[00]
	UninitEff_light_00();			// EFF:ライト				[00]
	UninitEff_screen_00();			// EFF:スクリーン			[00]
	UninitItem_bomb_00();			// ITM:爆弾					[00]
	UninitItem_medal_00();			// ITM:メダル				[00]
	UninitSys_stage_00();			// SYS:ステージ				[00]
	UninitUi_area_gage_00();		// UI :エリアゲージ			[00]
	UninitUi_arrow_00();			// UI :矢印					[00]
	UninitUi_bomb_00();				// UI :ボム					[00]
	UninitUi_bomb_get_00();			// UI :ボム獲得				[00]
	UninitUi_frame_01();			// UI :フレーム				[01]
	UninitUi_frame_02();			// UI :フレーム				[02]
	UninitUi_life_00();				// UI :体力					[00]
	UninitUi_life_bonus_00();		// UI :ライフボーナス		[00]
	UninitUi_menu_00();				// UI :メニュー				[00]
	UninitUi_score_00();			// UI :スコア				[00]
	UninitUi_warning_00();			// UI :警告					[00]
	UninitObj_plasma_block_00();	// OBJ:プラズマブロック		[00]
	UninitObj_plasma_wall_00();		// OBJ:プラズマウォール		[00]
}

//========================================
// UpdateMd_game_00関数 - ゲーム画面[00]の更新処理 -
//========================================
void UpdateMd_game_00(void)
{
	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_NONE)
		&& (g_gameState != MD_GAME_00_STATE_SETTING))
	{// 状態がポーズ/無し/設定でない時、
		// 更新処理
		UpdateObj_plasma_block_00_square();	// OBJ:プラズマブロック		[00] が描く四角形
		UpdateChr_fighter_00();				// CHR:機体					[00]
		UpdateObj_plasma_block_00();		// OBJ:プラズマブロック		[00]
		UpdateObj_plasma_wall_00();			// OBJ:プラズマウォール		[00]
		UpdateChr_enemy_00();				// CHR:敵					[00]
		UpdateAtk_bullet_00();				// ATK:弾					[00]
		UpdateAtk_explosion_00();			// ATK:爆発					[00]
		UpdateAtk_explosion_01();			// ATK:爆発					[01]
		UpdateBg_space_00();				// BG :宇宙					[00]
		UpdateEff_general_00();				// EFF:エフェクト全般		[00]
		UpdateEff_particle_00();			// EFF:パーティクル			[00]
		UpdateEff_laser_pointer_00();		// EFF:レーザーポインタ―	[00]
		UpdateEff_light_00();				// EFF:ライト				[00]
		UpdateEff_screen_00();				// EFF:スクリーン			[00]
		UpdateItem_bomb_00();				// ITM:爆弾					[00]
		UpdateItem_medal_00();				// ITM:メダル				[00]
		UpdateSys_stage_00();				// SYS:ステージ				[00]
		UpdateUi_area_gage_00();			// UI :エリアゲージ			[00]
		UpdateUi_arrow_00();				// UI :矢印					[00]
		UpdateUi_bomb_00();					// UI :ボム					[00]
		UpdateUi_bomb_get_00();				// UI :ボム獲得				[00]
		UpdateUi_frame_02();				// UI :フレーム				[02]
		UpdateUi_life_00();					// UI :体力					[00]
		UpdateUi_life_bonus_00();			// UI :ライフボーナス		[00]
		UpdateUi_score_00();				// UI :スコア				[00]
		UpdateUi_warning_00();				// UI :警告					[00]
	}

	// メニュー[00] の更新処理
	UpdateUi_menu_00();

	// ゲームの状態に応じた更新処理
	UpdateGameState();

	// テキスト内容の更新処理
	UpdateTxtMd_game_00();
}

//========================================
// DrawMd_game_00関数 - ゲーム画面[00]の描画処理 -
//========================================
void DrawMd_game_00(void)
{
	// 描画処理
	if (g_gameState != MD_GAME_00_STATE_RESULT)
	{// 状態がリザルトでない時、
		DrawUi_frame_01();		// UI :フレーム					[01]
	}
	DrawBg_space_00();			// BG :宇宙						[00]
	DrawObj_plasma_wall_00();	// OBJ:プラズマウォール			[00]
	DrawObj_plasma_block_00();	// OBJ:プラズマブロック			[00]
	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_NONE)
		&& (g_gameState != MD_GAME_00_STATE_SETTING))
	{// 状態がポーズ/無し/設定でない時、
		DrawChr_enemy_00();			// CHR:敵					[00]
		DrawEff_laser_pointer_00();	// EFF:レーザーポインタ―	[00]
		DrawChr_fighter_00();		// CHR:機体					[00]
		DrawEff_light_00();			// EFF:ライト				[00]
		DrawEff_general_00();		// EFF:エフェクト全般		[00]
		DrawAtk_bullet_00();		// ATK:弾					[00]
		DrawAtk_explosion_00();		// ATK:爆発					[00]
		DrawAtk_explosion_01();		// ATK:爆発					[01]
		DrawItem_bomb_00();			// ITM:爆弾					[00]
		DrawItem_medal_00();		// ITM:メダル				[00]
	}
	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_NONE)
		&& (g_gameState != MD_GAME_00_STATE_SETTING)
		&& (g_gameState != MD_GAME_00_STATE_RESULT))
	{// 状態がポーズ/無し/設定/リザルトでない時、
		DrawUi_bomb_00();		// UI :ボム						[00]
		DrawUi_life_00();		// UI :体力						[00]
		DrawUi_area_gage_00();	// UI :エリアゲージ				[00]
	}
	DrawUi_warning_00();		// UI :警告						[00]
	DrawUi_frame_02();			// UI :フレーム					[02]
	DrawUi_menu_00();			// UI :メニュー					[00]
	DrawTxt_00();				// テキスト
	DrawUi_arrow_00();			// UI :矢印						[00]
	DrawEff_screen_00();		// EFF:スクリーン				[00]
}

//========================================
// SetGameState関数 - ゲームの状態の設定 -
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state) 
{
	MD_GAME_00_STATE	stateTemp		// 
						= g_gameState;	// 元の状態を一時的に格納

	// ゲームの状態に応じた終了処理
	EndGameState(state);

	// ゲームの状態を反映
	g_gameState = state;

	// ゲームの状態に応じた開始処理
	StartGameState(stateTemp);
}
