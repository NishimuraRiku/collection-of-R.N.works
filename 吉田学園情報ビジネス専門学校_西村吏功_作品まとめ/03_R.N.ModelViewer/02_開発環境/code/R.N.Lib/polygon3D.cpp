//========================================
// 
// ポリゴン(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon3D.cpp ***
//========================================
#include <assert.h>
// _R.N.Lib
#include "RNmain.h"

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** モーション(3D)関連 ***
// モーション(3D)のアニメーション読み込み処理
void LoadMotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);
// モーション(3D)のアニメーション処理
void MotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** モーション(3D)関連 *** |----------
//============================================================
//========================================
// LoadMotionAnim3D関数 - モーション(3D)のアニメーション読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	// サウンドのアニメーションの読み込み
	for (int nCntSoundAnim = 0; nCntSoundAnim < motionSet.aMotion[nMotionNum].nSoundAnimNum; nCntSoundAnim++)
	{
		if (pPartsInfo->nCounterMotion == motionSet.aMotion[nMotionNum].aSoundAnim[nCntSoundAnim].nTime)
		{// モーションカウンターが切り替え時間に達した時、サウンドを再生
			PlaySound(motionSet.aMotion[nMotionNum].aSoundAnim[nCntSoundAnim].nSound);
		}
	}

	// 部品毎のアニメーションの読み込み
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		// 部品毎のモーション情報のポインタ
		PartsMotion3D *pPartsMotion = &motionSet.aMotion[nMotionNum].aPartsMotion[nCntParts];
		// 部品の情報のポインタ
		Parts3D *pParts = &pPartsInfo->aParts[nCntParts];

		// 位置のアニメーションの読み込み
		for (int nCntPosAnim = 0; nCntPosAnim < pPartsMotion->nPosAnimNum; nCntPosAnim++)
		{
			// 位置アニメーションの情報のポインタ
			PositionAnim3D *pPosAnim = &pPartsMotion->aPosAnim[nCntPosAnim];

			if (pPartsInfo->nCounterMotion != pPosAnim->nTime)
			{// モーションカウンターが切り替え時間に達していない時、
				continue;	// 繰り返し処理を折り返す
			}

			if ((pPosAnim->nTime == -1) || (pPosAnim->nMoveTime == 0))
			{// 切り替え時間が-1 or 位置アニメーションにかかる時間が0の時、
				// 位置と目標位置にアニメーション位置を代入
				pParts->pos = pPosAnim->pos;
				pParts->targetPos = pPosAnim->pos;
			}
			else
			{// 切り替え時間が-1でない時、
				pParts->posOld = pParts->pos;				// 元の位置を設定
				pParts->targetPos = pPosAnim->pos;			// 目標位置を代入
				pParts->nPosAnimTime = pPosAnim->nMoveTime;	// 位置アニメーションにかかる時間を代入
				pParts->nCounterPosAnim = 0;				// 位置アニメーションカウンターを初期化
				pParts->bPosAnim = true;					// 位置アニメーションフラグを真にする
			}
		}

		// 向きのアニメーションの読み込み
		for (int nCntRotAnim = 0; nCntRotAnim < pPartsMotion->nRotAnimNum; nCntRotAnim++)
		{
			// 向きアニメーションの情報のポインタ
			RotationAnim3D	*pRotAnim = &pPartsMotion->aRotAnim[nCntRotAnim];

			if (pPartsInfo->nCounterMotion != pRotAnim->nTime)
			{// モーションカウンターが切り替え時間に達していない時、
				continue;	// 繰り返し処理を折り返す
			}

			if ((pRotAnim->nTime == -1) || (pRotAnim->nMoveTime == 0))
			{// 切り替え時間が-1 or 向きアニメーションにかかる時間が0の時、
				pParts->rot =						// 向きと
				pParts->targetRot = pRotAnim->rot;	// 目標向きにアニメーション向きを代入
			}
			else
			{// 切り替え時間が-1でない時、
				pParts->rotOld = pParts->rot;				// 元の向きを設定
				pParts->targetRot = pRotAnim->rot;			// 目標向きを代入
				pParts->nRotAnimTime = pRotAnim->nMoveTime;	// 向きアニメーションにかかる時間を代入
				pParts->nCounterRotAnim = 0;				// 向きアニメーションカウンターを初期化
				pParts->bRotAnim = true;					// 向きアニメーションフラグを真にする
			}
		}
	}
}

//========================================
// MotionAnim3D関数 - モーション(3D)のアニメーション処理 -
// Author:RIKU NISHIMURA
//========================================
void MotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		// 部品の情報のポインタ
		Parts3D *pParts = &pPartsInfo->aParts[nCntParts];

		if (pParts->bPosAnim)
		{// 位置アニメーションフラグが真の時、
			if (++pParts->nCounterPosAnim >= pParts->nPosAnimTime)
			{// 位置アニメーションカウンターを加算した結果位置アニメーションにかかる時間を上回った時、
				pParts->bPosAnim = false;			// 位置アニメーションフラグを偽にする
				pParts->pos = pParts->targetPos;	// 位置に目標位置を代入
			}
		}

		if (pParts->bRotAnim)
		{// 角度アニメーションフラグが真の時、
			if (++pParts->nCounterRotAnim >= pParts->nRotAnimTime)
			{// 向きアニメーションカウンターを加算した結果向きアニメーションにかかる時間を上回った時、
				pParts->bRotAnim = false;			// 向きアニメーションフラグを偽にする
				pParts->rot = pParts->targetRot;	// 向きに目標向きを代入
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 部品(3D)関連 *** |----------------
//============================================================
//========================================
// LoadParts3DSet関数 - 部品設定情報(3D)の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet)
{
	// データ検索用
	char aDataSearch[TXT_MAX];
	float fAllScale;	// 全体の拡大倍率

	// 部品数を初期化
	pPartsSet->nPartsNum = 0;

	// 部品設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (false){}
		else if (!strcmp(aDataSearch, "PARTSSET_END"))
		{// 読み込みを終了
			break; 
		}
		else if (!strcmp(aDataSearch, "ALL_SCALE:"))
		{// 全体の拡大倍率
			fscanf(pFile, "%f", &fAllScale); 
		}
		else if (!strcmp(aDataSearch, "PARTS"))
		{
			// 部品の種類毎の情報のポインタ
			Parts3DType *pPartsType = &pPartsSet->aPartsType[pPartsSet->nPartsNum];

			// 部品情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "PARTS_END"))
				{// 読み込みを終了
					pPartsSet->nPartsNum++; break;
				}
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))
				{// モデルの相対パス
					fscanf(pFile, "%s", &pPartsType->aModelPath);
				}
				else if (!strcmp(aDataSearch, "PARENT:"))
				{// 親パーツ番号 
					fscanf(pFile, "%d", &pPartsType->nParent); 
				}
				else if (!strcmp(aDataSearch, "RELATIVE_POS:"))
				{// 固定の相対位置
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.x);	// X
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.y);	// Y
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "RELATIVE_ROT:"))
				{// 固定の相対角度
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.x);	// X
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.y);	// Y
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.z);	// Z
					pPartsType->fixedRelativeRot.x *= D3DX_PI;				// 円周率補正
					pPartsType->fixedRelativeRot.y *= D3DX_PI;				// 円周率補正
					pPartsType->fixedRelativeRot.z *= D3DX_PI;				// 円周率補正
				}
				else if (!strcmp(aDataSearch, "ROT_RELATIVE_POS:"))
				{// 回転軸の相対位置
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.x);	// X
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.y);	// Y
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.z);	// Z
				}
			}
		}
	}
}

//========================================
// InitParts3DInfo関数 - 部品(3D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParts3DInfo(Parts3DInfo *pPartsInfo)
{
	pPartsInfo->nCounterMotion	= 0;	// モーションカウンター
	pPartsInfo->nMotion			= 0;	// モーション番号

	for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++) 
	{
		pPartsInfo->aParts[nCntParts].posResult			= INITD3DXVECTOR3;	// 位置(結果)
		pPartsInfo->aParts[nCntParts].pos				= INITD3DXVECTOR3;	// 位置
		pPartsInfo->aParts[nCntParts].rotResult			= INITD3DXVECTOR3;	// 向き(結果)
		pPartsInfo->aParts[nCntParts].rot				= INITD3DXVECTOR3;	// 向き
		pPartsInfo->aParts[nCntParts].bPosAnim			= false;			// 位置アニメーションフラグ
		pPartsInfo->aParts[nCntParts].nCounterPosAnim	= 0;				// 位置アニメーションカウンター
		pPartsInfo->aParts[nCntParts].nPosAnimTime		= 0;				// 位置アニメーションにかかる時間
		pPartsInfo->aParts[nCntParts].posOld			= INITD3DXVECTOR3;	// 元の位置
		pPartsInfo->aParts[nCntParts].targetPos			= INITD3DXVECTOR3;	// 目標位置
		pPartsInfo->aParts[nCntParts].bRotAnim			= false;			// 角度アニメーションフラグ
		pPartsInfo->aParts[nCntParts].nCounterRotAnim	= 0;				// 角度アニメーションカウンター
		pPartsInfo->aParts[nCntParts].nRotAnimTime		= 0;				// 角度アニメーションにかかる時間
		pPartsInfo->aParts[nCntParts].rotOld			= INITD3DXVECTOR3;	// 元の角度
		pPartsInfo->aParts[nCntParts].targetRot			= INITD3DXVECTOR3;	// 目標角度
	}
}

//========================================
// InitParts3DTransform関数 - 部品(3D)のトランスフォームの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet)
{
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		= &pPartsInfo->aParts[nCntParts];		// 部品の情報のポインタ
		Parts3DType *pPartsType = &pPartsSet->aPartsType[nCntParts];	// 部品の種類毎の情報のポインタ

		// 位置を更新
		pParts->posResult = pPartsType->fixedRelativePos;
		pParts->rotResult = pPartsType->fixedRelativeRot;
	}
}

//========================================
// GetParts3DTransform関数 - 部品(3D)のトランスフォームの取得処理 -
// Author:RIKU NISHIMURA
//========================================
void GetParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet)
{
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		= &pPartsInfo->aParts[nCntParts];		// 部品の情報のポインタ
		Parts3DType *pPartsType = &pPartsSet->aPartsType[nCntParts];	// 部品の種類毎の情報のポインタ

		if (pParts->bPosAnim)
		{// 位置アニメーションフラグが真の時、
			float	fRate	// カウンターの進行率
					= (float)pParts->nCounterPosAnim / (float)pParts->nPosAnimTime;

			// 位置を更新
			pParts->pos = (pParts->posOld * (1.0f - fRate)) + (pParts->targetPos * fRate);
		}

		// 位置(結果)を更新
		pParts->posResult = pParts->pos + pPartsType->fixedRelativePos;

		if (pParts->bRotAnim)
		{// 向きアニメーションフラグが真の時、
			float	fRate	// カウンターの進行率
					= (float)pParts->nCounterRotAnim / (float)pParts->nRotAnimTime;

			// 向きを更新
			pParts->rot = (pParts->rotOld * (1.0f - fRate)) + (pParts->targetRot * fRate);
		}

		// 向き(結果)を更新
		pParts->rotResult = pParts->rot + pPartsType->fixedRelativeRot;
	}
}

//========================================
// InitParameterParts3DInfo関数 - 部品(3D)管理情報のパラメーター初期値を返す -
// Author:RIKU NISHIMURA
//========================================
Parts3DInfo InitParameterParts3DInfo(void)
{
	// 部品情報のパラメーター初期値
	Parts3D initParts3D = {
		INITD3DXVECTOR3,	// 位置(結果)
		INITD3DXVECTOR3,	// 位置
		INITD3DXVECTOR3,	// 向き(結果)
		INITD3DXVECTOR3,	// 向き
		false,				// 位置アニメーションフラグ
		0,					// 位置アニメーションカウンター
		0,					// 位置アニメーションにかかる時間
		INITD3DXVECTOR3,	// 元の位置
		INITD3DXVECTOR3,	// 目標位置
		false,				// 角度アニメーションフラグ
		0,					// 角度アニメーションカウンター
		0,					// 角度アニメーションにかかる時間
		INITD3DXVECTOR3,	// 元の角度
		INITD3DXVECTOR3,	// 目標角度
	};

	return{
		0,					// モーション番号
		0,					// モーションカウンター
		{ initParts3D },	// 部品情報
	};
}

//========================================
// GetMatrixParts3D関数 - 部品(3D)のマトリックスを取得 -
// Author:RIKU NISHIMURA
//========================================
D3DXMATRIX GetMatrixParts3D(int nParts, D3DXVECTOR3 pos, D3DXVECTOR3 rot, Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 部品のマトリックス
	D3DXMATRIX aMtxParts[PARTS_3D_MAX];
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxSelf;				// 本体のマトリックス

	// 本体のワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxSelf);

	// 本体の向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// 本体の位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++)
	{
		// 部品(3D)の情報のポインタ
		Parts3D *pParts = &pPartsInfo->aParts[nCntParts];
		// 部品(3D)の部品の種類毎の情報のポインタ
		Parts3DType *pPartsType = &pPartsSet->aPartsType[nCntParts];

		D3DXMATRIX	mtxParent;	// 親マトリックス

		// 部品のワールドマトリックスの初期化
		D3DXMatrixIdentity(&aMtxParts[nCntParts]);

		// 部品の向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&aMtxParts[nCntParts], &aMtxParts[nCntParts], &mtxRot);

		// 部品の位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&aMtxParts[nCntParts], &aMtxParts[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// 部品の親番号が-1(親無し)でない時、親マトリックスを設定
			mtxParent = aMtxParts[pPartsType->nParent];
		}
		else
		{// 部品の親番号が-1(親無し)の時、親マトリックスを本体マトリックスで設定
			mtxParent = mtxSelf;
		}

		// 算出した部品のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(
			&aMtxParts[nCntParts],
			&aMtxParts[nCntParts],
			&mtxParent);

		if (nCntParts >= nParts) 
		{// 部品のカウントが取得したい部品番号に達した時、
			return aMtxParts[nParts];	// マトリックスを返す
		}
	}

	assert(false);
	return mtxSelf;
}

//============================================================
//--------------------| *** モーション(3D)関連 *** |----------
//============================================================
//========================================
// LoadMotion3DSet関数 - モーション(3D)設定情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMotion3DSet(FILE *pFile, Motion3DSet *pMotionSet)
{
	char aDataSearch[TXT_MAX];	// データ検索用

	// モーション数を初期化
	pMotionSet->nMotionNum = 0;

	// モーション設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (!strcmp(aDataSearch, "MOTIONSET_END"))
		{// 読み込みを終了
			break;
		}
		else if (!strcmp(aDataSearch, "MOTION"))
		{
			// モーションの情報のポインタ
			Motion3D *pMotion = &pMotionSet->aMotion[pMotionSet->nMotionNum];
			int nCntParts = 0;	// 部品のカウント

			// サウンドのアニメーション数を初期化
			pMotion->nSoundAnimNum = 0;

			// モーション情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索
				
				if (!strcmp(aDataSearch, "MOTION_END"))
				{// モーション数を加算し、読み込みを終了
					pMotionSet->nMotionNum++;
					break; 
				}
				else if (!strcmp(aDataSearch, "LOOP_TIME:"))
				{// ループ時間 
					fscanf(pFile, "%d", &pMotion->nLoopTime);
				}
				else if (!strcmp(aDataSearch, "LOOP:"))
				{// ループするかフラグ
					int nTemp;
					fscanf(pFile, "%d", &nTemp);
					pMotion->bLoop = (nTemp != 0);
				}
				else if (!strcmp(aDataSearch, "PARTS"))
				{
					// 部品毎のモーションの情報のポインタ
					PartsMotion3D *pPartsMotion = &pMotion->aPartsMotion[nCntParts];

					// 位置/角度のアニメーション数を初期化
					pPartsMotion->nPosAnimNum = 0;
					pPartsMotion->nRotAnimNum = 0;

					// 部品毎のモーションの情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (false) {}
						else if (!strcmp(aDataSearch, "PARTS_END"))
						{// 部品のカウントを加算し、読み込みを終了
							nCntParts++;
							break;
						}
						else if (!strcmp(aDataSearch, "POSITION_ANIM"))
						{
							// 位置のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索

								if (false) {}
								else if (!strcmp(aDataSearch, "POSITION_ANIM_END"))
								{// 読み込みを終了
									break; 
								}
								else if (!strcmp(aDataSearch, "TIME"))
								{												// 位置のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.z);		// Z
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nMoveTime);	// 到達にかかる時間
									pPartsMotion->nPosAnimNum++;														// 位置のアニメーション数を加算
								}
							}
						}
						else if (!strcmp(aDataSearch, "ROTATION_ANIM"))
						{
							// 角度のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索
								
								if (!strcmp(aDataSearch, "ROTATION_ANIM_END"))
								{// 読み込みを終了
									break;
								}
								else if (!strcmp(aDataSearch, "TIME"))				{									// 角度のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z);		// Z
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x *= D3DX_PI;					// 円周率補正
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y *= D3DX_PI;					// 円周率補正
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z *= D3DX_PI;					// 円周率補正
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nMoveTime);	// 到達にかかる時間
									pPartsMotion->nRotAnimNum++;														// 角度のアニメーション数を加算
								}
							}
						}
					}
				}
				else if (!strcmp(aDataSearch, "SOUND_ANIM"))
				{
					// サウンドのアニメーションの情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (!strcmp(aDataSearch, "SOUND_ANIM_END"))
						{// 読み込みを終了
							break; 
						}
						else if (!strcmp(aDataSearch, "TIME"))
						{// サウンドのアニメーション情報
							fscanf(pFile, "%d", &pMotion->aSoundAnim[pMotion->nSoundAnimNum].nTime);	// 切り替え時間
							fscanf(pFile, "%d", &pMotion->aSoundAnim[pMotion->nSoundAnimNum].nSound);	// サウンド番号
							pMotion->nSoundAnimNum++;													// サウンドのアニメーション数を加算
						}
					}
				}
			}
		}
	}
}

//========================================
// UpdateMotion3D関数 - モーション(3D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
bool UpdateMotion3D(int nMotion, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	if (motionSet.aMotion[nMotion].nLoopTime == -1)
	{// モーションのループ時間が-1の時、
		// 処理を終了する
		return false;
	}

	// モーションのアニメーション(3D)読み込み処理
	LoadMotionAnim3D(nMotion, motionSet, pPartsInfo, partsSet);

	// モーションのアニメーション処理
	MotionAnim3D(nMotion, motionSet, pPartsInfo, partsSet);

	// モーションがループしたかフラグ
	bool bMotionLoop = false;

	if (++pPartsInfo->nCounterMotion > motionSet.aMotion[nMotion].nLoopTime)
	{// モーションカウンターを加算した結果ループ時間に達した時、
		bMotionLoop = true;	// モーションがループしたかフラグを真にする

		if (motionSet.aMotion[nMotion].bLoop)
		{// ループするかフラグが真の時、
			// モーションカウンターをループ制御
			IntLoopControl(&pPartsInfo->nCounterMotion, motionSet.aMotion[nMotion].nLoopTime, 0);
		}
		else
		{// ループするかフラグが偽の時、
			// モーションカウンターを制御
			IntControl(&pPartsInfo->nCounterMotion, motionSet.aMotion[nMotion].nLoopTime, 0);
		}
	}

	// モーションがループしたかフラグを返す
	return bMotionLoop;
}

//========================================
// SetMotion3D関数 - モーション(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetMotion3D(Parts3DInfo *pPartsInfo, int nMotion) 
{
	if (pPartsInfo->nMotion == nMotion) 
	{// 既にそのモーション番号に設定されている時、
		return;	// 処理を終了する
	}

	pPartsInfo->nMotion = nMotion;	// モーション番号を代入
	pPartsInfo->nCounterMotion = 0;	// モーションカウンターを初期化
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// SetVertexPos3D関数 - 頂点座標の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight)
{
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
	pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
}

//========================================
// SetNormalLine3D関数 - 法線ベクトルの設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor) 
{
	// 法線ベクトルの設定
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//========================================
// SetVertexColor3D関数 - 頂点色の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor3D(VERTEX_3D *pVtx, Color col)
{
	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos3D関数 - テクスチャ座標の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
{
	if (nPtnMaxX * nPtnMaxY == 0)
	{// パターンの上限が0の時、
		// 処理を終了する
		return;
	}

	D3DXVECTOR3 pos;	// 位置

	if (pPos == NULL)
	{// 位置のポインタがNULLの時、
		// 位置を初期化
		pos = D3DXVECTOR3(0, 0, 0);
	}
	else
	{// 位置のポインタがNULLで無い時、
		// 位置を-補正して代入
		pos = -*pPos;
	}

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
}

//========================================
// SetMaterial関数 - マテリアルの設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col)
{
	D3DXCOLOR mtlColTemp = pMat->Diffuse;	// マテリアルの色を保存

	// マテリアルの色を設定
	pMat->Diffuse.r = mtlColTemp.r * ((float)col.r / (float)255);
	pMat->Diffuse.g = mtlColTemp.g * ((float)col.g / (float)255);
	pMat->Diffuse.b = mtlColTemp.b * ((float)col.b / (float)255);
	pMat->Diffuse.a = mtlColTemp.a * ((float)col.a / (float)255);

	// マテリアルの設定
	pDevice->SetMaterial(pMat);

	// マテリアルの色を元に戻す
	pMat->Diffuse = mtlColTemp;
}