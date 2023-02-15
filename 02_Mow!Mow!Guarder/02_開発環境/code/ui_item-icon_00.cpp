//========================================
// 
// UI:アイテムアイコン[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_item-icon_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "obj_pedestal_00.h"	// OBJ:台座				[00]
#include "obj_stage_00.h"		// OBJ:ステージ			[00]
#include "ui_item-icon_00.h"	// UI :アイテムアイコン	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// UI:アイテムアイコン[00] の最大数
#define UI_ITEMICON_00_MAX	(OBJ_PEDESTAL_00_MAX)

// UI:アイテムアイコン[00] の幅
// UI:アイテムアイコン[00] の高さ
#define UI_ITEMICON_00_WIDTH	(PIXEL3D * 16)
#define UI_ITEMICON_00_HEIGHT	(PIXEL3D * 16)

// UI:アイテムアイコン[00] の法線ベクトル
// UI:アイテムアイコン[00] の相対位置
#define UI_ITEMICON_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define UI_ITEMICON_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 6.0f, 0.0f))

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_itemIcon_00	// UI:アイテムアイコン[00] のテクスチャへのポインタ
						[OBJ_PEDESTAL_00_ITEM_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_itemIcon_00	// UI:アイテムアイコン[00] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldUi_itemIcon_00;	// UI:アイテムアイコン[00] のワールドマトリックス

// UI:アイテムアイコン[00] のテクスチャの相対パス
const char g_aTexturePathUi_itemIcon_00[OBJ_PEDESTAL_00_ITEM_MAX][TXT_MAX] = 
{
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\block.png",			// ブロック
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\turret.png",			// タレット
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\reflector.png",		// 反射装置
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\discharger.png",		// 放電装置
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\player-hp-prus.png",	// プレイヤーのHPを回復
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\unit-hp-prus.png",		// ユニットのHPを回復
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\sword-atk-up.png",		// 剣の攻撃力アップ
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\bullet-atk-up.png",	// 弾の攻撃力アップ
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_itemIcon_00関数 - UI:アイテムアイコン[00] の初期化処理 -
//========================================
void InitUi_itemIcon_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	for (int nCntItem = 0; nCntItem < OBJ_PEDESTAL_00_ITEM_MAX; nCntItem++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aTexturePathUi_itemIcon_00[nCntItem], &g_aTextureUi_itemIcon_00[nCntItem]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * UI_ITEMICON_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_itemIcon_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_itemIcon_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < UI_ITEMICON_00_MAX; nCntUi++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(UI_ITEMICON_00_WIDTH * 0.5f),  (UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (UI_ITEMICON_00_WIDTH * 0.5f),  (UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(UI_ITEMICON_00_WIDTH * 0.5f), -(UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (UI_ITEMICON_00_WIDTH * 0.5f), -(UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);

		// 法線ベクトルの設定
		SetNormalLine3D(pVtx, UI_ITEMICON_00_NOR);

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_itemIcon_00->Unlock();
}

//========================================
// UninitUi_itemIcon_00関数 - UI:アイテムアイコン[00] の終了処理 -
//========================================
void UninitUi_itemIcon_00(void)
{
	// テクスチャの破棄
	for (int nCntItem = 0; nCntItem < OBJ_PEDESTAL_00_ITEM_MAX; nCntItem++)
	{
		if (g_aTextureUi_itemIcon_00[nCntItem] != NULL)
		{
			g_aTextureUi_itemIcon_00[nCntItem]->Release();
			g_aTextureUi_itemIcon_00[nCntItem] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_itemIcon_00 != NULL)
	{
		g_pVtxBuffUi_itemIcon_00->Release();
		g_pVtxBuffUi_itemIcon_00 = NULL;
	}
}

//========================================
// UpdateUi_itemIcon_00関数 - UI:アイテムアイコン[00] の更新処理 -
//========================================
void UpdateUi_itemIcon_00(void)
{
	VERTEX_3D	*pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_itemIcon_00->Lock(0, 0, (void**)&pVtx, 0);

	Obj_pedestal_00 *pObj	// OBJ:台座[00] の情報のポインタを取得
					= GetObj_pedestal_00();

	float	fArrivalRate	// 到着率
			= (float)GetObj_pedestal_00Control()->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME;

	for (int nCntUi = 0; nCntUi < UI_ITEMICON_00_MAX; nCntUi++, pObj++, pVtx += 4)
	{
		if (!pObj->bUse)
		{// カウントのOBJ:台座[00] が使用されていない時、
			continue;	// 繰り返し処理を折り返す
		}

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, { 255,255,255,(int)(255 * fArrivalRate) });
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_itemIcon_00->Unlock();
}

//========================================
// DrawUi_itemIcon_00関数 - UI:アイテムアイコン[00] の描画処理 -
//========================================
void DrawUi_itemIcon_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// 計算用マトリックス
	D3DXMATRIX			mtxView;	// ビューマトリックス取得用

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_itemIcon_00, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	Obj_pedestal_00 *pObj = GetObj_pedestal_00();	// OBJ:台座[00] の情報のポインタを取得

	for (int nCntUi = 0; nCntUi < UI_ITEMICON_00_MAX; nCntUi++, pObj++)
	{
		if (!pObj->bUse)
		{// カウントのOBJ:台座[00] が使用されていない時、
			continue;	// 繰り返し処理を折り返す
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldUi_itemIcon_00);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_mtxWorldUi_itemIcon_00, NULL, &mtxView);
		g_mtxWorldUi_itemIcon_00._41 = 0.0f;
		g_mtxWorldUi_itemIcon_00._42 = 0.0f;
		g_mtxWorldUi_itemIcon_00._43 = 0.0f;
		{
			D3DXVECTOR3 setPos = pObj->pos + UI_ITEMICON_00_RELATIVE_POS;	// 設定位置を設定

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldUi_itemIcon_00, &g_mtxWorldUi_itemIcon_00, &mtxTrans);
		}
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldUi_itemIcon_00);

		Obj_stage_00Type *pObj = GetObj_stage_00Type();

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_itemIcon_00[
			GetObj_stage_00Type()[GetObj_stage_00()->nType].itemsSet.aItems[GetObj_pedestal_00Control()->nCntItems].aItem[nCntUi]]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUi, 2);
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}