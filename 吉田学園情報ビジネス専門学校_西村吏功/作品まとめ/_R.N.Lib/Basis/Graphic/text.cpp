//========================================
// 
// テキストの処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** text.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//****************************************
// マクロ定義
//****************************************
// フォントのデータファイルの相対パス
#define FONT_DATA_FILE "data\\FONT_LIST.txt"
// フォントの最大数
#define FONT_MAX (16)

//****************************************
// グローバル変数宣言
//****************************************
static Font g_aFont[FONT_MAX];	// フォント毎の情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// InitText関数 - テキストの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitText(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetD3DDevice();

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// テクスチャの読み込み
		g_aFont[nCntFont].nTex = LoadTexture(g_aFont[nCntFont].aTexPath);
	}
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// LoadFont関数 - フォントの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadFont(void)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX] = "";	// データ検索用
	Font *pFont = g_aFont;		// フォント毎の情報のポインタ

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(FONT_DATA_FILE, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1) {
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))
		{// 読み込みを終了
			fclose(pFile);
			break; 
		}
		else if (!strcmp(aDataSearch, "FONTSET"))
		{// 部品の設定情報の読み込みを開始
			while (1) {
				fscanf(pFile, "%s", aDataSearch);	// 検索

				if (!strcmp(aDataSearch, "FONTSET_END")) 
				{// 読み込みを終了
					break;
				}
				else if (!strcmp(aDataSearch, "FONT"))
				{// 部品毎の情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	// 検索

						if (!strcmp(aDataSearch, "FONT_END"))
						{// ポインタを進める
							pFont++;
							break;
						}
						else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))
						{// ファイル名
							fscanf(pFile, "%s", &pFont->aTexPath); 
						}
						else if (!strcmp(aDataSearch, "START_CHAR:"))
						{// 開始文字
							fscanf(pFile, "%s", &pFont->cStartChar);
						}
						else if (!strcmp(aDataSearch, "PTN_WIDTH:"))
						{// パターン幅
							fscanf(pFile, "%d", &pFont->nPtnWidth);
						}
						else if (!strcmp(aDataSearch, "PTN_HEIGHT:"))
						{// パターン高さ
							fscanf(pFile, "%d", &pFont->nPtnHeight);
						}
						else if (!strcmp(aDataSearch, "SPACE_NUM:"))
						{// 間隔比率の分子 
							fscanf(pFile, "%d", &pFont->nSpaceNum);
						}
						else if (!strcmp(aDataSearch, "SPACE_DENOM:"))
						{// 間隔比率の分母 
							fscanf(pFile, "%d", &pFont->nSpaceDenom);
						}
					}
				}
			}
		}
	}
}

//========================================
// GetFont関数 - フォント毎の情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Font GetFont(unsigned int nFont) 
{
	return g_aFont[nFont];
}