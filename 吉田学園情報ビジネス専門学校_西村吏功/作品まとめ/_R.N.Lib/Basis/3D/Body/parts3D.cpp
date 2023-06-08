//========================================
// 
// 部品(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** parts3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadParts3DSet関数 - 部品設定情報(3D)の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet) {

	// 部品設定情報を初期化
	if (pPartsSet->pPartsType != NULL) {
		delete[] pPartsSet->pPartsType;
		pPartsSet->pPartsType = NULL;
	}
	*pPartsSet = {};

	// 部品数読み込み
	fscanf_int(pFile, &pPartsSet->nPartsNum);

	// 部品の種類毎の情報のメモリ確保
	GetMemory((void**)&pPartsSet->pPartsType, sizeof(Parts3DType),pPartsSet->nPartsNum);

	char aDataSearch[TXT_MAX] = "";	// 検索用文字列
	int nCntParts = 0;			// 部品のカウント

	// 部品設定情報の読み込みを開始
	while (DataSearch(pFile, aDataSearch, "PARTSSET_END")) {
		if (false) {}
		else if (!strcmp(aDataSearch, /* 拡大倍率 */"SCALE")) { fscanf_float(pFile, &pPartsSet->fScale); }
		else if (!strcmp(aDataSearch, /* 部品情報 */"PARTS")) {
			// 部品の種類毎の情報のポインタ
			pPartsSet->pPartsType[nCntParts] = {};
			Parts3DType *pPartsType = &pPartsSet->pPartsType[nCntParts];

			// 部品情報の読み込みを開始
			while (DataSearch(pFile, aDataSearch, "PARTS_END")) {
				if (false) {}
				else if (!strcmp(aDataSearch, /* 部品番号   */"INDEX"))      { fscanf_int(pFile, &pPartsType->nIdx); }
				else if (!strcmp(aDataSearch, /* モデルパス */"MODEL_PATH")) {
					fscanf_string(pFile, aDataSearch);	// パス読み込み
														// モデル読み込み
					pPartsType->nModelIdx = LoadModel(aDataSearch);
				}
				else if (!strcmp(aDataSearch, /* 親部品番号 */"PARENT"))       { fscanf_int(pFile, &pPartsType->nParent); }
				else if (!strcmp(aDataSearch, /* 相対位置   */"RELATIVE_POS")) { fscanf_D3DXVECTOR3(pFile, &pPartsType->fixedRelativePos); }
				else if (!strcmp(aDataSearch, /* 相対角度   */"RELATIVE_ROT")) { fscanf_D3DXVECTOR3(pFile, &pPartsType->fixedRelativeRot); }
				else if (!strcmp(aDataSearch, /* 親部品番号 */"LIGHTING"))     { fscanf_bool(pFile, &pPartsType->bLighting); }
			}

			nCntParts++;	// 部品のカウントを加算
			assert(nCntParts <= pPartsSet->nPartsNum);
		}
	}
}

//========================================
// SaveParts3DSet関数 - 部品設定情報(3D)の書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void SaveParts3DSet(FILE *pFile, Parts3DSet *pPartsSet) {
	fprintf(pFile, "PARTSSET %d\n", pPartsSet->nPartsNum);
	fprintf(pFile, "	SCALE %f\n", pPartsSet->fScale);
	
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++) {
		Parts3DType *pPartsType = &pPartsSet->pPartsType[nCntParts];

		fprintf(pFile, "	PARTS\n");
		fprintf(pFile, "		INDEX %d\n", pPartsType->nIdx);
		fprintf(pFile, "		MODEL_PATH %s\n", GetModelInfo()->aLoadModelPath[pPartsType->nModelIdx]);
		fprintf(pFile, "		PARENT %d\n", pPartsType->nParent);
		fprintf(pFile, "		RELATIVE_POS %f %f %f\n",
				pPartsType->fixedRelativePos.x,
				pPartsType->fixedRelativePos.y,
				pPartsType->fixedRelativePos.z);
		fprintf(pFile, "		RELATIVE_ROT %f %f %f\n",
				pPartsType->fixedRelativeRot.x,
				pPartsType->fixedRelativeRot.y,
				pPartsType->fixedRelativeRot.z);
		fprintf(pFile, "		LIGHTING %d\n", (int)pPartsType->bLighting);
		fprintf(pFile, "	PARTS_END\n");
	}
	fprintf(pFile, "PARTSSET_END\n");
}