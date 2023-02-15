//========================================
// 
// CHR:モデルのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_model.h ***
//========================================
#ifndef _CHR_MODEL_H_	// このマクロ定義がされていなかった時
#define _CHR_MODEL_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "../R.N.Lib/RNmain.h"

//****************************************
// 構造体の定義
//****************************************
// CHR:モデルの情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 現在の位置
	D3DXVECTOR3 rot;	// 向き

	// 分類関連
	int nType;	// 種類

	// 部品関連
	Parts3DInfo partsInfo;	// 部品管理
	int nMotion;			// モーション
}Chr_model;
// CHR:モデルの管理情報構造体
typedef struct
{
	int nTypeNum;		// 種類数
	bool bLoadModel;	// モデルを読み込めたかフラグ
}Chr_modelControl;
// CHR:モデルの種類毎の情報構造体
typedef struct
{
	Parts3DSet partsSet;	// 部品設定情報(3D)
	HitTestSet hitTestSet;	// 当たり判定設定情報
	Motion3DSet motionSet;	// モーション設定情報
}Chr_modelType;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// CHR:モデルの初期化処理
void InitChr_model(void);
// CHR:モデルの終了処理
void UninitChr_model(void);
// CHR:モデルの更新処理
void UpdateChr_model(void);
// CHR:モデルの描画処理
void DrawChr_model(void);
//========== *** 入出力 ***
// CHR:モデルの読み込み処理
void LoadChr_model(char *pPath);
//========== *** 取得 ***
// CHR:モデルの情報のポインタを取得
Chr_model *GetChr_model(void);
// CHR:モデルの管理情報のポインタを取得
Chr_modelControl *GetChr_modelControl(void);
// CHR:モデルの種類毎の情報を取得
Chr_modelType *GetChr_modelType(void);

#endif