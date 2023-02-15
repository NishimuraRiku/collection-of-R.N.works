//========================================
// 
// EFF:影[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_shadow_00.h ***
//========================================
#ifndef _EFF_SHADOW_00_H_	// このマクロ定義がされていなかった時
#define _EFF_SHADOW_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:影[00] の初期化処理
// EFF:影[00] の終了処理
// EFF:影[00] の更新処理
// EFF:影[00] の描画処理
void InitEff_shadow_00(void);
void UninitEff_shadow_00(void);
void UpdateEff_shadow_00(void);
void DrawEff_shadow_00(void);

// EFF:影[00] の設定処理
void SetEff_shadow_00(D3DXVECTOR3 pos, float fRadius);

#endif