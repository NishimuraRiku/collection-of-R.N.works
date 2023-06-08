//========================================
// 
// 幾何学関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** geometry.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// FindDistance関数 - 2点の距離を求める -
// Author:RIKU NISHIMURA
//========================================
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2) {
	float x = pos1.x - pos2.x;	// 平方根のX辺
	float y = pos1.y - pos2.y;	// 平方根のY辺
	float z = pos1.z - pos2.z;	// 平方根のZ辺

	// 2点の距離を返す
	return sqrtf((x * x) + (y * y) + (z * z));
}

//========================================
// FindDistanceLookDown関数 - 2点の距離を求める(見下ろし基準) -
// Author:RIKU NISHIMURA
//========================================
float FindDistanceLookDown(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2) {
	float x = pos1.x - pos2.x;	// 平方根のX辺
	float z = pos1.z - pos2.z;	// 平方根のZ辺

	// 2点の距離を返す
	return sqrtf((x * x) + (z * z));
}

//========================================
// FindAngle関数 - 2点の角度を求める -
// Author:RIKU NISHIMURA
//========================================
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos) {
	float fAngle;	// 角度

	fAngle = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);
	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1;

	return fAngle;
}

//========================================
// FindAngleLookDown関数 - 2点の角度を求める(見下ろし基準) -
// Author:RIKU NISHIMURA
//========================================
float FindAngleLookDown(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos) {
	float fAngle;	// 角度

	fAngle = atan2f(targetPos.z - pos.z, targetPos.x - pos.x);
	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1;

	return fAngle;
}

//========================================
// FindRot関数 - 2点の向きを求める -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 FindRot(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos) {
	D3DXVECTOR3 direction = targetPos - pos;
	D3DXVec3Normalize(&direction, &direction);
	return direction;
}

//========================================
// GetDirectionMatrix関数 - 2点の向きマトリックスを求める -
// Author:RIKU NISHIMURA
//========================================
D3DXMATRIX FindRotMtx(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos) {
	D3DXVECTOR3 direction = targetPos - pos;
	D3DXMATRIX matrix;
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matrix, &pos, &targetPos, &nor);
	return matrix;
}

//========================================
// FindIntersectionPointLookDown関数 - 交点を求める(見下ろし) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 FindIntersectionPointLookDown(D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2)
{
	D3DXVECTOR3 vB1B2 = posB2 - posB1;	// ベクトルB1⇒B2
	D3DXVECTOR3 vA1A2 = posA2 - posA1;	// ベクトルA1⇒A2
	D3DXVECTOR3 vA2B1 = posB1 - posA2;	// ベクトルA2⇒B1
	D3DXVECTOR3 vB1A1 = posA1 - posB1;	// ベクトルB1⇒A1

										// 面積S1
	float fAreaS1 = ((vB1B2.x * vB1A1.z) - (vB1B2.z * vB1A1.x)) * 0.5f;
	// 面積S2
	float fAreaS2 = ((vB1B2.x * vA2B1.z) - (vB1B2.z * vA2B1.x)) * 0.5f;

	// 面積S1の割合
	float fRate = (fAreaS1 / (fAreaS1 + fAreaS2));

	// 交点の座標を返す
	return	D3DXVECTOR3(
		posA1.x + (vA1A2.x * fRate),
		0.0f,
		posA1.z + (vA1A2.z * fRate));
}

//========================================
// AngleDifference関数 - 角度の差を求める -
// Author:RIKU NISHIMURA
//========================================
float AngleDifference(float fAngle, float fTargetAngle)
{
	float fAngleDifference = 0.0f;	// 角度の差

	// 角度を制御する
	ControlAngle(&fAngle);

	if ((fAngle >= 0.0f) && (fTargetAngle >= 0.0f))
	{// どちらの角度もプラスの時、
		// 角度の差を設定
		fAngleDifference = fTargetAngle - fAngle;

		return fAngleDifference;
	}
	else if ((fAngle <= 0.0f) && (fTargetAngle <= 0.0f))
	{// どちらの角度もマイナスの時、
		// 角度の差を設定
		fAngleDifference = fTargetAngle - fAngle;

		return fAngleDifference;
	}

	if ((fAngle >= 0.0f) && (fTargetAngle <= 0.0f))
	{// 角度がプラスで、目標角度がマイナスの時、
		if (fAngle - D3DX_PI <= fTargetAngle)
		{// 目標角度が下側に近い時、
			// 角度の差を設定
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle - D3DX_PI >= fTargetAngle)
		{// 目標角度が上側に近い時、
			// 角度の差を設定
			fAngleDifference = (D3DX_PI - fAngle) + (D3DX_PI + fTargetAngle);
		}
	}

	if ((fAngle <= 0.0f) && (fTargetAngle >= 0.0f))
	{// 角度がマイナスで、目標角度がプラスの時、
		if (fAngle + D3DX_PI >= fTargetAngle)
		{// 目標角度が下側に近い時、
			// 角度の差を設定
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle + D3DX_PI <= fTargetAngle)
		{// 目標角度が上側に近い時、
			// 角度の差を設定
			fAngleDifference = -(D3DX_PI + fAngle) - (D3DX_PI - fTargetAngle);
		}
	}

	return fAngleDifference;
}

//========================================
// ControlAngle関数 - 角度を制御する -
// Author:RIKU NISHIMURA
//========================================
void ControlAngle(float *pAngle)
{
	if (*pAngle > D3DX_PI)
	{// プラスの円周率を上回っている時、
		*pAngle = (D3DX_PI - (*pAngle - D3DX_PI)) * -1;
	}
	else if (*pAngle < -D3DX_PI)
	{// マイナスの円周率を下回っている時、
		*pAngle = (D3DX_PI + (*pAngle + D3DX_PI));
	}
}

//========================================
// ConvertAngleToDirection関数 - 角度を指定された分割数の方向に変換する -
// Author:RIKU NISHIMURA
//========================================
float ConvertAngleToDirection(float fAngle, int nDiv)
{
	float fRange = ((D3DX_PI * 2) / nDiv);	// 方向毎の角度幅

	for (int nCntDirection = 0; nCntDirection < nDiv + 1; nCntDirection++)
	{
		if ((fAngle >= -D3DX_PI - (fRange * 0.5f) + (fRange * nCntDirection))
			&&
			(fAngle <= -D3DX_PI + (fRange * 0.5f) + (fRange * nCntDirection)))
		{// カウントの方向の角度幅の内側の時、角度を設定
			fAngle = -D3DX_PI + (fRange * nCntDirection);
			break;	// 繰り返し処理を抜ける
		}
	}

	return fAngle;
}

//========================================
// ConvPosLookDown関数 - 位置を見下ろし基準に変換する(Z⇔Y) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvPosLookDown(D3DXVECTOR3 pos)
{
	float fYTemp = pos.y;
	pos.y = -pos.z;
	pos.z = fYTemp;

	return pos;
}

//========================================
// ConvRotLookDown関数 - 向きを見下ろし基準に変換する(Z⇔Y) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvRotLookDown(D3DXVECTOR3 rot)
{
	float fYTemp = rot.y;
	rot.y = -rot.z;
	rot.y *= -1;
	rot.y -= D3DX_PI;
	rot.z = fYTemp;
	rot.z += D3DX_PI;
	rot.z *= -1;

	return rot;
}

//========================================
// RotControl関数 - 向きを制御する -
// Author:RIKU NISHIMURA
//========================================
void RotControl(D3DXVECTOR3 *pRot)
{
	FloatLoopControl(&pRot->x, D3DX_PI, -D3DX_PI);
	FloatLoopControl(&pRot->y, D3DX_PI, -D3DX_PI);
	FloatLoopControl(&pRot->z, D3DX_PI, -D3DX_PI);
}

//========================================
// FindNormalVector関数 - 向きを元に法線ベクトルを求める -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 FindNormalVector(D3DXVECTOR3 rot) {
	// 各軸周りの回転を行う回転行列を作成
	float sx = sinf(rot.x);
	float cx = cosf(rot.x);
	float sy = sinf(rot.y);
	float cy = cosf(rot.y);
	float sz = sinf(rot.z);
	float cz = cosf(rot.z);

	// 各軸周りの回転行列をかけて、法線ベクトルを求める
	return D3DXVECTOR3(
		cy * cz,
		(-cx * sz) + (sx * sy * cz),
		(sx * sz) + (cx * sy * cz));
}

//========================================
// FindDistanceToCameraPlane関数 - カメラ面までの垂直距離を求める -
// Author:RIKU NISHIMURA
//========================================
float FindDistanceToCameraPlane(D3DXVECTOR3 pos) {
	D3DXVECTOR3 cameraToPosition = pos - GetCamera3D()->posV;
	D3DXVECTOR3 cameraPlaneNormal = GetCamera3D()->forward;

	D3DXVec3Normalize(&cameraPlaneNormal, &cameraPlaneNormal);

	float distance = D3DXVec3Dot(&cameraToPosition, &cameraPlaneNormal);

	return fabs(distance);
}

//========================================
// 描画位置を基準に合わせる
// Author:RIKU NISHIMURA
//========================================
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos, float fScWidth, float fScHeight)
{
	// 位置を基準の位置に合わせる
	pDrawPos->x = pDrawPos->x - basePos.x + (fScWidth * 0.5f);
	pDrawPos->y = pDrawPos->y - basePos.y + (fScHeight * 0.5f);
}