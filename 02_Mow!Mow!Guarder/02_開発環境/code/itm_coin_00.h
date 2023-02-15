//========================================
// 
// ITM:コイン[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** itm_coin_00.h ***
//========================================
#ifndef _ITM_COIN_00_H_	// このマクロ定義がされていなかった時
#define _ITM_COIN_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"

//****************************************
// プロトタイプ宣言
//****************************************
// ITM:コイン[00] の初期化処理
// ITM:コイン[00] の終了処理
// ITM:コイン[00] の更新処理
// ITM:コイン[00] の描画処理
void InitItm_coin_00(void);
void UninitItm_coin_00(void);
void UpdateItm_coin_00(void);
void DrawItm_coin_00(void);

// ITM:コイン[00] の読み込み処理
void LoadItm_coin_00(void);

// ITM:コイン[00] の設定処理
void SetItm_coin_00(D3DXVECTOR3 pos, int nSetNum);

// ITM:コイン[00] との当たり判定処理
// ITM:コイン[00] の破壊処理
int CollisionItm_coin_00(VECTOR vector, Collision *pItmCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);
void DestroyItm_coin_00(int nIndex);

#endif