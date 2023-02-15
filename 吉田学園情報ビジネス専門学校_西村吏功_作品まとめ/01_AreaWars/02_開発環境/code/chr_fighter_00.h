//========================================
//  *** chr_fighter_00.h ***
//========================================
#ifndef _CHR_FIGHTER_00_H_	// このマクロ定義がされていなかった時
#define _CHR_FIGHTER_00_H_	// 二重インクルード防止のマクロを定義する

#include "physics.h"
#include "atk_explosion_00.h"

//****************************************
// マクロ定義
//****************************************

// 機体[00] の種類の最大数
// 機体[00] の爆弾最大所持数
#define CHR_FIGHTER_00_TYPE_MAX	(80)
#define CHR_FIGHTER_00_BOMB_MAX	(2)

//****************************************
// 列挙型の定義
//****************************************

// 機体[00] の状態
typedef enum
{
	CHR_FIGHTER_00_STATE_NORMAL = 0,	// 通常
	CHR_FIGHTER_00_STATE_IN_POP,		// 出現中
	CHR_FIGHTER_00_STATE_DAMAGE,		// ダメージ
	CHR_FIGHTER_00_STATE_INVINCIBLE,	// 無敵
	CHR_FIGHTER_00_STATE_MAX
}CHR_FIGHTER_00_STATE;

//****************************************
// 構造体の定義
//****************************************

// 機体[00]構造体の定義
typedef struct
{
	// 位置関連
	D3DXVECTOR3	pos;			// 位置
	D3DXVECTOR3	move;			// 移動量
	float		fMove;			// 移動力
	D3DXVECTOR3	rot;			// 向き
	D3DXVECTOR3	targetRot;		// 目標向き
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	float		fHitTestWidth;	// 当たり判定の幅
	float		fHitTestHeight;	// 当たり判定の高さ

	// 分類情報
	int		nType;	// 種類
	bool	bUse;	// 使用されているかフラグ
	
	// 状態値
	int		nLife;						// 体力
	int		nBomb;						// 爆弾所持数
	bool	aState						//
			[CHR_FIGHTER_00_STATE_MAX];	// 状態フラグ
	int		aCounterState				//
			[CHR_FIGHTER_00_STATE_MAX];	// 状態管理カウンター
	int		nScore;						// スコア
	int		nPattern;					// テクスチャのパターンNo.
	int		nCounterAnim;				// アニメーションのカウンター
	int		nShotCounter;				// 弾の発射カウンター
	int		nShotTime;					// 弾の発射にかかる時間
	int		nInvincibleCounter;			// 無敵カウンター
	int		nCntDamage;					// 被弾カウント
}Chr_fighter_00;

// 機体[00] の種類毎の情報
typedef struct
{
	char			aName			//
					[TXT_MAX];		// 名称
	char			aTexturPath		//
					[TXT_MAX];		// テクスチャの相対パス
	int				nPtnMaxX;		// パターンの最大数(横)
	int				nPtnMaxY;		// パターンの最大数(縦)
	int				nFillStep;		// 変色部分のパターンの段数
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	float			fHitTestWidth;	// 当たり判定の幅
	float			fHitTestHeight;	// 当たり判定の高さ
	HIT_TEST_TYPE	hitTestType;	// 当たり判定の種類
	int				nLife;			// 体力
	float			fMove;			// 移動力
	int				nShotTime;		// 弾の発射にかかる時間
}Chr_fighter_00Type;

//****************************************
// プロトタイプ宣言
//****************************************

// 機体[00] の初期化処理
void InitChr_fighter_00(void);

// 機体[00] の終了処理
void UninitChr_fighter_00(void);

// 機体[00] の更新処理
void UpdateChr_fighter_00(void);

// 機体[00] の描画処理
void DrawChr_fighter_00(void);

// 機体[00] の読み込み処理
void LoadChr_fighter_00(void);

// 機体[00] の情報を取得
Chr_fighter_00 *GetChr_fighter_00(void);

// 機体[00] の種類毎の情報を取得
Chr_fighter_00Type *GetChr_fighter_00Type(void);

// 機体[00] の設定処理
void SetChr_fighter_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

// 機体[00] の破壊処理
void BreakChr_fighter_00(void);

// 機体[00] のヒット処理
void HitChr_fighter_00(int nDamage);

#endif