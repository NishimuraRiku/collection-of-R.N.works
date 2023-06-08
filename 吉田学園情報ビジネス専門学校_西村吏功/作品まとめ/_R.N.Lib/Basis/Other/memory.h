//========================================
// 
// メモリ関連処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** memory.h ***
//========================================
#ifndef _MEMORY_H_	// このマクロ定義がされていなかった時
#define _MEMORY_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
// メモリ確保処理
void GetMemory(void **pPointer, size_t singleSize, int nNum);
// メモリ再確保処理
void ReGetMemory(void **pPointer, size_t singleSize, int nNumOld, int nNumNew);
// メモリ解放処理
void ReleaseMemory(void **pPointer);

#endif