//========================================
// 
// UI:インベントリ[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_inventory_00.h ***
//========================================
#ifndef _UI_INVENTORY_00_H_	// このマクロ定義がされていなかった時
#define _UI_INVENTORY_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
// UI:インベントリ[00] の初期化処理
// UI:インベントリ[00] の終了処理
// UI:インベントリ[00] の更新処理
// UI:インベントリ[00] の描画処理
void InitUi_inventory_00(void);
void UninitUi_inventory_00(void);
void UpdateUi_inventory_00(void);
void DrawUi_inventory_00(void);

#endif