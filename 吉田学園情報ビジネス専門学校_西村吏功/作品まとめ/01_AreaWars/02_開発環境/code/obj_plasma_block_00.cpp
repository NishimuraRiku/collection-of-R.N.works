//========================================
// 
// プラズマブロック[00]の処理
// Author:西村 吏功
// 
//========================================
// *** obj_plasma_block_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "obj_plasma_block_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// マクロ定義
//****************************************
#define OBJ_PLASMA_BLOCK_00_NUM						(4)											// プラズマブロック[00] の数
#define OBJ_PLASMA_BLOCK_00_WIDTH					(PIXEL * 16)								// プラズマブロック[00] の幅
#define OBJ_PLASMA_BLOCK_00_HEIGHT					(PIXEL * 16)								// プラズマブロック[00] の高さ
#define OBJ_PLASMA_BLOCK_00_TEXTURE_PATH			"data\\TEXTURE\\obj_plasma_block_000.png"	// プラズマブロック[00] のテクスチャファイルの相対パス
#define OBJ_PLASMA_BLOCK_00_TEXTURE_X_PATTERN_MAX	(4)											// プラズマブロック[00] のテクスチャのパターン数(横)
#define OBJ_PLASMA_BLOCK_00_TEXTURE_Y_PATTERN_MAX	(1)											// プラズマブロック[00] のテクスチャのパターン数(縦)
#define OBJ_PLASMA_BLOCK_00_ANIM_CHANGE_TIME		(8)											// プラズマブロック[00] のアニメーションの切り替え時間(縦)
#define OBJ_PLASMA_BLOCK_00_SQUARE_WIDTH			(80.0f)										// プラズマブロック[00] が描く四角形の幅(初期)
#define OBJ_PLASMA_BLOCK_00_SQUARE_HEIGHT			(80.0f)										// プラズマブロック[00] が描く四角形の高さ(初期)

//****************************************
// 構造体の定義
//****************************************

// プラズマブロック[00]構造体の定義
typedef struct
{
							//========== *** 位置関連 ***
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 向き
							//========== *** テクスチャ関連 ***
	int nCounterAnim;		// アニメーションのカウンター
	int nAnimChangeTime;	// アニメーションの切り替え時間
	int nPattern;			// パターンNo.
}Obj_plasma_block_00;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureObj_plasma_block_00 = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObj_plasma_block_00 = NULL;			// 頂点バッファへのポインタ
Obj_plasma_block_00 g_aObj_plasma_block_00[OBJ_PLASMA_BLOCK_00_NUM];	// プラズマブロック[00] の情報
Obj_plasma_block_00_square g_square;									// プラズマブロック[00] が描く四角形の情報

//========== *** プラズマブロック[00] が描く四角形の情報を取得 ***
Obj_plasma_block_00_square *GetObj_plasma_block_00_square(void) 
{
	return &g_square;
}

//========================================
// InitObj_plasma_block_00関数 - プラズマブロック[00]の初期化処理 -
//========================================
void InitObj_plasma_block_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	g_square.nMoveCounter = OBJ_PLASMA_BLOCK_00_MOVE_TIME;	// 移動のカウンターを初期化

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, OBJ_PLASMA_BLOCK_00_TEXTURE_PATH, &g_pTextureObj_plasma_block_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * OBJ_PLASMA_BLOCK_00_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObj_plasma_block_00,
		NULL);

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffObj_plasma_block_00->Lock(0, 0, (void**)&pVtx, 0);

	//四角形の辺の座標を設定
	g_square.fTop		= -OBJ_PLASMA_BLOCK_00_SQUARE_HEIGHT	* 0.5f;
	g_square.fBottom	= OBJ_PLASMA_BLOCK_00_SQUARE_HEIGHT		* 0.5f;
	g_square.fLeft		= -OBJ_PLASMA_BLOCK_00_SQUARE_WIDTH		* 0.5f;
	g_square.fRight		= OBJ_PLASMA_BLOCK_00_SQUARE_WIDTH		* 0.5f;

	// 座標を設定
	g_aObj_plasma_block_00[0].pos = D3DXVECTOR3(g_square.fLeft	, g_square.fTop		, 0.0f);
	g_aObj_plasma_block_00[1].pos = D3DXVECTOR3(g_square.fRight	, g_square.fTop		, 0.0f);
	g_aObj_plasma_block_00[2].pos = D3DXVECTOR3(g_square.fLeft	, g_square.fBottom	, 0.0f);
	g_aObj_plasma_block_00[3].pos = D3DXVECTOR3(g_square.fRight	, g_square.fBottom	, 0.0f);

	for (int nCntObj_plasma_block_00 = 0; nCntObj_plasma_block_00 < OBJ_PLASMA_BLOCK_00_NUM; nCntObj_plasma_block_00++, pVtx += 4)
	{
		{
			D3DXVECTOR3 pos = g_aObj_plasma_block_00[nCntObj_plasma_block_00].pos;		// 位置
			D3DXVECTOR3 rot = g_aObj_plasma_block_00[nCntObj_plasma_block_00].rot;		// 向き
			float fWidth	= OBJ_PLASMA_BLOCK_00_WIDTH;								// 幅
			float fHeight	= OBJ_PLASMA_BLOCK_00_HEIGHT;								// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		{
			int nPtn		= g_aObj_plasma_block_00[nCntObj_plasma_block_00].nPattern;	// テクスチャのパターンNo.
			int nPtnMaxX	= OBJ_PLASMA_BLOCK_00_TEXTURE_X_PATTERN_MAX;				// テクスチャのパターン数(横)
			int nPtnMaxY	= OBJ_PLASMA_BLOCK_00_TEXTURE_Y_PATTERN_MAX;				// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}
	
	// 頂点座標 をアンロックする
	g_pVtxBuffObj_plasma_block_00->Unlock();
}

//========================================
// UninitObj_plasma_block_00関数 - プラズマブロック[00]の終了処理 -
//========================================
void UninitObj_plasma_block_00(void)
{
	// テクスチャの破棄
	if (g_pTextureObj_plasma_block_00 != NULL)
	{
		g_pTextureObj_plasma_block_00->Release();
		g_pTextureObj_plasma_block_00 = NULL;
	}
	
	// 頂点バッファの破棄
	if (g_pVtxBuffObj_plasma_block_00 != NULL) 
	{
		g_pVtxBuffObj_plasma_block_00->Release();
		g_pVtxBuffObj_plasma_block_00 = NULL;
	}
}

//========================================
// UpdateObj_plasma_block_00関数 - プラズマブロック[00]の更新処理 -
//========================================
void UpdateObj_plasma_block_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 座標を設定
	g_aObj_plasma_block_00[0].pos = D3DXVECTOR3(g_square.fLeft, g_square.fTop, 0.0f);
	g_aObj_plasma_block_00[1].pos = D3DXVECTOR3(g_square.fRight, g_square.fTop, 0.0f);
	g_aObj_plasma_block_00[2].pos = D3DXVECTOR3(g_square.fLeft, g_square.fBottom, 0.0f);
	g_aObj_plasma_block_00[3].pos = D3DXVECTOR3(g_square.fRight, g_square.fBottom, 0.0f);
	
	// 中心位置を設定
	g_square.centerPos.x = (g_square.fTargetLeft + g_square.fTargetRight) * 0.5f;
	g_square.centerPos.y = (g_square.fTargetTop + g_square.fTargetBottom) * 0.5f;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffObj_plasma_block_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntObj_plasma_block_00 = 0; nCntObj_plasma_block_00 < OBJ_PLASMA_BLOCK_00_NUM; nCntObj_plasma_block_00++, pVtx += 4)
	{
		Obj_plasma_block_00 *pObj = &g_aObj_plasma_block_00[nCntObj_plasma_block_00];	// プラズマブロック[00] の情報のポインタ

		float fWidth	= OBJ_PLASMA_BLOCK_00_WIDTH;	// 幅
		float fHeight	= OBJ_PLASMA_BLOCK_00_HEIGHT;	// 高さ

		{
			int nAnimChangeTime = OBJ_PLASMA_BLOCK_00_ANIM_CHANGE_TIME;			// アニメーションの切り替え時間
			int nPtnMaxX		= OBJ_PLASMA_BLOCK_00_TEXTURE_X_PATTERN_MAX;	// テクスチャのパターン数(横)
			int nPtnMaxY		= OBJ_PLASMA_BLOCK_00_TEXTURE_Y_PATTERN_MAX;	// テクスチャのパターン数(縦)

			// アニメーションのカウンターを加算
			pObj->nCounterAnim++;

			if (pObj->nCounterAnim >= nAnimChangeTime)
			{// アニメーションのカウンターがアニメーションにかかる時間に達した時

				// アニメーションのカウンターを初期化
				pObj->nCounterAnim = 0;

				// パターンNo.を更新
				pObj->nPattern = ++pObj->nPattern % (nPtnMaxX * nPtnMaxY);
			}

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}

		{
			D3DXVECTOR3 drawPos = pObj->pos;										// 描画位置
			float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle = atan2f(fWidth, fHeight);									// 対角線の角度

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

	// 頂点座標 をアンロックする
	g_pVtxBuffObj_plasma_block_00->Unlock();
}

//========================================
// UpdateObj_plasma_block_00_square関数 - プラズマブロック[00] が描く四角形の更新処理 -
//========================================
void UpdateObj_plasma_block_00_square(void)
{
	if (g_square.nMoveCounter > 0)
	{// 移動のカウンターが0を上回っている時、
		// 移動のカウンターを減算
		g_square.nMoveCounter--;

		if (g_square.nMoveCounter > 0)
		{// 移動のカウンターが0を上回っている時、
			// 各辺の位置を更新
			g_square.fTop		+= (g_square.fMoveTop / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
			g_square.fBottom	+= (g_square.fMoveBottom / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
			g_square.fLeft		+= (g_square.fMoveLeft / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
			g_square.fRight		+= (g_square.fMoveRight / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
		}
		else
		{// 移動のカウンターが0以下の時、
			// 各辺の位置を目標位置に設定
			g_square.fTop = g_square.fTargetTop;
			g_square.fBottom = g_square.fTargetBottom;
			g_square.fLeft = g_square.fTargetLeft;
			g_square.fRight = g_square.fTargetRight;
		}
	}
}

//========================================
// DrawObj_plasma_block_00関数 - プラズマブロック[00]の描画処理 -
//========================================
void DrawObj_plasma_block_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffObj_plasma_block_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureObj_plasma_block_00);

	for (int nCntObj_plasma_block_00 = 0; nCntObj_plasma_block_00 < OBJ_PLASMA_BLOCK_00_NUM; nCntObj_plasma_block_00++)
	{
		// プラズマブロック[00]の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntObj_plasma_block_00 * 4, 2);
	}
}

//========================================
// SetMoveObj_plasma_block_00関数 - プラズマブロック[00] の四角形の移動量を設定 -
//========================================
void SetMoveObj_plasma_block_00(void) 
{
	Obj_plasma_block_00_square	*pObj	// プラズマブロック[00] の描く四角形の情報
								= GetObj_plasma_block_00_square();

	// 移動のカウンターを設定
	pObj->nMoveCounter = OBJ_PLASMA_BLOCK_00_MOVE_TIME;

	// 四角形の各辺の移動量を設定
	pObj->fMoveTop		= (pObj->fTargetTop - pObj->fTop);
	pObj->fMoveBottom	= (pObj->fTargetBottom - pObj->fBottom);
	pObj->fMoveLeft		= (pObj->fTargetLeft - pObj->fLeft);
	pObj->fMoveRight	= (pObj->fTargetRight - pObj->fRight);
}