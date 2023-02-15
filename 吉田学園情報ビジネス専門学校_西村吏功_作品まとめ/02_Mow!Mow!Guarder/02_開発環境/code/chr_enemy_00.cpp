//========================================
// 
// CHR:敵[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_enemy_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "md_game_00.h"			// MD :ゲーム画面	[00]
#include "atk_bullet_00.h"		// ATK:弾			[00]
#include "atk_impact_00.h"		// ATK:衝撃			[00]
#include "chr_enemy_00.h"		// CHR:敵			[00]
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "itm_coin_00.h"		// ITM:コイン		[00]
#include "eff_explosion_00.h"	// EFF:爆発			[00]
#include "eff_shadow_00.h"		// EFF:影			[00]
#include "eff_shock-wave_00.h"	// EFF:衝撃波		[00]
#include "obj_block_00.h"		// OBJ:ブロック		[00]
#include "obj_discharger_00.h"	// OBJ:放電装置		[00]
#include "obj_core_00.h"		// OBJ:コア			[00]
#include "obj_mirror_00.h"		// OBJ:ミラー		[00]
#include "obj_turret_00.h"		// OBJ:タレット		[00]
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include "ui_damage_00.h"		// UI :ダメージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// CHR:敵[00] の種類毎の情報のデータファイルの相対パス
#define CHR_ENEMY_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\CHR_ENEMY_00_TYPE_DATA.txt"

// CHR:敵[00] の種類の最大数
#define CHR_ENEMY_00_TYPE_MAX	(32)

// CHR:敵[00] の回転倍率
// CHR:敵[00] の力の減衰倍率
// CHR:敵[00] の移動量の減衰倍率
#define CHR_ENEMY_00_ROT_DIAMETER	(0.25f)
#define CHR_ENEMY_00_FORCE_DAMP		(0.9f)
#define CHR_ENEMY_00_MOVE_DAMP		(0.8f)

// CHR:敵[00] の衝突時のノックバック
#define CHR_ENEMY_00_COLLISION_KNOCK_BACK	(3.0f)

// CHR:敵[00] の移動カウンターの下限
// CHR:敵[00] の移動カウンターの上限
#define CHR_ENEMY_00_MOVE_COUNTER_MIN	(30)
#define CHR_ENEMY_00_MOVE_COUNTER_MAX	(120)

// CHR:敵[00] の無敵時間
#define CHR_ENEMY_00_INVINCIBLE_TIME	(5)

// CHR:敵[00] のダメージ時間
#define CHR_ENEMY_00_DAMAGE_TIME	(20)

// CHR:敵[00] の麻痺の時間
// CHR:敵[00] の麻痺の減少倍率
// CHR:敵[00] の麻痺SE
#define CHR_ENEMY_00_PARALYSIS_TIME				(180)
#define CHR_ENEMY_00_PARALYSIS_DECREMENT_RATE	(0.5f)
#define CHR_ENEMY_00_PARALYSIS_SE				(SOUND_LABEL_SE_ELEC_001)

// CHR:敵[00] のウェーブカウント毎のHPの上昇倍率
#define CHR_ENEMY_00_WAVE_HP_ADDRATE	(0.1f)

// CHR:敵[00] の1度の振動にかかる時間
// CHR:敵[00] の振動の減衰倍率
// CHR:敵[00] の振動の強さの下限
// CHR:敵[00] のダメージによる振動の倍率
#define CHR_ENEMY_00_ONCE_VIBRATION_TIME	(1)
#define CHR_ENEMY_00_VIBRATION_DAMPING		(0.5f)
#define CHR_ENEMY_00_VIBRATION_MIN			(0.01f)
#define CHR_ENEMY_00_DAMAGE_VIBRATION_RATE	(5.0f)

// CHR:敵[00] のボス設定時のカメラ(3D)にかかる振動
// CHR:敵[00] のボス設定時の効果音
#define CHR_ENEMY_00_BOSS_SET_CAMERA3D_VIBRATION	(10.0f)
#define CHR_ENEMY_00_BOSS_SET_SE					(SOUND_LABEL_SE_FOOTSTEP_002)

// CHR:敵[00] のアピールモーション番号
#define CHR_ENEMY_00_APPEAL_MOTION	(0)

//****************************************
// 構造体の定義
//****************************************
// CHR:敵[00] の管理情報構造体
typedef struct 
{
	CHR_ENEMY_00_CONTROL_STATE	state;			// 状態
	int							nSetBossIdx;	// 設定したボスの番号
}Chr_enemy_00Control;

//****************************************
// プロトタイプ宣言
//****************************************
// CHR:敵[00] の行動パターン設定情報の読み込み処理
void LoadActPtnSetChr_enemy_00(FILE *pFile, Chr_enemy_00ActPtnSet *pActPtnSet);

// CHR:敵[00] のパラメーターの初期化処理
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の設定時のパラメーターの初期化処理
void InitParameterSetChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の管理情報のパラメーターの初期化処理
void InitParameterChr_enemy_00Control(void);

// CHR:敵[00] の番号の検索処理
int SearchIndexChr_enemy_00(Chr_enemy_00 *pChr);

// CHR:敵[00] の行動読み込み処理
void ActLoadChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の行動処理
void ActChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の移動処理
void MoveChr_enemy_00(Chr_enemy_00 *pChr, float fMove);
// CHR:敵[00] の位置更新処理
void UpdatePosChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の衝突チェック処理
void CheckCollisionChr_enemy_00(Chr_enemy_00 *pChr, VECTOR vector);
// CHR:敵[00] の衝突情報に応じた処理
void CollisionProcessChr_enemy_00(Chr_enemy_00 *pChr);

// CHR:敵[00] の破壊処理
void DestroyChr_enemy_00(Chr_enemy_00 *pChr);

// CHR:敵[00] の振動を設定
void SetChr_enemy_00Vibration(Chr_enemy_00 *pChr, float fVibration);

// CHR:敵[00] の子設定処理
void SetChildChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nParentIdx);
// CHR:敵[00] の子の移動処理
void ChildMoveChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の子のダメージカウンター設定処理
void ChildSetDamageCounterChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の子のアピールモーション設定処理
void ShildAppealMotionSetChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:敵[00] の子の破壊処理
void ChildDestoryChr_enemy_00(Chr_enemy_00 *pChr);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureChr_enemy_00	// CHR:敵[00] のテクスチャへのポインタ
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshChr_enemy_00		// CHR:敵[00] のメッシュ(頂点情報)へのポインタ
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					= {};
LPD3DXBUFFER		g_aBuffMatChr_enemy_00	// CHR:敵[00] のマテリアルへのポインタ
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					= {};
DWORD				g_aNumMatChr_enemy_00	// CHR:敵[00] のマテリアルの数
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					= {};
D3DXMATRIX			g_aMtxWorldChr_enemy_00	// CHR:敵[00] のワールドマトリックス
					[PARTS_3D_MAX];
Chr_enemy_00		g_aChr_enemy_00			// CHR:敵[00] の情報
					[CHR_ENEMY_00_MAX];
Chr_enemy_00Control	g_chr_enemy_00Control;	// CHR:敵[00] の管理情報
Chr_enemy_00Type	g_aChr_enemy_00Type		// CHR:敵[00] の種類毎の情報
					[CHR_ENEMY_00_TYPE_MAX];
int					g_nChr_enemy_00TypeNum;	// CHR:敵[00] の種類数

// CHR:敵[00] の行動の種類毎の情報構造体
const Chr_enemy_00ActType g_aChr_enemy_00ActType[CHR_ENEMY_00_COMMAND_MAX] =
{
	{ "TORTUOUS_MOVE_START"    ,0  },	// 曲折式移動開始
	{ "TORTUOUS_MOVE_END"      ,0  },	// 曲折式移動終了
	{ "TURN_MOVE_START"        ,2  },	// 方向転換式移動開始
	{ "TURN_MOVE_END"          ,0  },	// 方向転換式移動終了
	{ "FORCE_MOVE_START"       ,1  },	// 推進式移動開始
	{ "FORCE_MOVE_END"         ,0  },	// 推進式移動終了
	{ "TURN_TO_TURN_POS_START" ,0  },	// 振り向き位置に振り向き開始
	{ "TURN_TO_TURN_POS_END"   ,0  },	// 振り向き位置に振り向き終了
	{ "TURN_TO_MOVE_ROT_START" ,0  },	// 移動向きに振り向き開始
	{ "TURN_TO_MOVE_ROT_END"   ,0  },	// 移動向きに振り向き終了
	{ "Z_LOCK"                 ,0  },	// Z座標をロック
	{ "Z_UNLOCK"               ,0  },	// Z座標をアンロック
	{ "CHILD_SET"              ,7  },	// 子設定
	{ "CHILD_DEPENDENCY"       ,0  },	// 子依存
	{ "SPIN_START"             ,3  },	// 回転開始
	{ "SPIN_END"               ,0  },	// 回転終了
	{ "SHOT"                   ,7  },	// 弾発射
	{ "PLAY_SOUND"             ,1  },	// サウンド再生
	{ "MOTION_SET"             ,1  },	// モーション設定
	{ "AUTO_SHOT_START"        ,3  },	// 自動ショット開始
	{ "AUTO_SHOT_END"          ,0  },	// 自動ショット終了
	{ "UNIT_IGNORED_START"     ,0  },	// ユニット無視開始
	{ "UNIT_IGNORED_END"       ,0  },	// ユニット無視終了
	{ "MOVE_START"             ,3  },	// 移動開始
	{ "MOVE_END"               ,0  },	// 移動終了
	{ "ALLY_SET"               ,7  },	// 味方設定
	{ "ATK_IMPACT"             ,7  },	// ATK:衝撃
	{ "EFF_SHOCKWAVE_00"       ,18 },	// EFF:衝撃波[00]
	{ "COLLISION_DAMAGE_CHANGE",1  },	// 衝突ダメージを変更
	{ "COLLISION_DAMAGE_UNDO"  ,0  },	// 衝突ダメージを元に戻す
	{ "BOSS_DEPENDENCY"        ,0  },	// ボス依存
	{ "ALLY_SET_ABSOLUTE"      ,7  },	// 味方設定(絶対位置)
	{ "RECOGNITION_OFF"        ,0  },	// 認識OFF
	{ "TELEPORTATION"          ,3  },	// 瞬間移動
};

//========== *** CHR:敵[00] の情報を取得 ***
Chr_enemy_00 *GetChr_enemy_00(void) 
{
	return g_aChr_enemy_00;
}

//========== *** CHR:敵[00] の種類毎の情報を取得 ***
Chr_enemy_00Type *GetChr_enemy_00Type(void) 
{
	return g_aChr_enemy_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadActPtnSetChr_enemy_00関数 - CHR:敵[00] の行動パターン設定情報の読み込み処理 -
//========================================
void LoadActPtnSetChr_enemy_00(FILE *pFile, Chr_enemy_00ActPtnSet *pActPtnSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];
	int		nCntActPtn	// 行動パターンのカウント
			= 0;

	// 行動パターン設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "ACTPTNSET_END")) { break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "ACTPTN"))		{
			Chr_enemy_00ActPtn	*pActPtn	// 行動パターン情報のポインタ
								= &pActPtnSet->aActPtn[nCntActPtn];

			pActPtn->nActNum = 0;	// 行動数を初期化

			// 行動パターン情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "ACTPTN_END"))	{ nCntActPtn++; break; }						// 読み込みを終了
				else if (!strcmp(aDataSearch, "LOOP_TIME:"))	{ fscanf(pFile, "%d", &pActPtn->nLoopTime); }	// ループ時間
				else if (!strcmp(aDataSearch, "ACT"))			{
					Chr_enemy_00Act	*pAct	// 行動情報のポインタ
									= pActPtn->aAct;

					// 行動情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if		(!strcmp(aDataSearch, "ACT_END"))	{ break; }
						else if (!strcmp(aDataSearch, "TIME"))		{
							fscanf(pFile, "%d", &pAct->nTime);	// 行動する時間
							fscanf(pFile, "%s", aDataSearch);	// 検索

							for (int nCntActType = 0; nCntActType < CHR_ENEMY_00_COMMAND_MAX; nCntActType++) 
							{// 行動の種類を検索
								if (strcmp(aDataSearch, g_aChr_enemy_00ActType[nCntActType].aLabelName)) 
								{// カウントの行動の種類のラベル名と検索が一致しなかった時、
									continue;	// 繰り返し処理を折り返す
								}
								
								pAct->cmd = (CHR_ENEMY_00_COMMAND)nCntActType;	// 行動の種類を代入

								for (int nCntData = 0; nCntData < g_aChr_enemy_00ActType[nCntActType].nDataNum; nCntData++) 
								{// 値を必要数だけ読み込み
									fscanf(pFile, "%f", &pAct->aData[nCntData]);
								}

								break;
							}

							pAct++;				// 行動情報のポインタを進める
							pActPtn->nActNum++;	// 行動数を加算
						}
					}
				}
			}
		}
	}
}

//========================================
// InitParameterChr_enemy_00関数 - CHR:敵[00] のパラメーターの初期化処理 -
//========================================
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr)
{
	pChr->pos					= INITD3DXVECTOR3;				// 現在の位置
	pChr->posOld				= INITD3DXVECTOR3;				// 過去の位置
	pChr->force					= INITD3DXVECTOR3;				// 力
	pChr->move					= INITD3DXVECTOR3;				// 移動量
	pChr->rot					= INITD3DXVECTOR3;				// 現在の向き
	pChr->rotOld				= INITD3DXVECTOR3;				// 過去の向き
	pChr->moveRot				= INITD3DXVECTOR3;				// 移動向き
	pChr->pTurnPos				= NULL;							// 振り向き先の位置のポインタ
	pChr->nType					= 0;							// 種類
	pChr->bUse					= false;						// 使用されているフラグ
	pChr->nHP					=								// HP
	pChr->nHPMax				= 0;							// HPの上限
	pChr->state					= CHR_ENEMY_00_STATE_NORMAL;	// 状態
	pChr->nCounterState			= 0;							// 状態カウンター
	pChr->nCounterShot			= 0;							// ショットカウンター
	pChr->nCounterCollAtk		= 0;							// 衝突攻撃カウンター
	pChr->nCounterInvincible	= 0;							// 無敵カウンター
	pChr->nCounterDamage		= 0;							// ダメージカウンター
	pChr->nAttackIndex			= -1;							// 攻撃するオブジェクトの番号
	pChr->attackObj				= (CHR_ENEMY_00_HITOBJ)0;		// 攻撃するオブジェクトの分類
	pChr->fVibration			= 0;							// 振動の強さ
	pChr->nCounterVib			= 0;							// 振動カウンター
	pChr->vibrationPos			= INITD3DXVECTOR3;				// 振動位置
	pChr->nParentIdx			= -1;							// 親の番号
	pChr->bAutoShot				= false;						// 自動ショットフラグ
	pChr->nBulletType			= 0;							// 弾の種類
	pChr->nShotTimeMin			= 0;							// 発射間隔の下限
	pChr->nShotTimeMax			= 0;							// 発射間隔の上限
	pChr->bUnitIgnored			= false;						// ユニット無視フラグ
	pChr->bCollDamageChange		= false;						// 衝突ダメージ変更フラグ
	pChr->nNewCollDamage		= 0;							// 新規衝突ダメージ
	pChr->bBossDependency		= false;						// ボス依存フラグ
	pChr->bRecognitionOff		= false;						// 認識OFFフラグ
	pChr->hitTestInfo			= {};							// 当たり判定の管理情報
	pChr->cmnCollision			= {};							// 共通の衝突情報
	for (int nCntHitObj			= 0; nCntHitObj < CHR_ENEMY_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};					// オブジェクト毎の衝突情報
	}
	pChr->actInfo				= {};							// 行動管理情報
	pChr->partsInfo				= {};							// 部品管理
	pChr->nMotion				= 0;							// モーション
	pChr->nMotionTemp			= 0;							// モーション一時保存
}

//========================================
// InitParameterSetChr_enemy_00関数 - CHR:敵[00] の設定時のパラメーターの初期化処理 -
//========================================
void InitParameterSetChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[pChr->nType];

	pChr->force					= INITD3DXVECTOR3;				// 力
	pChr->move					= INITD3DXVECTOR3;				// 移動量
	pChr->pTurnPos				= NULL;							// 振り向き先の位置のポインタ
	pChr->bUse					= true;							// 使用されているフラグ
	pChr->state					= CHR_ENEMY_00_STATE_NORMAL;	// 状態
	pChr->nCounterState			= 0;							// 状態カウンター
	pChr->nCounterShot			= 0;							// ショットカウンター
	pChr->nCounterCollAtk		= 0;							// 衝突攻撃カウンター
	pChr->nCounterInvincible	= 0;							// 無敵カウンター
	pChr->nCounterDamage		= 0;							// ダメージカウンター
	pChr->fVibration			= 0;							// 振動の強さ
	pChr->nCounterVib			= 0;							// 振動カウンター
	pChr->vibrationPos			= INITD3DXVECTOR3;				// 振動位置
	pChr->bAutoShot				= false;						// 自動ショットフラグ
	pChr->bUnitIgnored			= false;						// ユニット無視フラグ
	pChr->bCollDamageChange		= false;						// 衝突ダメージ変更フラグ
	pChr->nNewCollDamage		= 0;							// 新規衝突ダメージ
	pChr->bBossDependency		= false;						// ボス依存フラグ
	pChr->bRecognitionOff		= false;						// 認識OFFフラグ
	pChr->actInfo				= {};							// 行動管理情報
	pChr->partsInfo				= {};							// 部品管理
	pChr->nMotion				= 0;							// モーション

	pChr->nHPMax =				// HPの上限を設定
		pType->nHP * (1.0f + (CHR_ENEMY_00_WAVE_HP_ADDRATE * GetObj_stage_00()->nCntWaveMain));
	pChr->nHP = pChr->nHPMax;	// HPに最大HPを代入
	if (pType->nCollAtkTime != -1) {
		pChr->nCounterCollAtk	// 衝突攻撃の間隔が-1でない時、衝突攻撃カウンターを設定
			= pType->nCollAtkTime;
	}

	// 部品(3D)のトランスフォームを取得
	GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);
}

//========================================
// InitParameterChr_enemy_00Control関数 - CHR:敵[00] 管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterChr_enemy_00Control(void)
{
	Chr_enemy_00Control *pChrCtl	// CHR:敵[00] の管理情報のポインタ
						= &g_chr_enemy_00Control;

	pChrCtl->state			= CHR_ENEMY_00_CONTROL_STATE_DYNAMIC;	// 状態
	pChrCtl->nSetBossIdx	= -1;									// 設定したボスの番号
}

//========================================
// SearchIndexChr_enemy_00関数 - CHR:敵[00] の番号の検索処理 -
//========================================
int SearchIndexChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00	*pSearchChr	// 検索用のCHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nIdx = 0; nIdx < CHR_ENEMY_00_MAX; nIdx++, pSearchChr++)
	{
		if (pChr == pSearchChr) 
		{// 検索用のポインタと一致した時、
			return nIdx;
		}
	}

	return -1;
}

//========================================
// ActLoadChr_enemy_00関数 - CHR:敵[00] の行動読み込み処理 -
//========================================
void ActLoadChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00Type	*pType		// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[pChr->nType];
	Chr_enemy_00ActInfo	*pActInfo	// CHR:敵[00] の行動管理情報のポインタ
						= &pChr->actInfo;
	Chr_enemy_00ActPtn	*pActPtn	// CHR:敵[00] の行動パターン情報のポインタ
						= &pType->actPtnSet.aActPtn[pActInfo->nActPtn];
	Chr_enemy_00Act		*pAct		// CHR:敵[00] の行動情報のポインタ
						= pActPtn->aAct;

	for (int nCntAct = 0; nCntAct < pActPtn->nActNum; nCntAct++, pAct++) 
	{
		if (pActInfo->nCounterAct != pAct->nTime) 
		{// 行動カウンターがカウントの行動する時間に一致していない時、
			continue;	// 処理を折り返す
		}

		// 行動の種類に応じた処理
		switch (pAct->cmd)
		{
			case /*/ 曲折式移動開始 /*/CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TORTUOUS_MOVE] = true;	// 曲折式移動フラグを真にする
				break;
			case /*/ 曲折式移動終了 /*/CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TORTUOUS_MOVE] = false;	// 曲折式移動フラグを偽にする
				break;
			case /*/ 方向転換式移動開始 /*/CHR_ENEMY_00_COMMAND_TURN_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_MOVE] = true;	// 方向転換式移動フラグを真にする
				pActInfo->fRotation			= pAct->aData[0];	// 回転力を代入
				pActInfo->fRandPosRadius	= pAct->aData[1];	// 乱数位置半径を代入
				break;
			case /*/ 方向転換式移動終了 /*/CHR_ENEMY_00_COMMAND_TURN_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_MOVE] = false;	// 方向転換式移動フラグを偽にする
				break;
			case /*/ 推進式移動開始 /*/CHR_ENEMY_00_COMMAND_FORCE_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_FORCE_MOVE] = true;	// 推進式移動フラグを真にする
				break;
			case /*/ 推進式移動終了 /*/CHR_ENEMY_00_COMMAND_FORCE_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_FORCE_MOVE] = false;	// 推進式移動フラグを偽にする
				break;
			case /*/ 振り向き位置に振り向き開始 /*/CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_TURN_POS] = true;	// 振り向き位置に振り向くフラグを真にする
				break;
			case /*/ 振り向き位置に振り向き終了 /*/CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_TURN_POS] = false;	// 振り向き位置に振り向くフラグを偽にする
				break;
			case /*/ 移動向きに振り向き開始 /*/CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT] = true;	// 移動向きに振り向くフラグを真にする
				break;
			case /*/ 移動向きに振り向き終了 /*/CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT] = false;	// 移動向きに振り向くフラグを偽にする
				break;
			case /*/ Z座標をロック /*/CHR_ENEMY_00_COMMAND_Z_LOCK:
				pActInfo->aAct[CHR_ENEMY_00_ACT_Z_LOCK] = true;	// Z座標ロックフラグを真にする
				break;
			case /*/ Z座標をアンロック /*/CHR_ENEMY_00_COMMAND_Z_UNLOCK:
				pActInfo->aAct[CHR_ENEMY_00_ACT_Z_LOCK] = false;	// Z座標ロックフラグを偽にする
				break;
			case /*/ 子設定 /*/CHR_ENEMY_00_COMMAND_CHILD_SET: {
				D3DXVECTOR3 setPos = pChr->pos;	// 設定位置

				// 設定位置を設定
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				
				// 子の設定処理
				SetChildChr_enemy_00(
					setPos,
					pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI),
					pAct->aData[6],
					SearchIndexChr_enemy_00(pChr));

				break;
			}
			case /*/ 子依存 /*/CHR_ENEMY_00_COMMAND_CHILD_DEPENDENCY:
				pActInfo->aAct[CHR_ENEMY_00_ACT_CHILD_DEPENDENCY] = true;	// 子依存フラグを真にする
				break;
			case /*/ 回転開始 /*/CHR_ENEMY_00_COMMAND_SPIN_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_SPIN] = true;	// 回転フラグを真にする
				pChr->actInfo.spin.x = pAct->aData[0];
				pChr->actInfo.spin.y = pAct->aData[1];
				pChr->actInfo.spin.z = pAct->aData[2];
				break;
			case /*/ 回転終了 /*/CHR_ENEMY_00_COMMAND_SPIN_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_SPIN] = false;	// 回転フラグを偽にする
				break;
			case /*/ 弾発射 /*/CHR_ENEMY_00_COMMAND_SHOT: {
				D3DXVECTOR3 setPos = pChr->pos;	// 設定位置

				// 設定位置を設定
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// ATK:弾[00] の設定処理
				SetAtk_bullet_00(
					setPos,
					pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI),
					pAct->aData[6],
					ATK_BULLET_00_PARENTTYPE_ENEMY);

				break;
			}
			case /*/ サウンド再生 /*/CHR_ENEMY_00_COMMAND_PLAY_SOUND:
				PlaySound((SOUND_LABEL)(int)pAct->aData[0]);
				break;
			case /*/ モーション設定 /*/CHR_ENEMY_00_COMMAND_MOTION_SET: {
				Motion3D *pMotion // 新規のモーションの情報のポインタ
					= &pType->motionSet.aMotion[(int)pAct->aData[0]];
				if (!pMotion->bLoop)
				{// 新規のモーションのループフラグが偽の時、
					pChr->nMotionTemp = pChr->nMotion;	// 現在のモーション番号を保存
				}
				pChr->nMotion = pAct->aData[0]; // モーション番号を代入
				SetMotion3D( // モーション設定
					&pChr->partsInfo,
					pChr->nMotion);
				break;
			}
			case /*/ 自動ショット開始 /*/CHR_ENEMY_00_COMMAND_AUTO_SHOT_START:
				pChr->nBulletType	= pAct->aData[0];	// 弾の種類を代入
				pChr->nShotTimeMin	= pAct->aData[1];	// 発射間隔の下限を代入
				pChr->nShotTimeMax	= pAct->aData[2];	// 発射間隔の上限を代入
				pChr->bAutoShot		= true;				// 自動ショットフラグを真にする

				pChr->nCounterShot		// ショットカウンターを設定
					= pChr->nShotTimeMin + (rand() % (pChr->nShotTimeMax - pChr->nShotTimeMin));
				break;
			case /*/ 自動ショット終了 /*/CHR_ENEMY_00_COMMAND_AUTO_SHOT_END:
				pChr->bAutoShot = false;	// 自動ショットフラグを偽にする
				break;
			case /*/ ユニット無視開始 /*/CHR_ENEMY_00_COMMAND_UNIT_IGNORED_START:
				pChr->bUnitIgnored = true;	// ユニット無視フラグを真にする
				break;
			case /*/ ユニット無視終了 /*/CHR_ENEMY_00_COMMAND_UNIT_IGNORED_END:
				pChr->bUnitIgnored = false;	// ユニット無視フラグを偽にする
				break;
			case /*/ 移動開始 /*/CHR_ENEMY_00_COMMAND_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_MOVE] = true;	// 移動フラグを真にする
				pChr->actInfo.move.x = pAct->aData[0];
				pChr->actInfo.move.y = pAct->aData[1];
				pChr->actInfo.move.z = pAct->aData[2];
				break;
			case /*/ 移動終了 /*/CHR_ENEMY_00_COMMAND_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_MOVE] = false;	// 移動フラグを偽にする
				break;
			case /*/ 味方設定 /*/CHR_ENEMY_00_COMMAND_ALLY_SET: {
				D3DXVECTOR3 setPos = pChr->pos;	// 設定位置
				D3DXVECTOR3 setRot 				// 設定向き
							= pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI);

				// 設定位置を設定
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// 衝突判定に必要な情報
				CollisionInfo myCollInfo = {
					&setPos,
					pChr->pos,
					NULL,
					&setRot,
					setRot,
					g_aChr_enemy_00Type[(int)pAct->aData[6]].hitTestSet.aHitTest[0] };

				Collision collision = {};	// 衝突情報

				// OBJ:ステージ[00] との衝突判定
				CollisionObj_stage_00(VECTOR_X, &collision, &collision, myCollInfo);
				CollisionObj_stage_00(VECTOR_Z, &collision, &collision, myCollInfo);

				if (!collision.bHit)
				{// 物理的に接触していない時、
					// CHR:敵[00] の設定処理
					SetChr_enemy_00(
						setPos,
						setRot,
						pAct->aData[6]);
				}

				break;
			}
			case /*/ ATK:衝撃 /*/CHR_ENEMY_00_COMMAND_ATK_IMPACT: {
				D3DXVECTOR3 setPos = pChr->pos;	// 設定位置
				D3DXVECTOR3 setRot 				// 設定向き
					= pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI);

				// 設定位置を設定
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// ATK:衝撃[00] の設定処理
				SetAtk_impact_00(
					setPos,
					setRot,
					pAct->aData[6],
					ATK_IMPACT_00_PARENTTYPE_ENEMY);

				break;
			}
			case /*/ EFF:衝撃波[00] /*/CHR_ENEMY_00_COMMAND_EFF_SHOCKWAVE_00: {
				D3DXVECTOR3 setPos = pChr->pos;	// 設定位置
				D3DXVECTOR3 setRot 				// 設定向き
					= pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI);

				// 設定位置を設定
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// EFF:衝撃波[00] の設定処理
				SetEff_shockWave_00(
					setPos,
					setRot,
					pAct->aData[6],
					pAct->aData[7],
					pAct->aData[8],
					{ (int)pAct->aData[9],(int)pAct->aData[10],(int)pAct->aData[11],(int)pAct->aData[12] },
					{ (int)pAct->aData[13],(int)pAct->aData[14],(int)pAct->aData[15],(int)pAct->aData[16] },
					(int)pAct->aData[17]);

				break;
			}
			case /*/ 衝突ダメージ変更 /*/CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_CHANGE:
				pChr->nNewCollDamage = pAct->aData[0];	// 新規衝突ダメージを設定
				pChr->bCollDamageChange = true;			// 衝突ダメージ変更フラグを真にする
				break;
			case /*/ 衝突ダメージを元に戻す /*/CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_UNDO:
				pChr->bCollDamageChange = false;	// 衝突ダメージ変更フラグを偽にする
				break;
			case /*/ ボス依存 /*/CHR_ENEMY_00_COMMAND_BOSS_DEPENDENCY:
				pChr->bBossDependency = true;	// ボス依存フラグを真にする
				break;
			case /*/ 味方設定(絶対位置) /*/CHR_ENEMY_00_COMMAND_ALLY_SET_ABSOLUTE:
				// CHR:敵[00] の設定処理
				SetChr_enemy_00(
					D3DXVECTOR3(pAct->aData[0], pAct->aData[1], pAct->aData[2]),
					D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI),
					pAct->aData[6]);
				break;
			case /*/ 認識OFF /*/CHR_ENEMY_00_COMMAND_RECOGNITION_OFF:
				pChr->bRecognitionOff = true;	// 認識OFFフラグを真にする
				break;
			case /*/ 瞬間移動 /*/CHR_ENEMY_00_COMMAND_TELEPORTATION:
				// 位置を設定する
				pChr->pos = D3DXVECTOR3(pAct->aData[0], pAct->aData[1], pAct->aData[2]);

				// 子の移動処理
				ChildMoveChr_enemy_00(pChr);

				// 過去の位置を設定する(衝突防止)
				pChr->posOld = D3DXVECTOR3(pAct->aData[0], pAct->aData[1], pAct->aData[2]);
				break;
		}
	}

	if (!((pActInfo->nCounterAct == 0) && (pActPtn->nLoopTime == 0)))
	{// (行動カウンターが0 & ループ時間が0) でない時、
		pActInfo->nCounterAct++;	// 行動カウンターを加算
		IntLoopControl(				// 行動カウンターを制御
			&pActInfo->nCounterAct,
			pActPtn->nLoopTime + 1,
			0);
	}
}

//========================================
// ActChr_enemy_00関数 - CHR:敵[00] の行動処理 -
//========================================
void ActChr_enemy_00(Chr_enemy_00 *pChr)
{
	// 行動読み込み処理
	ActLoadChr_enemy_00(pChr);

	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->bAutoShot)
	{// 自動ショットフラグが真の時、
		if (--pChr->nCounterShot <= 0)
		{// ショットカウンターを減算した結果0以下の時、
			SetAtk_bullet_00(pChr->pos, pChr->rot, pChr->nBulletType, ATK_BULLET_00_PARENTTYPE_ENEMY);		// ATK:弾[00] の設定処理
			pChr->nCounterShot = pChr->nShotTimeMin + (rand() % (pChr->nShotTimeMax - pChr->nShotTimeMin));	// ショットカウンターを設定
		}
	}

	if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk > 0))
	{// 衝突攻撃の間隔が-1でない & 衝突攻撃カウンターが0を上回っている時、
		pChr->nCounterCollAtk--;	// 衝突攻撃カウンターを減算
	}

	Chr_enemy_00ActInfo *pActInfo	// 行動管理情報
						= &pChr->actInfo;

	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_MAX; nCntAct++)
	{
		if (!pActInfo->aAct[nCntAct]) 
		{// カウントの行動フラグが偽の時、
			continue;	// 繰り返し処理を折り返す
		}

		// 行動に応じた処理
		switch (nCntAct)
		{
		case /*/ 曲折式移動 /*/CHR_ENEMY_00_ACT_TORTUOUS_MOVE: {
			if (--pActInfo->nCounterMove <= 0)
			{// 移動カウンターを減算した結果0以下の時、
				if ((FindDistance(
					D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
					D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f))
					<= pType->fPlayerSearchDistance)
					&&
					(Lottery(pType->fPlayerAimProbability))
					&&
					(GetChr_player_00()->state != CHR_PLAYER_00_STATE_DOWN))
				{// プレイヤーとの距離が索敵距離の内側 & プレイヤーを狙う確率の抽選に当たった & プレイヤーの状態がダウンでない時、
					pChr->moveRot.y = FindAngle(	// 移動向きをプレイヤーまでの角度に設定
						D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
						D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f));
					pChr->pTurnPos = &GetChr_player_00()->pos;	// 振り向き先の位置のポインタをプレイヤーの位置で設定
				}
				else if (Lottery(pType->fCoreAimProbability))
				{// コアを狙う確率の抽選に当たった時、
					pChr->moveRot.y = FindAngle(	// 移動向きをコアまでの角度に設定
						D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
						D3DXVECTOR3(GetObj_core_00()->pos.x, GetObj_core_00()->pos.z, 0.0f));
					pChr->pTurnPos = &GetObj_core_00()->pos;	// 振り向き先の位置のポインタをコアの位置で設定
				}
				else
				{// いずれも該当しない時、
					pChr->moveRot.y = -D3DX_PI + fRand(D3DX_PI * 2.0f);	// 移動向きを乱数で設定
				}

				pActInfo->nCounterMove =	// 移動カウンターを乱数で設定
					CHR_ENEMY_00_MOVE_COUNTER_MIN + (rand() % (CHR_ENEMY_00_MOVE_COUNTER_MAX - CHR_ENEMY_00_MOVE_COUNTER_MIN));
			}

			// 移動処理
			MoveChr_enemy_00(pChr, pType->fMoveForce);

			break;
		}
		case /*/ 方向転換式移動 /*/CHR_ENEMY_00_ACT_TURN_MOVE: {
			if (--pActInfo->nCounterMove <= 0)
			{// 移動カウンターを減算した結果0以下の時、
				if ((FindDistance(
					D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
					D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f))
					<= pType->fPlayerSearchDistance)
					&&
					(Lottery(pType->fPlayerAimProbability))
					&&
					(GetChr_player_00()->state != CHR_PLAYER_00_STATE_DOWN))
				{// プレイヤーとの距離が索敵距離の内側 & プレイヤーを狙う確率の抽選に当たった & プレイヤーの状態がダウンでない時、
					pChr->pTurnPos = &GetChr_player_00()->pos;	// 振り向き先の位置のポインタをプレイヤーの位置で設定
				}
				else if (Lottery(pType->fCoreAimProbability))
				{// コアを狙う確率の抽選に当たった時、
					pChr->pTurnPos = &GetObj_core_00()->pos;	// 振り向き先の位置のポインタをコアの位置で設定
				}
				else
				{// いずれも該当しない時、振り向き先の位置を乱数で設定
					float	fAngle	// 設定向き
							= -D3DX_PI + fRand(D3DX_PI * 2);
					pChr->pTurnPos =
						&D3DXVECTOR3(
							sinf(fAngle) * pActInfo->fRandPosRadius,
							0.0f,
							cosf(fAngle) * pActInfo->fRandPosRadius);
				}

				pActInfo->nCounterMove =	// 移動カウンターを乱数で設定
					CHR_ENEMY_00_MOVE_COUNTER_MIN + (rand() % (CHR_ENEMY_00_MOVE_COUNTER_MAX - CHR_ENEMY_00_MOVE_COUNTER_MIN));
			}

			// 方向転換処理
			TurnAroundLookDown(&pChr->moveRot, pChr->pos, *pChr->pTurnPos, pActInfo->fRotation);

			// 移動処理
			MoveChr_enemy_00(pChr, pType->fMoveForce);
			break;
		}
		case /*/ 推進式移動 /*/CHR_ENEMY_00_ACT_FORCE_MOVE: {
			break;
		}
		case /*/ 振り向き位置に振り向く /*/CHR_ENEMY_00_ACT_TURN_TO_TURN_POS: {
			if (pChr->pTurnPos != NULL)
			{// 振り向き先の位置のポインタが設定されている時、
				// 向きを振り向き先の位置に向けて推移する
				pChr->rot.y += (
					AngleDifference(
						pChr->rot.y,
						FindAngle(
							D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
							D3DXVECTOR3(pChr->pTurnPos->x, pChr->pTurnPos->z, 0.0f)))
					* CHR_ENEMY_00_ROT_DIAMETER);
			}
			break;
		}
		case /*/ 移動向きに振り向く /*/CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT:
			// 向きを移動向きに向けて推移する
			pChr->rot.y += (
				AngleDifference(
					pChr->rot.y,
					pChr->moveRot.y)
				* CHR_ENEMY_00_ROT_DIAMETER);
			break;
		case /*/ 回転 /*/CHR_ENEMY_00_ACT_SPIN:
			// 向きに回転量を加算
			pChr->rot += pChr->actInfo.spin;
			break;
		case /*/ 移動 /*/CHR_ENEMY_00_ACT_MOVE:
			// ※移動量に移動量を加算(向きに応じて)
			// X
			pChr->move.x += sinf(pChr->rot.y - (D3DX_PI*0.5f)) * pChr->actInfo.move.x;
			pChr->move.z += cosf(pChr->rot.y - (D3DX_PI*0.5f)) * pChr->actInfo.move.x;
			// Z
			pChr->move.x += sinf(pChr->rot.y) * pChr->actInfo.move.z;
			pChr->move.z += cosf(pChr->rot.y) * pChr->actInfo.move.z;
			// Y
			pChr->move.y += pChr->actInfo.move.y;
			break;
		}
	}

	// 移動向きを制御
	RotControl(&pChr->moveRot);
}

//========================================
// MoveChr_enemy_00関数 - CHR:敵[00] の移動処理 -
//========================================
void MoveChr_enemy_00(Chr_enemy_00 *pChr, float fMove)
{
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->state == CHR_ENEMY_00_STATE_PARALYSIS) 
	{// 状態が麻痺の時、
		fMove *= CHR_ENEMY_00_PARALYSIS_DECREMENT_RATE;	// 移動力を更新
	}

	// 移動量を更新
	pChr->move.x += sinf(pChr->moveRot.y) * fMove;
	pChr->move.z += cosf(pChr->moveRot.y) * fMove;
}

//========================================
// UpdatePosChr_enemy_00関数 - CHR:敵[00] の位置更新処理 -
//========================================
void UpdatePosChr_enemy_00(Chr_enemy_00 *pChr)
{
	// 共通の衝突情報を初期化
	pChr->cmnCollision = {};

	// オブジェクト毎の衝突情報を初期化
	for (int nCntHitObj = 0; nCntHitObj < CHR_ENEMY_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};
	}

	pChr->nAttackIndex = -1;	// 攻撃するオブジェクトの番号を初期化

	D3DXVECTOR3 posTemp;	// 位置を保存

	pChr->pos.x += pChr->move.x / (1.0f + fabsf(pChr->force.x));	// X座標に移動量を適用
	pChr->move.x *= CHR_ENEMY_00_MOVE_DAMP;							// X方向の移動量を減衰
	pChr->pos.x += pChr->force.x;									// X座標に力を適用
	pChr->force.x *= CHR_ENEMY_00_FORCE_DAMP;						// X方向の力を減衰
	if (pChr->nParentIdx != -1) { posTemp.x = pChr->pos.x; }		// X座標を保存(親持ちのみ)
	CheckCollisionChr_enemy_00(pChr, VECTOR_X);						// X方向の衝突チェック処理
	if (pChr->nParentIdx != -1) { pChr->pos.x = posTemp.x; }		// X座標を戻す(親持ちのみ)

	if (!pChr->actInfo.aAct[CHR_ENEMY_00_ACT_Z_LOCK])
	{// Z座標ロックが偽の時、
		pChr->pos.z += pChr->move.z / (1.0f + fabsf(pChr->force.z));	// Z座標に移動量を適用
		pChr->move.z *= CHR_ENEMY_00_MOVE_DAMP;							// Z方向の移動量を減衰
		pChr->pos.z += pChr->force.z;									// Z座標に力を適用
		pChr->force.z *= CHR_ENEMY_00_FORCE_DAMP;						// Z方向の力を減衰
	}
	if (pChr->nParentIdx != -1) { posTemp.z = pChr->pos.z; }			// Z座標を保存(親持ちのみ)
	CheckCollisionChr_enemy_00(pChr, VECTOR_Z);							// Z方向の衝突チェック処理
	if (pChr->nParentIdx != -1) { pChr->pos.z = posTemp.z; }			// Z座標を戻す(親持ちのみ)

	// 衝突情報に応じた処理
	CollisionProcessChr_enemy_00(pChr);

	// 子の移動処理
	ChildMoveChr_enemy_00(pChr);
}

//========================================
// CheckCollisionChr_enemy_00関数 - CHR:敵[00] の衝突チェック処理 -
//========================================
void CheckCollisionChr_enemy_00(Chr_enemy_00 *pChr, VECTOR vector)
{
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[pChr->nType];

	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&pChr->pos,
		pChr->posOld,
		NULL,
		&pChr->rot,
		pChr->rot,
		pType->hitTestSet.aHitTest[pChr->hitTestInfo.nHitTest]
	};

	// 衝突判定
	/*/ CHR:プレイヤー	[00] /*/CollisionChr_player_00	(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_CHR_PLAYER_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:コア		[00] /*/CollisionObj_core_00	(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_CORE_00]	, &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:ステージ	[00] /*/CollisionObj_stage_00	(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_STAGE_00]	, &pChr->cmnCollision, myCollInfo);

	// 衝突判定(ユニット)
	if(!pChr->bUnitIgnored)
	{// ユニット無視フラグが偽の時、
		int nIndex;	// 番号

		nIndex = /*/ OBJ:ブロック	[00] /*/CollisionObj_block_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// 衝突攻撃の間隔が-1でない & 衝突攻撃カウンターが0以下 & 攻撃するオブジェクト番号が-1の時、
			if (-1 != nIndex) {// OBJ:ブロック	[00] と接触した時、
				pChr->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
				pChr->attackObj					// 攻撃するオブジェクトの分類を設定
					= CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00;
			}
		}
		nIndex = /*/ OBJ:タレット	[00] /*/CollisionObj_turret_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// 衝突攻撃の間隔が-1でない & 衝突攻撃カウンターが0以下 & 攻撃するオブジェクト番号が-1の時、
			if (-1 != nIndex) {// OBJ:タレット	[00] と接触した時、
				pChr->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
				pChr->attackObj					// 攻撃するオブジェクトの分類を設定
					= CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00;
			}
		}
		nIndex = /*/ OBJ:放電装置	[00] /*/CollisionObj_discharger_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// 衝突攻撃の間隔が-1でない & 衝突攻撃カウンターが0以下 & 攻撃するオブジェクト番号が-1の時、
			if (-1 != nIndex) {// OBJ:放電装置	[00] と接触した時、
				pChr->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
				pChr->attackObj					// 攻撃するオブジェクトの分類を設定
					= CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00;
			}
		}
		nIndex = /*/ OBJ:ミラー	[00] /*/CollisionObj_mirror_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// 衝突攻撃の間隔が-1でない & 衝突攻撃カウンターが0以下 & 攻撃するオブジェクト番号が-1の時、
			if (-1 != nIndex) {// OBJ:ミラー	[00] と接触した時、
				pChr->nAttackIndex = nIndex;	// 攻撃するオブジェクトの番号を設定
				pChr->attackObj					// 攻撃するオブジェクトの分類を設定
					= CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00;
			}
		}
	}

	if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
	{// 衝突攻撃の間隔が-1でない & 衝突攻撃カウンターが0以下 & 攻撃するオブジェクトの番号が-1の時、
		bool	bAttack = true;	// 攻撃フラグ

		if (pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_CHR_PLAYER_00].bHit)
		{// CHR:プレイヤー[00] との当たりフラグが真の時、
			int nCollDamage = pChr->bCollDamageChange ? pChr->nNewCollDamage : pType->nCollisionDamage;
			int nDamage	// ダメージ
				= nCollDamage * (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));

			DamageChr_player_00(nDamage);	// ダメージ処理
			KnockBackChr_player_00(							// ノックバック処理
				D3DXVECTOR3(0.0f,
					FindAngle(
						D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
						D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f)), 0.0f),
				CHR_ENEMY_00_COLLISION_KNOCK_BACK);
		}
		else if (pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_CORE_00].bHit)
		{// OBJ:コア[00] との当たりフラグが真の時、
			DamageObj_core_00(pType->nCollisionDamage);	// ダメージ処理
		}
		else 
		{// いずれも該当しない時、
			bAttack = false;	// 攻撃フラグを偽にする
		}

		if (bAttack) 
		{// 攻撃フラグが真の時、
			pChr->nCounterCollAtk = pType->nCollAtkTime;	// 衝突攻撃カウンターを設定
			pChr->nAttackIndex = -1;						// 攻撃するオブジェクトの番号を初期化
		}
	}
}

//========================================
// CollisionProcessChr_enemy_00関数 - CHR:敵[00] の衝突情報に応じた処理 -
//========================================
void CollisionProcessChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->nAttackIndex != -1)
	{// 攻撃するオブジェクトの番号が-1で無い時、
		int nCollDamage = pChr->bCollDamageChange ? pChr->nNewCollDamage : pType->nCollisionDamage;
		int nDamage	// ダメージ
			= nCollDamage * (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));

		// 攻撃するオブジェクトの分類に応じたダメージ処理
		switch (pChr->attackObj)
		{
			case /*/ OBJ:ブロック	[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00:
				DamageObj_block_00(pChr->nAttackIndex, nDamage);
				break;
			case /*/ OBJ:タレット	[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00:
				DamageObj_turret_00(pChr->nAttackIndex, nDamage);
				break;
			case /*/ OBJ:ミラー		[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00:
				DamageObj_mirror_00(pChr->nAttackIndex, nDamage);
				break;
			case /*/ OBJ:放電装置	[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00:
				DamageObj_discharger_00(pChr->nAttackIndex, nDamage);
				break;
		}

		pChr->nCounterCollAtk = pType->nCollAtkTime;	// 衝突攻撃カウンターを設定
	}
}

//========================================
// DestroyChr_enemy_00関数 -CHR:敵[00] の破壊処理 -
//========================================
void DestroyChr_enemy_00(Chr_enemy_00 *pChr) 
{
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[pChr->nType];

	pChr->nHP	= 0;		// HPを0にする
	pChr->bUse	= false;	// 使用されていない状態にする

	// EFF:爆発[00] の設定処理
	SetEff_explosion_00(pChr->pos, pType->nExplosionType);

	// ITM:コイン[00] の設定処理
	SetItm_coin_00(pChr->pos, pType->nDropCoin);

	if ((pChr->nParentIdx == -1) &&
		(pType->rank != CHR_ENEMY_00_RANK_ALLY))
	{// 親が存在しない時、
		GetObj_stage_00()->nKill++;	// OBJ:ステージ[00] のキル数を加算
	}
	
	// プレイヤーのスコアを加算
	GetChr_player_00()->nScore += pType->nScore;

	// 子の破壊処理
	ChildDestoryChr_enemy_00(pChr);
}

//========================================
// SetChr_enemy_00Vibration関数 - CHR:敵[00] の振動を設定 -
//========================================
void SetChr_enemy_00Vibration(Chr_enemy_00 *pChr, float fVibration)
{
	// 振動の強さを設定
	pChr->fVibration += fVibration;

	// 振動カウンターを初期化
	pChr->nCounterVib = 0;
}

//========================================
// SetChildChr_enemy_00関数 - CHR:敵[00] の子の設定処理 -
//========================================
void SetChildChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nParentIdx)
{
	Chr_enemy_00		*pChr	// CHR:敵[00] の情報のポインタ
						= g_aChr_enemy_00;
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[nType];

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pChr->pos =						// 位置を代入
		pChr->posOld = pos;				// 過去の位置を設定
		pChr->rot =						// 向きを代入
		pChr->rotOld = 					// 過去の向きを代入
		pChr->moveRot = rot;			// 移動向きを代入
		pChr->nType = nType;			// 種類を代入
		pChr->nParentIdx = nParentIdx;	// 親の番号を代入

		// 設定時のパラメーター初期化処理
		InitParameterSetChr_enemy_00(pChr);

		// 行動読み込み処理
		ActLoadChr_enemy_00(pChr);

		// 繰り返し処理を抜ける
		break;
	}

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
	}
}

//========================================
// ChildMoveChr_enemy_00関数- CHR:敵[00] の子の移動処理 -
//========================================
void ChildMoveChr_enemy_00(Chr_enemy_00 *pChr)
{
	int nParentIdx	// 親番号
		= SearchIndexChr_enemy_00(pChr);

	// 位置/向きの推移値
	D3DXVECTOR3	posTrans = pChr->pos - pChr->posOld;
	D3DXVECTOR3	rotTrans = pChr->rot - pChr->rotOld;

	Chr_enemy_00	*pChildChr	// 子のCHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;	

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx))
		{// 使用されていない or 親番号が一致していない時、
			continue;	// 繰り返し処理を折り返す
		}
		
		// 位置を更新
		pChildChr->pos += posTrans;

		D3DXVECTOR3 rot	// 親から子への向き
				= D3DXVECTOR3(
				0.0f,
				FindAngleLookDown(pChr->pos, pChildChr->pos) + rotTrans.y,
				0.0f);
		float	fDistance	// 親と子の距離
				= FindDistanceLookDown(pChr->pos, pChildChr->pos);

		// 子に向きの推移値を適用
		pChildChr->rot += rotTrans;

		// 位置を親の所に戻した後、正しい角度に距離を加算する
		pChildChr->pos.x = pChr->pos.x;
		pChildChr->pos.z = pChr->pos.z;
		pChildChr->pos.x += sinf(rot.y) * fDistance;
		pChildChr->pos.z += cosf(rot.y) * fDistance;
	}
}

//========================================
// ChildSetDamageCounterChr_enemy_00関数- CHR:敵[00] の子のダメージカウンター設定処理 -
//========================================
void ChildSetDamageCounterChr_enemy_00(Chr_enemy_00 *pChr)
{
	int				nParentIdx	// 親番号
					= SearchIndexChr_enemy_00(pChr);
	Chr_enemy_00	*pChildChr	// 子のCHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx)
			&&
			(!((pChr->nParentIdx == nCntChr) && (pChildChr->actInfo.aAct[CHR_ENEMY_00_ACT_CHILD_DEPENDENCY]))))
		{// 使用されていない or 親番号が一致していない or 子依存の親の時、
			continue;	// 繰り返し処理を折り返す
		}

		pChildChr->nCounterDamage	// ダメージカウンターを設定
			= CHR_ENEMY_00_DAMAGE_TIME;
	}
}

//========================================
// ShildAppealMotionSetChr_enemy_00関数- CHR:敵[00] の子のアピールモーション設定処理 -
//========================================
void ShildAppealMotionSetChr_enemy_00(Chr_enemy_00 *pChr) 
{
	int				nParentIdx	// 親番号
					= SearchIndexChr_enemy_00(pChr);
	Chr_enemy_00	*pChildChr	// 子のCHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChild = 0; nCntChild < CHR_ENEMY_00_MAX; nCntChild++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx))
		{// 使用されていない or 親番号が一致していない or 子依存の親の時、
			continue;	// 繰り返し処理を折り返す
		}

		// 現在のモーション番号を保存
		pChildChr->nMotionTemp = pChildChr->nMotion;

		// モーションをアピールで設定
		pChildChr->nMotion = CHR_ENEMY_00_APPEAL_MOTION;
		SetMotion3D(
			&pChildChr->partsInfo,
			pChildChr->nMotion);

		{
			int				nParentIdx2		// 親番号
							= SearchIndexChr_enemy_00(pChildChr);
			Chr_enemy_00	*pGrandChildChr	// 孫のCHR:敵[00] の情報のポインタ
							= g_aChr_enemy_00;

			for (int nCntGrandChild = 0; nCntGrandChild < CHR_ENEMY_00_MAX; nCntGrandChild++, pGrandChildChr++)
			{
				if ((!pGrandChildChr->bUse)
					||
					(pGrandChildChr->nParentIdx != nParentIdx2))
				{// 使用されていない or 親番号が一致していない or 孫依存の親の時、
					continue;	// 繰り返し処理を折り返す
				}

				// 現在のモーション番号を保存
				pGrandChildChr->nMotionTemp = pGrandChildChr->nMotion;

				// モーションをアピールで設定
				pGrandChildChr->nMotion = CHR_ENEMY_00_APPEAL_MOTION;
				SetMotion3D(
					&pGrandChildChr->partsInfo,
					pGrandChildChr->nMotion);
			}
		}
	}
}

//========================================
// ChildDestoryChr_enemy_00関数- CHR:敵[00] の子の破壊処理 -
//========================================
void ChildDestoryChr_enemy_00(Chr_enemy_00 *pChr)
{
	int				nParentIdx	// 親番号
					= SearchIndexChr_enemy_00(pChr);
	Chr_enemy_00	*pChildChr	// 子のCHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx)
			&&
			(!((pChr->nParentIdx == nCntChr) && (pChildChr->actInfo.aAct[CHR_ENEMY_00_ACT_CHILD_DEPENDENCY]))))
		{// 使用されていない or 親番号が一致していない or 子依存の親の時、
			continue;	// 繰り返し処理を折り返す
		}

		// 破壊処理
		DestroyChr_enemy_00(pChildChr);
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_enemy_00関数 - CHR:敵[00] の読み込み処理 -
//========================================
void LoadChr_enemy_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Chr_enemy_00Type	*pChrType	// CHR:敵[00] の種類毎の情報
						= g_aChr_enemy_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(CHR_ENEMY_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	g_nChr_enemy_00TypeNum = 0;	// 種類数を初期化

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// 読み込みを終了
		if (aDataSearch[0] == '#')			{ continue; }				// 折り返す

		if (!strcmp(aDataSearch, "TYPE")) {
			g_nChr_enemy_00TypeNum++;	// 種類数を加算
			// CHR:敵[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))					{ pChrType++; break; }										// 読み込みを終了
				else if (!strcmp(aDataSearch, "HIT_POINT:"))				{ fscanf(pFile, "%d", &pChrType->nHP); }					// HP
				else if (!strcmp(aDataSearch, "MOVE_FORCE:"))				{ fscanf(pFile, "%f", &pChrType->fMoveForce); }				// 移動力
				else if (!strcmp(aDataSearch, "COLLISION_DAMAGE:"))			{ fscanf(pFile, "%d", &pChrType->nCollisionDamage); }		// 衝突ダメージ
				else if (!strcmp(aDataSearch, "COLLATK_TIME:"))				{ fscanf(pFile, "%d", &pChrType->nCollAtkTime); }			// 衝突攻撃の間隔
				else if (!strcmp(aDataSearch, "EXPLOSION_TYPE:"))			{ fscanf(pFile, "%d", &pChrType->nExplosionType); }			// 爆発の種類
				else if (!strcmp(aDataSearch, "SHADOW_RADIUS:"))			{ fscanf(pFile, "%f", &pChrType->fShadowRadius); }			// 影の半径
				else if (!strcmp(aDataSearch, "PLAYER_SEARCH_DISTANCE:"))	{ fscanf(pFile, "%f", &pChrType->fPlayerSearchDistance); }	// プレイヤー索敵距離
				else if (!strcmp(aDataSearch, "PLAYER_AIM_PROBABILITY:"))	{ fscanf(pFile, "%f", &pChrType->fPlayerAimProbability); }	// プレイヤーを狙う確率
				else if (!strcmp(aDataSearch, "CORE_AIM_PROBABILITY:"))		{ fscanf(pFile, "%f", &pChrType->fCoreAimProbability); }	// コアを狙う確率
				else if (!strcmp(aDataSearch, "DROP_COIN:"))				{ fscanf(pFile, "%d", &pChrType->nDropCoin); }				// ドロップするコイン量
				else if (!strcmp(aDataSearch, "SCORE:"))					{ fscanf(pFile, "%d", &pChrType->nScore); }					// スコア
				else if (!strcmp(aDataSearch, "RANK:"))						{ fscanf(pFile, "%d", &pChrType->rank); }					// ランク
				else if (!strcmp(aDataSearch, "DAMAGE_SE:"))				{ fscanf(pFile, "%d", &pChrType->damageSE); }				// ダメージSE
				else if (!strcmp(aDataSearch, "ACTPTNSET"))					{ LoadActPtnSetChr_enemy_00(pFile, &pChrType->actPtnSet); }	// 行動パターン設定情報
				else if (!strcmp(aDataSearch, "HITTESTSET"))				{ LoadHitTestSet(pFile, &pChrType->hitTestSet); }			// 当たり判定設定情報
				else if (!strcmp(aDataSearch, "PARTSSET"))					{ LoadParts3DSet(pFile, &pChrType->partsSet); }				// 部品設定情報
				else if (!strcmp(aDataSearch, "MOTIONSET"))					{ LoadMotion3DSet(pFile, &pChrType->motionSet); }			// モーション設定情報
			}
		}
	}
}

//========================================
// InitChr_enemy_00関数 - CHR:敵[00] の初期化処理 -
//========================================
void InitChr_enemy_00(void)
{
	// 管理情報のパラメーターの初期化
	InitParameterChr_enemy_00Control();

	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < g_nChr_enemy_00TypeNum; nCntType++)
	{
		Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
							= &g_aChr_enemy_00Type[nCntType];

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
				&g_aBuffMatChr_enemy_00[nCntType][nCntParts],
				NULL,
				&g_aNumMatChr_enemy_00[nCntType][nCntParts],
				&g_aMeshChr_enemy_00[nCntType][nCntParts]);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_aBuffMatChr_enemy_00[nCntType][nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_enemy_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureChr_enemy_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
							= &g_aChr_enemy_00Type[pChr->nType];

		// パラメーターの初期化処理
		InitParameterChr_enemy_00(pChr);

		// 部品(3D)のトランスフォームを初期化
		InitParts3DTransform(&pChr->partsInfo, &pType->partsSet);
	}
}

//========================================
// UninitChr_enemy_00関数 - CHR:敵[00] の終了処理 -
//========================================
void UninitChr_enemy_00(void)
{
	for (int nCntType = 0; nCntType < CHR_ENEMY_00_TYPE_MAX; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// メッシュの破棄
			if (g_aMeshChr_enemy_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshChr_enemy_00[nCntType][nCntParts]->Release();
				g_aMeshChr_enemy_00[nCntType][nCntParts] = NULL;
			}

			// マテリアルの破棄
			if (g_aBuffMatChr_enemy_00[nCntType][nCntParts] != NULL)
			{
				g_aBuffMatChr_enemy_00[nCntType][nCntParts]->Release();
				g_aBuffMatChr_enemy_00[nCntType][nCntParts] = NULL;
			}
		}
	}
}

//========================================
// UpdateChr_enemy_00関数 - CHR:敵[00] の更新処理 -
//========================================
void UpdateChr_enemy_00(void)
{
	Chr_enemy_00		*pChr		// CHR:敵[00] の情報のポインタ
						= g_aChr_enemy_00;
	Chr_enemy_00Control *pChrCtl	// CHR:敵[00] の管理情報のポインタ
						= &g_chr_enemy_00Control;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse) 
		{// 使用されていない状態の時、
			continue;	// 繰り返し処理を折り返す
		}

		Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
							= &g_aChr_enemy_00Type[pChr->nType];

		if (pChrCtl->state == CHR_ENEMY_00_CONTROL_STATE_BOSS_APPEAL) 
		{// 管理状態がボスアピールの時、
			if (((nCntChr == pChrCtl->nSetBossIdx) || 
				((pChr->nParentIdx == pChrCtl->nSetBossIdx) && (pChr->nParentIdx != -1)) ||
				((g_aChr_enemy_00[pChr->nParentIdx].nParentIdx == pChrCtl->nSetBossIdx) && (pChr->nParentIdx != -1)))
				&&
				(GetMd_game_00()->state != MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET))
			{// カウントか親番号か親の親番号が設定したボスの番号と一致している & MD:ゲーム画面[00] の状態がボス出現ムービー(視点リセット)でない時、
				// モーションの更新処理
				if (UpdateMotion3D(
					pChr->partsInfo.nMotion,
					g_aChr_enemy_00Type[pChr->nType].motionSet,
					&pChr->partsInfo,
					g_aChr_enemy_00Type[pChr->nType].partsSet))
				{// モーションがループに達した時、
					pChr->nMotion = pChr->nMotionTemp;	// モーションを保存したものに設定
					SetMotion3D(	// モーション設定
						&pChr->partsInfo,
						pChr->nMotion);

					// MD:ゲーム画面[00] の状態をボス出現ムービー(視点リセット)に設定
					SetMd_game_00State(MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET);
				}
			}

			// 部品(3D)のトランスフォームを取得
			GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);
		}
		else if (pChrCtl->state != CHR_ENEMY_00_CONTROL_STATE_STATIC)
		{// (管理状態がボスアピールでない) & 管理状態が静的で無い時、
			if (pChr->state != CHR_ENEMY_00_STATE_NORMAL)
			{// 状態が通常でない時、
				if (--pChr->nCounterState <= 0)
				{// 状態カウンターを減算した結果0以下の時、
					pChr->state = CHR_ENEMY_00_STATE_NORMAL;	// 状態を通常に設定
				}
			}

			// 行動処理
			ActChr_enemy_00(pChr);

			// 位置更新処理
			UpdatePosChr_enemy_00(pChr);

			// 向きを制御
			RotControl(&pChr->rot);
			
			// 部品(3D)のトランスフォームを取得
			GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);

			// モーションの更新処理
			if (UpdateMotion3D(
				pChr->partsInfo.nMotion,
				g_aChr_enemy_00Type[pChr->nType].motionSet,
				&pChr->partsInfo,
				g_aChr_enemy_00Type[pChr->nType].partsSet))
			{// モーションがループに達した時、
				Motion3D	*pMotion	// 現在のモーションの情報のポインタ
							= &pType->motionSet.aMotion[pChr->nMotion];
				if (!pMotion->bLoop)
				{// 現在のモーションのループフラグが偽の時、
					pChr->nMotion = pChr->nMotionTemp;	// モーションを保存したものに設定
					SetMotion3D( // モーション設定
						&pChr->partsInfo,
						pChr->nMotion);
				}
			}

			if (pChr->fVibration >= CHR_ENEMY_00_VIBRATION_MIN)
			{// 振動の強さが振動の強さの下限以上の時、
				if (--pChr->nCounterVib <= 0)
				{// 振動カウンターを減算した結果0以下の時、
					pChr->nCounterVib = CHR_ENEMY_00_ONCE_VIBRATION_TIME;	// 振動カウンターを設定
					float fAngle = -D3DX_PI + fRand(D3DX_PI * 2);			// 向きを乱数で設定
					pChr->vibrationPos.x = sinf(fAngle) * pChr->fVibration;
					pChr->vibrationPos.z = cosf(fAngle) * pChr->fVibration;
					pChr->fVibration *= CHR_ENEMY_00_VIBRATION_DAMPING;	// 振動の強さを減衰
				}
			}
			else
			{// 振動の強さが振動の強さの下限未満の時、振動位置を初期化
				pChr->vibrationPos = INITD3DXVECTOR3;
			}

			if (pChr->nCounterInvincible > 0)
			{// 無敵カウンターが0を上回っている時、
				pChr->nCounterInvincible--;	// 無敵カウンターを減算する
			}
			if (pChr->nCounterDamage > 0)
			{// ダメージカウンターが0を上回っている時、
				pChr->nCounterDamage--;	// ダメージカウンターを減算する
			}

			// 過去の位置/向きを保存
			pChr->posOld = pChr->pos;
			pChr->rotOld = pChr->rot;

			if ((pChr->bBossDependency) && (!g_aChr_enemy_00[pChrCtl->nSetBossIdx].bUse)) 
			{// ボス依存フラグが真 & ボスが使用されていない状態の時、
				DestroyChr_enemy_00(pChr);	// 破壊処理
			}
		}
		else 
		{// 管理状態が静的の時、
			pChr->nCounterDamage = 0;	// ダメージカウンターを初期化する
		}

		// EFF:影[00] の設定処理
		SetEff_shadow_00(pChr->pos, pType->fShadowRadius);
	}
}

//========================================
// DrawChr_enemy_00関数 - CHR:敵[00] の描画処理 -
//========================================
void DrawChr_enemy_00(void)
{
	Chr_enemy_00		*pChr				// CHR:敵[00] の情報のポインタ
						= g_aChr_enemy_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX			mtxSelf;			// 本体のマトリックス

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{// 使用されていない状態の時、
			continue;	// 繰り返し処理を折り返す
		}

		// 本体のワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxSelf);

		// 本体の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pChr->rot.y, pChr->rot.x, pChr->rot.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);
		{
			D3DXVECTOR3 setPos = pChr->pos + pChr->vibrationPos;

			// 本体の位置を反映
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);
		}
		// 本体のワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

		Chr_enemy_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
							= &g_aChr_enemy_00Type[pChr->nType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			Parts3D		*pParts		// 部品(3D)の情報のポインタ
						= &pChr->partsInfo.aParts[nCntParts];
			Parts3DType *pPartsType	// 部品(3D)の部品の種類毎の情報のポインタ
						= &pType->partsSet.aPartsType[nCntParts];
			D3DXMATRIX	mtxParent;	// 親マトリックス

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aBuffMatChr_enemy_00[pChr->nType][nCntParts]->GetBufferPointer();

			// 部品のワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMtxWorldChr_enemy_00[nCntParts]);

			// 部品の向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldChr_enemy_00[nCntParts], &g_aMtxWorldChr_enemy_00[nCntParts], &mtxRot);

			// 部品の位置を反映
			D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldChr_enemy_00[nCntParts], &g_aMtxWorldChr_enemy_00[nCntParts], &mtxTrans);

			if (pPartsType->nParent != -1)
			{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
				mtxParent = g_aMtxWorldChr_enemy_00[pPartsType->nParent];
			}
			else
			{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
				mtxParent = mtxSelf;
			}

			// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(
				&g_aMtxWorldChr_enemy_00[nCntParts],
				&g_aMtxWorldChr_enemy_00[nCntParts],
				&mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldChr_enemy_00[nCntParts]);

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_enemy_00[pChr->nType][nCntParts]; nCntMat++)
			{
				// マテリアルの設定
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
				{
					255,
					(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_ENEMY_00_DAMAGE_TIME))),
					(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_ENEMY_00_DAMAGE_TIME)) * !(pChr->state == CHR_ENEMY_00_STATE_PARALYSIS)),
					255
				});

				// テクスチャの設定
				pDevice->SetTexture(0, g_aTextureChr_enemy_00[pChr->nType][nCntParts][nCntMat]);

				// ポリゴン(パーツ)の描画
				g_aMeshChr_enemy_00[pChr->nType][nCntParts]->DrawSubset(nCntMat);
			}
		}
	}
}

//========================================
// SetChr_enemy_00関数 - CHR:敵[00] の設定処理 -
//========================================
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	Chr_enemy_00		*pChr	// CHR:敵[00] の情報のポインタ
						= g_aChr_enemy_00;
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[nType];

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pChr->pos =				// 位置を代入
		pChr->posOld = pos;		// 過去の位置を設定
		pChr->rot =				// 向きを代入
		pChr->rotOld = 			// 過去の向きを代入
		pChr->moveRot = rot;	// 移動向きを代入
		pChr->nType = nType;	// 種類を代入
		pChr->nParentIdx = -1;	// 親の番号を-1に設定

		// 設定時のパラメーター初期化処理
		InitParameterSetChr_enemy_00(pChr);

		// 繰り返し処理を抜ける
		break;
	}
}

//========================================
// SetBossChr_enemy_00関数 - CHR:敵[00] のボス設定処理 -
//========================================
void SetBossChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	Chr_enemy_00		*pChr		// CHR:敵[00] の情報のポインタ
						= g_aChr_enemy_00;
	Chr_enemy_00Type	*pType		// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[nType];
	Chr_enemy_00Control *pChrCtl	// CHR:敵[00] の管理情報のポインタ
						= &g_chr_enemy_00Control;

	// 設定したボスの番号を初期化
	pChrCtl->nSetBossIdx = -1;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pChr->pos =				// 位置を代入
		pChr->posOld = pos;		// 過去の位置を設定
		pChr->rot =				// 向きを代入
		pChr->rotOld = 			// 過去の向きを代入
		pChr->moveRot = rot;	// 移動向きを代入
		pChr->nType = nType;	// 種類を代入
		pChr->nParentIdx = -1;	// 親の番号を-1に設定

		// 設定時のパラメーター初期化処理
		InitParameterSetChr_enemy_00(pChr);

		// 設定したボスの番号を保存
		pChrCtl->nSetBossIdx = nCntChr;

		// 行動読み込み処理
		ActLoadChr_enemy_00(pChr);

		// 現在のモーション番号を保存
		pChr->nMotionTemp = pChr->nMotion;

		// モーションをアピールで設定
		pChr->nMotion = CHR_ENEMY_00_APPEAL_MOTION;
		SetMotion3D(
			&pChr->partsInfo,
			pChr->nMotion);

		// 子のアピールモーション設定
		ShildAppealMotionSetChr_enemy_00(pChr);

		// カメラ(3D)の振動を設定
		SetCamera3DVibration(CHR_ENEMY_00_BOSS_SET_CAMERA3D_VIBRATION);

		// 設定SEを再生
		PlaySound(CHR_ENEMY_00_BOSS_SET_SE);

		// 繰り返し処理を抜ける
		break;
	}
}

//========================================
// CollisionChr_enemy_00関数 - CHR:敵[00] との当たり判定処理 -
//========================================
int CollisionChr_enemy_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if (pObjCollision->bHit)
	{// 当たりフラグが真の時、
		return -1;	// 処理を終了する
	}

	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;
	int				nIndex	// CHR:敵[00] の返しの番号
					= -1;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{// 使用されていない状態の時、
			// 処理を折り返す
			continue;
		}

		Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
							= &g_aChr_enemy_00Type[pChr->nType];

		// 衝突判定に必要な情報
		CollisionCheck(
			vector,
			pObjCollision,
			myCollInfo,
			{
				&pChr->pos,
				pChr->pos,
				NULL,
				&pChr->rot,
				pChr->rot,
				pType->hitTestSet.aHitTest[pChr->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		if (pObjCollision->bHit)
		{// 当たりフラグが真の時、
			nIndex = nCntChr;	// 返しの番号を設定
			break;				// 繰り返し処理を抜ける
		}
	}

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// 番号を返す
	return nIndex;
}

//========================================
// DamageChr_enemy_00関数 - CHR:敵[00] のダメージ処理 -
//========================================
void DamageChr_enemy_00(int nIndex, int nDamage)
{
	Chr_enemy_00		*pChr	// CHR:敵[00] の情報のポインタ
						= &g_aChr_enemy_00[nIndex];
	Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->nCounterInvincible > 0) 
	{// 無敵カウンターが0を上回っている時、
		return;	// 処理を終了する
	}

	nDamage *= fDamageRand();	// 乱数補正
	SetChr_enemy_00Vibration(	// 振動を設定
		pChr,
		((float)nDamage / (float)pChr->nHPMax) * CHR_ENEMY_00_DAMAGE_VIBRATION_RATE);
	pChr->nHP -= nDamage;		// HPからダメージを減算
	pChr->nCounterDamage		// ダメージカウンターを設定
		= CHR_ENEMY_00_DAMAGE_TIME;
	// 子のダメージカウンター設定処理
	ChildSetDamageCounterChr_enemy_00(pChr);
	SetUi_damage_00(			// UI:ダメージ[00] の設定処理
		pChr->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_ENEMY);

	PlaySound(pType->damageSE);	// ダメージSEを再生

	if (pChr->nHP <= 0) 
	{// HPが0以下の時、
		DestroyChr_enemy_00(pChr);	// 破壊処理
	}
	else 
	{// HPが0を上回っている時、無敵カウンターを設定
		pChr->nCounterInvincible = CHR_ENEMY_00_INVINCIBLE_TIME;
	}
}

//========================================
// KnockBackChr_enemy_00関数 - CHR:敵[00] のノックバック処理 -
//========================================
void KnockBackChr_enemy_00(int nIndex, D3DXVECTOR3 rot, float fForce)
{
	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= &g_aChr_enemy_00[nIndex];

	if (pChr->nParentIdx != -1) 
	{// 親持ちの時、処理を終了する
		return;
	}

	// 力を加える
	pChr->force.x += sinf(rot.y) * fForce;
	pChr->force.z += cosf(rot.y) * fForce;
}

//========================================
// GiveStateChr_enemy_00関数 - CHR:敵[00] の状態付与処理 -
//========================================
void GiveStateChr_enemy_00(int nIndex, CHR_ENEMY_00_STATE state) 
{
	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= &g_aChr_enemy_00[nIndex];

	CHR_ENEMY_00_STATE stateTemp = pChr->state;	// 状態を保存

	pChr->state = state;	// 状態を設定

	switch (state)
	{
		//========== *** 通常 ***
	case CHR_ENEMY_00_STATE_NORMAL: {
		break;
	}
		//========== *** 麻痺 ***
	case CHR_ENEMY_00_STATE_PARALYSIS: {
		pChr->nCounterState = CHR_ENEMY_00_PARALYSIS_TIME;	// 時間を設定
		if (stateTemp != CHR_ENEMY_00_STATE_PARALYSIS) {
			PlaySound(CHR_ENEMY_00_PARALYSIS_SE);	// 麻痺SEを再生
		}
		break;
	}
	}
}

//========================================
// SetChr_enemy_00ControlState関数 - CHR:敵[00] の管理状態設定処理 -
//========================================
void SetChr_enemy_00ControlState(CHR_ENEMY_00_CONTROL_STATE state) 
{
	Chr_enemy_00Control *pChrCtl	// CHR:敵[00] の管理情報のポインタ
						= &g_chr_enemy_00Control;

	pChrCtl->state = state;	//	状態を代入
}

//========================================
// AllyChr_enemy_00Count関数 - CHR:敵[00] の味方カウント処理 -
//========================================
int AllyChr_enemy_00Count(void) 
{
	int				nAllyCount	// 味方カウント
					= 0;
	Chr_enemy_00	*pChr		// CHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報のポインタ
							= &g_aChr_enemy_00Type[pChr->nType];

		if ((pChr->bUse) && (pType->rank == CHR_ENEMY_00_RANK_ALLY))
		{// 使用されている & ランクが味方の時、
			nAllyCount++;	// 味方カウントを加算
		}
	}

	return nAllyCount;
}