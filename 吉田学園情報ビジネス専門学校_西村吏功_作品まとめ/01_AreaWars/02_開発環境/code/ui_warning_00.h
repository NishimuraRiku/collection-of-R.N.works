//========================================
// *** ui_warning_00.h ***
//========================================
#ifndef _UI_WARNING_00_H_	// このマクロ定義がされていなかった時
#define _UI_WARNING_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// 警告[00] の状態
typedef enum
{
	UI_WARNING_00_STATE_POP = 0,	// 出現
	UI_WARNING_00_STATE_IN_POP,		// 出現中
	UI_WARNING_00_STATE_CLEAR,		// 消えている
	UI_WARNING_00_STATE_IN_CLEAR,	// 消え中
	UI_WARNING_00_STATE_MAX,
}UI_WARNING_00_STATE;

//****************************************
// プロトタイプ宣言
//****************************************

// 警告[00] の初期化処理
void InitUi_warning_00(void);

// 警告[00] の終了処理
void UninitUi_warning_00(void);

// 警告[00] の更新処理
void UpdateUi_warning_00(void);

// 警告[00] の描画処理
void DrawUi_warning_00(void);

// 警告[00] の状態設定処理
void SetStateUi_warning_00(UI_WARNING_00_STATE state);

#endif