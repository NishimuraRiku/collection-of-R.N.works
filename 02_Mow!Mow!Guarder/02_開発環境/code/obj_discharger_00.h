//========================================
// 
// OBJ:放電装置[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_discharger_00.h ***
//========================================
#ifndef _OBJ_DISCHARGER_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_DISCHARGER_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"

//****************************************
// 列挙型の定義
//****************************************
// OBJ:放電装置[00] の管理状態
typedef enum
{
	OBJ_DISCHARGER_00_CONTROL_STATE_DYNAMIC,	// 動的
	OBJ_DISCHARGER_00_CONTROL_STATE_STATIC,		// 静的
	OBJ_DISCHARGER_00_CONTROL_STATE_MAX,
}OBJ_DISCHARGER_00_CONTROL_STATE;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:放電装置[00] の初期化処理
// OBJ:放電装置[00] の終了処理
// OBJ:放電装置[00] の更新処理
// OBJ:放電装置[00] の描画処理
void InitObj_discharger_00(void);
void UninitObj_discharger_00(void);
void UpdateObj_discharger_00(void);
void DrawObj_discharger_00(void);

// OBJ:放電装置[00] の読み込み処理
void LoadObj_discharger_00(void);

// OBJ:放電装置[00] の設定処理
void SetObj_discharger_00(D3DXVECTOR3 pos);

// OBJ:放電装置[00] との当たり判定処理
int CollisionObj_discharger_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:放電装置[00] のダメージ処理
// OBJ:放電装置[00] の全て回復処理
void DamageObj_discharger_00(int nIndex, int nDamage);
void HealAllObj_discharger_00(void);

// OBJ:放電装置[00] の管理状態設定処理
void SetObj_discharger_00ControlState(OBJ_DISCHARGER_00_CONTROL_STATE state);

#endif