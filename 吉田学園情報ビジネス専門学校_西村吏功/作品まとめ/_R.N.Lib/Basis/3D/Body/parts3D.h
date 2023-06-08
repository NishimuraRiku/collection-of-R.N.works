//========================================
// 
// 部品(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** parts3D.h ***
//========================================
#ifndef _PARTS3D_H_	// このマクロ定義がされていなかった時
#define _PARTS3D_H_	// 二重インクルード防止のマクロを定義する

#include "../../../RNmain.h"

//****************************************
// 構造体定義
//****************************************
// 部品(3D)の種類毎の情報構造体
typedef struct {
	// 読み込み情報
	int nIdx                     = 0;				// 自分の番号
	int nModelIdx                = DATANONE;		// モデルの番号
	int nParent                  = DATANONE;		// 親番号
	D3DXVECTOR3 fixedRelativePos = INITD3DXVECTOR3;	// 固定の相対位置
	D3DXVECTOR3 fixedRelativeRot = INITD3DXVECTOR3;	// 固定の相対角度
	D3DXVECTOR3 rotRelativePos   = INITD3DXVECTOR3;	// 回転軸の相対位置
	bool bLighting               = true;			// ライティングフラグ
}Parts3DType;

// 部品設定情報構造体
typedef struct {
	float fScale            = 1.0f;	// 拡大倍率
	int nPartsNum           = 0;	// 部品数
	Parts3DType *pPartsType = NULL;	// 部品の種類毎の情報
}Parts3DSet;

//****************************************
// プロトタイプ宣言
//****************************************
/* 部品設定情報(3D)の読み込み処理 */void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);
/* 部品設定情報(3D)の書き込み処理 */void SaveParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);

#endif