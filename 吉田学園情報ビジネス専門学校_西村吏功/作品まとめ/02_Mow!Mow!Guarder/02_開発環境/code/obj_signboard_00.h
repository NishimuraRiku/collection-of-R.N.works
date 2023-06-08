//========================================
// 
// OBJ:看板[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_signboard_00.h ***
//========================================
#ifndef _OBJ_SIGNBOARD_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_SIGNBOARD_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"

//****************************************
// 列挙型の定義
//****************************************
// OBJ:看板[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3	pos;	// 位置
	D3DXVECTOR3	rot;	// 向き

	// 分類関連
	int nType;	// 種類

	// 状態関連
	bool bUse;	// 使用されているかフラグ

	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報
}Obj_signboard_00;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:看板[00] の情報取得処理
Obj_signboard_00 *GetObj_signboard_00(void);
// OBJ:看板[00] の初期化処理
void InitObj_signboard_00(void);
// OBJ:看板[00] の終了処理
void UninitObj_signboard_00(void);
// OBJ:看板[00] の更新処理
void UpdateObj_signboard_00(void);
// OBJ:看板[00] の描画処理
void DrawObj_signboard_00(void);
// OBJ:看板[00] の読み込み処理
void LoadObj_signboard_00(void);
// OBJ:看板[00] の設定処理
void SetObj_signboard_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
// OBJ:看板[00] との当たり判定処理
int CollisionObj_signboard_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

#endif