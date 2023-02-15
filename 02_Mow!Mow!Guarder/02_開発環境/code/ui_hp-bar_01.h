//========================================
// 
// UI:HPバー[01] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_hp-bar_01.h ***
//========================================
#ifndef _UI_HPBAR_01_H_	// このマクロ定義がされていなかった時
#define _UI_HPBAR_01_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// UI:HPバー[01] の初期化処理
// UI:HPバー[01] の終了処理
// UI:HPバー[01] の更新処理
// UI:HPバー[01] の描画処理
void InitUi_hpBar_01(void);
void UninitUi_hpBar_01(void);
void UpdateUi_hpBar_01(void);
void DrawUi_hpBar_01(void);

// UI:HPバー[01] の読み込み処理
void LoadUi_hpBar_01(void);

// UI:HPバー[01] の設定処理
void SetUi_hpBar_01(D3DXVECTOR3 pos, int nType);

#endif