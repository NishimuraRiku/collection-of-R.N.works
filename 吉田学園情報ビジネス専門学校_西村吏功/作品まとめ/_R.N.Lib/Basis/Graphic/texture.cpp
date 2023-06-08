//========================================
// 
// テクスチャ関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** texture.cpp ***
//========================================
#include "assert.h"
// R.N.Lib
#include "../../R.N.Lib.h"

//****************************************
// マクロ定義
//****************************************
// テクスチャの最大数
#define TEXTURE_MAX (1024)

//****************************************
// 構造体定義
//****************************************
// テクスチャの管理情報
typedef struct
{
	//========== *** 読み込み関連 ***
	// 読み込み済のテクスチャパス
	char aLoadTexturePath[TEXTURE_MAX][TXT_MAX] = {};
	int nTextureNum = 0;	// テクスチャの種類数
}TextureInfo;

//****************************************
// グローバル変数宣言
//****************************************
// テクスチャ関連のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9 g_aTexture[TEXTURE_MAX] = {};
// テクスチャの管理情報
static TextureInfo g_textureSetUpInfo;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// UninitTexture関数 - テクスチャの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitTexture(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++) {
		if (g_aTexture[nCntTex] != NULL) {
			g_aTexture[nCntTex]->Release();
			g_aTexture[nCntTex] = NULL;
		}
	}
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// LoadTexture関数 - テクスチャの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
int LoadTexture(char *pTexturePath)
{
	if (!strcmp(pTexturePath, ""))
	{// テクスチャパスが何もない時、
		return -1;
	}

	// 既に読み込まれているテクスチャか確認
	for (int nCntTex = 0; nCntTex < g_textureSetUpInfo.nTextureNum; nCntTex++) 
	{
		if (!strcmp(pTexturePath, g_textureSetUpInfo.aLoadTexturePath[nCntTex])) 
		{// 既に同じパスのテクスチャを読み込んでいた時、
			return nCntTex;	// テクスチャ番号を返す
		}
	}

	if (g_textureSetUpInfo.nTextureNum == TEXTURE_MAX) 
	{// 既にテクスチャの数が最大に達していた時、
		assert(false);
		return 0;
	}

	// テクスチャのパスを保存
	sprintf(g_textureSetUpInfo.aLoadTexturePath[g_textureSetUpInfo.nTextureNum], "%s", pTexturePath);

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pTexturePath, &g_aTexture[g_textureSetUpInfo.nTextureNum]);

	// テクスチャ番号を返す
	return g_textureSetUpInfo.nTextureNum++;
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================

//========================================
// SetTexture関数 - テクスチャの設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetTexture(LPDIRECT3DDEVICE9 pDevice, int nTex) 
{
	if (nTex == -1) 
	{// テクスチャ番号が-1の時、
		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
		return;	// 処理を終了する
	}

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTexture[nTex]);
}