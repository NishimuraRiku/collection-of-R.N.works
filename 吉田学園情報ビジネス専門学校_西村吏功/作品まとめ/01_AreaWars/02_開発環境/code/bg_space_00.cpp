//========================================
// 
// 宇宙[00]の処理
// Author:西村 吏功
// 
//========================================
//  *** bg_space_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "bg_space_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//========== マクロ定義			==========// 
#define BG_SPACE_00_NUM				(4)										// 宇宙[00] の数
#define BG_SPACE_00_UV_MOVE_0		(0.025f)								// 宇宙[00] のUVの移動量 [0]
#define BG_SPACE_00_UV_MOVE_1		(0.2f)									// 宇宙[00] のUVの移動量 [1]
#define BG_SPACE_00_UV_MOVE_2		(0.4f)									// 宇宙[00] のUVの移動量 [2]
#define BG_SPACE_00_UV_MOVE_3		(0.6f)									// 宇宙[00] のUVの移動量 [3]
#define BG_SPACE_00_TEXTURE_PATH_0	"data\\TEXTURE\\bg_space_000_03.png"	// 宇宙[00] のテクスチャファイルの相対パス [0]
#define BG_SPACE_00_TEXTURE_PATH_1	"data\\TEXTURE\\bg_space_000_02.png"	// 宇宙[00] のテクスチャファイルの相対パス [1]
#define BG_SPACE_00_TEXTURE_PATH_2	"data\\TEXTURE\\bg_space_000_01.png"	// 宇宙[00] のテクスチャファイルの相対パス [2]
#define BG_SPACE_00_TEXTURE_PATH_3	"data\\TEXTURE\\bg_space_000_00.png"	// 宇宙[00] のテクスチャファイルの相対パス [3]
#define BG_SPACE_00_WIDTH			(PIXEL * 1024)							// 宇宙[00] の幅
#define BG_SPACE_00_HEIGHT			(PIXEL * 1024)							// 宇宙[00] の高さ

//========== グローバル宣言		==========// 
LPDIRECT3DTEXTURE9 g_aTextureBg_space_00[BG_SPACE_00_NUM] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_space_00 = NULL;			// 頂点バッファへのポインタ
POINT g_aTex[BG_SPACE_00_NUM];									// テクスチャ座標の開始位置(UV値)

//========================================
// InitBg_space_00関数 - 宇宙[00]の初期化処理 -
//========================================
void InitBg_space_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	POINT vertexPos[4];			// 四角形の頂点の座標
	
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_0, &g_aTextureBg_space_00[0]);
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_1, &g_aTextureBg_space_00[1]);
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_2, &g_aTextureBg_space_00[2]);
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_3, &g_aTextureBg_space_00[3]);

	// テクスチャ座標の開始位置の初期化
	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++) 
	{
		g_aTex[nCntBg_space_00].x = 0.0f;
		g_aTex[nCntBg_space_00].y = 0.0f;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * BG_SPACE_00_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_space_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++, pVtx += 4)
	{
		{
			D3DXVECTOR3 pos	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);	// 位置
			D3DXVECTOR3 rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// 向き
			float fWidth	= BG_SPACE_00_WIDTH;											// 幅
			float fHeight	= BG_SPACE_00_HEIGHT;											// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;		// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);										// 対角線の角度

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)	) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)	) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)	) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)	) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle				) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle				) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle				) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle				) * fLength;
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

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x			, g_aTex[nCntBg_space_00].y			);
		pVtx[1].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x + 1.0f	, g_aTex[nCntBg_space_00].y			);
		pVtx[2].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x			, g_aTex[nCntBg_space_00].y + 1.0f	);
		pVtx[3].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x + 1.0f	, g_aTex[nCntBg_space_00].y + 1.0f	);
	}

	// 頂点座標をアンロックする
	g_pVtxBuffBg_space_00->Unlock();
}

//========================================
// UninitBg_space_00関数 - 宇宙[00]の終了処理 -
//========================================
void UninitBg_space_00(void)
{
	// テクスチャの破棄
	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++)
	{
		if (g_aTextureBg_space_00[nCntBg_space_00] != NULL)
		{
			g_aTextureBg_space_00[nCntBg_space_00]->Release();
			g_aTextureBg_space_00[nCntBg_space_00] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg_space_00 != NULL) 
	{
		g_pVtxBuffBg_space_00->Release();
		g_pVtxBuffBg_space_00 = NULL;
	}
}

//========================================
// UpdateBg_space_00関数 - 宇宙[00]の更新処理 -
//========================================
void UpdateBg_space_00(void)
{
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ
	Camera *pCamera;	// 機体[00]の情報

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_00->Lock(0, 0, (void**)&pVtx, 0);

	// 機体[00]の情報を取得
	pCamera = GetCamera();

	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++, pVtx += 4)
	{
		// テクスチャ座標を更新
		g_aTex[nCntBg_space_00].y = ((int)g_aTex[nCntBg_space_00].y % 100);

		float fMove;	// 移動量

		if (nCntBg_space_00 == 0) { fMove = BG_SPACE_00_UV_MOVE_0; }
		if (nCntBg_space_00 == 1) { fMove = BG_SPACE_00_UV_MOVE_1; }
		if (nCntBg_space_00 == 2) { fMove = BG_SPACE_00_UV_MOVE_2; }
		if (nCntBg_space_00 == 3) { fMove = BG_SPACE_00_UV_MOVE_3; }

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove)			, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove)));
		pVtx[1].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f	, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove)));
		pVtx[2].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove)			, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f));
		pVtx[3].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f	, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f));
	}

	// 頂点座標をアンロックする
	g_pVtxBuffBg_space_00->Unlock();
}

//========================================
// DrawBg_space_00関数 - 宇宙[00]の描画処理 -
//========================================
void DrawBg_space_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg_space_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureBg_space_00[nCntBg_space_00]);

		// 宇宙[00]の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg_space_00 * 4, 2);
	}
}