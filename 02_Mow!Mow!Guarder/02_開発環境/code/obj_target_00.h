//========================================
// 
// OBJ:ターゲット[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_target_00.h ***
//========================================
#ifndef _OBJ_TARGET_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_TARGET_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ターゲット[00] の初期化処理
// OBJ:ターゲット[00] の終了処理
// OBJ:ターゲット[00] の更新処理
// OBJ:ターゲット[00] の描画処理
void InitObj_target_00(void);
void UninitObj_target_00(void);
void UpdateObj_target_00(void);
void DrawObj_target_00(void);

// OBJ:ターゲット[00] の読み込み処理
void LoadObj_target_00(void);

#endif