//========================================
// *** img_fighter_00.h ***
//========================================
#ifndef _IMG_FIGHTER_00_H_	// このマクロ定義がされていなかった時
#define _IMG_FIGHTER_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// 機体[00] の状態
typedef enum
{
	IMG_FIGHTER_00_STATE_POP = 0,		// 出現
	IMG_FIGHTER_00_STATE_IN_POP,		// 出現中
	IMG_FIGHTER_00_STATE_MAX,
}IMG_FIGHTER_00_STATE;

//****************************************
// 構造体の定義
//****************************************

// 機体[00] の情報構造体
typedef struct
{
	IMG_FIGHTER_00_STATE	state;				// 状態
	int						nCounterState;		// 状態カウンター
	float					fAlpha;				// 全体の透明度
	D3DXVECTOR3				pos;				// 位置
	int						nCounterFireBrink;	// 炎の点滅カウンター
	bool					bFireBrink;			// 炎の点滅フラグ
}Img_fighter_00;

//****************************************
// プロトタイプ宣言
//****************************************

// 機体[00] の情報を取得
Img_fighter_00 *GetImg_fighter_00(void);

// 機体[00] の初期化処理
void InitImg_fighter_00(void);

// 機体[00] の終了処理
void UninitImg_fighter_00(void);

// 機体[00] の更新処理
void UpdateImg_fighter_00(void);

// 機体[00] の描画処理
void DrawImg_fighter_00(void);

// 機体[00] の状態設定処理
void SetStateImg_fighter_00(IMG_FIGHTER_00_STATE state);

#endif