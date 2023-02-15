//========================================
// 
// UI:注意マーク[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_attention-mark_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "color.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "ui_attention-mark_00.h"	// UI :注意マーク	[00]
#include "obj_stage_00.h"			// OBJ:ステージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// UI:注意マーク[00] の最大数
#define UI_ATTENTIONMARK_00_MAX	(OBJ_STAGE_00_GATE_MAX)

// UI:注意マーク[00] の幅
// UI:注意マーク[00] の高さ
#define UI_ATTENTIONMARK_00_WIDTH			(64)
#define UI_ATTENTIONMARK_00_HEIGHT			(64)

// UI:注意マーク[00] の矢印の幅
// UI:注意マーク[00] の矢印の奥行き
// UI:注意マーク[00] の矢印の奥分割数
// UI:注意マーク[00] の矢印のY補正
// UI:注意マーク[00] の矢印の移動にかかる時間
#define UI_ATTENTIONMARK_00_ARROW_WIDTH				(64)
#define UI_ATTENTIONMARK_00_ARROW_DEPTH				(128)
#define UI_ATTENTIONMARK_00_ARROW_DIVISION_DEPTH	(8)
#define UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION		(0.1f)
#define UI_ATTENTIONMARK_00_ARROW_MOVE_TIME			(60)

// UI:注意マーク[00] の法線ベクトル
#define UI_ATTENTIONMARK_00_NOR	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

// UI:注意マーク[00] の色
// UI:注意マーク[00] の注意時の色
#define UI_ATTENTIONMARK_00_COLOR			Color{255,43,0,255}
#define UI_ATTENTIONMARK_00_WARNING_COLOR	Color{131,0,243,255}

// UI:注意マーク[00] の点滅にかかる時間
#define UI_ATTENTIONMARK_00_BRINK_TIME	(40)

//****************************************
// 列挙型の定義
//****************************************
// UI:注意マーク[00] のポリゴン番号
typedef enum
{
	UI_ATTENTIONMARK_00_POLYGON_BASE,	// 基本
	UI_ATTENTIONMARK_00_POLYGON_ARROW,	// 矢印
	UI_ATTENTIONMARK_00_POLYGON_MAX,
}UI_ATTENTIONMARK_00_POLYGON;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:注意マーク[00] のパラメーターの初期化処理
void InitParameterUi_attentionMark_00(Ui_attentionMark_00 *pUi);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9			g_pTextureUi_attentionMark_00	// UI:注意マーク[00] のテクスチャへのポインタ
							[UI_ATTENTIONMARK_00_POLYGON_MAX]
							= {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffUi_attentionMark_00	// UI:注意マーク[00] の頂点バッファへのポインタ
							= NULL;
D3DXMATRIX					g_mtxWorldUi_attentionMark_00;	// UI:注意マーク[00] のワールドマトリックス
Ui_attentionMark_00			g_aUi_attentionMark_00			// UI:注意マーク[00] の情報
							[UI_ATTENTIONMARK_00_MAX];
Ui_attentionMark_00Control	g_ui_attentionMark_00Control;	// UI:注意マーク[00] の管理情報

// UI:注意マーク[00] のテクスチャの相対パス
const char c_aUi_attentionMark_00Texture[UI_ATTENTIONMARK_00_POLYGON_MAX][TXT_MAX] = 
{
	"data\\TEXTURE\\UserInterface\\ui_attention-mark_000.png",
	"data\\TEXTURE\\UserInterface\\ui_attention-arrow_000.png",
};

//========== *** UI:注意マーク[00] の情報を取得 ***
Ui_attentionMark_00 *GetUi_attentionMark_00(void) 
{
	return g_aUi_attentionMark_00;
}

//========== *** UI:注意マーク[00] の管理情報を取得 ***
Ui_attentionMark_00Control *GetUi_attentionMark_00Control(void)
{
	return &g_ui_attentionMark_00Control;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_attentionMark_00関数 - UI:注意マーク[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_attentionMark_00(Ui_attentionMark_00 *pUi)
{
	pUi->pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pUi->rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pUi->bUse	= false;							// 使用されているかフラグ
	pUi->bShow	= false;							// 可視フラグ
}

//========================================
// InitParameterUi_attentionMark_00Control関数 - UI:注意マーク[00] の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterUi_attentionMark_00Control(void)
{
	Ui_attentionMark_00Control	*pUiCtl	// UI:注意マーク[00] の管理情報のポインタ
								= &g_ui_attentionMark_00Control;

	pUiCtl->nCounterBrink		= 0;	// 点滅カウンター
	pUiCtl->nCounterArrowMove	= 0;	// 矢印移動カウンター
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_attentionMark_00関数 - UI:注意マーク[00] の初期化処理 -
//========================================
void InitUi_attentionMark_00(void)
{
	// 管理情報の初期化処理
	InitParameterUi_attentionMark_00Control();

	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++)
	{
		D3DXCreateTextureFromFile(pDevice, c_aUi_attentionMark_00Texture[nCntPoly], &g_pTextureUi_attentionMark_00[nCntPoly]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * UI_ATTENTIONMARK_00_MAX * UI_ATTENTIONMARK_00_POLYGON_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_attentionMark_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_attentionMark_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_attentionMark_00	*pUi	// UI:注意マーク[00] の情報のポインタ
						= g_aUi_attentionMark_00;

	for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++, pUi++)
	{
		// パラメーターの初期化処理
		InitParameterUi_attentionMark_00(&g_aUi_attentionMark_00[nCntUi]);

		for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++, pVtx += 4)
		{
			switch (nCntPoly)
			{
			case /*/ 基本 /*/UI_ATTENTIONMARK_00_POLYGON_BASE:
				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), 0.0f, 0.0f);

				// 頂点カラーの設定
				SetVertexColor3D(pVtx, { 255,255,255,255 });

				// テクスチャ座標の設定
				SetTexturePos3D(pVtx, 0, 1, 1, NULL);

				break;
			case /*/ 矢印 /*/UI_ATTENTIONMARK_00_POLYGON_ARROW:
				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, 0.0f);
				pVtx[1].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, -UI_ATTENTIONMARK_00_ARROW_DEPTH);
				pVtx[3].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, -UI_ATTENTIONMARK_00_ARROW_DEPTH);

				// 色を設定
				Color col = GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
					UI_ATTENTIONMARK_00_WARNING_COLOR :
					UI_ATTENTIONMARK_00_COLOR;

				// 頂点カラーの設定
				pVtx[0].col =
				pVtx[1].col = ColorToD3DCOLOR(col);
				col.a = 0;
				pVtx[2].col =
				pVtx[3].col = ColorToD3DCOLOR(col);

				// テクスチャ座標の設定
				SetTexturePos3DCut(pVtx, { 1.0f,1.0f / (float)UI_ATTENTIONMARK_00_ARROW_DIVISION_DEPTH,0.0f }, INITD3DXVECTOR3);

				break;
			}
			
			// 法線ベクトルの設定
			SetNormalLine3D(pVtx, UI_ATTENTIONMARK_00_NOR);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_attentionMark_00->Unlock();
}

//========================================
// UninitUi_attentionMark_00関数 - UI:注意マーク[00] の終了処理 -
//========================================
void UninitUi_attentionMark_00(void)
{
	// テクスチャの破棄
	for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++)
	{
		if (g_pTextureUi_attentionMark_00[nCntPoly] != NULL)
		{
			g_pTextureUi_attentionMark_00[nCntPoly]->Release();
			g_pTextureUi_attentionMark_00[nCntPoly] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_attentionMark_00 != NULL)
	{
		g_pVtxBuffUi_attentionMark_00->Release();
		g_pVtxBuffUi_attentionMark_00 = NULL;
	}
}

//========================================
// UpdateUi_attentionMark_00関数 - UI:注意マーク[00] の更新処理 -
//========================================
void UpdateUi_attentionMark_00(void)
{
	if (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL)
	{// OBJ:ステージ[00] の状態がインターバルの時、
		Ui_attentionMark_00Control	*pUiCtl	// UI:注意マーク[00] の管理情報のポインタ
									= &g_ui_attentionMark_00Control;

		float fAlpha =	// 不透明度を求めつつ、点滅カウンターをカウント
			(float)Count(&pUiCtl->nCounterBrink, 0, UI_ATTENTIONMARK_00_BRINK_TIME, NULL, 0, COUNT_TYPE_TURN) /
			(float)UI_ATTENTIONMARK_00_BRINK_TIME;

		// 矢印の移動カウンターを加算制御
		++pUiCtl->nCounterArrowMove %= UI_ATTENTIONMARK_00_ARROW_MOVE_TIME;

		// 矢印の移動カウンターの進行率
		float fArwMvRate = (float)pUiCtl->nCounterArrowMove / (float)UI_ATTENTIONMARK_00_ARROW_MOVE_TIME;

		VERTEX_3D *pVtx;	//頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffUi_attentionMark_00->Lock(0, 0, (void**)&pVtx, 0);

		Ui_attentionMark_00	*pUi	// UI:注意マーク[00] の情報のポインタ
							= g_aUi_attentionMark_00;

		// 色を設定
		Color col = GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
			UI_ATTENTIONMARK_00_WARNING_COLOR :
			UI_ATTENTIONMARK_00_COLOR;

		// 不透明度を反映
		col.a *= fAlpha;

		for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++, pUi++)
		{
			if (!pUi->bUse)
			{// 使用されている状態でない時、
				pVtx += 4 * UI_ATTENTIONMARK_00_POLYGON_MAX;	// 頂点のポインタを加算
				continue;	// 繰り返し処理を折り返す
			}

			for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++, pVtx += 4)
			{
				switch (nCntPoly)
				{
				case /*/ 基本 /*/UI_ATTENTIONMARK_00_POLYGON_BASE:
					// 頂点カラーを設定
					SetVertexColor3D(pVtx, col);

					break;
				case /*/ 矢印 /*/UI_ATTENTIONMARK_00_POLYGON_ARROW:
					// 頂点カラーの設定
					pVtx[0].col =
					pVtx[1].col = ColorToD3DCOLOR(col);
					Color colTemp = col;
					colTemp.a = 0;
					pVtx[2].col =
					pVtx[3].col = ColorToD3DCOLOR(colTemp);

					// テクスチャ座標の設定
					SetTexturePos3DCut(pVtx, { 1.0f,UI_ATTENTIONMARK_00_ARROW_DIVISION_DEPTH,0.0f }, D3DXVECTOR3(0.0f, -fArwMvRate, 0.0f));

					break;
				}
			}
		}

		// 頂点バッファをアンロックする
		g_pVtxBuffUi_attentionMark_00->Unlock();
	}
}

//========================================
// DrawUi_attentionMark_00関数 - UI:注意マーク[00] の描画処理 -
//========================================
void DrawUi_attentionMark_00(void)
{
	if (GetObj_stage_00()->state != OBJ_STAGE_00_STATE_INTERVAL)
	{// OBJ:ステージ[00] の状態がインターバルのでない時、
		return;	// 処理を終了する
	}

	LPDIRECT3DDEVICE9	pDevice		// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot,		// 計算用マトリックス
						mtxTrans;
	D3DXMATRIX			mtxView;	// ビューマトリックス取得用
	Ui_attentionMark_00	*pUi		// UI:注意マーク[00] の情報のポインタ
						= g_aUi_attentionMark_00;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_attentionMark_00, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++,pUi++)
	{
		if ((!pUi->bUse) || (!pUi->bShow))
		{// 使用されている状態でない or 可視フラグが偽の時、
			// 処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldUi_attentionMark_00);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pUi->rot.y, pUi->rot.x, pUi->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldUi_attentionMark_00, &g_mtxWorldUi_attentionMark_00, &mtxRot);
		
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pUi->pos.x, pUi->pos.y, pUi->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldUi_attentionMark_00, &g_mtxWorldUi_attentionMark_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldUi_attentionMark_00);

		for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureUi_attentionMark_00[nCntPoly]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((4 * UI_ATTENTIONMARK_00_POLYGON_MAX) * nCntUi) + (nCntPoly * 4), 2);
		}
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//========================================
// SetUi_attentionMark_00関数 - UI:注意マーク[00] の設定処理 -
//========================================
void SetUi_attentionMark_00(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	Ui_attentionMark_00	*pUi	// UI:注意マーク[00] の情報のポインタ
						= g_aUi_attentionMark_00;

	for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++, pUi++)
	{
		if (pUi->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		pUi->pos = pos;		// 位置を代入
		pUi->rot = rot;		// 向きを代入
		pUi->bUse = true;	// 使用されている状態にする

		// 繰り返し処理を抜ける
		break;
	}
}