//========================================
// 
// カメラ(3D)の操作処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** camera3D_control.cpp ***
//========================================
// R.N.Lib
#include "../../../R.N.Lib.h"

//****************************************
// マクロ定義
//****************************************
// カメラ(3D)のカーソルの回転力
#define CAMERA3D_ROT_FORCE (D3DXVECTOR3(0.004f,-0.006f,0.0f))
// カメラ(3D)の回転力
#define CAMERA3D_ROT_FORCE_BY_CURSOR (D3DXVECTOR3(0.0005f,-0.00075f,0.0f))
// カメラ(3D)の回転の減衰値
#define CAMERA3D_SPIN_DAMP (0.9f)
// カメラ(3D)の初期の高さの上限
#define CAMERA3D_HEIGHT_MAX (0.99f)
// カメラ(3D)の初期の高さの下限
#define CAMERA3D_HEIGHT_MIN (-0.99f)
// カメラ(3D)の移動力
#define CAMERA3D_MOVEFORCE (0.1f)
// カメラ(3D)のズーム力
#define CAMERA3D_ZOOMFORCE (8.0f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------| *** カメラ(3D)の状態に応じた操作処理 *** |--------
//============================================================

//========================================
// GRAB_AIR_MOUSE
//========================================
void Camera3DControl_GRAB_AIR_MOUSE(Camera3D *pCamera3D)
{
	if (ImGui_GetWindowFocused()) 
	{// ImGuiのウィンドウにフォーカスが当たっている時、
		return;	// 処理を終了する
	}

	if (GetMousePress(MOUSEBUTTON_RIGHT))
	{// マウスの右ボタンが押されている時、
		// カーソルの移動量に応じて回転
		AxisRotationCamera3D(DIRECTION_UP, GetCursorMove().y * CAMERA3D_ROT_FORCE_BY_CURSOR.x);
		AxisRotationCamera3D(DIRECTION_LEFT, GetCursorMove().x * CAMERA3D_ROT_FORCE_BY_CURSOR.y);
	}
	else
	{// いずれも該当しない時、
		if (GetKeyboardPress(DIK_UP)) { AxisRotationCamera3D(DIRECTION_UP, CAMERA3D_ROT_FORCE.x); }	// 上軸回転
		if (GetKeyboardPress(DIK_DOWN)) { AxisRotationCamera3D(DIRECTION_DOWN, CAMERA3D_ROT_FORCE.x); }	// 下軸回転
		if (GetKeyboardPress(DIK_LEFT)) { AxisRotationCamera3D(DIRECTION_LEFT, CAMERA3D_ROT_FORCE.y); }	// 左軸回転
		if (GetKeyboardPress(DIK_RIGHT)) { AxisRotationCamera3D(DIRECTION_RIGHT, CAMERA3D_ROT_FORCE.y); }	// 右軸回転
	}

	pCamera3D->rot += pCamera3D->spin;				// 向きを更新
	pCamera3D->spin *= CAMERA3D_SPIN_DAMP;			// 回転量を減衰
	pCamera3D->fHeight += pCamera3D->fVerticalMove;	// 高さを更新
	pCamera3D->fVerticalMove *= CAMERA3D_SPIN_DAMP;	// 縦方向の移動量を減衰

	// 向きを制御
	RotControl(&pCamera3D->rot);

	// 高さを制御
	FloatControl(&pCamera3D->fHeight, CAMERA3D_HEIGHT_MAX, CAMERA3D_HEIGHT_MIN);

	if (GetMousePress(MOUSEBUTTON_LEFT))
	{// マウスの左ボタンが押されている時、
		D3DXVECTOR3 move;	// 移動量
							// カーソルの移動量
		D3DXVECTOR3 cursorMove = GetCursorMove();
		// 移動向きX
		float fMoveAngleX = pCamera3D->rot.y - (D3DX_PI * 0.5f);

		// XZ
		move.x = sinf(fMoveAngleX) * cursorMove.x;
		move.z = cosf(fMoveAngleX) * cursorMove.x;
		// Y
		move.y = cursorMove.y * (1.0f - fabsf(pCamera3D->fHeight));

		// 縦方向基準でXZにYの移動量加算
		move.x += sinf(pCamera3D->rot.y) * cursorMove.y * pCamera3D->fHeight;
		move.z += cosf(pCamera3D->rot.y) * cursorMove.y * pCamera3D->fHeight;

		// 注視点に移動量を反映
		pCamera3D->posR += move * CAMERA3D_MOVEFORCE;
	}

	if (GetWheelSpin() == WHEELSPIN_FRONT)
	{
		pCamera3D->posR.x += sinf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.z += cosf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.y += CAMERA3D_ZOOMFORCE * -pCamera3D->fHeight;
	}
	else if (GetWheelSpin() == WHEELSPIN_BACK)
	{
		pCamera3D->posR.x -= sinf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.z -= cosf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.y -= CAMERA3D_ZOOMFORCE * -pCamera3D->fHeight;
	}

	// 視点の位置を設定
	pCamera3D->posV.x = pCamera3D->posR.x + (sinf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
	pCamera3D->posV.y = pCamera3D->posR.y + (pCamera3D->fLength * pCamera3D->fHeight);
	pCamera3D->posV.z = pCamera3D->posR.z + (cosf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
}