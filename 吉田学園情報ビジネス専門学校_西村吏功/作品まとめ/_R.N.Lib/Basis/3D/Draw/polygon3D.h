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

//****************************************
// マクロ定義
//****************************************
// ポリゴン(3D)の最大数
#define POLYGON3D_MAX (1024)

//****************************************
// 構造体定義
//****************************************
// ポリゴン(3D)の設定情報構造体
typedef struct {
	int nTex        = -1;				// テクスチャ番号
	int nPtn        = 0;				// パターン番号
	int nPtnX       = 1;				// パターン上限X
	int nPtnY       = 1;				// パターン上限Y
	float fWidth    = 0.0f;				// 幅
	float fHeight   = 0.0f;				// 高さ
	D3DXMATRIX mtx  = INITD3DXMATRIX;	// マトリックス
	Color col       = INITCOLOR;		// 色
	bool bZtest     = true;				// Zテストフラグ
	bool bLighting  = true;				// ライティングフラグ
	bool bBillboard = false;			// ビルボードフラグ
	bool bMirror    = false;			// 左右反転フラグ
}Polygon3DSet;

// ポリゴン(3D)の管理情報構造体
typedef struct {
	// ポリゴン(3D)の設定情報
	Polygon3DSet aPolySet[POLYGON3D_MAX];
	int nPolygonNum = 0;	// ポリゴン(3D)の数
}Polygon3DControl;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// ポリゴン(3D)の初期化処理
void InitPolygon3D(void);
// ポリゴン(3D)の終了処理
void UninitPolygon3D(void);
// ポリゴン(3D)の描画処理
void DrawPolygon3D(void);
//========== *** 設定 ***
// ポリゴン(3D)の設定処理
void SetPolygon3D(
	int nTex        = -1,
	int nPtn        = 0,
	int nPtnX       = 1,
	int nPtnY       = 1,
	float fWidth    = 0.0f,
	float fHeight   = 0.0f,
	D3DXMATRIX mtx  = INITD3DXMATRIX,
	Color col       = INITCOLOR,
	bool bZtest     = true,
	bool bLighting  = true,
	bool bBillboard = false,
	bool bMirror    = false);
//========== *** 取得 ***
// ポリゴン(3D)の管理情報を取得
Polygon3DControl *GetPolygon3DControl(void);
//========== *** 頂点設定 ***
// 頂点座標の設定処理(3D)
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight);
// 法線ベクトルの設定処理(3D)
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
// 頂点色の設定処理(3D)
void SetVertexColor3D(VERTEX_3D *pVtx, Color col);
// テクスチャ座標の設定処理(3D)
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos, bool bMirror = false);

#endif