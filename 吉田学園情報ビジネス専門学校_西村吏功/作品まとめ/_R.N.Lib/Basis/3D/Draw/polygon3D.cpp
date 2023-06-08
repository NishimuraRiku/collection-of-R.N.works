//========================================
// 
// ポリゴン(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//****************************************
// グローバル変数宣言
//****************************************
// ポリゴン(3D)の頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffpolygon3D = NULL;
// ポリゴン(3D)の管理情報
Polygon3DControl g_polygon3DControl;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// InitPolygon3D関数 - ポリゴン(3D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitPolygon3D(void) {
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * POLYGON3D_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffpolygon3D,
		NULL);
}

//========================================
// UninitPolygon3D関数 - ポリゴン(3D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitPolygon3D(void) {
	// 頂点バッファの破棄
	if (g_pVtxBuffpolygon3D != NULL) {
		g_pVtxBuffpolygon3D->Release();
		g_pVtxBuffpolygon3D = NULL;
	}
}

//========================================
// DrawPolygon3D関数 - ポリゴン(3D)の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawPolygon3D(void) {
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxSelf;		// 本体マトリックス
	D3DXMATRIX mtxView;		// ビューマトリックス取得用

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffpolygon3D, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	int aOrderList[POLYGON3D_MAX];	// 順番リスト

	{// カメラ面までの距離でソート
		// カメラ面までの距離を計算&格納
		float aDistance[POLYGON3D_MAX];
		for (int nCntPoly = 0; nCntPoly < g_polygon3DControl.nPolygonNum; nCntPoly++) {
			aDistance[nCntPoly] = FindDistanceToCameraPlane(ConvMatrixToPos(g_polygon3DControl.aPolySet[nCntPoly].mtx));
			aOrderList[nCntPoly] = nCntPoly;
		}

		float fTemp;		// 一時的に値を格納
		int nTempCnt = -1;	// カウントを格納
		for (int nCntCompare1 = 0; nCntCompare1 < g_polygon3DControl.nPolygonNum - 1; nCntCompare1++) {
			// 先頭の要素を最小値として記憶
			fTemp = aDistance[nCntCompare1];

			for (int nCntCompare2 = nCntCompare1 + 1; nCntCompare2 < g_polygon3DControl.nPolygonNum; nCntCompare2++) {
				if (aDistance[nCntCompare2] > fTemp)
				{// 要素の値が最小値を上回っていた時、
					// 値を記憶
					fTemp = aDistance[nCntCompare2];
					// カウントを記憶
					nTempCnt = nCntCompare2;
				}
			}

			if (aDistance[nCntCompare1] < fTemp)
			{// 最小値が先頭の要素の値を下回っていた時、
				if (nTempCnt == -1) {
					assert(false);
					nTempCnt = 0;
				}

				// 値を入れ替える
				aDistance[nTempCnt] = aDistance[nCntCompare1];
				aDistance[nCntCompare1] = fTemp;

				// 順番を入れ替える
				int nTemp = aOrderList[nTempCnt];
				aOrderList[nTempCnt] = aOrderList[nCntCompare1];
				aOrderList[nCntCompare1] = nTemp;
			}
		}
	}

	for (int nCntPoly = 0; nCntPoly < g_polygon3DControl.nPolygonNum; nCntPoly++) {
		int nPoly = aOrderList[nCntPoly];

		// ポリゴン(3D)の設定情報
		Polygon3DSet polygon3DSet = g_polygon3DControl.aPolySet[nPoly];

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxSelf);

		if (!polygon3DSet.bZtest)
		{// Zテストフラグが偽の時、
			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}
		else 
		{// Zテストフラグが真の時、
			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

		// ライティングの有効/無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, polygon3DSet.bLighting);

		if (polygon3DSet.bBillboard)
		{// ビルボードフラグが真の時、
			// 位置マトリックスを設定
			D3DXVECTOR3 setPos = ConvMatrixToPos(polygon3DSet.mtx);
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&mtxSelf, NULL, &mtxView);
			mtxSelf._41 = 0.0f;
			mtxSelf._42 = 0.0f;
			mtxSelf._43 = 0.0f;

			// 位置マトリックスを設定
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);
		}
		else
		{// ビルボードフラグが偽の時、
			// 位置と向きを反映
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &polygon3DSet.mtx);
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

		// テクスチャの設定
		SetTexture(pDevice, polygon3DSet.nTex);

		// ポリゴン(3D)の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nPoly, 2);
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================

//========================================
// SetPolygon3D関数 - ポリゴン(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetPolygon3D(int nTex, int nPtn, int nPtnX, int nPtnY, float fWidth, float fHeight, D3DXMATRIX mtx, Color col, bool bZtest, bool bLighting, bool bBillboard, bool bMirror) {
	if (g_polygon3DControl.nPolygonNum >= POLYGON3D_MAX)
	{// ポリゴン(3D)の数が最大数に達している時、
		assert(false);
		return;	// 処理を終了する
	}

	// ポリゴン数
	int nPolyNum = g_polygon3DControl.nPolygonNum;

	// 設定情報を代入
	g_polygon3DControl.aPolySet[nPolyNum].nTex       = nTex; 
	g_polygon3DControl.aPolySet[nPolyNum].nPtn       = nPtn;
	g_polygon3DControl.aPolySet[nPolyNum].nPtnX      = nPtnX;
	g_polygon3DControl.aPolySet[nPolyNum].nPtnY      = nPtnY;
	g_polygon3DControl.aPolySet[nPolyNum].fWidth     = fWidth;
	g_polygon3DControl.aPolySet[nPolyNum].fHeight    = fHeight;
	g_polygon3DControl.aPolySet[nPolyNum].mtx        = mtx;
	g_polygon3DControl.aPolySet[nPolyNum].col        = col;
	g_polygon3DControl.aPolySet[nPolyNum].bZtest     = bZtest;
	g_polygon3DControl.aPolySet[nPolyNum].bLighting  = bLighting;
	g_polygon3DControl.aPolySet[nPolyNum].bBillboard = bBillboard;
	g_polygon3DControl.aPolySet[nPolyNum].bMirror    = bMirror;

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffpolygon3D->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 頂点情報へのポインタを進める
		pVtx += g_polygon3DControl.nPolygonNum * 4;

		// 頂点座標の設定
		SetVertexPos3D(pVtx, fWidth, fHeight);

		// 法線ベクトルの設定
		SetNormalLine3D(pVtx, FindNormalVector(ConvMatrixToRot(mtx)));

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, col);

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx, nPtn, nPtnX, nPtnY, NULL, bMirror);
	}
	// 頂点座標をアンロックする
	g_pVtxBuffpolygon3D->Unlock();

	// ポリゴン(3D)の数を加算
	g_polygon3DControl.nPolygonNum++;
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// GetPolygon3DControl関数 - ポリゴン(3D)の管理情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Polygon3DControl *GetPolygon3DControl(void) {
	return &g_polygon3DControl;
}

//============================================================
//--------------------| *** 頂点設定 *** |--------------------
//============================================================

//========================================
// SetVertexPos3D関数 - 頂点座標の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight) {
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
}

//========================================
// SetNormalLine3D関数 - 法線ベクトルの設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor) {
	// 法線ベクトルの設定
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//========================================
// SetVertexColor3D関数 - 頂点色の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor3D(VERTEX_3D *pVtx, Color col) {
	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos3D関数 - テクスチャ座標の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos, bool bMirror) {
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

	// 左右反転フラグに応じたテクスチャ座標の設定
	if (bMirror) {
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x, ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                    , ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                    , (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	}
	else {
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                     , ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x , ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                     , (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x , (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	}
}