//========================================
// *** directInput.h ***
//========================================
#ifndef _DIRECT_INPUT_H_	// このマクロ定義がされていなかった時
#define _DIRECT_INPUT_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
bool InitDirectInput	(HINSTANCE hInstance, HWND hWnd);	// DirectInputの初期化処理
void UninitDirectInput	(void);								// DirectInputの終了処理

#endif