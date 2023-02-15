//========================================
// 
// ATK:弾[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** atk_bullet_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "atk_bullet_00.h"		// ATK:弾			[00]
#include "atk_sword_00.h"		// ATK:剣			[00]
#include "chr_enemy_00.h"		// CHR:敵			[00]
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "eff_explosion_00.h"	// EFF:爆発			[00]
#include "eff_light_00.h"		// EFF:光			[00]
#include "eff_particle_00.h"	// EFF:パーティクル	[00]
#include "eff_shadow_00.h"		// EFF:影			[00]
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
// ATK:弾[00] の種類毎の情報のデータファイルの相対パス
#define ATK_BULLET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATTACK\\ATK_BULLET_00_TYPE_DATA.txt"

// ATK:弾[00] の最大数
// ATK:弾[00] の種類の最大数
#define ATK_BULLET_00_MAX		(128)
#define ATK_BULLET_00_TYPE_MAX	(8)

// ATK:弾[00] の法線ベクトル
// ATK:弾[00] の相対位置
#define ATK_BULLET_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define ATK_BULLET_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 4.0f, 0.0f))

// ATK:弾[00] の反射SE
#define ATK_BULLET_00_REFLECTION_SE	(SOUND_LABEL_SE_METAL_001)

//****************************************
// 列挙型の定義
//****************************************
// ATK:弾[00] の衝突オブジェクト番号
typedef enum 
{
	ATK_BULLET_00_HITOBJ_ATK_SWORD_00,		// ATK:剣			[00]
	ATK_BULLET_00_HITOBJ_CHR_ENEMY_00,		// CHR:敵			[00]
	ATK_BULLET_00_HITOBJ_CHR_PLAYER_00,		// CHR:プレイヤー	[00]
	ATK_BULLET_00_HITOBJ_OBJ_CORE_00,		// OBJ:コア			[00]
	ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:ミラー		[00]
	ATK_BULLET_00_HITOBJ_OBJ_STAGE_00,		// OBJ:ステージ		[00]
	ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:ブロック		[00]
	ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:放電装置		[00]
	ATK_BULLET_00_HITOBJ_OBJ_TURRET_00,		// OBJ:タレット		[00]
	ATK_BULLET_00_HITOBJ_MAX,
}ATK_BULLET_00_HITOBJ;

// ATK:弾[00] の属性番号
typedef enum
{
	ATK_BULLET_00_ELEM_NONE,		// 無し
	ATK_BULLET_00_ELEM_PARALYSIS,	// 麻痺
	ATK_BULLET_00_ELEM_MAX,
}ATK_BULLET_00_ELEM;

//****************************************
// 構造体の定義
//****************************************
// ATK:弾[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 現在の位置
	D3DXVECTOR3 posOld;	// 過去の位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 向き
	
	// 運動関連
	HitTestInfo	hitTestInfo;	// 当たり判定の管理情報
	Collision	cmnCollision; 	// 共通の衝突情報
	Collision	aObjCollision	// オブジェクト毎の衝突情報
				[ATK_BULLET_00_HITOBJ_MAX];

	// 分類関連
	int							nType;		// 種類
	ATK_BULLET_00_PARENTTYPE	parentType;	// 親の種類

	// ポリゴン関連
	int nPtn;			// パターンNo.
	int nCounterAnim;	// アニメーションカウンター

	// 状態関連
	int						nLife;			// 寿命
	bool					bUse;			// 使用されているかフラグ
	int						nDamage;		// ダメージ
	int						nAttackIndex;	// 攻撃するオブジェクトの番号
	ATK_BULLET_00_HITOBJ	attackObj;		// 攻撃するオブジェクトの分類
}Atk_bullet_00;

// ATK:弾[00] の種類毎の情報構造体
typedef struct
{
	int					nLife;			// 寿命
	float				fMoveForce;		// 移動力
	int					nDamage;		// ダメージ
	ATK_BULLET_00_ELEM	elem;			// 属性
	float				fWidth;			// 幅
	float				fHeight;		// 高さ
	char				aTexturePath	// テクスチャの相対パス
						[TXT_MAX];
	int					nPtnWidth;		// パターン幅
	int					nPtnHeight;		// パターン高さ
	int					nPtnMax;		// パターン上限
	int					nAnimTime;		// アニメーションにかかる時間
	float				fShadowRadius;	// 影の半径
	int					nLightType;		// 光の種類
	int					nExplosionType;	// 爆発の種類
	int					nParticleType;	// パーティクルの種類
	SOUND_LABEL			setSE;			// 設定SE
	HitTestSet			hitTestSet;		// 当たり判定設定情報
}Atk_bullet_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// ATK:弾[00] のパラメーターの初期化処理
void InitParameterAtk_bullet_00(Atk_bullet_00 *pAtk);

// ATK:弾[00] の破壊処理
void DestroyAtk_bullet_00(Atk_bullet_00 *pAtk);

// ATK:弾[00] の位置更新処理
// ATK:弾[00] の衝突チェック処理
// ATK:弾[00] の衝突情報に応じた処理
void UpdatePosAtk_bullet_00(Atk_bullet_00 *pAtk);
void CheckCollisionAtk_bullet_00(Atk_bullet_00 *pAtk, VECTOR vector);
void CollisionProcessAtk_bullet_00(Atk_bullet_00 *pAtk);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureAtk_bullet_00		// ATK:弾[00] のテクスチャへのポインタ
						[ATK_BULLET_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_bullet_00		// ATK:弾[00] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldAtk_bullet_00;	// ATK:弾[00] のワールドマトリックス
Atk_bullet_00			g_aAtk_bullet_00			// ATK:弾[00] の情報
						[ATK_BULLET_00_MAX];
Atk_bullet_00Type		g_aAtk_bullet_00Type		// ATK:弾[00] の種類毎の情報
						[ATK_BULLET_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterAtk_bullet_00関数 - ATK:弾[00] のパラメーターの初期化処理 -
//========================================
void InitParameterAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	pAtk->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pAtk->posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 過去の位置
	pAtk->move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	pAtk->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pAtk->hitTestInfo	= {};								// 当たり判定の管理情報
	pAtk->cmnCollision	= {};								// 共通の衝突情報
	for (int nCntHitObj = 0; nCntHitObj < ATK_BULLET_00_HITOBJ_MAX; nCntHitObj++) 
	{
		pAtk->aObjCollision[nCntHitObj] = {};				// オブジェクト毎の衝突情報
	}
	pAtk->nType			= 0;								// 種類
	pAtk->parentType	= (ATK_BULLET_00_PARENTTYPE)0;		// 親の種類
	pAtk->nPtn			= 0;								// パターンNo.
	pAtk->nCounterAnim	= 0;								// アニメーションカウンター
	pAtk->bUse			= false;							// 使用されているかフラグ
	pAtk->nDamage		= 0;								// ダメージ
	pAtk->nAttackIndex	= 0;								// 攻撃するオブジェクトの番号
	pAtk->attackObj		= (ATK_BULLET_00_HITOBJ)0;			// 攻撃するオブジェクトの分類
}

//========================================
// DestroyAtk_bullet_00関数 - ATK:弾[00] の破壊処理 -
//========================================
void DestroyAtk_bullet_00(Atk_bullet_00 *pAtk) 
{
	Atk_bullet_00Type	*pType	// ATK:弾[00] の種類毎の情報
						= &g_aAtk_bullet_00Type[pAtk->nType];

	pAtk->bUse = false;	// 使用されていない状態にする

	// EFF:爆発[00] の設定処理
	SetEff_explosion_00(pAtk->pos, g_aAtk_bullet_00Type[pAtk->nType].nExplosionType);
}

//========================================
// UpdatePosAtk_bullet_00関数 - ATK:弾[00] の位置更新処理 -
//========================================
void UpdatePosAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	Atk_bullet_00Type	*pType	// ATK:弾[00] の種類毎の情報
						= &g_aAtk_bullet_00Type[pAtk->nType];

	// 共通の衝突情報を初期化
	pAtk->cmnCollision = {};

	// オブジェクト毎の衝突情報を初期化
	for (int nCntObj = 0; nCntObj < ATK_BULLET_00_HITOBJ_MAX; nCntObj++)
	{
		pAtk->aObjCollision[nCntObj] = {};
	}

	pAtk->nAttackIndex = -1;	// 攻撃するオブジェクトの番号を初期化

	pAtk->pos.x += pAtk->move.x;					// X座標に移動量を適用
	pAtk->move.x = 0;								// X方向の移動量を初期化
	CheckCollisionAtk_bullet_00(pAtk, VECTOR_X);	// X方向の衝突チェック処理
	pAtk->pos.z += pAtk->move.z;					// Z座標に移動量を適用
	pAtk->move.z = 0;								// Z方向の移動量を初期化
	CheckCollisionAtk_bullet_00(pAtk, VECTOR_Z);	// Z方向の衝突チェック処理

	// ATK:弾[00] の衝突情報に応じた処理
	CollisionProcessAtk_bullet_00(pAtk);

	SetEff_shadow_00(pAtk->pos, pType->fShadowRadius);	// EFF:影[00] の設定処理
	SetEff_light_00(pAtk->pos, pType->nLightType);		// EFF:光[00] の設定処理
}

//========================================
// UpdatePosAtk_bullet_00関数 - ATK:弾[00] の衝突チェック処理 -
//========================================
void CheckCollisionAtk_bullet_00(Atk_bullet_00 *pAtk, VECTOR vector)
{
	Atk_bullet_00Type	*pType	// ATK:弾[00] の種類毎の情報
						= &g_aAtk_bullet_00Type[pAtk->nType];

	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&pAtk->pos,
		pAtk->posOld,
		NULL,
		&pAtk->rot,
		pAtk->rot,
		g_aAtk_bullet_00Type[pAtk->nType].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
	};

	// 衝突判定(共通)
	/*/ OBJ:剣			[00] /*/CollisionAtk_sword_00	(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_ATK_SWORD_00]	, &pAtk->cmnCollision, myCollInfo);
	/*/ OBJ:ステージ	[00] /*/CollisionObj_stage_00	(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_STAGE_00]	, &pAtk->cmnCollision, myCollInfo);

	// 衝突判定(非共通)
	switch (pAtk->parentType)
	{
		//========== *** プレイヤー ***
	case ATK_BULLET_00_PARENTTYPE_PLAYER:
	{
		int nIndex;	// 番号

		nIndex = CollisionChr_enemy_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_CHR_ENEMY_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// CHR:敵	[00] と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_BULLET_00_HITOBJ_CHR_ENEMY_00;
			break;
		}
	}
		break;
		//========== *** 敵 ***
	case ATK_BULLET_00_PARENTTYPE_ENEMY:
	{
		/*/ OBJ:ミラー	[00] /*/CollisionObj_mirror_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00], &pAtk->cmnCollision, myCollInfo);

		int nIndex;	// 番号

		nIndex = /*/ CHR:プレイヤー	[00] /*/CollisionChr_player_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_CHR_PLAYER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_BULLET_00_HITOBJ_CHR_PLAYER_00;
			break;
		}
		nIndex = /*/ OBJ:ブロック	[00] /*/CollisionObj_block_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00;
			break;
		}
		nIndex = /*/ OBJ:タレット	[00] /*/CollisionObj_turret_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_TURRET_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_BULLET_00_HITOBJ_OBJ_TURRET_00;
		}
		nIndex = /*/ OBJ:放電装置	[00] /*/CollisionObj_discharger_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00;
		}
		nIndex = /*/  OBJ:コア		[00] /*/CollisionObj_core_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_CORE_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// と接触した時、
			pAtk->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
			pAtk->attackObj					// 攻撃するオブジェクトの分類を設定
				= ATK_BULLET_00_HITOBJ_OBJ_CORE_00;
			break;
		}
	}
		break;
	}
}

//========================================
// CollisionProcessAtk_bullet_00関数 - ATK:弾[00] の衝突情報に応じた処理 -
//========================================
void CollisionProcessAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	Atk_bullet_00Type	*pType		// ATK:弾[00] の種類毎の情報
						= &g_aAtk_bullet_00Type[pAtk->nType];
	bool				bDestroy	// 破壊フラグ
						= true;

	if (pAtk->nAttackIndex != -1) 
	{// 攻撃するオブジェクトの番号が-1で無い時、
		// 攻撃するオブジェクトの分類に応じたダメージ処理
		switch (pAtk->attackObj)
		{
			case /*/ CHR:敵			[00] /*/ATK_BULLET_00_HITOBJ_CHR_ENEMY_00:
				DamageChr_enemy_00(pAtk->nAttackIndex, pAtk->nDamage);
				if (pType->elem == ATK_BULLET_00_ELEM_PARALYSIS) 
				{// 属性が麻痺の時、麻痺を付与
					GiveStateChr_enemy_00(pAtk->nAttackIndex, CHR_ENEMY_00_STATE_PARALYSIS);
				}
				
				break;
			case /*/ CHR:プレイヤー	[00] /*/ATK_BULLET_00_HITOBJ_CHR_PLAYER_00:
				DamageChr_player_00(pAtk->nDamage);
				break;
			case /*/ OBJ:ブロック	[00] /*/ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00:
				DamageObj_block_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:タレット	[00] /*/ATK_BULLET_00_HITOBJ_OBJ_TURRET_00:
				DamageObj_turret_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:放電装置	[00] /*/ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00:
				DamageObj_discharger_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:コア		[00] /*/ATK_BULLET_00_HITOBJ_OBJ_CORE_00:
				DamageObj_core_00(pAtk->nDamage);
				break;
			default /*/ 該当なし /*/:
				bDestroy = false;	// 破壊フラグを偽にする
				break;
		}
	}
	else
	{// 攻撃するオブジェクトの番号が-1の時、
		bDestroy = (pAtk->cmnCollision.bHit | pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_ATK_SWORD_00].bInside) * (!pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00].bHit);
	}

	if (pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00].bHit)
	{// OBJ:ミラー[00] との当たりフラグが真の時、
		PlaySound(ATK_BULLET_00_REFLECTION_SE);	// 反射SEを再生
		pAtk->parentType = ATK_BULLET_00_PARENTTYPE_PLAYER;	// 親の種類をプレイヤーに設定
	}

	if (bDestroy)
	{// 破壊フラグが真の時、
		SetEff_particle_00(pAtk->pos, pType->nParticleType);	// EFF:パーティクル[00] の設定処理
		DestroyAtk_bullet_00(pAtk);								// 破壊処理
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadAtk_bullet_00関数 - ATK:弾[00] の読み込み処理 -
//========================================
void LoadAtk_bullet_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Atk_bullet_00Type	*pAtkType	// ATK:弾[00] の種類毎の情報
						= g_aAtk_bullet_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(ATK_BULLET_00_TYPE_DATA_FILE_PATH, "r");

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
			// ATK:弾[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))			{ pAtkType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "LIFE:"))				{ fscanf(pFile, "%d", &pAtkType->nLife); }			// 寿命
				else if (!strcmp(aDataSearch, "MOVE_FORCE:"))		{ fscanf(pFile, "%f", &pAtkType->fMoveForce); }		// 移動力
				else if (!strcmp(aDataSearch, "DAMAGE:"))			{ fscanf(pFile, "%d", &pAtkType->nDamage); }		// ダメージ
				else if (!strcmp(aDataSearch, "ELEM:"))				{ fscanf(pFile, "%d", &pAtkType->elem); }			// 属性
				else if (!strcmp(aDataSearch, "WIDTH:"))			{ fscanf(pFile, "%f", &pAtkType->fWidth); }			// 幅
				else if (!strcmp(aDataSearch, "HEIGHT:"))			{ fscanf(pFile, "%f", &pAtkType->fHeight); }		// 高さ
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))		{ fscanf(pFile, "%s", &pAtkType->aTexturePath); }	// テクスチャの相対パス
				else if (!strcmp(aDataSearch, "PTN_WIDTH:"))		{ fscanf(pFile, "%d", &pAtkType->nPtnWidth); }		// パターン幅
				else if (!strcmp(aDataSearch, "PTN_HEIGHT:"))		{ fscanf(pFile, "%d", &pAtkType->nPtnHeight); }		// パターン高さ
				else if (!strcmp(aDataSearch, "PTN_MAX:"))			{ fscanf(pFile, "%d", &pAtkType->nPtnMax); }		// パターン上限
				else if (!strcmp(aDataSearch, "ANIM_TIME:"))		{ fscanf(pFile, "%d", &pAtkType->nAnimTime); }		// アニメーションにかかる時間
				else if (!strcmp(aDataSearch, "SHADOW_RADIUS:"))	{ fscanf(pFile, "%f", &pAtkType->fShadowRadius); }	// 影の半径
				else if (!strcmp(aDataSearch, "EXPLOSION_TYPE:"))	{ fscanf(pFile, "%d", &pAtkType->nExplosionType); }	// 爆発の種類
				else if (!strcmp(aDataSearch, "LIGHT_TYPE:"))		{ fscanf(pFile, "%d", &pAtkType->nLightType); }		// 光の種類
				else if (!strcmp(aDataSearch, "PARTICLE_TYPE:"))	{ fscanf(pFile, "%d", &pAtkType->nParticleType); }	// パーティクルの種類
				else if (!strcmp(aDataSearch, "SET_SE:"))			{ fscanf(pFile, "%d", &pAtkType->setSE); }			// 設定SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pAtkType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitAtk_bullet_00関数 - ATK:弾[00] の初期化処理 -
//========================================
void InitAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	for (int nCntType = 0; nCntType < ATK_BULLET_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aAtk_bullet_00Type[nCntType].aTexturePath, &g_aTextureAtk_bullet_00[nCntType]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * ATK_BULLET_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_bullet_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// ATK:弾[00] の情報のポインタ
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++, pVtx += 4)
	{
		// パラメーターの初期化処理
		InitParameterAtk_bullet_00(&g_aAtk_bullet_00[nCntAtk]);

		// 法線ベクトルの設定
		SetNormalLine3D(pVtx, ATK_BULLET_00_NOR);

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx, 0, g_aAtk_bullet_00Type[pAtk->nType].nPtnWidth, g_aAtk_bullet_00Type[pAtk->nType].nPtnHeight, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// UninitAtk_bullet_00関数 - ATK:弾[00] の終了処理 -
//========================================
void UninitAtk_bullet_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < ATK_BULLET_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureAtk_bullet_00[nCntType] != NULL)
		{
			g_aTextureAtk_bullet_00[nCntType]->Release();
			g_aTextureAtk_bullet_00[nCntType] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffAtk_bullet_00 != NULL)
	{
		g_pVtxBuffAtk_bullet_00->Release();
		g_pVtxBuffAtk_bullet_00 = NULL;
	}
}

//========================================
// UpdateAtk_bullet_00関数 - ATK:弾[00] の更新処理 -
//========================================
void UpdateAtk_bullet_00(void)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// ATK:弾[00] の情報のポインタ
					= g_aAtk_bullet_00;
	
	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++, pVtx += 4)
	{
		if (!pAtk->bUse)
		{// 使用されている状態でない時、
			// 処理を折り返す
			continue;
		}
		else if (--pAtk->nLife <= 0)
		{// 寿命を減算した結果0以下になった時、
			DestroyAtk_bullet_00(pAtk);	// 破壊処理
			continue;					// 処理を折り返す
		}

		// 現在の位置を過去の位置として保存
		pAtk->posOld = pAtk->pos;

		// 移動量を向きと移動力に応じて設定
		pAtk->move.x = sinf(pAtk->rot.y) * g_aAtk_bullet_00Type[pAtk->nType].fMoveForce;
		pAtk->move.z = cosf(pAtk->rot.y) * g_aAtk_bullet_00Type[pAtk->nType].fMoveForce;

		// 位置更新処理
		UpdatePosAtk_bullet_00(pAtk);

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx,
			Count(
				&pAtk->nPtn,
				0,
				g_aAtk_bullet_00Type[pAtk->nType].nPtnMax,
				&pAtk->nCounterAnim,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimTime,
				COUNT_TYPE_NORMAL),
			g_aAtk_bullet_00Type[pAtk->nType].nPtnWidth,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnHeight, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// DrawAtk_bullet_00関数 - ATK:弾[00] の描画処理 -
//========================================
void DrawAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// 計算用マトリックス
	D3DXMATRIX			mtxView;	// ビューマトリックス取得用
	Atk_bullet_00		*pAtk		// ATK:弾[00] の情報のポインタ
						= g_aAtk_bullet_00;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_bullet_00, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++,pAtk++)
	{
		if (!pAtk->bUse)
		{// 使用されている状態でない時、
			// 処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldAtk_bullet_00);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_mtxWorldAtk_bullet_00, NULL, &mtxView);
		g_mtxWorldAtk_bullet_00._41 = 0.0f;
		g_mtxWorldAtk_bullet_00._42 = 0.0f;
		g_mtxWorldAtk_bullet_00._43 = 0.0f;
		{
			D3DXVECTOR3 setPos = pAtk->pos + ATK_BULLET_00_RELATIVE_POS;	// 設定位置を設定

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldAtk_bullet_00, &g_mtxWorldAtk_bullet_00, &mtxTrans);
		}
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAtk_bullet_00);

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureAtk_bullet_00[pAtk->nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntAtk, 2);
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//========================================
// SetAtk_bullet_00関数 - ATK:弾[00] の設定処理 -
//========================================
void SetAtk_bullet_00(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nType, ATK_BULLET_00_PARENTTYPE parentType)
{
	if (GetMd_game_00()->state != MD_GAME_00_STATE_NORMAL) 
	{// MD:ゲーム画面[00] の状態が通常でない時、
		return;	// 処理を終了する
	}

	Atk_bullet_00	*pAtk	// ATK:弾[00] の情報のポインタ
					= g_aAtk_bullet_00;
	VERTEX_3D		*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++, pVtx += 4)
	{
		if (pAtk->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Atk_bullet_00Type	*pType	// ATK:弾[00] の種類毎の情報
							= &g_aAtk_bullet_00Type[nType];

		pAtk->pos = pos;				// 位置を代入
		pAtk->rot = rot;				// 向きを代入
		pAtk->nType = nType;			// 種類を代入
		pAtk->parentType = parentType;	// 親の種類を代入
		pAtk->nPtn = 0;					// パターンNo.を初期化
		pAtk->nCounterAnim = 0;			// アニメーションカウンターを初期化
		pAtk->nLife = pType->nLife;		// 寿命
		pAtk->bUse = true;				// 使用されている状態にする
		pAtk->nDamage = pType->nDamage;	// ダメージを設定

		if (parentType == ATK_BULLET_00_PARENTTYPE_PLAYER) 
		{// 親の種類がプレイヤーの時、ダメージに弾の攻撃力アップカウントを適用
			pAtk->nDamage *= (1.0f + (GetChr_player_00()->nBulletAtkUpCount * CHR_PLAYER_00_BULLET_ATK_ADDRATE));
		}
		else if (parentType == ATK_BULLET_00_PARENTTYPE_ENEMY) 
		{// 親の種類が敵の時、ダメージにウェーブカウントを適用
			pAtk->nDamage *= (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));
		}

		// 種類に応じた頂点座標の設定
		{
			float fWidth	= g_aAtk_bullet_00Type[nType].fWidth;	// 幅
			float fHeight	= g_aAtk_bullet_00Type[nType].fHeight;	// 高さ

			pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
		}

		PlaySound(pType->setSE);	// 設定SEを再生

		// 繰り返し処理を抜ける
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffAtk_bullet_00->Unlock();
}