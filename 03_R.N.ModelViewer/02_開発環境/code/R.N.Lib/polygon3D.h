//========================================
// 
// ポリゴン(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon3D.h ***
//========================================
#ifndef _POLYGON3D_H_	// このマクロ定義がされていなかった時
#define _POLYGON3D_H_	// 二重インクルード防止のマクロを定義する

#include <stdio.h>
// _R.N.Lib
#include "RNmain.h"

//****************************************
// マクロ定義
//****************************************
// マテリアル(3D)の上限
#define MATERIAL_3D_MAX (10)
// 部品(3D)の最大数
#define PARTS_3D_MAX (8)
// モーション(3D)の最大数
#define MOTION_3D_MAX (16)
// モーション(3D)の位置のアニメーション情報の最大数
#define MOTION_3D_POSITION_ANIM_MAX (8)
// モーション(3D)の角度のアニメーション情報の最大数
#define MOTION_3D_ROTATION_ANIM_MAX	(8)
// モーション(3D)のサウンのアニメーション情報の最大数
#define MOTION_3D_SOUND_ANIM_MAX (8)

//****************************************
// 構造体の定義
//****************************************
//========== *** 部品(3D)関連 ***
// 部品(3D)構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 posResult;	// 位置(結果)
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rotResult;	// 向き(結果)
	D3DXVECTOR3 rot;		// 向き

	// モーション関連
	bool bPosAnim;			// 位置アニメーションフラグ
	int nCounterPosAnim;	// 位置アニメーションカウンター
	int nPosAnimTime;		// 位置アニメーションにかかる時間
	D3DXVECTOR3 posOld;		// 元の位置
	D3DXVECTOR3 targetPos;	// 目標位置
	bool bRotAnim;			// 角度アニメーションフラグ
	int nCounterRotAnim;	// 角度アニメーションカウンター
	int nRotAnimTime;		// 角度アニメーションにかかる時間
	D3DXVECTOR3 rotOld;		// 元の角度
	D3DXVECTOR3 targetRot;	// 目標角度
}Parts3D;
// 部品(3D)管理構造体
typedef struct
{
	int nMotion;		// モーション番号
	int nCounterMotion;	// モーションカウンター
	// 部品情報
	Parts3D aParts[PARTS_3D_MAX];
}Parts3DInfo;
// 部品(3D)の種類毎の情報構造体
typedef struct
{
	// 読み込み情報
	char aModelPath[TXT_MAX];		// モデルの相対パス
	int nParent;					// 親番号
	D3DXVECTOR3 fixedRelativePos;	// 固定の相対位置
	D3DXVECTOR3 fixedRelativeRot;	// 固定の相対角度
	D3DXVECTOR3 rotRelativePos;		// 回転軸の相対位置
}Parts3DType;
// 部品設定情報構造体
typedef struct
{
	int nPartsNum;	// 部品数
	// 部品の種類毎の情報
	Parts3DType aPartsType[PARTS_3D_MAX];
}Parts3DSet;
//========== *** モーション関連(3D) ***
// 位置のアニメーション(3D)情報構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int nMoveTime;		// 到達にかかる時間
	int nTime;			// 切り替え時間
}PositionAnim3D;
// 角度のアニメーション(3D)情報構造体
typedef struct
{
	D3DXVECTOR3 rot;		// 角度
	int			nMoveTime;	// 到達にかかる時間
	int			nTime;		// 切り替え時間
}RotationAnim3D;
// サウンドのアニメーション(3D)情報構造体
typedef struct
{
	int	nSound;	// サウンド番号
	int	nTime;	// 切り替え時間
}SoundAnim3D;
// 部品毎のモーション(3D)情報構造体
typedef struct
{
	int nPosAnimNum;	// 位置(相対)のアニメーション数
	// 位置(相対)のアニメーション情報
	PositionAnim3D aPosAnim[MOTION_3D_POSITION_ANIM_MAX];
		
	int nRotAnimNum;	// 角度(相対)のアニメーション数
	// 角度(相対)のアニメーション情報
	RotationAnim3D aRotAnim[MOTION_3D_ROTATION_ANIM_MAX];
}PartsMotion3D;
// モーション(3D)情報構造体
typedef struct
{
	int nLoopTime;	// ループ時間
	bool bLoop;		// ループするかフラグ
	// 部品毎のモーション情報
	PartsMotion3D aPartsMotion[PARTS_3D_MAX];
					
	int nSoundAnimNum;	// サウンドのアニメーション数
	// サウンドのアニメーション情報
	SoundAnim3D aSoundAnim[MOTION_3D_SOUND_ANIM_MAX];
}Motion3D;
// モーション(3D)設定情報構造体
typedef struct
{
	int nMotionNum = 1;					// モーション数
	Motion3D aMotion[MOTION_3D_MAX];	// モーションの情報
}Motion3DSet;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 部品(3D)関連 ***
// 部品設定情報(3D)の読み込み処理
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);
// 部品(3D)の初期化処理
void InitParts3DInfo(Parts3DInfo *pPartsInfo);
// 部品(3D)のトランスフォームの初期化処理
void InitParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
// 部品(3D)のトランスフォームの取得処理
void GetParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
// 部品(3D)管理情報のパラメーター初期値を返す
Parts3DInfo InitParameterParts3DInfo(void);
// 部品(3D)のマトリックスを取得
D3DXMATRIX GetMatrixParts3D(int nParts, D3DXVECTOR3 pos, D3DXVECTOR3 rot, Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
//========== *** モーション(3D)関連 ***
// モーション(3D)設定情報の読み込み処理
void LoadMotion3DSet(FILE *pFile, Motion3DSet *pMotionSet);
// モーション(3D)の更新処理
bool UpdateMotion3D(int nMotion, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);
// モーション(3D)の設定処理
void SetMotion3D(Parts3DInfo *pPartsInfo, int nMotion);
//========== *** 設定 ***
// 頂点座標の設定処理(3D)
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight);
// 法線ベクトルの設定処理(3D)
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
// 頂点色の設定処理(3D)
void SetVertexColor3D(VERTEX_3D *pVtx, Color col);
// テクスチャ座標の設定処理(3D)
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
// マテリアルの設定
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col);

#endif