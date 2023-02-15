//========================================
// 
// カメラ(3D)のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** camera3D.h ***
//========================================
#ifndef _CAMERA3D_H_	// このマクロ定義がされていなかった時
#define _CAMERA3D_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "RNmain.h"
#include "physics.h"

//****************************************
// マクロ定義
//****************************************
// カメラ(3D)の拡大/縮小する長さ
#define CAMERA3D_ZOOM_LENGTH (2.0f)
// カメラ(3D)のY移動量
#define CAMERA3D_Y_MOVE (1.0f)

//****************************************
// 列挙型の定義
//****************************************
// カメラ(3D)の状態
typedef enum
{
	CAMERA3D_STATE_VIBRATION,	// 振動
	CAMERA3D_STATE_MAX,
}CAMERA3D_STATE;

//****************************************
// 構造体の定義
//****************************************
// カメラ(3D)の情報構造体
typedef struct 
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 posU;			// 上方向ベクトル
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 spin;			// 回転量
	float fLength;				// 距離
	float fHeight;				// 高さ
	float fVerticalMove;		// 縦方向の移動量
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	float fVibration;			// 振動の強さ
	int nCounterVib;			// 振動カウンター
	D3DXVECTOR3 vibrationPos;	// 振動位置
}Camera3D;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// カメラ(3D)の初期化処理
void InitCamera3D(void);
// カメラ(3D)の終了処理
void UninitCamera3D(void);
// カメラ(3D)の更新処理
void UpdateCamera3D(void);
//========== *** 取得 ***
// カメラ(3D)の情報を取得
Camera3D *GetCamera3D(void);
//========== *** 代入 ***
// カメラ(3D)のパラメーター初期化処理
void InitParameterCamera3D(void);
//========== *** 設定 ***
// カメラ(3D)の設定処理
void SetCamera3D(void);
// カメラ(3D)の視点の設定処理
void SetCamera3DPosV(D3DXVECTOR3 pos);
// カメラ(3D)の注視点の設定処理
void SetCamera3DPosR(D3DXVECTOR3 pos);
// カメラ(3D)の振動を設定
void SetCamera3DVibration(float fVibration);
//========== *** 操作 ***
// カメラ(3D)の移動処理
void MoveCamera3D(DIRECTION drct, float fMove);
// カメラ(3D)の軸回転処理
void AxisRotationCamera3D(DIRECTION drct, float fRot);
// カメラ(3D)の回転処理
void RotationCamera3D(DIRECTION drct, float fRot);

#endif