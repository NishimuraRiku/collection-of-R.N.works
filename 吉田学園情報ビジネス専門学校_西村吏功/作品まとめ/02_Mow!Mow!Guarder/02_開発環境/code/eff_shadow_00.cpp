//========================================
// 
// EFF:影[00] の関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_shadow_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon3D.h"
#include "sound.h"
#include "eff_shadow_00.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// EFF:影[00] の最大数
#define EFF_SHADOW_00_MAX	(256)

// EFF:影[00] のテクスチャの相対パス
#define EFF_SHADOW_00_TEXTURE_PATH	"data\\TEXTURE\\Effect\\eff_circle_000.jpg"

// EFF:影[00] の法線ベクトル
// EFF:影[00] の相対位置
#define EFF_SHADOW_00_NOR			(D3DXVECTOR3(0.0f,1.0f,0.0f))
#define EFF_SHADOW_00_RELATIVE_POS	(D3DXVECTOR3(0.0f,0.01f,0.0f))

//****************************************
// 列挙型の定義
//****************************************
// EFF:影[00] の情報構造体
typedef struct 
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
}Eff_shadow_00;

// EFF:影[00] の管理情報構造体
typedef struct
{
	int nUseNum;	// 使用されている数
}Eff_shadow_00Control;

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:影[00] のパラメーターの初期化処理
// EFF:影[00] のパラメーターの管理情報の初期化処理
void InitParameterEff_shadow_00(Eff_shadow_00 *pEff);
void InitParameterEff_shadow_00Control(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureEff_shadow_00		// EFF:影[00] のテクスチャへのポインタ
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_shadow_00		// EFF:影[00] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldEff_shadow_00;	// EFF:影[00] のワールドマトリックス
Eff_shadow_00			g_aEff_shadow_00
						[EFF_SHADOW_00_MAX];		// EFF:影[00] の情報
Eff_shadow_00Control	g_eff_shadow_00Control;		// EFF:影[00] の管理情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_shadow_00関数 - EFF:影[00] のパラメーターの初期化処理 -
//========================================
void InitParameterEff_shadow_00(Eff_shadow_00 *pEff) 
{
	pEff->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pEff->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
}

//========================================
// InitParameterEff_shadow_00Control関数 - EFF:影[00] の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterEff_shadow_00Control(void)
{
	Eff_shadow_00Control	*pEffCtl	// EFF:影[00] の管理情報
							= &g_eff_shadow_00Control;

	pEffCtl->nUseNum = 0;	// 使用されている数
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitEff_shadow_00関数 - EFF:影[00] の初期化処理 -
//========================================
void InitEff_shadow_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, EFF_SHADOW_00_TEXTURE_PATH, &g_pTextureEff_shadow_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFF_SHADOW_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_shadow_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_shadow_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_shadow_00	*pEff	// EFF:影[00] の情報構造体
					= g_aEff_shadow_00;

	for (int nCntEff = 0; nCntEff < EFF_SHADOW_00_MAX; nCntEff++, pVtx += 4)
	{
		// パラメーターの初期化処理
		InitParameterEff_shadow_00(pEff);

		// 法線ベクトルの設定
		SetNormalLine3D(pVtx, EFF_SHADOW_00_NOR);
		
		// 頂点カラーの設定
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_shadow_00->Unlock();
}

//========================================
// UninitEff_shadow_00関数 - EFF:影[00] の終了処理 -
//========================================
void UninitEff_shadow_00(void)
{
	// テクスチャの破棄
	if (g_pTextureEff_shadow_00 != NULL)
	{
		g_pTextureEff_shadow_00->Release();
		g_pTextureEff_shadow_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEff_shadow_00 != NULL)
	{
		g_pVtxBuffEff_shadow_00->Release();
		g_pVtxBuffEff_shadow_00 = NULL;
	}
}

//========================================
// UpdateEff_shadow_00関数 - EFF:影[00] の更新処理 -
//========================================
void UpdateEff_shadow_00(void)
{

}

//========================================
// DrawEff_shadow_00関数 - EFF:影[00] の描画処理 -
//========================================
void DrawEff_shadow_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	Eff_shadow_00			*pEff		// EFF:影[00] の情報構造体
							= g_aEff_shadow_00;
	Eff_shadow_00Control	*pEffCtl	// EFF:影[00] の管理情報
							= &g_eff_shadow_00Control;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEff_shadow_00, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEff_shadow_00);
	
	for (int nCntEff = 0; nCntEff < pEffCtl->nUseNum; nCntEff++, pEff++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldEff_shadow_00);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pEff->rot.y, pEff->rot.x, pEff->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shadow_00, &g_mtxWorldEff_shadow_00, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pEff->pos.x, pEff->pos.y, pEff->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shadow_00, &g_mtxWorldEff_shadow_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEff_shadow_00);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEff, 2);
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_eff_shadow_00Control.nUseNum = 0;	// 使用されている数を初期化
}

//========================================
// SetEff_shadow_00関数 - EFF:影[00] の設定処理 -
//========================================
void SetEff_shadow_00(D3DXVECTOR3 pos, float fRadius) 
{
	Eff_shadow_00			*pEff		// EFF:影[00] の情報構造体
							= g_aEff_shadow_00;
	Eff_shadow_00Control	*pEffCtl	// EFF:影[00] の管理情報
							= &g_eff_shadow_00Control;

	pEff[pEffCtl->nUseNum].pos 	// 位置を代入
		= pos + EFF_SHADOW_00_RELATIVE_POS;

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_shadow_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetVertexPos3D(&pVtx[pEffCtl->nUseNum * 4], fRadius * 2, fRadius * 2);

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_shadow_00->Unlock();

	pEffCtl->nUseNum++;			// 使用されている数を加算
}