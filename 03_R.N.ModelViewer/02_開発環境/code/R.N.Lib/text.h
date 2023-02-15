//========================================
// 
// テキストのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_	// このマクロ定義がされていなかった時
#define _TEXT_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "RNmain.h"

//****************************************
// マクロ定義
//****************************************
// テキスト(2D)の最大数
#define TEXT2D_MAX (128)
// テキスト(3D)の最大数
#define TEXT3D_MAX (128)

//****************************************
// 列挙型の定義
//****************************************
// 表示形式
typedef enum
{
	TEXT_DISP_CENTER,	// 中央揃え
	TEXT_DISP_LEFT,		// 左揃え
	TEXT_DISP_RIGHT,	// 右揃え
	TEXT_DISP_MAX,
}TEXT_DISP;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// テキストの初期化処理
void InitText(void);
// テキストの終了処理
void UninitText(void);
// テキストの描画処理
void DrawText_(void);
//========== *** 入出力 ***
// フォントの読み込み処理
void LoadFont(void);
//========== *** 設定 ***
// テキスト(2D)の設定処理
void SetText2D(char *pString, int nFont, TEXT_DISP disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight);
// テキスト(3D)の設定処理
void SetText3D(char *pString, int nFont, TEXT_DISP disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard);

#endif