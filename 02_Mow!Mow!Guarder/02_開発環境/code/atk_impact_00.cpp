//========================================
// 
// ATK:衝撃[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** atk_impact_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "atk_impact_00.h"		// ATK:衝撃			[00]
#include "chr_enemy_00.h"		// CHR:敵			[00]
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "md_game_00.h"			// MD :ゲーム		[00]
#include "obj_block_00.h"		// OBJ:ブロック		[00]
#include "obj_core_00.h"		// OBJ:コア			[00]
#include "obj_discharger_00.h"	// OBJ:放電装置		[00]
#include "obj_mirror_00.h"		// OBJ:ミラー		[00]
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include "obj_turret_00.h"		// OBJ:タレット		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// ATK:衝撃[00] の種類毎の情報のデータファイルの相対パス
#define ATK_IMPACT_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATTACK\\ATK_IMPACT_00_TYPE_DATA.txt"

// ATK:衝撃[00] の最大数
// ATK:衝撃[00] の種類の最大数
#define ATK_IMPACT_00_MAX		(128)
#define ATK_IMPACT_00_TYPE_MAX	(8)

// ATK:衝撃[00] の法線ベクトル
// ATK:衝撃[00] の相対位置
#define ATK_IMPACT_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define ATK_IMPACT_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 4.0f, 0.0f))

//****************************************
// 列挙型の定義
//****************************************
// ATK:衝撃[00] の衝突オブジェクト番号
typedef enum 
{
	ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00,		// CHR:敵			[00]
	ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00,		// CHR:プレイヤー	[00]
	ATK_IMPACT_00_HITOBJ_OBJ_CORE_00,		// OBJ:コア			[00]
	ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:ミラー		[00]
	ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:ブロック		[00]
	ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:放電装置		[00]
	ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00,		// OBJ:タレット		[00]
	ATK_IMPACT_00_HITOBJ_MAX,
}ATK_IMPACT_00_HITOBJ;

// ATK:衝撃[00] の属性番号
typedef enum
{
	ATK_IMPACT_00_ELEM_NONE,	// 無し
	ATK_IMPACT_00_ELEM_MAX,
}ATK_IMPACT_00_ELEM;

//****************************************
// 構造体の定義
//****************************************
// ATK:衝撃[00] の種類毎の情報構造体
typedef struct
{
	int					nLife;			// 寿命
	int					nDamage;		// ダメージ
	float				fKnockBack;		// ノックバック
	ATK_IMPACT_00_ELEM	elem;			// 属性
	SOUND_LABEL			setSE;			// 設定SE
	HitTestSet			hitTestSet;		// 当たり判定設定情報
}Atk_impact_00Type;

// ATK:衝撃[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き

	// 運動関連
	HitTestInfo	hitTestInfo;	// 当たり判定の管理情報
	Collision	cmnCollision; 	// 共通の衝突情報
	Collision	aObjCollision	// オブジェクト毎の衝突情報
				[ATK_IMPACT_00_HITOBJ_MAX];

	// 分類関連
	int							nType;		// 種類
	ATK_IMPACT_00_PARENTTYPE	parentType;	// 親の種類

	// 状態関連
	int						nLife;			// 寿命
	bool					bUse;			// 使用されているかフラグ
	int						nDamage;		// ダメージ
	int						nAttackIndex;	// 攻撃するオブジェクトの番号
	ATK_IMPACT_00_HITOBJ	attackObj;		// 攻撃するオブジェクトの分類
}Atk_impact_00;

//****************************************
// プロトタイプ宣言
//****************************************
// ATK:弾[00] のパラメーターの初期化処理
void InitParameterAtk_impact_00(Atk_impact_00 *pAtk);

// ATK:衝撃[00] の衝突チェック処理
// ATK:衝撃[00] の衝突情報に応じた処理
void CheckCollisionAtk_impact_00(Atk_impact_00 *pAtk, VECTOR vector);
void CollisionProcessAtk_impact_00(Atk_impact_00 *pAtk);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureAtk_impact_00		// ATK:衝撃[00] のテクスチャへのポインタ
						[ATK_IMPACT_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_impact_00		// ATK:衝撃[00] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldAtk_impact_00;	// ATK:衝撃[00] のワールドマトリックス
Atk_impact_00Type		g_aAtk_impact_00Type		// ATK:衝撃[00] の種類毎の情報
						[ATK_IMPACT_00_TYPE_MAX];
Atk_impact_00			g_aAtk_impact_00			// ATK:衝撃[00] の情報
						[ATK_IMPACT_00_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterAtk_impact_00関数 - ATK:弾[00] のパラメーターの初期化処理 -
//========================================
void InitParameterAtk_impact_00(Atk_impact_00 *pAtk)
{
	pAtk->pos			= INITD3DXVECTOR3;			// 位置
	pAtk->rot			= INITD3DXVECTOR3;			// 向き
	pAtk->hitTestInfo	= {};						// 当たり判定の管理情報
	pAtk->cmnCollision	= {};						// 共通の衝突情報
	for (int nCntHitObj = 0; nCntHitObj < ATK_IMPACT_00_HITOBJ_MAX; nCntHitObj++)
	{
		pAtk->aObjCollision[nCntHitObj] = {};		// オブジェクト毎の衝突情報
	}
	pAtk->nType			= 0;						// 種類
	pAtk->nLife			= 0;						// 寿命
	pAtk->bUse			= false;					// 使用されているかフラグ
	pAtk->nDamage		= 0;						// ダメージ
	pAtk->nAttackIndex	= 0;						// 攻撃するオブジェクトの番号
	pAtk->attackObj		= (ATK_IMPACT_00_HITOBJ)0;	// 攻撃するオブジェクトの分類
}

//========================================
// UpdatePosAtk_impact_00関数 - ATK:衝撃[00] の衝突チェック処理 -
//========================================
void CheckCollisionAtk_impact_00(Atk_impact_00 *pAtk, VECTOR vector)
{
	Atk_impact_00Type	*pType	// ATK:衝撃[00] の種類毎の情報
						= &g_aAtk_impact_00Type[pAtk->nType];

	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&pAtk->pos,
		pAtk->pos,
		NULL,
		&pAtk->rot,
		pAtk->rot,
		g_aAtk_impact_00Type[pAtk->nType].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
	};

	// 衝突判定(攻撃対象)
	switch (pAtk->parentType)
	{
		//========== *** プレイヤー ***
	case ATK_IMPACT_00_PARENTTYPE_PLAYER:
	{
		int nIndex;	// 番号

		nIndex = CollisionChr_enemy_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// CHR:敵	[00] と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00;
			break;
		}
	}
		break;
		//========== *** 敵 ***
	case ATK_IMPACT_00_PARENTTYPE_ENEMY:
	{
		int nIndex;	// 番号

		nIndex = /*/ CHR:プレイヤー	[00] /*/CollisionChr_player_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00;
			break;
		}
		nIndex = /*/ OBJ:ブロック	[00] /*/CollisionObj_block_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00;
			break;
		}
		nIndex = /*/ OBJ:タレット	[00] /*/CollisionObj_turret_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00;
		}
		nIndex = /*/ OBJ:放電装置	[00] /*/CollisionObj_discharger_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00;
		}
		nIndex = /*/  OBJ:コア		[00] /*/CollisionObj_core_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_CORE_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_IMPACT_00_HITOBJ_OBJ_CORE_00;
			break;
		}
		nIndex = /*/  OBJ:ミラー	[00] /*/CollisionObj_mirror_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00;
			break;
		}
	}
		break;
	}
}

//========================================
// CollisionProcessAtk_impact_00関数 - ATK:衝撃[00] の衝突情報に応じた処理 -
//========================================
void CollisionProcessAtk_impact_00(Atk_impact_00 *pAtk)
{
	Atk_impact_00Type	*pType	// ATK:衝撃[00] の種類毎の情報
						= &g_aAtk_impact_00Type[pAtk->nType];

	if (pAtk->nAttackIndex != -1) 
	{// 攻撃するオブジェクトの番号が-1で無い時、
		// 攻撃するオブジェクトの分類に応じたダメージ処理
		switch (pAtk->attackObj)
		{
			case /*/ CHR:敵			[00] /*/ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00:
				DamageChr_enemy_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ CHR:プレイヤー	[00] /*/ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00:
				DamageChr_player_00(pAtk->nDamage);
				KnockBackChr_player_00(
					D3DXVECTOR3(
						0.0f,
						FindAngleLookDown(pAtk->pos, GetChr_player_00()->pos),
						0.0f),
					pType->fKnockBack);
				break;
			case /*/ OBJ:ブロック	[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00:
				DamageObj_block_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:タレット	[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00:
				DamageObj_turret_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:放電装置	[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00:
				DamageObj_discharger_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:コア		[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_CORE_00:
				DamageObj_core_00(pAtk->nDamage);
				break;
			case /*/ OBJ:ミラー		[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00:
				DamageObj_mirror_00(pAtk->nAttackIndex, pAtk->nDamage);
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
// LoadAtk_impact_00関数 - ATK:衝撃[00] の読み込み処理 -
//========================================
void LoadAtk_impact_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Atk_impact_00Type	*pAtkType	// ATK:衝撃[00] の種類毎の情報
						= g_aAtk_impact_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(ATK_IMPACT_00_TYPE_DATA_FILE_PATH, "r");

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
			// ATK:衝撃[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pAtkType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "LIFE:"))			{ fscanf(pFile, "%d", &pAtkType->nLife); }			// 寿命
				else if (!strcmp(aDataSearch, "DAMAGE:"))		{ fscanf(pFile, "%d", &pAtkType->nDamage); }		// ダメージ
				else if (!strcmp(aDataSearch, "KNOCK_BACK:"))	{ fscanf(pFile, "%f", &pAtkType->fKnockBack); }		// ノックバック
				else if (!strcmp(aDataSearch, "ELEM:"))			{ fscanf(pFile, "%d", &pAtkType->elem); }			// 属性
				else if (!strcmp(aDataSearch, "SET_SE:"))		{ fscanf(pFile, "%d", &pAtkType->setSE); }			// 設定SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pAtkType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitAtk_impact_00関数 - ATK:衝撃[00] の初期化処理 -
//========================================
void InitAtk_impact_00(void)
{
	Atk_impact_00	*pAtk	// ATK:弾[00] の情報のポインタ
					= g_aAtk_impact_00;

	for (int nCntAtk = 0; nCntAtk < ATK_IMPACT_00_MAX; nCntAtk++, pAtk++)
	{
		// パラメーターの初期化処理
		InitParameterAtk_impact_00(&g_aAtk_impact_00[nCntAtk]);
	}
}

//========================================
// UninitAtk_impact_00関数 - ATK:衝撃[00] の終了処理 -
//========================================
void UninitAtk_impact_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < ATK_IMPACT_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureAtk_impact_00[nCntType] != NULL)
		{
			g_aTextureAtk_impact_00[nCntType]->Release();
			g_aTextureAtk_impact_00[nCntType] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffAtk_impact_00 != NULL)
	{
		g_pVtxBuffAtk_impact_00->Release();
		g_pVtxBuffAtk_impact_00 = NULL;
	}
}

//========================================
// UpdateAtk_impact_00関数 - ATK:衝撃[00] の更新処理 -
//========================================
void UpdateAtk_impact_00(void)
{
	Atk_impact_00	*pAtk	// ATK:弾[00] の情報のポインタ
					= g_aAtk_impact_00;

	for (int nCntAtk = 0; nCntAtk < ATK_IMPACT_00_MAX; nCntAtk++, pAtk++)
	{
		if (!pAtk->bUse)
		{// 使用されている状態でない時、
			// 処理を折り返す
			continue;
		}
		else if (--pAtk->nLife <= 0)
		{// 寿命を減算した結果0以下になった時、
			pAtk->bUse = false;	// 使用されていない状態にする
			continue;			// 処理を折り返す
		}

		// 共通の衝突情報を初期化
		pAtk->cmnCollision = {};

		// オブジェクト毎の衝突情報を初期化
		for (int nCntObj = 0; nCntObj < ATK_IMPACT_00_HITOBJ_MAX; nCntObj++)
		{
			pAtk->aObjCollision[nCntObj] = {};
		}

		pAtk->nAttackIndex = -1;	// 攻撃するオブジェクトの番号を初期化

		// ATK:衝撃[00] の衝突チェック
		CheckCollisionAtk_impact_00(pAtk, VECTOR_X);
		CheckCollisionAtk_impact_00(pAtk, VECTOR_Z);

		// ATK:衝撃[00] の衝突情報に応じた処理
		CollisionProcessAtk_impact_00(pAtk);
	}
}

//========================================
// SetAtk_impact_00関数 - ATK:衝撃[00] の設定処理 -
//========================================
void SetAtk_impact_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_IMPACT_00_PARENTTYPE parentType)
{
	if (GetMd_game_00()->state != MD_GAME_00_STATE_NORMAL)
	{// MD:ゲーム画面[00] の状態が通常でない時、
		return;	// 処理を終了する
	}

	Atk_impact_00	*pAtk	// ATK:弾[00] の情報のポインタ
					= g_aAtk_impact_00;

	for (int nCntAtk = 0; nCntAtk < ATK_IMPACT_00_MAX; nCntAtk++, pAtk++)
	{
		if (pAtk->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Atk_impact_00Type	*pType	// ATK:弾[00] の種類毎の情報
							= &g_aAtk_impact_00Type[nType];

		pAtk->pos = pos;				// 位置を代入
		pAtk->rot = rot;				// 向きを代入
		pAtk->nType = nType;			// 種類を代入
		pAtk->parentType = parentType;	// 親の種類を代入
		pAtk->nLife = pType->nLife;		// 寿命
		pAtk->bUse = true;				// 使用されている状態にする
		pAtk->nDamage = pType->nDamage;	// ダメージを設定

		if (parentType == ATK_IMPACT_00_PARENTTYPE_ENEMY)
		{// 親の種類が敵の時、ダメージにウェーブカウントを適用
			pAtk->nDamage *= (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));
		}

		// 設定SEを再生
		PlaySound(pType->setSE);

		// 繰り返し処理を抜ける
		break;
	}
}