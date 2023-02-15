//========================================
// 
// OBJ:ターゲット[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_target_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "chr_player_00.h"	// CHR:プレイヤー	[00]
#include "obj_target_00.h"	// OBJ:ターゲット	[00]
#include "obj_block_00.h"	// OBJ:ブロック		[00]
#include "obj_stage_00.h"	// OBJ:ステージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:ターゲット[00] のモデルの相対パス
// OBJ:ターゲット[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_TARGET_00_MODEL_PATH			"data\\MODEL\\Object\\obj_target_000\\Model\\body_x0.1.x"
#define OBJ_TARGET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\OBJ_TARGET_00_TYPE_DATA.txt"

// OBJ:ターゲット[00] の種類の最大数
#define OBJ_TARGET_00_TYPE_MAX	(1)

//****************************************
// 列挙型の定義
//****************************************
// OBJ:ターゲット[00] の衝突オブジェクト番号
typedef enum
{
	OBJ_TARGET_00_HITOBJ_BLOCK_00,	// OBJ:ブロック	[00]
	OBJ_TARGET_00_HITOBJ_STAGE_00,	// OBJ:ステージ	[00]
	OBJ_TARGET_00_HITOBJ_MAX,
}OBJ_TARGET_00_HITOBJ;

//****************************************
// 構造体の定義
//****************************************
// OBJ:ターゲット[00] の情報構造体
typedef struct
{
	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報
	Collision	cmnCollision; 	// 共通の衝突情報
	Collision	aObjCollision	// オブジェクト毎の衝突情報
				[OBJ_TARGET_00_HITOBJ_MAX];
}Obj_target_00;

// OBJ:ターゲット[00] の種類毎の情報構造体
typedef struct
{
	HitTestSet	hitTestSet;	// 当たり判定設定情報
}Obj_target_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ターゲット[00] のパラメーターの初期化処理
void InitParameterObj_target_00(void);

// OBJ:ターゲット[00] の衝突チェック処理
void CheckCollisionObj_target_00(VECTOR vector);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9	g_pTextureObj_target_00		// OBJ:ターゲット[00] のテクスチャへのポインタ
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_pMeshObj_target_00		// OBJ:ターゲット[00] のメッシュ(頂点情報)へのポインタ
					= NULL;
LPD3DXBUFFER		g_pBuffMatObj_target_00		// OBJ:ターゲット[00] のマテリアルへのポインタ
					= NULL;
DWORD				g_dwNumMatObj_target_00		// OBJ:ターゲット[00] のマテリアルの数
					= 0;
D3DXMATRIX			g_mtxWorldObj_target_00;	// OBJ:ターゲット[00] のワールドマトリックス
Obj_target_00		g_obj_target_00;			// OBJ:ターゲット[00] の情報
Obj_target_00Type	g_aObj_target_00Type		// OBJ:ターゲット[00] の種類毎の情報
					[OBJ_TARGET_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_target_00関数 - OBJ:ターゲット[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_target_00(void) 
{
	Obj_target_00	*pObj	// OBJ:ターゲット[00] の情報のポインタ
					= &g_obj_target_00;

	pObj->hitTestInfo					= {};	// 当たり判定の管理情報
	pObj->cmnCollision					= {};	// 共通の衝突情報
	for (int nCntHitObj = 0; nCntHitObj < OBJ_TARGET_00_HITOBJ_MAX; nCntHitObj++) 
	{
		pObj->aObjCollision[nCntHitObj]	= {};	// オブジェクト毎の衝突情報
	}
}

//========================================
// CheckCollisionObj_target_00関数 - OBJ:ターゲット[00] の衝突チェック処理 -
//========================================
void CheckCollisionObj_target_00(VECTOR vector)
{
	Obj_target_00	*pObj	// OBJ:ターゲット[00] の情報のポインタ
					= &g_obj_target_00;

	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&GetChr_player_00()->setPos,
		GetChr_player_00()->pos,
		NULL,
		&GetChr_player_00()->rot,
		GetChr_player_00()->rot,
		g_aObj_target_00Type[0].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
	};

	// 衝突判定
	/*/ OBJ:ブロック	[00] /*/CollisionObj_block_00(vector, &pObj->aObjCollision[OBJ_TARGET_00_HITOBJ_BLOCK_00], &pObj->cmnCollision, myCollInfo);
	/*/ OBJ:ステージ	[00] /*/CollisionObj_stage_00(vector, &pObj->aObjCollision[OBJ_TARGET_00_HITOBJ_BLOCK_00], &pObj->cmnCollision, myCollInfo);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_target_00関数 - OBJ:ターゲット[00] の読み込み処理 -
//========================================
void LoadObj_target_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Obj_target_00Type	*pObjType	// OBJ:ターゲット[00] の種類毎の情報
						= g_aObj_target_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_TARGET_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:ターゲット[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitObj_target_00関数 - OBJ:ターゲット[00] の初期化処理 -
//========================================
void InitObj_target_00(void)
{
	// パラメーターの初期化処理
	InitParameterObj_target_00();

	Obj_target_00		*pObj	// OBJ:ターゲット[00] の情報のポインタ
						= &g_obj_target_00;
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		OBJ_TARGET_00_MODEL_PATH,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObj_target_00,
		NULL,
		&g_dwNumMatObj_target_00,
		&g_pMeshObj_target_00
	);

	D3DXMATERIAL *pMat;	// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_target_00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObj_target_00; nCntMat++) 
	{
		if (pMat[nCntMat].pTextureFilename != NULL) 
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureObj_target_00[nCntMat]);
		}
	}
}

//========================================
// UninitObj_target_00関数 - OBJ:ターゲット[00] の終了処理 -
//========================================
void UninitObj_target_00(void)
{
	// メッシュの破棄
	if (g_pMeshObj_target_00 != NULL)
	{
		g_pMeshObj_target_00->Release();
		g_pMeshObj_target_00 = NULL;
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_target_00 != NULL)
	{
		g_pBuffMatObj_target_00->Release();
		g_pBuffMatObj_target_00 = NULL;
	}
}

//========================================
// UpdateObj_target_00関数 - OBJ:ターゲット[00] の更新処理 -
//========================================
void UpdateObj_target_00(void)
{
	Obj_target_00	*pObj	// OBJ:ターゲット[00] の情報のポインタ
					= &g_obj_target_00;
}

//========================================
// DrawObj_target_00関数 - OBJ:ターゲット[00] の描画処理 -
//========================================
void DrawObj_target_00(void)
{
	if ((!GetChr_player_00Item()[GetChr_player_00()->nSelectItem].bSet)
		||
		(GetChr_player_00()->setPosCmnCollision.bInside)) 
	{// 選択されているアイテムの設置フラグが偽 or 設定位置の重なりフラグが真の時、
		return;	// 処理を終了する
	}

	Obj_target_00		*pObj				// OBJ:ターゲット[00] の情報のポインタ
						= &g_obj_target_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldObj_target_00);
	
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_mtxWorldObj_target_00, &g_mtxWorldObj_target_00, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetChr_player_00()->setPos.x, GetChr_player_00()->setPos.y, GetChr_player_00()->setPos.z);
	D3DXMatrixMultiply(&g_mtxWorldObj_target_00, &g_mtxWorldObj_target_00, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_target_00);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_target_00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObj_target_00; nCntMat++) 
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureObj_target_00[nCntMat]);

		// ポリゴン(パーツ)の描画
		g_pMeshObj_target_00->DrawSubset(nCntMat);
	}
}