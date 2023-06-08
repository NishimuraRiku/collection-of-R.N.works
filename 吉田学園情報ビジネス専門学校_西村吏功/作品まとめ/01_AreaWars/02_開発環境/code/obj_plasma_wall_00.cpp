//========================================
// 
// プラズマウォール[00]の処理
// Author:西村 吏功
// 
//========================================
// *** obj_plasma_wall_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "sound.h"
#include "md_game_00.h"
#include "chr_fighter_00.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// マクロ定義
//****************************************
#define OBJ_PLASMA_WALL_00_HEIGHT					(PIXEL * 32)						// プラズマウォール[00] の高さ
#define OBJ_PLASMA_WALL_00_UV_MOVE					(0.015f)							// プラズマウォール[00] のUVの移動量
#define OBJ_PLASMA_WALL_00_TEXTURE_PATH				"data\\TEXTURE\\eff_plasma_000.png"	// プラズマウォール[00] のテクスチャファイルの相対パス
#define OBJ_PLASMA_WALL_00_TEXTURE_X_PATTERN_MAX	(4)									// プラズマウォール[00] のテクスチャのパターン数(横)
#define OBJ_PLASMA_WALL_00_TEXTURE_Y_PATTERN_MAX	(1)									// プラズマウォール[00] のテクスチャのパターン数(縦)
#define OBJ_PLASMA_WALL_00_ANIM_CHANGE_TIME			(6)									// プラズマウォール[00] のアニメーションの切り替え時間
#define OBJ_PLASMA_WALL_00_ALPHA_MAX				(255)								// プラズマウォール[00] の不透明度の上限値
#define OBJ_PLASMA_WALL_00_ALPHA_MIN				(70)								// プラズマウォール[00] の不透明度の下限値
#define OBJ_PLASMA_WALL_00_ALPHA_CHANGE				(2)									// プラズマウォール[00] の変動値
#define OBJ_PLASMA_WALL_00_DAMAGE					(1.0f)								// プラズマウォール[00] のダメージ
#define OBJ_PLASMA_WALL_00_REBOUND					(0.0f)								// プラズマウォール[00] の反発力

//****************************************
// プロトタイプ宣言
//****************************************
void SetPosition	(void);	// プラズマブロック[00] の座標に応じた位置/高さ/角度を取得

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureObj_plasma_wall_00 = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObj_plasma_wall_00 = NULL;	// 頂点バッファへのポインタ
Obj_plasma_wall_00 g_aObj_plasma_wall_00[4];					// プラズマウォール[00] の情報
int g_nAlpha;													// 不透明度

//========== *** プラズマウォール[00] の情報を取得 ***
Obj_plasma_wall_00 *GetObj_plasma_wall_00(void)
{
	return g_aObj_plasma_wall_00;
}

//========================================
// InitObj_plasma_wall_00関数 - プラズマウォール[00]の初期化処理 -
//========================================
void InitObj_plasma_wall_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 不透明度を初期化
	int g_nAlpha = OBJ_PLASMA_WALL_00_ALPHA_MAX;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, OBJ_PLASMA_WALL_00_TEXTURE_PATH, &g_pTextureObj_plasma_wall_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * OBJ_PLASMA_WALL_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObj_plasma_wall_00,
		NULL);

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffObj_plasma_wall_00->Lock(0, 0, (void**)&pVtx, 0);

	// プラズマブロック[00] の座標に応じた位置/高さ/角度を取得
	SetPosition();

	for (int nCntObj_plasma_wall_00 = 0; nCntObj_plasma_wall_00 < OBJ_PLASMA_WALL_00_MAX; nCntObj_plasma_wall_00++, pVtx += 4)
	{
		
		Obj_plasma_wall_00 *pObj = &g_aObj_plasma_wall_00[nCntObj_plasma_wall_00];	// プラズマウォール

		{
			float fHeight = pObj->fHeight;	// 高さ

			{
				D3DXVECTOR3 drawPos = pObj->pos;											// 描画位置
				float fWidth	= OBJ_PLASMA_WALL_00_WIDTH;									// 幅
				float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
				float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

				// 頂点座標 を設定
				pVtx[0].pos.x = drawPos.x + sinf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
				pVtx[0].pos.y = drawPos.y + cosf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
				pVtx[1].pos.x = drawPos.x + sinf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
				pVtx[1].pos.y = drawPos.y + cosf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
				pVtx[2].pos.x = drawPos.x + sinf(pObj->rot.z - fAngle) * fLength;
				pVtx[2].pos.y = drawPos.y + cosf(pObj->rot.z - fAngle) * fLength;
				pVtx[3].pos.x = drawPos.x + sinf(pObj->rot.z + fAngle) * fLength;
				pVtx[3].pos.y = drawPos.y + cosf(pObj->rot.z + fAngle) * fLength;
			}

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			{
				int nPtn		= pObj->nPattern;							// テクスチャのパターンNo.
				int nPtnMaxX	= OBJ_PLASMA_WALL_00_TEXTURE_X_PATTERN_MAX;	// テクスチャのパターン数(横)
				int nPtnMaxY	= OBJ_PLASMA_WALL_00_TEXTURE_Y_PATTERN_MAX;	// テクスチャのパターン数(縦)
				D3DXVECTOR3 tex = pObj->tex;								// テクスチャ座標の開始位置(UV値)

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y);
				pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y);
				pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));
				pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));
			}
		}
	}
	
	// 頂点座標 をアンロックする
	g_pVtxBuffObj_plasma_wall_00->Unlock();
}

//========================================
// UninitObj_plasma_wall_00関数 - プラズマウォール[00]の終了処理 -
//========================================
void UninitObj_plasma_wall_00(void)
{
	// テクスチャの破棄
	if (g_pTextureObj_plasma_wall_00 != NULL)
	{
		g_pTextureObj_plasma_wall_00->Release();
		g_pTextureObj_plasma_wall_00 = NULL;
	}
	
	// 頂点バッファの破棄
	if (g_pVtxBuffObj_plasma_wall_00 != NULL) 
	{
		g_pVtxBuffObj_plasma_wall_00->Release();
		g_pVtxBuffObj_plasma_wall_00 = NULL;
	}
}

//========================================
// UpdateObj_plasma_wall_00関数 - プラズマウォール[00]の更新処理 -
//========================================
void UpdateObj_plasma_wall_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffObj_plasma_wall_00->Lock(0, 0, (void**)&pVtx, 0);

	// プラズマブロック[00] の座標に応じた高さ/角度を取得
	SetPosition();

	for (int nCntObj_plasma_wall_00 = 0; nCntObj_plasma_wall_00 < OBJ_PLASMA_WALL_00_MAX; nCntObj_plasma_wall_00++, pVtx += 4)
	{
		Obj_plasma_wall_00 *pObj = &g_aObj_plasma_wall_00[nCntObj_plasma_wall_00];	// プラズマウォール[00] の情報のポインタ
		
		{
			float fHeight = pObj->fHeight;	// 高さ

			{
				int nAnimChangeTime = OBJ_PLASMA_WALL_00_ANIM_CHANGE_TIME;		// アニメーションの切り替え時間
				int nPtn			= pObj->nPattern;							// テクスチャのパターンNo.
				int nPtnMaxX		= OBJ_PLASMA_WALL_00_TEXTURE_X_PATTERN_MAX;	// テクスチャのパターン数(横)
				int nPtnMaxY		= OBJ_PLASMA_WALL_00_TEXTURE_Y_PATTERN_MAX;	// テクスチャのパターン数(縦)
				D3DXVECTOR3 tex		= pObj->tex;								// テクスチャ座標の開始位置(UV値)

				// アニメーションのカウンターを加算
				pObj->nCounterAnim++;

				if (pObj->nCounterAnim >= nAnimChangeTime)
				{// アニメーションのカウンターがアニメーションにかかる時間に達した時、
					// アニメーションのカウンターを初期化
					pObj->nCounterAnim = 0;

					// パターンNo.を更新
					nPtn = ++nPtn % (nPtnMaxX * nPtnMaxY);
				}

				// V座標を更新
				tex.y += OBJ_PLASMA_WALL_00_UV_MOVE;

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y);
				pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y);
				pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));
				pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));

				// パターンNo.を適用
				pObj->nPattern = nPtn;

				// テクスチャ座標の開始位置(UV値)を適用
				pObj->tex = tex;
			}

			{
				D3DXVECTOR3 drawPos = pObj->pos;												// 描画位置
				float fWidth		= OBJ_PLASMA_WALL_00_WIDTH;									// 幅
				float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
				float fAngle		= atan2f(fWidth, fHeight);									// 対角線の角度

				// 描画位置を設定
				MatchCameraPosition(&drawPos);

				// 頂点座標を設定
				pVtx[0].pos.x = drawPos.x + sinf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
				pVtx[0].pos.y = drawPos.y + cosf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
				pVtx[1].pos.x = drawPos.x + sinf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
				pVtx[1].pos.y = drawPos.y + cosf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
				pVtx[2].pos.x = drawPos.x + sinf(pObj->rot.z - fAngle) * fLength;
				pVtx[2].pos.y = drawPos.y + cosf(pObj->rot.z - fAngle) * fLength;
				pVtx[3].pos.x = drawPos.x + sinf(pObj->rot.z + fAngle) * fLength;
				pVtx[3].pos.y = drawPos.y + cosf(pObj->rot.z + fAngle) * fLength;
			}
		}

		{
			// 不透明度を変動させる
			g_nAlpha += OBJ_PLASMA_WALL_00_ALPHA_CHANGE;

			int nAlpha;										// 不透明度
			int nAlphaMax = OBJ_PLASMA_WALL_00_ALPHA_MAX;	// 不透明度の上限値
			int nAlphaMin = OBJ_PLASMA_WALL_00_ALPHA_MIN;	// 不透明度の下限値

			if ((g_nAlpha / (nAlphaMax - nAlphaMin)) % 2 == 0)
			{// 不透明度を上限で除算した結果が偶数だった場合、
				// 不透明度を上限で剰余算する
				nAlpha = nAlphaMin + (g_nAlpha % (nAlphaMax - nAlphaMin));
			}
			else
			{// 不透明度を上限で除算した結果が奇数だった場合、
				// 上限から不透明度を上限で剰余算した結果を減算する
				nAlpha = nAlphaMin + ((nAlphaMax - nAlphaMin) - (g_nAlpha % (nAlphaMax - nAlphaMin)));
			}

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
		}
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffObj_plasma_wall_00->Unlock();
}

//========================================
// DrawObj_plasma_wall_00関数 - プラズマウォール[00]の描画処理 -
//========================================
void DrawObj_plasma_wall_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffObj_plasma_wall_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureObj_plasma_wall_00);

	for (int nCntObj_plasma_wall_00 = 0; nCntObj_plasma_wall_00 < OBJ_PLASMA_WALL_00_MAX; nCntObj_plasma_wall_00++)
	{
		// プラズマウォール[00]の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntObj_plasma_wall_00 * 4, 2);
	}
}

//========================================
// SetPosition関数 - プラズマブロック[00] の座標に応じた位置/高さ/角度を取得 -
//========================================
void SetPosition(void)
{
	Obj_plasma_block_00_square *pObj_plasma_block_00_square = GetObj_plasma_block_00_square();	// プラズマブロック[00] の情報

	float fTop		= pObj_plasma_block_00_square->fTop;	// 上辺の座標
	float fBottom	= pObj_plasma_block_00_square->fBottom;	// 下辺の座標
	float fLeft		= pObj_plasma_block_00_square->fLeft;	// 左辺の座標
	float fRight	= pObj_plasma_block_00_square->fRight;	// 右辺の座標

	// 座標を更新
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_TOP]	.pos = D3DXVECTOR3(fLeft + ((fRight - fLeft) * 0.5f), fTop								, 0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_BOTTOM].pos = D3DXVECTOR3(fLeft + ((fRight - fLeft) * 0.5f), fBottom							, 0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_LEFT]	.pos = D3DXVECTOR3(fLeft							, fTop + ((fBottom - fTop) * 0.5f)	, 0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_RIGHT]	.pos = D3DXVECTOR3(fRight							, fTop + ((fBottom - fTop) * 0.5f)	, 0.0f);

	// 高さを更新
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_TOP]	.fHeight = fRight - fLeft;
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_BOTTOM].fHeight = fRight - fLeft;
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_LEFT]	.fHeight = fBottom - fTop;
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_RIGHT]	.fHeight = fBottom - fTop;

	// 向きを更新
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_TOP]	.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -	0.5f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_BOTTOM].rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI *	0.5f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_LEFT]	.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI *	0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_RIGHT]	.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI *	1.0f);
}