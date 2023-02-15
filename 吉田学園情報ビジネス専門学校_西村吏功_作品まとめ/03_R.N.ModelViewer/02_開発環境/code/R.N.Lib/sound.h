//========================================
// 
// サウンドのヘッダファイル
// Author:AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SOUND_H_	// このマクロ定義がされていなかった時
#define _SOUND_H_	// 二重インクルード防止のマクロを定義する

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

//****************************************
// 構造体の定義
//****************************************
// サウンド情報の構造体定義
typedef struct
{
	// ファイル名
	char aFileName[TXT_MAX];
	int nCntLoop;		// ループカウント
	float fVolume;		// 音量
	SOUND_TYPE type;	// サウンドの種類
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//========== *** 基本接続 ***
// サウンドの初期化処理
HRESULT InitSound(HWND hWnd);
// サウンドの終了処理
void UninitSound(void);
//========== *** 入出力 ***
// サウンドの読み込み処理
void LoadSound(void);
//========== *** 取得 ***
// サウンドの情報を取得
SOUNDINFO *GetSoundInfo(void);
//========== *** 操作 ***
// セグメント再生(再生中なら停止)
HRESULT PlaySound(int nSoundIdx);
// セグメント停止(ラベル指定)
void StopSound(int nSoundIdx);
// セグメント停止(全て)
void StopSound(void);
// 設定音量を設定
void SetSettingVolume(SOUND_TYPE type);
// 種類毎に音量変更
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);
// 種類毎に再生停止
void StopSoundType(SOUND_TYPE type);

#endif
