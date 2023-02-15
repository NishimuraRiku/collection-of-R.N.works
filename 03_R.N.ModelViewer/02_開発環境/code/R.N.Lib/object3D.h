//========================================
// 
// オブジェクト3Dのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** object3D.h ***
//========================================
#ifndef _OBJECT3D_H_	// このマクロ定義がされていなかった時
#define _OBJECT3D_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "RNmain.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"

//****************************************
// マクロ定義
//****************************************
// オブジェクト3Dの最大数
#define OBJECT3D_MAX (128)

//****************************************
// 構造体の定義
//****************************************
// オブジェクト3Dの情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 現在の位置
	D3DXVECTOR3 rot;	// 向き

	// 分類関連
	int nType;	// 種類

	// 状態関連
	bool bUse;	// 使用されているフラグ

	// 部品関連
	Parts3DInfo partsInfo;	// 部品管理
	int	 nMotion;			// モーション
}Object3D;
// オブジェクト3Dの管理情報構造体
typedef struct
{
	int nTypeNum;	// 種類数
}Object3DControl;
// オブジェクト3Dの種類毎の情報構造体
typedef struct
{
	Parts3DSet partsSet;	// 部品設定情報(3D)
	Motion3DSet motionSet;	// モーション設定情報
}Object3DType;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// オブジェクト3Dの初期化処理
void InitObject3D(void);
// オブジェクト3Dの終了処理
void UninitObject3D(void);
// オブジェクト3Dの更新処理
void UpdateObject3D(void);
// オブジェクト3Dの描画処理
void DrawObject3D(void);
//========== *** 描画 ***
// オブジェクト3Dの例外描画処理
void ExcDrawObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, Color col);
//========== *** 入出力 ***
// オブジェクト3Dの読み込み処理
void LoadObject3D(void);
//========== *** 取得 ***
// オブジェクト3Dの情報を取得
Object3D *GetObject3D(void);
// オブジェクト3Dの管理情報を取得
Object3DControl *GetObject3DControl(void);
// オブジェクト3Dの種類毎の情報を取得
Object3DType *GetObject3DType(void);
//========== *** 設定 ***
// オブジェクト3Dの設定処理
void SetObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
//========== *** 状態 ***
// オブジェクト3Dのクリア処理
void ClearObject3D(void);

#endif