//========================================
//  *** item_bomb_00.h ***
//========================================
#ifndef _ITEM_BOMB_00_H_	// このマクロ定義がされていなかった時
#define _ITEM_BOMB_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************

// 爆弾[00] の最大数
#define ITEM_BOMB_00_MAX	(256)

//****************************************
// 構造体の定義
//****************************************

// 爆弾[00] 構造体の定義
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置

	// 状態値
	bool	bUse;			// 使用されているかフラグ
	int		nLife;			// 寿命
	int		nCounterBrink;	// 点滅のカウンター
	bool	bBrink;			// 点滅フラグ

	// テクスチャ関連
	int nCounterAnim;	// アニメーションのカウンター
	int nPattern;		// パターンNo.
}Item_bomb_00;

//****************************************
// プロトタイプ宣言
//****************************************

// 爆弾[00] の初期化処理
void InitItem_bomb_00(void);

// 爆弾[00] の終了処理
void UninitItem_bomb_00(void);

// 爆弾[00] の更新処理
void UpdateItem_bomb_00(void);

// 爆弾[00] の描画処理
void DrawItem_bomb_00(void);

// 爆弾[00] の設定処理
void SetItem_bomb_00(D3DXVECTOR3 pos);

// 爆弾[00] の情報を取得
Item_bomb_00 *GetItem_bomb_00(void);

#endif