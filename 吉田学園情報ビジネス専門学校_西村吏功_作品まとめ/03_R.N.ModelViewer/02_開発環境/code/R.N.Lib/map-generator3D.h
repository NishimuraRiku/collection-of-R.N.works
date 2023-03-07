//========================================
// 
// マップ生成(3D)の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** map-generator3D.h ***
//========================================
#ifndef _MAPGENERATOR3D_H_	// このマクロ定義がされていなかった時
#define _MAPGENERATOR3D_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "RNmain.h"
#include "object3D.h"

//****************************************
// 構造体の定義
//****************************************
//========== *** マップ情報 ***
// オブジェクト3D設定情報構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
	int nType;			// 種類
}Obj3DSet;

// マップ設定情報構造体
typedef struct
{
	// オブジェクト3D設定情報
	Obj3DSet aObj3DSet[OBJECT3D_MAX];
	int nObj3DNum;	// オブジェクト3Dの設定数
}MapSet3D;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** ポインタ取得 ***
// マップ情報のポインタ取得
MapSet3D *GetMapSet(void);
//========== *** 取得 ***
// マップ設定情報のパラメーター初期値を返す
MapSet3D InitParameterMapSet3D(void);
//========== *** 入出力 ***
// マップ情報の読み込み処理
void LoadMapData(char *pPath);
// マップ情報の書き込み処理
void SaveMapData(char *pPath);

#endif