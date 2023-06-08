//========================================
// 
// 線(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** line3D.cpp ***
//========================================
// R.N.Lib
#include "../../../R.N.Lib.h"

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// 線(3D)関連の管理情報のパラメーター初期値を返す
void InitParameterLine3DControl(Line3DControl *pPoly);

//****************************************
// グローバル変数宣言
//****************************************
// 線(3D)の頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine3D = NULL;
static Line3DControl g_line3DControl;	// 線(3D)関連の管理情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// InitParameterLine3DControl関数 - 線(3D)関連の管理情報のパラメーター初期値を返す -
// Author:RIKU NISHIMURA
//========================================
void InitParameterLine3DControl(Line3DControl *pPoly) {
	pPoly->nLineNum = 0;	// 線の数
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// InitLine3D関数 - 線(3D)関連の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitLine3D(void) {
	// 物理関連処理の管理情報のポインタ
	Line3DControl *pPoly = &g_line3DControl;

	// パラメーターの初期化
	InitParameterLine3DControl(pPoly);

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * LINE3D_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine3D,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLine3D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLine = 0; nCntLine < LINE3D_MAX; nCntLine++, pVtx += 2)
	{
		// 頂点カラーを設定
		pVtx[0].col = ColorToD3DCOLOR(INITCOLOR);
		pVtx[1].col = ColorToD3DCOLOR(INITCOLOR);
	}

	// 頂点座標をアンロックする
	g_pVtxBuffLine3D->Unlock();
}

//========================================
// DrawLine3D関数 - 線(3D)関連の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawLine3D(void) {
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtx;
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// レンダリングステートを設定
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLine3D, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int nCntLine = 0; nCntLine < g_line3DControl.nLineNum; nCntLine++) {
		if (g_line3DControl.aLineSet[nCntLine].bZTest) {
			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
		else {
			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_LINELIST, 2 * nCntLine, 1);
	}

	// レンダリングステートを元に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//========================================
// UninitLine3D関数 - 線(3D)関連の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitLine3D(void) {
	// 線の頂点バッファの破棄
	if (g_pVtxBuffLine3D != NULL) {
		g_pVtxBuffLine3D->Release();
		g_pVtxBuffLine3D = NULL;
	}
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// GetLine3DControl関数 - 線(3D)の管理情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Line3DControl *GetLine3DControl(void) {
	return &g_line3DControl;
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================

//========================================
// SetLine3D関数 - 線(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetLine3D(Line3DSet lineSet) {
	// 物理関連処理の管理情報のポインタ
	Line3DControl *pPoly = &g_line3DControl;

	if (pPoly->nLineNum >= LINE3D_MAX)
	{// 当たり判定の線の数が最大数に達している時、
		return;	// 処理を終了する
	}

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLine3D->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタを進める
	pVtx += pPoly->nLineNum * 2;

	// 位置を代入
	pVtx[0].pos = lineSet.pos1;
	pVtx[1].pos = lineSet.pos2;

	// 色を代入
	pVtx[0].col = ColorToD3DCOLOR(lineSet.col1);
	pVtx[1].col = ColorToD3DCOLOR(lineSet.col2);

	// 頂点座標をアンロックする
	g_pVtxBuffLine3D->Unlock();

	// 設定情報を保存
	pPoly->aLineSet[pPoly->nLineNum] = lineSet;

	// 当たり判定の線の数を加算
	pPoly->nLineNum++;
}