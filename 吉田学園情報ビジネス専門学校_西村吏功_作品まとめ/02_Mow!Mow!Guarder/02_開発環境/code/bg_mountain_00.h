//========================================
// 
// BG:山[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** bg_mountain_00.h ***
//========================================
#ifndef _BG_MOUNTAIN_00_H_	// このマクロ定義がされていなかった時
#define _BG_MOUNTAIN_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
// BG:山[00] の初期化処理
// BG:山[00] の終了処理
// BG:山[00] の更新処理
// BG:山[00] の描画処理
void InitBg_mountain_00(void);
void UninitBg_mountain_00(void);
void UpdateBg_mountain_00(void);
void DrawBg_mountain_00(void);

#endif