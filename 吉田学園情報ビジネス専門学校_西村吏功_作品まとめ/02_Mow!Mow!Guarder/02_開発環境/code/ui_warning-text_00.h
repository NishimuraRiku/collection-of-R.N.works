//========================================
// 
// UI:注意テキスト[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_warning-text_00.h ***
//========================================
#ifndef _UI_WARNINGTEXT_00_H_	// このマクロ定義がされていなかった時
#define _UI_WARNINGTEXT_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
// UI:注意テキスト[00] の初期化処理
// UI:注意テキスト[00] の終了処理
// UI:注意テキスト[00] の更新処理
// UI:注意テキスト[00] の描画処理
void InitUi_warningText_00(void);
void UninitUi_warningText_00(void);
void UpdateUi_warningText_00(void);
void DrawUi_warningText_00(void);

#endif