//========================================
// 
// カメラの処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** camera.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "md_game_00.h"		// MD :ゲーム画面			[00]
#include "md_tutorial_00.h"	// MD :チュートリアル画面	[00]
#include "obj_stage_00.h"	// OBJ:ステージ				[00]

//****************************************
// マクロ定義
//****************************************
// カメラ(2D)の振動にかかる時間
// カメラ(2D)の1度の振動にかかる時間
// カメラ(2D)の振動の減衰倍率
#define CAMERA2D_VIBRATION_TIME			(20)
#define CAMERA2D_ONCE_VIBRATION_TIME	(2)
#define CAMERA2D_VIBRATION_DAMPING		(0.75f)

// カメラ(3D)の1度の振動にかかる時間
// カメラ(3D)の振動の減衰倍率
// カメラ(3D)の振動の強さの下限
#define CAMERA3D_ONCE_VIBRATION_TIME	(2)
#define CAMERA3D_VIBRATION_DAMPING		(0.75f)
#define CAMERA3D_VIBRATION_MIN			(0.01f)

// カメラ(2D)の振動をコントローラーの振動の時間に変換する際の倍率
// カメラ(2D)の振動がコントローラーの振動の最大値に達する値
// カメラ(3D)の振動をコントローラーの振動の時間に変換する際の倍率
// カメラ(3D)の振動がコントローラーの振動の最大値に達する値
#define CONTROLLER_CAMERA2D_VIBRATION_TIME_DIAMETER	(5)
#define CONTROLLER_CAMERA2D_VIBRATION_MAX_VIBRATION	(16.0f)
#define CONTROLLER_CAMERA3D_VIBRATION_TIME_DIAMETER	(5)
#define CONTROLLER_CAMERA3D_VIBRATION_MAX_VIBRATION	(4.0f)

// カメラ(3D)の初期の視点
// カメラ(3D)の初期の注視点
// カメラ(3D)の初期の上方向ベクトル
// カメラ(3D)の初期の距離高さ
// カメラ(3D)の初期の距離
// カメラ(3D)の初期の高さ
// カメラ(3D)の初期の高さの上限
// カメラ(3D)の初期の高さの下限
#define CAMERA3D_INIT_POS_V			(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CAMERA3D_INIT_POS_R			(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CAMERA3D_INIT_POS_U			(D3DXVECTOR3(0.0f,1.0f,0.0f))
#define CAMERA3D_INIT_ROT			(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CAMERA3D_INIT_LENGTH		(80.0f)
#define CAMERA3D_INIT_HEIGHT		(0.25f)
#define CAMERA3D_INIT_HEIGHT_MAX	(0.99f)
#define CAMERA3D_INIT_HEIGHT_MIN	(0.0f)

// カメラ(3D)の回転力
// カメラ(3D)のカーソルの回転力
// カメラ(3D)のスティックの回転力
// カメラ(3D)の回転の減衰値
#define CAMERA3D_ROT_FORCE				(D3DXVECTOR3(0.004f,-0.006f,0.0f))
#define CAMERA3D_ROT_FORCE_BY_CURSOR	(D3DXVECTOR3(0.002f,0.002f,0.0f))
#define CAMERA3D_ROT_FORCE_BY_STICK		(D3DXVECTOR3(0.004f,-0.006f,0.0f))
#define CAMERA3D_SPIN_DAMP				(0.9f)

//****************************************
// プロトタイプ宣言
//****************************************
// カメラ(2D)のパラメーター初期化処理
void InitParameterCamera2D(void);
// カメラ(2D)の状態処理
void StateProcessCamera2D(void);

// カメラ(2D)の初期化処理
void InitCamera2D(void);
// カメラ(2D)の終了処理
void UninitCamera2D(void);
// カメラ(2D)の更新処理
void UpdateCamera2D(void);

// カメラ(3D)の初期化処理
void InitCamera3D(void);
// カメラ(3D)の終了処理
void UninitCamera3D(void);
// カメラ(3D)の更新処理
void UpdateCamera3D(void);

// カメラ(3D)の操作処理
void ControlCamera3D(void);

// カメラ(3D)の移動処理
void MoveCamera3D(DIRECTION drct, float fMove);
// カメラ(3D)の軸回転処理
void AxisRotationCamera3D(DIRECTION drct, float fRot);
// カメラ(3D)の回転処理
void RotationCamera3D(DIRECTION drct, float fRot);

//****************************************
// グローバル変数
//****************************************
Camera2D g_camera2D;	// カメラ(2D)の情報
Camera3D g_camera3D;	// カメラ(3D)の情報

//========== *** カメラ2Dの情報を取得 ***
Camera2D *GetCamera2D(void)
{
	return &g_camera2D;
}

//========== *** カメラ3Dの情報を取得 ***
Camera3D *GetCamera3D(void)
{
	return &g_camera3D;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterCamera2D関数 - カメラ(2D)のパラメーター初期化処理 -
//========================================
void InitParameterCamera2D(void)
{
	g_camera2D.bTemp = false;	// フラグ保存
	g_camera2D.bMove = false;	// 移動フラグ
}

//========================================
// StateProcessCamera2D関数 - カメラ(2D)の状態処理 -
//========================================
void StateProcessCamera2D(void)
{
	for (int nCntState = 0; nCntState < CAMERA_STATE_MAX; nCntState++)
	{
		if (g_camera2D.aState[nCntState])
		{// その状態が真の時、
			switch (nCntState)
			{
				//========== *** 振動 ***
			case CAMERA_STATE_VIBRATION:

				if (g_camera2D.aCounterState[CAMERA_STATE_VIBRATION] < CAMERA2D_VIBRATION_TIME)
				{// 振動カウンターが振動にかかる時間を下回っている時、
					// 振動カウンターを加算
					g_camera2D.aCounterState[CAMERA_STATE_VIBRATION]++;

					if ((g_camera2D.aCounterState[CAMERA_STATE_VIBRATION] % CAMERA2D_ONCE_VIBRATION_TIME) == 0)
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
					g_camera2D.aState[CAMERA_STATE_VIBRATION] = false;
				}

				break;
			}
		}
	}
}

//========================================
// InitCamera2D関数 - カメラ(2D)の初期化処理 -
//========================================
void InitCamera2D(void)
{

}

//========================================
// UninitCamera2D関数 - カメラ(2D)の終了処理 -
//========================================
void UninitCamera2D(void)
{

}

//========================================
// UpdateCamera2D関数 - カメラ(2D)の更新処理 -
//========================================
void UpdateCamera2D(void)
{
	
}

//========================================
// InitCamera3D関数 - カメラ(3D)の初期化処理 -
//========================================
void InitCamera3D(void)
{
	// パラメーターの初期化処理
	InitParameterCamera3D();
}

//========================================
// UninitCamera3D関数 - カメラ(3D)の終了処理 -
//========================================
void UninitCamera3D(void)
{

}

//========================================
// UpdateCamera3D関数 - カメラ(3D)の更新処理 -
//========================================
void UpdateCamera3D(void)
{
	switch (GetMode())
	{
	case /*/ ゲーム画面[00] /*/MODE_GAME_00: {
		switch (GetMd_game_00()->state)
		{
		case /*/ 通常 /*/MD_GAME_00_STATE_NORMAL:
		case /*/ クリア /*/MD_GAME_00_STATE_CLEAR: {
			ControlCamera3D();	// カメラ操作処理
			break;
		}
		case /*/ チュートリアル[00] /*/MD_GAME_00_STATE_TUTORIAL_00: {
			// 視点の位置を設定
			g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
			g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
			g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));

			break;
		}
		case /*/ チュートリアル[01] /*/MD_GAME_00_STATE_TUTORIAL_01: {
			float	fRate	// 割合 
					= (float)GetMd_game_00()->nCounterState / (float)MD_GAME_00_STATE_TUTORIAL_01_TIME;

			g_camera3D.rot.y =	// 向きYを設定
				(MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_START_ROT_Y * (1.0f - fRate)) +
				(MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_END_ROT_Y * fRate);

			// 視点の位置を設定
			g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
			g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
			g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));

			break;
		}
		case /*/ コア破壊ムービー(拡大) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM: {
			float	fRate	// 割合 
					= (float)GetMd_game_00()->nCounterState / (float)MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_TIME;

			g_camera3D.fLength =	// 距離を設定
				(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_START_LENGTH * (1.0f - fRate)) +
				(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_END_LENGTH * fRate);

			g_camera3D.rot.y =	// 向きYを設定
				(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_START_ROT_Y * (1.0f - fRate)) +
				(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_END_ROT_Y * fRate);
			
			// 視点の位置を設定
			g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
			g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
			g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));

			break;
		}
		}
		break;
	}
	case /*/ タイトル画面[00] /*/MODE_TITLE_00: {
		// 視点の位置を設定
		g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
		g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
		g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));

		break;
	}
	case /*/ チュートリアル画面[00] /*/MODE_TUTORIAL_00: {
		switch (GetMd_tutorial_00()->state)
		{
		case /*/ 通常 /*/MD_TUTORIAL_00_STATE_NORMAL: {
			ControlCamera3D();	// カメラ操作処理
			break;
		}
		case /*/ チュートリアル[00] /*/MD_TUTORIAL_00_STATE_TUTORIAL_00: {
			// 視点の位置を設定
			g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
			g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
			g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
		}
		}
	}
	}

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

//========================================
// ControlCamera3D関数 - カメラ(3D)の操作処理 -
//========================================
void ControlCamera3D(void) 
{
	// カーソルの移動量に応じて回転
	AxisRotationCamera3D(DIRECTION_UP, GetCursorMove().y * CAMERA3D_ROT_FORCE_BY_CURSOR.x);
	AxisRotationCamera3D(DIRECTION_RIGHT, GetCursorMove().x * CAMERA3D_ROT_FORCE_BY_CURSOR.y);

	if (GetStick().aTplDiameter[STICK_TYPE_RIGHT] > 0.0f)
	{// 右スティックが倒されている時、
	 // スティックの角度/傾きに応じて回転
		AxisRotationCamera3D(DIRECTION_UP, (cosf(GetStick().aAngle[STICK_TYPE_RIGHT]) * GetStick().aTplDiameter[STICK_TYPE_RIGHT]) * CAMERA3D_ROT_FORCE_BY_STICK.x);
		AxisRotationCamera3D(DIRECTION_LEFT, (sinf(GetStick().aAngle[STICK_TYPE_RIGHT]) * GetStick().aTplDiameter[STICK_TYPE_RIGHT]) * CAMERA3D_ROT_FORCE_BY_STICK.y);
	}
	else
	{// いずれも該当しない時、
		if (GetKeyboardPress(DIK_UP))		{ AxisRotationCamera3D(DIRECTION_UP, CAMERA3D_ROT_FORCE.x); }	// 上軸回転
		if (GetKeyboardPress(DIK_DOWN))		{ AxisRotationCamera3D(DIRECTION_DOWN, CAMERA3D_ROT_FORCE.x); }	// 下軸回転
		if (GetKeyboardPress(DIK_LEFT))		{ AxisRotationCamera3D(DIRECTION_LEFT, CAMERA3D_ROT_FORCE.y); }	// 左軸回転
		if (GetKeyboardPress(DIK_RIGHT))	{ AxisRotationCamera3D(DIRECTION_RIGHT, CAMERA3D_ROT_FORCE.y); }	// 右軸回転
	}

	// 減衰値
	float fDamp = Prus(CAMERA3D_SPIN_DAMP - (fabsf(GetCursorMove().x) + fabsf(GetCursorMove().y)));

	g_camera3D.rot += g_camera3D.spin;				// 向きを更新
	g_camera3D.spin *= fDamp;						// 回転量を減衰
	g_camera3D.fHeight += g_camera3D.fVerticalMove;	// 高さを更新
	g_camera3D.fVerticalMove *= fDamp;				// 縦方向の移動量を減衰

	// 向きを制御
	RotControl(&g_camera3D.rot);

	// 高さを制御
	FloatControl(&g_camera3D.fHeight, CAMERA3D_INIT_HEIGHT_MAX, CAMERA3D_INIT_HEIGHT_MIN);

	// 視点の位置を設定
	g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));

	HitTest hitTest;
	hitTest.nHitTestPartsNum = 1;
	hitTest.aHitTestParts[0] = {};
	hitTest.aHitTestParts[0].hitTestForm = HIT_TEST_FORM_POINT_NOSLIP;
	hitTest.aHitTestParts[0].fWidth = 0.0f;
	hitTest.aHitTestParts[0].fDepth = 0.0f;

	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&g_camera3D.posV,
		g_camera3D.posR,
		NULL,
		NULL,
		INITD3DXVECTOR3,
		hitTest
	};

	Collision collision = {};

	// 衝突判定
	/*/ OBJ:ステージ	[00] /*/CollisionObj_stage_00(VECTOR_NONE, &collision, &collision, myCollInfo);
}

//========================================
// MoveCamera3D関数 - カメラ(3D)の移動処理 -
//========================================
void MoveCamera3D(DIRECTION drct, float fMove)
{
	// 方向に応じて視点位置を更新
	switch (drct)
	{
	case DIRECTION_UP		:g_camera3D.posV.z += fMove; break;	// 上
	case DIRECTION_DOWN		:g_camera3D.posV.z -= fMove; break;	// 下
	case DIRECTION_LEFT		:g_camera3D.posV.x += fMove; break;	// 左
	case DIRECTION_RIGHT	:g_camera3D.posV.x -= fMove; break;	// 右
	}

	// 注視点の位置を設定
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//========================================
// AxisRotationCamera3D関数 - カメラ(3D)の軸回転処理 -
//========================================
void AxisRotationCamera3D(DIRECTION drct, float fRot) 
{
	// 方向に応じて向きの移動量を更新
	switch (drct)
	{
	case DIRECTION_UP		:g_camera3D.fVerticalMove	+= fRot; break;
	case DIRECTION_DOWN		:g_camera3D.fVerticalMove	-= fRot; break;
	case DIRECTION_LEFT		:g_camera3D.spin.y			-= fRot; break;
	case DIRECTION_RIGHT	:g_camera3D.spin.y			+= fRot; break;
	}
}

//========================================
// RotationCamera3D関数 - カメラ(3D)の回転処理 -
//========================================
void RotationCamera3D(DIRECTION drct, float fRot)
{
	// 方向に応じて向きを更新
	switch (drct)
	{
	case DIRECTION_UP		:g_camera3D.fHeight	+= fRot; break;
	case DIRECTION_DOWN		:g_camera3D.fHeight	-= fRot; break;
	case DIRECTION_LEFT		:g_camera3D.rot.y	-= fRot; break;
	case DIRECTION_RIGHT	:g_camera3D.rot.y	+= fRot; break;
	}

	// 向きを制御
	RotControl(&g_camera3D.rot);

	// 高さを制御
	FloatControl(&g_camera3D.fHeight, CAMERA3D_INIT_HEIGHT_MAX, CAMERA3D_INIT_HEIGHT_MIN);

	// 注視点の位置を設定
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitCamera関数 - カメラの初期化処理 -
//========================================
void InitCamera(void)
{
	InitCamera2D();	// カメラ(2D)の初期化処理
	InitCamera3D();	// カメラ(3D)の初期化処理
}

//========================================
// UninitCamera関数 - カメラの終了処理 -
//========================================
void UninitCamera(void)
{
	UninitCamera2D();	// カメラ(2D)の終了処理
	UninitCamera3D();	// カメラ(3D)の終了処理
}

//========================================
// UpdateCamera関数 - カメラの更新処理 -
//========================================
void UpdateCamera(void)
{
	UpdateCamera2D();	// カメラ(2D)の更新処理
	UpdateCamera3D();	// カメラ(3D)の更新処理
}

//========================================
// MatchCamera2DPosition関数 - 描画位置をカメラ(2D)に合わせる -
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
//========================================
void SetCamera2DVibration(float fVibration)
{
	// 状態[振動]を真にする
	g_camera2D.aState[CAMERA_STATE_VIBRATION] = true;
	g_camera2D.aCounterState[CAMERA_STATE_VIBRATION] = 0;

	// 振動の強さを設定
	g_camera2D.fVibration += fVibration;

	// コントローラーの振動を設定
	SetVibration(
		fVibration / CONTROLLER_CAMERA2D_VIBRATION_MAX_VIBRATION, 
		fVibration * CONTROLLER_CAMERA2D_VIBRATION_TIME_DIAMETER);
}

//========================================
// InitParameterCamera3D関数 - カメラ(3D)のパラメーター初期化処理 -
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

//========================================
// SetCamera3D関数 - カメラ(3D)の設定処理 -
//========================================
void SetCamera3D(void) 
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// プロジェクションマトリックスの初期化処理
	D3DXMatrixIdentity(&g_camera3D.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera3D.mtxProjection, D3DXToRadian(45.0f), (float)BUFFER_WIDTH / (float)BUFFER_HEIGHT, 10.0f, 4000.0f);

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
//========================================
void SetCamera3DPosR(D3DXVECTOR3 pos)
{
	// 注視点の位置を代入
	g_camera3D.posR = pos;
}

//========================================
// SetCamera3DVibration関数 - カメラ(3D)の振動を設定 -
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