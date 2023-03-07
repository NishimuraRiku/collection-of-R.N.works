//========================================
// 
// MD:ゲーム画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "light.h"
#include "setting.h"
#include "sound.h"
#include "text.h"
#include "atk_bullet_00.h"			// ATK:弾					[00]
#include "atk_impact_00.h"			// ATK:衝撃					[00]
#include "atk_sword_00.h"			// ATK:剣					[00]
#include "bg_mountain_00.h"			// BG :山					[00]
#include "bg_space_00.h"			// BG :宇宙					[00]
#include "chr_player_00.h"			// CHR:プレイヤー			[00]
#include "chr_enemy_00.h"			// CHR:敵					[00]
#include "eff_explosion_00.h"		// EFF:爆発					[00]
#include "eff_light_00.h"			// EFF:光					[00]
#include "eff_particle_00.h"		// EFF:パーティクル			[00]
#include "eff_screen_00.h"			// EFF:スクリーン			[00]
#include "eff_shadow_00.h"			// EFF:影					[00]
#include "eff_shock-wave_00.h"		// EFF:衝撃波				[00]
#include "md_game_00.h"				// MD :ゲーム画面			[00]
#include "itm_coin_00.h"			// ITM:コイン				[00]
#include "obj_block_00.h"			// OBJ:ブロック				[00]
#include "obj_core_00.h"			// OBJ:コア					[00]
#include "obj_discharger_00.h"		// OBJ:放電装置				[00]
#include "obj_mirror_00.h"			// OBJ:ミラー				[00]
#include "obj_stage_00.h"			// OBJ:ステージ				[00]
#include "obj_switch_00.h"			// OBJ:スイッチ				[00]
#include "obj_target_00.h"			// OBJ:ターゲット			[00]
#include "obj_turret_00.h"			// OBJ:タレット				[00]
#include "sys_ranking_00.h"			// SYS:ランキング			[00]
#include "ui_attention-mark_00.h"	// UI :注意マーク			[00]
#include "ui_input-guid_00.h"		// UI :入力ガイド			[00]
#include "ui_coin-frame_00.h"		// UI :コインフレーム		[00]
#include "ui_damage_00.h"			// UI :ダメージ				[00]
#include "ui_dialog-box_00.h"		// UI :ダイアログボックス	[00]
#include "ui_frame_00.h"			// UI :フレーム				[00]
#include "ui_hp-bar_00.h"			// UI :HPバー				[00]
#include "ui_hp-bar_01.h"			// UI :HPバー				[01]
#include "ui_inventory_00.h"		// UI :インベントリ			[00]
#include "ui_item-icon_00.h"		// UI :アイテムアイコン		[00]
#include "ui_item-guid_00.h"		// UI :商品説明				[00]
#include "ui_map_00.h"				// UI :マップ				[00]
#include "ui_menu_00.h"				// UI :メニュー				[00]
#include "ui_parameter_00.h"		// UI :パラメーター			[00]
#include "ui_ranking-frame_00.h"	// UI :ランキングフレーム	[00]
#include "ui_tips_00.h"				// UI :TIPS					[00]
#include "ui_user-guid_00.h"		// UI :ユーザーガイド		[00]
#include "ui_warning-text_00.h"		// UI :注意テキスト			[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// MD:ゲーム画面[00] のポーズメニューの位置
// MD:ゲーム画面[00] のリザルトメニューの位置
// MD:ゲーム画面[00] のランキングの位置
// MD:ゲーム画面[00] のユーザーガイドメニューの位置
// MD:ゲーム画面[00] のユーザーガイドの位置
// MD:ゲーム画面[00] の設定メニューの位置
#define MD_GAME_00_PAUSE_MENU_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT-PIXEL*64,0.0f)
#define MD_GAME_00_RESULT_MENU_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT+(PIXEL*-32),0.0f)
#define MD_GAME_00_RANKING_POS			D3DXVECTOR3(BUFFER_WIDTH*0.5f,(BUFFER_HEIGHT*0.5f)-PIXEL*8,0.0f)
#define MD_GAME_00_USERGUID_MENU_POS	D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT-PIXEL*32,0.0f)
#define MD_GAME_00_USERGUID_POS			D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT*0.5f,0.0f)
#define MD_GAME_00_SETTING_MENU_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT-PIXEL*64,0.0f)

// MD:ゲーム画面[00] のボス出現ムービー(視点移動)にかかる時間
// MD:ゲーム画面[00] のボス出現ムービー(視点リセット)にかかる時間
#define MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE_TIME		(60)
#define MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET_TIME	(60)

// MD:ゲーム画面[00] のコア破壊ムービー(爆発)にかかる時間
// MD:ゲーム画面[00] のコア破壊ムービー(爆発)の明るくなり始める時間
#define MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_TIME		(120)
#define MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_BRIGHT_TIME	(60)

// MD:ゲーム画面[00] のカメラ(3D)の初期向き
#define MD_GAME_00_CAMERA3D_INIT_ROT	D3DXVECTOR3(0.0f,D3DX_PI,0.0f)

// MD:ゲーム画面[00] のポーズSE
// MD:ゲーム画面[00] の決定SE
// MD:ゲーム画面[00] のクリアSE
// MD:ゲーム画面[00] のテキストセットSE
#define MD_GAME_00_PAUSE_SE		(SOUND_LABEL_SE_PAUSE_000)
#define MD_GAME_00_RETRY_SE		(SOUND_LABEL_SE_DETERMINATION_000)
#define MD_GAME_00_CLEAR_SE		(SOUND_LABEL_BGM_CLEAR_000)
#define MD_GAME_00_TEXT_SET_SE	(SOUND_LABEL_SE_SELECT_000)

// MD:ゲーム画面[00] のクリア後の余韻
// MD:ゲーム画面[00] のクリア演出にかかる時間
// MD:ゲーム画面[00] のクリアテキストの表示にかかる時間
// MD:ゲーム画面[00] のクリアテキストの位置
// MD:ゲーム画面[00] のクリアテキストの色
// MD:ゲーム画面[00] のクリアテキストの表示形式
// MD:ゲーム画面[00] のクリアテキストの拡大倍率
#define MD_GAME_00_CLEAR_AFTERGLOW		(120)
#define MD_GAME_00_CLEAR_TIME			(420)
#define MD_GAME_00_CLEAR_TEXT_SHOW_TIME	(150)
#define MD_GAME_00_CLEAR_TEXT_POS		D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y,0.0f)
#define MD_GAME_00_CLEAR_TEXT_COLOR		Color{243,191,63,255}
#define MD_GAME_00_CLEAR_TEXT_DISPLAY	"STAGE CLEAR"
#define MD_GAME_00_CLEAR_TEXT_SCALE		(1.5f)

// MD:ゲーム画面[00] のクリアテキスト(影)の出現時間
// MD:ゲーム画面[00] のクリアテキスト(影)の消えるのにかかる時間
// MD:ゲーム画面[00] のクリアテキスト(影)の拡大倍率
#define MD_GAME_00_CLEAR_TEXT_SHADOW_POP_TIME	(340)
#define MD_GAME_00_CLEAR_TEXT_SHADOW_CLEAR_TIME	(60)
#define MD_GAME_00_CLEAR_TEXT_SHADOW_SCALE		(3.0f)

// MD:ゲーム画面[00] のTIPS待ちにかかる時間
#define MD_GAME_00_TIPS_WAIT_TIME	(60)

// MD;ゲーム画面[00] のステージの種類
#define MD_GAME_00_STAGE_TYPE	(0)

//****************************************
// 列挙型の定義
//****************************************
// ゲーム画面[00] のメニュー
typedef enum
{
	MD_GAME_00_MENU_RESULT,	// メイン
	MD_GAME_00_MENU_MAX,
}MD_GAME_00_MENU;

// ゲーム画面[00] のリザルトメニュー
typedef enum
{
	MD_GAME_00_RESULT_MENU_RETRY,			// リトライ
	MD_GAME_00_RESULT_MENU_BACK_TO_TITLE,	// タイトルに戻る
	MD_GAME_00_RESULT_MENU_MAX,
}MD_GAME_00_RESULT_MENU;

// ゲーム画面[00] のポーズメニュー
typedef enum
{
	MD_GAME_00_PAUSE_MENU_RESUME,			// 再開
	MD_GAME_00_PAUSE_MENU_RETRY,			// リトライ
	MD_GAME_00_PAUSE_MENU_USERGUID,			// ユーザーガイド
	MD_GAME_00_PAUSE_MENU_SETTINGS,			// 設定
	MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE,	// タイトルに戻る
	MD_GAME_00_PAUSE_MENU_MAX,
}MD_GAME_00_PAUSE_MENU;

// ゲーム画面[00] のユーザーガイドメニュー
typedef enum
{
	MD_GAME_00_USERGUID_MENU_OK,	// OK
	MD_GAME_00_USERGUID_MENU_MAX,
}MD_GAME_00_USERGUID_MENU;

//****************************************
// プロトタイプ宣言
//****************************************
// MD:ゲーム画面[00] のパラメーターの初期化処理
// MD:ゲーム画面[00] の状態に応じた更新処理
// MD:ゲーム画面[00] の状態に応じた開始処理
// MD:ゲーム画面[00] の状態に応じた終了処理
void InitParameterMd_game_00(void);
void UpdateMd_game_00State(void);
void StartMd_game_00State(void);
void EndMd_game_00State(void);

//****************************************
// グローバル宣言
//****************************************
Md_game_00	g_md_game_00;	// MD:ゲーム画面[00] の情報

// MD:ゲーム画面[00] のリザルトメニュー設定情報
Ui_menu_00Set g_aMd_game_00ResultMenuSet[MD_GAME_00_RESULT_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RETRY"        ,true },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE",true },
};

// MD:ゲーム画面[00] のポーズメニュー設定情報
Ui_menu_00Set g_aMd_game_00PauseMenuSet[MD_GAME_00_PAUSE_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RESUME"       ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"RETRY"        ,false },
	{ UI_MENU_00_TYPE_NORMAL,"CONTROLS"     ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"SETTINGS"     ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE",true  },
};

// MD:ゲーム画面[00] のユーザーガイドメニュー設定情報
Ui_menu_00Set g_aMd_game_00UserGuidSet[MD_GAME_00_USERGUID_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"OK",true },
};

//========== *** MD:ゲーム画面[00] の情報を取得 ***
Md_game_00 *GetMd_game_00(void) 
{
	return &g_md_game_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterMd_game_00関数 - MD:ゲーム画面[00] のパラメーターの初期化処理 -
//========================================
void InitParameterMd_game_00(void)
{
	Md_game_00	*pMd	// MD:ゲーム画面[00] の情報
				= &g_md_game_00;

	pMd->state			= MD_GAME_00_STATE_NONE;	// 状態
	pMd->previousPosR	= INITD3DXVECTOR3;			// 前の注視点の位置
}

//========================================
// StartMd_game_00State関数 - MD:ゲーム画面[00] の状態に応じた開始処理 -
//========================================
void StartMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:ゲーム画面[00] の情報
				= &g_md_game_00;

	// 状態カウンターを初期化
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	case /*/ ポーズ /*/MD_GAME_00_STATE_PAUSE: {
		// UI:メニュー[00] の中心座標を設定
		SetUi_menu_00Pos(MD_GAME_00_PAUSE_MENU_POS);

		// UI:メニュー[00] の設定処理(ポーズメニュー)
		SetUi_menu_00(
			g_aMd_game_00PauseMenuSet,
			MD_GAME_00_PAUSE_MENU_MAX);

		break;
	}
	case /*/ ユーザーガイド /*/MD_GAME_00_STATE_USERGUID: {
		// UI:ユーザーガイド[00] の設定処理
		SetUi_userGuid_00(MD_GAME_00_USERGUID_POS, GetActiveInputType());

		// UI:メニュー[00] の中心座標を設定
		SetUi_menu_00Pos(MD_GAME_00_USERGUID_MENU_POS);

		// UI:メニュー[00] の設定処理(ユーザーガイドメニュー)
		SetUi_menu_00(
			g_aMd_game_00UserGuidSet,
			MD_GAME_00_USERGUID_MENU_MAX);

		break;
	}
	case /*/ クリア演出 /*/MD_GAME_00_STATE_CLEAR: {
		
		break;
	}
	case /*/ ランキング /*/MD_GAME_00_STATE_RANKING: {
		// UI:ランキングフレーム[00] の名前入力設定処理
		SetNameEntryUi_rankingFrame_00(SetScore(GetChr_player_00()->nScore));

		// UI:ランキングフレーム[00] の設定処理
		SetUi_rankingFrame_00(MD_GAME_00_RANKING_POS);

		break;
	}
	case /*/ リザルト /*/MD_GAME_00_STATE_RESULT: {
		SetStateObj_stage_00(			// OBJ:ステージ[00] の状態を停止にする
			OBJ_STAGE_00_STATE_STOP);
		SetChr_enemy_00ControlState(	// CHR:敵[00] の管理状態を静的にする
			CHR_ENEMY_00_CONTROL_STATE_STATIC);
		SetChr_player_00ControlState(	// CHR:プレイヤー[00] の状態を静的にする
			CHR_PLAYER_00_CONTROL_STATE_STATIC);

		// UI:メニュー[00] の中心座標を設定
		SetUi_menu_00Pos(MD_GAME_00_RESULT_MENU_POS);

		// UI:メニュー[00] の設定処理(リザルトメニュー)
		SetUi_menu_00(
			g_aMd_game_00ResultMenuSet,
			MD_GAME_00_RESULT_MENU_MAX);

		break;
	}
	case /*/ チュートリアル[00] /*/MD_GAME_00_STATE_TUTORIAL_00: {
		SetChr_player_00ControlState(	// CHR:プレイヤー[00] の管理状態を入力停止にする
			CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP);

		// カメラ(3D)の注視点をOBJ:コア[00] に設定
		SetCamera3DPosR(
			GetObj_core_00()->pos +
			D3DXVECTOR3(0.0f, MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_R_HEIGHT, 0.0f));

		// カメラ(3D)の高さ/向き/距離を設定
		GetCamera3D()->fHeight	= MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_V_HEIGHT;
		GetCamera3D()->rot.y	= MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ROT;
		GetCamera3D()->fLength	= MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH;

		break;
	}
	case /*/ チュートリアル[01] /*/MD_GAME_00_STATE_TUTORIAL_01: {
		// UI:ダイアログボックス[00] の設定処理(種類:02)
		SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_02);

		// カメラ(3D)の注視点をCHR:プレイヤー[00] に設定
		SetCamera3DPosR(
			GetChr_player_00()->pos +
			D3DXVECTOR3(0.0f, MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_R_HEIGHT, 0.0f));

		// カメラ(3D)の高さ/距離を設定
		GetCamera3D()->fHeight = MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_V_HEIGHT;
		GetCamera3D()->fLength = MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_LENGTH;

		break;
	}
	case /*/ ボス出現ムービー(視点移動) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE: {
		SetChr_player_00ControlState(	// CHR:プレイヤー[00] の管理状態を静的にする
			CHR_PLAYER_00_CONTROL_STATE_STATIC);
		SetChr_enemy_00ControlState(	// CHR:敵[00] の管理状態をボスアピールにする
			CHR_ENEMY_00_CONTROL_STATE_BOSS_APPEAL);

		// 前の注視点の位置を保存
		pMd->previousPosR = GetCamera3D()->posR;

		break;
	}
	case /*/ ボス出現ムービー(出現) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_POP: {
		// OBJ:ステージ[00] のボス設定処理
		BossSetObj_stage_00();

		break;
	}
	case /*/ ボス出現ムービー(視点リセット) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET: {

		break;
	}
	case /*/ コア破壊ムービー(拡大) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM: {
		SetStateObj_stage_00(				// OBJ:ステージ[00] の状態を停止にする
			OBJ_STAGE_00_STATE_STOP);
		SetObj_turret_00ControlState(		// OBJ:タレット[00] の管理状態を静的にする
			OBJ_TURRET_00_CONTROL_STATE_STATIC);
		SetObj_discharger_00ControlState(	// OBJ:放電装置[00] の管理状態を静的にする
			OBJ_DISCHARGER_00_CONTROL_STATE_STATIC);
		SetChr_enemy_00ControlState(		// CHR:敵[00] の管理状態を静的にする
			CHR_ENEMY_00_CONTROL_STATE_STATIC);
		SetChr_player_00ControlState(		// CHR:プレイヤー[00] の管理状態を静的にする
			CHR_PLAYER_00_CONTROL_STATE_STATIC);

		// カメラ(3D)の注視点をOBJ:コア[00] に設定
		SetCamera3DPosR(
			GetObj_core_00()->pos + 
			D3DXVECTOR3(0.0f, MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_R_HEIGHT, 0.0f));

		// カメラ(3D)の高さを設定
		GetCamera3D()->fHeight = MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_V_HEIGHT;

		break;
	}
	case /*/ コア破壊ムービー(爆発) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION: {
		DestroyObj_core_00();		// OBJ:コア[00] の破壊処理
		SetColorLight3D(INITCOLOR);	// OBJ:コア[00] の色を初期化

		break;
	}
	}
}

//========================================
// EndMd_game_00State関数 - MD:ゲーム画面[00] の状態に応じた終了処理 -
//========================================
void EndMd_game_00State(void) 
{
	Md_game_00	*pMd	// MD:ゲーム画面[00] の情報
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	case /*/ ポーズ /*/MD_GAME_00_STATE_PAUSE: {

		break;
	}
	case /*/ ユーザーガイド /*/MD_GAME_00_STATE_USERGUID: {
		
		break;
	}
	case /*/ クリア演出 /*/MD_GAME_00_STATE_CLEAR: {
		SetChr_player_00ControlState(	// CHR:プレイヤー[00] の管理状態を静的にする
			CHR_PLAYER_00_CONTROL_STATE_STATIC);

		break;
	}
	case /*/ ランキング /*/MD_GAME_00_STATE_RANKING: {

		break;
	}
	case /*/ リザルト /*/MD_GAME_00_STATE_RESULT: {

		break;
	}
	case /*/ チュートリアル[00] /*/MD_GAME_00_STATE_TUTORIAL_00: {

		break;
	}
	case /*/ チュートリアル[01] /*/MD_GAME_00_STATE_TUTORIAL_01: {
		SetChr_player_00ControlState(	// CHR:プレイヤー[00] の管理状態を動的にする
			CHR_PLAYER_00_CONTROL_STATE_DYNAMIC);

		// カメラ(3D)のパラメーター初期化
		InitParameterCamera3D();

		break;
	}
	case /*/ ボス出現ムービー(視点移動) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE: {

		break;
	}
	case /*/ ボス出現ムービー(出現) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_POP: {

		break;
	}
	case /*/ ボス出現ムービー(視点リセット) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET: {
		SetChr_player_00ControlState(	// CHR:プレイヤー[00] の状態を動的にする
			CHR_PLAYER_00_CONTROL_STATE_DYNAMIC);
		SetChr_enemy_00ControlState(	// CHR:敵[00] の管理状態を動的にする
			CHR_ENEMY_00_CONTROL_STATE_DYNAMIC);

		break;
	}
	case /*/ コア破壊ムービー(拡大) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM: {
		// BGMを一括停止
		StopSoundType(SOUND_TYPE_BGM);

		// BGMの音量を元に戻す
		SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f);
		
		break;
	}
	case /*/ コア破壊ムービー(爆発) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION: {

		break;
	}
	}
}

//========================================
// UpdateMd_game_00State関数 - MD:ゲーム画面[00] の状態に応じた更新処理 -
//========================================
void UpdateMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:ゲーム画面[00] の情報
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_GAME_00_STATE_NORMAL: {
		if (GetObj_core_00()->nHP <= 0)
		{// OBJ:コア[00] のHPが0以下の時、
			SetMd_game_00State(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM);	// 状態をコア破壊ムービー(拡大)に設定
			break;
		}

		if ((GetKeyboardTrigger(DIK_P)) || (GetButtonTrigger(BUTTON_START))) 
		{// ポーズキーが押された時、
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);	// 状態をポーズに設定
			PlaySound(MD_GAME_00_PAUSE_SE);				// ポーズSE再生
		}

		break;
	}
	case /*/ ポーズ /*/MD_GAME_00_STATE_PAUSE: {
		// ポーズメニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ 再開 /*/MD_GAME_00_PAUSE_MENU_RESUME: {
			// 状態を通常に設定
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
			break;
		}
		case /*/ リトライ /*/MD_GAME_00_PAUSE_MENU_RETRY: {
			// 画面をゲーム画面[00] に設定
			SetFade(MODE_GAME_00);

			// リトライSEを再生
			PlaySound(MD_GAME_00_RETRY_SE);
			break;
		}
		case /*/ ユーザーガイド /*/MD_GAME_00_PAUSE_MENU_USERGUID: {
			// 状態をユーザーガイドにする
			SetMd_game_00State(MD_GAME_00_STATE_USERGUID);
			break;
		}
		case /*/ 設定 /*/MD_GAME_00_PAUSE_MENU_SETTINGS: {
			// UI:メニュー[00] の中心座標を設定
			SetUi_menu_00Pos(MD_GAME_00_SETTING_MENU_POS);

			// 設定メニューの設定処理
			SetSettingMenu();

			// 状態を設定にする
			SetMd_game_00State(MD_GAME_00_STATE_SETTING);
			break;
		}
		case /*/ タイトルに戻る /*/MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE: {
			// 画面をタイトル画面[00] に設定
			SetFade(MODE_TITLE_00);
			break;
		}
		}

		break;
	}
	case /*/ ユーザーガイド /*/MD_GAME_00_STATE_USERGUID: {
		// ユーザーガイドメニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ OK /*/MD_GAME_00_USERGUID_MENU_OK:
			// UI:ユーザーガイド[00] の状態を消え中にする
			SetStateUi_userGuid_00(UI_USERGUID_00_STATE_IN_CLEAR);
			break;
		}

		if (GetUi_userGuid_00()->state == UI_USERGUID_00_STATE_CLEAR)
		{// UI:ユーザーガイド[00] の状態が消えている時、
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);	// 状態をポーズにする
		}

		break;
	}
	case /*/ 設定 /*/MD_GAME_00_STATE_SETTING: {
		// 設定メニューの更新処理
		UpdateSettingMenu();

		if (!*GetSettingNow())
		{// 設定中フラグが偽の時、
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);	// 状態をポーズにする
		}

		break;
	}
	case /*/ クリア演出 /*/MD_GAME_00_STATE_CLEAR: {
		pMd->nCounterState++;	// 状態カウンターを加算

		if (pMd->nCounterState == MD_GAME_00_CLEAR_AFTERGLOW) 
		{// 状態のカウンターがクリア後の余韻に達した時、
			PlaySound(MD_GAME_00_CLEAR_SE);	// クリアSEを再生
		}

		if (pMd->nCounterState >= MD_GAME_00_CLEAR_TIME) 
		{// 状態のカウンターがクリア演出にかかる時間に達した時、
			SetMd_game_00State(MD_GAME_00_STATE_RANKING);	// 状態をランキングに設定
		}

		if (pMd->nCounterState >= MD_GAME_00_CLEAR_AFTERGLOW)
		{// 状態のカウンターがクリア後の余韻以上の時、
			{//========== *** クリアテキスト ***
				int nTextCounter = IntControlReturn(pMd->nCounterState - MD_GAME_00_CLEAR_AFTERGLOW, MD_GAME_00_CLEAR_TEXT_SHOW_TIME, 0);
				int nStrLen = strlen(MD_GAME_00_CLEAR_TEXT_DISPLAY);
				int nTextTime = MD_GAME_00_CLEAR_TEXT_SHOW_TIME / nStrLen;

				if ((nTextCounter % nTextTime == 0) &&
					(nTextCounter / nTextTime < nStrLen))
				{// テキストカウンターがクリアテキストにかかる時間で割り切れる時、
					PlaySound(MD_GAME_00_TEXT_SET_SE);	// テキストセットSEを再生
				}

				char aString[TXT_MAX];
				sprintf(aString, MD_GAME_00_CLEAR_TEXT_DISPLAY);
				int nText = (nTextCounter / nTextTime) + 1;
				IntControl(&nText, nStrLen, 0);
				aString[(nTextCounter / nTextTime) + 1] = '\0';
				SetText2D(
					aString,
					FONT_003,
					DISPLAY_CENTER,
					MD_GAME_00_CLEAR_TEXT_POS,
					MD_GAME_00_CLEAR_TEXT_COLOR,
					MD_GAME_00_CLEAR_TEXT_SCALE,
					MD_GAME_00_CLEAR_TEXT_SCALE);
			}
			if(pMd->nCounterState >= MD_GAME_00_CLEAR_TEXT_SHADOW_POP_TIME)
			{//========== *** クリアテキスト(影) ***
				int nShadowCounter = IntControlReturn(pMd->nCounterState - MD_GAME_00_CLEAR_TEXT_SHADOW_POP_TIME, MD_GAME_00_CLEAR_TEXT_SHADOW_CLEAR_TIME, 0);
				float fRate = (float)nShadowCounter / (float)MD_GAME_00_CLEAR_TEXT_SHADOW_CLEAR_TIME;
				Color setCol = MD_GAME_00_CLEAR_TEXT_COLOR; setCol.a *= (1.0f - fRate);
				float fScale = 
					(MD_GAME_00_CLEAR_TEXT_SCALE * (1.0f - fRate)) +
					(MD_GAME_00_CLEAR_TEXT_SHADOW_SCALE * fRate);

				SetText2D(
					MD_GAME_00_CLEAR_TEXT_DISPLAY,
					FONT_001,
					DISPLAY_CENTER,
					MD_GAME_00_CLEAR_TEXT_POS,
					setCol,
					fScale,
					fScale);
			}
		}

		break;
	}
	case /*/ ランキング /*/MD_GAME_00_STATE_RANKING: {
		if (GetUi_rankingFrame_00()->state == UI_RANKINGFRAME_00_STATE_POP)
		{// UI:ランキングフレーム[00] の状態が出現の時、
			SetMd_game_00State(MD_GAME_00_STATE_RESULT);	// 状態をリザルトにする
		}

		break;
	}
	case /*/ リザルト /*/MD_GAME_00_STATE_RESULT: {
		// リザルトメニューの選択処理
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ リトライ /*/MD_GAME_00_RESULT_MENU_RETRY: {
			// 画面をゲーム画面[00] に設定
			SetFade(MODE_GAME_00);
			break;
		}
		case /*/ タイトルに戻る /*/MD_GAME_00_RESULT_MENU_BACK_TO_TITLE: {
			// 画面をタイトル画面[00] に設定
			SetFade(MODE_TITLE_00);
			break;
		}
		}
		break;
	}
	case /*/ チュートリアル[00] /*/MD_GAME_00_STATE_TUTORIAL_00: {
		float	fRate	// カウンターの進行率
				= (float)pMd->nCounterState / (float)MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME;

		// カメラ(3D)の距離を更新
		GetCamera3D()->fLength =
			(MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH * (1.0f - fRate)) +
			(MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_END_LENGTH * fRate);

		pMd->nCounterState++;	// 状態カウンターを加算
		IntControl(				// 状態カウンターを制御
			&pMd->nCounterState,
			MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME, 0);

		if (pMd->nCounterState == MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME)
		{// 状態カウンターが拡大にかかる時間に達している時、
			if (GetUi_dialogBox_00()->state == UI_DIALOGBOX_00_STATE_CLEAR)
			{//  UI:ダイアログボックス[00] の状態が消えている時
				switch (GetUi_dialogBox_00()->type)
				{
				case /*/ 無し /*/UI_DIALOGBOX_00_TYPE_NONE:
					// UI:ダイアログボックス[00] の設定処理(種類:00)
					SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_00);
					break;
				case /*/ 00 /*/UI_DIALOGBOX_00_TYPE_00:
					// UI:ダイアログボックス[00] の設定処理(種類:01)
					SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_01);
					break;
				case /*/ 01 /*/UI_DIALOGBOX_00_TYPE_01:
					// 状態をチュートリアル[01]に設定
					SetMd_game_00State(MD_GAME_00_STATE_TUTORIAL_01);
					break;
				}
			}
		}

		break;
	}
	case /*/ チュートリアル[01] /*/MD_GAME_00_STATE_TUTORIAL_01: {
		if ((GetUi_dialogBox_00()->state == UI_DIALOGBOX_00_STATE_CLEAR)
			&&
			(GetFade() == FADE_NONE))
		{//  UI:ダイアログボックス[00] の状態が消えている & フェード無しの時、
			switch (GetUi_dialogBox_00()->type)
			{
			case /*/ 02 /*/UI_DIALOGBOX_00_TYPE_02:
				// UI:ダイアログボックス[00] の設定処理(種類:03)
				SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_03);
				break;
			case /*/ 03 /*/UI_DIALOGBOX_00_TYPE_03:
				// UI:ダイアログボックス[00] の設定処理(種類:04)
				SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_04);
				break;
			case /*/ 04 /*/UI_DIALOGBOX_00_TYPE_04:
				SetFade(MODE_NONE);	// フェード設定
				break;
			}
		}

		pMd->nCounterState++;	// 状態カウンターを加算
		IntControl(				// 状態カウンターを制御
			&pMd->nCounterState,
			MD_GAME_00_STATE_TUTORIAL_01_TIME, 0);

		if (GetFade() == FADE_IN) 
		{// フェードイン状態の時、
			// 状態を通常に設定
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

			// カメラ(3D)の向きを設定
			GetCamera3D()->rot = MD_GAME_00_CAMERA3D_INIT_ROT;
		}

		break;
	}
	case /*/ ボス出現ムービー(視点移動) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE: {
		float	fRate	// カウンターの進行率
				= ((float)pMd->nCounterState / (float)MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE_TIME);

		// カメラ(3D)の注視点を現在の注視点からボス設定位置に推移させる
		GetCamera3D()->posR =
			(GetObj_stage_00()->setBoss.pos * fRate) +
			(GetCamera3D()->posR * (1.0f - fRate));

		if (++pMd->nCounterState >= MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE_TIME)
		{// 状態カウンターを加算した結果ボス出現ムービー(視点移動)にかかる時間に達した時、
			SetMd_game_00State(MD_GAME_00_STATE_BOSS_POP_MOVIE_POP);	// 状態をボス出現ムービー(出現)に設定
		}

		break;
	}
	case /*/ ボス出現ムービー(出現) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_POP: {
		
		break;
	}
	case /*/ ボス出現ムービー(視点リセット) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET: {
		float	fRate	// カウンターの進行率
				= ((float)pMd->nCounterState / (float)MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET_TIME);

		// カメラ(3D)の注視点を現在の注視点から前の注視点の位置に推移させる
		GetCamera3D()->posR =
			(pMd->previousPosR * fRate) +
			(GetCamera3D()->posR * (1.0f - fRate));

		if (++pMd->nCounterState >= MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET_TIME)
		{// 状態カウンターを加算した結果ボス出現ムービー(視点リセット)にかかる時間に達した時、
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);	// 状態を通常に設定
		}

		break;
	}
	case /*/ コア破壊ムービー(拡大) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM: {
		float	fRate	// 割合
				= ((float)pMd->nCounterState / (float)MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_TIME);

		// 光(3D)の明るさを設定
		SetBrightnessLight3D(1.0f - fRate);

		if (++pMd->nCounterState >= MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_TIME) 
		{// 状態カウンターを加算した結果コア破壊ムービー(拡大)にかかる時間に達した時、
			SetMd_game_00State(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION);	// 状態をコア破壊ムービー(爆発)に設定
		}

		// BGMの音量を更新
		SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - fRate);

		break;
	}
	case /*/ コア破壊ムービー(爆発) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION: {
		float	fRate	// 割合
				= Prus(
				(float)(pMd->nCounterState - MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_BRIGHT_TIME) /
				(float)(MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_TIME - MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_BRIGHT_TIME));

		// 光(3D)の明るさを設定
		SetBrightnessLight3D(fRate);

		if (++pMd->nCounterState >= MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_TIME)
		{// 状態カウンターを加算した結果コア破壊ムービー(爆発)にかかる時間に達した時、
			SetMd_game_00State(MD_GAME_00_STATE_RANKING);	// 状態をランキングに設定
		}

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
// InitMd_game_00関数 - MD:ゲーム画面[00]の初期化処理 -
//========================================
void InitMd_game_00(void)
{
	// パラメーターの初期化処理
	InitParameterMd_game_00();

	// 初期化処理
	InitParameterCamera3D();	// カメラ(3D)のパラメーター
	InitAtk_bullet_00();		// ATK:弾					[00]
	InitAtk_impact_00();		// ATK:衝撃					[00]
	InitAtk_sword_00();			// ATK:剣					[00]
	InitBg_mountain_00();		// BG :山					[00]
	InitBg_space_00();			// BG :宇宙					[00]
	InitChr_player_00();		// CHR:プレイヤー			[00]
	InitChr_enemy_00();			// CHR:敵					[00]
	InitEff_explosion_00();		// EFF:爆発					[00]
	InitEff_light_00();			// EFF:光					[00]
	InitEff_particle_00();		// EFF:パーティクル			[00]
	InitEff_screen_00();		// EFF:スクリーン			[00]
	InitEff_shadow_00();		// EFF:影					[00]
	InitEff_shockWave_00();		// EFF:衝撃波				[00]
	InitItm_coin_00();			// ITM:コイン				[00]
	InitObj_block_00();			// OBJ:ブロック				[00]
	InitObj_core_00();			// OBJ:コア					[00]
	InitObj_discharger_00();	// OBJ:放電装置				[00]
	InitObj_mirror_00();		// OBJ:ミラー				[00]
	InitObj_pedestal_00();		// OBJ:台座					[00]
	InitUi_attentionMark_00();	// UI :注意マーク			[00]
	InitObj_turret_00();		// OBJ:タレット				[00]
	InitObj_switch_00();		// OBJ:スイッチ				[00]
	InitObj_stage_00(MD_GAME_00_STAGE_TYPE);	// OBJ:ステージ				[00]
	InitObj_target_00();		// OBJ:ターゲット			[00]
	InitUi_inputGuid_00();		// UI :入力ガイド			[00]
	InitUi_coinFrame_00();		// UI :コインフレーム		[00]
	InitUi_damage_00();			// UI :ダメージ				[00]
	InitUi_dialogBox_00();		// UI :ダイアログボックス	[00]
	InitUi_frame_00();			// UI :フレーム				[00]
	InitUi_hpBar_00();			// UI :HPバー				[00]
	InitUi_hpBar_01();			// UI :HPバー				[01]
	InitUi_inventory_00();		// UI :インベントリ			[00]
	InitUi_itemGuid_00();		// UI :商品説明				[00]
	InitUi_itemIcon_00();		// UI :アイテムアイコン		[00]
	InitUi_map_00();			// UI :マップ				[00]
	InitUi_parameter_00();		// UI :パラメーター			[00]
	InitUi_menu_00();			// UI :メニュー				[00]
	InitUi_rankingFrame_00();	// UI :ランキングフレーム	[00]
	InitUi_tips_00();			// UI :TIPS					[00]
	InitUi_userGuid_00();		// UI :ユーザーガイド		[00]
	InitUi_warningText_00();	// UI :注意テキスト			[00]

	// 状態をチュートリアル[00]に設定
	SetMd_game_00State(MD_GAME_00_STATE_TUTORIAL_00);

	SetColorLight3D(INITCOLOR);	// OBJ:コア[00] の色を初期化
}

//========================================
// UninitMd_game_00関数 - MD:ゲーム画面[00]の終了処理 -
//========================================
void UninitMd_game_00(void)
{
	// 終了処理
	UninitAtk_bullet_00();			// ATK:弾					[00]
	UninitAtk_impact_00();			// ATK:衝撃					[00]
	UninitAtk_sword_00();			// ATK:剣					[00]
	UninitBg_mountain_00();			// BG :山					[00]
	UninitBg_space_00();			// BG :宇宙					[00]
	UninitChr_player_00();			// CHR:プレイヤー			[00]
	UninitChr_enemy_00();			// CHR:敵					[00]
	UninitEff_explosion_00();		// EFF:爆発					[00]
	UninitEff_light_00();			// EFF:光					[00]
	UninitEff_particle_00();		// EFF:パーティクル			[00]
	UninitEff_screen_00();			// EFF:スクリーン			[00]
	UninitEff_shadow_00();			// EFF:影					[00]
	UninitEff_shockWave_00();		// EFF:衝撃波				[00]
	UninitItm_coin_00();			// ITM:コイン				[00]
	UninitObj_block_00();			// OBJ:ブロック				[00]
	UninitObj_core_00();			// OBJ:コア					[00]
	UninitObj_discharger_00();		// OBJ:放電装置				[00]
	UninitObj_mirror_00();			// OBJ:ミラー				[00]
	UninitObj_pedestal_00();		// OBJ:台座					[00]
	UninitObj_stage_00();			// OBJ:ステージ				[00]
	UninitObj_switch_00();			// OBJ:スイッチ				[00]
	UninitObj_target_00();			// OBJ:ターゲット			[00]
	UninitObj_turret_00();			// OBJ:タレット				[00]
	UninitUi_attentionMark_00();	// UI :注意マーク			[00]
	UninitUi_inputGuid_00();		// UI :入力ガイド			[00]
	UninitUi_coinFrame_00();		// UI :コインフレーム		[00]
	UninitUi_damage_00();			// UI :ダメージ				[00]
	UninitUi_dialogBox_00();		// UI :ダイアログボックス	[00]
	UninitUi_frame_00();			// UI :フレーム				[00]
	UninitUi_hpBar_00();			// UI :HPバー				[00]
	UninitUi_hpBar_01();			// UI :HPバー				[01]
	UninitUi_inventory_00();		// UI :インベントリ			[00]
	UninitUi_itemGuid_00();			// UI :商品説明				[00]
	UninitUi_itemIcon_00();			// UI :アイテムアイコン		[00]
	UninitUi_map_00();				// UI :マップ				[00]
	UninitUi_parameter_00();		// UI :パラメーター			[00]
	UninitUi_menu_00();				// UI :メニュー				[00]
	UninitUi_rankingFrame_00();		// UI :ランキングフレーム	[00]
	UninitUi_tips_00();				// UI :TIPS					[00]
	UninitUi_userGuid_00();			// UI :ユーザーガイド		[00]
	UninitUi_warningText_00();		// UI :注意テキスト			[00]
}

//========================================
// UpdateMd_game_00関数 - MD:ゲーム画面[00]の更新処理 -
//========================================
void UpdateMd_game_00(void)
{
	UpdateUi_frame_00();				// UI :フレーム				[00]
	UpdateUi_menu_00();					// UI :メニュー				[00]
	if ((g_md_game_00.state != MD_GAME_00_STATE_PAUSE) &&
		(g_md_game_00.state != MD_GAME_00_STATE_USERGUID) && 
		(g_md_game_00.state != MD_GAME_00_STATE_SETTING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RANKING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RESULT))
	{// 状態がポーズ/ユーザーガイド/設定/ランキング/リザルトで無い時、
		// 更新処理
		UpdateBg_mountain_00();			// BG :山					[00]
		UpdateBg_space_00();			// BG :宇宙					[00]
		UpdateItm_coin_00();			// ITM:コイン				[00]
		UpdateChr_enemy_00();			// CHR:敵					[00]
		UpdateChr_player_00();			// CHR:プレイヤー			[00]
		UpdateAtk_sword_00();			// ATK:剣					[00]
		UpdateEff_explosion_00();		// EFF:爆発					[00]
		UpdateEff_light_00();			// EFF:光					[00]
		UpdateEff_particle_00();		// EFF:パーティクル			[00]
		UpdateEff_screen_00();			// EFF:スクリーン			[00]
		UpdateEff_shadow_00();			// EFF:影					[00]
		UpdateEff_shockWave_00();		// EFF:衝撃波				[00]
		UpdateAtk_bullet_00();			// ATK:弾					[00]
		UpdateAtk_impact_00();			// ATK:衝撃					[00]
		UpdateObj_block_00();			// OBJ:ブロック				[00]
		UpdateObj_core_00();			// OBJ:コア					[00]
		UpdateObj_discharger_00();		// OBJ:放電装置				[00]
		UpdateObj_mirror_00();			// OBJ:ミラー				[00]
		UpdateObj_stage_00();			// OBJ:ステージ				[00]
		UpdateObj_target_00();			// OBJ:ターゲット			[00]
		UpdateObj_turret_00();			// OBJ:タレット				[00]
		UpdateObj_pedestal_00();		// OBJ:台座					[00]
		UpdateUi_attentionMark_00();	// UI :注意マーク			[00]
		UpdateUi_inputGuid_00();		// UI :入力ガイド			[00]
		UpdateUi_itemGuid_00();			// UI :商品説明				[00]
		UpdateUi_itemIcon_00();			// UI :アイテムアイコン		[00]
		UpdateObj_switch_00();			// OBJ:スイッチ				[00]
		if (g_md_game_00.state == MD_GAME_00_STATE_NORMAL)
		{// MD:ゲーム画面[00] の状態が通常の時、
			UpdateUi_coinFrame_00();	// UI :コインフレーム		[00]
			UpdateUi_damage_00();		// UI :ダメージ				[00]
			UpdateUi_hpBar_00();		// UI :HPバー				[00]
			UpdateUi_hpBar_01();		// UI :HPバー				[01]
			UpdateUi_inventory_00();	// UI :インベントリ			[00]
			UpdateUi_map_00();			// UI :マップ				[00]
			UpdateUi_parameter_00();	// UI :パラメーター			[00]
		}
		UpdateUi_dialogBox_00();		// UI :ダイアログボックス	[00]
		UpdateUi_warningText_00();		// UI :注意テキスト			[00]
	}
	UpdateUi_rankingFrame_00();			// UI :ランキングフレーム	[00]
	UpdateUi_tips_00();					// UI :TIPS					[00]
	UpdateUi_userGuid_00();				// UI :ユーザーガイド		[00]

	// MD:ゲーム画面[00] の状態に応じた更新処理
	UpdateMd_game_00State();
}

//========================================
// DrawMd_game_00関数 - MD:ゲーム画面[00]の描画処理 -
//========================================
void DrawMd_game_00(void)
{
	// 描画処理
	DrawAtk_bullet_00();			// ATK:弾					[00]
	DrawAtk_sword_00();				// ATK:剣					[00]
	DrawBg_space_00();				// BG :宇宙					[00]
	DrawBg_mountain_00();			// BG :山					[00]
	DrawChr_enemy_00();				// CHR:敵					[00]
	DrawObj_block_00();				// OBJ:ブロック				[00]
	DrawObj_core_00();				// OBJ:コア					[00]
	DrawObj_discharger_00();		// OBJ:放電装置				[00]
	DrawObj_pedestal_00();			// OBJ:台座					[00]
	DrawObj_stage_00();				// OBJ:ステージ				[00]
	DrawObj_switch_00();			// OBJ:スイッチ				[00]
	DrawObj_target_00();			// OBJ:ターゲット			[00]
	DrawObj_turret_00();			// OBJ:タレット				[00]
	DrawChr_player_00();			// CHR:プレイヤー			[00]
	DrawEff_explosion_00();			// EFF:爆発					[00]
	DrawEff_light_00();				// EFF:光					[00]
	DrawEff_shadow_00();			// EFF:影					[00]
	DrawEff_shockWave_00();			// EFF:衝撃波				[00]
	DrawItm_coin_00();				// ITM:コイン				[00]
	DrawObj_mirror_00();			// OBJ:ミラー				[00]
	DrawUi_attentionMark_00();		// UI :注意マーク			[00]
	DrawUi_menu_00();				// UI :メニュー				[00]
	if ((g_md_game_00.state != MD_GAME_00_STATE_PAUSE) &&
		(g_md_game_00.state != MD_GAME_00_STATE_USERGUID) &&
		(g_md_game_00.state != MD_GAME_00_STATE_SETTING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RANKING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RESULT))
	{// 状態がポーズ/ユーザーガイド/設定/ランキング/リザルトで無い時、
		DrawUi_itemIcon_00();		// UI :アイテムアイコン		[00]
	}
	DrawText3D();					// テキスト(3D)
	if (g_md_game_00.state == MD_GAME_00_STATE_NORMAL)
	{// MD:ゲーム画面[00] の状態が通常の時、
		DrawUi_hpBar_01();			// UI :HPバー				[01]
		DrawUi_hpBar_00();			// UI :HPバー				[00]
		DrawUi_coinFrame_00();		// UI :コインフレーム		[00]
		DrawUi_inventory_00();		// UI :インベントリ			[00]
		DrawUi_map_00();			// UI :マップ				[00]
		DrawUi_parameter_00();		// UI :パラメーター			[00]
	}
	DrawUi_dialogBox_00();			// UI :ダイアログボックス	[00]
	DrawUi_rankingFrame_00();		// UI :ランキングフレーム	[00]
	if (g_md_game_00.state == MD_GAME_00_STATE_NORMAL)
	{// MD:ゲーム画面[00] の状態が通常の時、
		DrawUi_warningText_00();	// UI :注意テキスト			[00]
		DrawUi_itemGuid_00();		// UI :商品説明				[00]
	}
	DrawUi_frame_00();				// UI :フレーム				[00]
	if ((g_md_game_00.state != MD_GAME_00_STATE_PAUSE) &&
		(g_md_game_00.state != MD_GAME_00_STATE_USERGUID) &&
		(g_md_game_00.state != MD_GAME_00_STATE_SETTING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RANKING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RESULT))
	{// 状態がポーズ/ユーザーガイド/設定/ランキング/リザルトで無い時、
		DrawUi_inputGuid_00();		// UI :入力ガイド			[00]
	}
	DrawUi_tips_00();				// UI :TIPS					[00]
	DrawUi_userGuid_00();			// UI :ユーザーガイド		[00]
	DrawText2D();					// テキスト(2D)
	DrawEff_screen_00();			// EFF:スクリーン			[00]
}

//========================================
// SetMd_game_00State関数 - MD:ゲーム画面[00] の状態を設定 -
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state)
{
	// 状態のカウンターを初期化
	g_md_game_00.nCounterState = 0;

	// MD:ゲーム画面[00] の状態に応じた終了処理
	EndMd_game_00State();

	// MD:ゲーム画面[00] の状態を代入
	g_md_game_00.state = state;

	// MD:ゲーム画面[00] の状態に応じた開始処理
	StartMd_game_00State();
}