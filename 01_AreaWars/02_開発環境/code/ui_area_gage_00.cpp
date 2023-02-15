//========================================
// 
// エリアゲージ[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_area_gage_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "sys_stage_00.h"
#include "ui_area_gage_00.h"

//****************************************
// マクロ定義
//****************************************

// エリアゲージ[00] の位置
// エリアゲージ[00] の幅
// エリアゲージ[00] の棒部分の幅
#define UI_AREA_GAGE_00_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT - PIXEL * 6), 0.0f))
#define UI_AREA_GAGE_00_WIDTH		(960)
#define UI_AREA_GAGE_00_BAR_WIDTH	(960 - PIXEL * 6)

//****************************************
// 列挙型の定義
//****************************************

// エリアゲージ[00] の部品番号
typedef enum
{
	UI_AREA_GAGE_00_PARTS_BACK_FRAME = 0,	// 奥の枠
	UI_AREA_GAGE_00_PARTS_BAR,				// 棒
	UI_AREA_GAGE_00_PARTS_BAR_RIGHT,		// 棒の右端
	UI_AREA_GAGE_00_PARTS_FRAME,			// 枠
	UI_AREA_GAGE_00_PARTS_FRAME_LEFT,		// 枠の左端
	UI_AREA_GAGE_00_PARTS_FRAME_RIGHT,		// 枠の右端
	UI_AREA_GAGE_00_PARTS_MAX,
}UI_AREA_GAGE_00_PARTS;

// エリアゲージ[00] のテクスチャ番号
typedef enum 
{
	UI_AREA_GAGE_00_TEXTURE_BAR = 0,	// 棒
	UI_AREA_GAGE_00_TEXTURE_FRAME,		// 枠
	UI_AREA_GAGE_00_TEXTURE_MAX,
}UI_AREA_GAGE_00_TEXTURE;

//****************************************
// 構造体の定義
//****************************************

// エリアゲージ[00] の部品毎の情報構造体
typedef struct
{
	UI_AREA_GAGE_00_TEXTURE texture;		// テクスチャ番号
	int						nPattern;		// パターンNo.
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
	int						nPolygonNum;	// 必要ポリゴン数
}Ui_area_gage_00Parts;

// エリアゲージ[00] のテクスチャ毎の情報構造体
typedef struct
{
	char	aTexturePath[TXT_MAX];	// テクスチャファイルの相対パス
	int		nPtnMaxX;				// パターン上限X
	int		nPtnMaxY;				// パターン上限Y
}Ui_area_gage_00Texture;

//****************************************
// プロトタイプ宣言
//****************************************

// エリアゲージ[00] の部品毎の頂点座標設定処理
void SetVertexPositionUi_area_gage_00(VERTEX_2D *pVtx, UI_AREA_GAGE_00_PARTS parts);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_area_gage_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_area_gage_00		// テクスチャへのポインタ
						[UI_AREA_GAGE_00_TEXTURE_MAX]	//
						= {};							//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_area_gage_00		// 頂点バッファへのポインタ
						= NULL;							//

// エリアゲージ[00] のテクスチャ毎の情報
Ui_area_gage_00Texture g_aUi_area_gage_00Texture[UI_AREA_GAGE_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\ui_gage_003_h12_bar_orange.png", 2, 1 },
	{ "data\\TEXTURE\\ui_gage_003_h12_frame.png"     , 4, 1 },
};

// エリアゲージ[00] の部品毎の情報
Ui_area_gage_00Parts g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_MAX] =
{
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 3, PIXEL * 0, PIXEL * 16, 1 },
	{ UI_AREA_GAGE_00_TEXTURE_BAR  , 0, PIXEL * 0, PIXEL * 8 , 1 },
	{ UI_AREA_GAGE_00_TEXTURE_BAR  , 1, PIXEL * 1, PIXEL * 8 , 1 },
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 1, PIXEL * 0, PIXEL * 16, 1 },
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 0, PIXEL * 4, PIXEL * 16, 1 },
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 2, PIXEL * 4, PIXEL * 16, 1 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_area_gage_00関数 - エリアゲージ[00] の部品毎の頂点座標設定処理 -
//========================================
void SetVertexPositionUi_area_gage_00(VERTEX_2D *pVtx, UI_AREA_GAGE_00_PARTS parts) 
{
	float	fBarLengthDiameter	// 棒の長さの倍率
			= ((float)GetSys_stage_00()->nWaveTime / (float)GetSys_stage_00()->nWaveEndTime);

	if (!GetSys_stage_00()->bShowAreaTime)
	{// エリア時間の表示フラグが偽の時、
		// 棒の長さの倍率を0にする
		fBarLengthDiameter = 0.0f;
	}

	switch (parts)
	{
		//========== *** 奥の枠 ***
	case UI_AREA_GAGE_00_PARTS_BACK_FRAME:
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_AREA_GAGE_00_BAR_WIDTH,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** 棒 ***
	case UI_AREA_GAGE_00_PARTS_BAR:
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS
			+ D3DXVECTOR3(
				- (UI_AREA_GAGE_00_BAR_WIDTH * fBarLengthDiameter) * 0.5f,
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			(UI_AREA_GAGE_00_BAR_WIDTH * (1.0f - fBarLengthDiameter))
			- (g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_BAR_RIGHT].fWidth),
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** 棒の右端 ***
	case UI_AREA_GAGE_00_PARTS_BAR_RIGHT:
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS
			+ D3DXVECTOR3(
				+ (UI_AREA_GAGE_00_BAR_WIDTH * 0.5f)
				- (UI_AREA_GAGE_00_BAR_WIDTH * fBarLengthDiameter)
				- (g_aUi_area_gage_00Parts[parts].fWidth * 0.5f),
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_area_gage_00Parts[parts].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** 枠 ***
	case UI_AREA_GAGE_00_PARTS_FRAME:
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_AREA_GAGE_00_WIDTH
			- g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_FRAME_LEFT].fWidth
			- g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_FRAME_RIGHT].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** 枠の左端 ***
	case UI_AREA_GAGE_00_PARTS_FRAME_LEFT:
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS 
			+ D3DXVECTOR3(- (UI_AREA_GAGE_00_WIDTH * 0.5f) + (g_aUi_area_gage_00Parts[parts].fWidth * 0.5f), 
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_area_gage_00Parts[parts].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** 枠の右端 ***
	case UI_AREA_GAGE_00_PARTS_FRAME_RIGHT:
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS
			+ D3DXVECTOR3((UI_AREA_GAGE_00_WIDTH * 0.5f) - (g_aUi_area_gage_00Parts[parts].fWidth * 0.5f),
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_area_gage_00Parts[parts].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_area_gage_00(int nParts) 
{
	int nPolygonNum	// 必要ポリゴン数
		= 0;		//

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_area_gage_00Parts[nCntPolygon].nPolygonNum;
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
// InitUi_area_gage_00関数 - エリアゲージ[00] の初期化処理 -
//========================================
void InitUi_area_gage_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_area_gage_00(UI_AREA_GAGE_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_area_gage_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_area_gage_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < UI_AREA_GAGE_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_area_gage_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_area_gage_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_AREA_GAGE_00_PARTS_MAX; nCntParts++)
	{
		for (int nCntPolygon = 0; nCntPolygon < g_aUi_area_gage_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 部品毎の頂点座標設定処理
			SetVertexPositionUi_area_gage_00(pVtx, (UI_AREA_GAGE_00_PARTS)nCntParts);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx, 
				g_aUi_area_gage_00Parts[nCntParts].nPattern, 
				g_aUi_area_gage_00Texture[g_aUi_area_gage_00Parts[nCntParts].texture].nPtnMaxX,
				g_aUi_area_gage_00Texture[g_aUi_area_gage_00Parts[nCntParts].texture].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_area_gage_00->Unlock();
}

//========================================
// UninitUi_area_gage_00関数 - エリアゲージ[00] の終了処理 -
//========================================
void UninitUi_area_gage_00(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < UI_AREA_GAGE_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_area_gage_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_area_gage_00[nCntTexture]->Release();
			g_aTextureUi_area_gage_00[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_area_gage_00 != NULL) 
	{
		g_pVtxBuffUi_area_gage_00->Release();
		g_pVtxBuffUi_area_gage_00 = NULL;
	}
}

//========================================
// UpdateUi_area_gage_00関数 - エリアゲージ[00] の更新処理 -
//========================================
void UpdateUi_area_gage_00(void) 
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_area_gage_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_AREA_GAGE_00_PARTS_MAX; nCntParts++)
	{
		for (int nCntPolygon = 0; nCntPolygon < g_aUi_area_gage_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 部品毎の頂点座標設定処理
			SetVertexPositionUi_area_gage_00(pVtx, (UI_AREA_GAGE_00_PARTS)nCntParts);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_area_gage_00->Unlock();
}

//========================================
// DrawUi_area_gage_00関数 - エリアゲージ[00] の描画処理 -
//========================================
void DrawUi_area_gage_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_area_gage_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < UI_AREA_GAGE_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_area_gage_00[g_aUi_area_gage_00Parts[nCntParts].texture]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_area_gage_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// エリアゲージ[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}