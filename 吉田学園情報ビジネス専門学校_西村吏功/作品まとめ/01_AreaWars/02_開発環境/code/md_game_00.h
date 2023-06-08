//========================================
// *** md_game_00.h ***
//========================================
#ifndef _MD_GAME_00_H_	// このマクロ定義がされていなかった時
#define _MD_GAME_00_H_	// 二重インクルード防止のマクロを定義する

#include "text.h"
#include "sys_ranking_00.h"
#include "ui_score_00.h"

//****************************************
// マクロ定義 
//****************************************

// 初期のクレジット数
#define INIT_CREDIT_NUM	(2)

//****************************************
// 列挙型の定義
//****************************************

// ゲーム画面[00] の状態
typedef enum 
{
	MD_GAME_00_STATE_NONE = 0,				// 何もしていない
	MD_GAME_00_STATE_PAUSE,					// ポーズ
	MD_GAME_00_STATE_SETTING,				// 設定
	MD_GAME_00_STATE_CONTINUE,				// コンティニュー
	MD_GAME_00_STATE_CONTINUE_AFTERGLOW,	// コンティニューの余韻
	MD_GAME_00_STATE_POP_WAIT,				// 出現待ち
	MD_GAME_00_STATE_WAVE_WAIT,				// ウェーブ待ち
	MD_GAME_00_STATE_START_WAIT,			// スタート待ち
	MD_GAME_00_STATE_CLEAR_WAIT,			// クリア待ち
	MD_GAME_00_STATE_NORMAL,				// 通常
	MD_GAME_00_STATE_WARNING,				// 警告
	MD_GAME_00_STATE_GAME_CLEAR,			// ゲームクリア
	MD_GAME_00_STATE_GAME_OVER,				// ゲームオーバー
	MD_GAME_00_STATE_RESULT,				// リザルト
	MD_GAME_00_STATE_DIED_AFTERGLOW,		// 死亡後の余韻
	MD_GAME_00_STATE_END,					// 終了
	MD_GAME_00_STATE_MAX
}MD_GAME_00_STATE;

// ゲーム画面[00] のテキスト番号
typedef enum
{
	MD_GAME_00_TEXT_SCORE = 0,		// スコア
	MD_GAME_00_TEXT_SIGNAL,			// 合図
	MD_GAME_00_TEXT_PAUSE,			// ポーズ
	MD_GAME_00_TEXT_CONTINUE,		// コンティニュー
	MD_GAME_00_TEXT_CONTINUE_COUNT,	// コンティニューカウント
	MD_GAME_00_TEXT_PUSH_CONTINUE,	// コンティニュー押下
	MD_GAME_00_TEXT_CREDIT,			// クレジット
	MD_GAME_00_TEXT_RESULT_SCORE,	// リザルトスコア
	MD_GAME_00_TEXT_LIFE_BONUS,		// ライフボーナス
	MD_GAME_00_TEXT_LIFE,			// ライフ
	MD_GAME_00_TEXT_BOMB,			// ボム
	MD_GAME_00_TEXT_BOMB_MAX,		// ボム最大
	MD_GAME_00_TEXT_BOMB_GET,		// ボム獲得
	MD_GAME_00_TEXT_AREA_TIME,		// エリア時間
	MD_GAME_00_TEXT_GAME_CLEAR,		// ゲームクリア
	MD_GAME_00_TEXT_GAME_OVER,		// ゲームオーバー
	MD_GAME_00_TEXT_RANKING_NAME,	// ランキング名
	MD_GAME_00_TEXT_RANKING,		// ランキング
	MD_GAME_00_TEXT_RANKING_MAX		// ランキングの最大
	= MD_GAME_00_TEXT_RANKING 
	+ RANKING_NUM - 1,
	MD_GAME_00_TEXT_SCORE_DISP,		// スコア表示
	MD_GAME_00_TEXT_SCORE_DISP_MAX	// スコア表示の最大
	= MD_GAME_00_TEXT_SCORE_DISP 
	+ UI_SCORE_00_MAX -1,
	MD_GAME_00_TEXT_MAX,
}MD_GAME_00_TEXT;

//****************************************
// プロトタイプ宣言
//****************************************

// ゲーム画面[00]の初期化処理
void InitMd_game_00(void);

// ゲーム画面[00]の終了処理
void UninitMd_game_00(void);

// ゲーム画面[00]の更新処理
void UpdateMd_game_00(void);

// ゲーム画面[00]の描画処理
void DrawMd_game_00(void);

// ゲーム画面[00]のテキストの情報を取得
Text GetMd_game_00Text(MD_GAME_00_TEXT text);

// ゲームの状態の設定
void SetMd_game_00State(MD_GAME_00_STATE state);

// ゲームの状態の取得
MD_GAME_00_STATE GetGameState(void);

// クレジットの取得
int *GetCredit(void);

#endif