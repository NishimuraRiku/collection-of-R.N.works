//========================================
// *** ui_team_logo_00.h ***
//========================================
#ifndef _UI_TEAM_LOGO_00_H_	// このマクロ定義がされていなかった時
#define _UI_TEAM_LOGO_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// チームロゴ[00] の状態
typedef enum
{
	UI_TEAM_LOGO_00_STATE_POP = 0,	// 出現
	UI_TEAM_LOGO_00_STATE_IN_POP,	// 出現中
	UI_TEAM_LOGO_00_STATE_CLEAR,	// 消えている
	UI_TEAM_LOGO_00_STATE_IN_CLEAR,	// 消え中
	UI_TEAM_LOGO_00_STATE_MAX,
}UI_TEAM_LOGO_00_STATE;

//****************************************
// 構造体の定義
//****************************************

// チームロゴ[00] の情報構造体
typedef struct
{
	UI_TEAM_LOGO_00_STATE	state;			// 状態
	int						nCounterState;	// 状態カウンター
	float					fAlpha;			// 全体の透明度
}Ui_team_logo_00;

//****************************************
// プロトタイプ宣言
//****************************************

// チームロゴ[00] の情報を取得
Ui_team_logo_00 *GetUi_team_logo_00(void);

// チームロゴ[00] の初期化処理
void InitUi_team_logo_00(void);

// チームロゴ[00] の終了処理
void UninitUi_team_logo_00(void);

// チームロゴ[00] の更新処理
void UpdateUi_team_logo_00(void);

// チームロゴ[00] の描画処理
void DrawUi_team_logo_00(void);

// チームロゴ[00] の状態設定処理
void SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE state);

#endif