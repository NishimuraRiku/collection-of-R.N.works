//========================================
// 
// OBJ:ステージ[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_stage_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "color.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"			// CHR:敵			[00]
#include "chr_player_00.h"			// CHR:プレイヤー	[00]
#include "eff_shadow_00.h"			// EFF:影			[00]
#include "obj_core_00.h"			// OBJ:コア			[00]
#include "obj_pedestal_00.h"		// OBJ:台座			[00]
#include "obj_stage_00.h"			// OBJ:ステージ		[00]
#include "obj_switch_00.h"			// OBJ:スイッチ		[00]
#include "md_game_00.h"				// MD :ゲーム		[00]
#include "md_title_00.h"			// MD :タイトル		[00]
#include "ui_attention-mark_00.h"	// UI :注意マーク	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// OBJ:ブロック[00] の種類毎の情報のデータファイルの相対パス
#define OBJ_STAGE_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_STAGE_00_TYPE_DATA.txt"

// OBJ:ステージ[00] の種類の数
#define OBJ_STAGE_00_TYPE_NUM	(1)

// OBJ:ステージ[00] の初期状態
#define OBJ_STAGE_00_INIT_STATE	(OBJ_STAGE_00_STATE_INTERVAL)

// OBJ:ステージ[00] のテキストの開始位置
// OBJ:ステージ[00] のテキストの終了位置
// OBJ:ステージ[00] のテキストの現れるのにかかる時間
// OBJ:ステージ[00] のテキストの消えるのにかかる時間
// OBJ:ステージ[00] のテキストのクリア時の色
#define OBJ_STAGE_00_TEXT_START_POS			D3DXVECTOR3(SCREEN_WIDTH*0.5f,(SCREEN_HEIGHT*0.5f)+160.0f,0.0f)
#define OBJ_STAGE_00_TEXT_END_POS			D3DXVECTOR3(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f,0.0f)
#define OBJ_STAGE_00_TEXT_SCALE				(2.0f)
#define OBJ_STAGE_00_TEXT_APPEAR_TIME		(40)
#define OBJ_STAGE_00_TEXT_DISAPPEAR_TIME	(80)
#define OBJ_STAGE_00_TEXT_CLEAR_COLOR		Color{243,191,63,255}

// OBJ:ステージ[00] の設定位置との当たり判定番号
#define OBJ_STAGE_00_HITTEST_WITH_SETPOS	(1)

// OBJ:ステージ[00] の変色にかかる時間
// OBJ:ステージ[00] の変色する部品
#define OBJ_STAGE_00_DISCOLORATION_TIME		(60)
#define OBJ_STAGE_00_DISCOLORATION_PARTS	(1)

// OBJ:ステージ[00] のタイムボーナスカウンターの初期値
// OBJ:ステージ[00] のタイムボーナスカウンターの分割値
// OBJ:ステージ[00] のタイムボーナスカウンターのスコア倍率
#define OBJ_STAGE_00_TIME_BONUS_COUNTER_START		(60*60)
#define OBJ_STAGE_00_TIME_BONUS_COUNTER_DIVISION	(60)
#define OBJ_STAGE_00_TIME_BONUS_COUNTER_SCORE_RATE	(50)

// OBJ:ステージ[00] のタイムボーナステキストの開始位置
// OBJ:ステージ[00] のタイムボーナステキストの終了位置
// OBJ:ステージ[00] のタイムボーナステキストの出現時間
// OBJ:ステージ[00] のタイムボーナステキストの移動にかかる時間
// OBJ:ステージ[00] のタイムボーナステキストの消え始める時間
// OBJ:ステージ[00] のタイムボーナステキストの色
// OBJ:ステージ[00] のタイムボーナステキストの点滅色
#define OBJ_STAGE_00_TIME_BONUS_TEXT_START_POS			D3DXVECTOR3(SCREEN_WIDTH+PIXEL* 61,PIXEL*35,0.0f)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_END_POS			D3DXVECTOR3(SCREEN_WIDTH+PIXEL*-61,PIXEL*35,0.0f)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME			(120)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_MOVE_TIME			(40)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME	(80)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_TIME			(2)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_COLOR				Color{255,255,255,255}
#define OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_COLOR		Color{255,119,183,255}

// OBJ:ステージ[00] のネクストウェーブテキストの位置
// OBJ:ステージ[00] のネクストウェーブテキストの色
// OBJ:ステージ[00] のネクストウェーブテキストの注意時の色
// OBJ:ステージ[00] のネクストウェーブテキストの不透明度の下限
// OBJ:ステージ[00] のネクストウェーブテキストの不透明度の上限
// OBJ:ステージ[00] のネクストウェーブテキストの拡大倍率の下限
// OBJ:ステージ[00] のネクストウェーブテキストの拡大倍率の上限
#define OBJ_STAGE_00_NEXTWAVE_POS					D3DXVECTOR3(SCREEN_CENTER_X,PIXEL*48,0.0f)
#define OBJ_STAGE_00_NEXTWAVE_TEXT_COLOR			Color{251,80,42,255}
#define OBJ_STAGE_00_NEXTWAVE_TEXT_WARNING_COLOR	Color{191,0,191,255}
#define OBJ_STAGE_00_NEXTWAVE_ALPHA_MIN				(0.75f)
#define OBJ_STAGE_00_NEXTWAVE_ALPHA_MAX				(1.0f)
#define OBJ_STAGE_00_NEXTWAVE_SCALE_MIN				(0.95f)
#define OBJ_STAGE_00_NEXTWAVE_SCALE_MAX				(1.0f)

// OBJ:ステージ[00] のカウントSE
#define OBJ_STAGE_00_COUNT_SE	(SOUND_LABEL_SE_COUNT_000)

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ステージ[00] のパラメーターの初期化処理
void InitParameterObj_stage_00(void);

// OBJ:ステージ[00] のゲート設定情報の読み込み処理
// OBJ:ステージ[00] の注意マーク設定情報の読み込み処理
// OBJ:ステージ[00] の台座設定情報の読み込み処理
// OBJ:ステージ[00] の品揃え設定情報の読み込み処理
// OBJ:ステージ[00] のウェーブ設定情報の読み込み処理
void LoadGateSetObj_stage_00(FILE *pFile, Obj_stage_00GateSet *pGateSet);
void LoadAttentionMarkSetObj_stage_00(FILE *pFile, Obj_stage_00AttentionMarkSet *pAttentionMarkSet);
void LoadPedestalSetObj_stage_00(FILE *pFile, Obj_stage_00PedestalSet *pPedestalSet);
void LoadItemsSetObj_stage_00(FILE *pFile, Obj_stage_00ItemsSet *pItemsSet, Obj_stage_00Type *pObjType);
void LoadWaveSetObj_stage_00(FILE *pFile, Obj_stage_00WaveSet *pWaveSet);

// OBJ:ステージ[00] のセッティング処理
void SettingObj_stage_00(void);

// OBJ:ステージ[00] の状態に応じた開始処理
// OBJ:ステージ[00] の状態に応じた更新処理
// OBJ:ステージ[00] の状態に応じた終了処理
void StartStateObj_stage_00(void);
void UpdateStateObj_stage_00(void);
void EndStateObj_stage_00(void);

// OBJ:ステージ[00] の警告設定処理
void SetWarningObj_stage_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureObj_stage_00	// OBJ:ステージ[00] のテクスチャへのポインタ
					[OBJ_STAGE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshObj_stage_00		// OBJ:ステージ[00] のメッシュ(頂点情報)へのポインタ
					[OBJ_STAGE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
LPD3DXBUFFER		g_pBuffMatObj_stage_00	// OBJ:ステージ[00] のマテリアルへのポインタ
					= NULL;
DWORD				g_aNumMatObj_stage_00	// OBJ:ステージ[00] のマテリアルの数
					[OBJ_STAGE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
D3DXMATRIX			g_aMtxWorldObj_stage_00	// OBJ:ステージ[00] のワールドマトリックス
					[PARTS_3D_MAX];
Obj_stage_00		g_obj_stage_00;			// OBJ:ステージ[00] の情報
Obj_stage_00Type	g_aObj_stage_00Type		// OBJ:ステージ[00] の種類毎の情報
					[OBJ_STAGE_00_TYPE_NUM];

//========== *** OBJ:ステージ[00] の情報を取得 ***
Obj_stage_00 *GetObj_stage_00(void) 
{
	return &g_obj_stage_00;
}

//========== *** OBJ:ステージ[00] の種類毎の情報を取得 ***
Obj_stage_00Type *GetObj_stage_00Type(void)
{
	return g_aObj_stage_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_stage_00関数 - OBJ:ステージ[00] のパラメーターの初期化処理 -
//========================================
void InitParameterObj_stage_00(void) 
{
	Obj_stage_00	*pObj	// OBJ:ステージ[00] の情報のポインタ
					= &g_obj_stage_00;

	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pObj->nType					= 0;								// 種類
	pObj->hitTestInfo			= {};								// 当たり判定の管理情報
	pObj->nCounter				= 0;								// カウンター
	pObj->state					= OBJ_STAGE_00_STATE_NONE;			// 状態
	pObj->nWave					= 0;								// ウェーブ番号
	pObj->nWaveOld				= 0;								// 過去のウェーブ番号
	pObj->nCntWaveMain			= 0;								// ウェーブカウント(メイン)
	pObj->nCntWaveSub			= 0;								// ウェーブカウント(サブ)
	pObj->nKill					= 0;								// キル数
	pObj->textState				= OBJ_STAGE_00_TEXT_STATE_HIDE;		// テキストの状態
	pObj->nCounterText			= 0;								// テキストカウンター
	pObj->nCounterDiscoloration = 0;								// 変色カウンター
	pObj->groundCol				= INITCOLOR;						// 地面の色
	pObj->partsInfo				= {};								// 部品管理
	pObj->setBoss				= {};								// ボス設定情報
	pObj->nTimeBonus			= 0;								// タイムボーナス
	pObj->nCounterTimeBonus		= 0;								// タイムボーナスカウンター
	pObj->nCounterTimeBonusText = 0;								// タイムボーナステキストカウンター
}

//========================================
// LoadGateSetObj_stage_00関数 - OBJ:ステージ[00] のゲート設定情報の読み込み処理 -
//========================================
void LoadGateSetObj_stage_00(FILE *pFile, Obj_stage_00GateSet *pGateSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];

	// ゲートの数を初期化
	pGateSet->nGateNum = 0;

	// 当たり判定設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "GATESET_END"))	{ break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "GATE"))			{
			Obj_stage_00Gate	*pGate	// ゲート情報のポインタ
								= &pGateSet->aGate[pGateSet->nGateNum];

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "GATE_END")) {
					pGateSet->nGateNum++;	// ゲートの数を加算
					break;					// 読み込みを終了
				}
				else if (!strcmp(aDataSearch, "POS:")) {	// 位置
					fscanf(pFile, "%f", &pGate->pos.x);		// X
					fscanf(pFile, "%f", &pGate->pos.y);		// Y
					fscanf(pFile, "%f", &pGate->pos.z);		// Z
				}
				else if (!strcmp(aDataSearch, "BOSS_POINT:")) {	// ボス設定位置
					fscanf(pFile, "%f", &pGate->bossPoint.x);	// X
					fscanf(pFile, "%f", &pGate->bossPoint.y);	// Y
					fscanf(pFile, "%f", &pGate->bossPoint.z);	// Z
				}
				else if (!strcmp(aDataSearch, "SETROT:")) {	// 設定向き
					fscanf(pFile, "%f", &pGate->setRot.x);	// X
					fscanf(pFile, "%f", &pGate->setRot.y);	// Y
					fscanf(pFile, "%f", &pGate->setRot.z);	// Z
					pGate->setRot *= D3DX_PI;				// 円周率補正
				}
				else if (!strcmp(aDataSearch, "WIDTH:")) { fscanf(pFile, "%f", &pGate->fWidth); }	// 幅
				else if (!strcmp(aDataSearch, "DEPTH:")) { fscanf(pFile, "%f", &pGate->fDepth); }	// 奥行き
			}
		}
	}
}

//========================================
// LoadAttentionMarkSetObj_stage_00関数 - OBJ:ステージ[00] の注意マーク設定情報の読み込み処理 -
//========================================
void LoadAttentionMarkSetObj_stage_00(FILE *pFile, Obj_stage_00AttentionMarkSet *pAttentionMarkSet)
{
	char						aDataSearch		// データ検索用
								[TXT_MAX];
	Obj_stage_00AttentionMark	*pAttentionMark	// 注意マークの情報のポインタ
								= pAttentionMarkSet->aAttentionMark;

	// 当たり判定設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "ATTENTIONMARKSET_END")) { break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "ATTENTIONMARK")) {
			
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "ATTENTIONMARK_END")) {
					pAttentionMark++;	// 注意マークの情報のポインタを進める
					break;				// 読み込みを終了
				}
				else if (!strcmp(aDataSearch, "POS:")) {			// 位置
					fscanf(pFile, "%f", &pAttentionMark->pos.x);	// X
					fscanf(pFile, "%f", &pAttentionMark->pos.y);	// Y
					fscanf(pFile, "%f", &pAttentionMark->pos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "ROT:")) {			// 向き
					fscanf(pFile, "%f", &pAttentionMark->rot.x);	// X
					fscanf(pFile, "%f", &pAttentionMark->rot.y);	// Y
					fscanf(pFile, "%f", &pAttentionMark->rot.z);	// Z
					pAttentionMark->rot.x *= D3DX_PI;				// 円周率補正X
					pAttentionMark->rot.y *= D3DX_PI;				// 円周率補正Y
					pAttentionMark->rot.z *= D3DX_PI;				// 円周率補正Z
				}
			}
		}
	}
}

//========================================
// LoadPedestalSetObj_stage_00関数 - OBJ:ステージ[00] の台座設定情報の読み込み処理 -
//========================================
void LoadPedestalSetObj_stage_00(FILE *pFile, Obj_stage_00PedestalSet *pPedestalSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];

	// 台座の数を初期化
	pPedestalSet->nPedestalNum = 0;

	// 当たり判定設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "PEDESTALSET_END"))	{ break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "PEDESTAL"))			{
			Obj_stage_00Pedestal	*pPedestal	// 台座情報のポインタ
									= &pPedestalSet->aPedestal[pPedestalSet->nPedestalNum];

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "PEDESTAL_END")) {
					pPedestalSet->nPedestalNum++;	// 台座の数を加算
					break;							// 読み込みを終了
				}
				else if (!strcmp(aDataSearch, "POS:"))	{	// 位置
					fscanf(pFile, "%f", &pPedestal->pos.x);	// X
					fscanf(pFile, "%f", &pPedestal->pos.y);	// Y
					fscanf(pFile, "%f", &pPedestal->pos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "TYPE:"))	{ fscanf(pFile, "%f", &pPedestal->nType); }	// 種類
			}
		}
	}
}

//========================================
// LoadWaveSetObj_stage_00関数 - OBJ:ステージ[00] のウェーブ設定情報の読み込み処理 -
//========================================
void LoadWaveSetObj_stage_00(FILE *pFile, Obj_stage_00WaveSet *pWaveSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];

	// ウェーブの数を初期化
	pWaveSet->nWaveNum = 0;

	// ウェーブ設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "WAVESET_END"))	{ break; }										// 読み込みを終了
		else if (!strcmp(aDataSearch, "LOOP_POINT:"))	{ fscanf(pFile, "%d", &pWaveSet->nLoopPoint); }	// ループ地点
		else if (!strcmp(aDataSearch, "WAVE"))			{
			Obj_stage_00Wave	*pWave	// ウェーブ情報のポインタ
								= &pWaveSet->aWave[pWaveSet->nWaveNum];

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "WAVE_END")) {
					pWaveSet->nWaveNum++;	// ウェーブの数を加算
					break;					// 読み込みを終了
				}
				else if (!strcmp(aDataSearch, "INTERVAL_TIME:")) {
					fscanf(pFile, "%d", &pWave->nIntervalTime);	// インターバル時間
					pWave->nIntervalTime *= 60;					// 秒単位に修正
				}
				else if (!strcmp(aDataSearch, "INTERVAL_BGM:"))			{ fscanf(pFile, "%d", &pWave->intervalBGM); }		// インターバルBGM
				else if (!strcmp(aDataSearch, "RAID_BGM:"))				{ fscanf(pFile, "%d", &pWave->raidBGM); }			// 襲撃BGM
				else if (!strcmp(aDataSearch, "INTERVAL_GROUND_COLOR:")){ LoadColor(pFile, &pWave->intervalGroundColor); }	// インターバル時の地面の色
				else if (!strcmp(aDataSearch, "INTERVAL_SKY_COLOR:"))	{ LoadColor(pFile, &pWave->intervalSkyColor); }		// インターバル時の空の色
				else if (!strcmp(aDataSearch, "RAID_GROUND_COLOR:"))	{ LoadColor(pFile, &pWave->raidGroundColor); }		// 襲撃時の地面の色
				else if (!strcmp(aDataSearch, "RAID_SKY_COLOR:"))		{ LoadColor(pFile, &pWave->raidSkyColor); }			// 襲撃時の空の色
				else if (!strcmp(aDataSearch, "WARNING_TEXT:"))			{ fscanf(pFile, "%d", &pWave->bWarningText); }		// 注意テキストフラグ
				else if (!strcmp(aDataSearch, "MAIN_COUNT:"))			{ fscanf(pFile, "%d", &pWave->bMainCount); }		// メインカウントフラグ
				else if (!strcmp(aDataSearch, "ENEMYSET"))				{
					Obj_stage_00EnemySet	*pEnemySet	// 敵設定情報のポインタ
											= pWave->aEnemySet;

					pWave->nEnemyNum = 0;	// 敵の数を初期化

					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if		(!strcmp(aDataSearch, "ENEMYSET_END"))	{ break; }	// 読み込みを終了
						else if (!strcmp(aDataSearch, "SET:"))			{	// 設定情報
							fscanf(pFile, "%d", &pEnemySet->nTime);			// 時間
							pEnemySet->nTime *= 60;							// 秒単位に修正
							fscanf(pFile, "%d", &pEnemySet->nGate);			// ゲート
							fscanf(pFile, "%d", &pEnemySet->nType);			// 種類
							pEnemySet++;									// 敵設定情報を加算
							pWave->nEnemyNum++;								// 敵の数を加算
						}
					}
				}
			}
		}
	}
}

//========================================
// LoadItemsSetObj_stage_00関数 - OBJ:ステージ[00] の品揃え設定情報の読み込み処理 -
//========================================
void LoadItemsSetObj_stage_00(FILE *pFile, Obj_stage_00ItemsSet *pItemsSet, Obj_stage_00Type *pObjType)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];

	// 品揃えの数を初期化
	pItemsSet->nItemsNum = 0;

	// 品揃え設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "ITEMSSET_END"))	{ break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "ITEMS"))			{
			Obj_stage_00Items	*pItems	// 品揃え情報のポインタ
								= &pItemsSet->aItems[pItemsSet->nItemsNum];

			// 商品番号を読み込み
			for (int nCntPedestal = 0; nCntPedestal < pObjType->pedestalSet.nPedestalNum; nCntPedestal++)
			{
				fscanf(pFile, "%d", &pItems->aItem[nCntPedestal]);
			}

			pItemsSet->nItemsNum++;	// 品揃え数を加算
		}
	}
}

//========================================
// SettingObj_stage_00関数 - OBJ:ステージ[00] のセッティング処理 -
//========================================
void SettingObj_stage_00(void) 
{
	Obj_stage_00Type	*pObjType	// OBJ:ステージ[00] の種類毎の情報
						= g_aObj_stage_00Type;

	for (int nCntPedestal = 0; nCntPedestal < pObjType->pedestalSet.nPedestalNum; nCntPedestal++) 
	{
		// OBJ:台座[00] の設定処理
		SetObj_pedestal_00(
			pObjType->pedestalSet.aPedestal[nCntPedestal].pos,
			pObjType->pedestalSet.aPedestal[nCntPedestal].nType);
	}

	for (int nCntGate = 0; nCntGate < pObjType->gateSet.nGateNum; nCntGate++)
	{
		// UI:注意マーク[00] の設定処理
		SetUi_attentionMark_00(
			pObjType->attentionMarkSet.aAttentionMark[nCntGate].pos,
			pObjType->attentionMarkSet.aAttentionMark[nCntGate].rot);
	}

	// OBJ:コア[00] の位置を代入
	GetObj_core_00()->pos = pObjType->coreSetPos;

	// OBJ:スイッチ[00] の位置を代入
	GetObj_switch_00()->pos = pObjType->switchSetPos;
}

//========================================
// SetStateObj_stage_00関数 - OBJ:ステージ[00] の状態設定処理 -
//========================================
void SetStateObj_stage_00(OBJ_STAGE_00_STATE state) 
{
	Obj_stage_00	*pObj	// OBJ:ステージ[00] の情報のポインタ
					= &g_obj_stage_00;

	// 状態に応じた終了処理
	EndStateObj_stage_00();

	// 状態を代入
	pObj->state = state;

	// 状態に応じた開始処理
	StartStateObj_stage_00();
}

//========================================
// StartStateObj_stage_00関数 - OBJ:ステージ[00] の状態に応じた開始処理 -
//========================================
void StartStateObj_stage_00(void)
{
	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];

	switch (pObj->state)
	{
	case /*/ インターバル /*/OBJ_STAGE_00_STATE_INTERVAL: {
		if ((pObj->nCntWaveMain > 0) ||
			(pObj->nCntWaveSub > 0))
		{// ウェーブカウント(メインorサブ)が0を上回っている時、
			// テキストの状態を現れ中にする
			pObj->textState = OBJ_STAGE_00_TEXT_STATE_APPEAR;
		}
		pObj->nCounterDiscoloration = 0;	// 変色カウンターを初期化
		pObj->nCounterTimeBonusText = 0;	// タイムボーナステキストカウンターを初期化

		// インターバルBGMを再生
		PlaySound(pType->waveSet.aWave[pObj->nWave].intervalBGM);

		break;
	}
	case /*/ 襲撃 /*/OBJ_STAGE_00_STATE_RAID: {
		pObj->nCounter = 0;									// カウンターを初期化
		pObj->nKill = 0;									// キル数を初期化
		pObj->textState = OBJ_STAGE_00_TEXT_STATE_APPEAR;	// テキストの状態を現れ中にする
		pObj->nCounterText = 0;								// テキストカウンターを初期化
		pObj->nCounterDiscoloration = 0;					// 変色カウンターを初期化
		pObj->nCounterTimeBonus								// タイムボーナスカウンターを設定
			= OBJ_STAGE_00_TIME_BONUS_COUNTER_START;

		// 襲撃BGMを再生
		PlaySound(pType->waveSet.aWave[pObj->nWave].raidBGM);

		break;
	}
	case /*/ クリア /*/OBJ_STAGE_00_STATE_CLEAR: {
		StopSoundType(SOUND_TYPE_BGM);		// BGMを停止
		pObj->nCounterDiscoloration = 0;	// 変色カウンターを初期化
		pObj->nTimeBonus = 0;				// タイムボーナス取得
		break;
	}
	case /*/ 停止 /*/OBJ_STAGE_00_STATE_STOP: {

		break;
	}
	}
}

//========================================
// EndStateObj_stage_00関数 - OBJ:ステージ[00] の状態に応じた終了処理 -
//========================================
void EndStateObj_stage_00(void)
{
	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];

	switch (pObj->state)
	{
	case /*/ インターバル /*/OBJ_STAGE_00_STATE_INTERVAL: {

		break;
	}
	case /*/ 襲撃 /*/OBJ_STAGE_00_STATE_RAID: {
		pObj->nCounter = 0;									// カウンターを初期化
		GetUi_attentionMark_00Control()->nCounterBrink = 0;	// UI:注意マーク[00] の点滅カウンターを初期化
		pObj->nCounterText = 0;								// テキストカウンターを初期化

		if (GetObj_core_00()->nHP > 0)
		{// OBJ:コア[00] のHPが0を上回っている時、
			// タイムボーナス取得
			pObj->nTimeBonus = (int)(pObj->nCounterTimeBonus / OBJ_STAGE_00_TIME_BONUS_COUNTER_DIVISION) * OBJ_STAGE_00_TIME_BONUS_COUNTER_SCORE_RATE;

			// タイムボーナスをスコアに加算
			GetChr_player_00()->nScore += pObj->nTimeBonus;
		}
		else 
		{// OBJ:コア[00] のHPが0以下の時、
			// タイムボーナスを初期化
			pObj->nTimeBonus = 0;
		}

		break;
	}
	case /*/ クリア /*/OBJ_STAGE_00_STATE_CLEAR: {
		
		break;
	}
	case /*/ 停止 /*/OBJ_STAGE_00_STATE_STOP: {

		break;
	}
	}
}

//========================================
// UpdateStateObj_stage_00関数 - OBJ:ステージ[00] の状態に応じた更新処理 -
//========================================
void UpdateStateObj_stage_00(void) 
{
	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];
	char				aString	// 文字列
						[TXT_MAX];
	Color				setCol;	// 設定色

	switch (pObj->state)
	{
	case /*/ インターバル /*/OBJ_STAGE_00_STATE_INTERVAL: {
		Obj_stage_00Wave	*pWave	// ウェーブの情報のポインタ
							= &pType->waveSet.aWave[pObj->nWave];

		if (GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL)
		{// MD:ゲーム画面[00] の状態が通常の時、
			if (++pObj->nCounter >= pWave->nIntervalTime)
			{// カウンターを加算した結果インターバルタイムに達した時、
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_RAID);	// 状態を襲撃に設定
			}

			if (pObj->nCounter % 60 == 0) 
			{// カウントのタイミングに達した時、
				float fRate = (float)pObj->nCounter / (float)pWave->nIntervalTime;
				SoundVolumeChange(OBJ_STAGE_00_COUNT_SE, fRate * fRate);
				PlaySound(OBJ_STAGE_00_COUNT_SE);	// カウントSEを再生
			}

			if (pObj->textState != OBJ_STAGE_00_TEXT_STATE_HIDE)
			{// テキストが消えている状態でない時、
				sprintf(aString, "WAVE CLEAR");			// 文字列を設定
				setCol = OBJ_STAGE_00_TEXT_CLEAR_COLOR;	// 設定色を設定
			}
		}
	}
		break;
	case /*/ 襲撃 /*/OBJ_STAGE_00_STATE_RAID: {
		Obj_stage_00Wave	*pWave	// ウェーブの情報のポインタ
							= &pType->waveSet.aWave[pObj->nWave];

		for (int nCntEnemySet = 0; nCntEnemySet < pWave->nEnemyNum; nCntEnemySet++)
		{
			if (pObj->nCounter == pWave->aEnemySet[nCntEnemySet].nTime) 
			{// カウンターと敵の設定時間が一致している時、
				Obj_stage_00Gate	*pGate =	// ゲート情報のポインタ
									&pType->gateSet.aGate[pWave->aEnemySet[nCntEnemySet].nGate];
				D3DXVECTOR3			setPos =	// 設定位置
									pGate->pos;

				switch (GetChr_enemy_00Type()[pWave->aEnemySet[nCntEnemySet].nType].rank)
				{
				case /*/ 低位 /*/CHR_ENEMY_00_RANK_LOWER:
					setPos = pGate->pos;	// 設定位置をゲートの位置に設定

					// 設定位置に乱数を適用
					setPos.x = setPos.x - (pGate->fWidth * 0.5f) + fRand(pGate->fWidth);
					setPos.z = setPos.z - (pGate->fDepth * 0.5f) + fRand(pGate->fDepth);

					// CHR:敵[00] の設定処理
					SetChr_enemy_00(setPos, pGate->setRot, pWave->aEnemySet[nCntEnemySet].nType);
					break;
				case /*/ ボス /*/CHR_ENEMY_00_RANK_BOSS:
					// ボス設定情報をそれぞれ代入(予約)
					pObj->setBoss.pos	= pGate->bossPoint;
					pObj->setBoss.rot	= pGate->setRot;
					pObj->setBoss.nType = pWave->aEnemySet[nCntEnemySet].nType;

					// MD:ゲーム画面[00] の状態をボス出現ムービー(視点移動)に設定
					SetMd_game_00State(MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE);
					break;
				}
			}
		}

		pObj->nCounter++;	// カウンターを加算

		if ((pObj->nKill >= pWave->nEnemyNum) && (AllyChr_enemy_00Count() == 0))
		{// キル数が敵の数に達した & CHR:敵[00] の味方カウントが0の時、
			// 過去のウェーブを保存
			pObj->nWaveOld = pObj->nWave;

			if (++pObj->nWave >= pType->waveSet.nWaveNum) 
			{// ウェーブ番号を加算した結果ウェーブ数以上の時、
				pObj->nWave--;	// ウェーブ番号を減算
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_CLEAR);	// 状態をクリアに設定
				SetMd_game_00State(MD_GAME_00_STATE_CLEAR);		// MD:ゲーム画面[00] の状態をクリアに設定
			}
			else 
			{// ウェーブ番号を加算した結果ウェーブ数未満の時、
				SetWarningObj_stage_00();		// 警告設定処理
				ItemsChangeObj_pedestal_00();	// 品揃えを変更する

				if (GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bMainCount)
				{// 現在のウェーブのメインカウントフラグが真の時、
					pObj->nCntWaveMain++;	// ウェーブカウント(メイン)を加算
					pObj->nCntWaveSub = 0;	// ウェーブカウント(サブ)を初期化
				}
				else
				{// 現在のウェーブのメインカウントフラグが偽の時、
					// ウェーブカウント(サブ)を加算
					pObj->nCntWaveSub++;
				}

				// 状態をインターバルに設定
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_INTERVAL);
			}
		}

		if (pObj->textState != OBJ_STAGE_00_TEXT_STATE_HIDE)
		{// テキストが消えている状態でない時、
			sprintf(aString, "WAVE %d-%d",	// 文字列を設定
				pObj->nCntWaveMain + 1,
				pObj->nCntWaveSub + 1);
			setCol = { 255,255,255,255 };	// 設定色を設定
		}

		pObj->nCounterTimeBonus--;	// タイムボーナスカウンターを減算制御
		IntControl(&pObj->nCounterTimeBonus, INT_MAX, 0);
	}
		break;
	case /*/ クリア /*/OBJ_STAGE_00_STATE_CLEAR: {

		break;
	}
	case /*/ 停止 /*/OBJ_STAGE_00_STATE_STOP: {
		
		break;
	}
	}

	if (pObj->textState != OBJ_STAGE_00_TEXT_STATE_HIDE)
	{// テキストが消えている状態でない時、
		float		fAlpha;	// 不透明度
		D3DXVECTOR3 setPos;	// 設定位置

		switch (pObj->textState)
		{
		case /*/ 現れ中 /*/OBJ_STAGE_00_TEXT_STATE_APPEAR: {
			float fRate	// 割合
				= (float)pObj->nCounterText / (float)OBJ_STAGE_00_TEXT_APPEAR_TIME;

			fAlpha = fRate;	// 不透明度を設定
			setPos =		// 設定位置を設定
				(OBJ_STAGE_00_TEXT_START_POS * (1.0f - fRate)) +
				(OBJ_STAGE_00_TEXT_END_POS * fRate);

			if (++pObj->nCounterText >= OBJ_STAGE_00_TEXT_APPEAR_TIME)
			{// テキストカウンターを加算した結果現れるのにかかる時間に達した時、
				pObj->textState = OBJ_STAGE_00_TEXT_STATE_DISAPPEAR;	// テキストの状態を消え中にする
				pObj->nCounterText = 0;									// テキストカウンターを初期化
			}
		}
		break;
		case /*/ 消え中 /*/OBJ_STAGE_00_TEXT_STATE_DISAPPEAR: {
			float fRate	// 割合
				= (float)pObj->nCounterText / (float)OBJ_STAGE_00_TEXT_DISAPPEAR_TIME;

			fAlpha = 1.0f - fRate;				// 不透明度を設定
			setPos = OBJ_STAGE_00_TEXT_END_POS;	// 設定位置を設定

			if (++pObj->nCounterText >= OBJ_STAGE_00_TEXT_DISAPPEAR_TIME)
			{// テキストカウンターを加算した結果消えるのにかかる時間に達した時、
				pObj->textState = OBJ_STAGE_00_TEXT_STATE_HIDE;	// テキストの状態を消えている状態にする
				pObj->nCounterText = 0;							// テキストカウンターを初期化
			}
		}
		break;
		}

		// ウェーブテキストの設定
		{
			setCol.a = 255 * fAlpha;
			SetText2D(
				aString,
				FONT_003,
				DISPLAY_CENTER,
				setPos,
				setCol,
				OBJ_STAGE_00_TEXT_SCALE, OBJ_STAGE_00_TEXT_SCALE);
		}
	}

	if (pObj->nTimeBonus > 0)
	{// タイムボーナスが0を上回っている時、
		pObj->nCounterTimeBonusText++;	// タイムボーナステキストカウンターを加算制御
		IntControl(&pObj->nCounterTimeBonusText, OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME, 0);

		{
			float fMoveRate =
				(float)IntControlReturn(
					pObj->nCounterTimeBonusText,
					OBJ_STAGE_00_TIME_BONUS_TEXT_MOVE_TIME, 0) /
					(float)OBJ_STAGE_00_TIME_BONUS_TEXT_MOVE_TIME;
			float fClearRate =
				(float)(IntControlReturn(pObj->nCounterTimeBonusText,
					OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME,
					OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME) -
					OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME) /
					(float)(OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME - OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME);
			setCol = (pObj->nCounterTimeBonusText % (OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_TIME * 2) < OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_TIME) ?
				OBJ_STAGE_00_TIME_BONUS_TEXT_COLOR : OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_COLOR;
			setCol.a *= (1.0f - fClearRate) * fMoveRate;
			sprintf(aString, "TIME BONUS +%d", pObj->nTimeBonus);
			SetText2D(
				aString,
				FONT_002,
				DISPLAY_RIGHT,
				(OBJ_STAGE_00_TIME_BONUS_TEXT_START_POS * (1.0f - fMoveRate)) +
				(OBJ_STAGE_00_TIME_BONUS_TEXT_END_POS * fMoveRate),
				setCol,
				1.0f, 1.0f);
		}
	}
}

//========================================
// SetWarningObj_stage_00関数 - OBJ:ステージ[00] の警告設定処理 -
//========================================
void SetWarningObj_stage_00(void) 
{
	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];
	Obj_stage_00Wave	*pWave	// ウェーブの情報のポインタ
						= &pType->waveSet.aWave[pObj->nWave];

	bool	aWarning	// 警告フラグ
			[OBJ_STAGE_00_GATE_MAX]
			= { false };

	for (int nCntEnemySet = 0; nCntEnemySet < pWave->nEnemyNum; nCntEnemySet++)
	{
		aWarning[pWave->aEnemySet[nCntEnemySet].nGate] = true;	// 警告フラグを真にする
	}

	// UI:注意マークの可視フラグに警告フラグを反映する
	for (int nCntGate = 0; nCntGate < pType->gateSet.nGateNum; nCntGate++)
	{
		GetUi_attentionMark_00()[nCntGate].bShow = aWarning[nCntGate];
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_stage_00関数 - OBJ:ステージ[00] の読み込み処理 -
//========================================
void LoadObj_stage_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Obj_stage_00Type	*pObjType	// OBJ:ステージ[00] の種類毎の情報
						= g_aObj_stage_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(OBJ_STAGE_00_TYPE_DATA_FILE_PATH, "r");

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

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; } // 読み込みを終了
				else if (!strcmp(aDataSearch, "CORE_SET_POS:"))	{	// 位置
					fscanf(pFile, "%f", &pObjType->coreSetPos.x);	// X
					fscanf(pFile, "%f", &pObjType->coreSetPos.y);	// Y
					fscanf(pFile, "%f", &pObjType->coreSetPos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "SWITCH_SET_POS:")) {	// 位置
					fscanf(pFile, "%f", &pObjType->switchSetPos.x);	// X
					fscanf(pFile, "%f", &pObjType->switchSetPos.y);	// Y
					fscanf(pFile, "%f", &pObjType->switchSetPos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "GATESET"))			{ LoadGateSetObj_stage_00(pFile, &pObjType->gateSet); }						// ゲート設定情報
				else if (!strcmp(aDataSearch, "ATTENTIONMARKSET"))	{ LoadAttentionMarkSetObj_stage_00(pFile, &pObjType->attentionMarkSet); }	// 注意マーク設定情報
				else if (!strcmp(aDataSearch, "PEDESTALSET"))		{ LoadPedestalSetObj_stage_00(pFile, &pObjType->pedestalSet); }				// 台座設定情報
				else if (!strcmp(aDataSearch, "ITEMSSET"))			{ LoadItemsSetObj_stage_00(pFile, &pObjType->itemsSet, pObjType); }			// 品揃え設定情報
				else if (!strcmp(aDataSearch, "WAVESET"))			{ LoadWaveSetObj_stage_00(pFile, &pObjType->waveSet); }						// ウェーブ設定情報
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }							// 当たり判定設定情報
				else if (!strcmp(aDataSearch, "PARTSSET"))			{ LoadParts3DSet(pFile, &pObjType->partsSet); }								// 部品設定情報
			}
		}
	}
}

//========================================
// InitObj_stage_00関数 - OBJ:ステージ[00] の初期化処理 -
//========================================
void InitObj_stage_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ
	
	for (int nCntType = 0; nCntType < OBJ_STAGE_00_TYPE_NUM; nCntType++)
	{
		Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
							= &g_aObj_stage_00Type[nCntType];

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
				&g_pBuffMatObj_stage_00,
				NULL,
				&g_aNumMatObj_stage_00[nCntType][nCntParts],
				&g_aMeshObj_stage_00[nCntType][nCntParts]);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObj_stage_00->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_stage_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_stage_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];

	// 部品(3D)のトランスフォームを初期化
	InitParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// パラメーターの初期化処理
	InitParameterObj_stage_00();

	if (GetMode() == MODE_GAME_00)
	{// モードがMD:ゲーム画面[00] の時、
		// セッティング処理
		SettingObj_stage_00();

		// 警告設定処理
		SetWarningObj_stage_00();

		// 状態を設定
		SetStateObj_stage_00(OBJ_STAGE_00_INIT_STATE);
	}
}

//========================================
// UninitObj_stage_00関数 - OBJ:ステージ[00] の終了処理 -
//========================================
void UninitObj_stage_00(void)
{
	// メッシュの破棄
	for (int nCntType = 0; nCntType < OBJ_STAGE_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			if (g_aMeshObj_stage_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshObj_stage_00[nCntType][nCntParts]->Release();
				g_aMeshObj_stage_00[nCntType][nCntParts] = NULL;
			}
		}
	}

	// マテリアルの破棄
	if (g_pBuffMatObj_stage_00 != NULL)
	{
		g_pBuffMatObj_stage_00->Release();
		g_pBuffMatObj_stage_00 = NULL;
	}
}

//========================================
// UpdateObj_stage_00関数 - OBJ:ステージ[00] の更新処理 -
//========================================
void UpdateObj_stage_00(void)
{
	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];

	// 部品(3D)のトランスフォームを取得
	GetParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	{
		float	fRate		// カウンターの進行率
				= (float)pObj->nCounterDiscoloration / (float)OBJ_STAGE_00_DISCOLORATION_TIME;

		if (GetMode() == MODE_TITLE_00)
		{// モードがMD:タイトル画面[00] の時、
			pObj->groundCol = pType->waveSet.aWave[0].intervalGroundColor;
			GetMain()->bgCol = pType->waveSet.aWave[0].intervalSkyColor;
		}
		else if (
			pObj->state == OBJ_STAGE_00_STATE_INTERVAL ||
			pObj->state == OBJ_STAGE_00_STATE_CLEAR)
		{// 状態がインターバル or クリアの時、
			pObj->groundCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWaveOld].raidGroundColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalGroundColor, fRate));
			GetMain()->bgCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWaveOld].raidSkyColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalSkyColor, fRate));
		}
		else if (pObj->state == OBJ_STAGE_00_STATE_RAID)
		{// 状態が襲撃の時、
			pObj->groundCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalGroundColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].raidGroundColor, fRate));
			GetMain()->bgCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalSkyColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].raidSkyColor, fRate));
		}
	}

	if (GetMode() == MODE_TITLE_00) 
	{// モードがMD:タイトル画面[00] の時、
		pObj->rot += MD_TITLE_00_CAMERA3D_BG_SPIN;	// 回転をかける
		RotControl(&pObj->rot);						// 向きを制御

		return;	// 処理を終了する
	}

	// 状態処理
	UpdateStateObj_stage_00();

	if (pObj->nCounterDiscoloration < OBJ_STAGE_00_DISCOLORATION_TIME) 
	{// 変色カウンターが変色にかかる時間を下回っている時、
		pObj->nCounterDiscoloration++;	// 変色カウンターを加算
	}

	if (GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL)
	{// MD:ゲーム画面[00] の状態が通常の時、
		if (pObj->state == OBJ_STAGE_00_STATE_INTERVAL)
		{// OBJ:ステージ[00] の状態がインターバルの時、
			// カウンターの進行率
			float fRate = (float)(pObj->nCounter % 60) / 60.0f;

			// 色をカウンターの進行率に応じて透明度を算出したものに設定
			Color col = GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
				OBJ_STAGE_00_NEXTWAVE_TEXT_WARNING_COLOR:
				OBJ_STAGE_00_NEXTWAVE_TEXT_COLOR;
			col.a *= (OBJ_STAGE_00_NEXTWAVE_ALPHA_MIN + ((OBJ_STAGE_00_NEXTWAVE_ALPHA_MAX - OBJ_STAGE_00_NEXTWAVE_ALPHA_MIN) * (1.0f - fRate)));

			// 拡大倍率をカウンターの進行率に応じて算出
			float fScale = (OBJ_STAGE_00_NEXTWAVE_SCALE_MIN + ((OBJ_STAGE_00_NEXTWAVE_SCALE_MAX - OBJ_STAGE_00_NEXTWAVE_SCALE_MIN) * (1.0f - fRate)));

			char aString[TXT_MAX];	// 文字列
			sprintf(aString, "NEXT.%d", (GetObj_stage_00Type()->waveSet.aWave[pObj->nWave].nIntervalTime - pObj->nCounter - 1) / 60);
			SetText2D(
				aString,
				FONT_003,
				DISPLAY_CENTER,
				OBJ_STAGE_00_NEXTWAVE_POS,
				col, fScale, fScale);
		}
	}
}

//========================================
// DrawObj_stage_00関数 - OBJ:ステージ[00] の描画処理 -
//========================================
void DrawObj_stage_00(void)
{
	Obj_stage_00		*pObj				// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX			mtxSelf;			// 本体のマトリックス

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_stage_00->GetBufferPointer();

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

	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// 部品(3D)の情報のポインタ
					= &pObj->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// 部品(3D)の部品の種類毎の情報のポインタ
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// 親マトリックス

		// 部品のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMtxWorldObj_stage_00[nCntParts]);

		// 部品の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_stage_00[nCntParts], &g_aMtxWorldObj_stage_00[nCntParts], &mtxRot);

		// 部品の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_stage_00[nCntParts], &g_aMtxWorldObj_stage_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
			mtxParent = g_aMtxWorldObj_stage_00[pPartsType->nParent];
		}
		else
		{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
			mtxParent = mtxSelf;
		}

		// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(
			&g_aMtxWorldObj_stage_00[nCntParts],
			&g_aMtxWorldObj_stage_00[nCntParts],
			&mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObj_stage_00[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_stage_00[pObj->nType][nCntParts]; nCntMat++)
		{
			if (nCntParts == OBJ_STAGE_00_DISCOLORATION_PARTS) 
			{// カウントが変色する部品の時、
				// マテリアルの設定
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D, pObj->groundCol);
			}
			else 
			{// カウントが変色する部品でない時、
				// マテリアルの設定
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);
			}

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureObj_stage_00[pObj->nType][nCntParts][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshObj_stage_00[pObj->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// CollisionObj_stage_00関数 - OBJ:ステージ[00] との当たり判定処理 -
//========================================
void CollisionObj_stage_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];

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
			pType->hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
		},
		COLLCHK_MODE_LOOKDOWN);

	if (pObjCollision != NULL)
	{// 衝突情報がNULLで無い時、
		// 共通の衝突情報を上書き
		CollisionOverwrite(pCmnCollision, *pObjCollision);
	}
}

//========================================
// CollisionObj_stage_00WithSetPos関数 - OBJ:ステージ[00] との当たり判定処理(With設定位置) -
//========================================
void CollisionObj_stage_00WithSetPos(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	Obj_stage_00		*pObj	// OBJ:ステージ[00] の情報のポインタ
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:ステージ[00] の種類毎の情報のポインタ
						= &g_aObj_stage_00Type[pObj->nType];

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
			pType->hitTestSet.aHitTest[OBJ_STAGE_00_HITTEST_WITH_SETPOS]
		},
		COLLCHK_MODE_LOOKDOWN);
}

//========================================
// BossSetObj_stage_00関数 - OBJ:ステージ[00] のボス設定処理 -
//========================================
void BossSetObj_stage_00(void)
{
	Obj_stage_00	*pObj	// OBJ:ステージ[00] の情報のポインタ
					= &g_obj_stage_00;

	// CHR:敵[00] のボス設定処理
	SetBossChr_enemy_00(pObj->setBoss.pos, pObj->setBoss.rot, pObj->setBoss.nType);
}