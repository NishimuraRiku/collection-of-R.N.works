//========================================
// 
// メインのヘッダファイル
// Author:RIKU NISHIMURA
// 
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
// ウインドウクラス
#define CLASS_NAME "WindowClass"
// ウインドウの幅
#define SCREEN_WIDTH (960)	// NORMAL=1280 4:3=960
// ウインドウの高さ
#define SCREEN_HEIGHT (720)
// 頂点フォーマット(2D)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
// 頂点フォーマット(3D)
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
// 文字列の最大数(汎用)
#define TXT_MAX (256)

// ウインドウの中心座標X
#define SCREEN_CENTER_X	(SCREEN_WIDTH*0.5f)
// ウインドウの中心座標Y
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT*0.5f)

// 内側スクリーンの左端
#define INSIDE_SCREEN_LEFTMOST (960)
// 内側スクリーンの右端
#define INSIDE_SCREEN_RIGHTMOST (720)

// D3DXVECTOR3の初期化値
#define INITD3DXVECTOR3 D3DXVECTOR3(0.0f,0.0f,0.0f)
// Colorの初期化値
#define INITCOLOR Color{255,255,255,255}
// 背景色の初期値
#define INITBGCOLOR Color{58,58,58,255}

//****************************************
// 構造体の定義
//****************************************
// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float		rhw;	// 座標変換用係数(1.0fで固定)
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_2D;
// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_3D;
// 拡大倍率構造体
typedef struct 
{
	float fWidth;	// 幅
	float fHeight;	// 高さ
	float fDepth;	// 奥行き
}Scale;
// 色構造体
typedef struct 
{
	int r;
	int g;
	int b;
	int a;
}Color;
// メイン処理の情報構造体
typedef struct 
{
	// Direct3Dオブジェクトへのポインタ
	LPDIRECT3D9 pD3D = NULL;
	// Direct3Dデバイスへのポインタ
	LPDIRECT3DDEVICE9 pD3DDevice = NULL;
	HWND hWnd;				// ウインドウハンドル(識別子)
	bool bPause = false;	// ポーズフラグ
	Color bgCol;			// 背景色
	int nCountFPS = 0;		// FPSのカウンター
	D3DXVECTOR3 windowPos;	// ウインドウの位置
}Main;

//****************************************
// プロトタイプ宣言
//****************************************
// メイン処理の情報の取得
Main *GetMain(void);
// デバイスを取得
LPDIRECT3DDEVICE9 GetDevice(void);
// ウインドウハンドルを取得
HWND *GetWindowHandle(void);
// ウインドウの位置を取得
D3DXVECTOR3 GetWindowPos(void);
// ウインドウのモードを切り替える
void WindowModeChange(bool bFullScreen);
// メッセージ処理
void Message(char *pText, char *pCaption);

#endif