//========================================
// 
// ポリゴン(2D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon2D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// GetPolygon2DNum関数 - ポリゴン(2D)の必要数を返す -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax)
{
	int nNum = 0;	// ポリゴン数

	// ポリゴン毎の最大数を加算
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nNum += pPoly2D->nMax;
	}

	return nNum;
}

//========================================
// GetPolygon2DIndex関数 - 現在のカウントのポリゴン(2D)の番号を返す -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount)
{
	int nLine = 0;	// ライン

	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nLine += pPoly2D->nMax;	// ラインに最大数を加算

		if (nCount < nLine) 
		{// カウントがライン未満の時、カウントの値を返す
			return nCntPoly;
		}
	}

	return 0;
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================
//========================================
// LoadPolygon2DTexture関数 - ポリゴン(2D)のテクスチャの読み込み -
// Author:RIKU NISHIMURA
//========================================
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax) 
{
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++, pTextureUi_map_00++)
	{
		D3DXCreateTextureFromFile(pDevice, pPoly2D->aTexturePath, pTextureUi_map_00);
	}
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// SetVertexPos2D関数 - 頂点座標の設定処理(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType)
{
	if (bMatchCamera)
	{// 位置をカメラに合わせるフラグが真の時、
		// 位置をカメラに合わせる
		MatchCamera2DPosition(&pos);
	}

	if ((angleType == ANGLE_TYPE_FIXED) || (rot.z == D3DX_PI))
	{// 角度の種類が固定、もしくは角度が真上の時、
		// 頂点座標を設定
		pVtx[0].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y + (fHeight * 0.5f);
	}
	else if (rot.z == 0.0f)
	{// 角度が真下の時、
		// 頂点座標を設定
		pVtx[0].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y - (fHeight * 0.5f);
	}
	else 
	{
		// 対角線の長さ
		float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;
		// 対角線の角度
		float fAngle = atan2f(fWidth, fHeight);

		// 頂点座標を設定
		pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
		pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
	}
}

//========================================
// SetRHW2D関数 - rhwの設定処理(2D) -
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
// SetVertexColor2D関数 - 頂点色の設定処理(2D) -
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
// SetTexturePos2D関数 - テクスチャ座標の設定処理(2D) -
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
// SetTexturePos2DCut関数 - テクスチャ座標の設定処理(2D)(切り取りver) -
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

//============================================================
//--------------------| *** その他 *** |----------------------
//============================================================
//========================================
// MatchPosition関数 - 描画位置を基準に合わせる -
// Author:RIKU NISHIMURA
//========================================
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos)
{
	// 位置を基準の位置に合わせる
	pDrawPos->x = pDrawPos->x - basePos.x + (SCREEN_WIDTH * 0.5f);
	pDrawPos->y = pDrawPos->y - basePos.y + (SCREEN_HEIGHT * 0.5f);
}