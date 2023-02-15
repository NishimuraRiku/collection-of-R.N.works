//========================================
// *** ui_title_logo_00.h ***
//========================================
#ifndef _UI_TITLE_LOGO_00_H_	// このマクロ定義がされていなかった時
#define _UI_TITLE_LOGO_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// タイトルロゴ[00] の状態
typedef enum
{
	UI_TITLE_LOGO_00_STATE_POP = 0,					// 出現
	UI_TITLE_LOGO_00_STATE_IN_POP,					// 出現中
	UI_TITLE_LOGO_00_STATE_CLEAR,					// 消えている
	UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE,		// プラズマウォールの移動
	UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT,	// プラズマウォールの移動待ち
	UI_TITLE_LOGO_00_STATE_MAX,
}UI_TITLE_LOGO_00_STATE;

//****************************************
// 構造体の定義
//****************************************

// 四角形の情報構造体
typedef struct
{
	float fTop;
	float fBottom;
	float fLeft;
	float fRight;
}Square;

// タイトルロゴ[00] の情報構造体
typedef struct
{
	UI_TITLE_LOGO_00_STATE	state;			// 状態
	int						nCounterState;	// 状態カウンター
	float					fAlpha;			// 全体の透明度
	D3DXVECTOR3				pos;			// 位置
	bool					bActive;		// 動的フラグ
	Square					square;			// 四角形の情報
	int						nCntBrink;		// 点滅カウント
	int						nCounterBrink;	// 点滅カウンター
}Ui_title_logo_00;

//****************************************
// プロトタイプ宣言
//****************************************

// タイトルロゴ[00] の情報を取得
Ui_title_logo_00 *GetUi_title_logo_00(void);

// タイトルロゴ[00] の初期化処理
void InitUi_title_logo_00(void);

// タイトルロゴ[00] の終了処理
void UninitUi_title_logo_00(void);

// タイトルロゴ[00] の更新処理
void UpdateUi_title_logo_00(void);

// タイトルロゴ[00] の描画処理
void DrawUi_title_logo_00(void);

// タイトルロゴ[00] の状態設定処理
void SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE state);

#endif