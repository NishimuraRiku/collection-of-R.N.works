//========================================
// *** ui_operation_00.h ***
//========================================
#ifndef _UI_OPERATION_00_H_	// このマクロ定義がされていなかった時
#define _UI_OPERATION_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// 操作説明[00] の状態
typedef enum
{
	UI_OPERATION_00_STATE_POP = 0,	// 出現
	UI_OPERATION_00_STATE_IN_POP,	// 出現中
	UI_OPERATION_00_STATE_CLEAR,	// 消えている
	UI_OPERATION_00_STATE_IN_CLEAR,	// 消え中
	UI_OPERATION_00_STATE_MAX,
}UI_OPERATION_00_STATE;

//****************************************
// プロトタイプ宣言
//****************************************

// 操作説明[00] の初期化処理
void InitUi_operation_00(void);

// 操作説明[00] の終了処理
void UninitUi_operation_00(void);

// 操作説明[00] の更新処理
void UpdateUi_operation_00(void);

// 操作説明[00] の描画処理
void DrawUi_operation_00(void);

// 操作説明[00] の状態設定処理
void SetStateUi_operation_00(UI_OPERATION_00_STATE state);

#endif