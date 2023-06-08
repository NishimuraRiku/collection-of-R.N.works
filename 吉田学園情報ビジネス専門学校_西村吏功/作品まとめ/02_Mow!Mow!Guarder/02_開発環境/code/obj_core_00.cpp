//========================================
// 
// OBJ:コア[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_core_00.cpp ***
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
#include "chr_enemy_00.h"		// CHR:敵			[00]
#include "eff_explosion_00.h"	// EFF:爆発			[00]
#include "md_game_00.h"			// MD :ゲーム画面	[00]
#include "obj_core_00.h"		// OBJ:コア			[00]
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include "ui_damage_00.h"		// UI :ダメージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:コア[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_CORE_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_CORE_00_TYPE_DATA.txt"

// OBJ:コア[00] の種類の数
#define OBJ_CORE_00_TYPE_NUM	(2)

// OBJ:コア[00] の無敵時間
#define OBJ_CORE_00_INVINCIBLE_TIME	(10)

// OBJ:コア[00] のダメージ時間
#define OBJ_CORE_00_DAMAGE_TIME	(10)

// OBJ:コア[00] の警告する距離
// OBJ:コア[00] の警告にかかる時間
// OBJ:コア[00] の警告SE
#define OBJ_CORE_00_WARNING_DISTANCE	(40.0f)
#define OBJ_CORE_00_WARNING_TIME		(40)
#define OBJ_CORE_00_WARNING_SE			(SOUND_LABEL_SE_WARNING_000)

// OBJ:コア[00] のゲーム時の種類
#define OBJ_CORE_00_GAME_TYPE		(0)
// OBJ:コア[00] のチュートリアル時の種類
#define OBJ_CORE_00_TUTORIAL_TYPE	(1)

//****************************************
// 構造体の定義
//****************************************
// OBJ:コア[00] の種類毎の情報構造体
typedef struct
{
	int			nHP;			// HP
	SOUND_LABEL	damageSE;		// ダメージSE
	SOUND_LABEL destroySE;		// 破壊SE
	int			nExplosionType;	// 爆発の種類
	HitTestSet	hitTestSet;		// 当たり判定設定情報
	Parts3DSet	partsSet;		// 部品設定情報(3D)
	Motion3DSet motionSet;		// モーション設定情報
}Obj_core_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:コア[00] のパラメーターの初期化処理
void InitParameterObj_core_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureObj_core_00	// OBJ:コア[00] のテクスチャへのポインタ
					[OBJ_CORE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshObj_core_00		// OBJ:コア[00] のメッシュ(頂点情報)へのポインタ
					[OBJ_CORE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
LPD3DXBUFFER		g_pBuffMatObj_core_00	// OBJ:コア[00] のマテリアルへのポインタ
					= NULL;
DWORD				g_aNumMatObj_core_00	// OBJ:コア[00] のマテリアルの数
					[OBJ_CORE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
D3DXMATRIX			g_aMtxWorldObj_core_00	// OBJ:コア[00] のワールドマトリックス
					[PARTS_3D_MAX];
Obj_core_00			g_obj_core_00;			// OBJ:コア[00] の情報
Obj_core_00Type		g_aObj_core_00Type		// OBJ:コア[00] の種類毎の情報
					[OBJ_CORE_00_TYPE_NUM];

//========== *** OBJ:コア[00] の情報を取得 ***
Obj_core_00 *GetObj_core_00(void) 
{
	return &g_obj_core_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_core_00関数 - OBJ:コア[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_core_00(void) 
{
	Obj_core_00	*pObj	// OBJ:コア[00] の情報のポインタ
				= &g_obj_core_00;

	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の位置
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き

	// 現在のモードに応じて
	if (GetMode() == MODE_GAME_00) 
	{// 種類をゲーム時のものに設定
		pObj->nType = OBJ_CORE_00_GAME_TYPE;
	}
	else if (GetMode() == MODE_TUTORIAL_00) 
	{// 種類をチューとリアル時のものに設定
		pObj->nType = OBJ_CORE_00_TUTORIAL_TYPE;
	}

	pObj->bUse					= true;								// 使用されているかフラグ
	pObj->nHP					=									// HP
	pObj->nHPMax				= 0;								// HPの上限
	pObj->nCounterInvincible	= 0;								// 無敵カウンター
	pObj->nCounterDamage		= 0;								// ダメージカウンター
	pObj->nCounterWarning		= 0;								// 警告カウンター
	pObj->bWarning				= false;							// 警告フラグ
	pObj->partsInfo				= {};								// 部品管理
	pObj->motion				= OBJ_CORE_00_MOTION_WAIT;			// モーション
}

//========================================
// MotionSetObj_core_00関数 - OBJ:コア[00] のモーション設定処理 -
//========================================
void MotionSetObj_core_00(void)
{
	Obj_core_00		*pObj		// OBJ:コア[00] の情報のポインタ
					= &g_obj_core_00;
	Obj_core_00Type	*pType		// OBJ:コア[00] の種類毎の情報のポインタ
					= &g_aObj_core_00Type[pObj->nType];
	Motion3D		*pMotion	// 現在のモーションの情報のポインタ
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
// LoadObj_core_00関数 - OBJ:コア[00] の読み込み処理 -
//========================================
void LoadObj_core_00(void)
{
	FILE			*pFile;		// ファイルポインタ
	char			aDataSearch	// データ検索用
					[TXT_MAX];
	Obj_core_00Type	*pObjType	// OBJ:コア[00] の種類毎の情報
					= g_aObj_core_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_CORE_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:コア[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))			{ pObjType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "HIT_POINT:"))		{ fscanf(pFile, "%d", &pObjType->nHP); }			// HP
				else if (!strcmp(aDataSearch, "DAMAGE_SE:"))		{ fscanf(pFile, "%d", &pObjType->damageSE); }		// ダメージSE
				else if (!strcmp(aDataSearch, "DESTROY_SE:"))		{ fscanf(pFile, "%d", &pObjType->destroySE); }		// 破壊SE
				else if (!strcmp(aDataSearch, "EXPLOSION_TYPE:"))	{ fscanf(pFile, "%d", &pObjType->nExplosionType); }	// 爆発の種類
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// 当たり判定設定情報
				else if (!strcmp(aDataSearch, "PARTSSET"))			{ LoadParts3DSet(pFile, &pObjType->partsSet); }		// 部品設定情報
				else if (!strcmp(aDataSearch, "MOTIONSET"))			{ LoadMotion3DSet(pFile, &pObjType->motionSet); }	// モーション設定情報
			}
		}
	}
}

//========================================
// InitObj_core_00関数 - OBJ:コア[00] の初期化処理 -
//========================================
void InitObj_core_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < OBJ_CORE_00_TYPE_NUM; nCntType++)
	{
		Obj_core_00Type	*pType	// OBJ:コア[00] の種類毎の情報のポインタ
						= &g_aObj_core_00Type[nCntType];

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
				&g_pBuffMatObj_core_00,
				NULL,
				&g_aNumMatObj_core_00[nCntType][nCntParts],
				&g_aMeshObj_core_00[nCntType][nCntParts]);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObj_core_00->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_core_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_core_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// パラメーターの初期化処理
	InitParameterObj_core_00();

	Obj_core_00		*pObj	// OBJ:コア[00] の情報のポインタ
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// OBJ:コア[00] の種類毎の情報のポインタ
					= &g_aObj_core_00Type[pObj->nType];

	// 部品(3D)のトランスフォームを初期化
	InitParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// HPを初期化
	pObj->nHP =
	pObj->nHPMax = pType->nHP;
}

//========================================
// UninitObj_core_00関数 - OBJ:コア[00] の終了処理 -
//========================================
void UninitObj_core_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_CORE_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			if (g_aMeshObj_core_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshObj_core_00[nCntType][nCntParts]->Release();
				g_aMeshObj_core_00[nCntType][nCntParts] = NULL;
			}

			// テクスチャの破棄
			for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
				if (g_aTextureObj_core_00[nCntType][nCntParts][nCntMat] != NULL)
				{
					g_aTextureObj_core_00[nCntType][nCntParts][nCntMat]->Release();
					g_aTextureObj_core_00[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_core_00 != NULL)
	{
		g_pBuffMatObj_core_00->Release();
		g_pBuffMatObj_core_00 = NULL;
	}
}

//========================================
// UpdateObj_core_00関数 - OBJ:コア[00] の更新処理 -
//========================================
void UpdateObj_core_00(void)
{
	Obj_core_00		*pObj	// OBJ:コア[00] の情報のポインタ
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
					= &g_aObj_core_00Type[pObj->nType];

	if (pObj->bWarning) 
	{// 警告フラグが真の時、
		float fNotRed =	// 赤度
			1.0f - ((float)Count(&pObj->nCounterWarning, 0, OBJ_CORE_00_WARNING_TIME, NULL, 0, COUNT_TYPE_TURN) /
			(float)OBJ_CORE_00_WARNING_TIME);

		// 光(3D)の色を設定
		SetColorLight3D({ 255,(int)(255 * fNotRed),(int)(255 * fNotRed),255 });

		if (pObj->nCounterWarning <= 0) 
		{// 警告カウンターが0以下の時、
			pObj->bWarning = false;	// 警告フラグを偽にする
		}
	}
	else if (GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL)
	{// [警告フラグが偽] & MD:ゲーム画面[00] の状態が通常の時、
		Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタを取得
						= GetChr_enemy_00();

		for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
		{
			if (!pChr->bUse)
			{// カウントのCHR:敵[00] が使用されていない時、
				continue;	// 繰り返し処理を折り返す
			}

			float fDistance = FindDistanceLookDown(pObj->pos, pChr->pos);	// 2点の距離を取得

			if (fDistance <= OBJ_CORE_00_WARNING_DISTANCE)
			{// 2点の距離が警告する距離以下の時、
				pObj->bWarning = true;				// 警告フラグを真にする
				PlaySound(OBJ_CORE_00_WARNING_SE);	// 警告SEを再生
				break;
			}
		}
	}

	// 部品(3D)のトランスフォームを取得
	GetParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// モーションの設定処理
	MotionSetObj_core_00();

	// モーションの更新処理
	if (UpdateMotion3D(
		pObj->partsInfo.nMotion,
		g_aObj_core_00Type[pObj->nType].motionSet,
		&pObj->partsInfo,
		g_aObj_core_00Type[pObj->nType].partsSet))
	{// モーションがループに達した時、
		Motion3D	*pMotion	// 現在のモーションの情報のポインタ
					= &pType->motionSet.aMotion[pObj->motion];
		if (!pMotion->bLoop)
		{// 現在のモーションのループフラグが偽の時、
			pObj->motion = OBJ_CORE_00_MOTION_WAIT;	// モーションを待機に設定
		}
	}

	if (pObj->nCounterInvincible > 0)
	{// 無敵カウンターが0を上回っている時、
		pObj->nCounterInvincible--;	// 無敵カウンターを減算する
	}
	if (pObj->nCounterDamage > 0)
	{// ダメージカウンターが0を上回っている時、
		pObj->nCounterDamage--;	// ダメージカウンターを減算する
	}
}

//========================================
// DrawObj_core_00関数 - OBJ:コア[00] の描画処理 -
//========================================
void DrawObj_core_00(void)
{
	Obj_core_00			*pObj				// OBJ:コア[00] の情報のポインタ
						= &g_obj_core_00;

	if (!pObj->bUse)
	{// 使用されているフラグが偽の時、
		return;	// 処理を終了する
	}

	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX			mtxSelf;			// 本体のマトリックス

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_core_00->GetBufferPointer();

	bool bLighting =	// ライティングフラグ
		(GetMd_game_00()->state == MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM)
		||
		(GetMd_game_00()->state == MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION) ? false : true;
	float fBrightnessTemp;	// 明るさを保存

	if (!bLighting)
	{// ライティングフラグが偽の時、
		fBrightnessTemp = GetLight3DControl()->fBrightness;	// 明るさを保存
		SetBrightnessLight3D(1.0f);							// 明るさを設定
	}

	// 本体のワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxSelf);

	// 本体の向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// 本体の位置を反映
	D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// 本体のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	Obj_core_00Type	*pType	// OBJ:コア[00] の種類毎の情報のポインタ
					= &g_aObj_core_00Type[pObj->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// 部品(3D)の情報のポインタ
					= &pObj->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// 部品(3D)の部品の種類毎の情報のポインタ
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// 親マトリックス

		// 部品のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMtxWorldObj_core_00[nCntParts]);

		// 部品の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_core_00[nCntParts], &g_aMtxWorldObj_core_00[nCntParts], &mtxRot);

		// 部品の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_core_00[nCntParts], &g_aMtxWorldObj_core_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
			mtxParent = g_aMtxWorldObj_core_00[pPartsType->nParent];
		}
		else
		{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
			mtxParent = mtxSelf;
		}

		// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(
			&g_aMtxWorldObj_core_00[nCntParts],
			&g_aMtxWorldObj_core_00[nCntParts],
			&mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObj_core_00[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_core_00[pObj->nType][nCntParts]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_CORE_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_CORE_00_DAMAGE_TIME))),
				255
			});

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_core_00[pObj->nType][nCntParts][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObj_core_00[pObj->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}

	if (!bLighting)
	{// ライティングフラグが偽の時、
		SetBrightnessLight3D(fBrightnessTemp);	// 明るさを元に戻す
	}
}

//========================================
// CollisionObj_core_00関数 - OBJ:コア[00] との当たり判定処理 -
//========================================
int CollisionObj_core_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if (pObjCollision->bHit)
	{// 当たりフラグが真の時、
		return -1;	// 処理を終了する
	}

	Obj_core_00		*pObj	// OBJ:コア[00] の情報のポインタ
					= &g_obj_core_00;
	int				nIndex	// OBJ:コア[00] の返しの番号
					= -1;
	Obj_core_00Type	*pType	// OBJ:コア[00] の種類毎の情報のポインタ
					= &g_aObj_core_00Type[pObj->nType];

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
// DamageObj_core_00関数 - OBJ:コア[00] のダメージ処理 -
//========================================
void DamageObj_core_00(int nDamage)
{
	if ((GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL) || (nDamage == 0))
	{// OBJ:ステージ[00] の状態がインターバルの時、
		return;	// 処理を終了する
	}

	Obj_core_00		*pObj	// OBJ:コア[00] の情報のポインタ
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// OBJ:コア[00] の種類毎の情報のポインタ
					= &g_aObj_core_00Type[pObj->nType];

	if (pObj->nCounterInvincible > 0)
	{// 無敵カウンターが0を上回っている時、
		return;	// 処理を終了する
	}

	nDamage *= fDamageRand();	// 乱数補正
	pObj->nHP -= nDamage;		// HPからダメージを減算
	pObj->nCounterDamage 		// ダメージカウンターを設定
		= OBJ_CORE_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:ダメージ[00] の設定処理
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);
	PlaySound(pType->damageSE);	// ダメージSEを再生

	if (pObj->nHP <= 0)
	{// HPが0以下の時、
		pObj->nHP = 0;	// HPを0にする
	}
	else
	{// HPが0を上回っている時、無敵カウンターを設定
		pObj->nCounterInvincible = OBJ_CORE_00_INVINCIBLE_TIME;
	}
}

//========================================
// DestroyObj_core_00関数 - OBJ:コア[00] の破壊処理 -
//========================================
void DestroyObj_core_00(void) 
{
	Obj_core_00		*pObj	// OBJ:コア[00] の情報のポインタ
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// OBJ:コア[00] の種類毎の情報のポインタ
					= &g_aObj_core_00Type[pObj->nType];

	pObj->bUse = false;				// 使用されていない状態にする
	SetEff_explosion_00(			// EFF;爆発[00] の設定処理
		pObj->pos,
		pType->nExplosionType);
	PlaySound(pType->destroySE);	// 破壊SEを再生
}