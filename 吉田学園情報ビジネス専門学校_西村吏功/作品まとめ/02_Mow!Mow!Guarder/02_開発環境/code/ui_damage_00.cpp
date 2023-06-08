//========================================
// 
// UI:ダメージ[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_damage_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "text.h"
#include "ui_damage_00.h"	// UI:ダメージ	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// UI:ダメージ[00] の最大数
#define UI_DAMAGE_00_MAX	(256)

// UI:ダメージ[00] の法線ベクトル
// UI:ダメージ[00] の相対位置
#define UI_DAMAGE_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define UI_DAMAGE_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

// UI:ダメージ[00] の寿命
#define UI_DAMAGE_00_LIFE	(30)

// UI:ダメージ[00] の移動力
// UI:ダメージ[00] の位置のぶれ幅
#define UI_DAMAGE_00_MOVE_FORCE		D3DXVECTOR3(0.0f,0.5f,0.0f)
#define UI_DAMAGE_00_POS_AMPLITUDE	(5.0f)

//****************************************
// 構造体の定義
//****************************************
// UI:ダメージ[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	
	// 分類関連
	UI_DAMAGE_00_COLOR_TYPE colType;	// 色の種類

	// 状態関連
	int		nDamage;	// ダメージ
	bool	bUse;		// 使用されているかフラグ
	int		nLife;		// 寿命
}Ui_damage_00;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:ダメージ[00] のパラメーターの初期化処理
void InitParameterUi_damage_00(Ui_damage_00 *pUi);

//****************************************
// グローバル宣言
//****************************************
Ui_damage_00	g_aUi_damage_00	// UI:ダメージ[00] の情報
				[UI_DAMAGE_00_MAX];

// UI:ダメージ[00] の色
const Color g_aUi_damage_00Color[UI_DAMAGE_00_COLOR_TYPE_MAX] =
{
	{219,43,0,255},
	{255,255,255,255},
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_damage_00関数 - UI:ダメージ[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_damage_00(Ui_damage_00 *pUi)
{
	pUi->pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pUi->colType	= (UI_DAMAGE_00_COLOR_TYPE)0;		// 色の種類
	pUi->nDamage	= 0;								// ダメージ
	pUi->bUse		= false;							// 使用されているかフラグ
	pUi->nLife		= 0;								// 寿命
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_damage_00関数 - UI:ダメージ[00] の初期化処理 -
//========================================
void InitUi_damage_00(void)
{
	Ui_damage_00	*pUi	// UI:ダメージ[00] の情報のポインタ
					= g_aUi_damage_00;

	for (int nCntEff = 0; nCntEff < UI_DAMAGE_00_MAX; nCntEff++, pUi++)
	{
		// パラメーターの初期化処理
		InitParameterUi_damage_00(&g_aUi_damage_00[nCntEff]);
	}
}

//========================================
// UninitUi_damage_00関数 - UI:ダメージ[00] の終了処理 -
//========================================
void UninitUi_damage_00(void)
{
	
}

//========================================
// UpdateUi_damage_00関数 - UI:ダメージ[00] の更新処理 -
//========================================
void UpdateUi_damage_00(void)
{
	Ui_damage_00	*pUi	// UI:ダメージ[00] の情報のポインタ
					= g_aUi_damage_00;
	
	for (int nCntEff = 0; nCntEff < UI_DAMAGE_00_MAX; nCntEff++, pUi++)
	{
		if (!pUi->bUse)
		{// 使用されている状態でない時、
			// 繰り返し処理を折り返す
			continue;
		}
		else if (--pUi->nLife <= 0) 
		{// 寿命を減算した結果0以下の時、
			pUi->bUse = false;	// 使用されていない状態にする
			continue;			// 繰り返し処理を折り返す
		}

		// 位置を更新
 		pUi->pos += (UI_DAMAGE_00_MOVE_FORCE * ((float)pUi->nLife / (float)UI_DAMAGE_00_LIFE));

		// テキストの設定処理
		{
			char aString[TXT_MAX];
			sprintf(aString, "%d", pUi->nDamage);
			SetText3D(
				aString,
				FONT_002,
				DISPLAY_CENTER,
				pUi->pos,
				g_aUi_damage_00Color[pUi->colType],
				1.0f, 1.0f, false, false, true);
		}
	}
}

//========================================
// SetUi_damage_00関数 - UI:ダメージ[00] の設定処理 -
//========================================
void SetUi_damage_00(D3DXVECTOR3 pos, int nDamage, UI_DAMAGE_00_COLOR_TYPE colType)
{
	Ui_damage_00	*pUi	// UI:ダメージ[00] の情報のポインタ
					= g_aUi_damage_00;

	for (int nCntEff = 0; nCntEff < UI_DAMAGE_00_MAX; nCntEff++, pUi++)
	{
		if (pUi->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pUi->pos = pos;			// 位置を代入
		pUi->nDamage = nDamage;	// ダメージを代入
		pUi->colType = colType;	// 色の種類を代入
		pUi->bUse = true;		// 使用されている状態にする

		// 位置に乱数を適用
		pUi->pos.x += -UI_DAMAGE_00_POS_AMPLITUDE + fRand(UI_DAMAGE_00_POS_AMPLITUDE * 2);
		pUi->pos.y += -UI_DAMAGE_00_POS_AMPLITUDE + fRand(UI_DAMAGE_00_POS_AMPLITUDE * 2);
		pUi->pos.z += -UI_DAMAGE_00_POS_AMPLITUDE + fRand(UI_DAMAGE_00_POS_AMPLITUDE * 2);

		// 寿命を設定
		pUi->nLife = UI_DAMAGE_00_LIFE;

		// 繰り返し処理を抜ける
		break;
	}
}