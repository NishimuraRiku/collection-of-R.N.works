//========================================
// 
// EFF:パーティクル[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_particle.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "eff_light_00.h"		// EFF:光			[00]
#include "eff_particle_00.h"	// EFF:パーティクル	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// EFF:パーティクル[00] の種類毎の情報のデータファイルの相対パス
#define EFF_PARTICLE_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\EFFECT\\EFF_PARTICLE_00_TYPE_DATA.txt"

// EFF:パーティクル[00] の最大数
// EFF:パーティクル[00] の種類の最大数
#define EFF_PARTICLE_00_MAX			(256)
#define EFF_PARTICLE_00_TYPE_MAX	(8)

// EFF:パーティクル[00] の法線ベクトル
// EFF:パーティクル[00] の相対位置
#define EFF_PARTICLE_00_NOR				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define EFF_PARTICLE_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//****************************************
// 構造体の定義
//****************************************
// EFF:パーティクル[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	
	// 分類関連
	int	nType;	// 種類

	// 状態関連
	bool	bUse;		// 使用されているかフラグ
	int		nLife;		// 寿命
	int		nLifeTemp;	// 元の寿命
}Eff_particle_00;

// EFF:パーティクル[00] の種類毎の情報構造体
typedef struct 
{
	int			nLightType;	// 光の種類
	int			nSetMin;	// 設定数の下限
	int			nSetMax;	// 設定数の上限
	int			nLifeMin;	// 寿命の下限
	int			nLifeMax;	// 寿命の上限
	D3DXVECTOR3 moveMin;	// 移動量の下限
	D3DXVECTOR3 moveMax;	// 移動量の上限
}Eff_particle_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:パーティクル[00] のパラメーターの初期化処理
void InitParameterEff_particle_00(Eff_particle_00 *pEff);

//****************************************
// グローバル宣言
//****************************************
Eff_particle_00		g_aEff_particle_00		// EFF:パーティクル[00] の情報
					[EFF_PARTICLE_00_MAX];
Eff_particle_00Type	g_aEff_particle_00Type	// EFF:パーティクル[00] の種類毎の情報
					[EFF_PARTICLE_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_particle_00関数 - EFF:パーティクル[00] のパラメーターの初期化処理 -
//========================================
void InitParameterEff_particle_00(Eff_particle_00 *pEff)
{
	pEff->pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pEff->move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	pEff->nType		= 0;								// 種類
	pEff->bUse		= false;							// 使用されているかフラグ
	pEff->nLife		= 0;								// 寿命
	pEff->nLifeTemp = 0;								// 元の寿命
}

//========================================
// DestroyEff_particle_00関数 - EFF:パーティクル[00] の破壊処理 -
//========================================
void DestroyEff_particle_00(Eff_particle_00 *pEff) 
{
	pEff->bUse = false;	// 使用されていない状態にする
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadEff_particle_00関数 - EFF:弾[00] の読み込み処理 -
//========================================
void LoadEff_particle_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Eff_particle_00Type	*pEffType	// EFF:弾[00] の種類毎の情報
						= g_aEff_particle_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(EFF_PARTICLE_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END")) { fclose(pFile); break; }	// 読み込みを終了
		if (aDataSearch[0] == '#') { continue; }				// 折り返す

		if (!strcmp(aDataSearch, "TYPE")) {
			// EFF:弾[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pEffType++; break; }							// 読み込みを終了
				else if (!strcmp(aDataSearch, "LIGHT_TYPE:"))	{ fscanf(pFile, "%d", &pEffType->nLightType); }	// 光の種類
				else if (!strcmp(aDataSearch, "SET_MIN:"))		{ fscanf(pFile, "%d", &pEffType->nSetMin); }	// 
				else if (!strcmp(aDataSearch, "SET_MAX:"))		{ fscanf(pFile, "%d", &pEffType->nSetMax); }	// 
				else if (!strcmp(aDataSearch, "LIFE_MIN:"))		{ fscanf(pFile, "%d", &pEffType->nLifeMin); }	// 
				else if (!strcmp(aDataSearch, "LIFE_MAX:"))		{ fscanf(pFile, "%d", &pEffType->nLifeMax); }	// 
				else if (!strcmp(aDataSearch, "MOVE_MIN:"))		{	// 移動量の下限
					fscanf(pFile, "%f", &pEffType->moveMin.x);		// X
					fscanf(pFile, "%f", &pEffType->moveMin.y);		// Y
					fscanf(pFile, "%f", &pEffType->moveMin.z);		// Z
				}
				else if (!strcmp(aDataSearch, "MOVE_MAX:")) {		// 移動量の上限
					fscanf(pFile, "%f", &pEffType->moveMax.x);		// X
					fscanf(pFile, "%f", &pEffType->moveMax.y);		// Y
					fscanf(pFile, "%f", &pEffType->moveMax.z);		// Z
				}
			}
		}
	}
}

//========================================
// InitEff_particle_00関数 - EFF:パーティクル[00] の初期化処理 -
//========================================
void InitEff_particle_00(void)
{
	Eff_particle_00	*pEff	// EFF:パーティクル[00] の情報のポインタ
					= g_aEff_particle_00;

	for (int nCntEff = 0; nCntEff < EFF_PARTICLE_00_MAX; nCntEff++, pEff++)
	{
		// パラメーターの初期化処理
		InitParameterEff_particle_00(&g_aEff_particle_00[nCntEff]);
	}
}

//========================================
// UninitEff_particle_00関数 - EFF:パーティクル[00] の終了処理 -
//========================================
void UninitEff_particle_00(void)
{
	
}

//========================================
// UpdateEff_particle_00関数 - EFF:パーティクル[00] の更新処理 -
//========================================
void UpdateEff_particle_00(void)
{
	Eff_particle_00	*pEff	// EFF:パーティクル[00] の情報のポインタ
					= g_aEff_particle_00;
	
	for (int nCntEff = 0; nCntEff < EFF_PARTICLE_00_MAX; nCntEff++, pEff++)
	{
		if (!pEff->bUse)
		{// 使用されている状態でない時、
			// 繰り返し処理を折り返す
			continue;
		}
		else if (--pEff->nLife <= 0) 
		{// 寿命を減算した結果0以下の時、
			pEff->bUse = false;	// 使用されていない状態にする
			continue;			// 繰り返し処理を折り返す
		}

 		pEff->pos.x += (pEff->move.x * ((float)pEff->nLife / (float)pEff->nLifeTemp));
		pEff->pos.y += (pEff->move.y * ((float)pEff->nLife / (float)pEff->nLifeTemp));
		pEff->pos.z += (pEff->move.z * ((float)pEff->nLife / (float)pEff->nLifeTemp));

		// EFF:光[00] の設定処理
		SetEff_light_00(pEff->pos, pEff->nType);
	}
}

//========================================
// SetEff_particle_00関数 - EFF:パーティクル[00] の設定処理 -
//========================================
void SetEff_particle_00(D3DXVECTOR3 pos, int nType)
{
	Eff_particle_00		*pEff	// EFF:パーティクル[00] の情報のポインタ
						= g_aEff_particle_00;
	Eff_particle_00Type	*pType	// EFF:パーティクル[00] の種類毎の情報のポインタ
						= &g_aEff_particle_00Type[nType];

	int nSetNum = pType->nSetMin + (rand() % (pType->nSetMin - pType->nSetMin + 1));	// 設定数を乱数で設定

	if (nSetNum <= 0) 
	{// 設定数が0以下の時、
		// 処理を終了する
		return;
	}

	for (int nCntEff = 0; nCntEff < EFF_PARTICLE_00_MAX; nCntEff++, pEff++)
	{
		if (pEff->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pEff->pos = pos;					// 位置を代入
		pEff->nType = pType->nLightType;	// 種類を代入
		pEff->bUse = true;					// 使用されている状態にする

		// 寿命を乱数で設定
		pEff->nLife = pType->nLifeMin + (rand() % (pType->nLifeMax - pType->nLifeMin));
		pEff->nLifeTemp = pEff->nLife;

		// 移動量を乱数で設定
		pEff->move.x = pType->moveMin.x + fRand(pType->moveMax.x - pType->moveMin.x);
		pEff->move.y = pType->moveMin.y + fRand(pType->moveMax.y - pType->moveMin.y);
		pEff->move.z = pType->moveMin.z + fRand(pType->moveMax.z - pType->moveMin.z);

		if (--nSetNum <= 0)
		{// 設定数を減算した結果0以下の時、
			// 繰り返し処理を抜ける
			break;
		}
	}
}