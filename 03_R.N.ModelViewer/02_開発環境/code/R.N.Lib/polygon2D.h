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

// _R.N.Lib
#include "RNmain.h"

//****************************************
// 列挙型の定義
//****************************************
// 角度の種類
typedef enum
{
	ANGLE_TYPE_FREE,	// 自由
	ANGLE_TYPE_FIXED,	// 固定
	ANGLE_TYPE_MAX,
}ANGLE_TYPE;

//****************************************
// 構造体の定義
//****************************************
// ポリゴン毎の情報(2D)
typedef struct 
{
	// テクスチャの相対パス
	char aTexturePath[TXT_MAX];
	float fWidth;	// 幅
	float fHeight;	// 高さ
	int nMax;		// 最大数
}Polygon2D;
//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// ポリゴン(2D)の必要数を返す
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax);
// 現在のカウントのポリゴン(2D)の番号を返す
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount);
//========== *** 入出力 ***
// ポリゴン(2D)のテクスチャの読み込み
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax);
//========== *** 設定 ***
// 頂点座標の設定処理(2D)
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType);
// rhwの設定処理(2D)
void SetRHW2D(VERTEX_2D *pVtx);
// 頂点色の設定処理(2D)
void SetVertexColor2D(VERTEX_2D *pVtx, Color col);
// テクスチャ座標の設定処理(2D)
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
// テクスチャ座標の設定処理(2D)(切り取りver)
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos);
//========== *** その他 ***
// 描画位置を基準に合わせる
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos);

#endif