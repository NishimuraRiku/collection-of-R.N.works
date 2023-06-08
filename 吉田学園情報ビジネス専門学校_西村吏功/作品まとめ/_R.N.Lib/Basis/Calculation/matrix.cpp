//========================================
// 
// マトリックス関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** matrix.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ConvPosRotToMatrix関数 - 位置と向きをマトリックスに変換して返す -
// Author:RIKU NISHIMURA
//========================================
D3DXMATRIX ConvPosRotToMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMATRIX mtx;	// 本体のマトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	return mtx;
}

//========================================
// MultiplyMatrix関数 - マトリックスを掛け合わせる -
// Author:RIKU NISHIMURA
//========================================
D3DXMATRIX MultiplyMatrix(D3DXMATRIX mtxParent, D3DXMATRIX mtxChild) {
	// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(
		&mtxChild,
		&mtxChild,
		&mtxParent);

	return mtxChild;
}

//========================================
// ConvMatrixToPos関数 - マトリックスの位置成分を分解して返す -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvMatrixToPos(D3DXMATRIX mtx) {
	return D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
}

//========================================
// ConvMatrixToRot関数 - マトリックスの向き成分を分解して返す -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvMatrixToRot(D3DXMATRIX mtx) {
	D3DXVECTOR3 rot;

	// 行列の3列目を取得して、D3DXVECTOR3に変換
	rot.x = mtx._31;
	rot.y = mtx._32;
	rot.z = mtx._33;

	// ベクトルを正規化
	D3DXVec3Normalize(&rot, &rot);

	return rot;
}

//========================================
// SetMatrixRot関数 - マトリックスに向きを代入 -
// Author:RIKU NISHIMURA
//========================================
void SetMatrixRot(D3DXMATRIX *pMtx, D3DXVECTOR3 rot) {
	// 初期化する向きの値
	float pitch = rot.x;
	float yaw = rot.y;
	float roll = rot.z;

	D3DXMATRIX matrix;
	D3DXMatrixIdentity(pMtx); // 行列を単位行列で初期化

	// 向きの要素を初期化
	pMtx->_11 = cos(yaw) * cos(roll);
	pMtx->_12 = cos(yaw) * sin(roll);
	pMtx->_13 = sin(yaw);
	pMtx->_21 = sin(pitch) * sin(yaw) * cos(roll) - cos(pitch) * sin(roll);
	pMtx->_22 = sin(pitch) * sin(yaw) * sin(roll) + cos(pitch) * cos(roll);
	pMtx->_23 = -sin(pitch) * cos(yaw);
	pMtx->_31 = -cos(pitch) * sin(yaw) * cos(roll) - sin(pitch) * sin(roll);
	pMtx->_32 = -cos(pitch) * sin(yaw) * sin(roll) + sin(pitch) * cos(roll);
	pMtx->_33 = cos(pitch) * cos(yaw);
}