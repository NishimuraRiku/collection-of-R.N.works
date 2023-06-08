//========================================
// 
// ヒューマン(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** human3D-base.cpp ***
//========================================
#include "../Body/human3D-base.h"
#include "../Body/model-setup3D.h"
#include "../../Other/memory.h"
#include "../../Other/file.h"
#include "../../../RNmain.h"

//****************************************
// グローバル変数宣言
//****************************************
static Human3DBase **g_ppHuman3DBase;	// ヒューマン(3D)ベース情報
static Human3DBaseControl g_human3DBaseInfo;	// ヒューマン(3D)ベース管理情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UninitHuman3DBase関数 - ヒューマン(3D)ベースの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitHuman3DBase(void) {
	// メモリ解放
	for (int nCnt = 0; nCnt < g_human3DBaseInfo.nHumanBaseNum; nCnt++) {
		ReleaseMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[nCnt]);	// 読込済セットアップパス
		ReleaseMemory((void**)&g_ppHuman3DBase[nCnt]);							// セットアップ情報
	}
	ReleaseMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath);	// 読込済セットアップパス
	ReleaseMemory((void**)&g_ppHuman3DBase);						// セットアップ情報
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// LoadHumanBase関数 - ヒューマン(3D)ベースの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
int LoadHuman3DBase(char *pHumanBasePath, bool bReload, int nHumanBaseIdx, bool bSetUpLoad) {
	bool bLoadDone = false;	// 読み込み済フラグ

	if (nHumanBaseIdx != -1)
	{// 読み込むヒューマン(3D)ベース番号が指定されている時、
		nHumanBaseIdx = nHumanBaseIdx;	// ヒューマン(3D)ベース番号を設定

		// 読み込み済フラグを真にする
		bLoadDone = true;
	}

	// 既に読み込まれているヒューマン(3D)ベースか確認
	for (int nCntType = 0; nCntType < g_human3DBaseInfo.nHumanBaseNum; nCntType++) {
		if (!strcmp(pHumanBasePath, g_human3DBaseInfo.ppLoadHumanBasePath[nCntType]))
		{// 既に同じパスのヒューマン(3D)ベースを読み込んでいた時、
			// 読み込み済フラグを真にする
			bLoadDone = true;

			if (nHumanBaseIdx != -1)
			{// 読み込むヒューマン(3D)ベース番号が指定されている時、
				break;
			}
			else if (bReload)
			{// 再読み込みフラグが真の時、
				nHumanBaseIdx = nCntType;
				break;
			}
			else
			{// 再読み込みフラグが偽の時、
				return nCntType;	// 種類番号を返す
			}
		}
	}

	if (!bLoadDone)
	{// 読み込み済みでない時、
		// ヒューマン(3D)ベース数を加算
		g_human3DBaseInfo.nHumanBaseNum++;

		// 文字列のメモリ再確保
		ReGetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath, sizeof(char*), g_human3DBaseInfo.nHumanBaseNum - 1, g_human3DBaseInfo.nHumanBaseNum);
		GetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[g_human3DBaseInfo.nHumanBaseNum - 1], sizeof(char), TXT_MAX);

		// ヒューマン(3D)ベース情報のメモリ再確保
		ReGetMemory((void**)&g_ppHuman3DBase, sizeof(Human3DBase), g_human3DBaseInfo.nHumanBaseNum - 1, g_human3DBaseInfo.nHumanBaseNum);
	}

	// ヒューマン(3D)ベースのパスを保存
	sprintf(g_human3DBaseInfo.ppLoadHumanBasePath[nHumanBaseIdx], "%s", pHumanBasePath);

	char aDataSearch[TXT_MAX] = "";	// データ検索用
	FILE *pFile = fopen(pHumanBasePath, "r");	// 種類毎の情報のデータファイルを開く

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、処理を終了する
		assert(false);
		return 0;
	}

	while (DataSearch(pFile, aDataSearch, "END")) {
		Human3DBase *pHumanBase = g_ppHuman3DBase[nHumanBaseIdx];	// ヒューマン(3D)ベース情報のポインタ

		if (false) {}
		else if (!strcmp(aDataSearch, "SETUP_PATH:")) {	// 部品設定情報
			if (!bSetUpLoad) {
				fscanf_string(pFile, aDataSearch);	// パスを読み込み

				// モデルのセットアップ(3D)を読み込み
				pHumanBase->nSetUpNum = LoadModelSetUp(aDataSearch);
			}
		}
		else if (!strcmp(aDataSearch, "EYE_POS:"))     { fscanf_D3DXVECTOR3(pFile, &pHumanBase->eyePos); }	// 目の位置
		else if (!strcmp(aDataSearch, "EYE_ROT:"))     { fscanf_D3DXVECTOR3(pFile, &pHumanBase->eyeRot); }	// 目の向き
		else if (!strcmp(aDataSearch, "EYE_PARENT:"))  { fscanf_int(pFile, &pHumanBase->nEyeParent); }		// 目の親部品番号
		else if (!strcmp(aDataSearch, "EYE_SPACE:"))   { fscanf_float(pFile, &pHumanBase->fEyeSpace); }		// 目の間隔
		else if (!strcmp(aDataSearch, "EYE_ROT_DIA:")) { fscanf_float(pFile, &pHumanBase->fEyeRotDia); }	// 目の回転倍率
	}

	// データファイルを閉じる
	fclose(pFile);

	// ヒューマン(3D)ベース番号を返す
	return nHumanBaseIdx;
}

//========================================
// SaveHuman3DBase関数 - ヒューマン(3D)ベース書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void SaveHuman3DBase(char *pPath, int nHumanBaseIdx) {
	FILE *pFile = fopen(pPath, "w"); // ファイルを開く

	if (pFile != NULL)
	{// ファイルが開けた場合、
		// 書き込むヒューマン(3D)ベース情報のポインタ
		Human3DBase *pHumanBase = g_ppHuman3DBase[nHumanBaseIdx];
		
		fprintf(pFile, "SETUP_PATH: %s\n", GetModelSetUpControl()->ppLoadModelSetUpPath[pHumanBase->nSetUpNum]);
		fprintf(pFile, "EYE_POS: %f %f %f\n", pHumanBase->eyePos.x, pHumanBase->eyePos.y, pHumanBase->eyePos.z);
		fprintf(pFile, "EYE_ROT: %f %f %f\n", pHumanBase->eyeRot.x, pHumanBase->eyeRot.y, pHumanBase->eyeRot.z);
		fprintf(pFile, "EYE_PARENT: %d\n", pHumanBase->nEyeParent);
		fprintf(pFile, "EYE_SPACE: %f\n", pHumanBase->fEyeSpace);
		fprintf(pFile, "EYE_ROT_DIA: %f\n", pHumanBase->fEyeRotDia);

		fprintf(pFile, "END\n");

		// ファイルを閉じる
		fclose(pFile);
	}
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// GetHuman3DBase関数 - ヒューマン(3D)ベース情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Human3DBase GetHuman3DBaseInfo(int nSetUp) {
	if (nSetUp == -1 || nSetUp >= g_human3DBaseInfo.nHumanBaseNum)
	{// セットアップ番号が-1 or ヒューマン(3D)ベース数の範囲を超えていた時、
		Human3DBase defSetUp = {};
		return defSetUp;
	}

	return *g_ppHuman3DBase[nSetUp];
}

//========================================
// GetHuman3DBase関数 - ヒューマン(3D)ベース情報を取得(ポインタ) -
// Author:RIKU NISHIMURA
//========================================
Human3DBase **GetHuman3DBase(void) {
	return g_ppHuman3DBase;
}

//========================================
// GetHuman3DBaseInfo関数 - ヒューマン(3D)ベース管理情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Human3DBaseControl *GetHuman3DBaseControl(void) {
	return &g_human3DBaseInfo;
}

//============================================================
//--------------------| *** その他 *** |----------------------
//============================================================

//========================================
// InitHuman3DBaseMemory関数 - ヒューマン(3D)ベース情報のメモリを初期化 -
// Author:RIKU NISHIMURA
//========================================
void InitHuman3DBaseMemory(int nSetUpNum) {
	// 現在のセットアップ数を保存
	int nSetUpNumTemp = g_human3DBaseInfo.nHumanBaseNum;

	// セットアップ数を代入
	g_human3DBaseInfo.nHumanBaseNum = nSetUpNum;

	// 文字列のメモリ解放
	for (int nCnt = 0; nCnt < nSetUpNumTemp; nCnt++) {
		ReleaseMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[nCnt]);
	}

	// 文字列のメモリ再確保
	ReGetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath, sizeof(char*), nSetUpNumTemp, g_human3DBaseInfo.nHumanBaseNum);
	for (int nCnt = 0; nCnt < g_human3DBaseInfo.nHumanBaseNum; nCnt++) {
		g_human3DBaseInfo.ppLoadHumanBasePath[nCnt] = NULL;
		GetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[nCnt], sizeof(char), TXT_MAX);
	}

	// モデルのセットアップ(3D)情報のメモリ解放
	for (int nCnt = 0; nCnt < nSetUpNumTemp; nCnt++) {
		ReleaseMemory((void**)&g_ppHuman3DBase[nCnt]);
	}

	// モデルのセットアップ(3D)情報のメモリ再確保
	ReGetMemory((void**)&g_ppHuman3DBase, sizeof(Human3DBase*), nSetUpNumTemp, g_human3DBaseInfo.nHumanBaseNum);
	for (int nCnt = 0; nCnt < nSetUpNum; nCnt++) {
		g_ppHuman3DBase[nCnt] = NULL;
		GetMemory((void**)&g_ppHuman3DBase[nCnt], sizeof(Human3DBase), 1);
		*g_ppHuman3DBase[nCnt] = {};
	}
}