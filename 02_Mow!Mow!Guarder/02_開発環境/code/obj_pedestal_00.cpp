//========================================
// 
// OBJ:台座[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_pedestal_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "specific.h"
#include "text.h"
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "md_game_00.h"			// MD :ゲーム画面	[00]
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:台座[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_PEDESTAL_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_PEDESTAL_00_TYPE_DATA.txt"

// OBJ:台座[00] の種類の数
#define OBJ_PEDESTAL_00_TYPE_NUM	(1)

// OBJ:台座[00] の非到着時の相対位置Y
#define OBJ_PEDESTAL_00_NON_ARRIVAL_RELATIVE_POS_Y	(-6)

// OBJ:台座[00] の注目にかかる時間
#define OBJ_PEDESTAL_00_NOTICE_TIME	(10)

// OBJ:台座[00] の値段テキストの開始相対位置Y
// OBJ:台座[00] の値段テキストの終了相対位置Y
#define OBJ_PEDESTAL_00_PRICE_TEXT_START_RELATIVE_POS_Y	(4)
#define OBJ_PEDESTAL_00_PRICE_TEXT_END_RELATIVE_POS_Y	(8)

// OBJ:台座[00] の商品の値上げカウント毎の上昇値
#define OBJ_PEDESTAL_00_PRICEUP_ADDRATE	(0.1f)

// OBJ:台座[00] のプレイヤーのHPの回復量
#define OBJ_PEDESTAL_00_PLAYER_HP_HEAL	(50)

//****************************************
// 構造体の定義
//****************************************
// OBJ:台座[00] の種類毎の情報構造体
typedef struct
{
	char		aModelPath	// モデルの相対パス
				[TXT_MAX];
	HitTestSet	hitTestSet;	// 当たり判定設定情報
}Obj_pedestal_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:台座[00] のパラメーターの初期化処理
// OBJ:台座[00] の管理情報のパラメーターの初期化処理
void InitParameterObj_pedestal_00(Obj_pedestal_00 *pObj);
void InitParameterObj_pedestal_00Control(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9			g_aTextureObj_pedestal_00		// OBJ:台座[00] のテクスチャへのポインタ
							[OBJ_PEDESTAL_00_TYPE_NUM]
							[MATERIAL_3D_MAX]
							= {};
LPD3DXMESH					g_aMeshObj_pedestal_00			// OBJ:台座[00] のメッシュ(頂点情報)へのポインタ
							[OBJ_PEDESTAL_00_TYPE_NUM]
							= {};
LPD3DXBUFFER				g_pBuffMatObj_pedestal_00		// OBJ:台座[00] のマテリアルへのポインタ
							= NULL;
DWORD						g_aNumMatObj_pedestal_00		// OBJ:台座[00] のマテリアルの数
							[OBJ_PEDESTAL_00_TYPE_NUM]
							= {};
D3DXMATRIX					g_mtxWorldObj_pedestal_00;		// OBJ:台座[00] のワールドマトリックス
Obj_pedestal_00				g_aObj_pedestal_00				// OBJ:台座[00] の情報
							[OBJ_PEDESTAL_00_MAX];
Obj_pedestal_00Type			g_aObj_pedestal_00Type			// OBJ:台座[00] の種類毎の情報
							[OBJ_PEDESTAL_00_TYPE_NUM];
Obj_pedestal_00Control		g_obj_pedestal_00Control;		// OBJ:台座[00] の管理情報
Obj_pedestal_00ItemControl	g_aObj_pedestal_00ItemControl	// OBJ:台座[00] の商品毎の管理情報
							[OBJ_PEDESTAL_00_ITEM_MAX];

// OBJ:台座[00] の商品情報
const Obj_pedestal_00Item g_aObj_pedestal_00Item[OBJ_PEDESTAL_00_ITEM_MAX] =
{
	5,	// ブロック
	25,	// タレット
	10,	// 反射装置
	20,	// 放電装置
	25,	// プレイヤーのHPを回復
	30,	// ユニットのHPを回復
	20,	// 剣の攻撃力アップ
	25,	// 弾の攻撃力アップ
};

//========== *** OBJ:台座[00] の情報を取得 ***
Obj_pedestal_00 *GetObj_pedestal_00(void) 
{
	return g_aObj_pedestal_00;
}

//========== *** OBJ:台座[00] の管理情報を取得 ***
Obj_pedestal_00Control *GetObj_pedestal_00Control(void)
{
	return &g_obj_pedestal_00Control;
}

//========== *** OBJ:台座[00] の商品毎の情報を取得 ***
const Obj_pedestal_00Item *GetObj_pedestal_00Item(void)
{
	return g_aObj_pedestal_00Item;
}

//========== *** OBJ:台座[00] の商品毎の情報を取得 ***
Obj_pedestal_00ItemControl *GetObj_pedestal_00ItemControl(void)
{
	return g_aObj_pedestal_00ItemControl;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_pedestal_00関数 - OBJ:台座[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_pedestal_00(Obj_pedestal_00 *pObj)
{
	pObj->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の位置
	pObj->basePos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 基準位置
	pObj->rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pObj->nType				= 0;								// 種類
	pObj->bUse				= false;							// 使用されているフラグ
	pObj->nCounterNotice	= 0;								// 注目カウンター
}

//========================================
// InitParameterObj_pedestal_00Control関数 - OBJ:台座[00] の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterObj_pedestal_00Control(void) 
{
	Obj_pedestal_00Control	*pObj	// OBJ:台座[00] の管理情報
							= &g_obj_pedestal_00Control;

	pObj->nCntItems			= 0;	// 品揃えカウント
	pObj->nCounterArrival	= 0;	// 到着カウンター
}

//========================================
// InitParameterObj_pedestal_00ItemControl関数 - OBJ:台座[00] の商品毎の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterObj_pedestal_00ItemControl(Obj_pedestal_00ItemControl	*pObjItmCtl)
{
	pObjItmCtl->nCntPriceUp = 0;	// 値上げカウント
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_pedestal_00関数 - OBJ:台座[00] の読み込み処理 -
//========================================
void LoadObj_pedestal_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Obj_pedestal_00Type	*pObjType	// OBJ:台座[00] の種類毎の情報
						= g_aObj_pedestal_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_PEDESTAL_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:台座[00] の種類毎の情報の読み込みを開始
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
// InitObj_pedestal_00関数 - OBJ:台座[00] の初期化処理 -
//========================================
void InitObj_pedestal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < OBJ_PEDESTAL_00_TYPE_NUM; nCntType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			g_aObj_pedestal_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_pedestal_00,
			NULL,
			&g_aNumMatObj_pedestal_00[nCntType],
			&g_aMeshObj_pedestal_00[nCntType]);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_pedestal_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_pedestal_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_pedestal_00[nCntType][nCntMat]);
			}
		}
	}

	// 管理情報の初期化処理
	InitParameterObj_pedestal_00Control();

	Obj_pedestal_00	*pObj	// OBJ:台座[00] の情報のポインタ
					= g_aObj_pedestal_00;

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		// パラメーターの初期化処理
		InitParameterObj_pedestal_00(pObj);
	}

	Obj_pedestal_00ItemControl	*pObjItmCtl	// OBJ:台座[00] の商品毎の管理情報
								= g_aObj_pedestal_00ItemControl;

	// 商品毎の管理情報のパラメーターの初期化処理
	for (int nCntItm = 0; nCntItm < OBJ_PEDESTAL_00_ITEM_MAX; nCntItm++, pObjItmCtl++)
	{
		InitParameterObj_pedestal_00ItemControl(pObjItmCtl);
	}
}

//========================================
// UninitObj_pedestal_00関数 - OBJ:台座[00] の終了処理 -
//========================================
void UninitObj_pedestal_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_PEDESTAL_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_pedestal_00[nCntType] != NULL)
		{
			g_aMeshObj_pedestal_00[nCntType]->Release();
			g_aMeshObj_pedestal_00[nCntType] = NULL;
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_pedestal_00 != NULL)
	{
		g_pBuffMatObj_pedestal_00->Release();
		g_pBuffMatObj_pedestal_00 = NULL;
	}
}

//========================================
// UpdateObj_pedestal_00関数 - OBJ:台座[00] の更新処理 -
//========================================
void UpdateObj_pedestal_00(void)
{
	Obj_pedestal_00Control	*pObjCtl	// OBJ:台座[00] の管理情報のポインタ
							= &g_obj_pedestal_00Control;

	if ((GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL) ||
		(GetMd_game_00()->state == MD_GAME_00_STATE_TIPS_WAIT))
	{// MD:ゲーム画面[00] の状態が通常orTIPS待ちの時、
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

	Obj_pedestal_00		*pObj	// OBJ:台座[00] の情報のポインタ
						= g_aObj_pedestal_00;
	Obj_stage_00Items	*pItems	// OBJ:ステージ[00] の品揃え情報のポインタ
						= &GetObj_stage_00Type()->itemsSet.aItems[pObjCtl->nCntItems];

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されている状態でない時、
			continue;	// 処理を折り返す
		}

		// Y座標を到着カウンターに応じて設定
		pObj->pos.y = pObj->basePos.y + (OBJ_PEDESTAL_00_NON_ARRIVAL_RELATIVE_POS_Y * (1.0f - ((float)pObjCtl->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME)));

		if (GetChr_player_00()->nPedestalIndex == nCntObj) 
		{// CHR:プレイヤー[00] の台座の番号がカウントと一致している時、
			pObj->nCounterNotice++;	// 注目カウンターを加算
		}
		else 
		{// CHR:プレイヤー[00] の台座の番号がカウントと一致していない時、
			pObj->nCounterNotice--;	// 注目カウンターを減算
		}

		// 注目カウンターを制御
		IntControl(&pObj->nCounterNotice, OBJ_PEDESTAL_00_NOTICE_TIME, 0);

		{
			char aString[TXT_MAX];
			OBJ_PEDESTAL_00_ITEM itm = pItems->aItem[nCntObj]; // 該当する商品番号
			sprintf(aString, "$%d", (int)(g_aObj_pedestal_00Item[pItems->aItem[nCntObj]].nPrice *
				(1.0f + (g_aObj_pedestal_00ItemControl[itm].nCntPriceUp * OBJ_PEDESTAL_00_PRICEUP_ADDRATE))));
			D3DXVECTOR3 setPos = pObj->pos;
			setPos.y +=
				(OBJ_PEDESTAL_00_PRICE_TEXT_START_RELATIVE_POS_Y * (1.0f - ((float)pObj->nCounterNotice / (float)OBJ_PEDESTAL_00_NOTICE_TIME))) +
				(OBJ_PEDESTAL_00_PRICE_TEXT_END_RELATIVE_POS_Y * ((float)pObj->nCounterNotice / (float)OBJ_PEDESTAL_00_NOTICE_TIME));
			SetText3D(
				aString,
				FONT_002,
				DISPLAY_CENTER,
				setPos,
				{ 255,255,255,(int)(255 * ((float)pObj->nCounterNotice / (float)OBJ_PEDESTAL_00_NOTICE_TIME)) },
				1.0f, 1.0f, false, false, true);
		}
	}
}

//========================================
// DrawObj_pedestal_00関数 - OBJ:台座[00] の描画処理 -
//========================================
void DrawObj_pedestal_00(void)
{
	Obj_pedestal_00		*pObj	// OBJ:台座[00] の情報のポインタ
						= g_aObj_pedestal_00;
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot,	// 計算用マトリックス
						mtxTrans;
	D3DMATERIAL9		matDef;	// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;	// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_pedestal_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			continue;	// 繰り返し処理を折り返す
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldObj_pedestal_00);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_pedestal_00, &g_mtxWorldObj_pedestal_00, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_pedestal_00, &g_mtxWorldObj_pedestal_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_pedestal_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_pedestal_00[pObj->nType]; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_pedestal_00[pObj->nType][nCntMat]);
			
			// ポリゴン(パーツ)の描画
			g_aMeshObj_pedestal_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_pedestal_00関数 - OBJ:台座[00] の設定処理 -
//========================================
void SetObj_pedestal_00(D3DXVECTOR3 pos, int nType)
{
	Obj_pedestal_00		*pObj	// OBJ:台座[00] の情報のポインタ
						= g_aObj_pedestal_00;
	Obj_pedestal_00Type	*pType	// OBJ:台座[00] の種類毎の情報のポインタ
						= &g_aObj_pedestal_00Type[nType];

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pObj->pos = pos;		// 位置を代入
		pObj->basePos = pos;	// 基準位置を代入
		pObj->bUse = true;		// 使用されている状態にする
		pObj->nType = nType;	// 種類を代入

		// 繰り返し処理を抜ける
		break;
	}
}

//========================================
// CollisionObj_pedestal_00関数 - OBJ:台座[00] との当たり判定処理 -
//========================================
int CollisionObj_pedestal_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if (pObjCollision->bHit)
	{// 当たりフラグが真の時、
		return -1;	// 処理を終了する
	}

	Obj_pedestal_00	*pObj	// OBJ:台座[00] の情報のポインタ
					= g_aObj_pedestal_00;
	int				nIndex	// OBJ:台座[00] の返しの番号
					= -1;

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// 使用されていない状態の時、
			// 処理を折り返す
			continue;
		}

		Obj_pedestal_00Type	*pType	// OBJ:台座[00] の種類毎の情報のポインタ
							= &g_aObj_pedestal_00Type[pObj->nType];

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

		if(pObjCollision->bHit)
		{// 当たりフラグが真の時、
			nIndex = nCntObj;	// 返しの番号を設定
			break;				// 繰り返し処理を抜ける
		}
	}

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// 番号を返す
	return nIndex;
}

//========================================
// PurchaseObj_pedestal_00関数 - OBJ:台座[00] の購入処理 -
//========================================
bool PurchaseObj_pedestal_00(void) 
{
	Chr_player_00				*pChr		// CHR:プレイヤー[00] の情報のポインタを取得
								= GetChr_player_00();
	Obj_pedestal_00				*pObj		// OBJ:台座[00] の情報のポインタ
								= g_aObj_pedestal_00;
	Obj_pedestal_00Control		*pObjCtl	// OBJ:台座[00] の管理情報のポインタ
								= &g_obj_pedestal_00Control;
	Obj_stage_00Items			*pItems		// OBJ:ステージ[00] の品揃え情報のポインタ
								= &GetObj_stage_00Type()->itemsSet.aItems[pObjCtl->nCntItems];
	OBJ_PEDESTAL_00_ITEM		itm			// 該当する商品番号
								= pItems->aItem[pChr->nPedestalIndex];

	if (!IntConsumption(
		&pChr->nCoin,
		g_aObj_pedestal_00Item[itm].nPrice *
		(1.0f + (g_aObj_pedestal_00ItemControl[itm].nCntPriceUp * OBJ_PEDESTAL_00_PRICEUP_ADDRATE))))
	{// コインを消費した結果足りなかった時、
		return false;	// 処理を終了する
	}

	// 商品の値上げ処理
	ItemPriceUpObj_pedestal_00(itm);

	switch (pItems->aItem[pChr->nPedestalIndex])
	{
	case /*/ ブロック             /*/OBJ_PEDESTAL_00_ITEM_BLOCK			:pChr->aItemCount[CHR_PLAYER_00_ITEM_BLOCK]++		; break;
	case /*/ タレット             /*/OBJ_PEDESTAL_00_ITEM_TURRET		:pChr->aItemCount[CHR_PLAYER_00_ITEM_TURRET]++		; break;
	case /*/ 反射装置             /*/OBJ_PEDESTAL_00_ITEM_REFLECTOR		:pChr->aItemCount[CHR_PLAYER_00_ITEM_REFLECTOR]++	; break;
	case /*/ 放電装置             /*/OBJ_PEDESTAL_00_ITEM_DISCHARGER	:pChr->aItemCount[CHR_PLAYER_00_ITEM_DISCHARGER]++	; break;
	case /*/ プレイヤーのHPを回復 /*/OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS:HealChr_player_00(OBJ_PEDESTAL_00_PLAYER_HP_HEAL)	; break;
	case /*/ ユニットのHPを回復   /*/OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS	:HealUnitAll()										; break;
	case /*/ 剣の攻撃力アップ     /*/OBJ_PEDESTAL_00_ITEM_SWORD_ATK_UP	:pChr->nSwordAtkUpCount++							; break;
	case /*/ 弾の攻撃力アップ     /*/OBJ_PEDESTAL_00_ITEM_BULLET_ATK_UP	:pChr->nBulletAtkUpCount++							; break;
	}

	return true;
}

//========================================
// ItemsChangeObj_pedestal_00関数 - OBJ:台座[00] の品揃え変更処理 -
//========================================
void ItemsChangeObj_pedestal_00(void)
{
	Obj_pedestal_00Control	*pObjCtl	// OBJ:台座[00] の管理情報のポインタ
							= &g_obj_pedestal_00Control;

	// 品揃え番号を加算し上限で剰余算
	++pObjCtl->nCntItems %= GetObj_stage_00Type()->itemsSet.nItemsNum;
}

//========================================
// ItemPriceUpObj_pedestal_00関数 - OBJ:台座[00] の商品の値上げ処理 -
//========================================
void ItemPriceUpObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm)
{
	Obj_pedestal_00ItemControl	*pObjItmCtl	// OBJ:台座[00] の商品毎の管理情報
								= &g_aObj_pedestal_00ItemControl[itm];

	pObjItmCtl->nCntPriceUp++;	// 該当する商品の値上げカウントを加算
}

//========================================
// ItemPriceDownObj_pedestal_00関数 - OBJ:台座[00] の商品の値下げ処理 -
//========================================
void ItemPriceDownObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm)
{
	Obj_pedestal_00ItemControl	*pObjItmCtl	// OBJ:台座[00] の商品毎の管理情報
								= &g_aObj_pedestal_00ItemControl[itm];

	pObjItmCtl->nCntPriceUp--;	// 該当する商品の値上げカウントを減算
}