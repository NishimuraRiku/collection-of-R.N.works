//========================================
// *** main.h ***
//========================================
#ifndef _MAIN_H_	// このマクロ定義がされていなかった時
#define _MAIN_H_	// 二重インクルード防止のマクロを定義する

#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"						// 入力処理に必要
#include "Xinput.h"						// 入力処理に必要
#include "xaudio2.h"					// サウンド処理に必要

//****************************************
// ライブラリのリンク
//****************************************
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネントを使用するのに必要
#pragma comment(lib,"dsound.lib")	// DirectSoundを使用するのに必要
#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要
#pragma comment(lib,"xinput.lib")	// 入力処理に必要
#pragma warning(disable:4091)		// 警告:C4091を無視する
#pragma warning(disable:4244)		// 警告:C4244を無視する

//****************************************
// マクロ定義
//****************************************
#define CLASS_NAME		"WindowClass"								// ウインドウクラス
#define SCREEN_WIDTH	(1280)										// ウインドウの幅
#define SCREEN_HEIGHT	(720)										// ウインドウの高さ
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// 頂点フォーマット(2D)
#define TXT_MAX			(256)										// 文字列の最大数(汎用)
#define PIXEL			(2.0f)										// 1ドットの大きさ

//****************************************
// 列挙型の定義
//****************************************

// 画面(モード)の種類
typedef enum
{
	MODE_TITLE_NONE = 0,	// 無し
	MODE_TITLE_00,			// タイトル画面[00]
	MODE_GAME_00,			// ゲーム画面[00]
	MODE_RESULT_00,			// リザルト画面[00]
	MODE_MAX
}MODE;

//****************************************
// 構造体の定義
//****************************************

// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_2D;

// 大きさの倍率構造体
typedef struct
{
	float fWidth;	// 幅
	float fHeight;	// 高さ
}ScaleDiameter;

// 色構造体
typedef struct 
{
	int nR;
	int nG;
	int nB;
	int nA;
}Color;

//****************************************
// プロトタイプ宣言
//****************************************
LPDIRECT3DDEVICE9 GetDevice	(void);											// デバイスを取得
HWND *GetWindowHandle		(void);											// ウインドウハンドルを取得
D3DXVECTOR3 GetWindowPos	(void);											// ウインドウの位置を取得
void SetMode				(MODE mode);									// モードの設定
MODE GetMode				(void);											// モードの取得
void WindowModeChange		(bool bFullScreen);								// ウインドウのモードを切り替える
bool Lottery				(float fProb);									// 抽選処理
float fRand					(float fMax);									// 小数の乱数を返す
void MatchPosition			(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos);	// 描画位置を基準に合わせる
int PrusMinus				(float fNum);									// 与えられた値に応じて1か-1を返す
void strinit				(char *pData, int nNum);						// 空白埋め

#endif