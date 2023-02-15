//========================================
// *** md_title_00.h ***
//========================================
#ifndef _MD_TITLE_00_H_	// このマクロ定義がされていなかった時
#define _MD_TITLE_00_H_	// 二重インクルード防止のマクロを定義する

#include "sys_ranking_00.h"

//****************************************
// マクロ定義
//****************************************

// タイトル画面[00] のタイトルアニメーションにかかる時間
#define MD_TITLE_00_TITLE_ANIM_TIME	(80)

//****************************************
// 列挙型の定義
//****************************************

// タイトル画面[00] の状態
typedef enum 
{
	MD_TITLE_00_STATE_NONE = 0,		// 何もしていない
	MD_TITLE_00_STATE_TEAM_LOGO,	// チームロゴ
	MD_TITLE_00_STATE_TITLE_ANIM,	// タイトルアニメーション
	MD_TITLE_00_STATE_FLASH,		// フラッシュ
	MD_TITLE_00_STATE_NORMAL,		// 通常
	MD_TITLE_00_STATE_USER_GUIDE,	// 操作説明
	MD_TITLE_00_STATE_RANKING,		// ランキング
	MD_TITLE_00_STATE_SETTING,		// 設定
	MD_TITLE_00_STATE_MAX,
}MD_TITLE_00_STATE;

// ゲーム画面[00] のテキスト番号
typedef enum
{
	MD_TITLE_00_TEXT_COPYRIGHT = 0,	// 著作権表示
	MD_TITLE_00_TEXT_CREDIT,		// クレジット
	MD_TITLE_00_TEXT_RANKING,		// ランキング
	MD_TITLE_00_TEXT_RANKING_MAX	// ランキングの最大
	= MD_TITLE_00_TEXT_RANKING
	+ RANKING_NUM - 1,
	MD_TITLE_00_TEXT_MAX,
}MD_TITLE_00_TEXT;

//****************************************
// 構造体の定義
//****************************************

// タイトル画面[00] の情報構造体
typedef struct 
{
	// 状態関連
	MD_TITLE_00_STATE	state;			// タイトル画面[00] の状態
	int					nCounterState;	// タイトル画面[00] の状態カウンター
	bool				bPlayBGM;		// BGM再生フラグ
	bool				bShowRanking;	// ランキング表示フラグ
}Md_title_00;

//****************************************
// プロトタイプ宣言
//****************************************

// タイトル画面[00] の情報を取得
Md_title_00 *GetMd_title_00(void);

// タイトル画面[00] の初期化処理
void InitMd_title_00(void);

// タイトル画面[00] の終了処理
void UninitMd_title_00(void);

// タイトル画面[00] の更新処理
void UpdateMd_title_00(void);

// タイトル画面[00] の描画処理
void DrawMd_title_00(void);

// タイトル画面[00] の状態を設定
void SetMd_title_00State(MD_TITLE_00_STATE state);

#endif