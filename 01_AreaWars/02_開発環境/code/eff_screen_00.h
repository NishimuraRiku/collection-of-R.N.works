//========================================
// *** eff_screen_00.h ***
//========================================
#ifndef _EFF_SCREEN_00_H_	// このマクロ定義がされていなかった時
#define _EFF_SCREEN_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************

// スクリーン[00] の初期化処理
void InitEff_screen_00(void);

// スクリーン[00] の終了処理
void UninitEff_screen_00(void);

// スクリーン[00] の更新処理
void UpdateEff_screen_00(void);

// スクリーン[00] の描画処理
void DrawEff_screen_00(void);

// スクリーン[00] の設定処理
void SetEff_screen_00(int nTime, Color col);

#endif