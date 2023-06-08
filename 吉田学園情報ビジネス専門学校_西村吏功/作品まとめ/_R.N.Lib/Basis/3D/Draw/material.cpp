//========================================
// 
// マテリアル関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** material.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetMaterial関数 - マテリアルの設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col, bool bLighting) {
	// マテリアルの材質パラメーターを保存
	D3DXCOLOR DiffuseTemp  = pMat->Diffuse;
	D3DXCOLOR EmissiveTemp = pMat->Emissive;

	// マテリアルの材質パラメーターを設定
	pMat->Diffuse.r = DiffuseTemp.r * ((float)col.r / (float)255);
	pMat->Diffuse.g = DiffuseTemp.g * ((float)col.g / (float)255);
	pMat->Diffuse.b = DiffuseTemp.b * ((float)col.b / (float)255);
	pMat->Diffuse.a = DiffuseTemp.a * ((float)col.a / (float)255);

	if (bLighting) {
		// ライティングの有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else {
		pMat->Emissive.r = DiffuseTemp.r * ((float)col.r / (float)255);
		pMat->Emissive.g = DiffuseTemp.g * ((float)col.g / (float)255);
		pMat->Emissive.b = DiffuseTemp.b * ((float)col.b / (float)255);
		pMat->Emissive.a = DiffuseTemp.a * ((float)col.a / (float)255);

		if (pMat->Emissive.r + pMat->Emissive.g + pMat->Emissive.b > 0.0f)
		{// 完全発光時、
			// ライティングの無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}
		else {
			// ライティングの有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	// マテリアルの設定
	pDevice->SetMaterial(pMat);

	// マテリアルの材質パラメーターを元に戻す
	pMat->Diffuse  = DiffuseTemp;
	pMat->Emissive = EmissiveTemp;
}