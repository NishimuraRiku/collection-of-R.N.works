//========================================
// 
// UI:マップ[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_map_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"			// CHR:プレイヤー	[00]
#include "chr_enemy_00.h"			// CHR:敵			[00]
#include "obj_core_00.h"			// OBJ:コア			[00]
#include "obj_stage_00.h"			// OBJ:ステージ		[00]
#include "ui_attention-mark_00.h"	// UI :注意マーク	[00]
#include "ui_map_00.h"				// UI :マップ		[00]

//****************************************
// マクロ定義
//****************************************
// UI:マップ[00] の位置
// UI:マップ[00] の切り取りスケール
// UI:マップ[00] のテクスチャの位置変換値
// UI:マップ[00] の位置変換値
// UI:マップ[00] のマップの色
#define UI_MAP_00_POS				D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*32),SCREEN_HEIGHT-(PIXEL*30),0.0f)
#define UI_MAP_00_CUT_SCALE			Scale{0.5f,0.5f,0.0f}
#define UI_MAP_00_TEXTURE_POS_CONV	(1.0f/8.0f/64.0f)
#define UI_MAP_00_POS_CONV			((UI_MAP_00_TEXTURE_POS_CONV*PIXEL*48)/0.5f)
#define UI_MAP_00_MAP_COLOR			Color{100,100,100,255}

//****************************************
// 列挙型の定義
//****************************************
// UI:マップ[00] のポリゴン
typedef enum 
{
	UI_MAP_00_POLYGON_MAP,			// マップ
	UI_MAP_00_POLYGON_ICON_CORE,	// アイコン(コア)
	UI_MAP_00_POLYGON_ICON_ENEMY,	// アイコン(敵)
	UI_MAP_00_POLYGON_ICON_PLAYER,	// アイコン(プレイヤー)
	UI_MAP_00_POLYGON_ICON_WARNING,	// アイコン(警告)
	UI_MAP_00_POLYGON_FRAME,		// フレーム
	UI_MAP_00_POLYGON_MAX,
}UI_MAP_00_POLYGON;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_map_00	// テクスチャへのポインタ
						[UI_MAP_00_POLYGON_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_map_00	// 頂点バッファへのポインタ
						= NULL;

// UI:マップ[00] のポリゴン毎の情報
const Polygon2D g_aUi_map_00Polygon[UI_MAP_00_POLYGON_MAX] = 
{
	{ "data\\TEXTURE\\UserInterface\\ui_stage-map_000.png"    ,PIXEL * 48,PIXEL * 48,1                     },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\core.png"   ,PIXEL * 4 ,PIXEL * 4 ,1                     },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\enemy.png"  ,PIXEL * 2 ,PIXEL * 2 ,CHR_ENEMY_00_MAX      },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\player.png" ,PIXEL * 4 ,PIXEL * 4 ,1                     },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\warning.png",PIXEL * 8 ,PIXEL * 8 ,OBJ_STAGE_00_GATE_MAX },
	{ "data\\TEXTURE\\UserInterface\\ui_map-frame_000.png"    ,PIXEL * 64,PIXEL * 64,1                     },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_map_00関数 - UI:マップ[00] の初期化処理 -
//========================================
void InitUi_map_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	LoadPolygon2DTexture(pDevice, g_aTextureUi_map_00, g_aUi_map_00Polygon, UI_MAP_00_POLYGON_MAX);
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * GetPolygon2DNum(g_aUi_map_00Polygon, UI_MAP_00_POLYGON_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_map_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_map_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// ポリゴン毎の情報
						= &g_aUi_map_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPos2D(pVtx,
				UI_MAP_00_POS,
				INITD3DXVECTOR3,
				false,
				pPoly->fWidth,
				pPoly->fHeight,
				ANGLE_TYPE_FIXED);

			// rhwの設定
			SetRHW2D(pVtx);

			switch (nCntPoly)
			{
			case /*/ マップ /*/UI_MAP_00_POLYGON_MAP:
				// テクスチャ座標の設定
				SetTexturePos2DCut(pVtx, UI_MAP_00_CUT_SCALE, ConvPosToBirdsEyeView(GetChr_player_00()->pos * UI_MAP_00_TEXTURE_POS_CONV));
				
				// 頂点カラーの設定
				SetVertexColor2D(pVtx, UI_MAP_00_MAP_COLOR);

				break;
			case/*/ アイコン(コア)			/*/ UI_MAP_00_POLYGON_ICON_CORE:
			case/*/ アイコン(敵)			/*/ UI_MAP_00_POLYGON_ICON_ENEMY:
			case/*/ アイコン(プレイヤー)	/*/ UI_MAP_00_POLYGON_ICON_PLAYER:
			case/*/ アイコン(警告)			/*/ UI_MAP_00_POLYGON_ICON_WARNING:
			case/*/ フレーム				/*/ UI_MAP_00_POLYGON_FRAME:
				// テクスチャ座標の設定
				SetTexturePos2D(pVtx, 0, 1, 1, NULL);

				// 頂点カラーの設定
				SetVertexColor2D(pVtx, INITCOLOR);

				break;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_map_00->Unlock();
}

//========================================
// UninitUi_map_00関数 - UI:マップ[00] の終了処理 -
//========================================
void UninitUi_map_00(void)
{
	// テクスチャの破棄
	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		if (g_aTextureUi_map_00[nCntPoly] != NULL)
		{
			g_aTextureUi_map_00[nCntPoly]->Release();
			g_aTextureUi_map_00[nCntPoly] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_map_00 != NULL) 
	{
		g_pVtxBuffUi_map_00->Release();
		g_pVtxBuffUi_map_00 = NULL;
	}
}

//========================================
// UpdateUi_map_00関数 - UI:マップ[00] の更新処理 -
//========================================
void UpdateUi_map_00(void)
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_map_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly		// ポリゴン毎の情報
						= &g_aUi_map_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			switch (nCntPoly)
			{
			case /*/ マップ /*/UI_MAP_00_POLYGON_MAP:
				// テクスチャ座標の設定
				SetTexturePos2DCut(pVtx, UI_MAP_00_CUT_SCALE, ConvPosToBirdsEyeView(GetChr_player_00()->pos * UI_MAP_00_TEXTURE_POS_CONV));

				break;
			case/*/ アイコン(敵) /*/ UI_MAP_00_POLYGON_ICON_ENEMY:
				if ((!GetChr_enemy_00()[nCntPolyIdx].bUse) || (GetChr_enemy_00()[nCntPolyIdx].bRecognitionOff))
				{// カウントのCHR:敵[00] が使用されていない状態 or 認識OFFフラグが真の時、
					break;	// Switch文を抜ける
				}

				// 頂点座標を設定
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS +
					ConvPosToBirdsEyeView((GetChr_enemy_00()[nCntPolyIdx].pos - GetChr_player_00()->pos) * UI_MAP_00_POS_CONV),
					INITD3DXVECTOR3,
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FIXED);

				break;
			case/*/ アイコン(コア) /*/ UI_MAP_00_POLYGON_ICON_CORE:
				// 頂点座標を設定
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS +
					ConvPosToBirdsEyeView((GetObj_core_00()->pos - GetChr_player_00()->pos) * UI_MAP_00_POS_CONV),
					INITD3DXVECTOR3,
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FIXED);

				break;
			case/*/ アイコン(プレイヤー) /*/ UI_MAP_00_POLYGON_ICON_PLAYER:
				// 頂点座標を設定
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS,
					ConvRotToBirdsEyeView(GetChr_player_00()->rot),
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FREE);

				break;
			case/*/ アイコン(警告)	/*/ UI_MAP_00_POLYGON_ICON_WARNING:
				if (!GetUi_attentionMark_00()[nCntPolyIdx].bUse)
				{// カウントのUI:注意マーク[00] が使用されていない状態の時、
					break;	// Switch文を抜ける
				}

				// 頂点座標を設定
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS +
					ConvPosToBirdsEyeView((GetUi_attentionMark_00()[nCntPolyIdx].pos - GetChr_player_00()->pos) * UI_MAP_00_POS_CONV),
					INITD3DXVECTOR3,
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FIXED);

				break;
			case/*/ フレーム /*/ UI_MAP_00_POLYGON_FRAME:

				break;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_map_00->Unlock();
}

//========================================
// DrawUi_map_00関数 - UI:マップ[00] の描画処理 -
//========================================
void DrawUi_map_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_map_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// ポリゴン毎の情報
			= &g_aUi_map_00Polygon[nCntPoly];

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_map_00[nCntPoly]);

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++)
		{
			switch (nCntPoly)
			{
			case/*/ アイコン(敵) /*/ UI_MAP_00_POLYGON_ICON_ENEMY:
				if (!GetChr_enemy_00()[nCntPolyIdx].bUse)
				{// カウントのCHR:敵[00] が使用されていない状態の時、
					continue;	// 繰り返し処理を折り返す
				}
				break;
			case/*/ アイコン(警告)	/*/ UI_MAP_00_POLYGON_ICON_WARNING:
				if ((!GetUi_attentionMark_00()[nCntPolyIdx].bUse) || (!GetUi_attentionMark_00()[nCntPolyIdx].bShow) || (GetObj_stage_00()->state != OBJ_STAGE_00_STATE_INTERVAL))
				{// カウントのUI:注意マーク[00] が使用されていない or 可視フラグが偽 or  OBJ:ステージ[00] の状態がインターバルのでない時、
					continue;	// 繰り返し処理を折り返す
				}
				break;
			}

			// UI:マップ[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (GetPolygon2DNum(g_aUi_map_00Polygon, nCntPoly) + nCntPolyIdx), 2);
		}
	}
}