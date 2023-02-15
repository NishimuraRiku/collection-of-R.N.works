//========================================
// 
// MD:メインの処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** md_main.h ***
//========================================
#ifndef _MD_MAIN_H_	// このマクロ定義がされていなかった時
#define _MD_MAIN_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************
// MD:メインの状態
typedef enum
{
	MD_MAIN_STATE_NONE,		// 無し
	MD_MAIN_STATE_NORMAL,	// 通常
	MD_MAIN_STATE_MAX,
}MD_MAIN_STATE;

//****************************************
// 構造体の定義
//****************************************
// MD:メインの情報構造体
typedef struct
{
	// 状態関連
	MD_MAIN_STATE state;	// MD:メイン画面[00] の状態
}Md_main;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// MD:メインの初期化処理
void InitMd_main(void);
// MD:メインの終了処理
void UninitMd_main(void);
// MD:メインの更新処理
void UpdateMd_main(void);
// MD:メインの描画処理
void DrawMd_main(void);
//========== *** 取得 ***
// MD:メインの情報を取得
Md_main *GetMd_main(void);
//========== *** 設定 ***
// MD:メインの状態設定処理
void SetStateMd_main(MD_MAIN_STATE state);

#endif