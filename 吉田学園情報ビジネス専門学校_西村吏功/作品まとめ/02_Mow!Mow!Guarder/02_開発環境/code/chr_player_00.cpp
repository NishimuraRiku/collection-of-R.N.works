//========================================
// 
// CHR:プレイヤー[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_player_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "eff_screen_00.h"		// EFF:スクリーン	[00]
#include "eff_shadow_00.h"		// EFF:影			[00]
#include "md_game_00.h"			// MD :ゲーム画面	[00]
#include "atk_bullet_00.h"		// ATK:弾			[00]
#include "atk_sword_00.h"		// ATK:剣			[00]
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "itm_coin_00.h"		// ITM:コイン		[00]
#include "obj_block_00.h"		// OBJ:ブロック		[00]
#include "obj_core_00.h"		// OBJ:コア			[00]
#include "obj_discharger_00.h"	// OBJ:放電装置		[00]
#include "obj_mirror_00.h"		// OBJ:ミラー		[00]
#include "obj_signboard_00.h"	// OBJ:看板			[00]
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include "obj_switch_00.h"		// OBJ:スイッチ		[00]
#include "obj_turret_00.h"		// OBJ:タレット		[00]
#include "ui_damage_00.h"		// UI :ダメージ		[00]
#include "ui_tips_00.h"			// UI :TIPS			[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// CHR:プレイヤー[00] の種類毎の情報のデータファイルの相対パス
#define CHR_PLAYER_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\CHR_PLAYER_00_TYPE_DATA.txt"

// CHR:プレイヤー[00] の種類の数
#define CHR_PLAYER_00_TYPE_NUM	(1)

// CHR:プレイヤー[00] の回転倍率
// CHR:プレイヤー[00] の力の減衰倍率
// CHR:プレイヤー[00] の移動量の減衰倍率
// CHR:プレイヤー[00] の斬撃時に加わる力の倍率
#define CHR_PLAYER_00_ROT_DIAMETER			(0.5f)
#define CHR_PLAYER_00_FORCE_DAMP			(0.8f)
#define CHR_PLAYER_00_MOVE_DAMP				(0.8f)
#define CHR_PLAYER_00_SLASH_FORCE_DIAMETER	(2.0f)

// CHR:プレイヤー[00] の初期向き
// CHR:プレイヤー[00] の初期選択アイテム
// CHR:プレイヤー[00] の初期所持コイン
#define CHR_PLAYER_00_INIT_ROT			D3DXVECTOR3(0.0f,D3DX_PI,0.0f)
#define CHR_PLAYER_00_INIT_SELECT_ITEM	(CHR_PLAYER_00_ITEM_SWORD)
#define CHR_PLAYER_00_INIT_COIN			(100)

// CHR:プレイヤー[00] の注視点の相対位置
// CHR:プレイヤー[00] の狙っている時の注視点の相対位置
// CHR:プレイヤー[00] の狙っている時の注視点の相対位置の移動にかかる時間
// CHR:プレイヤー[00] の注視点の相対位置の移動倍率
#define CHR_PLAYER_00_RELATIVE_POS_R				(D3DXVECTOR3(0.0f,15.0f,0.0f))
#define CHR_PLAYER_00_AIM_RELATIVE_POS_R			(D3DXVECTOR3(0.0f,20.0f,0.0f))
#define CHR_PLAYER_00_AIM_TIME						(30)
#define CHR_PLAYER_00_RELATIVE_POS_R_MOVE_DIAMETER	(0.25f)

// CHR:プレイヤー[00] の無敵時間
// CHR:プレイヤー[00] の無敵時の点滅にかかる時間
// CHR:プレイヤー[00] の復活後の無敵時間
// CHR:プレイヤー[00] のダメージ時間
// CHR:プレイヤー[00] のダメージ色
// CHR:プレイヤー[00] のダメージ時の振動
#define CHR_PLAYER_00_INVINCIBLE_TIME				(10)
#define CHR_PLAYER_00_INVINCIBLE_BLINK_TIME			(2)
#define CHR_PLAYER_00_AFTER_RESPAWN_INVINCIBLE_TIME	(60)
#define CHR_PLAYER_00_DAMAGE_TIME					(10)
#define CHR_PLAYER_00_DAMAGE_COLOR					Color{166,0,0,0}
#define CHR_PLAYER_00_DAMAGE_VIBRATION				(4.0f)

// CHR:プレイヤー[00] のOBJ:ミラー[00] との当たり判定番号
// CHR:プレイヤー[00] の設定位置の当たり判定番号
#define CHR_PLAYER_00_HITTEST_WITH_OBJ_MIRROR_00	(1)
#define CHR_PLAYER_00_HITTEST_SET_POS				(2)

// CHR:プレイヤー[00] の設置方向数
#define CHR_PLAYER_00_SET_DIRECTION	(8)

// CHR:プレイヤー[00] のインベントリ選択SE
// CHR:プレイヤー[00] の購入SE
// CHR:プレイヤー[00] の設置SE
#define CHR_PLAYER_00_INVENTORY_SELECT_SE	(SOUND_LABEL_SE_SELECT_000)
#define CHR_PLAYER_00_BUY_SE				(SOUND_LABEL_SE_BUY_000)
#define CHR_PLAYER_00_SET_SE				(SOUND_LABEL_SE_SET_000)

// CHR:プレイヤー[00] の復活にかかる時間の初期値
// CHR:プレイヤー[00] の復活にかかる時間の加算値
// CHR:プレイヤー[00] の復活テキストの位置
#define CHR_PLAYER_00_INIT_RESPAWN_TIME		(60*5)
#define CHR_PLAYER_00_ADD_RESPAWN_TIME		(60*0)
#define CHR_PLAYER_00_RESPAWN_HP_RATE		(0.25f)
#define CHR_PLAYER_00_RESPAWN_TEXT_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT*0.5f,0.0f)
#define CHR_PLAYER_00_RESPAWN_TEXT_COLOR	Color{ 219,43,0,255 }

//****************************************
// プロトタイプ宣言
//****************************************
// CHR:プレイヤー[00] のパラメーターの初期化処理
// CHR:プレイヤー[00] の管理情報のパラメーターの初期化処理
void InitParameterChr_player_00(void);
void InitParameterChr_player_00Control(void);

// CHR:プレイヤー[00] の状態設定処理
// CHR:プレイヤー[00] の状態に応じた開始処理
// CHR:プレイヤー[00] の状態に応じた更新処理
// CHR:プレイヤー[00] の状態に応じた終了処理
void SetChr_player_00State(CHR_PLAYER_00_STATE state);
void StartStateChr_player_00(void);
void UpdateStateChr_player_00(void);
void EndStateChr_player_00(void);

// CHR:プレイヤー[00] のキー入力処理
// CHR:プレイヤー[00] のキーボードによる移動処理
// CHR:プレイヤー[00] のスティックによる移動処理
// CHR:プレイヤー[00] の移動処理
// CHR:プレイヤー[00] の位置更新処理
// CHR:プレイヤー[00] のカメラ(3D)の注視点設定処理
// CHR:プレイヤー[00] の衝突チェック処理
// CHR:プレイヤー[00] の衝突情報に応じた処理
// CHR:プレイヤー[00] の設定位置の衝突チェック処理
void KeyInputChr_player_00(void);
void MoveChr_player_00ByKeyboard(DIRECTION drct);
void MoveChr_player_00ByStick(void);
void MoveChr_player_00(float fMove);
void UpdatePosChr_player_00(void);
void SetCamera3DPosRChr_player_00(void);
void CheckCollisionChr_player_00(VECTOR vector);
void CollisionProcessChr_player_00(void);
void CheckCollisionChr_player_00SetPos(void);

// CHR:プレイヤー[00] のモーション設定処理
void MotionSetChr_player_00(void);

// CHR:プレイヤー[00] のアイテム使用処理
void ItemUsingChr_player_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_player_00		// CHR:プレイヤー[00] のテクスチャへのポインタ
						[CHR_PLAYER_00_TYPE_NUM]
						[PARTS_3D_MAX]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshChr_player_00		// CHR:プレイヤー[00] のメッシュ(頂点情報)へのポインタ
						[CHR_PLAYER_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
LPD3DXBUFFER			g_pBuffMatChr_player_00[PARTS_3D_MAX]= {};	// CHR:プレイヤー[00] のマテリアルへのポインタ
DWORD					g_aNumMatChr_player_00		// CHR:プレイヤー[00] のマテリアルの数
						[CHR_PLAYER_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
D3DXMATRIX				g_aMtxWorldChr_player_00	// CHR:プレイヤー[00] のワールドマトリックス
						[PARTS_3D_MAX];
Chr_player_00			g_chr_player_00;			// CHR:プレイヤー[00] の情報
Chr_player_00Control	g_chr_player_00Control;		// CHR:プレイヤー[00] の管理情報
Chr_player_00Type		g_aChr_player_00Type		// CHR:プレイヤー[00] の種類毎の情報
						[CHR_PLAYER_00_TYPE_NUM];

// CHR:プレイヤー[00] のアイテム毎の情報
const Chr_player_00Item g_aChr_player_00Item[CHR_PLAYER_00_ITEM_MAX] =
{
	{ false , false , false , "SWORD"	},
	{ false , true  , false , "GUN" },
	{ true  , false , true  , "BLOCK" },
	{ true  , false , true  , "TURRET" },
	{ true  , false , true  , "REFLECTOR" },
	{ true  , false , true  , "DISCHARGER" },
};

//========== *** CHR:プレイヤー[00] の情報を取得 ***
Chr_player_00 *GetChr_player_00(void) 
{
	return &g_chr_player_00;
}

//========== *** CHR:プレイヤー[00] の管理情報を取得 ***
Chr_player_00Control *GetChr_player_00Control(void)
{
	return &g_chr_player_00Control;
}

//========== *** CHR:プレイヤー[00] の種類毎の情報を取得 ***
Chr_player_00Type *GetChr_player_00Type(void)
{
	return g_aChr_player_00Type;
}

//========== *** CHR:プレイヤー[00] のアイテム毎の情報を取得 ***
const Chr_player_00Item *GetChr_player_00Item(void)
{
	return g_aChr_player_00Item;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterChr_player_00関数 - CHR:プレイヤー[00] のパラメーターの初期化処理 -
//========================================
void InitParameterChr_player_00(void) 
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	pChr->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の位置
	pChr->posOld				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 過去の位置
	pChr->force					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 力
	pChr->move					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	pChr->rot					= CHR_PLAYER_00_INIT_ROT;			// 向き
	pChr->moveRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動向き
	pChr->targetRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標向き
	pChr->setPos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 設定位置
	pChr->relativePosR			= CHR_PLAYER_00_RELATIVE_POS_R;		// 注視点の相対位置
	pChr->targetRltvPosR		= CHR_PLAYER_00_RELATIVE_POS_R;		// 目標の注視点の相対位置
	pChr->nType					= 0;								// 種類
	pChr->state					= CHR_PLAYER_00_STATE_NORMAL;		// 状態
	pChr->nCounterState			= 0;								// 状態カウンターを設定
	pChr->nHP					=									// HP
	pChr->nHPMax				= 0;								// HPの上限
	pChr->nCoin					= CHR_PLAYER_00_INIT_COIN;			// コイン数
	pChr->nScore				= 0;								// スコア
	pChr->nSelectItem			= CHR_PLAYER_00_INIT_SELECT_ITEM;	// 選択アイテム
	pChr->bWalk					= false;							// 歩行フラグ
	pChr->bRotation				= false;							// 方向転換フラグ
	pChr->bSlash				= false;							// 斬撃
	pChr->nCounterAttack		= 0;								// 攻撃カウンター
	pChr->nCounterInvincible	= 0;								// 無敵カウンター
	pChr->nCounterDamage		= 0;								// ダメージカウンター
	pChr->nPedestalIndex		= -1;								// 台座の番号
	pChr->nSignboardIndex		= -1;								// 看板の番号
	pChr->nCoinIndex			= -1;								// コインの番号
	for (int nCntItem = 0; nCntItem < CHR_PLAYER_00_ITEM_MAX; nCntItem++)
	{
		pChr->aItemCount[nCntItem] = 0;								// アイテムカウント
	}
	pChr->nBulletAtkUpCount		= 0;								// 弾の攻撃力アップカウント
	pChr->nSwordAtkUpCount		= 0;								// 剣の攻撃力アップカウント
	pChr->nRespawnTime			= CHR_PLAYER_00_INIT_RESPAWN_TIME;	// 復活にかかる時間
	pChr->hitTestInfo			= {};								// 当たり判定の管理情報
	pChr->cmnCollision			= {};								// 共通の衝突情報
	for (int nCntHitObj = 0; nCntHitObj < CHR_PLAYER_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};						// オブジェクト毎の衝突情報
	}
	pChr->setPosCmnCollision	= {};								// 設定位置の共通の衝突情報
	pChr->partsInfo				= {};								// 部品管理
	pChr->motion				= CHR_PLAYER_00_MOTION_WAIT;		// モーション
}

//========================================
// InitParameterChr_player_00Control関数 - CHR:プレイヤー[00] の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterChr_player_00Control(void) 
{
	Chr_player_00Control	*pChrCtl	// CHR:プレイヤー[00] の管理情報のポインタ
							= &g_chr_player_00Control;

	pChrCtl->state = CHR_PLAYER_00_CONTROL_STATE_DYNAMIC;	// 状態
}

//========================================
// StartStateChr_player_00関数 - CHR:プレイヤー[00] の状態に応じた開始処理 -
//========================================
void StartStateChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	switch (pChr->state)
	{
	case /*/ 通常 /*/CHR_PLAYER_00_STATE_NORMAL:
		break;
	case /*/ ダウン /*/CHR_PLAYER_00_STATE_DOWN:
		pChr->nCounterState = pChr->nRespawnTime;				// 状態カウンターを設定
		pChr->nRespawnTime += CHR_PLAYER_00_ADD_RESPAWN_TIME;	// 復活にかかる時間を加算
		pChr->move =	// 移動量と力を初期化する
		pChr->force = INITD3DXVECTOR3;
		break;
	}
}

//========================================
// EndStateChr_player_00関数 - CHR:プレイヤー[00] の状態に応じた終了処理 -
//========================================
void EndStateChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	switch (pChr->state)
	{
	case /*/ 通常 /*/CHR_PLAYER_00_STATE_NORMAL:
		break;
	case /*/ ダウン /*/CHR_PLAYER_00_STATE_DOWN:
		// HPを最大HPに対する割合で設定
		pChr->nHP = pChr->nHPMax*CHR_PLAYER_00_RESPAWN_HP_RATE;
		// 無敵カウンターを設定
		pChr->nCounterInvincible = CHR_PLAYER_00_AFTER_RESPAWN_INVINCIBLE_TIME;
		break;
	}
}

//========================================
// UpdateStateChr_player_00関数 - CHR:プレイヤー[00] の状態に応じた更新処理 -
//========================================
void UpdateStateChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	switch (pChr->state)
	{
	case /*/ 通常 /*/CHR_PLAYER_00_STATE_NORMAL:
		break;
	case /*/ ダウン /*/CHR_PLAYER_00_STATE_DOWN: {
		char aString[TXT_MAX];	// 文字列
		sprintf(aString, "%d SEC TO RESPAWN,", (pChr->nCounterState / 60));
		SetText2D(
			aString, 
			FONT_002, 
			DISPLAY_CENTER, 
			CHR_PLAYER_00_RESPAWN_TEXT_POS, 
			CHR_PLAYER_00_RESPAWN_TEXT_COLOR, 
			1.0f, 1.0f);

			if (--pChr->nCounterState <= 0)
			{// 状態カウンターを減算した結果0以下の時、
				SetChr_player_00State(CHR_PLAYER_00_STATE_NORMAL);	// 状態を通常に設定
			}
		break;
	}
	}
}

//========================================
// KeyInputChr_player_00関数 - CHR:プレイヤー[00] のキー入力処理 -
//========================================
void KeyInputChr_player_00(void)
{
	Chr_player_00			*pChr		// CHR:プレイヤー[00] の情報のポインタ
							= &g_chr_player_00;
	Chr_player_00Control	*pChrCtl	// CHR:プレイヤー[00] の管理情報のポインタ
							= &g_chr_player_00Control;

	if ((pChr->bSlash) || (pChrCtl->state == CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP))
	{// 斬撃フラグが真 or 管理状態が入力停止の時、
		return;	// 処理を終了する
	}

	pChr->bWalk = true;		// 歩行フラグを真にする
	pChr->bRotation = true;	// 方向転換フラグを真にする

	if (GetStick().aTplDiameter[STICK_TYPE_LEFT] > 0.0f)
	{// 左スティックが倒されている時、
		// スティックによる移動処理
		MoveChr_player_00ByStick();
	}
	else if		(GetKeyboardPress(DIK_A))	{
		if		(GetKeyboardPress(DIK_W))	{ MoveChr_player_00ByKeyboard(DIRECTION_LEFT_BACK); }	// 左奥移動
		else if (GetKeyboardPress(DIK_S))	{ MoveChr_player_00ByKeyboard(DIRECTION_LEFT_FRONT); }	// 左手前移動
		else								{ MoveChr_player_00ByKeyboard(DIRECTION_LEFT); }		// 左移動
	}
	else if		(GetKeyboardPress(DIK_D))	{
		if		(GetKeyboardPress(DIK_W))	{ MoveChr_player_00ByKeyboard(DIRECTION_RIGHT_BACK); }	// 右奥移動
		else if (GetKeyboardPress(DIK_S))	{ MoveChr_player_00ByKeyboard(DIRECTION_RIGHT_FRONT); }	// 右手前移動
		else								{ MoveChr_player_00ByKeyboard(DIRECTION_RIGHT); }		// 右移動
	}
	else if		(GetKeyboardPress(DIK_W))	{ MoveChr_player_00ByKeyboard(DIRECTION_BACK); }		// 奥移動
	else if		(GetKeyboardPress(DIK_S))	{ MoveChr_player_00ByKeyboard(DIRECTION_FRONT); }		// 手前移動
	else 
	{// 移動入力が無かった時、
		pChr->bWalk		= false;	// 歩行フラグを偽にする
		pChr->bRotation = false;	// 方向転換フラグを偽にする
	}

	if		((GetKeyboardTrigger(DIK_Q)) || (GetButtonTrigger(BUTTON_LEFT_SHOULDER))) { 
		pChr->nSelectItem--;	// 選択アイテム減算 & インベントリ選択SE再生
		PlaySound(CHR_PLAYER_00_INVENTORY_SELECT_SE);
	}
	else if ((GetKeyboardTrigger(DIK_E)) || (GetButtonTrigger(BUTTON_RIGHT_SHOULDER))) {
		pChr->nSelectItem++;	// 選択アイテム加算 & インベントリ選択SE再生
		PlaySound(CHR_PLAYER_00_INVENTORY_SELECT_SE);
	}
	IntLoopControl(&pChr->nSelectItem, CHR_PLAYER_00_ITEM_MAX, 0);	// 選択アイテムループ制御
	
	if ((GetKeyboardTrigger(DIK_SPACE))
		||
		(GetButtonTrigger(BUTTON_RIGHT_TRIGGER))
		||
		((GetKeyboardPress(DIK_SPACE)) && (pChr->nSelectItem == CHR_PLAYER_00_ITEM_GUN))
		||
		(((GetButtonPress(BUTTON_RIGHT_TRIGGER))) && (pChr->nSelectItem == CHR_PLAYER_00_ITEM_GUN)))
	{// SPACEキーが押された or (SPACEキーが押されている & 選択アイテムが銃) の時、
		ItemUsingChr_player_00();	// アイテム使用処理
	}

	if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A)) || (GetMouseTrigger(MOUSE_LEFT)))
	{// ENTERキーorAボタンorマウス左ボタンが押された時、
		if (pChr->nPedestalIndex != -1)
		{// 台座の番号が-1でない時、
			if (PurchaseObj_pedestal_00())
			{// 購入出来た時、
				PlaySound(CHR_PLAYER_00_BUY_SE);	// 購入SEを再生
			}
		}
		else if (pChr->nSignboardIndex != -1)
		{// 看板との距離が判定距離内の時、
			// UI:TIPSを設定
			SetUi_tips_00(GetObj_signboard_00()[pChr->nSignboardIndex].nType);
		}
		else if (pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00].bHit)
		{// OBJ:スイッチ[00] の当たりフラグが真の時、
			PushObj_switch_00();	// OBJ:スイッチ[00] の押下処理
		}
	}
}

//========================================
// ItemUsingChr_player_00関数 - CHR:プレイヤー[00] のアイテム使用処理 -
//========================================
void ItemUsingChr_player_00(void) 
{
	Chr_player_00		*pChr		// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType		// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];
	bool				bSetItem	// アイテムの設置フラグ
						= g_aChr_player_00Item[pChr->nSelectItem].bSet;

	if ((bSetItem) && (GetChr_player_00()->setPosCmnCollision.bInside))
	{// 選択しているアイテムの設置フラグが真 & 設定位置の重なりフラグが真の時、
		return;	// 処理を終了する
	}

	if (g_aChr_player_00Item[pChr->nSelectItem].bCount)
	{// 選択しているアイテムのカウントフラグが偽の時、
		if (!IntConsumption(&pChr->aItemCount[pChr->nSelectItem], 1))
		{// アイテムのカウントを消費した結果足りなかった時、
			return;	// 処理を終了する
		}
		else if (bSetItem)
		{// [アイテムのカウントを消費した結果足りている] & 選択しているアイテムの設置フラグが真の時、
			pChr->motion = CHR_PLAYER_00_MOTION_SET;	// モーションを設置にする
		}
	}

	switch (pChr->nSelectItem)
	{
		//========== *** 剣 ***
	case CHR_PLAYER_00_ITEM_SWORD: {
		if (!pChr->bSlash) 
		{// 斬撃フラグが偽の時、
			pChr->bSlash = true;		// 斬撃フラグを真にする
			pChr->nCounterAttack = 0;	// 攻撃カウンターを初期化
			pChr->force.x += sinf(pChr->rot.y) * pType->fMoveForce * CHR_PLAYER_00_SLASH_FORCE_DIAMETER;
			pChr->force.z += cosf(pChr->rot.y) * pType->fMoveForce * CHR_PLAYER_00_SLASH_FORCE_DIAMETER;
			PlaySound(GetAtk_sword_00Type()->slashSE);	// 斬撃SEを再生
			pChr->motion = CHR_PLAYER_00_MOTION_SLASH;	// モーションを斬撃にする
		}
	}
		break;
		//========== *** 銃 ***
	case CHR_PLAYER_00_ITEM_GUN: {
		if (pChr->nCounterAttack <= 0) 
		{// 攻撃カウンターが0以下の時、ATK:弾[00] の設定処理
			SetAtk_bullet_00(pChr->pos, pChr->rot, pType->nBulletType, ATK_BULLET_00_PARENTTYPE_PLAYER);
			pChr->nCounterAttack = pType->nShotTime;	// 攻撃カウンターを設定
			pChr->motion = CHR_PLAYER_00_MOTION_SHOT;	// モーションを発射にする
		}
	}
		break;
		//========== *** ブロック ***
	case CHR_PLAYER_00_ITEM_BLOCK: {
		// OBJ:ブロック[00] の設定処理
		SetObj_block_00(pChr->setPos);
		PlaySound(CHR_PLAYER_00_SET_SE);	// 設置SEを再生
	}
		break;
		//========== *** タレット ***
	case CHR_PLAYER_00_ITEM_TURRET: {
		// OBJ:タレット[00] の設定処理
		SetObj_turret_00(pChr->setPos, D3DXVECTOR3(0.0f, ConvertAngleToDirection(pChr->rot.y, CHR_PLAYER_00_SET_DIRECTION), 0.0f));
		PlaySound(CHR_PLAYER_00_SET_SE);	// 設置SEを再生
	}
		break;
		//========== *** 反射装置 ***
	case CHR_PLAYER_00_ITEM_REFLECTOR: {
		// OBJ:ミラー[00] の設置処理
		SetObj_mirror_00(pChr->setPos);
		PlaySound(CHR_PLAYER_00_SET_SE);	// 設置SEを再生
	}
		break;
		//========== *** 放電装置 ***
	case CHR_PLAYER_00_ITEM_DISCHARGER: {
		// OBJ:放電装置[00] の設置処理
		SetObj_discharger_00(pChr->setPos);
		PlaySound(CHR_PLAYER_00_SET_SE);	// 設置SEを再生
	}
		break;
	}
}

//========================================
// MoveChr_player_00ByKeyboard関数 - CHR:プレイヤー[00] のキーボードによる移動処理 -
//========================================
void MoveChr_player_00ByKeyboard(DIRECTION drct) 
{
	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	// 移動向きに円周率を代入
	pChr->moveRot.y = D3DX_PI;

	switch (drct)
	{
	case DIRECTION_LEFT			:pChr->moveRot.y *= -0.5f	; break;	// 左
	case DIRECTION_RIGHT		:pChr->moveRot.y *=  0.5f	; break;	// 右
	case DIRECTION_BACK			:pChr->moveRot.y *=  0.0f	; break;	// 奥
	case DIRECTION_FRONT		:pChr->moveRot.y *=  1.0f	; break;	// 手前
	case DIRECTION_LEFT_BACK	:pChr->moveRot.y *= -0.25f	; break;	// 左奥
	case DIRECTION_LEFT_FRONT	:pChr->moveRot.y *= -0.75f	; break;	// 左手前
	case DIRECTION_RIGHT_BACK	:pChr->moveRot.y *=  0.25f	; break;	// 右奥
	case DIRECTION_RIGHT_FRONT	:pChr->moveRot.y *=  0.75f	; break;	// 右手前
	}

	// 移動処理
	MoveChr_player_00(pType->fMoveForce);
}

//========================================
// MoveChr_player_00ByStick関数 - CHR:プレイヤー[00] のスティックによる移動処理 -
//========================================
void MoveChr_player_00ByStick(void)
{
	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	// 目標向きにスティックの角度を代入
	pChr->moveRot.y = -GetStick().aAngle[STICK_TYPE_LEFT] + D3DX_PI;

	// 移動処理
	MoveChr_player_00(pType->fMoveForce * GetStick().aTplDiameter[STICK_TYPE_LEFT]);
}

//========================================
// MoveChr_player_00関数 - CHR:プレイヤー[00] の移動処理 -
//========================================
void MoveChr_player_00(float fMove) 
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	D3DXVECTOR3 posV = GetCamera3D()->posV;	// 視点
	D3DXVECTOR3 posR = GetCamera3D()->posR;	// 注視点

	// 視点から注視点までの角度を求め、移動向きに加算
	pChr->moveRot.y += FindAngle(
		D3DXVECTOR3(posV.x, posV.z, 0.0f),
		D3DXVECTOR3(posR.x, posR.z, 0.0f));

	// 移動量を更新
	pChr->move.x += sinf(pChr->moveRot.y) * fMove;
	pChr->move.z += cosf(pChr->moveRot.y) * fMove;
}

//========================================
// UpdatePosChr_player_00関数 - CHR:プレイヤー[00] の位置更新処理 -
//========================================
void UpdatePosChr_player_00(void) 
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	// 共通の衝突情報を初期化
	pChr->cmnCollision = {};

	// オブジェクト毎の衝突情報を初期化
	for (int nCntHitObj = 0; nCntHitObj < CHR_PLAYER_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};
	}

	// 設定位置の共通の衝突情報を初期化
	pChr->setPosCmnCollision = {};

	pChr->nPedestalIndex = -1;	// 台座の番号を初期化
	pChr->nCoinIndex = -1;		// コインの番号を初期化

	pChr->pos.x += pChr->move.x;				// X座標に移動量を適用
	pChr->move.x *= CHR_PLAYER_00_MOVE_DAMP;	// X方向の移動量を減衰
	pChr->pos.x += pChr->force.x;				// X座標に力を適用
	pChr->force.x *= CHR_PLAYER_00_FORCE_DAMP;	// X方向の力を減衰
	CheckCollisionChr_player_00(VECTOR_X);		// X方向の衝突チェック処理

	pChr->pos.z += pChr->move.z;				// Z座標に移動量を適用
	pChr->move.z *= CHR_PLAYER_00_MOVE_DAMP;	// Z方向の移動量を減衰
	pChr->pos.z += pChr->force.z;				// Z座標に力を適用
	pChr->force.z *= CHR_PLAYER_00_FORCE_DAMP;	// Z方向の力を減衰
	CheckCollisionChr_player_00(VECTOR_Z);		// Z方向の衝突チェック処理

	CollisionProcessChr_player_00();	// 衝突情報に応じた処理

	// 設置位置を設定
	pChr->setPos = pChr->pos;
	pChr->setPos.x = (int)((pChr->setPos.x + (OBJECT_SET_SPACE * 0.5f * PrusMinus(pChr->setPos.x))) / OBJECT_SET_SPACE) * OBJECT_SET_SPACE;
	pChr->setPos.z = (int)((pChr->setPos.z + (OBJECT_SET_SPACE * 0.5f * PrusMinus(pChr->setPos.z))) / OBJECT_SET_SPACE) * OBJECT_SET_SPACE;

	// 設定位置の衝突チェック処理
	CheckCollisionChr_player_00SetPos();

	if (g_aChr_player_00Item[pChr->nSelectItem].bAim)
	{// 選択しているアイテムの狙いフラグが真の時、
		D3DXVECTOR3 posV = GetCamera3D()->posV;	// 視点
		D3DXVECTOR3 posR = GetCamera3D()->posR;	// 注視点
		float		fAngleVR					// 視点から注視点までの角度
					= FindAngle(D3DXVECTOR3(posV.x, posV.z, 0.0f), D3DXVECTOR3(posR.x, posR.z, 0.0f));
		pChr->targetRot.y = fAngleVR;			// 目標向きYに移動向きを代入
		pChr->bRotation = true;					// 方向転換フラグを真にする
	}
	else 
	{// 選択しているアイテムの狙いフラグが偽の時、.
		pChr->targetRot = pChr->moveRot;		// 目標向きに移動向きを代入
	}
}

//========================================
// SetCamera3DPosRChr_player_00関数 - CHR:プレイヤー[00] のカメラ(3D)の注視点設定処理 -
//========================================
void SetCamera3DPosRChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	// 選択しているアイテムの狙いフラグに応じて目標の相対位置を設定
	g_aChr_player_00Item[pChr->nSelectItem].bAim ?
		pChr->targetRltvPosR = CHR_PLAYER_00_AIM_RELATIVE_POS_R :
		pChr->targetRltvPosR = CHR_PLAYER_00_RELATIVE_POS_R;

	// 注視点の相対位置を目標の相対位置に推移させる
	pChr->relativePosR += (pChr->targetRltvPosR - pChr->relativePosR) * CHR_PLAYER_00_RELATIVE_POS_R_MOVE_DIAMETER;

	D3DXVECTOR3 setPos = pChr->pos;			// 設定位置
	D3DXVECTOR3 posV = GetCamera3D()->posV;	// 視点
	D3DXVECTOR3 posR = GetCamera3D()->posR;	// 注視点
	float		fAngleVR					// 視点から注視点までの角度
				= FindAngle(D3DXVECTOR3(posV.x, posV.z, 0.0f), D3DXVECTOR3(posR.x, posR.z, 0.0f));

	// 設定位置に相対位置を適用
	setPos.y += pChr->relativePosR.y;
	setPos.x += sinf(fAngleVR) * pChr->relativePosR.z;
	setPos.z += cosf(fAngleVR) * pChr->relativePosR.z;
	
	// カメラ(3D)の注視点を設定
	SetCamera3DPosR(setPos);
}

//========================================
// CheckCollisionChr_player_00関数 - CHR:プレイヤー[00] の衝突チェック処理 -
//========================================
void CheckCollisionChr_player_00(VECTOR vector)
{
	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	// 衝突判定に必要な情報
	CollisionInfo myCollInfo =
	{
		&pChr->pos,
		pChr->posOld,
		NULL,
		&pChr->rot,
		pChr->rot,
		pType->hitTestSet.aHitTest[pChr->hitTestInfo.nHitTest]
	};

	// 衝突判定
	/*/ OBJ:ブロック	[00] /*/CollisionObj_block_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_BLOCK_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:コア		[00] /*/CollisionObj_core_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_CORE_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:ステージ	[00] /*/CollisionObj_stage_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_STAGE_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:スイッチ	[00] /*/CollisionObj_switch_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00], &pChr->cmnCollision, myCollInfo);

	// 衝突判定(台座)
	if (GetObj_pedestal_00Control()->nCounterArrival == OBJ_PEDESTAL_00_ARRIVAL_TIME)
	{// OBJ:台座	[00] の到着カウンターが到着にかかる時間と一致していた時、
		int nIndex;	// 番号

		nIndex = CollisionObj_pedestal_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_PEDESTAL_00], &pChr->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// OBJ:台座[00] と接触した時、
			pChr->nPedestalIndex = nIndex;	// 台座の番号を設定
		}
	}

	// 衝突判定(コイン)
	{
		int nIndex;	// 番号

		nIndex = CollisionItm_coin_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_ITM_COIN_00], &pChr->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// ITM:コイン[00] と接触した時、
			pChr->nCoinIndex = nIndex;	// コインの番号を設定
		}
	}

	// 衝突判定に必要な情報
	myCollInfo =
	{
		&pChr->pos,
		pChr->posOld,
		NULL,
		&pChr->rot,
		pChr->rot,
		pType->hitTestSet.aHitTest[CHR_PLAYER_00_HITTEST_WITH_OBJ_MIRROR_00]
	};

	/*/ OBJ:ミラー	[00] /*/CollisionObj_mirror_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_MIRROR_00], &pChr->cmnCollision, myCollInfo);
	
	// 衝突判定(看板)
	pChr->nSignboardIndex = CollisionObj_signboard_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SIGNBOARD_00], &pChr->cmnCollision, myCollInfo);
}

//========================================
// CollisionProcessChr_player_00関数 - CHR:プレイヤー[00] の衝突情報に応じた処理 -
//========================================
void CollisionProcessChr_player_00(void)
{
	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;

	if (pChr->nCoinIndex != -1) 
	{// コインの番号が-1でない時、
		DestroyItm_coin_00(pChr->nCoinIndex);	// ITM:コイン[00] の破壊処理
		pChr->nCoin++;							// コイン数を加算
	}
}

//========================================
// CheckCollisionChr_player_00SetPos関数 - CHR:プレイヤー[00] の設定位置の衝突チェック処理 -
//========================================
void CheckCollisionChr_player_00SetPos(void)
{
	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	if (!g_aChr_player_00Item[pChr->nSelectItem].bSet) 
	{// 選択しているアイテムの設置フラグが偽の時、
		return;	// 処理を終了する
	}

	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&pChr->setPos,
		pChr->setPos,
		NULL,
		NULL,
		INITD3DXVECTOR3,
		pType->hitTestSet.aHitTest[CHR_PLAYER_00_HITTEST_SET_POS]
	};

	// 衝突判定
	/*/ OBJ:ブロック	[00] /*/CollisionObj_block_00			(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:放電装置	[00] /*/CollisionObj_discharger_00		(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:ミラー		[00] /*/CollisionObj_mirror_00			(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:タレット	[00] /*/CollisionObj_turret_00			(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:ステージ	[00] /*/CollisionObj_stage_00WithSetPos	(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
}

//========================================
// MotionSetChr_player_00関数 - CHR:プレイヤー[00] のモーション設定処理 -
//========================================
void MotionSetChr_player_00(void) 
{
	Chr_player_00		*pChr		// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType		// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];
	Motion3D			*pMotion	// 現在のモーションの情報のポインタ
						= &pType->motionSet.aMotion[pChr->motion];;

	if (pChr->state == CHR_PLAYER_00_STATE_DOWN)
	{// 状態がダウンの時、ダウン
		pChr->motion = CHR_PLAYER_00_MOTION_DOWN;
	}
	else if (pMotion->bLoop) 
	{// 現在のモーションのループフラグが真の時、
		pChr->motion =	// 歩行/待機
			pChr->bWalk ?
			CHR_PLAYER_00_MOTION_WALK :
			CHR_PLAYER_00_MOTION_WAIT;
	}

	// モーション設定
	SetMotion3D(
		&pChr->partsInfo,
		pChr->motion);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_player_00関数 - CHR:プレイヤー[00] の読み込み処理 -
//========================================
void LoadChr_player_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Chr_player_00Type	*pChrType	// CHR:プレイヤー[00] の種類毎の情報
						= g_aChr_player_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(CHR_PLAYER_00_TYPE_DATA_FILE_PATH, "r");

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
			// CHR:プレイヤー[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))			{ pChrType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "HIT_POINT:"))		{ fscanf(pFile, "%d", &pChrType->nHP); }			// HP
				else if (!strcmp(aDataSearch, "MOVE_FORCE:"))		{ fscanf(pFile, "%f", &pChrType->fMoveForce); }		// 移動力
				else if (!strcmp(aDataSearch, "SLASH_DAMAGE:"))		{ fscanf(pFile, "%d", &pChrType->nSlashDamage); }	// 斬撃ダメージ
				else if (!strcmp(aDataSearch, "BULLET_TYPE:"))		{ fscanf(pFile, "%d", &pChrType->nBulletType); }	// 弾の種類
				else if (!strcmp(aDataSearch, "SHOT_TIME:"))		{ fscanf(pFile, "%d", &pChrType->nShotTime); }		// 発射間隔
				else if (!strcmp(aDataSearch, "SHADOW_RADIUS:"))	{ fscanf(pFile, "%f", &pChrType->fShadowRadius); }	// 影の半径
				else if (!strcmp(aDataSearch, "DAMAGE_SE:"))		{ fscanf(pFile, "%d", &pChrType->damageSE); }		// ダメージSE
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pChrType->hitTestSet); }	// 当たり判定設定情報
				else if (!strcmp(aDataSearch, "PARTSSET"))			{ LoadParts3DSet(pFile, &pChrType->partsSet); }		// 部品設定情報
				else if (!strcmp(aDataSearch, "MOTIONSET"))			{ LoadMotion3DSet(pFile, &pChrType->motionSet); }	// モーション設定情報
			}
		}
	}
}

//========================================
// InitChr_player_00関数 - CHR:プレイヤー[00] の初期化処理 -
//========================================
void InitChr_player_00(void)
{
	// 管理情報のパラメータの初期化処理
	InitParameterChr_player_00Control();

	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXMATERIAL		*pMat;	// マテリアルへのポインタ

	for (int nCntType = 0; nCntType < CHR_PLAYER_00_TYPE_NUM; nCntType++)
	{
		Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
							= &g_aChr_player_00Type[nCntType];

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
				&g_pBuffMatChr_player_00[nCntParts],
				NULL,
				&g_aNumMatChr_player_00[nCntType][nCntParts],
				&g_aMeshChr_player_00[nCntType][nCntParts]);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatChr_player_00[nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_player_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureChr_player_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// パラメーターの初期化処理
	InitParameterChr_player_00();

	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	// 部品(3D)のトランスフォームを初期化
	InitParts3DTransform(&pChr->partsInfo, &pType->partsSet);

	// HPを初期化
	pChr->nHP =
	pChr->nHPMax = pType->nHP;

	// カメラ(3D)の注視点を設定
	SetCamera3DPosR(pChr->pos + CHR_PLAYER_00_RELATIVE_POS_R);
}

//========================================
// UninitChr_player_00関数 - CHR:プレイヤー[00] の終了処理 -
//========================================
void UninitChr_player_00(void)
{
	for (int nCntType = 0; nCntType < CHR_PLAYER_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// メッシュの破棄
			if (g_aMeshChr_player_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshChr_player_00[nCntType][nCntParts]->Release();
				g_aMeshChr_player_00[nCntType][nCntParts] = NULL;
			}

			// マテリアルの破棄
			if (g_pBuffMatChr_player_00[nCntParts] != NULL)
			{
				g_pBuffMatChr_player_00[nCntParts]->Release();
				g_pBuffMatChr_player_00[nCntParts] = NULL;
			}

			// テクスチャの破棄
			for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
				if (g_aTextureChr_player_00[nCntType][nCntParts][nCntMat] != NULL)
				{
					g_aTextureChr_player_00[nCntType][nCntParts][nCntMat]->Release();
					g_aTextureChr_player_00[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}
}

//========================================
// UpdateChr_player_00関数 - CHR:プレイヤー[00] の更新処理 -
//========================================
void UpdateChr_player_00(void)
{
	Chr_player_00			*pChr		// CHR:プレイヤー[00] の情報のポインタ
							= &g_chr_player_00;
	Chr_player_00Type		*pType		// CHR:プレイヤー[00] の種類毎の情報のポインタ
							= &g_aChr_player_00Type[pChr->nType];
	Chr_player_00Control	*pChrCtl	// CHR:プレイヤー[00] の管理情報のポインタ
							= &g_chr_player_00Control;

	if (pChrCtl->state != CHR_PLAYER_00_CONTROL_STATE_STATIC)
	{// 管理状態が静的でない時、
		// 現在の位置を過去の位置として保存
		pChr->posOld = pChr->pos;

		// 状態に応じた更新処理
		UpdateStateChr_player_00();

		if (pChr->state != CHR_PLAYER_00_STATE_DOWN)
		{// 状態がダウンでない時、
			// キー入力処理
			KeyInputChr_player_00();

			// 位置更新処理
			UpdatePosChr_player_00();

			// 向き/移動向き/目標向きを制御
			RotControl(&pChr->rot);
			RotControl(&pChr->moveRot);
			RotControl(&pChr->targetRot);

			// 角度を目標角度に向けて推移する
			pChr->rot.y += (AngleDifference(pChr->rot.y, pChr->targetRot.y) * CHR_PLAYER_00_ROT_DIAMETER) * pChr->bRotation * (pChr->bSlash ^ 1);
		}

		if (pChrCtl->state != CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP)
		{// 管理状態が入力停止でない時、
			// カメラ(3D)の注視点設定処理
			SetCamera3DPosRChr_player_00();
		}

		// 部品(3D)のトランスフォームを取得
		GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);

		// モーションの設定処理
		MotionSetChr_player_00();

		// モーションの更新処理
		if (UpdateMotion3D(
			pChr->partsInfo.nMotion,
			g_aChr_player_00Type[pChr->nType].motionSet,
			&pChr->partsInfo,
			g_aChr_player_00Type[pChr->nType].partsSet)) 
		{// モーションがループに達した時、
			Motion3D	*pMotion	// 現在のモーションの情報のポインタ
						= &pType->motionSet.aMotion[pChr->motion];
			if (!pMotion->bLoop)
			{// 現在のモーションのループフラグが偽の時、
				pChr->motion = CHR_PLAYER_00_MOTION_WAIT;	// モーションを待機に設定
			}
		}
	}

	// EFF:影[00] の設定処理
	SetEff_shadow_00(pChr->pos, pType->fShadowRadius);

	if (pChr->bSlash) 
	{// 斬撃フラグが真の時、
		if (++pChr->nCounterAttack >= CHR_PLAYER_00_SLASH_TIME) 
		{// 攻撃カウンターを加算した結果斬撃時間に達した時、
			pChr->bSlash = false;	
		}
	}

	if (pChr->nCounterInvincible > 0)
	{// 無敵カウンターが0を上回っている時、
		pChr->nCounterInvincible--;	// 無敵カウンターを減算する
	}
	if (pChr->nCounterDamage > 0)
	{// ダメージカウンターが0を上回っている時、
		pChr->nCounterDamage--;	// ダメージカウンターを減算する
	}

	if ((pChr->nSelectItem == CHR_PLAYER_00_ITEM_GUN) && (pChr->nCounterAttack > 0))
	{// 選択アイテムが銃 & 攻撃カウンターが0を上回っている時、
		pChr->nCounterAttack--;	// 攻撃カウンターを減算
	}
}

//========================================
// DrawChr_player_00関数 - CHR:プレイヤー[00] の描画処理 -
//========================================
void DrawChr_player_00(void)
{
	Chr_player_00		*pChr				// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在のマテリアルの保存用
	D3DXMATERIAL		*pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX			mtxSelf;			// 本体のマトリックス

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 本体のワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxSelf);

	// 本体の向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pChr->rot.y, pChr->rot.x, pChr->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// 本体の位置を反映
	D3DXMatrixTranslation(&mtxTrans, pChr->pos.x, pChr->pos.y, pChr->pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// 本体のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// 部品(3D)の情報のポインタ
					= &pChr->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// 部品(3D)の部品の種類毎の情報のポインタ
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// 親マトリックス

		// 部品のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMtxWorldChr_player_00[nCntParts]);

		// 部品の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_player_00[nCntParts], &g_aMtxWorldChr_player_00[nCntParts], &mtxRot);

		// 部品の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_player_00[nCntParts], &g_aMtxWorldChr_player_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1) 
		{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
			mtxParent = g_aMtxWorldChr_player_00[pPartsType->nParent];
		}
		else 
		{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
			mtxParent = mtxSelf;
		}

		// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(
			&g_aMtxWorldChr_player_00[nCntParts],
			&g_aMtxWorldChr_player_00[nCntParts],
			&mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldChr_player_00[nCntParts]);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatChr_player_00[nCntParts]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_player_00[pChr->nType][nCntParts]; nCntMat++)
		{
			// マテリアルの設定
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_PLAYER_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_PLAYER_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - (pChr->nCounterInvincible % CHR_PLAYER_00_INVINCIBLE_BLINK_TIME)))
			});

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureChr_player_00[pChr->nType][nCntParts][nCntMat]);

			// ポリゴン(パーツ)の描画
			g_aMeshChr_player_00[pChr->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// CollisionChr_player_00関数 - CHR:プレイヤー[00] との当たり判定処理 -
//========================================
int CollisionChr_player_00(VECTOR vector, Collision *pChrCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;
	
	if ((pChrCollision->bHit) || (pChr->state==CHR_PLAYER_00_STATE_DOWN))
	{// 当たりフラグが真 or CHR:プレイヤー[00] の状態がダウンの時、
		return -1;	// 処理を終了する
	}

	int					nIndex	// CHR:プレイヤー[00] の返しの番号
						= -1;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	// 衝突判定に必要な情報
	CollisionCheck(
		vector,
		pChrCollision,
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

	if (pChrCollision->bHit)
	{// 当たりフラグが真の時、
		nIndex = 0;	// 返しの番号を設定
	}

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pChrCollision);

	// 番号を返す
	return nIndex;
}

//========================================
// DamageChr_player_00関数 - CHR:プレイヤー[00] のダメージ処理 -
//========================================
void DamageChr_player_00(int nDamage)
{
	if ((GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL) || (nDamage == 0))
	{// OBJ:ステージ[00] の状態がインターバルの時、
		return;	// 処理を終了する
	}

	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	if ((pChr->nCounterInvincible > 0) || (pChr->state == CHR_PLAYER_00_STATE_DOWN))
	{// 無敵カウンターが0を上回っている or 状態がダウンの時、
		return;	// 処理を終了する
	}

	nDamage *= fDamageRand();	// 乱数補正
	pChr->nHP -= nDamage;		// HPからダメージを減算
	pChr->nCounterDamage		// ダメージカウンターを設定
		= CHR_PLAYER_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:ダメージ[00] の設定処理
		pChr->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);
	SetEff_screen_00(			// EFF:スクリーン[00] の設定処理
		CHR_PLAYER_00_DAMAGE_TIME, CHR_PLAYER_00_DAMAGE_COLOR);
	PlaySound(pType->damageSE);	// ダメージSEを再生

	// カメラ(3D)の振動を設定
	SetCamera3DVibration(CHR_PLAYER_00_DAMAGE_VIBRATION);

	if (pChr->nHP <= 0)
	{// HPが0以下の時、
		pChr->nHP = 0;	// HPを0にする
		// 状態をダウンに設定
		SetChr_player_00State(CHR_PLAYER_00_STATE_DOWN);		
	}
	else
	{// HPが0を上回っている時、無敵カウンターを設定
		pChr->nCounterInvincible = CHR_PLAYER_00_INVINCIBLE_TIME;
	}
}

//========================================
// HealChr_player_00関数 - CHR:プレイヤー[00] の回復処理 -
//========================================
void HealChr_player_00(int nHeal)
{
	Chr_player_00		*pChr	// CHR:プレイヤー[00] の情報のポインタ
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:プレイヤー[00] の種類毎の情報のポインタ
						= &g_aChr_player_00Type[pChr->nType];

	pChr->nHPMax =		// 最大HPを更新
		pType->nHP *
		(1.0f + (OBJ_PEDESTAL_00_PLAYER_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS].nCntPriceUp));
	pChr->nHP += nHeal;	// HPを加算
	IntControl(			// HPを制御
		&pChr->nHP, 
		pChr->nHPMax, 0);
}

//========================================
// KnockBackChr_player_00関数 - CHR:プレイヤー[00] のノックバック処理 -
//========================================
void KnockBackChr_player_00(D3DXVECTOR3 rot, float fForce)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	// 力を加える
	pChr->force.x += sinf(rot.y) * fForce;
	pChr->force.z += cosf(rot.y) * fForce;
}

//========================================
// SetChr_player_00State関数 - CHR:プレイヤー[00] の状態設定処理 -
//========================================
void SetChr_player_00State(CHR_PLAYER_00_STATE state)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;

	// 状態に応じた終了処理
	EndStateChr_player_00();

	// 状態を代入
	pChr->state = state;

	// 状態に応じた開始処理
	StartStateChr_player_00();
}

//========================================
// SetChr_player_00ControlState関数 - CHR:プレイヤー[00] の管理状態設定処理 -
//========================================
void SetChr_player_00ControlState(CHR_PLAYER_00_CONTROL_STATE state)
{
	Chr_player_00Control	*pChrCtl	// CHR:プレイヤー[00] の管理情報のポインタ
							= &g_chr_player_00Control;

	pChrCtl->state = state;	// 状態を代入
}

#if _DEBUG_COMMENT
//========================================
// DrawCommentChr_player_00関数 - CHR:プレイヤー[00] のコメント描画処理 -
//========================================
void DrawCommentChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:プレイヤー[00] の情報のポインタ
					= &g_chr_player_00;
	char			aString	// 文字列
					[TXT_MAX];

	CommentColorChange({ 88,248,152 });	// コメント色設定
	sprintf(aString, "*========* プレイヤー情報 *========*"); DrawComment(aString);
	sprintf(aString, " 位置: X %.1f Y %.1f Z %.1f", pChr->pos.x, pChr->pos.y, pChr->pos.z); DrawComment(aString);
	sprintf(aString, " 向き: X %.1f Y %.1f Z %.1f", pChr->rot.x, pChr->rot.y, pChr->rot.z); DrawComment(aString);
	sprintf(aString, " 斬撃: %d", pChr->rot.x, pChr->rot.y, pChr->rot.z); DrawComment(aString);
	sprintf(aString, "*==================================*"); DrawComment(aString);
}
#endif