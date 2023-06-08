//========================================
// 
// 部品(3D)のクラス関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** parts3D_class.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CParts3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CParts3D::CParts3D(void) {
	m_pos				= INITD3DXVECTOR3;	// 本体位置
	m_rot				= INITD3DXVECTOR3;	// 本体向き
	m_bMotionStop		= false;			// 停止フラグ
	m_bDrawCollision	= false;			// 当たり判定描画フラグ
	m_nModelSetUp		= 0;				// モデルのセットアップ(3D)番号を設定
	m_nMotion			= 0;				// モーション番号
	m_nMotionOld		= 0;				// モーション番号(過去)
	m_nMotionCounter	= 0;				// モーションカウンター
	m_col				= CLEARCOLOR;		// 色
	m_pParts			= NULL;				// 部品毎の情報のポインタ
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CParts3D::~CParts3D(void) {

}

//========================================
// ModelSetParts3D関数 - 部品(3D)のモデル設定処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::ModelSetParts3D(Parts3DSet *pPartsSet) {
	// 部品数0以下の時、処理を終了する
	if (pPartsSet->nPartsNum <= 0) {
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();	// デバイスを取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxSelf;				// 本体のマトリックス
	D3DXMATRIX *pMtxParts = NULL;	// 部品毎のマトリックスのポインタ

	// 部品毎のマトリックスのメモリを部品数分確保
	GetMemory((void**)&pMtxParts, sizeof(D3DXMATRIX), pPartsSet->nPartsNum);

	// 本体のワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxSelf);

	// 本体の向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// 本体の位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// 本体のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++) {
		Parts3D *pParts = &m_pParts[nCntParts];	// 部品の情報のポインタ
		Parts3DType *pPartsType = &pPartsSet->pPartsType[nCntParts];	// 部品の種類毎の情報のポインタ
		D3DXVECTOR3 posResult;	// 位置(結果)
		D3DXVECTOR3 rotResult;	// 向き(結果)

		{//========== *** 位置(結果)を更新 ***
			if (pParts->bPosAnim)
			{// 位置アニメーションフラグが真の時、カウンターの進行率に応じて位置を更新
				float fRate = Easing(pParts->easePos, pParts->nCounterPosAnim, pParts->nPosAnimTime);
				pParts->pos = (pParts->posOld * (1.0f - fRate)) + (pParts->targetPos * fRate);
			}

			// 結果位置を求める
			posResult = pParts->pos + pPartsType->fixedRelativePos;

			// 親番号が-1の時、結果位置に拡大倍率を乗算
			if (pPartsType->nParent == -1) {
				posResult *= pPartsSet->fScale;
			}
		}

		{//========== *** 向き(結果)を更新 ***
			if (pParts->bStep) 
			{// 踏むフラグが真の時、
				int nCounter = pParts->nCounterRotAnim;
				if (nCounter > pParts->nRotAnimTime) {
					nCounter = pParts->nRotAnimTime;
				}
				float fRate = Easing(EASE_LINEAR, nCounter, pParts->nRotAnimTime);
				pParts->rot = (pParts->rotOld * (1.0f - fRate)) + (INITD3DXVECTOR3 * fRate);
			}
			else if (pParts->bRotAnim)
			{// 向きアニメーションフラグが真の時、カウンターの進行率に応じて向きを更新
				float fRate = Easing(pParts->easeRot, pParts->nCounterRotAnim, pParts->nRotAnimTime);
				pParts->rot = (pParts->rotOld * (1.0f - fRate)) + (pParts->targetRot * fRate);
			}

			// 結果位置を求める
			rotResult = pParts->rot + pPartsType->fixedRelativeRot;
		}

		{// 部品のワールドマトリックスの算出
			// 部品のワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMtxParts[nCntParts]);

			if (pPartsType->nParent == -1)
			{// 親番号が-1の時、モデルのサイズを変更
				D3DXMatrixScaling(&pMtxParts[nCntParts], pPartsSet->fScale, pPartsSet->fScale, pPartsSet->fScale);
			}

			// 部品の向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rotResult.y, rotResult.x, rotResult.z);
			D3DXMatrixMultiply(&pMtxParts[nCntParts], &pMtxParts[nCntParts], &mtxRot);

			// 部品の位置を反映
			D3DXMatrixTranslation(&mtxTrans, posResult.x, posResult.y, posResult.z);
			D3DXMatrixMultiply(&pMtxParts[nCntParts], &pMtxParts[nCntParts], &mtxTrans);

			// 親の部品のマトリックス
			D3DXMATRIX mtxParent;

			// 部品の親番号が-1(親無し)でない時、親マトリックスを設定。親無しであれば自身のマトリックスを設定
			if (pPartsType->nParent != -1) { mtxParent = pMtxParts[pPartsType->nParent]; }
			else                           { mtxParent = mtxSelf; }

			// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(
				&pMtxParts[nCntParts],
				&pMtxParts[nCntParts],
				&mtxParent);

			if (pParts->bStep) {
				D3DXVECTOR3 pos = ConvMatrixToPos(pMtxParts[nCntParts]);
				D3DXVECTOR3 rot = posResult;
				D3DXMATRIX mtxOld = pMtxParts[nCntParts];
				pMtxParts[nCntParts] = ConvPosRotToMatrix(pos, rot);
				pMtxParts[nCntParts]._11 = mtxOld._11;
				pMtxParts[nCntParts]._22 = mtxOld._22;
				pMtxParts[nCntParts]._33 = mtxOld._33;
			}
		}

		// モデルの設定処理
		SetModel(pPartsType->nModelIdx, pMtxParts[nCntParts], m_col, pPartsType->bLighting);

		// マトリックスを部品に保存
		pParts->mtx = pMtxParts[nCntParts];
	}

	// 部品毎のマトリックスのメモリ解放
	ReleaseMemory((void**)&pMtxParts);
}

//========================================
// HitTestSet関数 - 当たり判定の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::HitTestSet(Parts3DSet *pPartsSet, HitTest3DSet *pHitTestSet) {
	if (pHitTestSet->nHitTestNum <= 0) {
		return;	// 当たり判定の数が0以下の時、処理を終了する
	}

	for (int nCntHitTestParts = 0; nCntHitTestParts < pHitTestSet->pHitTest[0].nHitTestPartsNum; nCntHitTestParts++) {
		D3DXMATRIX setMtx = INITD3DXMATRIX;	// 設定マトリックス

		if (pHitTestSet->pHitTest[0].pHitTestParts[nCntHitTestParts].nParent == DATANONE)
		{// 親番号が指定なしの時、
			// 本体マトリックスを設定マトリックスとする
			setMtx = ConvPosRotToMatrix(m_pos, m_rot);
		}
		else {
			// 親番号と一致した番号の部品のマトリックスを設定マトリックスとする
			for (int nCntParts3D = 0; nCntParts3D < pPartsSet->nPartsNum; nCntParts3D++) {
				if (pHitTestSet->pHitTest[0].pHitTestParts[nCntHitTestParts].nParent == pPartsSet->pPartsType[nCntParts3D].nIdx) {
					setMtx = m_pParts[nCntParts3D].mtx;
				}
			}
		}

		// 描画する当たり判定(3D)情報構造体
		DrawHitTest3DSet drawHitTestSet;
		/* マトリックス */drawHitTestSet.mtx = setMtx;
		/* 部品情報     */drawHitTestSet.hitTestParts = pHitTestSet->pHitTest[0].pHitTestParts[nCntHitTestParts];
		// 描画する当たり判定(3D)登録処理
		DrawHitTest3DSignUp(drawHitTestSet);
	}
}

//========================================
// InitParts3DInfo関数 - 部品(3D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::Init(int nSetUpNum) {
	// モデルのセットアップ(3D)情報
	ModelSetUp3D setUp = GetModelSetUpInfo(nSetUpNum);

	// 初期化
	m_nModelSetUp		= nSetUpNum;		// モデルのセットアップ(3D)番号を設定
	m_pos				= INITD3DXVECTOR3;	// 本体位置
	m_rot				= INITD3DXVECTOR3;	// 本体向き
	m_bMotionStop		= false;			// 停止フラグ
	m_bDrawCollision	= false;			// 当たり判定描画フラグ
	m_nMotion			= DATANONE;			// モーション番号
	m_nMotionOld		= DATANONE;			// モーション番号(過去)
	m_nMotionCounter	= 0;				// モーションカウンター
	m_col				= INITCOLOR;		// 色

	if (setUp.partsSet.nPartsNum > 0) 
	{// 部品数が0を超えていた時、
		// 部品情報のポインタのメモリを部品数分確保する
		GetMemory((void**)&m_pParts, sizeof(Parts3D), setUp.partsSet.nPartsNum);

		for (int nCntParts = 0; nCntParts < setUp.partsSet.nPartsNum; nCntParts++) {
			Parts3DType *pPartsType = &setUp.partsSet.pPartsType[nCntParts];	// 部品の種類毎の情報のポインタ

			m_pParts[nCntParts] = {};	// 部品の情報を初期化

			// マトリックスに相対位置/向きを適用
			m_pParts[nCntParts].mtx = ConvPosRotToMatrix(pPartsType->fixedRelativePos, pPartsType->fixedRelativeRot);
		}
	}
	else 
	{// 部品数が0を超えていなかった時、
		if (m_pParts != NULL) {
			delete[] m_pParts;
			m_pParts = NULL;
		}
	}
}

//========================================
// UninitParts3DInfo関数 - 部品(3D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::Uninit(void) {
	ReleaseMemory((void**)&m_pParts);	// 部品情報のメモリ解放
}

//========================================
// UpdateParts3DInfo関数 - 部品(3D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::Update(void) {
	// モデルのセットアップ(3D)情報
	ModelSetUp3D setUp = GetModelSetUpInfo(m_nModelSetUp);

	if (m_nModelSetUp != DATANONE)
	{// モデルのセットアップ(3D)番号が-1 or の時、
		if (!m_bMotionStop)
		{// 停止していない時、
			// モーション設定
			SetMotion3D();

			// モーションの更新処理
			UpdateMotion3D(m_nMotion, setUp.partsSet);
		}

		// モデルの設定処理
		ModelSetParts3D(&setUp.partsSet);

		if (m_bDrawCollision) 
		{// 当たり判定描画フラグが真の時、
			HitTestSet(&setUp.partsSet, &setUp.hitTestSet);	// 当たり判定の設定処理
		}
	}
}

//========================================
// SetMotion3D関数 - モーション(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::SetMotion3D(void) {
	if (m_nMotion == m_nMotionOld)
	{// 既にそのモーション番号に設定されている時、
		return;	// 処理を終了する
	}

	m_nMotionOld = m_nMotion;	// 過去のモーション番号として保存
	m_nMotionCounter = 0;		// モーションカウンターを初期化
}

//========================================
// UpdateMotion3D関数 - モーション(3D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
bool CParts3D::UpdateMotion3D(int nMotion, Parts3DSet partsSet) {
	if (nMotion == DATANONE) {
		return false;
	}

	Motion3D motion = GetMotion3D()[nMotion];	// モーション情報

	if (motion.nLoopTime == DATANONE)
	{// モーションのループ時間が無しの時、
		// 処理を終了する
		return false;
	}

	// モーションのアニメーション(3D)読み込み処理
	LoadMotionAnim3D(motion, partsSet);

	// モーションのアニメーション処理
	MotionAnim3D(partsSet);

	// モーションがループしたかフラグ
	bool bMotionLoop = false;

	if (++m_nMotionCounter > motion.nLoopTime)
	{// モーションカウンターを加算した結果ループ時間に達した時、
		bMotionLoop = true;	// モーションがループしたかフラグを真にする

		if (motion.bLoop)
		{// ループするかフラグが真の時、
			// モーションカウンターをループ制御
			IntLoopControl(&m_nMotionCounter, motion.nLoopTime, 0);
		}
		else
		{// ループするかフラグが偽の時、
			// モーションカウンターを制御
			IntControl(&m_nMotionCounter, motion.nLoopTime, 0);
		}
	}

	// モーションがループしたかフラグを返す
	return bMotionLoop;
}


//========================================
// LoadMotionAnim3D関数 - モーション(3D)のアニメーション読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::LoadMotionAnim3D(Motion3D motion, Parts3DSet partsSet) {
	// 部品毎のアニメーションの読み込み
	for (int nCntPartsAnim = 0; nCntPartsAnim < motion.nPartsMotionNum; nCntPartsAnim++) {
		// 部品毎のモーション(3D)の情報
		PartsMotion3D partsMotion3D = motion.pPartsMotion[nCntPartsAnim];

		for (int nCntCmd = 0; nCntCmd < partsMotion3D.nCommandNum; nCntCmd++) {
			Motion3DCommand cmd = partsMotion3D.pCommand[nCntCmd];	// コマンド情報
			Parts3D *pParts = m_pParts;	// 部品(3D)の情報

			{// 一致した部品番号までポインタを進める
				bool bFind = false;	// 一致部品確認フラグ
				for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++, pParts++)
				{
					if (partsSet.pPartsType[nCntParts].nIdx == nCntPartsAnim)
					{// 部品毎のアニメカウントと部品番号が一致した時、
						bFind = true;
						break;
					}
				}

				if ((m_nMotionCounter != cmd.nTime) || (!bFind))
				{// カウンターが実行時間と一致していない or 一致部品未確認、
					continue;
				}
			}

			// コマンドラベルに応じた処理
			switch (cmd.command) {
			case MOTION3D_COMMAND_LABEL_MOVE: {
				// 位置
				D3DXVECTOR3 pos = D3DXVECTOR3(cmd.pData[0], cmd.pData[1], cmd.pData[2]);
				// 移動にかかる時間
				int nMoveTime = (int)cmd.pData[3];
				// 補間
				EASE ease = (EASE)((int)cmd.pData[4]);

				if (nMoveTime == 0)
				{// 移動にかかる時間が0の時、
					pParts->pos = pos;	// 位置を直接代入
				}
				else
				{// 移動にかかる時間が0でない時、
					// 位置補間を設定
					pParts->easePos = ease;
					// 元の位置を設定
					pParts->posOld = pParts->pos;
					// 目標位置を代入
					pParts->targetPos = pos;
					// 位置アニメーションにかかる時間を代入
					pParts->nPosAnimTime = nMoveTime;
					// 位置アニメーションカウンターを初期化
					pParts->nCounterPosAnim = 0;
					// 位置アニメーションフラグを真にする
					pParts->bPosAnim = true;
				}
			}
			break;
			case MOTION3D_COMMAND_LABEL_SPIN: {
				// 向き
				D3DXVECTOR3 rot = D3DXVECTOR3(cmd.pData[0], cmd.pData[1], cmd.pData[2]);
				// 回転にかかる時間
				int nSpinTime = (int)cmd.pData[3];
				// 補間
				EASE ease = (EASE)((int)cmd.pData[4]);

				if (nSpinTime == 0)
				{// 回転にかかる時間が0の時、
					pParts->rot = rot;	// 向きを直接代入
				}
				else
				{// 回転にかかる時間が0でない時、
					pParts->easeRot         = ease;			// 向き補間を設定
					pParts->rotOld          = pParts->rot;	// 元の向きを設定
					pParts->targetRot       = rot;			// 目標向きを代入
					pParts->nRotAnimTime    = nSpinTime;	// 向きアニメーションにかかる時間を代入
					pParts->nCounterRotAnim = 0;			// 向きアニメーションカウンターを初期化
					pParts->bRotAnim        = true;			// 向きアニメーションフラグを真にする
				}
			}
			break;
			case MOTION3D_COMMAND_LABEL_STEP: {
				pParts->rotOld          = ConvMatrixToRot(pParts->mtx);	// 元の向きを設定
				pParts->nRotAnimTime    = (int)cmd.pData[0];	// 向きアニメーションにかかる時間を代入
				pParts->nCounterRotAnim = 0;					// 向きアニメーションカウンターを初期化
				pParts->bRotAnim        = false;				// 向きアニメーションフラグを真にする
				pParts->bStep           = true;					// 踏むフラグ真
				pParts->nStepTime       = (int)cmd.pData[1];	// 踏む時間
			}
			break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//========================================
// MotionAnim3D関数 - モーション(3D)のアニメーション処理 -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::MotionAnim3D(Parts3DSet partsSet) {
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++) {
		// 部品の情報のポインタ
		Parts3D *pParts = &m_pParts[nCntParts];

		if (pParts->bPosAnim)
		{// 位置アニメーションフラグが真の時、
			if (++pParts->nCounterPosAnim >= pParts->nPosAnimTime)
			{// 位置アニメーションカウンターを加算した結果位置アニメーションにかかる時間を上回った時、
				pParts->bPosAnim = false;	// 位置アニメーションフラグを偽にする
			}
		}

		if (pParts->bStep) 
		{// 踏むフラグが真の時、
			if (++pParts->nCounterRotAnim >= pParts->nStepTime)
			{// 向きアニメーションカウンターを加算した結果踏む時間を上回った時、
				pParts->bStep = false;	// 踏むフラグを偽にする
			}
		}
		else if (pParts->bRotAnim)
		{// 角度アニメーションフラグが真の時、
			if (++pParts->nCounterRotAnim >= pParts->nRotAnimTime)
			{// 向きアニメーションカウンターを加算した結果向きアニメーションにかかる時間を上回った時、
				pParts->bRotAnim = false;	// 向きアニメーションフラグを偽にする
			}
		}
	}
}