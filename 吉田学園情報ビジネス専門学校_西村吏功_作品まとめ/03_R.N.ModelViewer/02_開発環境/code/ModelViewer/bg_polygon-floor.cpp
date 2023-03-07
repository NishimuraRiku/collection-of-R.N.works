//========================================
// 
// BG:ポリゴン床の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_polygon-floor.cpp ***
//========================================
#include <stdio.h>
#include "main.h"
// BackGround
#include "bg_polygon-floor.h"
// R.N.Lib
#include "../R.N.Lib/RNmain.h"

//****************************************
// マクロ定義
//****************************************
// BG:ポリゴン床のテクスチャの相対パス
#define BG_POLYGONFLOOR_TEXTURE_PATH "data\\APPDATA\\TEXTURE\\BackGround\\bg_plaid-tile_000.png"
// BG:ポリゴン床の幅
#define BG_POLYGONFLOOR_WIDTH (100.0f)
// BG:ポリゴン床の高さ
#define BG_POLYGONFLOOR_HEIGHT (100.0f)
// BG:ポリゴン床の法線ベクトル
#define BG_POLYGONFLOOR_NOR (D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//****************************************
// 構造体の定義
//****************************************
// BG:ポリゴン床の情報構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
}Bg_polygonFloor;

//****************************************
// グローバル宣言
//****************************************
// BG:ポリゴン床のテクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBg_polygonFloor = NULL;
// BG:ポリゴン床の頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_polygonFloor = NULL;
D3DXMATRIX g_mtxWorldBg_polygonFloor;		// BG:ポリゴン床のワールドマトリックス
Bg_polygonFloor g_bg_polygonFloor_00;		// BG:ポリゴン床の情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// InitBg_polygonFloor関数 - BG:ポリゴン床の初期化処理 -
//========================================
void InitBg_polygonFloor(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_POLYGONFLOOR_TEXTURE_PATH, &g_pTextureBg_polygonFloor);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_polygonFloor,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_polygonFloor->Lock(0, 0, (void**)&pVtx, 0);
	
	// 頂点座標の設定
	SetVertexPos3D(pVtx, BG_POLYGONFLOOR_WIDTH, BG_POLYGONFLOOR_HEIGHT);
	
	// 法線ベクトルの設定
	SetNormalLine3D(pVtx, BG_POLYGONFLOOR_NOR);
	
	// 頂点カラーの設定
	SetVertexColor3D(pVtx, { 255,255,255,255 });
	
	// テクスチャ座標の設定
	SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	
	// 頂点座標をアンロックする
	g_pVtxBuffBg_polygonFloor->Unlock();
}

//========================================
// UninitBg_polygonFloor関数 - BG:ポリゴン床の終了処理 -
//========================================
void UninitBg_polygonFloor(void)
{
	// テクスチャの破棄
	if (g_pTextureBg_polygonFloor != NULL)
	{
		g_pTextureBg_polygonFloor->Release();
		g_pTextureBg_polygonFloor = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg_polygonFloor != NULL)
	{
		g_pVtxBuffBg_polygonFloor->Release();
		g_pVtxBuffBg_polygonFloor = NULL;
	}
}

//========================================
// UpdateBg_polygonFloor関数 - BG:ポリゴン床の更新処理 -
//========================================
void UpdateBg_polygonFloor(void)
{

}

//========================================
// DrawBg_polygonFloor関数 - BG:ポリゴン床の描画処理 -
//========================================
void DrawBg_polygonFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	Bg_polygonFloor *pBg = &g_bg_polygonFloor_00;	// BG:ポリゴン床の情報のポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBg_polygonFloor);
	
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pBg->rot.y, pBg->rot.x, pBg->rot.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_polygonFloor, &g_mtxWorldBg_polygonFloor, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pBg->pos.x, pBg->pos.y, pBg->pos.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_polygonFloor, &g_mtxWorldBg_polygonFloor, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg_polygonFloor);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg_polygonFloor, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg_polygonFloor);

	// ポリゴン床の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}