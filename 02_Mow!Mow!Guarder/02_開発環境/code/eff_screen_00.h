//========================================
// 
// EFF:スクリーン[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_screen_00.h ***
//========================================
#ifndef _EFF_SCREEN_00_H_	// このマクロ定義がされていなかった時
#define _EFF_SCREEN_00_H_	// 二重インクルード防止のマクロを定義する

#include "eff_screen_00.h"

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:スクリーン[00] の初期化処理
// EFF:スクリーン[00] の終了処理
// EFF:スクリーン[00] の更新処理
// EFF:スクリーン[00] の描画処理
void InitEff_screen_00(void);
void UninitEff_screen_00(void);
void UpdateEff_screen_00(void);
void DrawEff_screen_00(void);

// EFF:スクリーン[00] の設定処理
void SetEff_screen_00(int nTime, Color col);

#endif