//========================================
// 
// モデルのセットアップ(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** model-setup3D.h ***
//========================================
#ifndef _MODELSETUP3D_H_	// このマクロ定義がされていなかった時
#define _MODELSETUP3D_H_	// 二重インクルード防止のマクロを定義する

#include "motion3D.h"
#include "../Draw/model.h"
#include "../../Other/physics.h"

//****************************************
// 列挙型定義
//****************************************
// モデル構成
typedef enum {
	MODELPLATFORM_NONE,				// 無し
	MODELPLATFORM_BIPEDALISM_00,	// 直立2足歩行00
	MODELPLATFORM_BIPEDALISM_01,	// 直立2足歩行01
	MODELPLATFORM_MAX,
}MODELPLATFORM;

//****************************************
// 構造体定義
//****************************************
// モデルのセットアップ(3D)情報構造体
typedef struct {
	Parts3DSet partsSet;		// 部品設定情報(3D)
	HitTest3DSet hitTestSet;	// 当たり判定設定情報
	MODELPLATFORM platform = MODELPLATFORM_NONE;	// 構成
}ModelSetUp3D;

// モデルのセットアップ(3D)管理情報
typedef struct {
	char **ppLoadModelSetUpPath = NULL;	// 読み込み済のモデルセットアップパス
	int nModelSetUpNum = 0;	// モデルのセットアップ(3D)数
}ModelSetUp3DControl;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// モデルのセットアップ(3D)の終了処理
void UninitModelSetUp(void);
//========== *** 入出力 ***
// モデルのセットアップ(3D)の読み込み処理
int LoadModelSetUp(char *pModelSetUpPath,	// モデルのセットアップ(3D)のパス
				   bool bReload = false,	// 再読み込みフラグ
				   int nSetUpIdx = -1);		// セットアップ番号
// モデルのセットアップ(3D)書き込み処理
void SaveModelSetUp(char *pPath, int nSetUpIdx);
//========== *** 取得 ***
// モデルのセットアップ(3D)情報を取得
ModelSetUp3D GetModelSetUpInfo(int nSetUp);
// モデルのセットアップ(3D)情報を取得(ポインタ)
ModelSetUp3D **GetModelSetUp(void);
// モデルのセットアップ(3D)管理情報を取得
ModelSetUp3DControl *GetModelSetUpControl(void);
//========== *** その他 ***
// モデルのセットアップ(3D)情報のメモリを初期化
void InitModelSetUpMemory(int nNum);

#endif