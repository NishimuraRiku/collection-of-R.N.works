//========================================
// 
// EFF:爆発[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_explosion_00.h ***
//========================================
#ifndef _EFF_EXPLOSION_00_H_	// このマクロ定義がされていなかった時
#define _EFF_EXPLOSION_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:爆発[00] の初期化処理
// EFF:爆発[00] の終了処理
// EFF:爆発[00] の更新処理
// EFF:爆発[00] の描画処理
void InitEff_explosion_00(void);
void UninitEff_explosion_00(void);
void UpdateEff_explosion_00(void);
void DrawEff_explosion_00(void);

// EFF:爆発[00] の読み込み処理
void LoadEff_explosion_00(void);

// EFF:爆発[00] の設定処理
void SetEff_explosion_00(D3DXVECTOR3 pos, int nType);

#endif