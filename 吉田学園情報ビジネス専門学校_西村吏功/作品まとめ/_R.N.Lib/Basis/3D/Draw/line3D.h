//========================================
// 
// 線(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** line3D.h ***
//========================================
#ifndef _LINE3D_H_	// このマクロ定義がされていなかった時
#define _LINE3D_H_	// 二重インクルード防止のマクロを定義する

#include "../../Other/color.h"

//****************************************
// マクロ定義
//****************************************
// 線(3D)の最大数
#define LINE3D_MAX (1024)

//****************************************
// 構造体定義
//****************************************
// 線(3D)の設定情報構造体
typedef struct 
{
	D3DXVECTOR3 pos1 = INITD3DXVECTOR3;	// 点位置1
	D3DXVECTOR3 pos2 = INITD3DXVECTOR3;	// 点位置2
	Color col1 = INITCOLOR;	// 点色1
	Color col2 = INITCOLOR;	// 点色2
	bool bZTest = false;	// Zテストフラグ
#if /* 設定フォーマット */0
	// 線(3D)の設定情報
	Line3DSet lineSet;
	/* 点1     */lineSet.pos1 = ;
	/* 点2     */lineSet.pos2 = ;
	/* 色1     */lineSet.col1 = ;
	/* 色2     */lineSet.col2 = ;
	/* Zテスト */lineSet.bZTest = ;
	// 線(3D)の設定処理
	SetLine3D(lineSet);
#endif
}Line3DSet;

// 線(3D)関連の管理情報構造体
typedef struct {
	int nLineNum = 0;	// 線の数
	Line3DSet aLineSet[LINE3D_MAX];	// 線(3D)の設定情報を保存
}Line3DControl;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// 線(3D)関連の初期化処理
void InitLine3D(void);
// 線(3D)関連の終了処理
void UninitLine3D(void);
// 線(3D)関連の描画処理
void DrawLine3D(void);
//========== *** 取得 ***
// 線(3D)の管理情報を取得
Line3DControl *GetLine3DControl(void);
//========== *** 設定 ***
// 線(3D)の設定処理
void SetLine3D(Line3DSet lineSet);

#endif