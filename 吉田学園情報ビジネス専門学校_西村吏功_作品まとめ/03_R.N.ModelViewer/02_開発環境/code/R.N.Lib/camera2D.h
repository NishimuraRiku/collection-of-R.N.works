//========================================
// 
// カメラ(2D)のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** camera2D.h ***
//========================================
#ifndef _CAMERA2D_H_	// このマクロ定義がされていなかった時
#define _CAMERA2D_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "RNmain.h"

//****************************************
// 列挙型の定義
//****************************************
// カメラの状態
typedef enum
{
	CAMERA2D_STATE_VIBRATION,	// 振動
	CAMERA2D_STATE_MAX,
}CAMERA2D_STATE;

//****************************************
// 構造体の定義
//****************************************
// カメラ(2D)の情報構造体
typedef struct
{
	//========== *** 位置関連 ***
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posOld;				// 元の位置
	D3DXVECTOR3 vibrationPos;		// 振動位置
	D3DXVECTOR3 vibrationTargetPos;	// 振動の目標位置
	D3DXVECTOR3 *pTargetPos;		// 目標位置のポインタ
	D3DXVECTOR3 targetRelativePos;	// 目標位置のポインタ

	//========== *** 状態関連 ***
	// 状態フラグ
	bool aState[CAMERA2D_STATE_MAX];
	// 状態管理カウンター
	int aCounterState[CAMERA2D_STATE_MAX];
	float fVibration;	// 振動の強さ
	bool bTemp;			// フラグ保存
	bool bMove;			// 移動フラグ
	int nCounterMove;	// 移動カウンター
}Camera2D;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// カメラ(2D)の情報を取得
Camera2D *GetCamera2D(void);
//========== *** 基本接続 ***
// カメラ(2D)の初期化処理
void InitCamera2D(void);
// カメラ(2D)の終了処理
void UninitCamera2D(void);
// カメラ(2D)の更新処理
void UpdateCamera2D(void);
//========== *** その他 ***
// 描画位置をカメラ(2D)の位置に合わせる
void MatchCamera2DPosition(D3DXVECTOR3 *pDrawPos);
// カメラ(2D)の振動を設定
void SetCamera2DVibration(float fVibration);

#endif