//========================================
// 
// UI:HPバー[01] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_hp-bar_01.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "obj_core_00.h"	// OBJ:コア		[00]
#include "ui_hp-bar_01.h"	// UI :HPバー	[01]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// UI:HPバー[01] の法線ベクトル
// UI:HPバー[01] の相対位置
#define UI_HPBAR_01_NOR				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define UI_HPBAR_01_RELATIVE_POS	(D3DXVECTOR3(0.0f, 20.0f, 0.0f))

// UI:HPバー[01] のバー部分の相対位置
// UI:HPバー[01] のバー部分の色
#define UI_HPBAR_01_BAR_RELATIVE_POS	(D3DXVECTOR3(PIXEL3D * -27, PIXEL3D * 3, 0.0f))
#define UI_HPBAR_01_BAR_COLOR			{243,191,63,255}

//****************************************
// 列挙型の定義
//****************************************
// HPバー[01] の部品番号
typedef enum
{
	UI_HPBAR_01_PARTS_FRAME,	// フレーム
	UI_HPBAR_01_PARTS_BAR,		// バー
	UI_HPBAR_01_PARTS_MAX,
}UI_HPBAR_01_PARTS;

// HPバー[01] のテクスチャ番号
typedef enum
{
	UI_HPBAR_01_TEXTURE_FRAME,	// フレーム
	UI_HPBAR_01_TEXTURE_BAR,	// バー
	UI_HPBAR_01_TEXTURE_MAX,
}UI_HPBAR_01_TEXTURE;

//****************************************
// 構造体の定義
//****************************************
// HPバー[01] の部品毎の情報構造体
typedef struct
{
	UI_HPBAR_01_TEXTURE texture;	// テクスチャ
	int					nPattern;	// パターンNo.
	float				fWidth;		// 幅
	float				fHeight;	// 高さ
}Ui_hpBar_01Parts;

// HPバー[01] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath[TXT_MAX];	// テクスチャファイルの相対パス
	int		nPtnWidth;				// パターン幅
	int		nPtnHeight;				// パターン高さ
}Ui_hpBar_01Texture;

//****************************************
// プロトタイプ宣言
//****************************************
// HPバー[01] の部品毎の頂点座標設定処理
void UpdatePartsUi_hpBar_01(VERTEX_3D *pVtx, UI_HPBAR_01_PARTS parts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_hpBar_01	// UI:HPバー[01] のテクスチャへのポインタ
						[UI_HPBAR_01_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_hpBar_01	// UI:HPバー[01] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldUi_hpBar_01;	// UI:HPバー[01] のワールドマトリックス

// HPバー[01] のテクスチャ毎の情報
const Ui_hpBar_01Texture g_aUi_hpBar_01Texture[UI_HPBAR_01_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_hp-frame_000.png" , 1, 1 },	// フレーム
	{ ""                                                  , 2, 1 },	// バー
};

// HPバー[01] の部品毎の情報
const Ui_hpBar_01Parts g_aUi_hpBar_01Parts[UI_HPBAR_01_PARTS_MAX] =
{
	{ UI_HPBAR_01_TEXTURE_FRAME, 0, PIXEL3D * 64 , PIXEL3D * 16 },	// フレーム
	{ UI_HPBAR_01_TEXTURE_BAR  , 0, PIXEL3D * 54 , PIXEL3D * 4  },	// バー
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_hpBar_01関数 - HPバー[01] の部品毎の頂点座標設定処理 -
//========================================
void UpdatePartsUi_hpBar_01(VERTEX_3D *pVtx, UI_HPBAR_01_PARTS parts)
{
	float					fBarLengthDiameter	// バーの長さの倍率
							= ((float)GetObj_core_00()->nHP / (float)GetObj_core_00()->nHPMax);
	const Ui_hpBar_01Parts	*pParts				// 部品毎の情報のポインタ
							= &g_aUi_hpBar_01Parts[parts];

	switch (parts)
	{
		//========== *** フレーム ***
	case UI_HPBAR_01_PARTS_FRAME:
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(pParts->fWidth * 0.5f),  (pParts->fHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (pParts->fWidth * 0.5f),  (pParts->fHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(pParts->fWidth * 0.5f), -(pParts->fHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (pParts->fWidth * 0.5f), -(pParts->fHeight * 0.5f), 0.0f);
	}
	break;
		//========== *** バー ***
	case UI_HPBAR_01_PARTS_BAR:
	{
		D3DXVECTOR3 posR = UI_HPBAR_01_BAR_RELATIVE_POS;	// 相対位置

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(posR.x											, posR.y + (pParts->fHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posR.x + (pParts->fWidth * fBarLengthDiameter)	, posR.y + (pParts->fHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posR.x											, posR.y - (pParts->fHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posR.x + (pParts->fWidth * fBarLengthDiameter)	, posR.y - (pParts->fHeight * 0.5f), 0.0f);
	}
	break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_hpBar_01関数 - UI:HPバー[01] の初期化処理 -
//========================================
void InitUi_hpBar_01(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < UI_HPBAR_01_TEXTURE_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_hpBar_01Texture[nCntTexture].aTexturePath, &g_aTextureUi_hpBar_01[nCntTexture]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * UI_HPBAR_01_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_hpBar_01,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_hpBar_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_01_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 法線ベクトルの設定
		SetNormalLine3D(pVtx, UI_HPBAR_01_NOR);

		// 頂点カラーの設定
		if (nCntParts == UI_HPBAR_01_PARTS_BAR) {
			SetVertexColor3D(pVtx, UI_HPBAR_01_BAR_COLOR);
		}
		else {
			SetVertexColor3D(pVtx, { 255,255,255,255 });
		}

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx,
			g_aUi_hpBar_01Parts[nCntParts].nPattern,
			g_aUi_hpBar_01Texture[g_aUi_hpBar_01Parts[nCntParts].texture].nPtnWidth,
			g_aUi_hpBar_01Texture[g_aUi_hpBar_01Parts[nCntParts].texture].nPtnHeight,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_hpBar_01->Unlock();
}

//========================================
// UninitUi_hpBar_01関数 - UI:HPバー[01] の終了処理 -
//========================================
void UninitUi_hpBar_01(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < UI_HPBAR_01_TEXTURE_MAX; nCntType++)
	{
		if (g_aTextureUi_hpBar_01[nCntType] != NULL)
		{
			g_aTextureUi_hpBar_01[nCntType]->Release();
			g_aTextureUi_hpBar_01[nCntType] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_hpBar_01 != NULL)
	{
		g_pVtxBuffUi_hpBar_01->Release();
		g_pVtxBuffUi_hpBar_01 = NULL;
	}
}

//========================================
// UpdateUi_hpBar_01関数 - UI:HPバー[01] の更新処理 -
//========================================
void UpdateUi_hpBar_01(void)
{
	VERTEX_3D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_hpBar_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_01_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// 部品毎の更新処理
		UpdatePartsUi_hpBar_01(pVtx, (UI_HPBAR_01_PARTS)nCntParts);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_hpBar_01->Unlock();
}

//========================================
// DrawUi_hpBar_01関数 - UI:HPバー[01] の描画処理 -
//========================================
void DrawUi_hpBar_01(void)
{
	LPDIRECT3DDEVICE9	pDevice		// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// 計算用マトリックス
	D3DXMATRIX			mtxView;	// ビューマトリックス取得用

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_hpBar_01, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_01_PARTS_MAX; nCntParts++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldUi_hpBar_01);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_mtxWorldUi_hpBar_01, NULL, &mtxView);
		g_mtxWorldUi_hpBar_01._41 = 0.0f;
		g_mtxWorldUi_hpBar_01._42 = 0.0f;
		g_mtxWorldUi_hpBar_01._43 = 0.0f;
		{
			// 位置を反映
			D3DXVECTOR3 setPos = GetObj_core_00()->pos + UI_HPBAR_01_RELATIVE_POS;
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldUi_hpBar_01, &g_mtxWorldUi_hpBar_01, &mtxTrans);
		}
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldUi_hpBar_01);

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_hpBar_01[g_aUi_hpBar_01Parts[nCntParts].texture]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParts, 2);
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}