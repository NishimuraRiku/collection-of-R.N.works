//========================================
// 
// MD:メインの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_main.cpp ***
//========================================
// BackGround
#include "../ModelViewer/bg_polygon-floor.h"
// Character
#include "../ModelViewer/chr_model.h"
// Other
#include "../ModelViewer/model-viewer.h"
// _R.N.Writable
#include "../R.N.Lib/RNmain.h"

//****************************************
// マクロ定義
//****************************************
//========== *** 状態関連 ***
// 初期の状態
#define INIT_STATE (MD_MAIN_STATE_NORMAL)

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// MD:メインのパラメーター初期値を取得
Md_main InitParameterMd_main();

//****************************************
// グローバル変数宣言
//****************************************
Md_main g_md_main;	// MD:メインの情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// StartMd_mainState関数 - MD:メイン画面の状態に応じた開始処理 -
//========================================
void StartMd_mainState(void)
{
	// MD:メイン画面の情報のポインタ
	Md_main	*pMd = &g_md_main;

	switch (pMd->state)
	{
	case MD_MAIN_STATE_NORMAL: {

		break; }
	}
}

//========================================
// EndMd_mainState関数 - MD:メイン画面の状態に応じた終了処理 -
//========================================
void EndMd_mainState(void)
{
	// MD:メイン画面の情報のポインタ
	Md_main	*pMd = &g_md_main;

	switch (pMd->state)
	{
	case MD_MAIN_STATE_NORMAL: {

		break; }
	}
}

//========================================
// UpdateMd_mainState関数 - MD:メイン画面の状態に応じた更新処理 -
//========================================
void UpdateMd_mainState(void)
{
	// MD:メイン画面の情報のポインタ
	Md_main	*pMd = &g_md_main;

	switch (pMd->state)
	{
	case MD_MAIN_STATE_NORMAL: {

		break; }
	}
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// - MD:メインのパラメーター初期値を取得 -
//========================================
Md_main InitParameterMd_main()
{
	return{
		MD_MAIN_STATE_NONE,	// 状態
	};
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// InitMd_main関数 - MD:メインの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitMd_main(void) 
{
	// MD:メイン画面の情報のポインタ
	Md_main	*pMd = &g_md_main;

	// パラメーター初期化
	*pMd = InitParameterMd_main();

	// 初期化処理
	InitBg_polygonFloor();	// BG :ポリゴン床
	InitChr_model();		// CHR:プレイヤー
	InitModelViewer();		// モーションビューワ

	// 初期状態を設定
	SetStateMd_main(INIT_STATE);

	// モデル情報の初期読み込み処理
	InitLoadModelDataModelViewer();
}

//========================================
// UninitMd_main関数 - MD:メインの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_main(void) 
{
	UninitBg_polygonFloor();	// BG :ポリゴン床
	UninitChr_model();			// CHR:プレイヤー
}

//========================================
// UpdateMd_main関数 - MD:メインの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_main(void) 
{
	// 状態に応じた更新処理
	UpdateMd_mainState();

	// 更新処理
	UpdateModelViewer();			// モーションビューワ
	if (!GetModelViewer()->bPause)
	{// モデルビューワのポーズフラグが偽の時、
		UpdateBg_polygonFloor();	// BG :ポリゴン床
		UpdateChr_model();			// CHR:プレイヤー
	}
}

//========================================
// DrawMd_main関数 - MD:メインの描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_main(void)
{
	DrawBg_polygonFloor();	// BG :ポリゴン床
	DrawChr_model();		// CHR:プレイヤー
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// GetMd_main関数 - MD:メインの情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Md_main *GetMd_main(void) 
{
	return &g_md_main;
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// SetStateMd_main関数 - MD:メインの状態設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetStateMd_main(MD_MAIN_STATE state)
{
	// MD:メイン画面の情報のポインタ
	Md_main	*pMd = &g_md_main;

	// 状態に応じた終了処理
	EndMd_mainState();
	
	// 状態を代入
	pMd->state = state;

	// 状態に応じた開始処理
	StartMd_mainState();
}