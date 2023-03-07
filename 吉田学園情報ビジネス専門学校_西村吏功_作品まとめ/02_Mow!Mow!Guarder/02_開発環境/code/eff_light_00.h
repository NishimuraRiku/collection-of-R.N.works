//========================================
// 
// EFF:光[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_light_00.h ***
//========================================
#ifndef _EFF_LIGHT_00_H_	// このマクロ定義がされていなかった時
#define _EFF_LIGHT_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:光[00] の初期化処理
// EFF:光[00] の終了処理
// EFF:光[00] の更新処理
// EFF:光[00] の描画処理
void InitEff_light_00(void);
void UninitEff_light_00(void);
void UpdateEff_light_00(void);
void DrawEff_light_00(void);

// EFF:光[00] の読み込み処理
void LoadEff_light_00(void);

// EFF:光[00] の設定処理
void SetEff_light_00(D3DXVECTOR3 pos, int nType, float fScale);

#endif