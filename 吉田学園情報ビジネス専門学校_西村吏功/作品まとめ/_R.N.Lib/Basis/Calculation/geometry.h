//========================================
// 
// 幾何学関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** geometry.h ***
//========================================
#ifndef _GEOMETRY_H_	// このマクロ定義がされていなかった時
#define _GEOMETRY_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 関数宣言
//****************************************
/* 2点の距離を求める                  */float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
/* 2点の距離を求める(見下ろし基準)    */float FindDistanceLookDown(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
/* 2点の角度を求める                  */float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
/* 2点の角度を求める(見下ろし基準)    */float FindAngleLookDown(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
/* 2点の向きを求める                  */D3DXVECTOR3 FindRot(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
/* 2点の向きマトリックスを求める      */D3DXMATRIX FindRotMtx(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
/* 交点の座標を求める(見下ろし基準)   */D3DXVECTOR3 FindIntersectionPointLookDown(D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2);
/* 角度の差を求める                   */float AngleDifference(float fAngle, float fTargetAngle);
/* 角度を制御する                     */void ControlAngle(float *pAngle);
/* 角度を指定された分割数に変換する   */float ConvertAngleToDirection(float fAngle, int nDiv);
/* 位置を見下ろし基準に変換する(Z⇔Y) */D3DXVECTOR3 ConvPosLookDown(D3DXVECTOR3 pos);
/* 向きを見下ろし基準に変換する(Z⇔Y) */D3DXVECTOR3 ConvRotLookDown(D3DXVECTOR3 rot);
/* 向きを制御                         */void RotControl(D3DXVECTOR3 *pRot);
/* 向きを元に法線ベクトルを求める     */D3DXVECTOR3 FindNormalVector(D3DXVECTOR3 rot);
/* カメラ面までの垂直距離を求める     */float FindDistanceToCameraPlane(D3DXVECTOR3 pos);
/* 描画位置を基準に合わせる           */void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos, float fScWidth, float fScHeight);

#endif