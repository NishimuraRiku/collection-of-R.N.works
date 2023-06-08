//========================================
// 
// 色の処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** color.h ***
//========================================
#ifndef _COLOR_H_	// このマクロ定義がされていなかった時
#define _COLOR_H_	// 二重インクルード防止のマクロを定義する

#include "../../RNmain.h"

//****************************************
// マクロ定義
//****************************************
// 背景色の初期値
#define INITBGCOLOR Color{0,0,0,255}
// Colorの初期値
#define INITCOLOR Color{255,255,255,255}
// Colorのクリア値
#define CLEARCOLOR Color{0,0,0,0}

//****************************************
// 構造体定義
//****************************************
// 色管理情報構造体
struct ColorInfo {
	// 背景色
	Color bgCol = INITBGCOLOR;
};

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// 色管理情報の取得
ColorInfo *GetColorInfo(void);
//========== *** 設定 ***
// 背景色を設定
void SetBGColor(Color bgCol);
//========== *** 入出力 ***
// 色の読み込み処理
void LoadColor(FILE *pFile, Color *pCol);
//========== *** 変換 ***
// Color形式の色をD3DXCOLOR形式に変換
D3DXCOLOR ColorToD3DXCOLOR(Color col);
// Color形式の色をD3DCOLOR形式に変換
D3DCOLOR ColorToD3DCOLOR(Color col);
//========== *** その他 ***
// 色に明るさを反映
Color BrightnessToColor(Color col, float fBrightness);
// Color形式の色の値を割合で返す
Color ColorRate(Color col, float fRate);
// Color形式の色を加算
Color ColorAdd(Color col1, Color col2);
// Color形式の色を混ぜる
Color ColorMix(Color col1, Color col2);

#endif