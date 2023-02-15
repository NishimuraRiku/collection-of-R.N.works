//========================================
// 
// BG:宇宙[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_space_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon3D.h"
#include "sound.h"
#include "bg_space_00.h"	// BG :宇宙		[00]
#include "md_game_00.h"		// MD :ゲーム	[00]
#include "obj_stage_00.h"	// OBJ:ステージ	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// BG:宇宙[00] のテクスチャの相対パス
#define BG_SPACE_00_TEXTURE_PATH	"data\\TEXTURE\\Background\\bg_space_000.png"

// BG:宇宙[00] の半径
// BG:宇宙[00] の高さ
// BG:宇宙[00] の分割幅
// BG:宇宙[00] の分割高さ
// BG:宇宙[00] の位置
// BG:宇宙[00] の法線ベクトル
// BG:宇宙[00] の回転量
#define BG_SPACE_00_RADIUS			(475.0f)
#define BG_SPACE_00_HEIGHT			(475.0f)
#define BG_SPACE_00_DIVISION_WIDTH	(16)
#define BG_SPACE_00_DIVISION_HEIGHT	(8)
#define BG_SPACE_00_POS				(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define BG_SPACE_00_NOR				(D3DXVECTOR3(0.0f,0.0f,1.0f))
#define BG_SPACE_00_SPIN			(D3DXVECTOR3(0.0f,0.001f,0.0f))

//****************************************
// 構造体の定義
//****************************************
// BG:宇宙[00] の情報構造体
typedef struct
{
	D3DXVECTOR3 rot;	// 向き
}Bg_space_00;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureBg_space_00		// BG:宇宙[00] のテクスチャへのポインタ
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_space_00		// BG:宇宙[00] の頂点バッファへのポインタ
						= NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBg_space_00		// BG:宇宙[00] のインデックスバッファのポインタ
						= NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBg_space_00Lid	// BG:宇宙[00] の蓋のインデックスバッファのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldBg_space_00;		// BG:宇宙[00] のワールドマトリックス
Bg_space_00				g_bg_space_00;				// BG:宇宙[00] の情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_space_00関数 - BG:宇宙[00] のパラメーターの初期化処理 -
//========================================
void InitParameterBg_space_00(void)
{
	Bg_space_00	*pBg	// BG:宇宙[00] の情報のポインタ
				= &g_bg_space_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitBg_space_00関数 - BG:宇宙[00] の初期化処理 -
//========================================
void InitBg_space_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH, &g_pTextureBg_space_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (((BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT) + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_space_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_00->Lock(0, 0, (void**)&pVtx, 0);

	// パラメーターの初期化処理
	InitParameterBg_space_00();

	// 真上の頂点座標/テクスチャ座標の設定
	pVtx[(BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT].pos = D3DXVECTOR3(0.0f, BG_SPACE_00_HEIGHT, 0.0f);
	pVtx[(BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点座標/法線ベクトル/テクスチャ座標の設定
	for (int nCntHeight = 0; nCntHeight < BG_SPACE_00_DIVISION_HEIGHT; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_SPACE_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			// 頂点座標
			pVtx[(nCntHeight * (BG_SPACE_00_DIVISION_WIDTH + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))) * (BG_SPACE_00_RADIUS * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_SPACE_00_DIVISION_HEIGHT)))),
					BG_SPACE_00_HEIGHT - (((nCntHeight + 1) * (BG_SPACE_00_HEIGHT / BG_SPACE_00_DIVISION_HEIGHT)) * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_SPACE_00_DIVISION_HEIGHT)))),
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))) * (BG_SPACE_00_RADIUS * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_SPACE_00_DIVISION_HEIGHT)))));

			// 法線ベクトル
			pVtx[(nCntHeight * (BG_SPACE_00_DIVISION_WIDTH + 1)) + nCntWidth].nor
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))),
					0.0f,
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))));;

			// テクスチャ座標
			pVtx[(nCntHeight * (BG_SPACE_00_DIVISION_WIDTH + 1)) + nCntWidth].tex
				= D3DXVECTOR2(
					nCntWidth * (1.0f / (float)BG_SPACE_00_DIVISION_WIDTH),
					(nCntHeight + 1) * (1.0f / (float)BG_SPACE_00_DIVISION_HEIGHT));
		}
	}

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((((BG_SPACE_00_DIVISION_HEIGHT - 1) * 2 * (BG_SPACE_00_DIVISION_WIDTH + 1)) + ((BG_SPACE_00_DIVISION_HEIGHT - 2) * 2)) + 1),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBg_space_00,
		NULL);

	WORD *pIdx;	// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffBg_space_00->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点番号データの設定
	int nCntIdx = 0;	// インデックスのカウント

	for (int nCntHeight = 0; nCntHeight < BG_SPACE_00_DIVISION_HEIGHT - 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_SPACE_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((BG_SPACE_00_DIVISION_WIDTH + 1) * (nCntHeight + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((BG_SPACE_00_DIVISION_WIDTH + 1) * nCntHeight);
			nCntIdx += 2;
		}

		if (nCntHeight < BG_SPACE_00_DIVISION_HEIGHT - 2)
		{
			pIdx[nCntIdx] = ((BG_SPACE_00_DIVISION_WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (BG_SPACE_00_DIVISION_WIDTH + 1) * (nCntHeight + 2);
			nCntIdx += 2;
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffBg_space_00->Unlock();

	{
		// 蓋のインデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * (BG_SPACE_00_DIVISION_WIDTH + 2),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_pIdxBuffBg_space_00Lid,
			NULL);

		WORD *pIdx;	// 蓋のインデックス情報へのポインタ

		// 蓋のインデックスバッファをロックし、頂点番号データへのポインタを取得
		g_pIdxBuffBg_space_00Lid->Lock(0, 0, (void**)&pIdx, 0);

		pIdx[0] = (BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT;
		for (int nCntIdx = 0; nCntIdx < BG_SPACE_00_DIVISION_WIDTH + 1; nCntIdx++) 
		{
			pIdx[nCntIdx + 1] = BG_SPACE_00_DIVISION_WIDTH - nCntIdx;
		}

		// 蓋のインデックスバッファをアンロックする
		g_pIdxBuffBg_space_00Lid->Unlock();
	}

	for (int nCntVtx = 0; nCntVtx < ((BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT) + 1; nCntVtx++)
	{
		// 頂点カラーの設定
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBg_space_00->Unlock();
}

//========================================
// UninitBg_space_00関数 - BG:宇宙[00] の終了処理 -
//========================================
void UninitBg_space_00(void)
{
	// テクスチャの破棄
	if (g_pTextureBg_space_00 != NULL)
	{
		g_pTextureBg_space_00->Release();
		g_pTextureBg_space_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg_space_00 != NULL)
	{
		g_pVtxBuffBg_space_00->Release();
		g_pVtxBuffBg_space_00 = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffBg_space_00 != NULL)
	{
		g_pIdxBuffBg_space_00->Release();
		g_pIdxBuffBg_space_00 = NULL;
	}

	// 蓋のインデックスバッファの破棄
	if (g_pIdxBuffBg_space_00Lid != NULL)
	{
		g_pIdxBuffBg_space_00Lid->Release();
		g_pIdxBuffBg_space_00Lid = NULL;
	}
}

//========================================
// UpdateBg_space_00関数 - BG:宇宙[00] の更新処理 -
//========================================
void UpdateBg_space_00(void)
{
	Bg_space_00	*pBg	// BG:宇宙[00] の情報のポインタ
				= &g_bg_space_00;

	pBg->rot += BG_SPACE_00_SPIN;	// 回転をかける
	RotControl(&pBg->rot);			// 向きを制御
}

//========================================
// DrawBg_space_00関数 - BG:宇宙[00] の描画処理 -
//========================================
void DrawBg_space_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	Bg_space_00			*pBg				// BG:宇宙[00] の情報のポインタ
						= &g_bg_space_00;

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBg_space_00);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetObj_stage_00()->rot.y + pBg->rot.y, GetObj_stage_00()->rot.x + pBg->rot.x, GetObj_stage_00()->rot.z + pBg->rot.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_space_00, &g_mtxWorldBg_space_00, &mtxRot);

	{
		D3DXVECTOR3 setPos;	// 設定位置

		if (GetMode() == MODE_TITLE_00)
		{// モードがMD:タイトル画面[00] の時、
			setPos = GetObj_stage_00()->pos;	// 設定位置にOBJ:ステージ[00] の位置を代入
		}
		else if (GetMode() == MODE_GAME_00)
		{// モードがMD:ゲーム画面[00] の時、
			setPos = GetCamera3D()->posV;	// 設定位置にカメラ(3D)の視点の位置を代入
			setPos.y = MD_GAME_00_BG_POS_Y;	// Y座標初期化
		}

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
		D3DXMatrixMultiply(&g_mtxWorldBg_space_00, &g_mtxWorldBg_space_00, &mtxTrans);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg_space_00);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg_space_00, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg_space_00);

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffBg_space_00);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		(BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT,
		0,
		((BG_SPACE_00_DIVISION_WIDTH * (BG_SPACE_00_DIVISION_HEIGHT - 1)) * 2) + (4 * ((BG_SPACE_00_DIVISION_HEIGHT - 1) - 1)));

	// 蓋のインデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffBg_space_00Lid);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,
		0,
		0,
		(BG_SPACE_00_DIVISION_WIDTH + 2),
		0,
		BG_SPACE_00_DIVISION_WIDTH);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}