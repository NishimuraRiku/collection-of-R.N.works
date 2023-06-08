//========================================
// 
// マトリックス関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** matrix.h ***
//========================================
#ifndef _MATRIX_H_	// このマクロ定義がされていなかった時
#define _MATRIX_H_	// 二重インクルード防止のマクロを定義する

#include "../../RNmain.h"

//****************************************
// マクロ定義
//****************************************
// D3DXMATRIX型の初期値
#define INITD3DXMATRIX D3DXMATRIX(1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)

//****************************************
// プロトタイプ宣言
//****************************************
// 位置と向きをマトリックスに変換して返す
D3DXMATRIX ConvPosRotToMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
// マトリックスを掛け合わせる
D3DXMATRIX MultiplyMatrix(D3DXMATRIX mtxParent, D3DXMATRIX mtxChild);
// マトリックスの位置成分を分解して返す
D3DXVECTOR3 ConvMatrixToPos(D3DXMATRIX mtx);
// マトリックスの向き成分を分解して返す
D3DXVECTOR3 ConvMatrixToRot(D3DXMATRIX mtx);
// マトリックスに向きを代入
void SetMatrixRot(D3DXMATRIX *pMtx, D3DXVECTOR3 rot);

#endif