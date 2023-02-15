//========================================
// 
// カメラ(2D)の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** camera2D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// マクロ定義
//****************************************
// カメラ(2D)の振動にかかる時間
#define CAMERA2D_VIBRATION_TIME (20)
// カメラ(2D)の1度の振動にかかる時間
#define CAMERA2D_ONCE_VIBRATION_TIME (2)
// カメラ(2D)の振動の減衰倍率
#define CAMERA2D_VIBRATION_DAMPING (0.75f)
// カメラ(2D)の振動をコントローラーの振動の時間に変換する際の倍率
#define CONTROLLER_CAMERA2D_VIBRATION_TIME_DIAMETER (5)
// カメラ(2D)の振動がコントローラーの振動の最大値に達する値
#define CONTROLLER_CAMERA2D_VIBRATION_MAX_VIBRATION (16.0f)

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// カメラ(2D)のパラメーター初期化処理
void InitParameterCamera2D(void);
//========== *** 更新 ***
// カメラ(2D)の状態処理
void StateProcessCamera2D(void);

//****************************************
// グローバル変数
//****************************************
static Camera2D g_camera2D;	// カメラ(2D)の情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// InitParameterCamera2D関数 - カメラ(2D)のパラメーター初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterCamera2D(void)
{
	g_camera2D.bTemp = false;	// フラグ保存
	g_camera2D.bMove = false;	// 移動フラグ
}

//============================================================
//--------------------| *** 更新 *** |------------------------
//============================================================
//========================================
// StateProcessCamera2D関数 - カメラ(2D)の状態処理 -
// Author:RIKU NISHIMURA
//========================================
void StateProcessCamera2D(void)
{
	for (int nCntState = 0; nCntState < CAMERA2D_STATE_MAX; nCntState++)
	{
		if (g_camera2D.aState[nCntState])
		{// その状態が真の時、
			switch (nCntState)
			{
			case CAMERA2D_STATE_VIBRATION:

				if (g_camera2D.aCounterState[CAMERA2D_STATE_VIBRATION] < CAMERA2D_VIBRATION_TIME)
				{// 振動カウンターが振動にかかる時間を下回っている時、
					// 振動カウンターを加算
					g_camera2D.aCounterState[CAMERA2D_STATE_VIBRATION]++;

					if ((g_camera2D.aCounterState[CAMERA2D_STATE_VIBRATION] % CAMERA2D_ONCE_VIBRATION_TIME) == 0)
					{// 振動カウンターが1度の振動にかかる時間に達した時、
						float	fAngle	// 角度を乱数で設定
								= -D3DX_PI + fRand(D3DX_PI * 2);

						// 振動の目標位置を設定
						g_camera2D.vibrationTargetPos.x = sinf(fAngle) * g_camera2D.fVibration;
						g_camera2D.vibrationTargetPos.y = cosf(fAngle) * g_camera2D.fVibration;

						// 振動の強さを減衰させる
						g_camera2D.fVibration *= CAMERA2D_VIBRATION_DAMPING;
					}
				}
				else
				{// 振動カウンターが出現にかかる時間に達した時、
					// 振動フラグを偽にする
					g_camera2D.aState[CAMERA2D_STATE_VIBRATION] = false;
				}

				break;
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// GetCamera2Ds関数 - カメラ(2D)の情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Camera2D *GetCamera2D(void)
{
	return &g_camera2D;
}

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// InitCamera2D関数 - カメラ(2D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitCamera2D(void)
{
	
}

//========================================
// UninitCamera2D関数 - カメラ(2D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitCamera2D(void)
{
	
}

//========================================
// UpdateCamera2D関数 - カメラ(2D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateCamera2D(void)
{
	
}

//============================================================
//--------------------| *** その他 *** |----------------------
//============================================================
//========================================
// MatchCamera2DPosition関数 - 描画位置をカメラ(2D)に合わせる -
// Author:RIKU NISHIMURA
//========================================
void MatchCamera2DPosition(D3DXVECTOR3 *pDrawPos)
{
	// 描画位置をカメラに合わせる
	MatchPosition(pDrawPos, g_camera2D.pos);

	// 描画位置に振動位置を更新する
	*pDrawPos += g_camera2D.vibrationPos;
}

//========================================
// SetCamera2DVibration関数 - カメラ(2D)の振動を設定 -
// Author:RIKU NISHIMURA
//========================================
void SetCamera2DVibration(float fVibration)
{
	// 振動状態を真にする
	g_camera2D.aState[CAMERA2D_STATE_VIBRATION] = true;
	g_camera2D.aCounterState[CAMERA2D_STATE_VIBRATION] = 0;

	// 振動の強さを設定
	g_camera2D.fVibration += fVibration;

	// コントローラーの振動を設定
	SetVibration(
		fVibration / CONTROLLER_CAMERA2D_VIBRATION_MAX_VIBRATION, 
		fVibration * CONTROLLER_CAMERA2D_VIBRATION_TIME_DIAMETER);
}
