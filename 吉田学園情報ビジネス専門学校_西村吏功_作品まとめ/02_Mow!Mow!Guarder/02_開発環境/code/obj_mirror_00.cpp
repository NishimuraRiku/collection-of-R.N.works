//========================================
// 
// OBJ:ミラー[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_mirror_00.cpp ***
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
#include "obj_mirror_00.h"		// OBJ:ミラー		[00]
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "ui_damage_00.h"		// UI :ダメージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:ミラー[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_MIRROR_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_MIRROR_00_TYPE_DATA.txt"

// OBJ:ミラー[00] の最大数
// OBJ:ミラー[00] の種類の最大数
#define OBJ_MIRROR_00_MAX		(256)
#define OBJ_MIRROR_00_TYPE_NUM	(1)

// OBJ:ミラー[00] の無敵時間
#define OBJ_MIRROR_00_INVINCIBLE_TIME	(10)

// OBJ:ミラー[00] のダメージ時間
#define OBJ_MIRROR_00_DAMAGE_TIME	(10)

// OBJ:ミラー[00] の回転量
#define OBJ_MIRROR_00_SPIN	D3DXVECTOR3(0.0f,0.005f,0.0f)

//****************************************
// 列挙型の定義
//****************************************
// OBJ:ミラー[00] の情報構造体
typedef struct 
{
	// 位置関連
	D3DXVECTOR3	pos;	// 位置

	// 分類関連
	int nType;	// 種類

	// 状態関連
	bool	bUse;				// 使用されているかフラグ
	int		nHP;				// ヒットポイント
	int		nHPMax;				// ヒットポイントの上限
	int		nCounterInvincible;	// 無敵カウンター
	int		nCounterDamage;		// ダメージカウンター

	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報
}Obj_mirror_00;

// OBJ:ミラー[00] の管理情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3	rot;	// 現在の向き
	D3DXVECTOR3	rotOld;	// 過去の向き
}Obj_mirror_00Control;

// OBJ:ミラー[00] の種類毎の情報構造体
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
}Obj_mirror_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ミラー[00] のパラメーターの初期化処理
void InitParameterObj_mirror_00(Obj_mirror_00 *pObj);

// OBJ:ミラー[00] の破壊処理
void DestroyObj_mirror_00(Obj_mirror_00 *pObj);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_mirror_00		// OBJ:ミラー[00] のテクスチャへのポインタ
						[OBJ_MIRROR_00_TYPE_NUM]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_mirror_00		// OBJ:ミラー[00] のメッシュ(頂点情報)へのポインタ
						[OBJ_MIRROR_00_TYPE_NUM]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_mirror_00		// OBJ:ミラー[00] のマテリアルへのポインタ
						= NULL;
DWORD					g_aNumMatObj_mirror_00		// OBJ:ミラー[00] のマテリアルの数
						[OBJ_MIRROR_00_TYPE_NUM]
						= {};
D3DXMATRIX				g_mtxWorldObj_mirror_00;	// OBJ:ミラー[00] のワールドマトリックス
Obj_mirror_00			g_aObj_mirror_00			// OBJ:ミラー[00] の情報
						[OBJ_MIRROR_00_MAX];
Obj_mirror_00Control	g_obj_mirror_00Control;		// OBJ:ミラー[00] の管理情報
Obj_mirror_00Type		g_aObj_mirror_00Type		// OBJ:ミラー[00] の種類毎の情報
						[OBJ_MIRROR_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_mirror_00関数 - OBJ:ミラー[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_mirror_00(Obj_mirror_00 *pObj)
{
	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pObj->nType					= 0;								// 種類
	pObj->bUse					= false;							// 使用されているかフラグ
	pObj->nHP					=									// ヒットポイント
	pObj->nHPMax				= 0;								// ヒットポイントの上限
	pObj->nCounterInvincible	= 0;								// 無敵カウンター
	pObj->nCounterDamage		= 0;								// ダメージカウンター
	pObj->hitTestInfo			= {};								// 当たり判定の管理情報
}

//========================================
// InitParameterObj_mirror_00Control関数 - OBJ:ミラー[00] の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterObj_mirror_00Control(void)
{
	Obj_mirror_00Control	*pObjCtl	// OBJ:ミラー[00] の管理情報のポインタ 
							= &g_obj_mirror_00Control;

	pObjCtl->rot	= INITD3DXVECTOR3;	// 現在の向き
	pObjCtl->rotOld = INITD3DXVECTOR3;	// 過去の向き
}

//========================================
// DestroyObj_mirror_00関数 - OBJ:ミラー[00] の破壊処理 -
//========================================
void DestroyObj_mirror_00(Obj_mirror_00 *pObj)
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
// LoadObj_mirror_00関数 - OBJ:ミラー[00] の読み込み処理 -
//========================================
void LoadObj_mirror_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Obj_mirror_00Type	*pObjType	// OBJ:ミラー[00] の種類毎の情報
						= g_aObj_mirror_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_MIRROR_00_TYPE_DATA_FILE_PATH, "r");

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
// InitObj_mirror_00関数 - OBJ:ミラー[00] の初期化処理 -
//========================================
void InitObj_mirror_00(void)
{
	// 管理情報の初期化処理
	InitParameterObj_mirror_00Control();

	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < OBJ_MIRROR_00_TYPE_NUM; nCntType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			g_aObj_mirror_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_mirror_00,
			NULL,
			&g_aNumMatObj_mirror_00[nCntType],
			&g_aMeshObj_mirror_00[nCntType]);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_mirror_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_mirror_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_mirror_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_mirror_00	*pObj	// OBJ:ミラー[00] の情報のポインタ
					= g_aObj_mirror_00;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		// パラメーターの初期化処理
		InitParameterObj_mirror_00(pObj);
	}
}

//========================================
// UninitObj_mirror_00関数 - OBJ:ミラー[00] の終了処理 -
//========================================
void UninitObj_mirror_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_MIRROR_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_mirror_00[nCntType] != NULL)
		{
			g_aMeshObj_mirror_00[nCntType]->Release();
			g_aMeshObj_mirror_00[nCntType] = NULL;
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_mirror_00 != NULL)
	{
		g_pBuffMatObj_mirror_00->Release();
		g_pBuffMatObj_mirror_00 = NULL;
	}
}

//========================================
// UpdateObj_mirror_00関数 - OBJ:ミラー[00] の更新処理 -
//========================================
void UpdateObj_mirror_00(void)
{
	Obj_mirror_00			*pObj		// OBJ:ミラー[00] の情報のポインタ
							= g_aObj_mirror_00;
	Obj_mirror_00Control	*pObjCtl	// OBJ:ミラー[00] の管理情報のポインタ 
							= &g_obj_mirror_00Control;

	pObjCtl->rotOld = pObjCtl->rot;		// 過去の向きを保存
	pObjCtl->rot += OBJ_MIRROR_00_SPIN;	// 回転をかける
	RotControl(&pObjCtl->rot);			// 向きを制御

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		Obj_mirror_00Type	*pType	// OBJ:ミラー[00] の種類毎の情報のポインタ
							= &g_aObj_mirror_00Type[pObj->nType];

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
}

//========================================
// DrawObj_mirror_00関数 - OBJ:ミラー[00] の描画処理 -
//========================================
void DrawObj_mirror_00(void)
{
	Obj_mirror_00		*pObj				// OBJ:ミラー[00] の情報のポインタ
						= g_aObj_mirror_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_mirror_00->GetBufferPointer();

	Obj_mirror_00Control	*pObjCtl	// OBJ:ミラー[00] の管理情報のポインタ 
							= &g_obj_mirror_00Control;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldObj_mirror_00);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObjCtl->rot.y, pObjCtl->rot.x, pObjCtl->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_mirror_00, &g_mtxWorldObj_mirror_00, &mtxRot);
		
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_mirror_00, &g_mtxWorldObj_mirror_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_mirror_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_mirror_00[pObj->nType]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, 
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_MIRROR_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_MIRROR_00_DAMAGE_TIME))),
				255
			});

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_mirror_00[pObj->nType][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObj_mirror_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_mirror_00関数 - OBJ:ミラー[00] の設定処理 -
//========================================
void SetObj_mirror_00(D3DXVECTOR3 pos)
{
	Obj_mirror_00	*pObj	// OBJ:ミラー[00] の情報のポインタ
					= g_aObj_mirror_00;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_mirror_00Type	*pType	// OBJ:ミラー[00] の種類毎の情報のポインタ
							= &g_aObj_mirror_00Type[pObj->nType];

		pObj->pos = pos;			// 位置を代入
		pObj->bUse = true;			// 使用されている状態にする
		pObj->nHPMax = pType->nHP *	// 最大HPを設定
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HPを初期化

		// 繰り返し処理を抜ける
		break;
	}
}

//========================================
// CollisionObj_mirror_00関数 - OBJ:ミラー[00] との当たり判定処理 -
//========================================
int CollisionObj_mirror_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool					bHitTemp	// 当たりフラグを保存
							= pObjCollision->bHit;
	Obj_mirror_00			*pObj		// OBJ:ミラー[00] の情報のポインタ
							= g_aObj_mirror_00;
	Obj_mirror_00Control	*pObjCtl	// OBJ:ミラー[00] の管理情報のポインタ 
							= &g_obj_mirror_00Control;
	int						nIndex		// CHR:敵[00] の返しの番号
							= -1;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
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
				&pObjCtl->rot,
				pObjCtl->rotOld,
				g_aObj_mirror_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
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
// DamageObj_mirror_00関数 - OBJ:ミラー[00] のダメージ処理 -
//========================================
void DamageObj_mirror_00(int nIndex, int nDamage) 
{
	Obj_mirror_00	*pObj	// CHR:敵[00] の情報のポインタ
					= &g_aObj_mirror_00[nIndex];

	if (pObj->nCounterInvincible > 0)
	{// 無敵カウンターが0を上回っている時、
		return;	// 処理を終了する
	}

	nDamage *= fDamageRand();	// 乱数補正
	pObj->nHP -= nDamage;		// ヒットポイントからダメージを減算
	pObj->nCounterDamage		// ダメージカウンターを設定
		= OBJ_MIRROR_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:ダメージ[00] の設定処理
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);

	if (pObj->nHP <= 0)
	{// ヒットポイントが0以下の時、
		DestroyObj_mirror_00(pObj);	// 破壊処理
	}
	else
	{// ヒットポイントが0を上回っている時、無敵カウンターを設定
		pObj->nCounterInvincible = OBJ_MIRROR_00_INVINCIBLE_TIME;
	}
}

//========================================
// HealAllObj_mirror_00関数 - OBJ:反射装置[00] の全て回復処理 -
//========================================
void HealAllObj_mirror_00(void)
{
	Obj_mirror_00	*pObj	// OBJ:反射装置[00] の情報のポインタ
					= g_aObj_mirror_00;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_mirror_00Type	*pType	// OBJ:反射装置[00] の種類毎の情報
							= &g_aObj_mirror_00Type[pObj->nType];

		pObj->nHPMax = pType->nHP *	// 最大HPを更新
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HPに最大HPを代入
	}
}