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

// R.N.Lib
#include "../../Other/physics.h"

//****************************************
// マクロ定義
//****************************************
//========== *** 初期パラメーター ***
// カメラ(3D)の初期の視点
#define CAMERA3D_INIT_POS_V (D3DXVECTOR3(0.0f,0.0f,0.0f))
// カメラ(3D)の初期の注視点
#define CAMERA3D_INIT_POS_R (D3DXVECTOR3(0.0f,0.0f,0.0f))
// カメラ(3D)の初期の上方向ベクトル
#define CAMERA3D_INIT_POS_U (D3DXVECTOR3(0.0f,1.0f,0.0f))
// カメラ(3D)の初期の向き
#define CAMERA3D_INIT_ROT (D3DXVECTOR3(0.0f,0.0f,0.0f))
// カメラ(3D)の初期の距離
#define CAMERA3D_INIT_LENGTH (80.0f)
// カメラ(3D)の初期の高さ
#define CAMERA3D_INIT_HEIGHT (0.25f)

//****************************************
// 列挙型定義
//****************************************
// カメラ(3D)の状態
typedef enum {
	CAMERA3D_STATE_NONE,
	CAMERA3D_STATE_GRAB_AIR_MOUSE,	// マウスで宙を掴む
	CAMERA3D_STATE_MAX,
}CAMERA3D_STATE;

//****************************************
// 構造体定義
//****************************************
// カメラ(3D)の情報構造体
typedef struct {
	CAMERA3D_STATE state = CAMERA3D_STATE_NONE;	// 状態
	D3DXVECTOR3 posV = CAMERA3D_INIT_POS_V;	// 視点
	D3DXVECTOR3 posR = CAMERA3D_INIT_POS_R;	// 注視点
	D3DXVECTOR3 posU = CAMERA3D_INIT_POS_U;	// 上方向ベクトル
	D3DXVECTOR3 move = INITD3DXVECTOR3;		// 移動量
	D3DXVECTOR3 rot = CAMERA3D_INIT_ROT;	// 向き
	D3DXVECTOR3 spin = INITD3DXVECTOR3;		// 回転量
	float fLength = CAMERA3D_INIT_LENGTH;	// 距離
	float fHeight = CAMERA3D_INIT_HEIGHT;	// 高さ
	float fVerticalMove = 0.0f;	// 縦方向の移動量
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	float fVibration = 0.0f;	// 振動の強さ
	int nCounterVib = 0;		// 振動カウンター
	D3DXVECTOR3 vibrationPos = INITD3DXVECTOR3;	// 振動位置
	D3DXVECTOR3 forward = INITD3DXVECTOR3;		// 前方ベクトル
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
//========== *** 設定 ***
// カメラ(3D)の設定処理
void SetCamera3D(void);
// カメラ(3D)の状態設定処理
void SetCamera3DState(CAMERA3D_STATE state);
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