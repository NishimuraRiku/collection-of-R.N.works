//========================================
// *** ui_menu_00.h ***
//========================================
#ifndef UI_MENU_00_H_	// このマクロ定義がされていなかった時
#define UI_MENU_00_H_	// 二重インクルード防止のマクロを定義する

//========== マクロ定義			==========// 
#define UI_MENU_00_DEFAULT_SPACE_X	(PIXEL * 0)		// メニュー[00] の配置間隔X
#define UI_MENU_00_DEFAULT_SPACE_Y	(PIXEL * 16)	// メニュー[00] の配置間隔Y
#define UI_MENU_00_CLEAR_TIME		(4)				// メニュー[00] が消えるのにかかる時間

//========== 列挙型の定義		==========// 

// メニュー[00] の入力モード
typedef enum
{
	UI_MENU_00_INPUT_MODE_UP_AND_DOWN = 0,
	UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT,
	UI_MENU_00_INPUT_MODE_MAX
}UI_MENU_00_INPUT_MODE;

// メニュー[00] の種類
typedef enum
{
	UI_MENU_00_TYPE_NORMAL = 0,	// 通常
	UI_MENU_00_TYPE_SELECTION,	// 選択
	UI_MENU_00_TYPE_MAX
}UI_MENU_00_TYPE;

// メニュー[00] の状態
typedef enum
{
	UI_MENU_00_STATE_NONE = 0,
	UI_MENU_00_STATE_IN_POP,	// 出現中
	UI_MENU_00_STATE_POP,		// 出現
	UI_MENU_00_STATE_IN_CLEAR,	// 消え中
	UI_MENU_00_STATE_MAX
}UI_MENU_00_STATE;

//****************************************
// 構造体の定義
//****************************************

// メニュー[00] の全体管理構造体
typedef struct
{
	UI_MENU_00_STATE	state;			// 状態
	int					nCounterState;	// 状態のカウンター
	ScaleDiameter		scale;			// 拡大率
}Ui_menu_00Control;

// メニュー[00] の情報構造体
typedef struct
{
											//========== *** 位置関連 ***
	D3DXVECTOR3			pos;				// 位置
	D3DXVECTOR3			rot;				// 向き
	float				fWidth;				// 幅
	float				fHeight;			// 高さ
											//========== *** 分類情報 ***
	UI_MENU_00_TYPE		type;				// 種類
	bool				bUse;				// 使用されているかフラグ
	bool				bSelect;			// 選択されているかフラグ
											//========== *** 状態値 ***
	char				aString[TXT_MAX];	// 表示する文字列
	int					nSelectNum;			// 現在の選択肢の番号
											//========== *** テクスチャ関連 ***
	int					nCounterAnim;		// アニメーションのカウンター
	int					nAnimChangeTime;	// アニメーションの切り替え時間
	int					nPattern;			// パターンNo.
}Ui_menu_00;

// メニュー[00] の設定用の情報構造体
typedef struct 
{
	UI_MENU_00_TYPE type;	// 種類
	char *aString;			// 表示する文字列
}Ui_menu_00_set;

//========== プロトタイプ宣言	==========// 
void InitUi_menu_00(void);															// メニュー[00] の初期化処理
void UninitUi_menu_00(void);														// メニュー[00] の終了処理
void UpdateUi_menu_00(void);														// メニュー[00] の更新処理
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode);									// メニュー[00] の入力処理
void DrawUi_menu_00(void);															// メニュー[00] の描画処理
void SetUi_menu_00Pos(D3DXVECTOR3 pos);												// メニュー[00] の中心座標を設定
void SetUi_menu_00(Ui_menu_00_set *set, int nMax, float fSpaseX, float fSpaceY);	// メニュー[00] の設定処理
Ui_menu_00 *GetUi_menu_00(void);													// メニュー[00] の情報を取得
Ui_menu_00Control *GetUi_menu_00Control(void);										// メニュー[00] の全体管理の情報を取得
int *GetSelect(void);																// メニュー[00] の現在の選択番号を取得

#endif