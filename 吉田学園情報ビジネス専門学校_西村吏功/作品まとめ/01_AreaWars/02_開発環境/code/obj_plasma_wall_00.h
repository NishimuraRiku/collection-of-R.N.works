//========================================
//  *** obj_plasma_wall_00.h ***
//========================================
#ifndef _OBJ_PLASMA_WALL_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_PLASMA_WALL_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
#define OBJ_PLASMA_WALL_00_WIDTH	(PIXEL * 16)	// プラズマウォール[00] の幅

//****************************************
// 列挙型の定義
//****************************************

// プラズマウォール[00] の番号
typedef enum
{
	OBJ_PLASMA_WALL_00_TOP = 0,
	OBJ_PLASMA_WALL_00_BOTTOM,
	OBJ_PLASMA_WALL_00_LEFT,
	OBJ_PLASMA_WALL_00_RIGHT,
	OBJ_PLASMA_WALL_00_MAX
}OBJ_PLASMA_WALL_00;

//****************************************
// 構造体の定義
//****************************************

// プラズマウォール[00]構造体の定義
typedef struct
{
						//========== *** 位置関連 ***
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
	float fHeight;		// 高さ
						//========== *** テクスチャ関連 ***
	int nCounterAnim;	// アニメーションのカウンター
	int nPattern;		// パターンNo.
	D3DXVECTOR3 tex;	// テクスチャ座標の開始位置(UV値)
}Obj_plasma_wall_00;

//****************************************
// プロトタイプ宣言
//****************************************
void InitObj_plasma_wall_00					(void);	// プラズマウォール[00] の初期化処理
void UninitObj_plasma_wall_00				(void);	// プラズマウォール[00] の終了処理
void UpdateObj_plasma_wall_00				(void);	// プラズマウォール[00] の更新処理
void DrawObj_plasma_wall_00					(void);	// プラズマウォール[00] の描画処理
Obj_plasma_wall_00 *GetObj_plasma_wall_00	(void);	// プラズマウォール[00] の情報を取得

#endif