//========================================
// 
// サウンドのヘッダファイル
// Author : AKIRA TANAKA
// |
// Arrange : RIKU NISHIMURA
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
	SOUND_TYPE_BGM,	// 音楽
	SOUND_TYPE_SE,	// 効果音
	SOUND_TYPE_MAX,
}SOUND_TYPE;

// サウンド一覧
typedef enum
{
	SOUND_LABEL_BGM_STAGE_000,			// BGM:ステージ	[000]
	SOUND_LABEL_SE_DAMAGE_000,			// SE :ダメージ	[000]
	SOUND_LABEL_SE_DAMAGE_001,			// SE :ダメージ	[001]
	SOUND_LABEL_SE_BREAK_000,			// SE :破壊		[000]
	SOUND_LABEL_SE_SHOT_000,			// SE :ショット	[000]
	SOUND_LABEL_SE_SHOT_001,			// SE :ショット	[001]
	SOUND_LABEL_SE_SHOT_002,			// SE :ショット	[002]
	SOUND_LABEL_SE_EXPLOSION_000,		// SE :爆発		[000]
	SOUND_LABEL_SE_EXPLOSION_001,		// SE :爆発		[001]
	SOUND_LABEL_SE_SELECT_000,			// SE :選択		[001]
	SOUND_LABEL_SE_BUY_000,				// SE :購入		[000]
	SOUND_LABEL_SE_SET_000,				// SE :設置		[000]
	SOUND_LABEL_SE_GET_000,				// SE :獲得		[000]
	SOUND_LABEL_SE_EXPLOSION_002,		// SE :爆発		[002]
	SOUND_LABEL_SE_SLASH_000,			// SE :斬撃		[000]
	SOUND_LABEL_SE_FOOTSTEP_000,		// SE :足音		[000]
	SOUND_LABEL_SE_FOOTSTEP_001,		// SE :足音		[001]
	SOUND_LABEL_SE_CRY_000,				// SE :鳴き声	[000]
	SOUND_LABEL_SE_CRY_001,				// SE :鳴き声	[001]
	SOUND_LABEL_SE_CRY_002,				// SE :鳴き声	[002]
	SOUND_LABEL_BGM_STAGE_001,			// BGM:ステージ	[001]
	SOUND_LABEL_SE_WARNING_000,			// SE :警告		[000]
	SOUND_LABEL_BGM_STAGE_002,			// BGM:ステージ	[002]
	SOUND_LABEL_SE_CRY_003,				// SE :鳴き声	[003]
	SOUND_LABEL_SE_CRY_004,				// SE :鳴き声	[004]
	SOUND_LABEL_SE_DETERMINATION_000,	// SE :決定		[000]
	SOUND_LABEL_SE_FOOTSTEP_002,		// SE :足音		[002]
	SOUND_LABEL_SE_PUSH_000,			// SE :押下		[000]
	SOUND_LABEL_SE_DETERMINATION_001,	// SE :決定		[001]
	SOUND_LABEL_BGM_TITLE_000,			// BGM:タイトル	[000]
	SOUND_LABEL_SE_SWITCH_000,			// SE :切り替え	[000]
	SOUND_LABEL_SE_PAUSE_000,			// SE :ポーズ	[000]
	SOUND_LABEL_SE_FOOTSTEP_003,		// SE :足音		[003]
	SOUND_LABEL_SE_METAL_000,			// SE :金属		[000]
	SOUND_LABEL_SE_IMPACT_000,			// SE :衝撃		[000]
	SOUND_LABEL_BGM_STAGE_003,			// BGM:ステージ	[003]
	SOUND_LABEL_BGM_STAGE_004,			// BGM:ステージ	[004]
	SOUND_LABEL_BGM_STAGE_005,			// BGM:ステージ	[005]
	SOUND_LABEL_BGM_CLEAR_000,			// BGM:クリア	[000]
	SOUND_LABEL_SE_METAL_001,			// SE :金属		[001]
	SOUND_LABEL_SE_ELEC_001,			// SE :電気		[000]
	SOUND_LABEL_SE_COUNT_000,			// SE :カウント	[000]
	SOUND_LABEL_MAX,
}SOUND_LABEL;

//****************************************
// 構造体の定義
//****************************************

// サウンド情報の構造体定義
typedef struct
{
	char		*pFilename;	// ファイル名
	int			nCntLoop;	// ループカウント
	float		fVolume;	// 音量
	SOUND_TYPE	type;		// サウンドの種類
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// サウンドの情報を取得
SOUNDINFO *GetSoundInfo(void);

// サウンドの初期化処理
// サウンドの終了処理
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

// セグメント再生(再生中なら停止)
// セグメント停止(ラベル指定)
// セグメント停止(全て)
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

// 設定音量を設定
void SetSettingVolume(SOUND_TYPE type);
// 種類毎に音量変更
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);
// 音量変更
void SoundVolumeChange(SOUND_LABEL label, float fVolume);
// 種類毎に再生停止
void StopSoundType(SOUND_TYPE type);

#endif
