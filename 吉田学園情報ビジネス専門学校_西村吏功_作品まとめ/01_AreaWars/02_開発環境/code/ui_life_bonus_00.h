//========================================
//  *** ui_life_bonus_00.h ***
//========================================
#ifndef _UI_LIFE_BONUS_00_H_	// このマクロ定義がされていなかった時
#define _UI_LIFE_BONUS_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 構造体の定義 
//****************************************

// ライフボーナス[00] 構造体の定義
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置

	// 状態関連
	int		nLifeBonus;		// 表示するライフボーナス
	bool	bUse;			// 使用されているかフラグ
	int		nLife;			// 寿命
	int		nCounterBrink;	// 点滅カウンター
	bool	bBrink;			// 点滅フラグ
}Ui_life_bonus_00;

//****************************************
// プロトタイプ宣言
//****************************************

// ライフボーナス[00] の情報を取得
Ui_life_bonus_00 *GetUi_life_bonus_00(void);

// ライフボーナス[00] の初期化処理
void InitUi_life_bonus_00(void);

// ライフボーナス[00] の終了処理
void UninitUi_life_bonus_00(void);

// ライフボーナス[00] の更新処理
void UpdateUi_life_bonus_00(void);

// ライフボーナス[00] の設定処理
void SetUi_life_bonus_00(int nLifeBonus);

#endif