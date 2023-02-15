//========================================
// 
// テキストのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_	// このマクロ定義がされていなかった時
#define _TEXT_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// マクロ定義
//****************************************
// テキスト(2D)の最大数
#define TEXT2D_MAX	(128)

// テキスト(3D)の最大数
#define TEXT3D_MAX	(128)

//****************************************
// 列挙型の定義
//****************************************
// フォント
typedef enum
{
	FONT_000,
	FONT_001,
	FONT_002,
	FONT_003,
	FONT_004,
	FONT_MAX
}FONT;

// 表示形式
typedef enum
{
	DISPLAY_CENTER,	// 中央揃え
	DISPLAY_LEFT,	// 左詰め
	DISPLAY_RIGHT,	// 右詰め
}DISPLAY;

// フォントの種類毎の情報構造体
typedef struct
{
	const char	c_aPath			// テクスチャファイルの相対パス
				[TXT_MAX];
	const float	c_fWidth;		// 幅
	const float	c_fHeight;		// 高さ
	const float	c_fSpaceX;		// 配置間隔X
	const float	c_fSpaceY;		// 配置間隔Y
	const char	c_cStartChar;	// 開始文字
	const int	c_nPtnMaxX;		// パターン数(横)
	const int	c_nPtnMaxY;		// パターン数(縦)
}Font;

//****************************************
// プロトタイプ宣言
//****************************************
// フォントの情報を取得
Font *GetFont(void);

// テキストの初期化処理
// テキストの終了処理
// テキストの描画処理
void InitText(void);
void UninitText(void);
void DrawText_(void);

// テキスト(2D)の描画処理
void DrawText2D(void);

// テキスト(3D)の描画処理
void DrawText3D(void);

// テキスト(2D)の設定処理
void SetText2D(char *pString, FONT font, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight);

// テキスト(3D)の設定処理
void SetText3D(char *pString, FONT font, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard);

#endif