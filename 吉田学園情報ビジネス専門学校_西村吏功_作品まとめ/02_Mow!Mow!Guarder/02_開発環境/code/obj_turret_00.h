//========================================
// 
// OBJ:タレット[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_turret_00.h ***
//========================================
#ifndef _OBJ_TURRET_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_TURRET_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"

//****************************************
// 列挙型の定義
//****************************************
// OBJ:タレット[00] の管理状態
typedef enum
{
	OBJ_TURRET_00_CONTROL_STATE_DYNAMIC,	// 動的
	OBJ_TURRET_00_CONTROL_STATE_STATIC,		// 静的
	OBJ_TURRET_00_CONTROL_STATE_MAX,
}OBJ_TURRET_00_CONTROL_STATE;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:タレット[00] の初期化処理
// OBJ:タレット[00] の終了処理
// OBJ:タレット[00] の更新処理
// OBJ:タレット[00] の描画処理
void InitObj_turret_00(void);
void UninitObj_turret_00(void);
void UpdateObj_turret_00(void);
void DrawObj_turret_00(void);

// OBJ:タレット[00] の読み込み処理
void LoadObj_turret_00(void);

// OBJ:タレット[00] の設定処理
void SetObj_turret_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

// OBJ:タレット[00] との当たり判定処理
int CollisionObj_turret_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:タレット[00] のダメージ処理
// OBJ:タレット[00] の全て回復処理
void DamageObj_turret_00(int nIndex, int nDamage);
void HealAllObj_turret_00(void);

// OBJ:タレット[00] の管理状態設定処理
void SetObj_turret_00ControlState(OBJ_TURRET_00_CONTROL_STATE state);

#endif