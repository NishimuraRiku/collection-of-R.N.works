//========================================
// 
// CHR:プレイヤー[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_player_00.h ***
//========================================
#ifndef _CHR_PLAYER_00_H_	// このマクロ定義がされていなかった時
#define _CHR_PLAYER_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"
#include "polygon3D.h"
#include "sound.h"

//****************************************
// マクロ定義
//****************************************
// CHR:プレイヤー[00] の斬撃の内角
// CHR:プレイヤー[00] の斬撃にかかる時間
#define CHR_PLAYER_00_SLASH_INSIDE	(D3DX_PI * 1.5f)
#define CHR_PLAYER_00_SLASH_TIME	(15)

// OBJ:プレイヤー[00] の剣の攻撃力アップカウント毎の上昇値
// OBJ:プレイヤー[00] の弾の攻撃力アップカウント毎の上昇値
#define CHR_PLAYER_00_SWORD_ATK_ADDRATE		(0.25f)
#define CHR_PLAYER_00_BULLET_ATK_ADDRATE	(0.1f)

// CHR;プレイヤー[00] のHP割合表示距離
// CHR;プレイヤー[00] のHP割合テキストの拡大倍率
#define CHR_PLAYER_00_HPRATE_DISPLAY_DISTANCE	(80.0f)
#define CHR_PLAYER_00_HPRATE_TEXT_SCALE			(0.5f)

//****************************************
// 列挙型の定義
//****************************************
// CHR:プレイヤー[00] の衝突オブジェクト番号
typedef enum
{
	CHR_PLAYER_00_HITOBJ_ITM_COIN_00,		// ITM:コイン	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:ブロック	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_CORE_00,		// OBJ:コア		[00]
	CHR_PLAYER_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:ミラー	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_PEDESTAL_00,	// OBJ:台座		[00]
	CHR_PLAYER_00_HITOBJ_OBJ_STAGE_00,		// OBJ:ステージ	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00,		// OBJ:スイッチ	[00]
	CHR_PLAYER_00_HITOBJ_MAX,
}CHR_PLAYER_00_HITOBJ;

// CHR:プレイヤー[00] の設定位置の衝突オブジェクト番号
typedef enum 
{
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_BLOCK_00,		// OBJ:ブロック	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:放電装置	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_MIRROR_00,		// OBJ:ミラー	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_TURRET_00,		// OBJ:タレット	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_MAX,
}CHR_PLAYER_00_SETPOS_HITOBJ;

// CHR:プレイヤー[00] のアイテム番号
typedef enum
{
	CHR_PLAYER_00_ITEM_SWORD,		// 剣
	CHR_PLAYER_00_ITEM_GUN,			// 銃
	CHR_PLAYER_00_ITEM_BLOCK,		// ブロック
	CHR_PLAYER_00_ITEM_TURRET,		// タレット
	CHR_PLAYER_00_ITEM_REFLECTOR,	// シールド
	CHR_PLAYER_00_ITEM_DISCHARGER,	// 放電装置
	CHR_PLAYER_00_ITEM_MAX,
}CHR_PLAYER_00_ITEM;

// CHR:プレイヤー[00] の状態
typedef enum
{
	CHR_PLAYER_00_STATE_NORMAL,	// 通常
	CHR_PLAYER_00_STATE_DOWN,	// ダウン
	CHR_PLAYER_00_STATE_MAX,
}CHR_PLAYER_00_STATE;

// CHR:プレイヤー[00] の管理状態
typedef enum 
{
	CHR_PLAYER_00_CONTROL_STATE_DYNAMIC,	// 動的
	CHR_PLAYER_00_CONTROL_STATE_STATIC,		// 静的
	CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP,	// 入力停止
	CHR_PLAYER_00_CONTROL_STATE_MAX,
}CHR_PLAYER_00_CONTROL_STATE;

// CHR:プレイヤー[00] のモーション番号
typedef enum
{
	CHR_PLAYER_00_MOTION_WAIT,	// 待機
	CHR_PLAYER_00_MOTION_WALK,	// 歩行
	CHR_PLAYER_00_MOTION_SET,	// 設定
	CHR_PLAYER_00_MOTION_SHOT,	// 発射
	CHR_PLAYER_00_MOTION_SLASH,	// 斬撃
	CHR_PLAYER_00_MOTION_DOWN,	// ダウン
	CHR_PLAYER_00_MOTION_MAX,
}CHR_PLAYER_00_MOTION;

//****************************************
// 構造体の定義
//****************************************
// CHR:プレイヤー[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 posOld;			// 過去の位置(1フレーム前)
	D3DXVECTOR3 force;			// 力
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 moveRot;		// 移動向き
	D3DXVECTOR3 targetRot;		// 目標向き
	D3DXVECTOR3 setPos;			// 設置位置
	D3DXVECTOR3 relativePosR;	// 注視点の相対位置
	D3DXVECTOR3 targetRltvPosR;	// 目標の注視点の相対位置
	
	// 分類関連
	int	nType;	// 種類

	// 状態関連
	CHR_PLAYER_00_STATE	state;				// 状態
	int					nCounterState;		// 状態カウンター
	int					nHP;				// ヒットポイント
	int					nHPMax;				// ヒットポイントの上限
	int					nCoin;				// コイン数
	int					nScore;				// スコア
	int					nSelectItem;		// 選択アイテム番号
	bool				bWalk;				// 歩行フラグ
	bool				bRotation;			// 方向転換フラグ
	bool				bSlash;				// 斬撃フラグ
	int					nCounterAttack;		// 攻撃カウンター
	int					nCounterInvincible;	// 無敵カウンター
	int					nCounterDamage;		// ダメージカウンター
	int					aItemCount			// アイテムカウント
						[CHR_PLAYER_00_ITEM_MAX];
	int					nPedestalIndex;		// 台座の番号
	int					nCoinIndex;			// コインの番号
	int					nBulletAtkUpCount;	// 弾の攻撃力アップカウント
	int					nSwordAtkUpCount;	// 剣の攻撃力アップカウント
	int					nRespawnTime;		// 復活にかかる時間

	// 運動関連
	HitTestInfo hitTestInfo;		// 当たり判定の管理情報
	Collision	cmnCollision; 		// 共通の衝突情報
	Collision	aObjCollision		// オブジェクト毎の衝突情報
				[CHR_PLAYER_00_HITOBJ_MAX];
	Collision	setPosCmnCollision; // 設定位置の共通の衝突情報

	// 部品関連
	Parts3DInfo				partsInfo;		// 部品管理
	CHR_PLAYER_00_MOTION	motion;			// モーション
}Chr_player_00;

// CHR:プレイヤー[00] の管理情報構造体
typedef struct
{
	CHR_PLAYER_00_CONTROL_STATE state;	// 状態
}Chr_player_00Control;

// CHR:プレイヤー[00] の種類毎の情報構造体
typedef struct
{
	int			nHP;			// ヒットポイント
	float		fMoveForce;		// 移動力
	int			nSlashDamage;	// 斬撃ダメージ
	int			nBulletType;	// 弾の種類
	int			nShotTime;		// 発射間隔
	float		fShadowRadius;	// 影の半径
	SOUND_LABEL	damageSE;		// ダメージSE
	HitTestSet	hitTestSet;		// 当たり判定設定情報
	Parts3DSet	partsSet;		// 部品設定情報(3D)
	Motion3DSet motionSet;		// モーション設定情報
}Chr_player_00Type;

// CHR:プレイヤー[00] のアイテム毎の情報構造体
typedef struct
{
	bool	bSet;	// 設置フラグ
	bool	bAim;	// 狙いフラグ
	bool	bCount;	// カウントフラグ
	char	aName	// 名称
			[TXT_MAX];
}Chr_player_00Item;

//****************************************
// プロトタイプ宣言
//****************************************
// CHR:プレイヤー[00] の情報を取得
// CHR:プレイヤー[00] の管理情報を取得
// CHR:プレイヤー[00] の種類毎の情報を取得
// CHR:プレイヤー[00] のアイテムの情報を取得
Chr_player_00 *GetChr_player_00(void);
Chr_player_00Control *GetChr_player_00Control(void);
Chr_player_00Type *GetChr_player_00Type(void);
const Chr_player_00Item *GetChr_player_00Item(void);

// CHR:プレイヤー[00] の初期化処理
// CHR:プレイヤー[00] の終了処理
// CHR:プレイヤー[00] の更新処理
// CHR:プレイヤー[00] の描画処理
void InitChr_player_00(void);
void UninitChr_player_00(void);
void UpdateChr_player_00(void);
void DrawChr_player_00(void);

// CHR:プレイヤー[00] の読み込み処理
void LoadChr_player_00(void);

// CHR:プレイヤー[00] との当たり判定
int CollisionChr_player_00(VECTOR vector, Collision *pChrCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// CHR:プレイヤー[00] のダメージ処理
// CHR:プレイヤー[00] の回復処理
// CHR:プレイヤー[00] のノックバック処理
void DamageChr_player_00(int nDamage);
void HealChr_player_00(int nHeal);
void KnockBackChr_player_00(D3DXVECTOR3 rot, float fForce);

// CHR:プレイヤー[00] の管理状態設定処理
void SetChr_player_00ControlState(CHR_PLAYER_00_CONTROL_STATE state);

#if _DEBUG_COMMENT
// CHR:プレイヤー[00] のコメント描画処理
void DrawCommentChr_player_00(void);
#endif

#endif