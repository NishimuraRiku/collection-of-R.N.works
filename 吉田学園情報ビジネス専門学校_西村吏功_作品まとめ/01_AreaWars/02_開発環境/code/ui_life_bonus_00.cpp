//========================================
// 
// ライフボーナス[00]の処理
// Author:西村 吏功
// 
//========================================
// *** ui_life_bonus_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "md_game_00.h"
#include "ui_life_bonus_00.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// マクロ定義
//****************************************

// ライフボーナス[00] の寿命
// ライフボーナス[00] の点滅時の色
// ライフボーナス[00] の点滅時にかかる時間
// ライフボーナス[00] の開始位置
// ライフボーナス[00] の移動量
#define UI_LIFE_BONUS_00_LIFE			(80)
#define UI_LIFE_BONUS_00_BRINK_COLOR	{255, 119, 183, 255}
#define UI_LIFE_BONUS_00_BRINK_TIME		(4)
#define UI_LIFE_BONUS_00_START_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + PIXEL * 8 * 8, PIXEL * 88, 0.0f))
#define UI_LIFE_BONUS_00_MOVE			(D3DXVECTOR3(0.0f,-1.0f,0.0f))

//****************************************
// プロトタイプ宣言
//****************************************

// ライフボーナス[00] のパラメーターの初期化
void InitParameterUi_life_bonus_00(void);

//****************************************
// グローバル宣言
//****************************************
Ui_life_bonus_00 g_ui_life_bonus_00;	// ライフボーナス[00] の情報

//========== *** ライフボーナス[00] の情報を取得 ***
Ui_life_bonus_00 *GetUi_life_bonus_00(void) 
{
	return &g_ui_life_bonus_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_life_bonus_00関数 - ライフボーナス[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_life_bonus_00(void) 
{
	g_ui_life_bonus_00.pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	g_ui_life_bonus_00.bUse				= false;							// 使用しているかフラグ
	g_ui_life_bonus_00.nLife			= 0;								// 寿命
	g_ui_life_bonus_00.nCounterBrink	= 0;								// 点滅カウンター
	g_ui_life_bonus_00.bBrink			= false;							// 点滅フラグ
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_life_bonus_00関数 - ライフボーナス[00]の初期化処理 -
//========================================
void InitUi_life_bonus_00(void)
{
	// ライフボーナス[00] のパラメーターの初期化処理
	InitParameterUi_life_bonus_00();
}

//========================================
// UninitUi_life_bonus_00関数 - ライフボーナス[00]の終了処理 -
//========================================
void UninitUi_life_bonus_00(void)
{
	// 非表示にする
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE_BONUS, false);
}

//========================================
// UpdateUi_life_bonus_00関数 - ライフボーナス[00]の更新処理 -
//========================================
void UpdateUi_life_bonus_00(void)
{
	Ui_life_bonus_00	*pUi					//
						= &g_ui_life_bonus_00;	// ライフボーナス[00] の情報

	if (pUi->bUse)
	{// ライフボーナス[00] が使用されている時、
		// 表示する
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE_BONUS, true);
	}
	else
	{// ライフボーナス[00] が使用されていない時、
		// 非表示にする
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE_BONUS, false);

		// 処理を終了する
		return;
	}

	if (--pUi->nLife <= 0)
	{// 寿命を減算し、尽きた時、
		// 使用していない状態にする
		pUi->bUse = false;
	}

	// 位置に移動量を更新
	pUi->pos += UI_LIFE_BONUS_00_MOVE;

	D3DXVECTOR3 pos			//
				= pUi->pos;	// 位置

	if (++pUi->nCounterBrink >= UI_LIFE_BONUS_00_BRINK_TIME)
	{// 点滅カウンターを加算し、点滅にかかる時間に達した時、
		// 点滅フラグを初期化
		pUi->nCounterBrink = 0;

		// 点滅フラグを切り替える
		pUi->bBrink ^= 1;
	}

	Color	col						//
			= { 255,255,255,255 };	// 色

	if (pUi->bBrink)
	{// 点滅フラグが真の時、
		// 色を点滅時の物にする
		col = UI_LIFE_BONUS_00_BRINK_COLOR;
	}

	// 透明度を寿命に応じて設定
	col.nA = 255 * ((float)pUi->nLife / (float)UI_LIFE_BONUS_00_LIFE);

	{
		char aString[TXT_MAX]; // 表示する文字列

		// 文字列を設定
		sprintf(aString, GetMd_game_00Text(MD_GAME_00_TEXT_LIFE_BONUS).aDisplay, pUi->nLifeBonus);

		// テキストの更新処理
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_LIFE_BONUS,
			aString,
			pos,
			col,
			TEXT_MODE_CENTER,
			GetMd_game_00Text(MD_GAME_00_TEXT_LIFE_BONUS).font,
			1.0f,
			1.0f);
	}
}

//========================================
// SetUi_life_bonus_00関数 - ライフボーナス[00]の設定処理 -
//========================================
void SetUi_life_bonus_00(int nLifeBonus)
{
	Ui_life_bonus_00	*pUi					//
						= &g_ui_life_bonus_00;	// ライフボーナス[00] の情報

	// パラメーターの初期化処理
	InitParameterUi_life_bonus_00();

	// ライフボーナスを代入
	pUi->nLifeBonus = nLifeBonus;

	// 位置を設定
	pUi->pos = UI_LIFE_BONUS_00_START_POS;

	// 寿命を設定する
	pUi->nLife = UI_LIFE_BONUS_00_LIFE;

	// 使用している状態にする
	pUi->bUse = true;
}