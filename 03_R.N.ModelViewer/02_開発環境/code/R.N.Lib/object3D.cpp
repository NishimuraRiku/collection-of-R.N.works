//========================================
// 
// オブジェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** object3D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// マクロ定義
//****************************************
// オブジェクト3Dの種類毎の情報のデータファイルの相対パス
#define OBJECT3D_TYPE_DATA_FILE_PATH "data\\OBJECT3D_DATA.txt"
// オブジェクト3Dの種類の最大数
#define OBJECT3D_TYPE_MAX (64)
// オブジェクト3Dの注視点の相対位置の移動倍率
#define OBJECT3D_RELATIVE_POS_R_MOVE_DIAMETER (0.25f)

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// オブジェクト3Dのパラメーターの初期値を返す
Object3D InitParameterObject3D(void);
//========== *** 更新 ***
// オブジェクト3Dのモーション設定処理
void MotionSetObject3D(Object3D *pObj3D);

//****************************************
// グローバル変数宣言
//****************************************
// オブジェクト3Dのテクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_aTextureObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX][MATERIAL_3D_MAX] = {};
// オブジェクト3Dのメッシュ(頂点情報)へのポインタ
LPD3DXMESH g_aMeshObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX] = {};
// オブジェクト3Dのマテリアルへのポインタ
LPD3DXBUFFER g_pBuffMatObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX] = {};
// オブジェクト3Dのマテリアルの数
DWORD g_aNumMatObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX] = {};
// オブジェクト3Dのワールドマトリックス
D3DXMATRIX g_aMtxWorldObject3D[PARTS_3D_MAX];
// オブジェクト3Dの情報
Object3D g_aObject3D[OBJECT3D_MAX];
// オブジェクト3Dの管理情報
Object3DControl g_object3DControl;
// オブジェクト3Dの種類毎の情報
Object3DType g_aObject3DType[OBJECT3D_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================
//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// InitParameterObject3D関数 - オブジェクト3Dのパラメーターの初期値を返す -
// Author:RIKU NISHIMURA
//========================================
Object3D InitParameterObject3D(void)
{
	return{
		INITD3DXVECTOR3,	// 現在の位置
		INITD3DXVECTOR3,	// 向き
		0,					// 種類
		false,				// 使用されているかフラグ
		{},					// 部品管理
		0,					// モーション
	};
}

//============================================================
//--------------------| *** 更新 *** |------------------------
//============================================================
//========================================
// MotionSetObject3D関数 - オブジェクト3Dのモーション設定処理 -
// Author:RIKU NISHIMURA
//========================================
void MotionSetObject3D(Object3D *pObj3D)
{
	// オブジェクト3Dの種類毎の情報のポインタ
	Object3DType *pType = &g_aObject3DType[pObj3D->nType];
	// 現在のモーションの情報のポインタ
	Motion3D *pMotion = &pType->motionSet.aMotion[pObj3D->nMotion];

	// モーション設定
	SetMotion3D(
		&pObj3D->partsInfo,
		pObj3D->nMotion);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================
//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// InitObject3D関数 - オブジェクト3Dの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitObject3D(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// マテリアルへのポインタ
	D3DXMATERIAL *pMat;
	// オブジェクト3Dの管理情報のポインタ
	Object3DControl *pObj3DCtl = &g_object3DControl;

	for (int nCntType = 0; nCntType < pObj3DCtl->nTypeNum; nCntType++)
	{
		// オブジェクト3Dの種類毎の情報のポインタ
		Object3DType *pType = &g_aObject3DType[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			// 部品(3D)の種類毎の情報構造体
			Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];

			// Xファイルの読み込み
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_pBuffMatObject3D[nCntType][nCntParts],
				NULL,
				&g_aNumMatObject3D[nCntType][nCntParts],
				&g_aMeshObject3D[nCntType][nCntParts]);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObject3D[nCntType][nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObject3D[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObject3D[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// クリア処理
	ClearObject3D();
}

//========================================
// UninitObject3D関数 - オブジェクト3Dの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitObject3D(void)
{
	for (int nCntType = 0; nCntType < OBJECT3D_TYPE_MAX; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// メッシュの破棄
			if (g_aMeshObject3D[nCntType][nCntParts] != NULL)
			{
				g_aMeshObject3D[nCntType][nCntParts]->Release();
				g_aMeshObject3D[nCntType][nCntParts] = NULL;
			}

			// マテリアルの破棄
			if (g_pBuffMatObject3D[nCntType][nCntParts] != NULL)
			{
				g_pBuffMatObject3D[nCntType][nCntParts]->Release();
				g_pBuffMatObject3D[nCntType][nCntParts] = NULL;
			}
		}
	}
}

//========================================
// UpdateObject3D関数 - オブジェクト3Dの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateObject3D(void)
{
	// オブジェクト3Dの管理情報のポインタ
	Object3DControl *pObj3DCtl = &g_object3DControl;
	// オブジェクト3Dの情報のポインタ
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		// オブジェクト3Dの種類毎の情報のポインタ
		Object3DType *pType = &g_aObject3DType[pObj3D->nType];
	}
}

//========================================
// DrawObject3D関数 - オブジェクト3Dの描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawObject3D(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアルの保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxSelf;				// 本体のマトリックス

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// オブジェクト3Dの情報のポインタ
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		if (!pObj3D->bUse)
		{// 使用されていない状態の時、
			continue;	// 繰り返し処理を折り返す
		}

		// 本体のワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxSelf);

		// 本体の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj3D->rot.y, pObj3D->rot.x, pObj3D->rot.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

		// 本体の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pObj3D->pos.x, pObj3D->pos.y, pObj3D->pos.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

		// 本体のワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

		// オブジェクト3Dの種類毎の情報のポインタ
		Object3DType *pType = &g_aObject3DType[pObj3D->nType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			// 部品(3D)の情報のポインタ
			Parts3D *pParts = &pObj3D->partsInfo.aParts[nCntParts];
			// 部品(3D)の部品の種類毎の情報のポインタ
			Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];
			D3DXMATRIX mtxParent;	// 親マトリックス

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObject3D[pObj3D->nType][nCntParts]->GetBufferPointer();

			// 部品のワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMtxWorldObject3D[nCntParts]);

			// 部品の向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxRot);

			// 部品の位置を反映
			D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxTrans);

			if (pPartsType->nParent != -1)
			{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
				mtxParent = g_aMtxWorldObject3D[pPartsType->nParent];
			}
			else
			{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
				mtxParent = mtxSelf;
			}

			// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(
				&g_aMtxWorldObject3D[nCntParts],
				&g_aMtxWorldObject3D[nCntParts],
				&mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObject3D[nCntParts]);

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObject3D[pObj3D->nType][nCntParts]; nCntMat++)
			{
				// マテリアルの設定
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

				// テクスチャの設定
				pDevice->SetTexture(0, g_aTextureObject3D[pObj3D->nType][nCntParts][nCntMat]);

				// ポリゴン(パーツ)の描画
				g_aMeshObject3D[pObj3D->nType][nCntParts]->DrawSubset(nCntMat);
			}
		}
	}
}

//============================================================
//--------------------| *** 描画 *** |------------------------
//============================================================
//========================================
// ExcDrawObject3D関数 - オブジェクト3Dの例外描画処理 -
// Author:RIKU NISHIMURA
//========================================
void ExcDrawObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, Color col)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアルの保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxSelf;				// 本体のマトリックス

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 本体のワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxSelf);

	// 本体の向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// 本体の位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// 本体のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	// オブジェクト3Dの種類毎の情報のポインタ
	Object3DType *pType = &g_aObject3DType[nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		// 部品(3D)の部品の種類毎の情報のポインタ
		Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// 親マトリックス

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObject3D[nType][nCntParts]->GetBufferPointer();

		// 部品のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMtxWorldObject3D[nCntParts]);

		// 部品の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pPartsType->fixedRelativeRot.y, pPartsType->fixedRelativeRot.x, pPartsType->fixedRelativeRot.z);
		D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxRot);

		// 部品の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pPartsType->fixedRelativePos.x, pPartsType->fixedRelativePos.y, pPartsType->fixedRelativePos.z);
		D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
			mtxParent = g_aMtxWorldObject3D[pPartsType->nParent];
		}
		else
		{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
			mtxParent = mtxSelf;
		}

		// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(
			&g_aMtxWorldObject3D[nCntParts],
			&g_aMtxWorldObject3D[nCntParts],
			&mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObject3D[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObject3D[nType][nCntParts]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, col);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObject3D[nType][nCntParts][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObject3D[nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================
//========================================
// LoadObject3D関数 - オブジェクト3Dの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadObject3D(void)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX];	// データ検索用
	// オブジェクト3Dの種類毎の情報
	Object3DType *pObj3DType = g_aObject3DType;
	// オブジェクト3Dの管理情報のポインタ
	Object3DControl *pObj3DCtl = &g_object3DControl;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJECT3D_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// 種類数を初期化
	pObj3DCtl->nTypeNum = 0;

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))
		{// 読み込みを終了
			fclose(pFile);
			break; 
		}

		if (aDataSearch[0] == '#')
		{// 折り返す 
			continue; 
		}

		if (!strcmp(aDataSearch, "TYPE"))
		{
			// オブジェクト3Dの種類毎の情報の読み込みループ
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "TYPE_END"))
				{ 
					pObj3DType++; pObj3DCtl->nTypeNum++; break; }		// 読み込みを終了
				else if (!strcmp(aDataSearch, "PARTSSET"))
				{// 部品設定情報
					LoadParts3DSet(pFile, &pObj3DType->partsSet);
				}
				else if (!strcmp(aDataSearch, "MOTIONSET"))
				{// モーション設定情報
					LoadMotion3DSet(pFile, &pObj3DType->motionSet);
				}
			}
		}
	}
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// GetObject3D関数 - オブジェクト3Dの情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Object3D *GetObject3D(void)
{
	return g_aObject3D;
}

//========================================
// GetObject3DControl関数 - オブジェクト3Dの管理情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Object3DControl *GetObject3DControl(void)
{
	return &g_object3DControl;
}

//========================================
// GetObject3DType関数 - オブジェクト3Dの種類毎の情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Object3DType *GetObject3DType(void)
{
	return g_aObject3DType;
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// SetObject3D関数 - オブジェクト3Dの設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// オブジェクト3Dの情報のポインタ
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		if (pObj3D->bUse) 
		{
			continue;
		}

		pObj3D->pos = pos;		// 位置の代入
		pObj3D->rot = rot;		// 向きの代入
		pObj3D->nType = nType;	// 種類の代入
		pObj3D->bUse = true;	// 使用されている状態にする

		break;
	}
}

//============================================================
//--------------------| *** 状態 *** |------------------------
//============================================================
//========================================
// ClearObject3D関数 - オブジェクト3Dのクリア処理 -
// Author:RIKU NISHIMURA
//========================================
void ClearObject3D(void) 
{
	// オブジェクト3Dの情報のポインタ
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		// オブジェクト3Dの種類毎の情報のポインタ
		Object3DType *pType = &g_aObject3DType[pObj3D->nType];

		// パラメーターの初期化処理
		*pObj3D = InitParameterObject3D();

		// 部品(3D)のトランスフォームを初期化
		InitParts3DTransform(&pObj3D->partsInfo, &pType->partsSet);
	}
}