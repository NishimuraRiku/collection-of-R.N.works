//========================================
// *** eff_laser_pointer_00.h ***
//========================================
#ifndef _EFF_LASER_POINTER_00_H_	// このマクロ定義がされていなかった時
#define _EFF_LASER_POINTER_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

typedef enum
{
	EFF_LASER_POINTER_00_STATE_POP = 0,		// 出現
	EFF_LASER_POINTER_00_STATE_IN_CLEAR,	// 消え中
	EFF_LASER_POINTER_00_STATE_CLEAR,		// 消えている
}EFF_LASER_POINTER_00_STATE;

//****************************************
// 構造体の定義
//****************************************

// レーザーポインタ―[00] の情報構造体
typedef struct
{
	D3DXVECTOR3					rot;			// 向き
	EFF_LASER_POINTER_00_STATE	state;			// 状態
	int							nCounterState;	// 状態カウンター
	float						fAlpha;			// 全体の透明度
}Eff_laser_pointer_00;

//****************************************
// プロトタイプ宣言
//****************************************

// レーザーポインタ―[00] の情報を取得
Eff_laser_pointer_00 *GetEff_laser_pointer_00(void);

// レーザーポインタ―[00] の初期化処理
void InitEff_laser_pointer_00(void);

// レーザーポインタ―[00] の終了処理
void UninitEff_laser_pointer_00(void);

// レーザーポインタ―[00] の更新処理
void UpdateEff_laser_pointer_00(void);

// レーザーポインタ―[00] の描画処理
void DrawEff_laser_pointer_00(void);

// レーザーポインタ―[00] の状態設定処理
void SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE state);

#endif