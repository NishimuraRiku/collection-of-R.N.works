//========================================
//  *** ui_score_00.h ***
//========================================
#ifndef _UI_SCORE_00_H_	// このマクロ定義がされていなかった時
#define _UI_SCORE_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
#define UI_SCORE_00_MAX	(32)	// スコア[00] の最大数

//****************************************
// 構造体の定義 
//****************************************

// スコア[00]構造体の定義
typedef struct
{
						//========== *** 位置関連 ***
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 向き
	float fWidth;		// 幅
	float fHeight;		// 高さ
						//========== *** 分類情報 ***
	bool bUse;			// 使用されているかフラグ
						//========== *** 状態値 ***
	int nLife;			// 寿命
	int nScore;			// スコア量
	int nCounterBrink;	// 点滅カウンター
	bool bBrink;		// 点滅フラグ
}Ui_score_00;

//========== プロトタイプ宣言	==========// 
Ui_score_00 *GetUi_score_00(void);					// スコア[00] の情報を取得
void InitUi_score_00(void);							// スコア[00] の初期化処理
void UninitUi_score_00(void);						// スコア[00] の終了処理
void UpdateUi_score_00(void);						// スコア[00] の更新処理
void SetUi_score_00(D3DXVECTOR3 pos, int nScore);	// スコア[00] の設定処理

#endif