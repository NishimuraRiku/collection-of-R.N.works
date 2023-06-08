//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SYS_STAGE_00_H_	// このマクロ定義がされていなかった時
#define _SYS_STAGE_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
#define ENEMY_SET_MAX	(256)	// 敵の配置情報の最大記憶数

//****************************************
// 構造体の定義
//****************************************

// 敵の配置情報構造体
typedef struct 
{
	int			nTime;		// 配置する時間
	int			nType;		// 配置する種類
	D3DXVECTOR3 setPos;		// 配置する位置
	D3DXVECTOR3 setRot;		// 配置する角度
	bool		bAreaBoss;	// エリアボスフラグ
	bool		bBomb;		// 爆弾所持フラグ
	int			nParentNum;	// 親番号
	int			nChildNum;	// 子供番号
}EnemySet;

// ステージ構造体
typedef struct
{
	int			nStageNum;					// ステージ番号
	int			nWaveMax;					// ウェーブの最大数
	int			nWaveCount;					// ウェーブのカウント
	int			nDispWaveCount;				// 表示するウェーブのカウント
	int			nWaveTime;					// ウェーブの時間
	int			nWaveEndTime;				// ウェーブの終了時間
	bool		bWaveUp;					// ウェーブ数を増やすか否か
	bool		bWarning;					// 警告フラグ
	bool		bSavePoint;					// セーブ地点フラグ
	bool		bShowAreaTime;				// エリア時間の表示フラグ
	int			nLastWave;					// 最終ウェーブ
	int			nLastDispWave;				// 最終表示ウェーブ
	int			nLastScore;					// 最終スコア
	int			nLastBomb;					// 最終ボム保持数
	int			nEnemySetNum;				// 敵の配置数
	EnemySet	aEnemySet[ENEMY_SET_MAX];	// 敵の配置情報
	SOUND_LABEL	sound;						// サウンド番号
	SOUND_LABEL soundNow;					// 現在再生中のサウンド番号
}Sys_stage_00;

//****************************************
// プロトタイプ宣言
//****************************************
// システム:ステージ[00] の初期化処理
void InitSys_stage_00(int nStageNum);

// システム:ステージ[00] の終了処理
void UninitSys_stage_00(void);

// システム:ステージ[00] の更新処理
void UpdateSys_stage_00(void);

// システム:ステージ[00] の情報を取得
Sys_stage_00 *GetSys_stage_00(void);

// システム:ステージ[00] のウェーブの推移処理
void WaveTrendSys_stage_00(void);

// システム:ステージ[00] のコンティニュー処理
void ContinueSys_stage_00(void);

#endif