//========================================
// 
// テキスト(3D)の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** text3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================

//========================================
// SetText3D関数 - テキスト(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetText3D(char *pString, int nFont, TEXT_DISP disp, D3DXMATRIX mtx, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard) {
	assert(pString != NULL);

	// カメラの向き(注視点から視点)
	D3DXVECTOR3 cameraRot; {
		D3DXVECTOR3 posV = GetCamera3D()->posV;
		posV.y           = GetCamera3D()->posR.y;
		cameraRot        = FindRot(GetCamera3D()->posR, posV);
	}

	for (int nCntDigit = 0; nCntDigit < (int)strlen(pString); nCntDigit++) {
		if (pString[nCntDigit] == ' ') {
			continue;	// カウントの文字が空白の時、折り返す
		}

		D3DXVECTOR3 setPos = INITD3DXVECTOR3;	// 設定位置		
		Font font          = GetFont(nFont);	// フォントの情報
		float fSpaceRate   = (float)font.nSpaceNum / (float)font.nSpaceDenom;	// 間隔比率

		// 表示形式に応じた設定位置の設定
		switch (disp) {
		case TEXT_DISP_CENTER:
			// 設定位置に文字の最終的な長さの半分を減算
			setPos.y += ((fWidth * fSpaceRate) * strlen(pString)) * 0.5f;
			// 設定位置に配置間隔Xを適用(加算)
			setPos.y -= ((float)nCntDigit + 0.5f) * (fWidth * fSpaceRate);
			break;
		case TEXT_DISP_LEFT:
			// 設定位置に配置間隔Xを適用(加算)
			setPos.y -= nCntDigit * (fWidth * fSpaceRate);
			break;
		case TEXT_DISP_RIGHT:
			// 設定位置に文字の最終的な長さを減算
			setPos.y += (fWidth * fSpaceRate) * (strlen(pString) - 1);
			// 設定位置に配置間隔Xを適用(加算)
			setPos.y -= nCntDigit * (fWidth * fSpaceRate);
			break;
		}

		// 結果マトリックス
		D3DXMATRIX resultMtx; {
			D3DXVECTOR3 charPos;
			D3DXVec3Cross(&charPos, &setPos, &cameraRot);

			// 基準マトリックスとテキストマトリックスを掛け合わせる
			resultMtx = MultiplyMatrix(
				ConvPosRotToMatrix(ConvMatrixToPos(mtx), INITD3DXVECTOR3),
				ConvPosRotToMatrix(charPos, INITD3DXVECTOR3));
		}

		// ポリゴン(3D)を設定
		SetPolygon3D(
			font.nTex,
			(int)pString[nCntDigit] - (int)font.cStartChar,
			font.nPtnWidth,
			font.nPtnHeight,
			fWidth,
			fHeight,
			resultMtx,
			col,
			bZtest,
			bLighting,
			bBillboard);
	}
}