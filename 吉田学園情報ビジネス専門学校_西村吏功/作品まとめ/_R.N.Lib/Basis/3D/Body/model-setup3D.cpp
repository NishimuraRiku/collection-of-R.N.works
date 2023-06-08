//========================================
// 
// モデルのセットアップ(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** model-setup3D.cpp ***
//========================================
#include "model-setup3D.h"
#include "../../Other/memory.h"
#include "../../Other/file.h"
#include "../../Other/window.h"
#include "../../Other/print.h"

//****************************************
// グローバル変数宣言
//****************************************
static ModelSetUp3DControl g_modelSetUpInfo;	// モデルの管理情報
static ModelSetUp3D **g_ppModelSetUp;	// モデルのセットアップ(3D)情報

//****************************************
// グローバル定数宣言
//****************************************
// モデル構成名
static const char g_aModelPlatformName[MODELPLATFORM_MAX][TXT_MAX] = {
	"NONE",				// 無し
	"BIPEDALISM_00",	// 直立2足歩行00
	"BIPEDALISM_01",	// 直立2足歩行01
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UninitModelSetUp関数 - モデルのセットアップ(3D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitModelSetUp(void) {
	// メモリ解放
	for (int nCntSetUp = 0; nCntSetUp < g_modelSetUpInfo.nModelSetUpNum; nCntSetUp++) {
		// セットアップ情報 > 部品設定情報 > 部品の種類毎の設定情報
		ReleaseMemory((void**)&g_ppModelSetUp[nCntSetUp]->partsSet.pPartsType);

		// セットアップ情報 > 当たり判定設定情報 > 当たり判定の情報 >当たり判定の部品の情報
		for (int nCntHitTest = 0; nCntHitTest < g_ppModelSetUp[nCntSetUp]->hitTestSet.nHitTestNum; nCntHitTest++) {
			ReleaseMemory((void**)&g_ppModelSetUp[nCntSetUp]->hitTestSet.pHitTest[nCntHitTest].pHitTestParts);
		}

		// セットアップ情報 > 当たり判定設定情報 > 当たり判定の情報
		ReleaseMemory((void**)&g_ppModelSetUp[nCntSetUp]->hitTestSet.pHitTest);

		// 読込済セットアップパス
		ReleaseMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[nCntSetUp]);
	}
	// 読込済セットアップパス全体
	ReleaseMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath);

	// セットアップ情報
	ReleaseMemory((void**)&g_ppModelSetUp);
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// LoadModelSetUp関数 - モデルのセットアップ(3D)の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
int LoadModelSetUp(char *pModelSetUpPath, bool bReload, int nSetUpIdx) {
	int nIdx = 0;	// セットアップ番号

	if (nSetUpIdx >= 0 || bReload)
	{// 読み込むセットアップ番号が指定されている or 再読み込みフラグが真の時、
		// 番号が範囲外であれば-1を返し終了
		if (nIdx >= g_modelSetUpInfo.nModelSetUpNum) {
			assert(false);
			return -1;
		}

		// セットアップ番号を設定
		nIdx = nSetUpIdx;
	}
	else 
	{// 読み込むセットアップ番号が指定されていない時、
		nIdx = g_modelSetUpInfo.nModelSetUpNum;	// セットアップ番号を先頭番号にする
	}

	FILE *pFile;	// ファイルポインタ

	if (!bReload) 
	{// 再読み込みでない時、
		// 既に読み込まれているモデルセットアップか確認
		for (int nCntType = 0; nCntType < g_modelSetUpInfo.nModelSetUpNum; nCntType++) {
			if (!strcmp(pModelSetUpPath, g_modelSetUpInfo.ppLoadModelSetUpPath[nCntType]))
			{// 既に同じパスのモデルセットアップを読み込んでいた時、
				return nCntType;	// 種類番号を返す
			}
		}

		// 種類毎の情報のデータファイルを開く
		pFile = fopen(pModelSetUpPath, "r");
		if (pFile == NULL) {// ファイルが開けなかった場合、処理を終了する
			Message(CreateText("モデルのセットアップの読み込みに失敗しました。\n\"%s\"", pModelSetUpPath), "ERROR!");
			return -1;
		}

		// セットアップ数を加算
		g_modelSetUpInfo.nModelSetUpNum++;

		// 文字列のメモリ再確保
		ReGetMemory(
			/* ポインタ */(void**)&g_modelSetUpInfo.ppLoadModelSetUpPath,
			/* 型サイズ */sizeof(char*),
			/* 過去数   */g_modelSetUpInfo.nModelSetUpNum - 1,
			/* 現在数   */g_modelSetUpInfo.nModelSetUpNum);
		// 増設したポインタにNULLを入れる
		g_modelSetUpInfo.ppLoadModelSetUpPath[g_modelSetUpInfo.nModelSetUpNum - 1] = NULL;
		// 文字数分のメモリを確保する
		GetMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[g_modelSetUpInfo.nModelSetUpNum - 1], sizeof(char), TXT_MAX);

		// モデルのセットアップ(3D)情報のメモリ再確保
		ReGetMemory(
			/* ポインタ */(void**)&g_ppModelSetUp,
			/* 型サイズ */sizeof(ModelSetUp3D*),
			/* 過去数   */g_modelSetUpInfo.nModelSetUpNum - 1,
			/* 現在数   */g_modelSetUpInfo.nModelSetUpNum);
		// 増設したポインタにNULLを入れる
		g_ppModelSetUp[g_modelSetUpInfo.nModelSetUpNum - 1] = NULL;
		// モデルのセットアップ分のメモリを確保する
		GetMemory((void**)&g_ppModelSetUp[g_modelSetUpInfo.nModelSetUpNum - 1], sizeof(ModelSetUp3D), 1);
	}
	else 
	{// 再読み込みの時、
		// 種類毎の情報のデータファイルを開く
		pFile = fopen(pModelSetUpPath, "r");
		if (pFile == NULL) {// ファイルが開けなかった場合、処理を終了する
			Message(CreateText("モデルのセットアップの読み込みに失敗しました。\n\"%s\"", pModelSetUpPath), "ERROR!");
			return -1;
		}
	}

	// モデルのセットアップ(3D)のパスを保存
	strcpy(g_modelSetUpInfo.ppLoadModelSetUpPath[nSetUpIdx], pModelSetUpPath);

	// 読み込みループ
	char aDataSearch[TXT_MAX] = "";
	while (DataSearch(pFile, aDataSearch, "END")) {
		ModelSetUp3D *pSetUp = g_ppModelSetUp[nSetUpIdx];	// モデルのセットアップ(3D)情報のポインタ

		if (false) {}
		else if (!strcmp(aDataSearch, "PARTSSET"))   { LoadParts3DSet(pFile, &pSetUp->partsSet); }		// 部品設定情報
		else if (!strcmp(aDataSearch, "HITTESTSET")) { LoadHitTest3DSet(pFile, &pSetUp->hitTestSet); }	// 当たり判定設定情報 
		else if (!strcmp(aDataSearch, "PLATFORM"))
		{// モデル構成
			fscanf_string(pFile, aDataSearch);

			// モデル構成初期化
			pSetUp->platform = MODELPLATFORM_NONE;

			for (int nCntPlatform = 0; nCntPlatform < MODELPLATFORM_MAX; nCntPlatform++) {
				if (!strcmp(aDataSearch, g_aModelPlatformName[nCntPlatform]))
				{// カウントのモデル構成名と一致した時、モデル構成設定
					pSetUp->platform = (MODELPLATFORM)nCntPlatform;
					break;
				}
			}
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	// モデルのセットアップ(3D)番号を返す
	return nSetUpIdx;
}

//========================================
// SaveModelSetUp関数 - モデルのセットアップ(3D)書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void SaveModelSetUp(char *pPath, int nSetUpIdx) {
	FILE *pFile = fopen(pPath, "w"); // ファイルを開く

	if (pFile != NULL)
	{// ファイルが開けた場合、
		// 書き込むモデルのセットアップ(3D)情報のポインタ
		ModelSetUp3D *pSetUp = g_ppModelSetUp[nSetUpIdx];

		// 部品設定情報(3D)の書き込み処理
		SaveParts3DSet(pFile, &pSetUp->partsSet);

		{// 当たり判定の設定情報を書き込み
			// 当たり判定の設定情報のポインタ
			HitTest3DSet *pPartsSet = &g_ppModelSetUp[nSetUpIdx]->hitTestSet;

			fprintf(pFile, "HITTESTSET %d\n", pPartsSet->nHitTestNum);
			for (int nCntHitTest = 0; nCntHitTest < pPartsSet->nHitTestNum; nCntHitTest++) {
				// 当たり判定の情報のポインタ
				HitTest3D *pHitTest = &pPartsSet->pHitTest[nCntHitTest];

				fprintf(pFile, "	HITTEST %d\n", pHitTest->nHitTestPartsNum);
				for (int nCntParts = 0; nCntParts < pHitTest->nHitTestPartsNum; nCntParts++) {
					// 当たり判定の部品毎の情報のポインタ
					HitTest3DParts *pParts = &pHitTest->pHitTestParts[nCntParts];

					fprintf(pFile, "		HITTEST_PARTS\n");
					fprintf(pFile, "			FORM %d\n", (int)pParts->hitTestForm);
					fprintf(pFile, "			PARENT %d\n", pParts->nParent);
					fprintf(pFile, "			WIDTH %f\n", pParts->fWidth);
					fprintf(pFile, "			HEIGHT %f\n", pParts->fHeight);
					fprintf(pFile, "			DEPTH %f\n", pParts->fDepth);
					fprintf(pFile, "			RELATIVE_POS %f %f %f\n",
						pParts->fixedRelativePos.x,
						pParts->fixedRelativePos.y,
						pParts->fixedRelativePos.z);
					fprintf(pFile, "			RELATIVE_ROT %f %f %f\n",
						pParts->fixedRelativeRot.x,
						pParts->fixedRelativeRot.y,
						pParts->fixedRelativeRot.z);
					fprintf(pFile, "			GHOST");
					for (int nCntDir = 0; nCntDir < HITDIRECTION_MAX; nCntDir++) {
						fprintf(pFile, " %d", (int)pParts->aGhost[nCntDir]);
					}
					fprintf(pFile, "\n");
					fprintf(pFile, "		HITTEST_PARTS_END\n");
				}
				fprintf(pFile, "	HITTEST_END\n");
			}
			fprintf(pFile, "HITTESTSET_END\n");
		}
		fprintf(pFile, "PLATFORM %s\n", g_aModelPlatformName[pSetUp->platform]);
		fprintf(pFile, "END\n");

		// ファイルを閉じる
		fclose(pFile);
	}
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// GetModelSetUp関数 - モデルのセットアップ(3D)情報を取得 -
// Author:RIKU NISHIMURA
//========================================
ModelSetUp3D GetModelSetUpInfo(int nSetUp) {
	if (nSetUp <= -1 || nSetUp >= g_modelSetUpInfo.nModelSetUpNum)
	{// セットアップ番号が範囲外の時、デフォルトのセットアップ情報を返す
		ModelSetUp3D defSetUp = {};
		return defSetUp;
	}

	return *g_ppModelSetUp[nSetUp];
}

//========================================
// GetModelSetUp関数 - モデルのセットアップ(3D)情報を取得(ポインタ) -
// Author:RIKU NISHIMURA
//========================================
ModelSetUp3D **GetModelSetUp(void) {
	return g_ppModelSetUp;
}

//========================================
// GetModelSetUpInfo関数 - モデルのセットアップ(3D)管理情報を取得 -
// Author:RIKU NISHIMURA
//========================================
ModelSetUp3DControl *GetModelSetUpControl(void) {
	return &g_modelSetUpInfo;
}

//============================================================
//--------------------| *** その他 *** |----------------------
//============================================================

//========================================
// InitModelSetUpMemory関数 - モデルのセットアップ(3D)情報のメモリを初期化 -
// Author:RIKU NISHIMURA
//========================================
void InitModelSetUpMemory(int nNum) {
	// 現在のセットアップ数を保存
	int nNumTemp = g_modelSetUpInfo.nModelSetUpNum;

	// セットアップ数を代入
	g_modelSetUpInfo.nModelSetUpNum = nNum;

	{//========== *** 読み込み済みモデルセットアップパス ***
		// 文字列ポインタのメモリ解放
		for (int nCnt = 0; nCnt < nNumTemp; nCnt++) {
			ReleaseMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[nCnt]);
		}

		// 文字列ダブルポインタのメモリ確保
		GetMemory(
			/* ポインタ */(void**)&g_modelSetUpInfo.ppLoadModelSetUpPath,
			/* 型サイズ */sizeof(char*),
			/* 確保数   */g_modelSetUpInfo.nModelSetUpNum);

		// 文字列ポインタのメモリ確保(文字数分)
		for (int nCnt = 0; nCnt < g_modelSetUpInfo.nModelSetUpNum; nCnt++) {
			g_modelSetUpInfo.ppLoadModelSetUpPath[nCnt] = NULL;
			GetMemory(
				/* ポインタ */(void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[nCnt],
				/* 型サイズ */sizeof(char),
				/* 確保数   */TXT_MAX);
		}
	}

	{//========== *** モデルのセットアップ(3D)情報 ***
		// モデルのセットアップ(3D)情報のメモリ解放
		for (int nCnt = 0; nCnt < nNumTemp; nCnt++) {
			ReleaseMemory((void**)&g_ppModelSetUp[nCnt]);
		}

		// モデルのセットアップ(3D)情報ダブルポインタのメモリ確保
		GetMemory(
			/* ポインタ */(void**)&g_ppModelSetUp,
			/* 型サイズ */sizeof(ModelSetUp3D*),
			/* 確保数   */g_modelSetUpInfo.nModelSetUpNum);

		// モデルのセットアップ(3D)情報ポインタのメモリ確保
		for (int nCnt = 0; nCnt < nNum; nCnt++) {
			g_ppModelSetUp[nCnt] = NULL;
			GetMemory(
				/* ポインタ */(void**)&g_ppModelSetUp[nCnt],
				/* 型サイズ */sizeof(ModelSetUp3D),
				/* 確保数   */1);
			*g_ppModelSetUp[nCnt] = {};	// 初期化
		}
	}
}