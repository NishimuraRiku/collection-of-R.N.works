//========================================
// 
// ATK:剣[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** atk_sword_00.h ***
//========================================
#ifndef _ATK_SWORD_00_H_	// このマクロ定義がされていなかった時
#define _ATK_SWORD_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"
#include "sound.h"

//****************************************
// 列挙型の定義
//****************************************
// ATK:剣[00] の種類毎の情報構造体
typedef struct
{
	float		fKnockBack;	// ノックバック
	char		aModelPath	// モデルの相対パス
				[TXT_MAX];
	int			nLightType;	// 光の種類
	SOUND_LABEL slashSE;	// 斬撃SE
	HitTestSet	hitTestSet;	// 当たり判定設定情報
}Atk_sword_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// ATK:剣[00] の初期化処理
// ATK:剣[00] の終了処理
// ATK:剣[00] の更新処理
// ATK:剣[00] の描画処理
void InitAtk_sword_00(void);
void UninitAtk_sword_00(void);
void UpdateAtk_sword_00(void);
void DrawAtk_sword_00(void);

// ATK:剣[00] の読み込み処理
void LoadAtk_sword_00(void);

// ATK:剣[00] の種類毎の情報を取得
Atk_sword_00Type *GetAtk_sword_00Type(void);

// ATK:剣[00] との当たり判定処理
void CollisionAtk_sword_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

#endif