//========================================
// 
// UI:商品ガイド[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_item-guid_00.h ***
//========================================
#ifndef _UI_ITEMGUID_00_H_	// このマクロ定義がされていなかった時
#define _UI_ITEMGUID_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
// UI:商品ガイド[00] の到着にかかる時間
#define UI_ITEMGUID_00_ARRIVAL_TIME	(10)

// UI:商品ガイド[00] の非到着時の相対位置Y
#define UI_ITEMGUID_00_NON_ARRIVAL_RELATIVE_POS_Y	(-10)

//****************************************
// 構造体の定義
//****************************************
// UI:商品ガイド[00] の情報構造体
typedef struct
{
	int nTex;				// テクスチャ番号
	int nCounterArrival;	// 到着カウンター
}Ui_itemGuid_00;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:商品ガイド[00] の情報取得処理
Ui_itemGuid_00 *GetUi_itemGuid_00(void);

// UI:商品ガイド[00] の初期化処理
// UI:商品ガイド[00] の終了処理
// UI:商品ガイド[00] の更新処理
// UI:商品ガイド[00] の描画処理
void InitUi_itemGuid_00(void);
void UninitUi_itemGuid_00(void);
void UpdateUi_itemGuid_00(void);
void DrawUi_itemGuid_00(void);

#endif