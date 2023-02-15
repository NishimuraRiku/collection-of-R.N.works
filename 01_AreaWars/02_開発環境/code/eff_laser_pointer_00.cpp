//========================================
// 
// レーザーポインタ―[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** eff_laser_pointer_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "setting.h"
#include "sound.h"
#include "polygon.h"
#include "chr_fighter_00.h"
#include "md_title_00.h"
#include "eff_laser_pointer_00.h"

//****************************************
// マクロ定義
//****************************************

// レーザーポインタ―[00] の消えるのにかかる時間
#define EFF_LASER_POINTER_00_CLEAR_TIME	(15)

//****************************************
// 列挙型の定義
//****************************************

// レーザーポインタ―[00] の部品番号
typedef enum
{
	EFF_LASER_POINTER_00_PARTS_LASER_POINTER = 0,	// レーザーポインタ―
	EFF_LASER_POINTER_00_PARTS_MAX,
}EFF_LASER_POINTER_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// レーザーポインタ―[00] のポリゴン毎の情報構造体
typedef struct
{
	char	aTexturePath	// テクスチャファイルの相対パス
			[TXT_MAX];		// 
	int		nPtnMaxX;		// パターンの上限(横)
	int		nPtnMaxY;		// パターンの上限(縦)
	int		nPtnMin;		// パターンNo.の下限
	int		nPtnMax;		// パターンNo.の上限
	int		nAnimTime;		// アニメーションにかかる時間
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	int		nPolygonNum;	// 必要ポリゴン数

	int		nPtn;			// パターンNo.
	int		nCounterAnim;	// アニメーションのカウンター
}Eff_laser_pointer_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterEff_laser_pointer_00(void);

// 部品(レーザーポインタ―)の更新処理
void UpdateEff_laser_pointer_00Parts_top_arrow(void);

// 状態に応じた開始処理
void StartStateProcessEff_laser_pointer_00(void);

// 状態に応じた終了処理
void EndStateProcessEff_laser_pointer_00(void);

// 状態に応じた更新処理
void UpdateStateProcessEff_laser_pointer_00(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumEff_laser_pointer_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_laser_pointer_00		// テクスチャへのポインタ
						[EFF_LASER_POINTER_00_PARTS_MAX]	//
						= {};								//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_laser_pointer_00		// 頂点バッファへのポインタ
						= NULL;								//
Eff_laser_pointer_00	g_eff_laser_pointer_00;				// レーザーポインタ―[00] の情報

// レーザーポインタ―[00] の部品毎の情報
Eff_laser_pointer_00Parts g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\eff_laser_pointer_000_red.jpg" , 1 , 1 , 0 , 0 , -1 , PIXEL * 2 , PIXEL * 128 , 1 },
};

//========== *** レーザーポインタ―[00] 情報を取得 ***
Eff_laser_pointer_00 *GetEff_laser_pointer_00(void)
{
	return &g_eff_laser_pointer_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_laser_pointer_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterEff_laser_pointer_00(void) 
{
	g_eff_laser_pointer_00.rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	g_eff_laser_pointer_00.state			= EFF_LASER_POINTER_00_STATE_CLEAR;	// 状態
	g_eff_laser_pointer_00.nCounterState	= 0;								// 状態カウンター
	g_eff_laser_pointer_00.fAlpha			= 0.0f;								// 透明度
}

//========================================
// UpdateEff_laser_pointer_00Parts_top_arrow関数 - 部品(レーザーポインタ―)の更新処理 -
//========================================
void UpdateEff_laser_pointer_00Parts_top_arrow(void)
{
	Eff_laser_pointer_00Parts	*pParts										// 
								= &g_aEff_laser_pointer_00Parts				//
								[EFF_LASER_POINTER_00_PARTS_LASER_POINTER];	// 部品の情報のポインタ
	VERTEX_2D					*pVtx;										// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_laser_pointer_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumEff_laser_pointer_00(EFF_LASER_POINTER_00_PARTS_LASER_POINTER));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		D3DXVECTOR3 setPos						// 
					= GetChr_fighter_00()->pos;	// 設定位置

		// 設定位置を向きに半高さ分加算
		setPos.x += sinf(g_eff_laser_pointer_00.rot.z)
			* (g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fHeight * 0.5f);
		setPos.y += cosf(g_eff_laser_pointer_00.rot.z)
			* (g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fHeight * 0.5f);

		// 頂点座標を設定
		SetVertexPosition(pVtx,
			setPos,
			g_eff_laser_pointer_00.rot,
			true,
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fWidth,
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fHeight,
			ANGLE_TYPE_FREE);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtn,
				g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMin,
				g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMax,
				&g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nCounterAnim,
				g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMaxX,
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMaxY,
			NULL);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_eff_laser_pointer_00.fAlpha) });
	}
}

//========================================
// StartStateProcessEff_laser_pointer_00関数 - 状態に応じた開始処理 -
//========================================
void StartStateProcessEff_laser_pointer_00(void) 
{
	Eff_laser_pointer_00	*pEff						// 
							= &g_eff_laser_pointer_00;	// レーザーポインタ―[00] の情報のポインタ 

	// 状態カウンターを初期化
	pEff->nCounterState = 0;

	switch (pEff->state)
	{
		//========== *** 出現 ***
	case EFF_LASER_POINTER_00_STATE_POP:
	{
		// 透明度を設定
		pEff->fAlpha = 1.0f;
	}
		break;
		//========== *** 消え中 ***
	case EFF_LASER_POINTER_00_STATE_IN_CLEAR:
	{
		// 目標向きを機体[00] から代入
		pEff->rot = GetChr_fighter_00()->targetRot;
	}
		break;
		//========== *** 消えている ***
	case EFF_LASER_POINTER_00_STATE_CLEAR:
	{
		// 透明度を設定
		pEff->fAlpha = 0.0f;
	}
		break;
	}
}

//========================================
// EndStateProcessEff_laser_pointer_00関数 - 状態に応じた終了処理 -
//========================================
void EndStateProcessEff_laser_pointer_00(void) 
{
	Eff_laser_pointer_00	*pEff						// 
							= &g_eff_laser_pointer_00;	// レーザーポインタ―[00] の情報のポインタ 

	switch (pEff->state)
	{
		//========== *** 出現 ***
	case EFF_LASER_POINTER_00_STATE_POP:
	{

	}
	break;
		//========== *** 消え中 ***
	case EFF_LASER_POINTER_00_STATE_IN_CLEAR:
	{

	}
	break;
		//========== *** 消えている ***
	case EFF_LASER_POINTER_00_STATE_CLEAR:
	{

	}
	break;
	}
}

//========================================
// UpdateStateProcessEff_laser_pointer_00関数 - 状態に応じた更新処理 -
//========================================
void UpdateStateProcessEff_laser_pointer_00(void) 
{
	Eff_laser_pointer_00	*pEff						// 
							= &g_eff_laser_pointer_00;	// レーザーポインタ―[00] の情報のポインタ 

	switch (pEff->state)
	{
		//========== *** 出現 ***
	case EFF_LASER_POINTER_00_STATE_POP:
	{

	}
	break;
		//========== *** 消え中 ***
	case EFF_LASER_POINTER_00_STATE_IN_CLEAR:
	{
		// 透明度を更新
		pEff->fAlpha = (1.0f - ((float)pEff->nCounterState / (float)EFF_LASER_POINTER_00_CLEAR_TIME));

		if (++pEff->nCounterState >= EFF_LASER_POINTER_00_CLEAR_TIME) 
		{// 状態カウンターを加算した結果消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE_CLEAR);
		}
	}
	break;
		//========== *** 消えている ***
	case EFF_LASER_POINTER_00_STATE_CLEAR:
	{

	}
	break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumEff_laser_pointer_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// 必要ポリゴン数

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aEff_laser_pointer_00Parts[nCntPolygon].nPolygonNum;
	}

	// 必要ポリゴン数を返す
	return nPolygonNum;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitEff_laser_pointer_00関数 - レーザーポインタ―[00] の初期化処理 -
//========================================
void InitEff_laser_pointer_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumEff_laser_pointer_00(EFF_LASER_POINTER_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_laser_pointer_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_laser_pointer_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aEff_laser_pointer_00Parts[nCntParts].aTexturePath, &g_aTextureEff_laser_pointer_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aEff_laser_pointer_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aEff_laser_pointer_00Parts[nCntParts].fWidth,
				g_aEff_laser_pointer_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx,
				0,
				g_aEff_laser_pointer_00Parts[nCntParts].nPtnMaxX,
				g_aEff_laser_pointer_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffEff_laser_pointer_00->Unlock();
}

//========================================
// UninitEff_laser_pointer_00関数 - レーザーポインタ―[00] の終了処理 -
//========================================
void UninitEff_laser_pointer_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureEff_laser_pointer_00[nCntParts] != NULL)
		{
			g_aTextureEff_laser_pointer_00[nCntParts]->Release();
			g_aTextureEff_laser_pointer_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEff_laser_pointer_00 != NULL)
	{
		g_pVtxBuffEff_laser_pointer_00->Release();
		g_pVtxBuffEff_laser_pointer_00 = NULL;
	}
}

//========================================
// UpdateEff_laser_pointer_00関数 - レーザーポインタ―[00] の更新処理 -
//========================================
void UpdateEff_laser_pointer_00(void)
{
	Eff_laser_pointer_00	*pUi						//
							= &g_eff_laser_pointer_00;	// レーザーポインタ―[00] の情報のポインタ

	// 状態に応じた更新処理
	UpdateStateProcessEff_laser_pointer_00();

	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** レーザーポインタ― ***
		case EFF_LASER_POINTER_00_PARTS_LASER_POINTER:
		{
			// 部品(レーザーポインタ―)の更新処理
			UpdateEff_laser_pointer_00Parts_top_arrow();
		}
		break;
		}
	}
}

//========================================
// DrawEff_laser_pointer_00関数 - レーザーポインタ―[00] の描画処理 -
//========================================
void DrawEff_laser_pointer_00(void)
{
	if (!GetSetting()->bLaser) 
	{// 設定の光線フラグが偽の時、
		// 処理を終了する
		return;
	}

	Eff_laser_pointer_00	*pUi						//
							= &g_eff_laser_pointer_00;	// レーザーポインタ―[00] の情報のポインタ

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEff_laser_pointer_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureEff_laser_pointer_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aEff_laser_pointer_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// レーザーポインタ―[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}

	// アルファブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// SetStateEff_laser_pointer_00関数 - レーザーポインタ―[00] の状態設定処理 -
//========================================
void SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE state)
{
	Eff_laser_pointer_00	*pUi						//
							= &g_eff_laser_pointer_00;	// レーザーポインタ―[00] の情報のポインタ 

	// 状態に応じた終了処理
	EndStateProcessEff_laser_pointer_00();

	// 状態を代入
	pUi->state = state;

	// 状態に応じた開始処理
	StartStateProcessEff_laser_pointer_00();

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}