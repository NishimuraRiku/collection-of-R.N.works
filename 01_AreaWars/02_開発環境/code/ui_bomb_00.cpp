//========================================
// 
// ボム[00]の処理
// Author:西村 吏功
// 
//========================================
// *** ui_bomb_00.cpp ***
//========================================
#include "main.h"
#include "chr_fighter_00.h"
#include "ui_bomb_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// マクロ定義
//****************************************
#define UI_BOMB_00_MAX						(64)											// ボム[00] の最大表示数
#define UI_BOMB_00_START_POS				(D3DXVECTOR3(PIXEL * 8 * 32, PIXEL * 20, 0.0f))	// ボム[00] の開始位置
#define UI_BOMB_00_TEXTURE_X_PATTERN_MAX	(1)												// ボム[00] のテクスチャのパターン数(横)
#define UI_BOMB_00_TEXTURE_Y_PATTERN_MAX	(1)												// ボム[00] のテクスチャのパターン数(縦)

//****************************************
// 構造体の定義
//****************************************

// ボム[00]構造体の定義
typedef struct
{
							//========== *** 位置関連 ***
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
							//========== *** テクスチャ関連 ***
	int nCounterAnim;		// アニメーションのカウンター
	int nAnimChangeTime;	// アニメーションの切り替え時間
	int nPattern;			// パターンNo.
}Ui_bomb_00;

// ボム[00] の種類毎の情報
typedef struct
{
	char aTexturPath[TXT_MAX];	// テクスチャの相対パス
	int nPtnMaxX;				// パターンの最大数(横)
	int nPtnMaxY;				// パターンの最大数(縦)
	float fWidth;				// 幅
	float fHeight;				// 高さ
	Color col;
}Ui_bomb_00Type;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureUi_bomb_00 = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_bomb_00 = NULL;	// 頂点バッファへのポインタ
Ui_bomb_00 g_aUi_bomb_00[UI_BOMB_00_MAX];				// ボム[00] の情報

// テクスチャの相対パス
Ui_bomb_00Type g_ui_bomb_00Type =
{
	"data\\TEXTURE\\ui_bomb_000_02.png", 1, 1, PIXEL * 8	, PIXEL * 8, {255,255,255,255}
};

//========================================
// InitUi_bomb_00関数 - ボム[00]の初期化処理 -
//========================================
void InitUi_bomb_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, g_ui_bomb_00Type.aTexturPath, &g_pTextureUi_bomb_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_BOMB_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_bomb_00,
		NULL);

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffUi_bomb_00->Lock(0, 0, (void**)&pVtx, 0);

	float fX = UI_BOMB_00_START_POS.x;	// X座標

	for (int nCntUi_bomb_00 = 0; nCntUi_bomb_00 < UI_BOMB_00_MAX; nCntUi_bomb_00++, pVtx += 4)
	{
		{
			// 幅と高さを設定
			g_aUi_bomb_00[nCntUi_bomb_00].fWidth	= g_ui_bomb_00Type.fWidth;
			g_aUi_bomb_00[nCntUi_bomb_00].fHeight	= g_ui_bomb_00Type.fHeight;

			D3DXVECTOR3 pos = g_aUi_bomb_00[nCntUi_bomb_00].pos;						// 位置
			D3DXVECTOR3 rot = g_aUi_bomb_00[nCntUi_bomb_00].rot;						// 向き
			float fWidth	= g_aUi_bomb_00[nCntUi_bomb_00].fWidth;						// 幅
			float fHeight	= g_aUi_bomb_00[nCntUi_bomb_00].fHeight;					// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

			// X座標を更新
			fX += fWidth * 0.5f;

			// 位置を設定
			g_aUi_bomb_00[nCntUi_bomb_00].pos = UI_BOMB_00_START_POS;
			g_aUi_bomb_00[nCntUi_bomb_00].pos.x = fX;

			// X座標を更新
			fX += fWidth * 0.5f;

			// 頂点座標 を設定
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);
		pVtx[1].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);
		pVtx[2].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);
		pVtx[3].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);

		{
			int nPtn		= g_aUi_bomb_00[nCntUi_bomb_00].nPattern;	// テクスチャのパターンNo.
			int nPtnMaxX	= g_ui_bomb_00Type.nPtnMaxX;				// テクスチャのパターン数(横)
			int nPtnMaxY	= g_ui_bomb_00Type.nPtnMaxY;				// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}
	
	// 頂点座標 をアンロックする
	g_pVtxBuffUi_bomb_00->Unlock();
}

//========================================
// UninitUi_bomb_00関数 - ボム[00]の終了処理 -
//========================================
void UninitUi_bomb_00(void)
{
	// テクスチャの破棄
	if (g_pTextureUi_bomb_00 != NULL)
	{
		g_pTextureUi_bomb_00->Release();
		g_pTextureUi_bomb_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_bomb_00 != NULL) 
	{
		g_pVtxBuffUi_bomb_00->Release();
		g_pVtxBuffUi_bomb_00 = NULL;
	}
}

//========================================
// UpdateUi_bomb_00関数 - ボム[00]の更新処理 -
//========================================
void UpdateUi_bomb_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	POINT vertexPos[4];			// 四角形の頂点の座標
	
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffUi_bomb_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_bomb_00 = 0; nCntUi_bomb_00 < UI_BOMB_00_MAX; nCntUi_bomb_00++, pVtx += 4)
	{
		D3DXVECTOR3 pos		= g_aUi_bomb_00[nCntUi_bomb_00].pos;		// 位置
		D3DXVECTOR3 rot		= g_aUi_bomb_00[nCntUi_bomb_00].rot;		// 向き
		int nPtn			= g_aUi_bomb_00[nCntUi_bomb_00].nPattern;	// テクスチャのパターンNo.
		float fWidth		= g_aUi_bomb_00[nCntUi_bomb_00].fWidth;		// 幅
		float fHeight		= g_aUi_bomb_00[nCntUi_bomb_00].fHeight;	// 高さ
		int nPtnMaxX		= g_ui_bomb_00Type.nPtnMaxX;				// テクスチャのパターン数(横)
		int nPtnMaxY		= g_ui_bomb_00Type.nPtnMaxY;				// テクスチャのパターン数(縦)

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));

		{
			float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle = atan2f(fWidth, fHeight);									// 対角線の角度

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;

			// テクスチャのパターンNo.を適用
			g_aUi_bomb_00[nCntUi_bomb_00].nPattern = nPtn;
		}
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffUi_bomb_00->Unlock();
}

//========================================
// DrawUi_bomb_00関数 - ボム[00]の描画処理 -
//========================================
void DrawUi_bomb_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_bomb_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_bomb_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_bomb_00 = 0; nCntUi_bomb_00 < GetChr_fighter_00()->nBomb; nCntUi_bomb_00++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureUi_bomb_00);

		// ボム[00]の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_bomb_00 * 4, 2);
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffUi_bomb_00->Unlock();
}