//========================================
// 
// UI:ユーザーガイド[00] のヘッダファイル
// Author:西村 吏功
// 
//========================================
// *** ui_user-guid_00.h ***
//========================================
#ifndef _UI_USERGUID_00_H_	// このマクロ定義がされていなかった時
#define _UI_USERGUID_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "input.h"

//****************************************
// 列挙型の定義
//****************************************
// UI:ユーザーガイド[00] の状態
typedef enum
{
	UI_USERGUID_00_STATE_POP,		// 出現
	UI_USERGUID_00_STATE_IN_POP,	// 出現中
	UI_USERGUID_00_STATE_CLEAR,		// 消えている
	UI_USERGUID_00_STATE_IN_CLEAR,	// 消え中
	UI_USERGUID_00_STATE_SWITCH,	// 切り替え
	UI_USERGUID_00_STATE_MAX,
}UI_USERGUID_00_STATE;

//****************************************
// 構造体の定義
//****************************************
// UI:ユーザーガイド[00] の情報構造体
typedef struct
{
	ACTIVE_INPUT_TYPE		type;			// 種類
	UI_USERGUID_00_STATE	state;			// 状態
	int						nCounterState;	// 状態カウンター
	float					fAlpha;			// 全体の透明度
	Scale					scale;			// 拡大倍率
	D3DXVECTOR3				pos;			// 位置
}Ui_userGuid_00;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:ユーザーガイド[00] の情報を取得
Ui_userGuid_00 *GetUi_userGuid_00(void);

// UI:ユーザーガイド[00] の初期化処理
// UI:ユーザーガイド[00] の終了処理
// UI:ユーザーガイド[00] の更新処理
// UI:ユーザーガイド[00] の描画処理
void InitUi_userGuid_00(void);
void UninitUi_userGuid_00(void);
void UpdateUi_userGuid_00(void);
void DrawUi_userGuid_00(void);

// UI:ユーザーガイド[00] の状態設定処理
// UI:ユーザーガイド[00] の設定処理
void SetStateUi_userGuid_00(UI_USERGUID_00_STATE state);
void SetUi_userGuid_00(D3DXVECTOR3 pos, ACTIVE_INPUT_TYPE type);

#endif