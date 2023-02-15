//========================================
// 
// ATK:弾[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** atk_bullet_00.h ***
//========================================
#ifndef _ATK_BULLET_00_H_	// このマクロ定義がされていなかった時
#define _ATK_BULLET_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 列挙型の定義
//****************************************
// ATK:弾[00] の親の種類
typedef enum
{
	ATK_BULLET_00_PARENTTYPE_PLAYER,	// プレイヤー
	ATK_BULLET_00_PARENTTYPE_ENEMY,		// 敵
	ATK_BULLET_00_PARENTTYPE_MAX,
}ATK_BULLET_00_PARENTTYPE;

//****************************************
// プロトタイプ宣言
//****************************************
// ATK:弾[00] の初期化処理
// ATK:弾[00] の終了処理
// ATK:弾[00] の更新処理
// ATK:弾[00] の描画処理
void InitAtk_bullet_00(void);
void UninitAtk_bullet_00(void);
void UpdateAtk_bullet_00(void);
void DrawAtk_bullet_00(void);

// ATK:弾[00] の読み込み処理
void LoadAtk_bullet_00(void);

// ATK:弾[00] の設定処理
void SetAtk_bullet_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_BULLET_00_PARENTTYPE parentType);

#endif