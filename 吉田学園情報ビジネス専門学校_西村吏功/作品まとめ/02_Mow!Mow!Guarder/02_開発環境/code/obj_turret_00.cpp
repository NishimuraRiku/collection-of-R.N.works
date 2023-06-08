//========================================
// 
// OBJ:タレット[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_turret_00.cpp ***
//========================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "atk_bullet_00.h"		// ATK:弾			[00]
#include "chr_enemy_00.h"		// CHR:敵			[00]
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "eff_shadow_00.h"		// EFF:影			[00]
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "obj_turret_00.h"		// OBJ:タレット		[00]
#include "ui_damage_00.h"		// UI :ダメージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:タレット[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_TURRET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_TURRET_00_TYPE_DATA.txt"

// OBJ:タレット[00] の最大数
// OBJ:タレット[00] の種類の最大数
#define OBJ_TURRET_00_MAX		(256)
#define OBJ_TURRET_00_TYPE_NUM	(1)

// OBJ:タレット[00] の無敵時間
#define OBJ_TURRET_00_INVINCIBLE_TIME	(10)

// OBJ:タレット[00] のダメージ時間
#define OBJ_TURRET_00_DAMAGE_TIME	(10)

//****************************************
// 構造体の定義
//****************************************
// OBJ:タレット[00] の情報構造体
typedef struct 
{
	// 位置関連
	D3DXVECTOR3	pos;		// 位置
	D3DXVECTOR3	rot;		// 向き
	D3DXVECTOR3 targetRot;	// 目標向き
	D3DXVECTOR3 rotTemp;	// 向きの保存

	// 分類関連
	int nType;	// 種類

	// 状態関連
	bool	bUse;				// 使用されているかフラグ
	int		nHP;				// ヒットポイント
	int		nHPMax;				// ヒットポイントの上限
	int		nCounterInvincible;	// 無敵カウンター
	int		nCounterDamage;		// ダメージカウンター
	bool	bAim;				// 狙いフラグ
	int		nCounterShot;		// 発射カウンター
	int		nRigorCounter;		// 硬直カウンター

	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報
}Obj_turret_00;

// OBJ:タレット[00] の種類毎の情報構造体
typedef struct
{
	int			nHP;			// ヒットポイント
	int			nBulletType;	// 弾の種類
	int			nShotTime;		// 発射間隔
	int			nRigorTime;		// 硬直時間
	float		fSearchRange;	// 索敵範囲
	char		aModelPath		// モデルの相対パス
				[TXT_MAX];
	HitTestSet	hitTestSet;		// 当たり判定設定情報
}Obj_turret_00Type;

// OBJ:タレット[00] の管理情報構造体
typedef struct
{
	OBJ_TURRET_00_CONTROL_STATE state;	// 状態
}Obj_turret_00Control;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:タレット[00] のパラメーターの初期化処理
void InitParameterObj_turret_00(Obj_turret_00 *pObj);

// OBJ:タレット[00] の破壊処理
void DestroyObj_turret_00(Obj_turret_00 *pObj);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_turret_00		// OBJ:タレット[00] のテクスチャへのポインタ
						[OBJ_TURRET_00_TYPE_NUM]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_turret_00		// OBJ:タレット[00] のメッシュ(頂点情報)へのポインタ
						[OBJ_TURRET_00_TYPE_NUM]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_turret_00		// OBJ:タレット[00] のマテリアルへのポインタ
						= NULL;
DWORD					g_aNumMatObj_turret_00		// OBJ:タレット[00] のマテリアルの数
						[OBJ_TURRET_00_TYPE_NUM]
						= {};
D3DXMATRIX				g_mtxWorldObj_turret_00;	// OBJ:タレット[00] のワールドマトリックス
Obj_turret_00			g_aObj_turret_00			// OBJ:タレット[00] の情報
						[OBJ_TURRET_00_MAX];
Obj_turret_00Control	g_obj_turret_00Control;		// OBJ:タレット[00] の管理情報
Obj_turret_00Type		g_aObj_turret_00Type		// OBJ:タレット[00] の種類毎の情報
						[OBJ_TURRET_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_turret_00関数 - OBJ:タレット[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_turret_00(Obj_turret_00 *pObj)
{
	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pObj->targetRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標向き
	pObj->rotTemp				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの保存
	pObj->nType					= 0;								// 種類
	pObj->bUse					= false;							// 使用されているかフラグ
	pObj->nHP					=									// ヒットポイント
	pObj->nHPMax				= 0;								// ヒットポイントの上限
	pObj->nCounterInvincible	= 0;								// 無敵カウンター
	pObj->nCounterDamage		= 0;								// ダメージカウンター
	pObj->bAim					= false;							// 狙いフラグ
	pObj->nCounterShot			= 0;								// 発射カウンター
	pObj->nRigorCounter			= 0;								// 硬直カウンター
	pObj->hitTestInfo			= {};								// 当たり判定の管理情報
}

//========================================
// InitParameterControlObj_turret_00関数 - OBJ:タレット[00] の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterControlObj_turret_00(void) 
{
	Obj_turret_00Control	*pObjCtl	// OBJ:タレット[00] の管理情報のポインタ
							= &g_obj_turret_00Control;

	pObjCtl->state = OBJ_TURRET_00_CONTROL_STATE_DYNAMIC;	// 状態
}

//========================================
// DestroyObj_turret_00関数 - OBJ:タレット[00] の破壊処理 -
//========================================
void DestroyObj_turret_00(Obj_turret_00 *pObj)
{
	pObj->bUse = false;				// 使用されていない状態にする
	ItemPriceDownObj_pedestal_00(	// OBJ:台座[00] の商品(タレット)の値下げ処理
		OBJ_PEDESTAL_00_ITEM_TURRET);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_turret_00関数 - OBJ:タレット[00] の読み込み処理 -
//========================================
void LoadObj_turret_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Obj_turret_00Type	*pObjType	// OBJ:タレット[00] の種類毎の情報
						= g_aObj_turret_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_TURRET_00_TYPE_DATA_FILE_PATH, "r");

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
				else if (!strcmp(aDataSearch, "HIT_POINT:"))	{ fscanf(pFile, "%d", &pObjType->nHP); }			// ヒットポイント
				else if (!strcmp(aDataSearch, "BULLET_TYPE:"))	{ fscanf(pFile, "%d", &pObjType->nBulletType); }	// 弾の種類
				else if (!strcmp(aDataSearch, "SHOT_TIME:"))	{ fscanf(pFile, "%d", &pObjType->nShotTime); }		// 発射間隔
				else if (!strcmp(aDataSearch, "RIGOR_TIME:"))	{ fscanf(pFile, "%d", &pObjType->nRigorTime); }		// 硬直時間
				else if (!strcmp(aDataSearch, "SEARCH_RANGE:")) { fscanf(pFile, "%f", &pObjType->fSearchRange); }	// 索敵範囲
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pObjType->aModelPath); }		// モデルの相対パス
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitObj_turret_00関数 - OBJ:タレット[00] の初期化処理 -
//========================================
void InitObj_turret_00(void)
{
	// 管理情報のパラメーターの初期化処理
	InitParameterControlObj_turret_00();

	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < OBJ_TURRET_00_TYPE_NUM; nCntType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			g_aObj_turret_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_turret_00,
			NULL,
			&g_aNumMatObj_turret_00[nCntType],
			&g_aMeshObj_turret_00[nCntType]);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_turret_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_turret_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_turret_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_turret_00	*pObj	// OBJ:タレット[00] の情報のポインタ
					= g_aObj_turret_00;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		// パラメーターの初期化処理
		InitParameterObj_turret_00(pObj);
	}
}

//========================================
// UninitObj_turret_00関数 - OBJ:タレット[00] の終了処理 -
//========================================
void UninitObj_turret_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_TURRET_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_turret_00[nCntType] != NULL)
		{
			g_aMeshObj_turret_00[nCntType]->Release();
			g_aMeshObj_turret_00[nCntType] = NULL;
		}

		// テクスチャの破棄
		for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
			if (g_aTextureObj_turret_00[nCntType][nCntMat] != NULL)
			{
				g_aTextureObj_turret_00[nCntType][nCntMat]->Release();
				g_aTextureObj_turret_00[nCntType][nCntMat] = NULL;
			}
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_turret_00 != NULL)
	{
		g_pBuffMatObj_turret_00->Release();
		g_pBuffMatObj_turret_00 = NULL;
	}
}

//========================================
// UpdateObj_turret_00関数 - OBJ:タレット[00] の更新処理 -
//========================================
void UpdateObj_turret_00(void)
{
	Obj_turret_00			*pObj		// OBJ:タレット[00] の情報のポインタ
							= g_aObj_turret_00;
	Obj_turret_00Control	*pObjCtl	// OBJ:タレット[00] の管理情報のポインタ
							= &g_obj_turret_00Control;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (pObjCtl->state != OBJ_TURRET_00_CONTROL_STATE_STATIC)
		{// 管理状態が静的で無い時、
			Obj_turret_00Type	*pType	// OBJ:タレット[00] の種類毎の情報のポインタ
				= &g_aObj_turret_00Type[pObj->nType];

			if (!pObj->bAim)
			{// 狙いフラグが偽の時、
				Chr_enemy_00	*pChr = GetChr_enemy_00();	// CHR:敵[00] の情報のポインタを取得
				float			fNear = 0;					// 最も近い距離
				D3DXVECTOR3		targetPos;					// 目標位置
				bool			bAim = false;				// 狙いフラグ

				for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
				{
					if ((!pChr->bUse) || (pChr->bRecognitionOff))
					{// 使用されていない状態 or 認識OFFフラグが真の時、
						continue;	// 繰り返し処理を折り返す
					}

					float fDistance = FindDistanceLookDown(pObj->pos, pChr->pos);	// 2点の距離を取得

					if ((fDistance < fNear) || (!bAim))
					{// 2点の距離が最も近い距離を下回っている or 狙いフラグが偽の時、
						fNear = fDistance;		// 最も近い距離を設定
						targetPos = pChr->pos;	// 目標位置を設定
						bAim = true;			// 狙いフラグを真にする
					}
				}

				if ((fNear <= pType->fSearchRange) && (bAim))
				{// 最も近い距離が索敵範囲内 & 狙いフラグが真の時、
					pObj->rotTemp = pObj->rot;	// 向きを記憶
					pObj->targetRot.y			// 目標向きを設定
						= FindAngleLookDown(pObj->pos, targetPos);
					pObj->bAim = true;			// 狙いフラグを真にする
					pObj->nCounterShot			// 発射カウンターを設定
						= pType->nShotTime;
				}
			}

			if (pObj->nCounterShot > 0)
			{// 発射カウンターが0を上回っている時、
				pObj->nCounterShot--;	// 発射カウンターを減算
				pObj->rot.y += (AngleDifference(pObj->rotTemp.y, pObj->targetRot.y) / (float)pType->nShotTime);

				if (pObj->nCounterShot <= 0)
				{// 発射カウンターが0以下の時、
					pObj->nRigorCounter	// 硬直カウンターを設定
						= pType->nRigorTime;
					SetAtk_bullet_00(	// ATK:弾[00] の設定処理
						pObj->pos,
						pObj->rot,
						pType->nBulletType,
						ATK_BULLET_00_PARENTTYPE_PLAYER);
				}
			}

			if (pObj->nRigorCounter > 0)
			{// 硬直カウンターが0を上回っている時。
				if (--pObj->nRigorCounter <= 0)
				{// 硬直カウンターを減算した結果0以下の時、
					pObj->bAim = false;	// 狙いフラグを偽にする
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

			{
				float fDitance = FindDistanceLookDown(pObj->pos, GetChr_player_00()->pos);
				if (fDitance <= CHR_PLAYER_00_HPRATE_DISPLAY_DISTANCE)
				{// CHR;プレイヤー[00] までの距離がHP割合を表示する距離以下の時、HP割合テキストを表示
					char aString[TXT_MAX];
					float fHPRate = ((float)pObj->nHP / (float)pObj->nHPMax);
					sprintf(aString, "%d%%", (int)(fHPRate * 100));
					SetText3D(
						aString,
						FONT_002,
						DISPLAY_CENTER,
						pObj->pos,
						{ 255,(int)(255 * fHPRate),(int)(255 * fHPRate),(int)(255 * (1.0f - ((float)fDitance / (float)CHR_PLAYER_00_HPRATE_DISPLAY_DISTANCE))) },
						CHR_PLAYER_00_HPRATE_TEXT_SCALE, CHR_PLAYER_00_HPRATE_TEXT_SCALE, false, false, false);
				}
			}
		}
		else 
		{// 管理状態が静的の時、
			pObj->nCounterDamage = 0;	// ダメージカウンターを初期化する
		}
	}
}

//========================================
// DrawObj_turret_00関数 - OBJ:タレット[00] の描画処理 -
//========================================
void DrawObj_turret_00(void)
{
	Obj_turret_00		*pObj				// OBJ:タレット[00] の情報のポインタ
						= g_aObj_turret_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_turret_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldObj_turret_00);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_turret_00, &g_mtxWorldObj_turret_00, &mtxRot);
		
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_turret_00, &g_mtxWorldObj_turret_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_turret_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_turret_00[pObj->nType]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_TURRET_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_TURRET_00_DAMAGE_TIME))),
				255
			});

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_turret_00[pObj->nType][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObj_turret_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_turret_00関数 - OBJ:タレット[00] の設定処理 -
//========================================
void SetObj_turret_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	Obj_turret_00	*pObj	// OBJ:タレット[00] の情報のポインタ
					= g_aObj_turret_00;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse) 
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_turret_00Type	*pType	// OBJ:タレット[00] の種類毎の情報のポインタ
							= &g_aObj_turret_00Type[pObj->nType];

		pObj->pos = pos;			// 位置を代入
		pObj->rot = rot;			// 向きを代入
		pObj->bUse = true;			// 使用されている状態にする
		pObj->nHPMax = pType->nHP *	// 最大HPを設定
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HPを初期化
		pObj->bAim = false;			// 狙いフラグを初期化
		pObj->nCounterShot = 0;		// 発射カウンターを初期化
		pObj->nRigorCounter = 0;	// 硬直カウンターを初期化

		// 繰り返し処理を抜ける
		break;
	}
}

//========================================
// CollisionObj_turret_00関数 - OBJ:タレット[00] との当たり判定処理 -
//========================================
int CollisionObj_turret_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool			bHitTemp	// 当たりフラグを保存
					= pObjCollision->bHit;
	Obj_turret_00	*pObj		// OBJ:タレット[00] の情報のポインタ
					= g_aObj_turret_00;
	int				nIndex		// CHR:敵[00] の返しの番号
					= -1;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
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
				g_aObj_turret_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		if ((pObjCollision->bHit) && (!bHitTemp))
		{// 当たりフラグが真の時、
			nIndex = nCntObj;	// 返しの番号を設定
			bHitTemp = true;	// 当たりフラグの保存を真にする
		}
	}

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// 番号を返す
	return nIndex;
}

//========================================
// DamageObj_turret_00関数 - OBJ:タレット[00] のダメージ処理 -
//========================================
void DamageObj_turret_00(int nIndex, int nDamage) 
{
	Obj_turret_00	*pObj	// CHR:敵[00] の情報のポインタ
					= &g_aObj_turret_00[nIndex];

	if (pObj->nCounterInvincible > 0)
	{// 無敵カウンターが0を上回っている時、
		return;	// 処理を終了する
	}

	nDamage *= fDamageRand();	// 乱数補正
	pObj->nHP -= nDamage;		// ヒットポイントからダメージを減算
	pObj->nCounterDamage		// ダメージカウンターを設定
		= OBJ_TURRET_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:ダメージ[00] の設定処理
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);

	if (pObj->nHP <= 0)
	{// ヒットポイントが0以下の時、
		DestroyObj_turret_00(pObj);	// 破壊処理
	}
	else
	{// ヒットポイントが0を上回っている時、無敵カウンターを設定
		pObj->nCounterInvincible = OBJ_TURRET_00_INVINCIBLE_TIME;
	}
}

//========================================
// SetObj_turret_00ControlState関数 - OBJ:タレット[00] の管理状態設定処理 -
//========================================
void SetObj_turret_00ControlState(OBJ_TURRET_00_CONTROL_STATE state) 
{
	Obj_turret_00Control	*pObjCtl	// OBJ:タレット[00] の管理情報のポインタ
							= &g_obj_turret_00Control;

	pObjCtl->state = state;	// 状態を代入
}

//========================================
// HealAllObj_turret_00関数 - OBJ:タレット[00] の全て回復処理 -
//========================================
void HealAllObj_turret_00(void)
{
	Obj_turret_00	*pObj	// OBJ:タレット[00] の情報のポインタ
					= g_aObj_turret_00;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_turret_00Type	*pType	// OBJ:タレット[00] の種類毎の情報
							= &g_aObj_turret_00Type[pObj->nType];

		pObj->nHPMax = pType->nHP *	// 最大HPを更新
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HPに最大HPを代入
	}
}