//========================================
// 
// UI:メニュー[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_menu_00.h ***
//========================================
#ifndef UI_MENU_00_H_	// このマクロ定義がされていなかった時
#define UI_MENU_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************
// UI:メニュー[00] の入力モード
typedef enum
{
	UI_MENU_00_INPUT_MODE_UP_AND_DOWN,		// 上下
	UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT,	// 左右
	UI_MENU_00_INPUT_MODE_MAX
}UI_MENU_00_INPUT_MODE;

// UI:メニュー[00] の種類
typedef enum
{
	UI_MENU_00_TYPE_NORMAL,		// 通常
	UI_MENU_00_TYPE_SELECTION,	// 選択
	UI_MENU_00_TYPE_MAX
}UI_MENU_00_TYPE;

// UI:メニュー[00] の状態
typedef enum
{
	UI_MENU_00_STATE_NONE,
	UI_MENU_00_STATE_IN_POP,	// 出現中
	UI_MENU_00_STATE_POP,		// 出現
	UI_MENU_00_STATE_IN_CLEAR,	// 消え中
	UI_MENU_00_STATE_MAX
}UI_MENU_00_STATE;

//****************************************
// 構造体の定義
//****************************************
// UI:メニュー[00] の管理情報構造体
typedef struct
{
	UI_MENU_00_STATE	state;			// 状態
	int					nCounterState;	// 状態のカウンター
	Scale				scale;			// 拡大率
	int					nNum;			// 現在の使用数
	int					nSelect;		// 現在の選択番号
	D3DXVECTOR3			centerPos;		// 中心座標
}Ui_menu_00Control;

// UI:メニュー[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3	pos;	// 位置

	// 分類情報
	UI_MENU_00_TYPE	type;	// 種類
	
	// 状態値
	bool	bUse;		// 使用されているかフラグ
	bool	bSelect;	// 選択されているかフラグ
	char	aString
			[TXT_MAX];	// 表示する文字列
	int		nSelectNum;	// 現在の選択肢の番号
}Ui_menu_00;

// UI:メニュー[00] の設定情報構造体
typedef struct 
{
	UI_MENU_00_TYPE type;			// 種類
	char			*aString;		// 表示する文字列
	bool			bDecisionSE;	// 決定SEフラグ
}Ui_menu_00Set;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:メニュー[00] の情報を取得
// UI:メニュー[00] の全体管理の情報を取得
// UI:メニュー[00] の現在の選択番号を取得
Ui_menu_00 *GetUi_menu_00(void);
Ui_menu_00Control *GetUi_menu_00Control(void);
int *GetSelect(void);

// UI:メニュー[00] の初期化処理
// UI:メニュー[00] の終了処理
// UI:メニュー[00] の更新処理
// UI:メニュー[00] の描画処理
void InitUi_menu_00(void);
void UninitUi_menu_00(void);
void UpdateUi_menu_00(void);
void DrawUi_menu_00(void);

// UI:メニュー[00] の入力処理
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode);

// UI:メニュー[00] の中心座標を設定
// UI:メニュー[00] の設定処理
void SetUi_menu_00Pos(D3DXVECTOR3 pos);
void SetUi_menu_00(Ui_menu_00Set *pSet, int nNum);

#endif