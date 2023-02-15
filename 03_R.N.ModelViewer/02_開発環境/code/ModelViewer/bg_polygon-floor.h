//========================================
// 
// BG:ポリゴン床の処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** bg_polygonFloor_00.h ***
//========================================
#ifndef _BG_POLYGONFLOOR_H_	// このマクロ定義がされていなかった時
#define _BG_POLYGONFLOOR_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// BG:ポリゴン床の初期化処理
void InitBg_polygonFloor(void);
// BG:ポリゴン床の終了処理
void UninitBg_polygonFloor(void);
// BG:ポリゴン床の更新処理
void UpdateBg_polygonFloor(void);
// BG:ポリゴン床の描画処理
void DrawBg_polygonFloor(void);

#endif