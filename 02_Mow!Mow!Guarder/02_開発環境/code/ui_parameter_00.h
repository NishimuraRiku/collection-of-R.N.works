//========================================
// 
// UI:パラメーター[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_parameter_00.h ***
//========================================
#ifndef _UI_PARAMETER_00_H_	// このマクロ定義がされていなかった時
#define _UI_PARAMETER_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// UI:パラメーター[00] の初期化処理
// UI:パラメーター[00] の終了処理
// UI:パラメーター[00] の更新処理
// UI:パラメーター[00] の描画処理
void InitUi_parameter_00(void);
void UninitUi_parameter_00(void);
void UpdateUi_parameter_00(void);
void DrawUi_parameter_00(void);

#endif