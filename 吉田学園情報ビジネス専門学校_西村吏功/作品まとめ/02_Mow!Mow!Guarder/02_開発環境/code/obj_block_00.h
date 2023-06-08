//========================================
// 
// OBJ:ブロック[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_block_00.h ***
//========================================
#ifndef _OBJ_BLOCK_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_BLOCK_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ブロック[00] の初期化処理
// OBJ:ブロック[00] の終了処理
// OBJ:ブロック[00] の更新処理
// OBJ:ブロック[00] の描画処理
void InitObj_block_00(void);
void UninitObj_block_00(void);
void UpdateObj_block_00(void);
void DrawObj_block_00(void);

// OBJ:ブロック[00] の読み込み処理
void LoadObj_block_00(void);

// OBJ:ブロック[00] の設定処理
void SetObj_block_00(D3DXVECTOR3 pos);

// OBJ:ブロック[00] との当たり判定処理
int CollisionObj_block_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:ブロック[00] のダメージ処理
// OBJ:ブロック[00] の全て回復処理
void DamageObj_block_00(int nIndex, int nDamage);
void HealAllObj_block_00(void);

#endif