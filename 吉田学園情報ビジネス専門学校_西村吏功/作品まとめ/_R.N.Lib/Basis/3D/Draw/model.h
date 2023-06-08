//========================================
// 
// モデル関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** model.h ***
//========================================
#ifndef _MODEL_H_	// このマクロ定義がされていなかった時
#define _MODEL_H_	// 二重インクルード防止のマクロを定義する

#include "../../../RNmain.h"
#include "../../Other/color.h"
#include "../../Calculation/matrix.h"

//****************************************
// マクロ定義
//****************************************
#define MODELTYPE_MAX  (256)	// モデルの種類の最大数
#define MODEL_MAX      (256)	// モデルの最大数
#define MATERIAL3D_MAX (16)		// マテリアルの最大数

//****************************************
// 構造体定義
//****************************************
// モデルの設定情報構造体
typedef struct {
	int nModelIdx  = -1;				// 種類番号
	D3DXMATRIX mtx = INITD3DXMATRIX;	// マトリックス
	Color col      = INITCOLOR;			// 色
	bool bLighting = false;				// ライティングフラグ
}ModelSet;

// モデルの管理情報
typedef struct {
	//========== *** 読み込み関連 ***
	// 読み込み済のモデルパス
	char aLoadModelPath[MODELTYPE_MAX][TXT_MAX] = {};
	int nModelTypeNum = 0;	// モデルの種類数

	//========== *** 設定関連 ***
	int nModelSetNum = 0;		// モデルの設定数
								// モデルの設定情報
	ModelSet aModelSet[MODEL_MAX] = {};
}ModelInfo;

//****************************************
// プロトタイプ宣言
//****************************************
/* モデルの初期化処理   */void InitModel(void);
/* モデルの終了処理     */void UninitModel(void);
/* モデルの描画処理     */void DrawModel(void);
/* モデルの読み込み処理 */int LoadModel(char *pPath);
/* モデルの設定処理     */void SetModel(int nModelIdx = -1, D3DXMATRIX mtx = INITD3DXMATRIX, Color col = INITCOLOR, bool bLighting = false);
/* モデルの管理情報取得 */ModelInfo *GetModelInfo(void);

#endif