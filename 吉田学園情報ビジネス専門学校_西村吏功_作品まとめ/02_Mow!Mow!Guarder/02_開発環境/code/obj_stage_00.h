//========================================
// 
// OBJ:ステージ[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_stage_00.h ***
//========================================
#ifndef _OBJ_STAGE_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_STAGE_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "obj_pedestal_00.h"	// OBJ:台座	[00]

//****************************************
// マクロ定義
//****************************************
// OBJ:ステージ[00] のゲートの最大数
// OBJ:ステージ[00] の台座の最大数
#define OBJ_STAGE_00_GATE_MAX		(4)
#define OBJ_STAGE_00_PEDESTAL_MAX	(8)

// OBJ:ステージ[00] のウェーブ情報の最大数
// OBJ:ステージ[00] の敵設定情報の最大数
// OBJ:ステージ[00] の品揃え情報の最大数
#define OBJ_STAGE_00_WAVE_MAX		(64)
#define OBJ_STAGE_00_ENEMYSET_MAX	(64)
#define OBJ_STAGE_00_ITEMS_MAX		(8)

//****************************************
// 列挙型の定義
//****************************************
// OBJ:ステージ[00] の状態
typedef enum 
{
	OBJ_STAGE_00_STATE_NONE,		// 無し
	OBJ_STAGE_00_STATE_INTERVAL,	// インターバル
	OBJ_STAGE_00_STATE_RAID,		// 襲撃
	OBJ_STAGE_00_STATE_CLEAR,		// クリア
	OBJ_STAGE_00_STATE_STOP,		// 停止
}OBJ_STAGE_00_STATE;

// OBJ:ステージ[00] のテキストの状態
typedef enum
{
	OBJ_STAGE_00_TEXT_STATE_HIDE,		// 消えている
	OBJ_STAGE_00_TEXT_STATE_APPEAR,		// 現れ中
	OBJ_STAGE_00_TEXT_STATE_DISAPPEAR,	// 消え中
	OBJ_STAGE_00_TEXT_STATE_MAX,
}OBJ_STAGE_00_TEXT_STATE;

//****************************************
// 構造体の定義
//****************************************
// OBJ:ステージ[00] のボス設定情報
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
	int			nType;	// 種類
}Obj_stage_00SetBoss;

// OBJ:ステージ[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き

	// 分類情報
	int	nType;	// 種類

	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報

	// 状態関連
	int						nCounter;				// カウンター
	OBJ_STAGE_00_STATE		state;					// 状態
	int						nWave;					// ウェーブ番号
	int						nWaveOld;				// 過去のウェーブ番号
	int						nCntWaveMain;			// ウェーブカウント(メイン)
	int						nCntWaveSub;			// ウェーブカウント(サブ)
	int						nKill;					// キル数
	OBJ_STAGE_00_TEXT_STATE	textState;				// テキストの状態
	int						nCounterText;			// テキストカウンター
	int						nCounterDiscoloration;	// 変色カウンター
	Color					groundCol;				// 地面の色
	Obj_stage_00SetBoss		setBoss;				// ボス設定情報
	int						nTimeBonus;				// タイムボーナス
	int						nCounterTimeBonus;		// タイムボーナスカウンター
	int						nCounterTimeBonusText;	// タイムボーナステキストカウンター

	// 部品関連
	Parts3DInfo	partsInfo;	// 部品管理
}Obj_stage_00;

// OBJ:ステージ[00] のゲート情報
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 bossPoint;	// ボス設定位置
	D3DXVECTOR3 setRot;		// 設定向き
	float		fWidth;		// 幅
	float		fDepth;		// 奥行き
}Obj_stage_00Gate;

// OBJ:ステージ[00] のゲート設定情報
typedef struct
{
	int					nGateNum;	// ゲート数
	Obj_stage_00Gate	aGate		// ゲート情報
						[OBJ_STAGE_00_GATE_MAX];
}Obj_stage_00GateSet;

// OBJ:ステージ[00] の注意マーク情報
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
}Obj_stage_00AttentionMark;

// OBJ:ステージ[00] の注意マーク設定情報
typedef struct
{
	Obj_stage_00AttentionMark	aAttentionMark	// 注意マーク情報
								[OBJ_STAGE_00_GATE_MAX];
}Obj_stage_00AttentionMarkSet;

// OBJ:ステージ[00] の台座情報
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	int			nType;	// 種類
}Obj_stage_00Pedestal;

// OBJ:ステージ[00] の台座設定情報
typedef struct
{
	int						nPedestalNum;	// 台座数
	Obj_stage_00Pedestal	aPedestal		// 台座情報
							[OBJ_STAGE_00_PEDESTAL_MAX];
}Obj_stage_00PedestalSet;

// OBJ:ステージ[00] の敵設定情報
typedef struct
{
	int nTime;	// 時間
	int	nGate;	// ゲート番号
	int nType;	// 種類
}Obj_stage_00EnemySet;

// OBJ:ステージ[00] のウェーブ情報
typedef struct
{
	int						nIntervalTime;			// インターバルタイム
	int						nEnemyNum;				// 敵の数
	Obj_stage_00EnemySet	aEnemySet				// 敵設定情報
							[OBJ_STAGE_00_ENEMYSET_MAX];
	SOUND_LABEL				intervalBGM;			// インターバルBGM
	SOUND_LABEL				raidBGM;				// 襲撃BGM
	Color					intervalGroundColor;	// インターバル時の地面の色
	Color					intervalSkyColor;		// インターバル時の空の色
	Color					raidGroundColor;		// 襲撃時の地面の色
	Color					raidSkyColor;			// 襲撃時の空の色
	bool					bWarningText;			// 注意テキストフラグ
	bool					bMainCount;				// メインカウントフラグ
}Obj_stage_00Wave;

// OBJ:ステージ[00] のウェーブ設定情報
typedef struct
{
	int					nLoopPoint;	// ループ地点
	int					nWaveNum;	// ウェーブ数
	Obj_stage_00Wave	aWave		// ウェーブ情報
						[OBJ_STAGE_00_WAVE_MAX];
}Obj_stage_00WaveSet;

// OBJ:ステージ[00] の品揃え情報
typedef struct
{
	OBJ_PEDESTAL_00_ITEM	aItem	// 商品番号
							[OBJ_STAGE_00_PEDESTAL_MAX];
}Obj_stage_00Items;

// OBJ:ステージ[00] の品揃え設定情報
typedef struct
{
	int					nItemsNum;	// 品揃え数
	Obj_stage_00Items	aItems		// 品揃え情報
						[OBJ_STAGE_00_ITEMS_MAX];
}Obj_stage_00ItemsSet;

// OBJ:ステージ[00] の種類毎の情報構造体
typedef struct
{
	D3DXVECTOR3						coreSetPos;			// コア設定位置
	D3DXVECTOR3						switchSetPos;		// スイッチ設定位置
	Obj_stage_00GateSet				gateSet;			// ゲート設定情報
	Obj_stage_00AttentionMarkSet	attentionMarkSet;	// 注意マーク設定情報
	Obj_stage_00PedestalSet			pedestalSet;		// 台座設定情報
	Obj_stage_00ItemsSet			itemsSet;			// 品揃え設定処理
	Obj_stage_00WaveSet				waveSet;			// ウェーブ設定情報
	HitTestSet						hitTestSet;			// 当たり判定設定情報
	Parts3DSet						partsSet;			// 部品設定情報(3D)
}Obj_stage_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:ステージ[00] の情報を取得
// OBJ:ステージ[00] の種類毎の情報を取得
Obj_stage_00 *GetObj_stage_00(void);
Obj_stage_00Type *GetObj_stage_00Type(void);

// OBJ:ステージ[00] の初期化処理
// OBJ:ステージ[00] の終了処理
// OBJ:ステージ[00] の更新処理
// OBJ:ステージ[00] の描画処理
void InitObj_stage_00(void);
void UninitObj_stage_00(void);
void UpdateObj_stage_00(void);
void DrawObj_stage_00(void);

// OBJ:ステージ[00] の読み込み処理
void LoadObj_stage_00(void);

// OBJ:ステージ[00] との当たり判定処理
// OBJ:ステージ[00] との当たり判定処理(With設定位置)
void CollisionObj_stage_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);
void CollisionObj_stage_00WithSetPos(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:ステージ[00] の状態設定処理
void SetStateObj_stage_00(OBJ_STAGE_00_STATE state);

// OBJ:ステージ[00] のボス設定処理
void BossSetObj_stage_00(void);

#endif