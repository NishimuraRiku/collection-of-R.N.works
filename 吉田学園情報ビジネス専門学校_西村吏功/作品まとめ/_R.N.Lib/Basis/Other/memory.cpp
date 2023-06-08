//========================================
// 
// メモリ関連処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** memory.cpp ***
//========================================
#include <assert.h>
// R.N.Lib
#include "../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// GetMemory関数 - メモリ確保処理 -
// Author:RIKU NISHIMURA
//========================================
void GetMemory(void **ppPointer, size_t singleSize, int nNum) {
	ReleaseMemory(ppPointer);	// メモリ解放
	assert(singleSize * nNum > 0);
	void *pNew = malloc(singleSize * nNum);	// 確保
	assert(pNew != NULL);	// !メモリ確保失敗
	*ppPointer = pNew;	// 新しいポインタに入れ替える
}

//========================================
// ReGetMemory関数 - メモリ再確保処理 -
// Author:RIKU NISHIMURA
//========================================
void ReGetMemory(void **ppPointer, size_t singleSize, int nNumOld, int nNumNew) {
	if (nNumNew < nNumOld)
	{// 新サイズ数が過去のサイズ数を下回っている時、
		nNumOld = nNumNew;	// 過去サイズ数を新サイズ数とする
	}

	// 新しいサイズが0以下の時、NULLを入れる
	if (singleSize * nNumNew <= 0) {
		*ppPointer = NULL;
	}

	// 新しいポインタのメモリ確保
	void *pNew = NULL;
	GetMemory(&pNew, singleSize, nNumNew);	// メモリ確保

	if (nNumOld > 0) {
		memcpy(pNew, *ppPointer, singleSize * nNumOld); // データをコピー
	}
	ReleaseMemory(ppPointer);	// 古い領域を解放
	assert(pNew != NULL);	// !メモリ確保失敗
	*ppPointer = pNew;	// 新しいポインタに入れ替える
}

//========================================
// ReleaseMemory関数 - メモリ解放処理 -
// Author:RIKU NISHIMURA
//========================================
void ReleaseMemory(void **ppPointer) {
	if (*ppPointer != NULL) {
		delete[] *ppPointer;	// 解放
		*ppPointer = NULL;
	}
}