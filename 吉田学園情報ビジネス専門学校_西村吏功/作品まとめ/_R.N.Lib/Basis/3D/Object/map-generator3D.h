#if 0
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

// R.N.Lib
#include "../Space/fog.h"
#include "map-chip3D.h"

//****************************************
// 構造体定義
//****************************************
//========== *** マップ情報 ***
// マップチップ(3D)設定情報構造体
typedef struct
{
	D3DXVECTOR3 pos = INITD3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = INITD3DXVECTOR3;	// 向き
	int nType = 0;	// 種類
}MapChip3DSet;

// マップ設定情報構造体
typedef struct
{
	// マップチップ(3D)設定情報
	MapChip3DSet aMapChip3DSet[MAPCHIP3D_MAX];
	int nMapChip3DNum = 0;	// マップチップ(3D)の設定数
	
	bool bUseFog = false;	// フォグの使用フラグ
	FogSet fogSet;	// フォグの設定情報
}MapSet3D;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** ポインタ取得 ***
// マップ情報のポインタ取得
MapSet3D *GetMapSet(void);
//========== *** 入出力 ***
// マップ情報の読み込み処理
void LoadMapData(char *pPath);
// マップ情報の書き込み処理
void SaveMapData(char *pPath);

#endif
#endif