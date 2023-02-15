//========================================
// 
// 光のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** light.h ***
//========================================
#ifndef _LIGHT_H_	// このマクロ定義がされていなかった時
#define _LIGHT_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "RNmain.h"

//****************************************
// 構造体の定義
//****************************************
// 光(2D)の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	float fLength;		// 距離

	// 状態関連
	bool bUse;	// 使用されているかフラグ
	Color col;	// 色
}Light2D;
// 光(3D)の管理情報構造体
typedef struct
{
	Color col;			// 色
	float fBrightness;	// 明るさ
}Light3DControl;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// 光の初期化処理
void InitLight(void);
// 光の終了処理
void UninitLight(void);
// 光の更新処理
void UpdateLight(void);
//========== *** 光(2D) ***
// 光(2D)の予約処理
void ReserveLight2D(D3DXVECTOR3 pos, Color col, float fLength);
// 光(2D)との距離から色を取得
Color GetColorForLight2D(D3DXVECTOR3 pos, Color myCol);
//========== *** 光(3D) ***
// 光(3D)の管理情報を取得
Light3DControl *GetLight3DControl(void);
// 光(3D)の明るさ設定処理
void SetBrightnessLight3D(float fBrightness);
// 光(3D)の色設定処理
void SetColorLight3D(Color col);

#endif