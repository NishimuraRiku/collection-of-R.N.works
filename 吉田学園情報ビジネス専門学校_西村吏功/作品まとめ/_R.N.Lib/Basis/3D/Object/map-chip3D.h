#if 0
//========================================
// 
// マップチップ(3D)のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** map-chip3D.h ***
//========================================
#ifndef _MAPCHIP3D_H_	// このマクロ定義がされていなかった時
#define _MAPCHIP3D_H_	// 二重インクルード防止のマクロを定義する

#include "../Body\motion3D.h"

//****************************************
// マクロ定義
//****************************************
// マップチップ(3D)の最大数
#define MAPCHIP3D_MAX (1024)
// マップチップ(3D)の種類の最大数
#define MAPCHIP3D_TYPE_MAX (64)
// マップチップ(3D)の種類の中の種類の最大数
#define MAPCHIP3D_SUBTYPE_MAX (4)

//****************************************
// 構造体定義
//****************************************
// マップチップ(3D)の情報構造体
typedef struct
{
	// 分類関連
	int nType;		// 種類
	int nSubType;	// 種類の中の種類

	// 状態関連
	bool bUse;	// 使用されているフラグ

	// 部品関連
	//Parts3DInfo partsInfo;	// 部品管理
}MapChip3D;

// マップチップ(3D)の管理情報構造体
typedef struct
{
	// マップチップ(3D)の種類毎の種類数
	int aSubTypeNum[MAPCHIP3D_TYPE_MAX];
	// マップチップ(3D)のパス
	char aMapChipPath[MAPCHIP3D_TYPE_MAX][TXT_MAX] = "";
}MapChip3DControl;

// マップチップ(3D)の種類毎の情報構造体
typedef struct
{
	int nSetUp = -1;	// モデルのセットアップ(3D)番号
}MapChip3DType;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// マップチップ(3D)の初期化処理
void InitMapChip3D(void);
// マップチップ(3D)の終了処理
void UninitMapChip3D(void);
// マップチップ(3D)の更新処理
void UpdateMapChip3D(void);
// マップチップ(3D)の描画処理
void DrawMapChip3D(void);
//========== *** 描画 ***
// マップチップ(3D)の表示描画処理
void RenderMapChip3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nSubType, Color col);
//========== *** 入出力 ***
// マップチップ(3D)リストの読み込み処理
void LoadMapChip3DList(void);
//========== *** 取得 ***
// マップチップ(3D)の情報を取得
MapChip3D *GetMapChip3D(void);
// マップチップ(3D)の管理情報を取得
MapChip3DControl *GetMapChip3DControl(void);
// マップチップ(3D)の種類毎の情報を取得
MapChip3DType *GetMapChip3DType(void);
//========== *** 設定 ***
// マップチップ(3D)の設定処理
void SetMapChip3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nSubType);
//========== *** 状態 ***
// マップチップ(3D)のクリア処理
void ClearMapChip3D(void);

#endif
#endif