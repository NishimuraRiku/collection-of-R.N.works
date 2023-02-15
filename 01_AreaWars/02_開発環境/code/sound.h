//========================================
// 
// サウンド処理のヘッダファイル
// Author : AKIRA TANAKA
// |
// Arrange : 西村 吏功
// 
//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SOUND_H_	// このマクロ定義がされていなかった時
#define _SOUND_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 列挙型の定義
//****************************************

// サウンドの種類
typedef enum 
{
	SOUND_TYPE_BGM = 0,	// 音楽
	SOUND_TYPE_SE,		// 効果音
	SOUND_TYPE_MAX,
}SOUND_TYPE;

// サウンド一覧
typedef enum
{
	SOUND_LABEL_BGM_TITLE_000 = 0,	// タイトルBGM			[000]
	SOUND_LABEL_BGM_STAGE_000,		// ステージBGM			[000]
	SOUND_LABEL_BGM_STAGE_001,		// ステージBGM			[001]
	SOUND_LABEL_BGM_BOSS_000,		// ボスBGM				[000]
	SOUND_LABEL_BGM_BOSS_001,		// ボスBGM				[001]
	SOUND_LABEL_SE_GAME_CLEAR_000,	// ゲームクリアSE		[000]
	SOUND_LABEL_SE_GAME_OVER_000,	// ゲームオーバーSE		[000]
	SOUND_LABEL_SE_SELECT_000,		// 選択SE				[000]
	SOUND_LABEL_SE_SELECT_001,		// 選択SE				[001]
	SOUND_LABEL_SE_DECITION_000,	// 決定SE				[000]
	SOUND_LABEL_SE_DECITION_001,	// 決定SE				[001]
	SOUND_LABEL_SE_DELETION_000,	// 削除SE				[000]
	SOUND_LABEL_SE_PAUSE_START_000,	// ポーズ開始SE			[000]
	SOUND_LABEL_SE_PAUSE_END_000,	// ポーズ終了SE			[000]
	SOUND_LABEL_SE_COUNT_DOWN_000,	// カウントダウンSE		[000]
	SOUND_LABEL_SE_SCORE_COUNT_000,	// スコアカウントSE		[000]
	SOUND_LABEL_SE_TEXT_COUNT_000,	// テキストカウントSE	[000]
	SOUND_LABEL_SE_RANKING_SET_000,	// ランキングセットSE	[000]
	SOUND_LABEL_SE_DAMAGE_000,		// ダメージSE			[000]
	SOUND_LABEL_SE_DAMAGE_001,		// ダメージSE			[001]
	SOUND_LABEL_SE_BONUS_000,		// ボーナスSE			[000]
	SOUND_LABEL_SE_WARNING_000,		// 警告SE				[000]
	SOUND_LABEL_SE_SHOT_000,		// ショットSE			[000]
	SOUND_LABEL_SE_SHOT_001,		// ショットSE			[001]
	SOUND_LABEL_SE_SHOT_002,		// ショットSE			[002]
	SOUND_LABEL_SE_SHOT_003,		// ショットSE			[003]
	SOUND_LABEL_SE_SHOT_004,		// ショットSE			[004]
	SOUND_LABEL_SE_EXPLOSION_000,	// 爆発SE				[000]
	SOUND_LABEL_SE_EXPLOSION_001,	// 爆発SE				[001]
	SOUND_LABEL_SE_EXPLOSION_002,	// 爆発SE				[002]
	SOUND_LABEL_SE_EXPLOSION_003,	// 爆発SE				[003]
	SOUND_LABEL_SE_EXPLOSION_004,	// 爆発SE				[004]
	SOUND_LABEL_SE_EXPLOSION_005,	// 爆発SE				[005]
	SOUND_LABEL_SE_DIFFUSION_000,	// 拡散SE				[000]
	SOUND_LABEL_SE_LOCK_ON_000,		// ロックオンSE			[000]
	SOUND_LABEL_SE_OPEN_000,		// 開くSE				[000]
	SOUND_LABEL_SE_METAL_000,		// 金属SE				[000]
	SOUND_LABEL_SE_COLLISION_000,	// 衝突SE				[000]
	SOUND_LABEL_SE_SORTIE_000,		// 出撃SE				[000]
	SOUND_LABEL_SE_LANDING_000,		// 着地SE				[000]
	SOUND_LABEL_SE_GET_000,			// 獲得SE				[000]
	SOUND_LABEL_MAX
}SOUND_LABEL;

//****************************************
// 構造体の定義
//****************************************

// サウンド情報の構造体定義
typedef struct
{
	char		*pFilename;	// ファイル名
	char		*pLoadName;	// 読み込み名
	int			nCntLoop;	// ループカウント
	float		fVolume;	// 音量
	SOUND_TYPE	type;		// サウンドの種類
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

// サウンドの情報を取得
SOUNDINFO *GetSoundInfo(void);

// サウンドの種類を文字列から読み込む
void StringLoadSound(char aString[TXT_MAX], SOUND_LABEL *pLabel);

// サウンドの初期化処理
HRESULT InitSound(HWND hWnd);

// サウンドの終了処理
void UninitSound(void);

// セグメント再生(再生中なら停止)
HRESULT PlaySound(SOUND_LABEL label);

// セグメント停止(ラベル指定)
void StopSound(SOUND_LABEL label);

// セグメント停止(全て)
void StopSound(void);

// 種類毎に音量変更
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);

// 設定音量を設定
void SetSettingVolume(SOUND_TYPE type);

// 種類毎に再生停止
void StopSoundType(SOUND_TYPE type);

#endif
