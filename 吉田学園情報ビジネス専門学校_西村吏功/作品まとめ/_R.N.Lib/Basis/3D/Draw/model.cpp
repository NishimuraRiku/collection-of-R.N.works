//========================================
// 
// モデル関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** model.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//****************************************
// グローバル変数宣言
//****************************************
// モデル関連のテクスチャ番号
static int g_aTextureIdxModel[MODELTYPE_MAX][MATERIAL3D_MAX] = {};
// モデル関連のメッシュ(頂点情報)へのポインタ
static LPD3DXMESH g_aMeshModel[MODELTYPE_MAX] = {};
// モデル関連のマテリアルへのポインタ
static LPD3DXBUFFER g_pBuffMatModel[MODELTYPE_MAX] = {};
// モデル関連のマテリアルの数
static DWORD g_aNumMatModel[MODELTYPE_MAX] = {};
// モデルの管理情報
static ModelInfo g_modelSetUpInfo;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitModel関数 - モデルの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitModel(void) {

}

//========================================
// UninitModel関数 - モデルの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitModel(void) {
	for (int nCntType = 0; nCntType < MODELTYPE_MAX; nCntType++) {
		// メッシュの破棄
		if (g_aMeshModel[nCntType] != NULL) {
			g_aMeshModel[nCntType]->Release();
			g_aMeshModel[nCntType] = NULL;
		}

		// マテリアルの破棄
		if (g_pBuffMatModel[nCntType] != NULL) {
			g_pBuffMatModel[nCntType]->Release();
			g_pBuffMatModel[nCntType] = NULL;
		}
	}
}

//========================================
// DrawModel関数 - モデルの描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawModel(void) {
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DMATERIAL9 matDef;	// 現在のマテリアルの保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// モデルの設定数分描画
	for (int nCntModel = 0; nCntModel < g_modelSetUpInfo.nModelSetNum; nCntModel++) {
		// 種類番号を設定し、範囲外なら折り返す
		int nType = g_modelSetUpInfo.aModelSet[nCntModel].nModelIdx;
		if (nType < 0 || nType >= MODELTYPE_MAX) {
			assert(false);
			continue;
		}

		// 本体のワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_modelSetUpInfo.aModelSet[nCntModel].mtx);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatModel[nType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatModel[nType]; nCntMat++) {
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, g_modelSetUpInfo.aModelSet[nCntModel].col, g_modelSetUpInfo.aModelSet[nCntModel].bLighting);

			// テクスチャの設定
			SetTexture(pDevice, g_aTextureIdxModel[nType][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshModel[nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// LoadModel関数 - モデルの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
int LoadModel(char *pPath) {
	// 既に読み込まれているモデルか確認
	for (int nCntType = 0; nCntType < g_modelSetUpInfo.nModelTypeNum; nCntType++) {
		if (!strcmp(pPath, g_modelSetUpInfo.aLoadModelPath[nCntType])) 
		{// 既に同じパスのモデルを読み込んでいた時、
			return nCntType;	// 種類番号を返す
		}
	}

	if (g_modelSetUpInfo.nModelTypeNum >= MODELTYPE_MAX) 
	{// 既にモデルの種類数が最大に達していた時、
		assert(false);
		return -1;
	}

	// モデルのパスを保存
	sprintf(g_modelSetUpInfo.aLoadModelPath[g_modelSetUpInfo.nModelTypeNum], "%s", pPath);

	// 種類番号
	int nType = g_modelSetUpInfo.nModelTypeNum;
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[nType],
		NULL,
		&g_aNumMatModel[nType],
		&g_aMeshModel[nType]);

	if (g_pBuffMatModel[nType] == NULL) 
	{// NULLだった時、
		char aMessage[TXT_MAX] = "";
		sprintf(aMessage, "モデルの読み込みに失敗しました。\n%s", pPath);
		MessageBox(*GetWindowHandle(), aMessage, "ERROR!", MB_ICONWARNING);
		return -1;
	}
	else {
		// マテリアル情報に対するポインタを取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatModel[nType]->GetBufferPointer();

		// マテリアル数を置き換えし、もし最大数を越えていたら制御
		int nNumMat = (int)g_aNumMatModel[nType];
		if (nNumMat >= MATERIAL3D_MAX) {
			assert(false);
			nNumMat = MATERIAL3D_MAX - 1;
			g_aNumMatModel[nType] = (DWORD)nNumMat;
		}

		// マテリアル数分読み込み
		for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++) {
			assert(nCntMat < MATERIAL3D_MAX);

			if (pMat[nCntMat].pTextureFilename != NULL)
			{// テクスチャのファイル名がNULLでない時、
				// テクスチャを読み込み
				g_aTextureIdxModel[nType][nCntMat] = LoadTexture(pMat[nCntMat].pTextureFilename);
			}
			else
			{// テクスチャのファイル名がNULLの時、
				// テクスチャ番号を-1で指定
				g_aTextureIdxModel[nType][nCntMat] = -1;
			}
		}

		// モデルの数を加算
		g_modelSetUpInfo.nModelTypeNum++;
	}

	// 種類番号を返す
	return nType;
}

//========================================
// SetModel関数 - モデルの設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetModel(int nModelIdx, D3DXMATRIX mtx, Color col, bool bLighting) {
	if (nModelIdx < 0 || nModelIdx >= g_modelSetUpInfo.nModelTypeNum) 
	{// モデルの種類番号が範囲外の時、
		return;	// 処理を終了
	}
	else if (g_modelSetUpInfo.nModelSetNum >= MODEL_MAX)
	{// モデルの設定数が最大数に達している時、
		assert(false);
		return;	// 処理を終了
	}

	// モデルの設定情報を代入
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].nModelIdx = nModelIdx;
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].mtx = mtx;
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].col = col;
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].bLighting = bLighting;

	// モデルの設定数を加算
	g_modelSetUpInfo.nModelSetNum++;
}

//========================================
// GetModelInfo関数 - モデルの管理情報取得 -
// Author:RIKU NISHIMURA
//========================================
ModelInfo *GetModelInfo(void) {
	return &g_modelSetUpInfo;
}