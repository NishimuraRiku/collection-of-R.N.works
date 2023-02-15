//========================================
// 
// OBJ:ミラー[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_mirror_00.h ***
//========================================
#ifndef _OBJ_MIRROR_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_MIRROR_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ミラー[00] の初期化処理
// OBJ:ミラー[00] の終了処理
// OBJ:ミラー[00] の更新処理
// OBJ:ミラー[00] の描画処理
void InitObj_mirror_00(void);
void UninitObj_mirror_00(void);
void UpdateObj_mirror_00(void);
void DrawObj_mirror_00(void);

// OBJ:ミラー[00] の読み込み処理
void LoadObj_mirror_00(void);

// OBJ:ミラー[00] の設定処理
void SetObj_mirror_00(D3DXVECTOR3 pos);

// OBJ:ミラー[00] との当たり判定処理
int CollisionObj_mirror_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:ミラー[00] のダメージ処理
// OBJ:ミラー[00] の全て回復処理
void DamageObj_mirror_00(int nIndex, int nDamage);
void HealAllObj_mirror_00(void);

#endif