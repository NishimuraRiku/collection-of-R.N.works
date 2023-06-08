//========================================
// 
// ヒューマン(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** human3D-base.h ***
//========================================
#ifndef _HUMAN3D_H_	// このマクロ定義がされていなかった時
#define _HUMAN3D_H_	// 二重インクルード防止のマクロを定義する

#include "../../../RNmain.h"

//****************************************
// 構造体定義
//****************************************
// ヒューマン(3D)ベース構造体
typedef struct {
	int nSetUpNum = 0;	// モデルのセットアップ番号
	//========== *** 目 ***
	D3DXVECTOR3 eyePos = INITD3DXVECTOR3;	// 目の位置
	D3DXVECTOR3 eyeRot = INITD3DXVECTOR3;	// 目の向き
	int nEyeParent = -1;	// 目の親部品番号
	float fEyeSpace = 0.0f;	// 目の間隔
	float fEyeRotDia = 0.0f;// 目の回転倍率
}Human3DBase;

// ヒューマン(3D)ベース管理情報
typedef struct {
	char **ppLoadHumanBasePath = NULL;// 読み込み済のヒューマン(3D)ベースパス
	int nHumanBaseNum = 0;	// ヒューマン(3D)ベース数
}Human3DBaseControl;

//****************************************
// 関数宣言
//****************************************
//========== *** 基本接続 ***
// ヒューマン(3D)ベースの終了処理
void UninitHuman3DBase(void);
//========== *** 入出力 ***
// ヒューマン(3D)ベース情報の読み込み処理
int LoadHuman3DBase(char *pHumanBasePath,		// ヒューマン(3D)ベースのパス
					bool bReload      = false,	// 再読み込みフラグ
					int nHumanBaseIdx = -1,		// ヒューマン(3D)ベース番号
					bool bSetUpLoad   = true);	// セットアップ読み込みフラグ
// ヒューマン(3D)ベース情報の書き込み処理
void SaveHuman3DBase(char *pPath, int nHumanBaseIdx);
//========== *** 取得 ***
// ヒューマン(3D)ベース情報を取得
Human3DBase GetHuman3DBaseInfo(int nSetUp);
// ヒューマン(3D)ベース情報を取得(ポインタ)
Human3DBase **GetHuman3DBase(void);
// ヒューマン(3D)ベース管理情報を取得
Human3DBaseControl *GetHuman3DBaseControl(void);
//========== *** その他 ***
// ヒューマン(3D)ベース情報のメモリを初期化
void InitHuman3DBaseMemory(int nSetUpNum);

#endif