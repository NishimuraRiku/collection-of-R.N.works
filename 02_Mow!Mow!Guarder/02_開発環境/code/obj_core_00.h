//========================================
// 
// OBJ:コア[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_core_00.h ***
//========================================
#ifndef _OBJ_CORE_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_CORE_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"
#include "polygon3D.h"

//****************************************
// 列挙型の定義
//****************************************
// OBJ:コア[00] のモーション番号
typedef enum
{
	OBJ_CORE_00_MOTION_WAIT,	// 待機
	OBJ_CORE_00_MOTION_MAX,
}OBJ_CORE_00_MOTION;

//****************************************
// 構造体の定義
//****************************************
// OBJ:コア[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 現在の位置
	D3DXVECTOR3 rot;	// 向き
	
	// 分類関連
	int	nType;	// 種類

	// 状態関連
	bool	bUse;				// 使用されているかフラグ
	int		nHP;				// ヒットポイント
	int		nHPMax;				// ヒットポイントの上限
	int		nCounterInvincible;	// 無敵カウンター
	int		nCounterDamage;		// ダメージカウンター
	int		nCounterWarning;	// 警告カウンター
	bool	bWarning;			// 警告フラグ

	// 部品関連
	Parts3DInfo			partsInfo;	// 部品管理
	OBJ_CORE_00_MOTION	motion;		// モーション
}Obj_core_00;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:コア[00] の情報を取得
Obj_core_00 *GetObj_core_00(void);

// OBJ:コア[00] の初期化処理
// OBJ:コア[00] の終了処理
// OBJ:コア[00] の更新処理
// OBJ:コア[00] の描画処理
void InitObj_core_00(void);
void UninitObj_core_00(void);
void UpdateObj_core_00(void);
void DrawObj_core_00(void);

// OBJ:コア[00] の読み込み処理
void LoadObj_core_00(void);

// OBJ:コア[00] との当たり判定処理
int CollisionObj_core_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:コア[00] のダメージ処理
// OBJ:コア[00] の破壊処理
void DamageObj_core_00(int nDamage);
void DestroyObj_core_00(void);

#endif