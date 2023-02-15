//========================================
// 
// EFF:衝撃波[00] の関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_shock-wave_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "eff_shock-wave_00.h"

//****************************************
// マクロ定義
//****************************************
#define EFF_SHOCKWAVE_00_MAX		(16)	// EFF:衝撃波[00] の最大数
#define EFF_SHOCKWAVE_00_DIVISION	(16)	// EFF:衝撃波[00] の分割数

// EFF:衝撃波[00] の法線ベクトル
#define EFF_SHOCKWAVE_00_NOR	D3DXVECTOR3(0.0f,1.0f,0.0f)

//****************************************
// 構造体の定義
//****************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	bool		bUse;			// 使用されているかフラグ
	int			nLife;			// 寿命
	int			nLifeMax;		// 寿命の最大値
	float		fRadius;		// 半径
	float		fThickness;		// 太さ
	float		fEdgeHeight;	// 縁の高さ
	Color		inCol;			// 内側の色
	Color		outCol;			// 外側の色
}Eff_shockWave_00;

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:衝撃波[00] のパラメーター初期化処理
void InitParameterEff_shockWave_00(Eff_shockWave_00 *pEff);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_shockWave_00	// EFF:衝撃波[00] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldEff_shockWave_00;	// EFF:衝撃波[00] のワールドマトリックス
Eff_shockWave_00		g_aEff_shockWave_00			// EFF:衝撃波[00] の情報
						[EFF_SHOCKWAVE_00_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_shockWave_00関数 - EFF:衝撃波[00] のパラメーター初期化処理 -
//========================================
void InitParameterEff_shockWave_00(Eff_shockWave_00 *pEff) 
{
	pEff->pos			= INITD3DXVECTOR3;	// 位置
	pEff->rot			= INITD3DXVECTOR3;	// 向き
	pEff->bUse			= false;			// 使用されているかフラグ
	pEff->nLife			= 0;				// 寿命
	pEff->nLifeMax		= 0;				// 寿命の最大値
	pEff->fRadius		= 0.0f;				// 半径
	pEff->fThickness	= 0.0f;				// 太さ
	pEff->fEdgeHeight	= 0.0f;				// 縁の高さ
	pEff->inCol			= INITCOLOR;		// 色
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitEff_shockWave_00関数 - EFF:衝撃波[00] の初期化処理 -
//========================================
void InitEff_shockWave_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((EFF_SHOCKWAVE_00_DIVISION + 1) * 2) * EFF_SHOCKWAVE_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_shockWave_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_shockWave_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_shockWave_00	*pEff	// EFF:衝撃波[00] の情報構造体
						= g_aEff_shockWave_00;

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++,pEff++)
	{
		// パラメーターの初期化処理
		InitParameterEff_shockWave_00(pEff);

		for (int nCntVtx = 0; nCntVtx < (EFF_SHOCKWAVE_00_DIVISION + 1) * 2; nCntVtx++, pVtx++)
		{
			pVtx->nor = EFF_SHOCKWAVE_00_NOR;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_shockWave_00->Unlock();
}

//========================================
// UninitEff_shockWave_00関数 - EFF:衝撃波[00] の初期化処理 -
//========================================
void UninitEff_shockWave_00(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffEff_shockWave_00 != NULL)
	{
		g_pVtxBuffEff_shockWave_00->Release();
		g_pVtxBuffEff_shockWave_00 = NULL;
	}
}

//========================================
// UpdateEff_shockWave_00関数 - EFF:衝撃波[00] の更新処理 -
//========================================
void UpdateEff_shockWave_00(void)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_shockWave_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_shockWave_00	*pEff	// EFF:衝撃波[00] の情報のポインタ
						= g_aEff_shockWave_00;

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++, pEff++, pVtx += ((EFF_SHOCKWAVE_00_DIVISION + 1) * 2))
	{
		if (!pEff->bUse)
		{// 使用されている状態でない時、
			// 繰り返し処理を折り返す
			continue;
		}
		else if (--pEff->nLife <= 0)
		{// 寿命を減算した結果0以下の時、
			pEff->bUse = false;	// 使用されていない状態にする
			continue;			// 繰り返し処理を折り返す
		}

		for (int nCntDiv = 0; nCntDiv < EFF_SHOCKWAVE_00_DIVISION + 1; nCntDiv++)
		{
			float fSetAngle = nCntDiv * (D3DX_PI / (float)EFF_SHOCKWAVE_00_DIVISION) * 2;
			int   nStartVtx = nCntDiv * 2;
			float fLifeRate = (float)pEff->nLife / (float)pEff->nLifeMax;
			float fRadius	= pEff->fRadius * (1.0f - fLifeRate);

			// 内側の頂点座標を設定
			{
				float fDistance = Prus(fRadius - pEff->fThickness);
				pVtx[nStartVtx].pos.x = sinf(fSetAngle) * (fDistance);
				pVtx[nStartVtx].pos.z = cosf(fSetAngle) * (fDistance);
			}
			// 外側の頂点座標を設定
			pVtx[nStartVtx + 1].pos.x = sinf(fSetAngle) * fRadius;
			pVtx[nStartVtx + 1].pos.z = cosf(fSetAngle) * fRadius;
			pVtx[nStartVtx + 1].pos.y = pEff->fEdgeHeight;

			// 色を設定
			pVtx[nStartVtx].col =
				D3DCOLOR_RGBA(
					pEff->inCol.r,
					pEff->inCol.g,
					pEff->inCol.b,
					(int)(pEff->inCol.a * fLifeRate));
			pVtx[nStartVtx + 1].col =
				D3DCOLOR_RGBA(
					pEff->outCol.r,
					pEff->outCol.g,
					pEff->outCol.b,
					(int)(pEff->outCol.a * fLifeRate));
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_shockWave_00->Unlock();
}

//========================================
// DrawEff_shockWave_00関数 - EFF:衝撃波[00] の描画処理 -
//========================================
void DrawEff_shockWave_00(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイス取得
	D3DXMATRIX		  mtxRot, mtxTrans;				// 計算用マトリックス
	Eff_shockWave_00  *pEff = g_aEff_shockWave_00;	// EFF:衝撃波[00] の情報構造体
		
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEff_shockWave_00, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++, pEff++)
	{
		if ((!pEff->bUse) || (pEff->nLife == pEff->nLifeMax)) 
		{// 使用されていない状態 or 寿命が最大値に等しい時、
			continue;	// 処理を折り返す
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldEff_shockWave_00);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pEff->rot.y, pEff->rot.x, pEff->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shockWave_00, &g_mtxWorldEff_shockWave_00, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pEff->pos.x, pEff->pos.y, pEff->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shockWave_00, &g_mtxWorldEff_shockWave_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEff_shockWave_00);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((EFF_SHOCKWAVE_00_DIVISION + 1) * 2) * nCntEff, (EFF_SHOCKWAVE_00_DIVISION * 2));
	}
}

//========================================
// SetEff_shockWave_00関数 - EFF:衝撃波[00] の設定処理 -
//========================================
void SetEff_shockWave_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fThickness, float fEdgeHeight,Color inCol, Color outCol, int nLife)
{
	Eff_shockWave_00 *pEff	// EFF:衝撃波[00] の情報のポインタ
					 = g_aEff_shockWave_00;
	VERTEX_3D		 *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_shockWave_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++, pEff++)
	{
		if (pEff->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pEff->pos = pos;					// 位置を代入
		pEff->rot = rot;					// 向きを代入
		pEff->fRadius = fRadius;			// 半径を代入
		pEff->fThickness = fThickness;		// 太さを代入
		pEff->fEdgeHeight = fEdgeHeight;	// 縁の高さを代入
		pEff->inCol = inCol;				// 内側の色を代入
		pEff->outCol = outCol;				// 外側の色を代入
		pEff->nLife	=						// 寿命を代入
		pEff->nLifeMax = nLife;				// 寿命の最大値を代入
		pEff->bUse = true;					// 使用されている状態にする

		// 繰り返し処理を抜ける
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_shockWave_00->Unlock();
}