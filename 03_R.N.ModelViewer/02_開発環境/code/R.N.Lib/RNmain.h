//========================================
// 
// R.N.Libのメイン処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** RNmain.h ***
//========================================
#ifndef _RNMAIN_H_	// このマクロ定義がされていなかった時
#define _RNMAIN_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "../R.N.Writable/RNsetup.h"	// セットアップ
#include "camera2D.h"
#include "camera3D.h"
#include "chunk.h"
#include "color.h"
#include "fade.h"
#include "file.h"
#include "input.h"
#include "light.h"
#include "map-generator3D.h"
#include "object3D.h"
#include "physics.h"
#include "polygon2D.h"
#include "polygon3D.h"
#include "setting.h"
#include "sound.h"
#include "text.h"

//****************************************
// 構造体の定義
//****************************************
// R.N.Libのメイン処理の情報構造体
typedef struct 
{
	MODE mode;	// モード
}RNmain;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// 初期化処理
void RNInit(HINSTANCE hInstance);
// 終了処理
void RNUninit(void);
// 更新処理
void RNUpdate(void);
// 描画処理
void RNDraw(void);
//========== *** 入出力 ***
// 読み込み処理
void RNLoad(void);
// 書き込み処理
void RNSave(void);
//========== *** 取得 ***
// モードの取得
MODE GetMode(void);
//========== *** 設定 ***
// モードの設定処理
void SetMode(MODE mode);

#endif