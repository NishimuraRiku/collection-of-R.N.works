//========================================
// 
// UI:メニューコメント[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_menu-comment_00.h ***
//========================================
#ifndef _UI_MENUCOMMENT_00_H_	// このマクロ定義がされていなかった時
#define _UI_MENUCOMMENT_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// UI:メニューコメント[00] の初期化処理
void InitUi_menuComment_00(void);
// UI:メニューコメント[00] の終了処理
void UninitUi_menuComment_00(void);
// UI:メニューコメント[00] の更新処理
void UpdateUi_menuComment_00(void);
// UI:メニューコメント[00] の描画処理
void DrawUi_menuComment_00(void);

#endif