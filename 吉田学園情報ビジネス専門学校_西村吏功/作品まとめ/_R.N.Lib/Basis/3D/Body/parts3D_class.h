//========================================
// 
// 部品(3D)のクラス関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** parts3D_class.h ***
//========================================
#ifndef _PARTS3D_CLASS_H_	// このマクロ定義がされていなかった時
#define _PARTS3D_CLASS_H_	// 二重インクルード防止のマクロを定義する

#include "parts3D.h"
#include "motion3D.h"
#include "../../Other/physics.h"
#include "../../Calculation/matrix.h"
#include "../../Calculation/easing.h"

//****************************************
// クラス定義
//****************************************
// 部品(3D)クラス
class CParts3D {
public:
	//========== *** 構造体定義 ***
	// 部品(3D)構造体
	typedef struct {
		// 位置関連
		D3DXVECTOR3 pos = INITD3DXVECTOR3;	// 位置
		D3DXVECTOR3 rot = INITD3DXVECTOR3;	// 向き
		D3DXMATRIX  mtx = INITD3DXMATRIX;	// マトリックス(最終位置)

		//========== *** モーション関連 ***
		// 移動アニメーション
		EASE easePos          = EASE_LINEAR;		// 位置補間
		bool bPosAnim         = false;				// 位置アニメーションフラグ
		int nCounterPosAnim   = 0;					// 位置アニメーションカウンター
		int nPosAnimTime      = 0;					// 位置アニメーションにかかる時間
		D3DXVECTOR3 posOld    = INITD3DXVECTOR3;	// 元の位置
		D3DXVECTOR3 targetPos = INITD3DXVECTOR3;	// 目標位置
		// 向きアニメーション
		EASE easeRot          = EASE_LINEAR;		// 向き補間
		bool bRotAnim         = false;				// 向きアニメーションフラグ
		int nCounterRotAnim   = 0;					// 向きアニメーションカウンター
		int nRotAnimTime      = 0;					// 向きアニメーションにかかる時間
		D3DXVECTOR3 rotOld    = INITD3DXVECTOR3;	// 元の角度
		D3DXVECTOR3 targetRot = INITD3DXVECTOR3;	// 目標角度
		// 踏む
		bool bStep            = false;				// 踏むフラグ(向きアニメーション変数使用)
		int nStepTime         = 0;					// 踏む時間
	}Parts3D;

	//========== *** 関数宣言 ***
	/* コンストラクタ         */CParts3D();
	/* デストラクタ           */~CParts3D();
	/* 初期化処理             */void Init(int nModelSetUp);
	/* 終了処理               */void Uninit(void);
	/* 更新処理               */void Update(void);
	/*========================*/
	/* モーション設定処理     */void SetMotion(int nIdx)       { m_nMotion = nIdx; }
	/* モーション停止設定     */void SetMotionStop(bool bStop) { m_bMotionStop = bStop; }
	/* モーション停止設定     */void DrawCollision(bool bDraw) { m_bDrawCollision = bDraw; }
	/*========================*/
	/* 部品情報初期化         */void InitParts3D(int nParts)   { m_pParts[nParts] = {}; }
	/* 部品情報を取得         */Parts3D GetParts3D(int nParts) { return m_pParts[nParts]; }
	/* 部品情報取得           */Parts3D **GetParts3Dpp(void)   { return &m_pParts; }
	/* 現モーション情報取得   */Motion3D GetMotionInfo(void)   { return GetMotion3D()[m_nMotion]; }
	/* モーションカウント取得 */int GetMotionCounter(void)     { return m_nMotionCounter; }
	/*========================*/

private:
	//========== *** 関数宣言 ***
	/* 部品(3D)のモデル設定処理   */void ModelSetParts3D(Parts3DSet *pPartsSet);
	/* 当たり判定の設定処理       */void HitTestSet(Parts3DSet *pPartsSet, HitTest3DSet *pHitTestSet);
	/* モーションの設定処理       */void SetMotion3D(void);
	/* モーションの更新処理       */bool UpdateMotion3D(int nMotion, Parts3DSet partsSet);
	/* モーションのアニメ読込処理 */void LoadMotionAnim3D(Motion3D motion, Parts3DSet partsSet);
	/* モーションのアニメ処理     */void MotionAnim3D(Parts3DSet partsSet);

	//========== *** 変数宣言 ***
	D3DXVECTOR3 m_pos;		// 位置(本体)
	D3DXVECTOR3 m_rot;		// 向き(本体)
	bool m_bMotionStop;		// モーション停止フラグ
	bool m_bDrawCollision;	// 当たり判定描画フラグ
	int m_nModelSetUp;		// モデルのセットアップ(3D)番号
	int m_nMotion;			// モーション番号
	int m_nMotionOld;		// モーション番号(過去)
	int m_nMotionCounter;	// モーションカウンター
	Color m_col;			// 色
	Parts3D *m_pParts;		// 部品毎の情報のポインタ
};

#endif