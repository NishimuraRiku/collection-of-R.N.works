//========================================
// 
// EFF:パーティクル[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_particle_00.h ***
//========================================
#ifndef _EFF_PARTICLE_00_H_	// このマクロ定義がされていなかった時
#define _EFF_PARTICLE_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:パーティクル[00] の初期化処理
// EFF:パーティクル[00] の終了処理
// EFF:パーティクル[00] の更新処理
void InitEff_particle_00(void);
void UninitEff_particle_00(void);
void UpdateEff_particle_00(void);

// EFF:パーティクル[00] の読み込み処理
void LoadEff_particle_00(void);

// EFF:パーティクル[00] の設定処理
void SetEff_particle_00(D3DXVECTOR3 pos, int nType);

#endif