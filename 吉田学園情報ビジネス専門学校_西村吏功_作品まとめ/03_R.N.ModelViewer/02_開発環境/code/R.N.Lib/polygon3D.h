//========================================
// 
// ポリゴン(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon3D.h ***
//========================================
#ifndef _POLYGON3D_H_	// このマクロ定義がされていなかった時
#define _POLYGON3D_H_	// 二重インクルード防止のマクロを定義する

#include <stdio.h>
// _R.N.Lib
#include "RNmain.h"

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// ポリゴン(3D)関連の初期化処理
void InitPolygon3D(void);
// ポリゴン(3D)関連の描画処理
void DrawPolygon3D(void);
// 線(3D)の数を初期化
void InitLine3DNum(void);
//========== *** 設定 ***
// 線(3D)の設定処理
void SetLine3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, Color col);
// 頂点座標の設定処理(3D)
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight);
// 法線ベクトルの設定処理(3D)
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
// 頂点色の設定処理(3D)
void SetVertexColor3D(VERTEX_3D *pVtx, Color col);
// テクスチャ座標の設定処理(3D)
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
// マテリアルの設定
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col);

#endif