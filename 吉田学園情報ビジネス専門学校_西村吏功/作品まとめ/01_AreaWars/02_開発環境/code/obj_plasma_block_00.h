//========================================
// *** obj_plasma_block_00.h ***
//========================================
#ifndef _OBJ_PLASMA_BLOCK_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_PLASMA_BLOCK_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
#define OBJ_PLASMA_BLOCK_00_MOVE_TIME	(60)	// プラズマブロック[00] の移動にかかる時間

//****************************************
// 構造体の定義
//****************************************

// プラズマブロック[00] が描く四角形構造体の定義
typedef struct
{
	int nMoveCounter;		// 移動のカウンター

	float fTop;				// 上辺の座標
	float fBottom;			// 下辺の座標
	float fLeft;			// 左辺の座標
	float fRight;			// 右辺の座標

	float fTargetTop;		// 上辺の目標座標
	float fTargetBottom;	// 下辺の目標座標
	float fTargetLeft;		// 左辺の目標座標
	float fTargetRight;		// 右辺の目標座標

	float fMoveTop;			// 上辺の移動量
	float fMoveBottom;		// 下辺の移動量
	float fMoveLeft;		// 左辺の移動量
	float fMoveRight;		// 右辺の移動量

	D3DXVECTOR3 centerPos;	// 中心位置
}Obj_plasma_block_00_square;

//****************************************
// プロトタイプ宣言
//****************************************

// プラズマブロック[00] の初期化処理
void InitObj_plasma_block_00(void);

// プラズマブロック[00] の終了処理
void UninitObj_plasma_block_00(void);

// プラズマブロック[00] の更新処理
void UpdateObj_plasma_block_00(void);

// プラズマブロック[00] が描く四角形の更新処理
void UpdateObj_plasma_block_00_square(void);

// プラズマブロック[00] の描画処理
void DrawObj_plasma_block_00(void);

// プラズマブロック[00] が描く四角形の情報を取得
Obj_plasma_block_00_square *GetObj_plasma_block_00_square(void);

// プラズマブロック[00] の四角形の移動量を設定
void SetMoveObj_plasma_block_00(void);

#endif