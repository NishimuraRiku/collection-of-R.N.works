//========================================
// 
// BG:山[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_mountain_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon3D.h"
#include "sound.h"
#include "bg_mountain_00.h"	// BG :山		[00]
#include "md_game_00.h"		// MD :ゲーム	[00]
#include "obj_stage_00.h"	// OBJ:ステージ	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// BG:山[00] のテクスチャの相対パス
#define BG_MOUNTAIN_00_TEXTURE_PATH	"data\\TEXTURE\\Background\\bg_mountain_000.png"

// BG:山[00] の半径
// BG:山[00] の高さ
// BG:山[00] の分割幅
// BG:山[00] の分割高さ
// BG:山[00] の位置
// BG:山[00] の法線ベクトル
#define BG_MOUNTAIN_00_HEIGHT			(256.0f)
#define BG_MOUNTAIN_00_RADIUS			(400.0f)
#define BG_MOUNTAIN_00_DIVISION_WIDTH	(16)
#define BG_MOUNTAIN_00_DIVISION_HEIGHT	(1)
#define BG_MOUNTAIN_00_POS				(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define BG_MOUNTAIN_00_NOR				(D3DXVECTOR3(0.0f,0.0f,1.0f))

//****************************************
// 構造体の定義
//****************************************
// BG:山[00] の情報構造体
typedef struct
{
	D3DXVECTOR3 rot;	// 向き
}Bg_mountain_00;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureBg_mountain_00	// BG:山[00] のテクスチャへのポインタ
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_mountain_00	// BG:山[00] の頂点バッファへのポインタ
						= NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBg_mountain_00	// BG:山[00] のインデックスバッファのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldBg_mountain_00;	// BG:山[00] のワールドマトリックス
Bg_mountain_00			g_bg_mountain_00;			// BG:山[00] の情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_mountain_00関数 - BG:山[00] のパラメーターの初期化処理 -
//========================================
void InitParameterBg_mountain_00(void) 
{
	Bg_mountain_00		*pBg				// BG:山[00] の情報のポインタ
						= &g_bg_mountain_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitBg_mountain_00関数 - BG:山[00] の初期化処理 -
//========================================
void InitBg_mountain_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_MOUNTAIN_00_TEXTURE_PATH, &g_pTextureBg_mountain_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (BG_MOUNTAIN_00_DIVISION_HEIGHT + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_mountain_00,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_mountain_00->Lock(0, 0, (void**)&pVtx, 0);

	// パラメーターの初期化処理
	InitParameterBg_mountain_00();
	
	// 頂点座標/法線ベクトル/テクスチャ座標の設定
	for (int nCntHeight = 0; nCntHeight < BG_MOUNTAIN_00_DIVISION_HEIGHT + 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_MOUNTAIN_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			pVtx[(nCntHeight * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))) * BG_MOUNTAIN_00_RADIUS,
					BG_MOUNTAIN_00_HEIGHT - (nCntHeight * (BG_MOUNTAIN_00_HEIGHT / BG_MOUNTAIN_00_DIVISION_HEIGHT)),
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))) * BG_MOUNTAIN_00_RADIUS);

			pVtx[(nCntHeight * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + nCntWidth].nor 
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))),
					0.0f,
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))));;

			pVtx[(nCntHeight * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + nCntWidth].tex 
				= D3DXVECTOR2(
					nCntWidth * (1.0f / (float)BG_MOUNTAIN_00_DIVISION_WIDTH),
					nCntHeight * (1.0f / (float)BG_MOUNTAIN_00_DIVISION_HEIGHT));
		}
	}

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((BG_MOUNTAIN_00_DIVISION_HEIGHT * 2 * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + ((BG_MOUNTAIN_00_DIVISION_HEIGHT - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBg_mountain_00,
		NULL);

	WORD *pIdx;	// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffBg_mountain_00->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点番号データの設定
	int nCntIdx = 0;	// インデックスのカウント

	for (int nCntDepth = 0; nCntDepth < BG_MOUNTAIN_00_DIVISION_HEIGHT; nCntDepth++) 
	{
		for (int nCntWidth = 0; nCntWidth < BG_MOUNTAIN_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (nCntDepth + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * nCntDepth);
			nCntIdx += 2;
		}

		if (nCntDepth < BG_MOUNTAIN_00_DIVISION_HEIGHT - 1)
		{
			pIdx[nCntIdx] = ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (nCntDepth + 1)) - 1;
			pIdx[nCntIdx + 1] = (BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (nCntDepth + 2);
			nCntIdx += 2;
		}
	}
	
	// インデックスバッファをアンロックする
	g_pIdxBuffBg_mountain_00->Unlock();

	for (int nCntVtx = 0; nCntVtx < ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (BG_MOUNTAIN_00_DIVISION_HEIGHT + 1)); nCntVtx++)
	{
		// 頂点カラーの設定
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBg_mountain_00->Unlock();
}

//========================================
// UninitBg_mountain_00関数 - BG:山[00] の終了処理 -
//========================================
void UninitBg_mountain_00(void)
{
	// テクスチャの破棄
	if (g_pTextureBg_mountain_00 != NULL)
	{
		g_pTextureBg_mountain_00->Release();
		g_pTextureBg_mountain_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg_mountain_00 != NULL)
	{
		g_pVtxBuffBg_mountain_00->Release();
		g_pVtxBuffBg_mountain_00 = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffBg_mountain_00 != NULL) 
	{
		g_pIdxBuffBg_mountain_00->Release();
		g_pIdxBuffBg_mountain_00 = NULL;
	}
}

//========================================
// UpdateBg_mountain_00関数 - BG:山[00] の更新処理 -
//========================================
void UpdateBg_mountain_00(void)
{

}

//========================================
// DrawBg_mountain_00関数 - BG:山[00] の描画処理 -
//========================================
void DrawBg_mountain_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	Bg_mountain_00		*pBg				// BG:山[00] の情報のポインタ
						= &g_bg_mountain_00;

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBg_mountain_00);
	
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetObj_stage_00()->rot.y, GetObj_stage_00()->rot.x, GetObj_stage_00()->rot.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_mountain_00, &g_mtxWorldBg_mountain_00, &mtxRot);

	{
		D3DXVECTOR3 setPos;	// 設定位置

		if (GetMode() == MODE_TITLE_00) 
		{// モードがMD:タイトル画面[00] の時、
			setPos = GetObj_stage_00()->pos;	// 設定位置にOBJ:ステージ[00] の位置を代入
		}
		if ((GetMode() == MODE_GAME_00) || (GetMode() == MODE_TUTORIAL_00))
		{// モードがMD:ゲーム画面orチュートリアル画面の時、
			setPos = GetCamera3D()->posV;	// 設定位置にカメラ(3D)の視点の位置を代入
			setPos.y = MD_GAME_00_BG_POS_Y;	// Y座標初期化
		}

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
		D3DXMatrixMultiply(&g_mtxWorldBg_mountain_00, &g_mtxWorldBg_mountain_00, &mtxTrans);
	}
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg_mountain_00);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg_mountain_00, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffBg_mountain_00);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg_mountain_00);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0, 
		0,
		((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (BG_MOUNTAIN_00_DIVISION_HEIGHT + 1)),
		0, 
		((BG_MOUNTAIN_00_DIVISION_WIDTH * BG_MOUNTAIN_00_DIVISION_HEIGHT) * 2) + (4 * (BG_MOUNTAIN_00_DIVISION_HEIGHT - 1)));

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}