//========================================
// 
// OBJ:看板[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_signboard_00.cpp ***
//========================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"		// CHR:敵			[00]
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "obj_signboard_00.h"	// OBJ:看板			[00]
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "ui_damage_00.h"		// UI :ダメージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:看板[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_SIGNBOARD_00_TYPE_DATA_FILE_PATH "data\\GAMEDATA\\OBJECT\\OBJ_SIGNBOARD_00_TYPE_DATA.txt"
// OBJ:看板[00] の最大数
#define OBJ_SIGNBOARD_00_MAX (256)
// OBJ:看板[00] の種類の最大数
#define OBJ_SIGNBOARD_00_TYPE_NUM (2)
// OBJ:看板[00] の無敵時間
#define OBJ_SIGNBOARD_00_INVINCIBLE_TIME (10)
// OBJ:看板[00] のダメージ時間
#define OBJ_SIGNBOARD_00_DAMAGE_TIME (10)
// OBJ:看板[00] の回転量
#define OBJ_SIGNBOARD_00_SPIN D3DXVECTOR3(0.0f,0.005f,0.0f)

//****************************************
// 列挙型の定義
//****************************************
// OBJ:看板[00] の種類毎の情報構造体
typedef struct
{
	char aModelPath[TXT_MAX];	// モデルの相対パス
	HitTestSet hitTestSet;		// 当たり判定設定情報
}Obj_signboard_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:看板[00] のパラメーターの初期化処理
void InitParameterObj_signboard_00(Obj_signboard_00 *pObj);

// OBJ:看板[00] の破壊処理
void DestroyObj_signboard_00(Obj_signboard_00 *pObj);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_signboard_00	// OBJ:看板[00] のテクスチャへのポインタ
						[OBJ_SIGNBOARD_00_TYPE_NUM]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_signboard_00		// OBJ:看板[00] のメッシュ(頂点情報)へのポインタ
						[OBJ_SIGNBOARD_00_TYPE_NUM]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_signboard_00	// OBJ:看板[00] のマテリアルへのポインタ
						= NULL;
DWORD					g_aNumMatObj_signboard_00	// OBJ:看板[00] のマテリアルの数
						[OBJ_SIGNBOARD_00_TYPE_NUM]
						= {};
D3DXMATRIX				g_mtxWorldObj_signboard_00;	// OBJ:看板[00] のワールドマトリックス
Obj_signboard_00		g_aObj_signboard_00			// OBJ:看板[00] の情報
						[OBJ_SIGNBOARD_00_MAX];
Obj_signboard_00Type	g_aObj_signboard_00Type		// OBJ:看板[00] の種類毎の情報
						[OBJ_SIGNBOARD_00_TYPE_NUM];

//========== *** OBJ:看板[00] の情報取得処理 ***
Obj_signboard_00 *GetObj_signboard_00(void) 
{
	return g_aObj_signboard_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_signboard_00関数 - OBJ:看板[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_signboard_00(Obj_signboard_00 *pObj)
{
	pObj->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pObj->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pObj->nType			= 0;								// 種類
	pObj->bUse			= false;							// 使用されているかフラグ
	pObj->hitTestInfo	= {};								// 当たり判定の管理情報
}

//========================================
// DestroyObj_signboard_00関数 - OBJ:看板[00] の破壊処理 -
//========================================
void DestroyObj_signboard_00(Obj_signboard_00 *pObj)
{
	pObj->bUse = false;				// 使用されていない状態にする
	ItemPriceDownObj_pedestal_00(	// OBJ:台座[00] の商品(反射装置)の値下げ処理
		OBJ_PEDESTAL_00_ITEM_REFLECTOR);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_signboard_00関数 - OBJ:看板[00] の読み込み処理 -
//========================================
void LoadObj_signboard_00(void)
{
	FILE					*pFile;		// ファイルポインタ
	char					aDataSearch	// データ検索用
							[TXT_MAX];
	Obj_signboard_00Type	*pObjType	// OBJ:看板[00] の種類毎の情報
							= g_aObj_signboard_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_SIGNBOARD_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// 読み込みを終了
		if (aDataSearch[0] == '#')			{ continue; }				// 折り返す

		if (!strcmp(aDataSearch, "TYPE")) {
			// OBJ:ステージ[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pObjType->aModelPath); }		// モデルの相対パス
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitObj_signboard_00関数 - OBJ:看板[00] の初期化処理 -
//========================================
void InitObj_signboard_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < OBJ_SIGNBOARD_00_TYPE_NUM; nCntType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			g_aObj_signboard_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_signboard_00,
			NULL,
			&g_aNumMatObj_signboard_00[nCntType],
			&g_aMeshObj_signboard_00[nCntType]);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_signboard_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_signboard_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_signboard_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_signboard_00	*pObj	// OBJ:看板[00] の情報のポインタ
					= g_aObj_signboard_00;

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++, pObj++)
	{
		// パラメーターの初期化処理
		InitParameterObj_signboard_00(pObj);
	}
}

//========================================
// UninitObj_signboard_00関数 - OBJ:看板[00] の終了処理 -
//========================================
void UninitObj_signboard_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_SIGNBOARD_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_signboard_00[nCntType] != NULL)
		{
			g_aMeshObj_signboard_00[nCntType]->Release();
			g_aMeshObj_signboard_00[nCntType] = NULL;
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_signboard_00 != NULL)
	{
		g_pBuffMatObj_signboard_00->Release();
		g_pBuffMatObj_signboard_00 = NULL;
	}

	// 初期化処理
	InitObj_signboard_00();
}

//========================================
// UpdateObj_signboard_00関数 - OBJ:看板[00] の更新処理 -
//========================================
void UpdateObj_signboard_00(void)
{
	
}

//========================================
// DrawObj_signboard_00関数 - OBJ:看板[00] の描画処理 -
//========================================
void DrawObj_signboard_00(void)
{
	Obj_signboard_00		*pObj				// OBJ:看板[00] の情報のポインタ
						= g_aObj_signboard_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_signboard_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldObj_signboard_00);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_signboard_00, &g_mtxWorldObj_signboard_00, &mtxRot);
		
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_signboard_00, &g_mtxWorldObj_signboard_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_signboard_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_signboard_00[pObj->nType]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_signboard_00[pObj->nType][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObj_signboard_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_signboard_00関数 - OBJ:看板[00] の設定処理 -
//========================================
void SetObj_signboard_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType)
{
	Obj_signboard_00	*pObj	// OBJ:看板[00] の情報のポインタ
						= g_aObj_signboard_00;

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_signboard_00Type	*pType	// OBJ:看板[00] の種類毎の情報のポインタ
							= &g_aObj_signboard_00Type[pObj->nType];

		pObj->pos = pos;		// 位置を代入
		pObj->rot = rot;		// 向きを代入
		pObj->nType = nType;	// 種類を代入
		pObj->bUse = true;		// 使用されている状態にする

		// 繰り返し処理を抜ける
		break;
	}
}

//========================================
// CollisionObj_signboard_00関数 - OBJ:看板[00] との当たり判定処理 -
//========================================
int CollisionObj_signboard_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool				bHitTemp	// 当たりフラグを保存
						= pObjCollision->bHit;
	Obj_signboard_00	*pObj		// OBJ:看板[00] の情報のポインタ
						= g_aObj_signboard_00;
	int					nIndex		// CHR:敵[00] の返しの番号
						= -1;

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 処理を折り返す
			continue;
		}

		// 衝突判定に必要な情報
		CollisionCheck(
			vector,
			pObjCollision,
			myCollInfo,
			{
				&pObj->pos,
				pObj->pos,
				NULL,
				&pObj->rot,
				pObj->rot,
				g_aObj_signboard_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		D3DXVECTOR3 pos = pObj->pos;
		pos.x += sinf(pObj->rot.y) * CHR_PLAYER_00_SIGNBOARD_DECISION_DISTANCE;
		pos.z += cosf(pObj->rot.y) * CHR_PLAYER_00_SIGNBOARD_DECISION_DISTANCE;

		if (FindDistanceLookDown(pos, GetChr_player_00()->pos) <= CHR_PLAYER_00_SIGNBOARD_DISTANCE)
		{// プレイヤーとの距離が判定距離内の時、
			if (fabsf(AngleDifference(myCollInfo.pRot->y, pObj->rot.y)) >= D3DX_PI * 0.5f)
			{// 角度差が90度以上(正面を向いている)
				nIndex = nCntObj;
			}
		}
	}

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// 番号を返す
	return nIndex;
}