//========================================
// 
// OBJ:スイッチ[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_switch_00.h ***
//========================================
#ifndef _OBJ_SWITCH_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_SWITCH_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"
#include "polygon3D.h"

//****************************************
// 列挙型の定義
//****************************************
// OBJ:スイッチ[00] のモーション番号
typedef enum
{
	OBJ_SWITCH_00_MOTION_WAIT,	// 待機
	OBJ_SWITCH_00_MOTION_PUSH,	// 押下
	OBJ_SWITCH_00_MOTION_MAX,
}OBJ_SWITCH_00_MOTION;

//****************************************
// 構造体の定義
//****************************************
// OBJ:スイッチ[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き

	// 分類関連
	int	nType;	// 種類

	// 部品関連
	Parts3DInfo				partsInfo;	// 部品管理
	OBJ_SWITCH_00_MOTION	motion;		// モーション
}Obj_switch_00;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:スイッチ[00] の情報を取得
Obj_switch_00 *GetObj_switch_00(void);

// OBJ:スイッチ[00] の初期化処理
// OBJ:スイッチ[00] の終了処理
// OBJ:スイッチ[00] の更新処理
// OBJ:スイッチ[00] の描画処理
void InitObj_switch_00(void);
void UninitObj_switch_00(void);
void UpdateObj_switch_00(void);
void DrawObj_switch_00(void);

// OBJ:スイッチ[00] の読み込み処理
void LoadObj_switch_00(void);

// OBJ:スイッチ[00] との当たり判定処理
int CollisionObj_switch_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:スイッチ[00] の押下処理
void PushObj_switch_00(void);

#endif