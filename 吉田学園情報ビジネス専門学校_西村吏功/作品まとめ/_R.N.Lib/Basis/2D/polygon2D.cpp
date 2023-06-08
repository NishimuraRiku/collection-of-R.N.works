//========================================
// 
// ポリゴン(2D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon2D.cpp ***
//========================================
#include "assert.h"
// R.N.Lib
#include "../../R.N.Lib.h"

//****************************************
// グローバル変数宣言
//****************************************
// ポリゴン(2D)の頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon2D = NULL;
// ポリゴン(2D)の管理情報
static Polygon2DControl g_polygon2DControl;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// ポリゴン(2D)の初期化処理
// Author:RIKU NISHIMURA
//========================================
void InitPolygon2D(void) {
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POLYGON2D_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon2D,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < POLYGON2D_MAX; nCntPoly++, pVtx += 4)
	{
		// rhwの設定
		SetRHW2D(pVtx);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPolygon2D->Unlock();
}

//========================================
// ポリゴン(2D)の終了処理
// Author:RIKU NISHIMURA
//========================================
void UninitPolygon2D(void) {
	// 頂点バッファの破棄
	if (g_pVtxBuffPolygon2D != NULL)
	{
		g_pVtxBuffPolygon2D->Release();
		g_pVtxBuffPolygon2D = NULL;
	}
}

//========================================
// ポリゴン2Dの描画処理
// Author:RIKU NISHIMURA
//========================================
void DrawPolygon2D(void) {
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetD3DDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon2D, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴン(2D)の管理情報のポインタ
	Polygon2DControl *pPolyCtl = &g_polygon2DControl;
	// ポリゴン(2D)の設定情報のポインタ
	Polygon2DSet *pPolySet = pPolyCtl->aPolySet;

	for (int nCntPoly = 0; nCntPoly < pPolyCtl->nPolygonNum; nCntPoly++, pPolySet++)
	{
		// テクスチャの設定
		SetTexture(pDevice, pPolySet->nTex);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPoly * 4, 2);
	}
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================

//========================================
// ポリゴン(2D)の設定処理
// Author:RIKU NISHIMURA
//========================================
void SetPolygon2D(Polygon2DSet polygon2DSet) {
	if (g_polygon2DControl.nPolygonNum == POLYGON2D_MAX)
	{// ポリゴン(2D)の数が最大数に達している時、
		assert(false);
		return;	// 処理を終了する
	}

	// 設定情報を代入
	g_polygon2DControl.aPolySet[g_polygon2DControl.nPolygonNum] = polygon2DSet;

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon2D->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 頂点情報へのポインタを進める
		pVtx += g_polygon2DControl.nPolygonNum * 4;

		// ウィンドウ領域補正
		polygon2DSet.pos.x *= IMGUI_WINDOW_CORRECT_X;
		polygon2DSet.pos.y *= IMGUI_WINDOW_CORRECT_Y;
		polygon2DSet.fWidth *= IMGUI_WINDOW_CORRECT_X;
		polygon2DSet.fHeight *= IMGUI_WINDOW_CORRECT_Y;

		// 頂点座標の設定
		SetVertexPos2D(pVtx,
			polygon2DSet.pos,
			polygon2DSet.rot,
			polygon2DSet.bMatchCamera,
			polygon2DSet.fWidth,
			polygon2DSet.fHeight);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, polygon2DSet.col);

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			polygon2DSet.nPtn,
			polygon2DSet.nPtnX,
			polygon2DSet.nPtnY,
			NULL);
	}
	// 頂点座標をアンロックする
	g_pVtxBuffPolygon2D->Unlock();

	// ポリゴン(2D)の数を加算
	g_polygon2DControl.nPolygonNum++;
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// ポリゴン(2D)の管理情報を取得
// Author:RIKU NISHIMURA
//========================================
Polygon2DControl *GetPolygon2DControl(void)
{
	return &g_polygon2DControl;
}

//============================================================
//--------------------| *** 頂点設定 *** |--------------------
//============================================================

//========================================
// 頂点座標の設定処理(2D)
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight) {
	if (bMatchCamera)
	{// 位置をカメラに合わせるフラグが真の時、
		// 位置をカメラに合わせる
		//MatchCamera2DPosition(&pos);
	}

	// 対角線の長さ
	float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;
	// 対角線の角度
	float fAngle = atan2f(fWidth, fHeight);

	// 頂点座標を設定
	pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
	pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
}

//========================================
// rhwの設定処理(2D)
// Author:RIKU NISHIMURA
//========================================
void SetRHW2D(VERTEX_2D *pVtx) 
{
	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// 頂点色の設定処理(2D)
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor2D(VERTEX_2D *pVtx, Color col)
{
	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// テクスチャ座標の設定処理(2D)
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
{
	if (nPtnMaxX * nPtnMaxY == 0) 
	{// パターンの上限が0の時、
		// 処理を終了する
		return;
	}

	D3DXVECTOR3 pos;	// 位置

	if (pPos == NULL) 
	{// 位置のポインタがNULLの時、
		// 位置を初期化
		pos = D3DXVECTOR3(0, 0, 0);
	}
	else 
	{// 位置のポインタがNULLで無い時、
		// 位置を-補正して代入
		pos = -*pPos;
	}

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
}

//========================================
// テクスチャ座標の設定処理(2D)(切り取りver)
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos)
{
	pos.x += 0.5f;
	pos.y += 0.5f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[1].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[2].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
	pVtx[3].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
}