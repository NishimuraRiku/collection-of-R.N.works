//========================================
// 
// カメラ(3D)の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** camera3D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// マクロ定義
//****************************************
//========== *** 調整パラメーター ***
// カメラ(3D)の1度の振動にかかる時間
#define CAMERA3D_ONCE_VIBRATION_TIME (2)
// カメラ(3D)の振動の減衰倍率
#define CAMERA3D_VIBRATION_DAMPING (0.75f)
// カメラ(3D)の振動の強さの下限
#define CAMERA3D_VIBRATION_MIN (0.01f)
// カメラ(3D)の振動をコントローラーの振動の時間に変換する際の倍率
#define CONTROLLER_CAMERA3D_VIBRATION_TIME_DIAMETER	(5)
// カメラ(3D)の振動がコントローラーの振動の最大値に達する値
#define CONTROLLER_CAMERA3D_VIBRATION_MAX_VIBRATION	(4.0f)
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
// グローバル変数
//****************************************
static Camera3D g_camera3D;	// カメラ(3D)の情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// InitCamera3D関数 - カメラ(3D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitCamera3D(void)
{
	// パラメーターの初期化処理
	InitParameterCamera3D();
}

//========================================
// UninitCamera3D関数 - カメラ(3D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitCamera3D(void)
{
	
}

//========================================
// UpdateCamera3D関数 - カメラ(3D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateCamera3D(void)
{
	if (g_camera3D.fVibration >= CAMERA3D_VIBRATION_MIN)
	{// 振動の強さが振動の強さの下限以上の時、
		if (--g_camera3D.nCounterVib <= 0)
		{// 振動カウンターを減算した結果0以下の時、
			g_camera3D.nCounterVib = CAMERA3D_ONCE_VIBRATION_TIME;	// 振動カウンターを設定
			float fAngle = -D3DX_PI + fRand(D3DX_PI * 2);			// 向きを乱数で設定
			g_camera3D.vibrationPos.x = sinf(fAngle) * g_camera3D.fVibration;
			g_camera3D.vibrationPos.y = cosf(fAngle) * g_camera3D.fVibration;
			g_camera3D.fVibration *= CAMERA3D_VIBRATION_DAMPING;	// 振動の強さを減衰
		}
	}
	else
	{// 振動の強さが振動の強さの下限未満の時、振動位置を初期化
		g_camera3D.vibrationPos = INITD3DXVECTOR3;
	}
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// GetCamera3D関数 - カメラ3Dの情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Camera3D *GetCamera3D(void)
{
	return &g_camera3D;
}

//============================================================
//--------------------| *** 代入 *** |------------------------
//============================================================
//========================================
// InitParameterCamera3D関数 - カメラ(3D)のパラメーター初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterCamera3D(void)
{
	g_camera3D.posV				= CAMERA3D_INIT_POS_V;	// 視点
	g_camera3D.posR				= CAMERA3D_INIT_POS_R;	// 注視点
	g_camera3D.posU				= CAMERA3D_INIT_POS_U;	// 上方向ベクトル
	g_camera3D.move				= INITD3DXVECTOR3;		// 移動量
	g_camera3D.rot				= CAMERA3D_INIT_ROT;	// 向き
	g_camera3D.spin				= INITD3DXVECTOR3;		// 回転量
	g_camera3D.fLength			= CAMERA3D_INIT_LENGTH;	// 距離
	g_camera3D.fHeight			= CAMERA3D_INIT_HEIGHT;	// 高さ
	g_camera3D.fVerticalMove	= 0.0f;					// 縦方向の移動量
	g_camera3D.fVibration		= 0.0f;					// 振動の強さ
	g_camera3D.nCounterVib		= 0;					// 振動カウンター
	g_camera3D.vibrationPos		= INITD3DXVECTOR3;		// 振動位置
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// SetCamera3D関数 - カメラ(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3D(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	// プロジェクションマトリックスの初期化処理
	D3DXMatrixIdentity(&g_camera3D.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera3D.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 4000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera3D.mtxProjection);

	// ビューマトリックスの設定
	D3DXMatrixIdentity(&g_camera3D.mtxView);

	{
		D3DXVECTOR3 posV = g_camera3D.posV;	// 視点
		D3DXVECTOR3 posR = g_camera3D.posR;	// 注視点

		if (g_camera3D.vibrationPos != INITD3DXVECTOR3) 
		{// 振動位置に変動がある時、
			float fAngle = FindAngleLookDown(g_camera3D.posV, g_camera3D.posR) - (D3DX_PI * 0.5f);
			D3DXVECTOR3 addPos;	// 加算位置を振動位置に応じて設定
			addPos.x = sinf(fAngle) * g_camera3D.vibrationPos.x;
			addPos.z = cosf(fAngle) * g_camera3D.vibrationPos.x;
			addPos.y = g_camera3D.vibrationPos.y;

			// 視点/注視点に加算位置を適用
			posV += addPos;
			posR += addPos;
		}
		
		// ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_camera3D.mtxView, &posV, &posR, &g_camera3D.posU);
	}
	
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera3D.mtxView);
}

//========================================
// SetCamera3DPosV関数 - カメラ(3D)の視点の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3DPosV(D3DXVECTOR3 pos) 
{
	// 視点の位置を代入
	g_camera3D.posV = pos;

	// 注視点の位置を設定
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//========================================
// SetCamera3DPosR関数 - カメラ(3D)の注視点の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3DPosR(D3DXVECTOR3 pos)
{
	// 注視点の位置を代入
	g_camera3D.posR = pos;

	// 視点の位置を設定
	g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//========================================
// SetCamera3DVibration関数 - カメラ(3D)の振動を設定 -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3DVibration(float fVibration)
{
	// 振動の強さを設定
	g_camera3D.fVibration += fVibration;

	// 振動カウンターを初期化
	g_camera3D.nCounterVib = 0;

	// コントローラーの振動を設定
	SetVibration(
		fVibration / CONTROLLER_CAMERA3D_VIBRATION_MAX_VIBRATION,
		fVibration * CONTROLLER_CAMERA3D_VIBRATION_TIME_DIAMETER);
}

//============================================================
//--------------------| *** 操作 *** |------------------------
//============================================================
//========================================
// MoveCamera3D関数 - カメラ(3D)の移動処理 -
// Author:RIKU NISHIMURA
//========================================
void MoveCamera3D(DIRECTION drct, float fMove)
{
	// 方向に応じて視点位置を更新
	switch (drct)
	{
	case DIRECTION_UP:g_camera3D.posV.z += fMove; break;	// 上
	case DIRECTION_DOWN:g_camera3D.posV.z -= fMove; break;	// 下
	case DIRECTION_LEFT:g_camera3D.posV.x += fMove; break;	// 左
	case DIRECTION_RIGHT:g_camera3D.posV.x -= fMove; break;	// 右
	}

	// 注視点の位置を設定
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//========================================
// AxisRotationCamera3D関数 - カメラ(3D)の軸回転処理 -
// Author:RIKU NISHIMURA
//========================================
void AxisRotationCamera3D(DIRECTION drct, float fRot)
{
	// 方向に応じて向きの移動量を更新
	switch (drct)
	{
	case DIRECTION_UP:g_camera3D.fVerticalMove += fRot; break;
	case DIRECTION_DOWN:g_camera3D.fVerticalMove -= fRot; break;
	case DIRECTION_LEFT:g_camera3D.spin.y -= fRot; break;
	case DIRECTION_RIGHT:g_camera3D.spin.y += fRot; break;
	}
}

//========================================
// RotationCamera3D関数 - カメラ(3D)の回転処理 -
// Author:RIKU NISHIMURA
//========================================
void RotationCamera3D(DIRECTION drct, float fRot)
{
	// 方向に応じて向きを更新
	switch (drct)
	{
	case DIRECTION_UP:g_camera3D.fHeight += fRot; break;
	case DIRECTION_DOWN:g_camera3D.fHeight -= fRot; break;
	case DIRECTION_LEFT:g_camera3D.rot.y -= fRot; break;
	case DIRECTION_RIGHT:g_camera3D.rot.y += fRot; break;
	}

	// 向きを制御
	RotControl(&g_camera3D.rot);

	// 注視点の位置を設定
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}
