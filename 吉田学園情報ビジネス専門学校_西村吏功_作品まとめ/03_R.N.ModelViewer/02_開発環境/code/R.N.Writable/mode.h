//========================================
// 
// モードのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** mode.h ***
//========================================
#ifndef _MODE_H_	// このマクロ定義がされていなかった時
#define _MODE_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "../R.N.Lib/RNmain.h"

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// モードに応じた初期化処理
void ModeInit(MODE mode);
// モードに応じた終了処理
void ModeUninit(MODE mode);
// モードに応じた更新処理
void ModeUpdate(MODE mode);
// モードに応じた描画処理
void ModeDraw(MODE mode);

#endif