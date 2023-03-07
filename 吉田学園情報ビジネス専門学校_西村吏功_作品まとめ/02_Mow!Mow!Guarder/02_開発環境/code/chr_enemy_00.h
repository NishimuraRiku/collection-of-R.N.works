//========================================
// 
// CHR:敵[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_enemy_00.h ***
//========================================
#ifndef _CHR_ENEMY_00_H_	// このマクロ定義がされていなかった時
#define _CHR_ENEMY_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"
#include "polygon3D.h"
#include "sound.h"

//****************************************
// マクロ定義
//****************************************
// CHR:敵[00] の最大数
#define CHR_ENEMY_00_MAX	(64)

// CHR:敵[00] の行動パターンの最大数
// CHR:敵[00] の行動情報の最大記憶数
// CHR:敵[00] の行動の種類毎の読み込める汎用値の最大数
#define CHR_ENEMY_00_ACTPTN_MAX			(8)
#define CHR_ENEMY_00_ACT_MEMORY_MAX		(128)
#define CHR_ENEMY_00_COMMAND_DATA_MAX	(32)

// CHR:敵[00] のウェーブカウント毎のダメージの上昇倍率
#define CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE	(0.05f)

//****************************************
// 列挙型の定義
//****************************************
// CHR:敵[00] の衝突オブジェクト番号
typedef enum
{
	CHR_ENEMY_00_HITOBJ_CHR_PLAYER_00,		// CHR:プレイヤー	[00]
	CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:ブロック		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_CORE_00,		// OBJ:コア			[00]
	CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:放電装置		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:ミラー		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00,		// OBJ:タレット		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_STAGE_00,		// OBJ:ステージ		[00]
	CHR_ENEMY_00_HITOBJ_MAX,
}CHR_ENEMY_00_HITOBJ;

// CHR:敵[00] の状態
typedef enum
{
	CHR_ENEMY_00_STATE_NORMAL,		// 通常
	CHR_ENEMY_00_STATE_PARALYSIS,	// 麻痺
	CHR_ENEMY_00_STATE_MAX,
}CHR_ENEMY_00_STATE;

// CHR:敵[00] の管理状態
typedef enum
{
	CHR_ENEMY_00_CONTROL_STATE_DYNAMIC,		// 動的
	CHR_ENEMY_00_CONTROL_STATE_STATIC,		// 静的
	CHR_ENEMY_00_CONTROL_STATE_BOSS_APPEAL,	// ボスアピール
	CHR_ENEMY_00_CONTROL_STATE_MAX,
}CHR_ENEMY_00_CONTROL_STATE;

// CHR:敵[00] のコマンド
typedef enum
{
	CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_START,			// 曲折式移動開始
	CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_END,				// 曲折式移動終了
	CHR_ENEMY_00_COMMAND_TURN_MOVE_START,				// 方向転換式移動開始
	CHR_ENEMY_00_COMMAND_TURN_MOVE_END,					// 方向転換式移動終了
	CHR_ENEMY_00_COMMAND_FORCE_MOVE_START,				// 推進式移動開始
	CHR_ENEMY_00_COMMAND_FORCE_MOVE_END,				// 推進式移動終了
	CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_START,		// 振り向き位置に振り向き開始
	CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_END,			// 振り向き位置に振り向き終了
	CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_START,		// 移動向きに振り向き開始
	CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_END,			// 移動向きに振り向き終了
	CHR_ENEMY_00_COMMAND_Z_LOCK,						// Z座標をロック
	CHR_ENEMY_00_COMMAND_Z_UNLOCK,						// Z座標をアンロック
	CHR_ENEMY_00_COMMAND_CHILD_SET,						// 子設定
	CHR_ENEMY_00_COMMAND_CHILD_DEPENDENCY,				// 子依存
	CHR_ENEMY_00_COMMAND_SPIN_START,					// 回転開始
	CHR_ENEMY_00_COMMAND_SPIN_END,						// 回転終了
	CHR_ENEMY_00_COMMAND_SHOT,							// 弾発射
	CHR_ENEMY_00_COMMAND_PLAY_SOUND,					// サウンド再生
	CHR_ENEMY_00_COMMAND_MOTION_SET,					// モーション設定
	CHR_ENEMY_00_COMMAND_AUTO_SHOT_START,				// 自動ショット開始
	CHR_ENEMY_00_COMMAND_AUTO_SHOT_END,					// 自動ショット終了
	CHR_ENEMY_00_COMMAND_UNIT_IGNORED_START,			// ユニット無視開始
	CHR_ENEMY_00_COMMAND_UNIT_IGNORED_END,				// ユニット無視終了
	CHR_ENEMY_00_COMMAND_MOVE_START,					// 移動開始
	CHR_ENEMY_00_COMMAND_MOVE_END,						// 移動終了
	CHR_ENEMY_00_COMMAND_ALLY_SET,						// 味方設定
	CHR_ENEMY_00_COMMAND_ATK_IMPACT,					// ATK:衝撃
	CHR_ENEMY_00_COMMAND_EFF_SHOCKWAVE_00,				// EFF:衝撃波[00]
	CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_CHANGE,		// 衝突ダメージ変更
	CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_UNDO,			// 衝突ダメージを元に戻す
	CHR_ENEMY_00_COMMAND_BOSS_DEPENDENCY,				// ボス依存
	CHR_ENEMY_00_COMMAND_ALLY_SET_ABSOLUTE,				// 味方設定(絶対位置)
	CHR_ENEMY_00_COMMAND_RECOGNITION_OFF,				// 認識OFF
	CHR_ENEMY_00_COMMAND_TELEPORTATION,					// 瞬間移動
	CHR_ENEMY_00_COMMAND_REFLECTION_START,				// 反射開始
	CHR_ENEMY_00_COMMAND_REFLECTION_END,				// 反射終了
	CHR_ENEMY_00_COMMAND_CHANGE_ACT_BY_HP,				// 残りHPに応じて行動パターンを変更
	CHR_ENEMY_00_COMMAND_RELEASE_PARENT,				// 親離れ
	CHR_ENEMY_00_COMMAND_CHANGE_POS,					// 座標変更
	CHR_ENEMY_00_COMMAND_DESTRUCTION,					// 自己破壊
	CHR_ENEMY_00_COMMAND_COUNT_SET,						// カウント設定
	CHR_ENEMY_00_COMMAND_PARENT_COUNT_UPDATE_ON_DEATH,	// 死んだ時に親のカウント値を変動させる
	CHR_ENEMY_00_COMMAND_CHANGE_ACT_BY_COUNT,			// カウントに応じて行動パターンを変更
	CHR_ENEMY_00_COMMAND_EFF_EXPLOSION_00,				// EFF:爆発[00]
	CHR_ENEMY_00_COMMAND_MAX,
}CHR_ENEMY_00_COMMAND;

// CHR:敵[00] の行動
typedef enum
{
	CHR_ENEMY_00_ACT_TORTUOUS_MOVE,		// 曲折式移動
	CHR_ENEMY_00_ACT_TURN_MOVE,			// 方向転換式移動
	CHR_ENEMY_00_ACT_FORCE_MOVE,		// 推進式移動
	CHR_ENEMY_00_ACT_TURN_TO_TURN_POS,	// 振り向き位置に振り向く
	CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT,	// 移動向きに振り向く
	CHR_ENEMY_00_ACT_Z_LOCK,			// Z座標ロック
	CHR_ENEMY_00_ACT_CHILD_DEPENDENCY,	// 子依存
	CHR_ENEMY_00_ACT_SPIN,				// 回転
	CHR_ENEMY_00_ACT_MOVE,				// 移動
	CHR_ENEMY_00_ACT_MAX,
}CHR_ENEMY_00_ACT;

// CHR:敵[00] のランク
typedef enum
{
	CHR_ENEMY_00_RANK_LOWER,	// 低位
	CHR_ENEMY_00_RANK_BOSS,		// ボス
	CHR_ENEMY_00_RANK_ALLY,		// 味方
	CHR_ENEMY_00_RANK_MAX
}CHR_ENEMY_00_RANK;

//****************************************
// 構造体の定義
//****************************************
// CHR:敵[00] の行動管理情報構造体
typedef struct 
{
	int			nActPtn;		// 行動パターン番号
	bool		aAct			// 行動フラグ
				[CHR_ENEMY_00_ACT_MAX];
	int			nCounterAct		// 行動カウンター
				= -1;
	int			nCounterMove;	// 移動カウンター
	float		fRotation;		// 回転力
	float		fRandPosRadius;	// 乱数位置半径
	D3DXVECTOR3 spin;			// 回転量
	D3DXVECTOR3 move;			// 移動量
}Chr_enemy_00ActInfo;

// CHR:敵[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 posOld;		// 過去の位置(1フレーム前)
	D3DXVECTOR3 force;		// 力
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 現在の向き
	D3DXVECTOR3 rotOld;		// 過去の位置(1フレーム前)
	D3DXVECTOR3 moveRot;	// 移動向き
	D3DXVECTOR3 *pTurnPos;	// 振り向き先の位置のポインタ

	// 分類情報
	int nType;	// 種類

	// 状態関連
	bool				bUse;				// 使用されているかフラグ
	int					nHP;				// ヒットポイント
	int					nHPMax;				// ヒットポイントの上限
	CHR_ENEMY_00_STATE	state;				// 状態
	int					nCounterState;		// 状態カウンター
	int					nCounterShot;		// ショットカウンター
	int					nCounterCollAtk;	// 衝突攻撃カウンター
	int					nCounterInvincible;	// 無敵カウンター
	int					nCounterDamage;		// ダメージカウンター
	int					nAttackIndex;		// 攻撃するオブジェクトの番号
	CHR_ENEMY_00_HITOBJ	attackObj;			// 攻撃するオブジェクトの分類
	float				fVibration;			// 振動の強さ
	int					nCounterVib;		// 振動カウンター
	D3DXVECTOR3			vibrationPos;		// 振動位置
	int					nParentIdx;			// 親の番号
	int					nParentIdxOld;		// 過去の親の番号
	bool				bAutoShot;			// 自動ショットフラグ
	int					nBulletType;		// 弾の種類
	int					nShotTimeMin;		// 発射間隔の下限
	int					nShotTimeMax;		// 発射間隔の上限
	bool				bUnitIgnored;		// ユニット無視フラグ
	bool				bCollDamageChange;	// 衝突ダメージ変更フラグ
	int					nNewCollDamage;		// 新規衝突ダメージ
	bool				bBossDependency;	// ボス依存フラグ
	bool				bRecognitionOff;	// 認識OFFフラグ
	bool				bReflection;		// 反射フラグ
	int					nChangeActHP;		// 行動パターンを変更するHP
	int					nCount;				// カウント
	int			nParentCountUpdateOnDeath;	// 死んだ時の親のカウント値を変動値
	int					nChangeActCount;	// 行動パターンを変更するカウント
	int					nChangeAct;			// 変更する行動パターン

	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報
	Collision	cmnCollision; 	// 共通の衝突情報
	Collision	aObjCollision	// オブジェクト毎の衝突情報
				[CHR_ENEMY_00_HITOBJ_MAX];

	// AI関連
	Chr_enemy_00ActInfo actInfo;	// 行動管理情報

	// 部品関連
	Parts3DInfo	partsInfo;		// 部品管理
	int			nMotion;		// モーション
	int			nMotionTemp;	// モーション一時保存
}Chr_enemy_00;

// CHR:敵[00] の行動の種類毎の情報構造体
typedef struct
{
	char	aLabelName	// ラベル名
			[TXT_MAX];
	int		nDataNum;	// 値の数
}Chr_enemy_00ActType;

// CHR:敵[00] の行動情報構造体
typedef struct
{
	int						nTime;	// 行動する時間
	CHR_ENEMY_00_COMMAND	cmd;	// コマンド
	float					aData	// 汎用値
							[CHR_ENEMY_00_COMMAND_DATA_MAX];
}Chr_enemy_00Act;

// CHR:敵[00] の行動パターン情報構造体
typedef struct
{
	int				nLoopTime;	// ループ時間
	int				nActNum;	// 行動数
	Chr_enemy_00Act	aAct		// 行動情報
					[CHR_ENEMY_00_ACT_MEMORY_MAX];
}Chr_enemy_00ActPtn;

// CHR:敵[00] の行動パターン設定情報構造体
typedef struct
{
	Chr_enemy_00ActPtn	aActPtn	// 行動パターン情報
						[CHR_ENEMY_00_ACTPTN_MAX];
}Chr_enemy_00ActPtnSet;

// CHR:敵[00] の種類毎の情報構造体
typedef struct
{
	int						nHP;					// HP
	float					fMoveForce;				// 移動力
	int						nCollisionDamage;		// 衝突ダメージ
	int						nCollAtkTime;			// 衝突攻撃の間隔
	int						nExplosionType;			// 爆発の種類
	float					fShadowRadius;			// 影の半径
	float					fPlayerSearchDistance;	// プレイヤー索敵距離
	float					fPlayerAimProbability;	// プレイヤーを狙う確率
	float					fCoreAimProbability;	// コアを狙う確率
	int						nDropCoin;				// ドロップするコイン量
	int						nScore;					// スコア
	CHR_ENEMY_00_RANK		rank;					// ランク
	SOUND_LABEL				damageSE;				// ダメージSE
	Chr_enemy_00ActPtnSet	actPtnSet;				// 行動パターン設定情報
	HitTestSet				hitTestSet;				// 当たり判定設定情報
	Parts3DSet				partsSet;				// 部品設定情報(3D)
	Motion3DSet				motionSet;				// モーション設定情報
}Chr_enemy_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// CHR:敵[00] の情報を取得
Chr_enemy_00 *GetChr_enemy_00(void);
// CHR:敵[00] の種類毎の情報を取得
Chr_enemy_00Type *GetChr_enemy_00Type(void);

// CHR:敵[00] の初期化処理
void InitChr_enemy_00(void);
// CHR:敵[00] の終了処理
void UninitChr_enemy_00(void);
// CHR:敵[00] の更新処理
void UpdateChr_enemy_00(void);
// CHR:敵[00] の描画処理
void DrawChr_enemy_00(void);

// CHR:敵[00] の読み込み処理
void LoadChr_enemy_00(void);
// CHR:敵[00] の設定処理
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
// CHR:敵[00] のボス設定処理
void SetBossChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
// CHR:敵[00] との当たり判定処理
int CollisionChr_enemy_00(VECTOR vector, Collision *pChrCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);
// CHR:敵[00] のダメージ処理
void DamageChr_enemy_00(int nIndex, int nDamage);
// CHR:敵[00] のノックバック処理
void KnockBackChr_enemy_00(int nIndex, D3DXVECTOR3 rot, float fForce);
// CHR:敵[00] の状態付与処理
void GiveStateChr_enemy_00(int nIndex, CHR_ENEMY_00_STATE state);
// CHR:敵[00] の管理状態設定処理
void SetChr_enemy_00ControlState(CHR_ENEMY_00_CONTROL_STATE state);
// CHR:敵[00] の味方カウント処理
int AllyChr_enemy_00Count(void);

#endif