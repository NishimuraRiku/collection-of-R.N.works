//========================================
// 
// ポリゴン(2D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon2D.h ***
//========================================
#ifndef _POLYGON2D_H_	// このマクロ定義がされていなかった時
#define _POLYGON2D_H_	// 二重インクルード防止のマクロを定義する

#include "../Other/color.h"

//****************************************
// マクロ定義
//****************************************
// ポリゴン(2D)の最大数
#define POLYGON2D_MAX (1024)

//****************************************
// 構造体定義
//****************************************
// ポリゴン(2D)の設定情報構造体
typedef struct Polygon2DSet_ {
	int nTex = -1;	// テクスチャ番号
	int nPtn = 0;	// パターン番号
	int nPtnX = 1;	// パターン上限X
	int nPtnY = 1;	// パターン上限Y
	float fWidth = 0.0f;	// 幅
	float fHeight = 0.0f;	// 高さ
	D3DXVECTOR3 pos = INITD3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = INITD3DXVECTOR3;	// 向き
	Color col = INITCOLOR;	// 色
	bool bMatchCamera = false;	// カメラに位置合わせフラグ
#if /* 設定フォーマット */0
	// ポリゴン(2D)の設定処理
	Polygon2DSet polySet;
	/* テクスチャ番号 */polySet.nTex = ;
	/* パターン番号   */polySet.nPtn = ;
	/* パターン上限X  */polySet.nPtnX = ;
	/* パターン上限Y  */polySet.nPtnY = ;
	/* 幅             */polySet.fWidth = ;
	/* 高さ           */polySet.fHeight = ;
	/* 位置           */polySet.pos = ;
	/* 向き           */polySet.rot = ;
	/* 色             */polySet.col = ;
	/* カメラ合わせ   */polySet.bMatchCamera = ;
	SetPolygon2D(polySet);
#endif
}Polygon2DSet;

// ポリゴン(2D)の管理情報構造体
typedef struct {
	// ポリゴン(2D)の設定情報
	Polygon2DSet aPolySet[POLYGON2D_MAX];
	int nPolygonNum = 0;	// ポリゴン(2D)の数
}Polygon2DControl;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// ポリゴン(2D)の初期化処理
void InitPolygon2D(void);
// ポリゴン(2D)の終了処理
void UninitPolygon2D(void);
// ポリゴン(2D)の描画処理
void DrawPolygon2D(void);
//========== *** 設定 ***
// ポリゴン(2D)の設定処理
void SetPolygon2D(Polygon2DSet polygon2DSet);
//========== *** 取得 ***
// ポリゴン(2D)の管理情報を取得
Polygon2DControl *GetPolygon2DControl(void);
//========== *** 頂点設定 ***
// 頂点座標の設定処理(2D)
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight);
// rhwの設定処理(2D)
void SetRHW2D(VERTEX_2D *pVtx);
// 頂点色の設定処理(2D)
void SetVertexColor2D(VERTEX_2D *pVtx, Color col);
// テクスチャ座標の設定処理(2D)
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
// テクスチャ座標の設定処理(2D)(切り取りver)
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos);

#endif