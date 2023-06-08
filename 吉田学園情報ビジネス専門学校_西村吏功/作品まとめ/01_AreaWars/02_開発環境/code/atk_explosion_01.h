//========================================
//  *** atk_explosion_01.h ***
//========================================
#ifndef _ATK_EXPLOSION_01_H_	// このマクロ定義がされていなかった時
#define _ATK_EXPLOSION_01_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// 爆発[00] の種類
typedef enum
{
	ATK_EXPLOSION_01_TYPE_00 = 0,
	ATK_EXPLOSION_01_TYPE_MAX,
}ATK_EXPLOSION_01_TYPE;

//****************************************
// プロトタイプ宣言
//****************************************
void InitAtk_explosion_01	(void);															// 爆発[00] の初期化処理
void UninitAtk_explosion_01	(void);															// 爆発[00] の終了処理
void UpdateAtk_explosion_01	(void);															// 爆発[00] の更新処理
void DrawAtk_explosion_01	(void);															// 爆発[00] の描画処理
void SetAtk_explosion_01	(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ATK_EXPLOSION_01_TYPE type);	// 爆発[00] の設定処理

#endif