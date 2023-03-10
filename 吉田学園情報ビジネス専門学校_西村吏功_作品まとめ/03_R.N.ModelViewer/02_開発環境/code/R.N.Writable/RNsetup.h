//========================================
// 
// R.N.Libのセットアップ
// Author:RIKU NISHIMURA
// 
//========================================
//  *** RNsetup.h ***
//========================================
#ifndef _RNSETUP_H_	// このマクロ定義がされていなかった時
#define _RNSETUP_H_	// 二重インクルード防止のマクロを定義する

// メインヘッダファイル
#include "../ModelViewer/main.h"

// モード
#include "../ModelViewer/md_main.h"

//****************************************
// マクロ定義
//****************************************
// 初期のモード
#define INIT_MODE (MODE_MAIN)

//****************************************
// 列挙型の定義
//****************************************
// モード(画面)の種類
typedef enum
{
	MODENONE = -1,	// 無し
	MODE_MAIN,
	MODE_MAX,
}MODE;

#endif