//========================================
// 
// ポリゴン(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon3D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// マクロ定義
//****************************************
// 線(3D)の最大数
#define LINE3D_MAX (1024)

//****************************************
// 構造体の定義
//****************************************
// ポリゴン(3D)関連の管理情報構造体
typedef struct
{
	int nLineNum;	// 当たり判定の線の数
}Polygon3DControl;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// ポリゴン(3D)関連の管理情報のパラメーター初期値を返す
Polygon3DControl InitParameterPolygon3DControl(void);

//****************************************
// グローバル変数宣言
//****************************************
// 線(3D)の頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine3D = NULL;
Polygon3DControl g_polygon3DControl;	// ポリゴン(3D)関連の管理情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// InitParameterPolygon3DControl関数 - ポリゴン(3D)関連の管理情報のパラメーター初期値を返す -
// Author:RIKU NISHIMURA
//========================================
Polygon3DControl InitParameterPolygon3DControl(void)
{
	return{
		0,	// 線の数
	};
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
// InitPolygon3D関数 - ポリゴン(3D)関連の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitPolygon3D(void)
{
	// 物理関連処理の管理情報のポインタ
	Polygon3DControl pPoly = g_polygon3DControl;

	// パラメーターの初期化
	pPoly = InitParameterPolygon3DControl();

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
// DrawPolygon3D関数 - ポリゴン(3D)関連の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawPolygon3D(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
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
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLine3D, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntLine = 0; nCntLine < g_polygon3DControl.nLineNum; nCntLine++)
	{
		// ポリゴン床の描画
		pDevice->DrawPrimitive(D3DPT_LINELIST, 2 * nCntLine, 1);
	}

	// レンダリングステートを元に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//========================================
// InitLine3DNum関数 - 線(3D)の数を初期化 -
// Author:RIKU NISHIMURA
//========================================
void InitLine3DNum(void)
{
	// 線の数を初期化
	g_polygon3DControl.nLineNum = 0;
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// SetLine3D関数 - 線(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetLine3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, Color col)
{
	// 物理関連処理の管理情報のポインタ
	Polygon3DControl *pPoly = &g_polygon3DControl;

	if (pPoly->nLineNum >= LINE3D_MAX)
	{
		return;	// 当たり判定の線の数が最大数に達している時、
	}

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLine3D->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタを進める
	pVtx += pPoly->nLineNum * 2;

	// 位置を代入
	pVtx[0].pos = pos1;
	pVtx[1].pos = pos2;

	// 色を代入
	pVtx[0].col = ColorToD3DCOLOR(col);
	pVtx[1].col = ColorToD3DCOLOR(col);

	// 頂点座標をアンロックする
	g_pVtxBuffLine3D->Unlock();

	// 当たり判定の線の数を加算
	pPoly->nLineNum++;
}

//========================================
// SetVertexPos3D関数 - 頂点座標の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight)
{
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
	pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
}

//========================================
// SetNormalLine3D関数 - 法線ベクトルの設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor) 
{
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
void SetVertexColor3D(VERTEX_3D *pVtx, Color col)
{
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
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
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
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
}

//========================================
// SetMaterial関数 - マテリアルの設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col)
{
	D3DXCOLOR mtlColTemp = pMat->Diffuse;	// マテリアルの色を保存

	// マテリアルの色を設定
	pMat->Diffuse.r = mtlColTemp.r * ((float)col.r / (float)255);
	pMat->Diffuse.g = mtlColTemp.g * ((float)col.g / (float)255);
	pMat->Diffuse.b = mtlColTemp.b * ((float)col.b / (float)255);
	pMat->Diffuse.a = mtlColTemp.a * ((float)col.a / (float)255);

	// マテリアルの設定
	pDevice->SetMaterial(pMat);

	// マテリアルの色を元に戻す
	pMat->Diffuse = mtlColTemp;
}