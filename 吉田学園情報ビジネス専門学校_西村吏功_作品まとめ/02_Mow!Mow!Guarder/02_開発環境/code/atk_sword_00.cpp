//========================================
// 
// ATK:剣[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** atk_sword_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "atk_sword_00.h"	// ATK:剣			[00]
#include "chr_enemy_00.h"	// CHR:敵			[00]
#include "chr_player_00.h"	// CHR:プレイヤー	[00]
#include "eff_light_00.h"	// EFF:光			[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// ATK:剣[00] の種類毎の情報のデータファイルの相対パス
#define ATK_SWORD_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATTACK\\ATK_SWORD_00_TYPE_DATA.txt"

// ATK:剣[00] の種類の数
#define ATK_SWORD_00_TYPE_NUM	(1)

// TK:剣[00] の相対角度
#define ATK_SWORD_00_RELATIVE_ROT	(D3DXVECTOR3(0.0f,D3DX_PI,0.0f))

// ATK:剣[00] の相対位置
// ATK:剣[00] のプレイヤーからの距離
#define ATK_SWORD_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 2.0f, 0.0f))
#define ATK_SWORD_00_LENGTH_PLAYER	(10.0f)

//****************************************
// 列挙型の定義
//****************************************
// ATK:剣[00] の衝突オブジェクト番号
typedef enum
{
	ATK_SWORD_00_HITOBJ_CHR_ENEMY_00,	// CHR:敵	[00]
	ATK_SWORD_00_HITOBJ_MAX,
}ATK_SWORD_00_HITOBJ;

//****************************************
// 構造体の定義
//****************************************
// ATK:剣[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 現在の位置
	D3DXVECTOR3 posOld;	// 過去の位置
	D3DXVECTOR3 rot;	// 向き

	// 分類情報
	int nType;	// 種類

	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報
	Collision	cmnCollision; 	// 共通の衝突情報
	Collision	aObjCollision	// オブジェクト毎の衝突情報
				[ATK_SWORD_00_HITOBJ_MAX];

	// 状態関連
	int					nAttackIndex;	// 攻撃するオブジェクトの番号
	ATK_SWORD_00_HITOBJ	attackObj;		// 攻撃するオブジェクトの分類
}Atk_sword_00;

//****************************************
// プロトタイプ宣言
//****************************************
// ATK:剣[00] のパラメーターの初期化処理
void InitParameterAtk_sword_00(void);

// ATK:剣[00] の位置更新処理
// ATK:剣[00] の衝突チェック処理
// ATK:剣[00] の衝突情報に応じた処理
void UpdatePosAtk_sword_00(void);
void CheckCollisionAtk_sword_00(VECTOR vector);
void CollisionProcessAtk_sword_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureAtk_sword_00	// ATK:剣[00] のテクスチャへのポインタ
					[ATK_SWORD_00_TYPE_NUM]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshAtk_sword_00		// ATK:剣[00] のメッシュ(頂点情報)へのポインタ
					[ATK_SWORD_00_TYPE_NUM]
					= {};
LPD3DXBUFFER		g_pBuffMatAtk_sword_00	// ATK:剣[00] のマテリアルへのポインタ
					= NULL;
DWORD				g_aNumMatAtk_sword_00	// ATK:剣[00] のマテリアルの数
					[ATK_SWORD_00_TYPE_NUM]
					= {};
D3DXMATRIX			g_mtxWorldAtk_sword_00;	// ATK:剣[00] のワールドマトリックス
Atk_sword_00		g_atk_sword_00;			// ATK:剣[00] の情報
Atk_sword_00Type	g_aAtk_sword_00Type		// ATK:剣[00] の種類毎の情報
					[ATK_SWORD_00_TYPE_NUM];

//========== *** ATK:剣[00] の種類毎の情報を取得 ***
Atk_sword_00Type *GetAtk_sword_00Type(void) 
{
	return g_aAtk_sword_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterAtk_sword_00関数 - ATK:剣[00] のパラメーターの初期化処理 -
//========================================
void InitParameterAtk_sword_00(void) 
{
	Atk_sword_00	*pAtk	// ATK:剣[00] の情報のポインタ
					= &g_atk_sword_00;

	pAtk->pos			= INITD3DXVECTOR3;			// 現在の位置
	pAtk->posOld		= INITD3DXVECTOR3;			// 過去の位置
	pAtk->rot			= INITD3DXVECTOR3;			// 向き
	pAtk->nType			= 0;						// 種類
	pAtk->hitTestInfo	= {};						// 当たり判定の管理情報
	pAtk->cmnCollision	= {};						// 共通の衝突情報
	for (int nCntHitAtk = 0; nCntHitAtk < ATK_SWORD_00_HITOBJ_MAX; nCntHitAtk++) 
	{
		pAtk->aObjCollision[nCntHitAtk]	= {};		// オブジェクト毎の衝突情報
	}
	pAtk->nAttackIndex	= -1;						// 攻撃するオブジェクトの番号
	pAtk->attackObj		= (ATK_SWORD_00_HITOBJ)0;	// 攻撃するオブジェクトの分類
}

//========================================
// UpdatePosAtk_sword_00関数 - ATK:剣[00] の位置更新処理 -
//========================================
void UpdatePosAtk_sword_00(void)
{
	Atk_sword_00		*pAtk	// ATK:剣[00] の情報のポインタ
						= &g_atk_sword_00;
	Atk_sword_00Type	*pType	// ATK:剣[00] の種類毎の情報
						= &g_aAtk_sword_00Type[pAtk->nType];

	// 割合
	float fRate = ((float)GetChr_player_00()->nCounterAttack / (float)CHR_PLAYER_00_SLASH_TIME);

	pAtk->rot.y =
		GetChr_player_00()->rot.y - (CHR_PLAYER_00_SLASH_INSIDE * 0.5f)
		+ (CHR_PLAYER_00_SLASH_INSIDE * fRate);
	pAtk->pos = GetChr_player_00()->pos;
	pAtk->pos.x += sinf(pAtk->rot.y) * ATK_SWORD_00_LENGTH_PLAYER;
	pAtk->pos.z += cosf(pAtk->rot.y) * ATK_SWORD_00_LENGTH_PLAYER;

	// 共通の衝突情報を初期化
	pAtk->cmnCollision = {};

	// オブジェクト毎の衝突情報を初期化
	for (int nCntHitObj = 0; nCntHitObj < ATK_SWORD_00_HITOBJ_MAX; nCntHitObj++)
	{
		pAtk->aObjCollision[nCntHitObj] = {};
	}

	pAtk->nAttackIndex = -1;	// 攻撃するオブジェクトの番号を初期化

	CheckCollisionAtk_sword_00(VECTOR_X);	// X方向の衝突チェック処理
	CheckCollisionAtk_sword_00(VECTOR_Z);	// Z方向の衝突チェック処理

	// ATK:剣[00] の衝突情報に応じた処理
	CollisionProcessAtk_sword_00();

	SetEff_light_00(pAtk->pos,		// EFF:光[00] の設定処理
		pType->nLightType, fRate);
}

//========================================
// CheckCollisionAtk_sword_00関数 - ATK:剣[00] の衝突チェック処理 -
//========================================
void CheckCollisionAtk_sword_00(VECTOR vector)
{
	Atk_sword_00	*pAtk	// ATK:剣[00] の情報のポインタ
					= &g_atk_sword_00;
	
	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&pAtk->pos,
		pAtk->posOld,
		NULL,
		&pAtk->rot,
		pAtk->rot,
		g_aAtk_sword_00Type[0].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
	};

	// 衝突判定(攻撃対象)
	{
		int nIndex;	// 番号

		nIndex = CollisionChr_enemy_00(vector, &pAtk->aObjCollision[ATK_SWORD_00_HITOBJ_CHR_ENEMY_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// CHR:敵	[00] と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_SWORD_00_HITOBJ_CHR_ENEMY_00;
		}
	}
}

//========================================
// CollisionProcessAtk_sword_00関数 - ATK:剣[00] の衝突情報に応じた処理 -
//========================================
void CollisionProcessAtk_sword_00(void)
{
	Atk_sword_00		*pAtk	// ATK:剣[00] の情報のポインタ
						= &g_atk_sword_00;
	Atk_sword_00Type	*pType	// ATK:剣[00] の種類毎の情報
						= &g_aAtk_sword_00Type[pAtk->nType];

	if (pAtk->nAttackIndex != -1)
	{// 攻撃するオブジェクトの番号が-1で無い時、
		// 攻撃するオブジェクトの分類に応じたダメージ処理
		switch (pAtk->attackObj)
		{
  			case /*/ CHR:敵	[00] /*/ATK_SWORD_00_HITOBJ_CHR_ENEMY_00:
				DamageChr_enemy_00(pAtk->nAttackIndex, GetChr_player_00Type()[GetChr_player_00()->nType].nSlashDamage * (1.0f + (GetChr_player_00()->nSwordAtkUpCount * CHR_PLAYER_00_SWORD_ATK_ADDRATE)));
				KnockBackChr_enemy_00(pAtk->nAttackIndex, pAtk->rot,
					pType->fKnockBack * 
					((float)GetChr_player_00Type()[GetChr_player_00()->nType].nSlashDamage / 
					(float)GetChr_enemy_00Type()[GetChr_enemy_00()[pAtk->nAttackIndex].nType].nHP));
				break;
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadAtk_sword_00関数 - ATK:剣[00] の読み込み処理 -
//========================================
void LoadAtk_sword_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Atk_sword_00Type	*pAtkType	// ATK:剣[00] の種類毎の情報
						= g_aAtk_sword_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(ATK_SWORD_00_TYPE_DATA_FILE_PATH, "r");

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
			// ATK:剣[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pAtkType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "KNOCK_BACK:"))	{ fscanf(pFile, "%f", &pAtkType->fKnockBack); }		// ノックバック
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pAtkType->aModelPath); }		// モデルの相対パス
				else if (!strcmp(aDataSearch, "LIGHT_TYPE:"))	{ fscanf(pFile, "%d", &pAtkType->nLightType); }		// 光の種類
				else if (!strcmp(aDataSearch, "SLASH_SE:"))		{ fscanf(pFile, "%d", &pAtkType->slashSE); }		// 斬撃SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pAtkType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitAtk_sword_00関数 - ATK:剣[00] の初期化処理 -
//========================================
void InitAtk_sword_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < ATK_SWORD_00_TYPE_NUM; nCntType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			g_aAtk_sword_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatAtk_sword_00,
			NULL,
			&g_aNumMatAtk_sword_00[nCntType],
			&g_aMeshAtk_sword_00[nCntType]);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatAtk_sword_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatAtk_sword_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureAtk_sword_00[nCntType][nCntMat]);
			}
		}
	}

	// パラメーターの初期化処理
	InitParameterAtk_sword_00();
}

//========================================
// UninitAtk_sword_00関数 - ATK:剣[00] の終了処理 -
//========================================
void UninitAtk_sword_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < ATK_SWORD_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshAtk_sword_00[nCntType] != NULL)
		{
			g_aMeshAtk_sword_00[nCntType]->Release();
			g_aMeshAtk_sword_00[nCntType] = NULL;
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatAtk_sword_00 != NULL)
	{
		g_pBuffMatAtk_sword_00->Release();
		g_pBuffMatAtk_sword_00 = NULL;
	}
}

//========================================
// UpdateAtk_sword_00関数 - ATK:剣[00] の更新処理 -
//========================================
void UpdateAtk_sword_00(void)
{
	Atk_sword_00	*pAtk	// ATK:剣[00] の情報のポインタ
					= &g_atk_sword_00;

	if (!GetChr_player_00()->bSlash) 
	{// CHR:プレイヤー[00] の斬撃フラグが偽の時、
		return;	// 処理を終了する
	}

	UpdatePosAtk_sword_00();	// 位置更新処理
	pAtk->posOld = pAtk->pos;	// 過去の位置を保存
}

//========================================
// DrawAtk_sword_00関数 - ATK:剣[00] の描画処理 -
//========================================
void DrawAtk_sword_00(void)
{
	if (!GetChr_player_00()->bSlash)
	{// CHR:プレイヤー[00] の斬撃フラグが偽の時、
		return;	// 処理を終了する
	}

	Atk_sword_00		*pAtk				// ATK:剣[00] の情報のポインタ
						= &g_atk_sword_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldAtk_sword_00);
	
	// 向きを反映
	{
		D3DXVECTOR3 setRot = pAtk->rot + ATK_SWORD_00_RELATIVE_ROT;	// 設定向き

		D3DXMatrixRotationYawPitchRoll(&mtxRot, setRot.y, setRot.x, setRot.z);
		D3DXMatrixMultiply(&g_mtxWorldAtk_sword_00, &g_mtxWorldAtk_sword_00, &mtxRot);
	}
	// 位置を反映
	{
		D3DXVECTOR3 setPos = pAtk->pos + ATK_SWORD_00_RELATIVE_POS;	// 設定位置

		D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
		D3DXMatrixMultiply(&g_mtxWorldAtk_sword_00, &g_mtxWorldAtk_sword_00, &mtxTrans);
	}
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAtk_sword_00);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatAtk_sword_00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aNumMatAtk_sword_00[pAtk->nType]; nCntMat++) 
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureAtk_sword_00[pAtk->nType][nCntMat]);

		// ポリゴン(パーツ)の描画
		g_aMeshAtk_sword_00[pAtk->nType]->DrawSubset(nCntMat);
	}
}

//========================================
// CollisionAtk_sword_00関数 - ATK:剣[00] との当たり判定処理 -
//========================================
void CollisionAtk_sword_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool			bHitTemp	// 当たりフラグを保存
					= pObjCollision->bHit;
	Atk_sword_00	*pAtk		// ATK:剣[00] の情報のポインタ
					= &g_atk_sword_00;

	if (!GetChr_player_00()->bSlash)
	{// CHR:プレイヤー[00] の斬撃フラグが偽の時、
		return;	// 処理を終了する
	}

	// 衝突判定に必要な情報
	CollisionCheck(
		vector,
		pObjCollision,
		myCollInfo,
		{
			&pAtk->pos,
			pAtk->posOld,
			NULL,
			&pAtk->rot,
			pAtk->rot,
			g_aAtk_sword_00Type[pAtk->nType].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
		},
		COLLCHK_MODE_LOOKDOWN);

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pObjCollision);
}