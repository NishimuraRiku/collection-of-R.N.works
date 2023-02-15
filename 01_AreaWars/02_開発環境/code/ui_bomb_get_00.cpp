//========================================
// 
// 爆弾獲得[00]の処理
// Author:西村 吏功
// 
//========================================
// *** ui_bomb_get_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "md_game_00.h"
#include "ui_bomb_get_00.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// マクロ定義
//****************************************

// 爆弾獲得[00] の寿命
// 爆弾獲得[00] の点滅時の色
// 爆弾獲得[00] の点滅時にかかる時間
// 爆弾獲得[00] の移動量
#define UI_BOMB_GET_00_LIFE				(40)
#define UI_BOMB_GET_00_BRINK_COLOR		{243, 191, 63, 255}
#define UI_BOMB_GET_00_BRINK_TIME		(4)
#define UI_BOMB_GET_00_MOVE				(D3DXVECTOR3(0.0f,-1.0f,0.0f))

//****************************************
// プロトタイプ宣言
//****************************************

// 爆弾獲得[00] のパラメーターの初期化
void InitParameterUi_bomb_get_00(void);

//****************************************
// グローバル宣言
//****************************************
Ui_bomb_get_00 g_ui_bomb_get_00;	// 爆弾獲得[00] の情報

//========== *** 爆弾獲得[00] の情報を取得 ***
Ui_bomb_get_00 *GetUi_bomb_get_00(void) 
{
	return &g_ui_bomb_get_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_bomb_get_00関数 - 爆弾獲得[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_bomb_get_00(void) 
{
	g_ui_bomb_get_00.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	g_ui_bomb_get_00.bUse			= false;							// 使用しているかフラグ
	g_ui_bomb_get_00.nLife			= 0;								// 寿命
	g_ui_bomb_get_00.nCounterBrink	= 0;								// 点滅カウンター
	g_ui_bomb_get_00.bBrink			= false;							// 点滅フラグ
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_bomb_get_00関数 - 爆弾獲得[00]の初期化処理 -
//========================================
void InitUi_bomb_get_00(void)
{
	// 爆弾獲得[00] のパラメーターの初期化処理
	InitParameterUi_bomb_get_00();
}

//========================================
// UninitUi_bomb_get_00関数 - 爆弾獲得[00]の終了処理 -
//========================================
void UninitUi_bomb_get_00(void)
{
	// 非表示にする
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_GET, false);
}

//========================================
// UpdateUi_bomb_get_00関数 - 爆弾獲得[00]の更新処理 -
//========================================
void UpdateUi_bomb_get_00(void)
{
	Ui_bomb_get_00	*pUi					//
					= &g_ui_bomb_get_00;	// 爆弾獲得[00] の情報

	if (pUi->bUse)
	{// 爆弾獲得[00] が使用されている時、
		// 表示する
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_GET, true);
	}
	else
	{// 爆弾獲得[00] が使用されていない時、
		// 非表示にする
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_GET, false);

		// 処理を終了する
		return;
	}

	if (--pUi->nLife <= 0)
	{// 寿命を減算し、尽きた時、
		// 使用していない状態にする
		pUi->bUse = false;
	}

	// 位置に移動量を更新
	pUi->pos += UI_BOMB_GET_00_MOVE;

	D3DXVECTOR3 pos			//
				= pUi->pos;	// 位置

	// 位置をカメラに合わせる
	MatchCameraPosition(&pos);

	if (++pUi->nCounterBrink >= UI_BOMB_GET_00_BRINK_TIME)
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
		col = UI_BOMB_GET_00_BRINK_COLOR;
	}

	// テキストの更新処理
	UpdateTxt_00(
		TEXT_FIELD_UI, 
		MD_GAME_00_TEXT_BOMB_GET, 
		GetMd_game_00Text(MD_GAME_00_TEXT_BOMB_GET).aDisplay, 
		pos, 
		col, 
		TEXT_MODE_CENTER, 
		GetMd_game_00Text(MD_GAME_00_TEXT_BOMB_GET).font,
		1.0f,
		1.0f);
}

//========================================
// SetUi_bomb_get_00関数 - 爆弾獲得[00]の設定処理 -
//========================================
void SetUi_bomb_get_00(D3DXVECTOR3 pos)
{
	Ui_bomb_get_00	*pUi					//
					= &g_ui_bomb_get_00;	// 爆弾獲得[00] の情報

	// パラメーターの初期化処理
	InitParameterUi_bomb_get_00();

	// 位置を代入
	pUi->pos = pos;

	// 寿命を設定する
	pUi->nLife = UI_BOMB_GET_00_LIFE;

	// 使用している状態にする
	pUi->bUse = true;
}