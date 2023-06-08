//========================================
// 
// OBJ:スイッチ[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_switch_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "light.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"		// CHR:敵					[00]
#include "eff_explosion_00.h"	// EFF:爆発					[00]
#include "md_game_00.h"			// MD :ゲーム画面			[00]
#include "md_tutorial_00.h"		// MD :チュートリアル画面	[00]
#include "obj_switch_00.h"		// OBJ:スイッチ				[00]
#include "obj_stage_00.h"		// OBJ:ステージ				[00]
#include "ui_damage_00.h"		// UI :ダメージ				[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:スイッチ[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_SWITCH_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_SWITCH_00_TYPE_DATA.txt"

// OBJ:スイッチ[00] の種類の数
#define OBJ_SWITCH_00_TYPE_NUM	(1)

// OBJ:スイッチ[00] の到着にかかる時間
// OBJ:スイッチ[00] の非到着時の相対位置Y
#define OBJ_SWITCH_00_ARRIVAL_TIME					(60)
#define OBJ_SWITCH_00_NON_ARRIVAL_RELATIVE_POS_Y	(-6)

// OBJ:スイッチ[00] のテキストの相対位置Y
// OBJ:スイッチ[00] のテキストの拡大倍率
#define OBJ_SWITCH_00_TEXT_RELATIVE_POS_Y	(5)
#define OBJ_SWITCH_00_TEXT_SCALE			(0.5f)

//****************************************
// 構造体の定義
//****************************************
// OBJ:スイッチ[00] の種類毎の情報構造体
typedef struct
{
	HitTestSet	hitTestSet;		// 当たり判定設定情報
	Parts3DSet	partsSet;		// 部品設定情報(3D)
	Motion3DSet motionSet;		// モーション設定情報
}Obj_switch_00Type;

// OBJ:スイッチ[00] の管理情報構造体
typedef struct
{
	int nCounterArrival;	// 到着カウンター
}Obj_switch_00Control;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:スイッチ[00] のパラメーターの初期化処理
// OBJ:スイッチ[00] の管理情報のパラメーターの初期化処理
void InitParameterObj_switch_00(void);
void InitParameterObj_switch_00Control(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_switch_00		// OBJ:スイッチ[00] のテクスチャへのポインタ
						[OBJ_SWITCH_00_TYPE_NUM]
						[PARTS_3D_MAX]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_switch_00		// OBJ:スイッチ[00] のメッシュ(頂点情報)へのポインタ
						[OBJ_SWITCH_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_switch_00		// OBJ:スイッチ[00] のマテリアルへのポインタ
						= NULL;
DWORD					g_aNumMatObj_switch_00		// OBJ:スイッチ[00] のマテリアルの数
						[OBJ_SWITCH_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
D3DXMATRIX				g_aMtxWorldObj_switch_00	// OBJ:スイッチ[00] のワールドマトリックス
						[PARTS_3D_MAX];
Obj_switch_00			g_obj_switch_00;			// OBJ:スイッチ[00] の情報
Obj_switch_00Control	g_obj_switch_00Control;		// OBJ:スイッチ[00] の管理情報
Obj_switch_00Type		g_aObj_switch_00Type		// OBJ:スイッチ[00] の種類毎の情報
						[OBJ_SWITCH_00_TYPE_NUM];

//========== *** OBJ:スイッチ[00] の情報を取得 ***
Obj_switch_00 *GetObj_switch_00(void) 
{
	return &g_obj_switch_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_switch_00関数 - OBJ:スイッチ[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_switch_00(void) 
{
	Obj_switch_00	*pObj	// OBJ:スイッチ[00] の情報のポインタ
					= &g_obj_switch_00;

	pObj->pos		= INITD3DXVECTOR3;				// 現在の位置
	pObj->rot		= INITD3DXVECTOR3;				// 向き
	pObj->nType		= 0;							// 種類
	pObj->partsInfo	= {};							// 部品管理
	pObj->motion	= OBJ_SWITCH_00_MOTION_WAIT;	// モーション
}

//========================================
// InitParameterObj_switch_00Control関数 - OBJ:スイッチ[00] の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterObj_switch_00Control(void)
{
	Obj_switch_00Control	*pObjCtl	// OBJ:スイッチ[00] の情報のポインタ
							= &g_obj_switch_00Control;

	pObjCtl->nCounterArrival = 0;	// 到着カウンター
}

//========================================
// MotionSetObj_switch_00関数 - OBJ:スイッチ[00] のモーション設定処理 -
//========================================
void MotionSetObj_switch_00(void)
{
	Obj_switch_00		*pObj		// OBJ:スイッチ[00] の情報のポインタ
						= &g_obj_switch_00;
	Obj_switch_00Type	*pType		// OBJ:スイッチ[00] の種類毎の情報のポインタ
						= &g_aObj_switch_00Type[pObj->nType];
	Motion3D			*pMotion	// 現在のモーションの情報のポインタ
						= &pType->motionSet.aMotion[pObj->motion];;

	// モーション設定
	SetMotion3D(
		&pObj->partsInfo,
		pObj->motion);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_switch_00関数 - OBJ:スイッチ[00] の読み込み処理 -
//========================================
void LoadObj_switch_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Obj_switch_00Type	*pObjType	// OBJ:スイッチ[00] の種類毎の情報
						= g_aObj_switch_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_SWITCH_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:スイッチ[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// 当たり判定設定情報
				else if (!strcmp(aDataSearch, "PARTSSET"))		{ LoadParts3DSet(pFile, &pObjType->partsSet); }		// 部品設定情報
				else if (!strcmp(aDataSearch, "MOTIONSET"))		{ LoadMotion3DSet(pFile, &pObjType->motionSet); }	// モーション設定情報
			}
		}
	}
}

//========================================
// InitObj_switch_00関数 - OBJ:スイッチ[00] の初期化処理 -
//========================================
void InitObj_switch_00(void)
{
	// 管理情報の初期化
	InitParameterObj_switch_00Control();

	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < OBJ_SWITCH_00_TYPE_NUM; nCntType++)
	{
		Obj_switch_00Type	*pType	// OBJ:スイッチ[00] の種類毎の情報のポインタ
							= &g_aObj_switch_00Type[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			Parts3DType *pPartsType	// 部品(3D)の種類毎の情報構造体
						= &pType->partsSet.aPartsType[nCntParts];

			// Xファイルの読み込み
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_pBuffMatObj_switch_00,
				NULL,
				&g_aNumMatObj_switch_00[nCntType][nCntParts],
				&g_aMeshObj_switch_00[nCntType][nCntParts]);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObj_switch_00->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_switch_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// パラメーターの初期化処理
	InitParameterObj_switch_00();

	Obj_switch_00		*pObj	// OBJ:スイッチ[00] の情報のポインタ
						= &g_obj_switch_00;
	Obj_switch_00Type	*pType	// OBJ:スイッチ[00] の種類毎の情報のポインタ
						= &g_aObj_switch_00Type[pObj->nType];

	// 部品(3D)のトランスフォームを初期化
	InitParts3DTransform(&pObj->partsInfo, &pType->partsSet);
}

//========================================
// UninitObj_switch_00関数 - OBJ:スイッチ[00] の終了処理 -
//========================================
void UninitObj_switch_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_SWITCH_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			if (g_aMeshObj_switch_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshObj_switch_00[nCntType][nCntParts]->Release();
				g_aMeshObj_switch_00[nCntType][nCntParts] = NULL;
			}

			// テクスチャの破棄
			for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
				if (g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat] != NULL)
				{
					g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat]->Release();
					g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_switch_00 != NULL)
	{
		g_pBuffMatObj_switch_00->Release();
		g_pBuffMatObj_switch_00 = NULL;
	}
}

//========================================
// UpdateObj_switch_00関数 - OBJ:スイッチ[00] の更新処理 -
//========================================
void UpdateObj_switch_00(void)
{
	Obj_switch_00Control	*pObjCtl	// OBJ:台座[00] の管理情報のポインタ
							= &g_obj_switch_00Control;

	if (((GetMode() == MODE_GAME_00) &&
		(GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL))
		||
		(GetMode() == MODE_TUTORIAL_00) &&
		(GetMd_tutorial_00()->state == MD_TUTORIAL_00_STATE_NORMAL))
	{// (MD:ゲーム画面[00] の状態が通常orTIPS待ち)or(チュートリアル画面の状態が通常)の時、
		if (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL)
		{// OBJ:ステージ[00] の状態がインターバルの時、
			pObjCtl->nCounterArrival++;	// 到着カウンターを加算
		}
		else if (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_RAID)
		{// OBJ:ステージ[00] の状態が襲撃の時、
			pObjCtl->nCounterArrival--;	// 到着カウンターを減算
		}
	}

	// 到着カウンターを制御
	IntControl(&pObjCtl->nCounterArrival, OBJ_PEDESTAL_00_ARRIVAL_TIME, 0);

	Obj_switch_00		*pObj	// OBJ:スイッチ[00] の情報のポインタ
						= &g_obj_switch_00;
	Obj_switch_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aObj_switch_00Type[pObj->nType];
	{
		float	fRate	// カウンターの割合
				= (float)pObjCtl->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME;

		// テキスト(3D)の設定処理
		SetText3D(
			"SKIP",
			FONT_002, DISPLAY_CENTER,
			pObj->pos + D3DXVECTOR3(
				0.0f,
				OBJ_SWITCH_00_TEXT_RELATIVE_POS_Y + (OBJ_SWITCH_00_NON_ARRIVAL_RELATIVE_POS_Y * (1.0f - fRate)),
				0.0f),
				{ 255,255,255,(int)(255 * fRate) },
			OBJ_SWITCH_00_TEXT_SCALE, OBJ_SWITCH_00_TEXT_SCALE, true, false, true);
	}

	// 部品(3D)のトランスフォームを取得
	GetParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// モーションの設定処理
	MotionSetObj_switch_00();

	// モーションの更新処理
	if (UpdateMotion3D(
		pObj->partsInfo.nMotion,
		g_aObj_switch_00Type[pObj->nType].motionSet,
		&pObj->partsInfo,
		g_aObj_switch_00Type[pObj->nType].partsSet))
	{// モーションがループに達した時、
		Motion3D	*pMotion	// 現在のモーションの情報のポインタ
					= &pType->motionSet.aMotion[pObj->motion];
		if (!pMotion->bLoop)
		{// 現在のモーションのループフラグが偽の時、
			if (pObj->motion == OBJ_SWITCH_00_MOTION_PUSH)
			{// モーションが押下の時、
				// OBJ:ゲーム画面[00] の状態を襲撃に設定
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_RAID);
			}
			pObj->motion = OBJ_SWITCH_00_MOTION_WAIT;	// モーションを待機に設定
		}
	}
}

//========================================
// DrawObj_switch_00関数 - OBJ:スイッチ[00] の描画処理 -
//========================================
void DrawObj_switch_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX			mtxSelf;			// 本体のマトリックス

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_switch_00->GetBufferPointer();

	// 本体のワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxSelf);

	Obj_switch_00	*pObj	// OBJ:スイッチ[00] の情報のポインタ
					= &g_obj_switch_00;

	// 本体の向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);
	{
		Obj_switch_00Control	*pObjCtl	// OBJ:台座[00] の管理情報のポインタ
								= &g_obj_switch_00Control;

		// 本体の位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pObj->pos.x,
			pObj->pos.y + (OBJ_SWITCH_00_NON_ARRIVAL_RELATIVE_POS_Y * (1.0f - ((float)pObjCtl->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME))),
			pObj->pos.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);
	}
	// 本体のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	Obj_switch_00Type	*pType	// OBJ:スイッチ[00] の種類毎の情報のポインタ
						= &g_aObj_switch_00Type[pObj->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// 部品(3D)の情報のポインタ
					= &pObj->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// 部品(3D)の部品の種類毎の情報のポインタ
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// 親マトリックス

		// 部品のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMtxWorldObj_switch_00[nCntParts]);

		// 部品の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_switch_00[nCntParts], &g_aMtxWorldObj_switch_00[nCntParts], &mtxRot);

		// 部品の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_switch_00[nCntParts], &g_aMtxWorldObj_switch_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
			mtxParent = g_aMtxWorldObj_switch_00[pPartsType->nParent];
		}
		else
		{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
			mtxParent = mtxSelf;
		}

		// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(
			&g_aMtxWorldObj_switch_00[nCntParts],
			&g_aMtxWorldObj_switch_00[nCntParts],
			&mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObj_switch_00[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_switch_00[pObj->nType][nCntParts]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_switch_00[pObj->nType][nCntParts][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObj_switch_00[pObj->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// CollisionObj_switch_00関数 - OBJ:スイッチ[00] との当たり判定処理 -
//========================================
int CollisionObj_switch_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if ((GetObj_stage_00()->state == OBJ_STAGE_00_STATE_RAID) || (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_CLEAR))
	{// OBJ:ステージ[00] の状態が襲撃 or クリアの時、
		return -1;	// 処理を終了する
	}

	Obj_switch_00		*pObj	// OBJ:スイッチ[00] の情報のポインタ
						= &g_obj_switch_00;
	int					nIndex	// OBJ:スイッチ[00] の返しの番号
						= -1;
	Obj_switch_00Type	*pType	// OBJ:スイッチ[00] の種類毎の情報のポインタ
						= &g_aObj_switch_00Type[pObj->nType];

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
			pType->hitTestSet.aHitTest[0]
		},
		COLLCHK_MODE_LOOKDOWN);

	if (pObjCollision->bHit)
	{// 当たりフラグが真の時、
		nIndex = 0;	// 返しの番号を設定
	}

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// 番号を返す
	return nIndex;
}

//========================================
// PushObj_switch_00関数 - OBJ:スイッチ[00] の押下処理 -
//========================================
void PushObj_switch_00(void)
{
	Obj_switch_00	*pObj	// OBJ:スイッチ[00] の情報のポインタ
					= &g_obj_switch_00;

	// モーションを押下で設定
	pObj->motion = OBJ_SWITCH_00_MOTION_PUSH;
	SetMotion3D(
		&pObj->partsInfo,
		pObj->motion);	
}