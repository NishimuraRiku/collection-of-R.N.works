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

//****************************************
// 列挙型定義
//****************************************
// 表示形式
typedef enum
{
	TEXT_DISP_CENTER,	// 中央揃え
	TEXT_DISP_LEFT,		// 左揃え
	TEXT_DISP_RIGHT,	// 右揃え
	TEXT_DISP_MAX,
}TEXT_DISP;

//****************************************
// 構造体定義
//****************************************
// フォント毎の情報構造体
typedef struct
{
	char aTexPath[TXT_MAX] = "";		// テクスチャファイルの相対パス
	char cStartChar;			// 開始文字
	unsigned int nPtnWidth;		// パターン幅
	unsigned int nPtnHeight;	// パターン高さ
	unsigned int nSpaceNum;		// 間隔比率の分子
	unsigned int nSpaceDenom;	// 間隔比率の分母
	int nTex;					// テクスチャ番号
}Font;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// テキストの初期化処理
void InitText(void);
//========== *** 入出力 ***
// フォントの読み込み処理
void LoadFont(void);
//========== *** 取得 ***
// フォント毎の情報を取得
Font GetFont(unsigned int nFont);

#endif