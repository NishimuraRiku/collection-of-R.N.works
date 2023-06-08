//========================================
// 
// UI:TIPS[00] のヘッダファイル
// Author:西村 吏功
// 
//========================================
// *** ui_tips_00.h ***
//========================================
#ifndef _UI_TIPS_00_H_	// このマクロ定義がされていなかった時
#define _UI_TIPS_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "input.h"

//****************************************
// 列挙型の定義
//****************************************
// UI:TIPS[00] の状態
typedef enum
{
	UI_TIPS_00_STATE_POP,		// 出現
	UI_TIPS_00_STATE_IN_POP,	// 出現中
	UI_TIPS_00_STATE_CLEAR,		// 消えている
	UI_TIPS_00_STATE_IN_CLEAR,	// 消え中
	UI_TIPS_00_STATE_SWITCH,	// 切り替え
	UI_TIPS_00_STATE_MAX,
}UI_TIPS_00_STATE;

//****************************************
// 構造体の定義
//****************************************
// UI:TIPS[00] の情報構造体
typedef struct
{
	int nType;				// 種類
	int nStep;				// ステップ
	bool bStepViewAll;		// ステップを全て閲覧したフラグ
	bool bStepViewAllOld;	// ステップを全て閲覧したフラグ(過去)
	UI_TIPS_00_STATE state;	// 状態
	int nCounterState;		// 状態カウンター
	float fAlpha;			// 全体の透明度
	Scale scale;			// 拡大倍率
}Ui_tips_00;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:TIPS[00] の情報を取得
Ui_tips_00 *GetUi_tips_00(void);
// UI:TIPS[00] の切替を取得
bool GetSwitchUi_tips_00(void);

// UI:TIPS[00] の初期化処理
void InitUi_tips_00(void);
// UI:TIPS[00] の終了処理
void UninitUi_tips_00(void);
// UI:TIPS[00] の更新処理
void UpdateUi_tips_00(void);
// UI:TIPS[00] の描画処理
void DrawUi_tips_00(void);

// UI:TIPS[00] の状態設定処理
void SetStateUi_tips_00(UI_TIPS_00_STATE state);
// UI:TIPS[00] の設定処理
void SetUi_tips_00(int nType);

#endif