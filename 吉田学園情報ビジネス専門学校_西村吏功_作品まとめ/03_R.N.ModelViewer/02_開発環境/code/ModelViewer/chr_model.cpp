//========================================
// 
// CHR:モデルの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_model.cpp ***
//========================================
#include <stdio.h>
#include "main.h"
// Character
#include "chr_model.h"	// CHR:モデル
// R.N.Lib
#include "../R.N.Lib/RNmain.h"

//****************************************
// マクロ定義
//****************************************
// CHR:モデルの種類の最大数
#define CHR_MODEL_TYPE_MAX (64)
// CHR:モデルの初期向き
#define CHR_MODEL_INIT_ROT D3DXVECTOR3(0.0f,D3DX_PI,0.0f)
// CHR:モデルの注視点の相対位置の移動倍率
#define CHR_MODEL_RELATIVE_POS_R_MOVE_DIAMETER (0.25f)

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 更新 ***
// CHR:モデルのモーション設定処理
void MotionSetChr_model(void);
//========== *** 入出力 ***
// CHR:モデルのモデル読み込み処理
void LoadModelSetChr_model(void);
//========== *** 取得 ***
// CHR:モデルのパラメーターの初期値を返す
Chr_model InitParameterChr_model(void);
// CHR:モデルの管理情報のパラメーターの初期値を返す
Chr_modelControl InitParameterChr_modelControl(void);

//****************************************
// グローバル宣言
//****************************************
// CHR:モデルのテクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_aTextureChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX][MATERIAL_3D_MAX] = {};
// CHR:モデルのメッシュ(頂点情報)へのポインタ
LPD3DXMESH g_aMeshChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX] = {};
// CHR:モデルのマテリアルへのポインタ
LPD3DXBUFFER g_aBuffMatChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX] = {};
// CHR:モデルのマテリアルの数
DWORD g_aNumMatChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX] = {};
// CHR:モデルのワールドマトリックス
D3DXMATRIX g_aMtxWorldChr_model[PARTS_3D_MAX];
// CHR:モデルの情報
Chr_model g_chr_model;
// CHR:モデルの管理情報
Chr_modelControl g_chr_modelControl;
// CHR:モデルの種類毎の情報
Chr_modelType g_aChr_modelType[CHR_MODEL_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 更新 *** |------------------------
//============================================================
//========================================
// MotionSetChr_model関数 - CHR:モデルのモーション設定処理 -
//========================================
void MotionSetChr_model(void)
{
	// CHR:モデルの情報のポインタ
	Chr_model *pChr = &g_chr_model;
	// CHR:モデルの種類毎の情報のポインタ
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// 現在のモーションの情報のポインタ
	Motion3D *pMotion = &pType->motionSet.aMotion[pChr->nMotion];

	// モーション設定
	SetMotion3D(
		&pChr->partsInfo,
		pChr->nMotion);
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================
//========================================
// LoadModelSetChr_model関数 - CHR:モデルのモデル読み込み処理 -
//========================================
void LoadModelSetChr_model(void) 
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL*pMat;	// マテリアルへのポインタ

	// CHR:モデルの情報のポインタ
	Chr_model *pChr = &g_chr_model;
	// CHR:モデルの種類毎の情報のポインタ
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// CHR:モデルの管理情報のポインタ
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	for (int nCntType = 0; nCntType < pChrCtl->nTypeNum; nCntType++) 
	{
		// CHR:モデルの種類毎の情報のポインタ
		Chr_modelType *pType = &g_aChr_modelType[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			// 部品(3D)の種類毎の情報構のポインタ
			Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];

			// Xファイルの読み込み
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aBuffMatChr_model[nCntType][nCntParts],
				NULL,
				&g_aNumMatChr_model[nCntType][nCntParts],
				&g_aMeshChr_model[nCntType][nCntParts]);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_aBuffMatChr_model[nCntType][nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_model[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{// ファイル名がNULLでない時、
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureChr_model[nCntType][nCntParts][nCntMat]);
				}
				else 
				{// ファイル名がNULLの時、
					// テクスチャをNULL指定
					g_aTextureChr_model[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// InitParameterChr_model関数 - CHR:モデルのパラメーターの初期値を返す -
//========================================
Chr_model InitParameterChr_model(void)
{
	return{
		INITD3DXVECTOR3,			// 位置
		CHR_MODEL_INIT_ROT,			// 向き
		0,							// 種類
		InitParameterParts3DInfo(),	// 部品管理
		g_chr_model.nMotion,		// モーション
	};
}

//========================================
// InitParameterChr_modelControl関数 - CHR:モデルの管理情報のパラメーターの初期値を返す -
//========================================
Chr_modelControl InitParameterChr_modelControl(void)
{
	return{
		1,		// 種類数
		false,	// モデルを読み込めたかフラグ
	};
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
// InitChr_model関数 - CHR:モデルの初期化処理 -
//========================================
void InitChr_model(void)
{
	// CHR:モデルの情報のポインタ
	Chr_model *pChr = &g_chr_model;
	// CHR:モデルの種類毎の情報のポインタ
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// CHR:モデルの管理情報のポインタ
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	{// 管理情報のパラメータの初期化
		bool bTemp = pChrCtl->bLoadModel;
		*pChrCtl = InitParameterChr_modelControl();
		pChrCtl->bLoadModel ^= bTemp;
	}
	
	// パラメーターの初期化
	*pChr = InitParameterChr_model();

	// 部品(3D)のトランスフォームを初期化
	InitParts3DTransform(&pChr->partsInfo, &pType->partsSet);
}

//========================================
// UninitChr_model関数 - CHR:モデルの終了処理 -
//========================================
void UninitChr_model(void)
{
	for (int nCntType = 0; nCntType < CHR_MODEL_TYPE_MAX; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// メッシュの破棄
			if (g_aMeshChr_model[nCntType][nCntParts] != NULL)
			{
				g_aMeshChr_model[nCntType][nCntParts]->Release();
				g_aMeshChr_model[nCntType][nCntParts] = NULL;
			}

			// マテリアルの破棄
			if (g_aBuffMatChr_model[nCntType][nCntParts] != NULL)
			{
				g_aBuffMatChr_model[nCntType][nCntParts]->Release();
				g_aBuffMatChr_model[nCntType][nCntParts] = NULL;
			}
		}
	}
}

//========================================
// UpdateChr_model関数 - CHR:モデルの更新処理 -
//========================================
void UpdateChr_model(void)
{
	// CHR:モデルの情報のポインタ
	Chr_model *pChr = &g_chr_model;
	// CHR:モデルの種類毎の情報のポインタ
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// CHR:モデルの管理情報のポインタ
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	// 種類加算/減算
	pChr->nType += (int)GetKeyboardTrigger(DIK_2);
	pChr->nType -= (int)GetKeyboardTrigger(DIK_1);
	IntLoopControl(&pChr->nType, pChrCtl->nTypeNum, 0);	// 制御

	// モーション番号加算/減算
	pChr->nMotion += (int)GetKeyboardTrigger(DIK_4);
	pChr->nMotion -= (int)GetKeyboardTrigger(DIK_3);
	IntLoopControl(&pChr->nMotion, pType->motionSet.nMotionNum, 0);	// 制御

	// 部品(3D)のトランスフォームを取得
	GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);

	// モーションの設定処理
	MotionSetChr_model();

	// モーションの更新処理
	UpdateMotion3D(
		pChr->partsInfo.nMotion,
		g_aChr_modelType[pChr->nType].motionSet,
		&pChr->partsInfo,
		g_aChr_modelType[pChr->nType].partsSet);

	// 衝突判定に必要な情報を作成
	CollisionInfo myCollInfo =
	{
		&pChr->pos,						// 現在の位置のポインタ
		pChr->pos,						// 過去の位置
		NULL,							// 移動量のポインタ
		&pChr->rot,						// 現在の向きのポインタ
		pChr->rot,						// 過去の向き
		&pChr->partsInfo,				// 部品管理情報
		&pType->partsSet,				// 部品設定情報
		&pType->hitTestSet.aHitTest[0],	// 当たり判定の情報
		(COLLCHK_MODE)0,				// 衝突チェックのモード
	};

	// 当たり判定登録処理
	HitTestSignUp(myCollInfo);
}

//========================================
// DrawChr_model関数 - CHR:モデルの描画処理 -
//========================================
void DrawChr_model(void)
{
	if (!g_chr_modelControl.bLoadModel)
	{
		return;	// モデルが読み込めていない時、処理を終了する
	}

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();  
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアルの保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxSelf;				// 本体のマトリックス

	// CHR:モデルの情報のポインタ
	Chr_model *pChr = &g_chr_model;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 本体のワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxSelf);

	// 本体の向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pChr->rot.y, pChr->rot.x, pChr->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// 本体の位置を反映
	D3DXMatrixTranslation(&mtxTrans, pChr->pos.x, pChr->pos.y, pChr->pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// 本体のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	// CHR:モデルの種類毎の情報のポインタ
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		// 部品(3D)の情報のポインタ
		Parts3D *pParts = &pChr->partsInfo.aParts[nCntParts];		
		// 部品(3D)の部品の種類毎の情報のポインタ
		Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX mtxParent;	// 親マトリックス

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aBuffMatChr_model[pChr->nType][nCntParts]->GetBufferPointer();

		// 部品のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMtxWorldChr_model[nCntParts]);

		if (pPartsType->nParent == -1)
		{// 親番号が-1の時、
			// モデルのサイズを変更
			D3DXMatrixScaling(&g_aMtxWorldChr_model[nCntParts], pType->partsSet.fScale, pType->partsSet.fScale, pType->partsSet.fScale);
		}

		// 部品の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_model[nCntParts], &g_aMtxWorldChr_model[nCntParts], &mtxRot);

		// 部品の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_model[nCntParts], &g_aMtxWorldChr_model[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1) 
		{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
			mtxParent = g_aMtxWorldChr_model[pPartsType->nParent];
		}
		else 
		{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
			mtxParent = mtxSelf;
		}

		// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(
			&g_aMtxWorldChr_model[nCntParts],
			&g_aMtxWorldChr_model[nCntParts],
			&mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldChr_model[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_model[pChr->nType][nCntParts]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureChr_model[pChr->nType][nCntParts][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshChr_model[pChr->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================
//========================================
// LoadChr_model関数 - CHR:モデルの読み込み処理 -
//========================================
void LoadChr_model(char *pPath)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX];	// データ検索用

	// CHR:モデルの種類毎の情報
	Chr_modelType *pChrType = g_aChr_modelType;
	// CHR:モデルの管理情報のポインタ
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(pPath, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、処理を終了する
		return;
	}

	pChrCtl->nTypeNum = 0;		// 種類数を初期化
	pChrCtl->bLoadModel = true;	// 読み込めたかフラグを真にする

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
		{// CHR:モデルの種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "TYPE_END"))
				{// 読み込みを終了
					pChrType++; 
					pChrCtl->nTypeNum++; 
					break;
				}
				else if (!strcmp(aDataSearch, "PARTSSET"))
				{// 部品設定情報
					LoadParts3DSet(pFile, &pChrType->partsSet);
				}
				else if (!strcmp(aDataSearch, "HITTESTSET"))
				{// 当たり判定設定情報 
					LoadHitTestSet(pFile, &pChrType->hitTestSet); 
				}
				else if (!strcmp(aDataSearch, "MOTIONSET"))
				{// モーション設定情報
					LoadMotion3DSet(pFile, &pChrType->motionSet);
				}
			}
		}
	}

	// モデルの読み込み処理
	LoadModelSetChr_model();
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// GetChr_model関数 - CHR:モデルの情報のポインタを取得 -
//========================================
Chr_model *GetChr_model(void)
{
	return &g_chr_model;
}

//========================================
// GetChr_modelControl関数 - CHR:モデルの管理情報のポインタを取得 -
//========================================
Chr_modelControl *GetChr_modelControl(void)
{
	return &g_chr_modelControl;
}

//========================================
// GetChr_modelType関数 - CHR:モデルの種類毎の情報を取得 -
//========================================
Chr_modelType *GetChr_modelType(void)
{
	return g_aChr_modelType;
}