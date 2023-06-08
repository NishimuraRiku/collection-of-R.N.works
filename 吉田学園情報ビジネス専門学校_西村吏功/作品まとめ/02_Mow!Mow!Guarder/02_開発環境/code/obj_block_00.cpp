//========================================
// 
// OBJ:ブロック[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_block_00.cpp ***
//========================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "eff_shadow_00.h"		// EFF:影			[00]
#include "obj_block_00.h"		// OBJ:ブロック		[00]
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "ui_damage_00.h"		// UI :ダメージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:ブロック[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_BLOCK_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_BLOCK_00_TYPE_DATA.txt"

// OBJ:ブロック[00] の最大数
// OBJ:ブロック[00] の種類の最大数
#define OBJ_BLOCK_00_MAX		(256)
#define OBJ_BLOCK_00_TYPE_NUM	(1)

// OBJ:ブロック[00] の無敵時間
#define OBJ_BLOCK_00_INVINCIBLE_TIME	(10)

// OBJ:ブロック[00] のダメージ時間
#define OBJ_BLOCK_00_DAMAGE_TIME	(10)

//****************************************
// 列挙型の定義
//****************************************
// OBJ:ブロック[00] の情報構造体
typedef struct 
{
	// 位置関連
	D3DXVECTOR3	pos;	// 位置
	D3DXVECTOR3	rot;	// 向き

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
}Obj_block_00;

// OBJ:ブロック[00] の種類毎の情報構造体
typedef struct
{
	int			nHP;		// ヒットポイント
	char		aModelPath	// モデルの相対パス
				[TXT_MAX];
	HitTestSet	hitTestSet;	// 当たり判定設定情報
}Obj_block_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ブロック[00] のパラメーターの初期化処理
void InitParameterObj_block_00(Obj_block_00 *pObj);

// OBJ:ブロック[00] の破壊処理
void DestroyObj_block_00(Obj_block_00 *pObj);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureObj_block_00	// OBJ:ブロック[00] のテクスチャへのポインタ
					[OBJ_BLOCK_00_TYPE_NUM]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshObj_block_00		// OBJ:ブロック[00] のメッシュ(頂点情報)へのポインタ
					[OBJ_BLOCK_00_TYPE_NUM]
					= {};
LPD3DXBUFFER		g_pBuffMatObj_block_00	// OBJ:ブロック[00] のマテリアルへのポインタ
					= NULL;
DWORD				g_aNumMatObj_block_00	// OBJ:ブロック[00] のマテリアルの数
					[OBJ_BLOCK_00_TYPE_NUM]
					= {};
D3DXMATRIX			g_mtxWorldObj_block_00;	// OBJ:ブロック[00] のワールドマトリックス
Obj_block_00		g_aObj_block_00			// OBJ:ブロック[00] の情報
					[OBJ_BLOCK_00_MAX];
Obj_block_00Type	g_aObj_block_00Type		// OBJ:ブロック[00] の種類毎の情報
					[OBJ_BLOCK_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_block_00関数 - OBJ:ブロック[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_block_00(Obj_block_00 *pObj)
{
	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pObj->nType					= 0;								// 種類
	pObj->bUse					= false;							// 使用されているかフラグ
	pObj->nHP					=									// ヒットポイント
	pObj->nHPMax				= 0;								// ヒットポイントの上限
	pObj->nCounterInvincible	= 0;								// 無敵カウンター
	pObj->nCounterDamage		= 0;								// ダメージカウンター
	pObj->hitTestInfo			= {};								// 当たり判定の管理情報
}

//========================================
// DestroyObj_block_00関数 - OBJ:ブロック[00] の破壊処理 -
//========================================
void DestroyObj_block_00(Obj_block_00 *pObj)
{
	pObj->bUse = false;				// 使用されていない状態にする
	ItemPriceDownObj_pedestal_00(	// OBJ:台座[00] の商品(ブロック)の値下げ処理
		OBJ_PEDESTAL_00_ITEM_BLOCK);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_block_00関数 - OBJ:ブロック[00] の読み込み処理 -
//========================================
void LoadObj_block_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Obj_block_00Type	*pObjType	// OBJ:ブロック[00] の種類毎の情報
						= g_aObj_block_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_BLOCK_00_TYPE_DATA_FILE_PATH, "r");

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
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pObjType->aModelPath); }		// モデルの相対パス
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitObj_block_00関数 - OBJ:ブロック[00] の初期化処理 -
//========================================
void InitObj_block_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
		= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < OBJ_BLOCK_00_TYPE_NUM; nCntType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			g_aObj_block_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_block_00,
			NULL,
			&g_aNumMatObj_block_00[nCntType],
			&g_aMeshObj_block_00[nCntType]);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_block_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_block_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_block_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_block_00	*pObj	// OBJ:ブロック[00] の情報のポインタ
					= g_aObj_block_00;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		// パラメーターの初期化処理
		InitParameterObj_block_00(pObj);
	}
}

//========================================
// UninitObj_block_00関数 - OBJ:ブロック[00] の終了処理 -
//========================================
void UninitObj_block_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_BLOCK_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_block_00[nCntType] != NULL)
		{
			g_aMeshObj_block_00[nCntType]->Release();
			g_aMeshObj_block_00[nCntType] = NULL;
		}

		// テクスチャの破棄
		for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
			if (g_aTextureObj_block_00[nCntType][nCntMat] != NULL)
			{
				g_aTextureObj_block_00[nCntType][nCntMat]->Release();
				g_aTextureObj_block_00[nCntType][nCntMat] = NULL;
			}
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_block_00 != NULL)
	{
		g_pBuffMatObj_block_00->Release();
		g_pBuffMatObj_block_00 = NULL;
	}
}

//========================================
// UpdateObj_block_00関数 - OBJ:ブロック[00] の更新処理 -
//========================================
void UpdateObj_block_00(void)
{
	Obj_block_00	*pObj	// OBJ:ブロック[00] の情報のポインタ
					= g_aObj_block_00;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
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
}

//========================================
// DrawObj_block_00関数 - OBJ:ブロック[00] の描画処理 -
//========================================
void DrawObj_block_00(void)
{
	Obj_block_00		*pObj				// OBJ:ブロック[00] の情報のポインタ
						= g_aObj_block_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_block_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldObj_block_00);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_block_00, &g_mtxWorldObj_block_00, &mtxRot);
		
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_block_00, &g_mtxWorldObj_block_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_block_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_block_00[pObj->nType]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_BLOCK_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_BLOCK_00_DAMAGE_TIME))),
				255
			});

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_block_00[pObj->nType][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObj_block_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_block_00関数 - OBJ:ブロック[00] の設定処理 -
//========================================
void SetObj_block_00(D3DXVECTOR3 pos) 
{
	Obj_block_00		*pObj	// OBJ:ブロック[00] の情報のポインタ
						= g_aObj_block_00;
	
	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse) 
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_block_00Type	*pType	// OBJ:ブロック[00] の種類毎の情報のポインタ
							= &g_aObj_block_00Type[pObj->nType];

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
// CollisionObj_block_00関数 - OBJ:ブロック[00] との当たり判定処理 -
//========================================
int CollisionObj_block_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool			bHitTemp	// 当たりフラグを保存
					= pObjCollision->bHit;
	Obj_block_00	*pObj		// OBJ:ブロック[00] の情報のポインタ
					= g_aObj_block_00;
	int				nIndex		// CHR:敵[00] の返しの番号
					= -1;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
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
				g_aObj_block_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
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
// DamageObj_block_00関数 - OBJ:ブロック[00] のダメージ処理 -
//========================================
void DamageObj_block_00(int nIndex, int nDamage) 
{
	Obj_block_00	*pObj	// CHR:敵[00] の情報のポインタ
					= &g_aObj_block_00[nIndex];

	if (pObj->nCounterInvincible > 0)
	{// 無敵カウンターが0を上回っている時、
		return;	// 処理を終了する
	}

	nDamage *= fDamageRand();	// 乱数補正
	pObj->nHP -= nDamage;		// ヒットポイントからダメージを減算
	pObj->nCounterDamage		// ダメージカウンターを設定
		= OBJ_BLOCK_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:ダメージ[00] の設定処理
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);

	if (pObj->nHP <= 0)
	{// ヒットポイントが0以下の時、
		DestroyObj_block_00(pObj);	// 破壊処理
	}
	else
	{// ヒットポイントが0を上回っている時、無敵カウンターを設定
		pObj->nCounterInvincible = OBJ_BLOCK_00_INVINCIBLE_TIME;
	}
}

//========================================
// HealAllObj_block_00関数 - OBJ:ブロック[00] の全て回復処理 -
//========================================
void HealAllObj_block_00(void)
{
	Obj_block_00	*pObj	// OBJ:ブロック[00] の情報のポインタ
					= g_aObj_block_00;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_block_00Type	*pType	// OBJ:ブロック[00] の種類毎の情報
							= &g_aObj_block_00Type[pObj->nType];

		pObj->nHPMax = pType->nHP *	// 最大HPを更新
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HPに最大HPを代入
	}
}