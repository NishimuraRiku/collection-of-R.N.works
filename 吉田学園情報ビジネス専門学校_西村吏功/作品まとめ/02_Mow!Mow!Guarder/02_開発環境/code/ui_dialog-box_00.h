//========================================
// 
// UI:ダイアログボックス[00] のヘッダファイル
// Author:西村 吏功
// 
//========================================
// *** ui_dialog-box_00.h ***
//========================================
#ifndef _UI_DIALOGBOX_00_H_	// このマクロ定義がされていなかった時
#define _UI_DIALOGBOX_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// マクロ定義
//****************************************
// UI:ダイアログボックス[00] の位置
#define UI_DIALOGBOX_00_POS	D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y+(PIXEL*64),0.0f)

//****************************************
// 列挙型の定義
//****************************************
// UI:ダイアログボックス[00] の状態
typedef enum
{
	UI_DIALOGBOX_00_STATE_POP,		// 出現
	UI_DIALOGBOX_00_STATE_IN_POP,	// 出現中
	UI_DIALOGBOX_00_STATE_CLEAR,	// 消えている
	UI_DIALOGBOX_00_STATE_IN_CLEAR,	// 消え中
	UI_DIALOGBOX_00_STATE_MAX,
}UI_DIALOGBOX_00_STATE;

// UI:ダイアログボックス[00] の種類
typedef enum
{
	UI_DIALOGBOX_00_TYPE_NONE = -1,	// 無し
	UI_DIALOGBOX_00_TYPE_00,		// 00
	UI_DIALOGBOX_00_TYPE_01,		// 01
	UI_DIALOGBOX_00_TYPE_02,		// 02
	UI_DIALOGBOX_00_TYPE_03,		// 03
	UI_DIALOGBOX_00_TYPE_04,		// 04
	UI_DIALOGBOX_00_TYPE_05,		// 05
	UI_DIALOGBOX_00_TYPE_06,		// 06
	UI_DIALOGBOX_00_TYPE_07,		// 07
	UI_DIALOGBOX_00_TYPE_MAX,
}UI_DIALOGBOX_00_TYPE;

//****************************************
// 構造体の定義
//****************************************
// UI:ダイアログボックス[00] の情報構造体
typedef struct
{
	UI_DIALOGBOX_00_TYPE	type;			// 種類
	UI_DIALOGBOX_00_STATE	state;			// 状態
	int						nCounterState;	// 状態カウンター
	float					fAlpha;			// 全体の透明度
	Scale					scale;			// 拡大倍率
}Ui_dialogBox_00;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:ダイアログボックス[00] の情報を取得
Ui_dialogBox_00 *GetUi_dialogBox_00(void);

// UI:ダイアログボックス[00] の初期化処理
// UI:ダイアログボックス[00] の終了処理
// UI:ダイアログボックス[00] の更新処理
// UI:ダイアログボックス[00] の描画処理
void InitUi_dialogBox_00(void);
void UninitUi_dialogBox_00(void);
void UpdateUi_dialogBox_00(void);
void DrawUi_dialogBox_00(void);

// UI:ダイアログボックス[00] の状態設定処理
// UI:ダイアログボックス[00] の設定処理
void SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE state);
void SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE type);

#endif