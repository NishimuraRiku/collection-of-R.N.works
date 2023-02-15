//========================================
// 
// 宇宙[02] の処理
// Author:西村 吏功
// 
//========================================
//  *** bg_space_01.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "bg_space_01.h"

//****************************************
// マクロ定義
//****************************************

// 宇宙[01] の出現にかかる時間
// 宇宙[01] の消えるのにかかる時間
// 宇宙[01] の透明度の最大値
// 宇宙[01] の開始位置
// 宇宙[01] の移動角度
#define BG_SPACE_01_IN_POP_TIME		(10)
#define BG_SPACE_01_IN_CLEAR_TIME	(10)
#define BG_SPACE_01_ALPHA_MAX		(1.0f)
#define BG_SPACE_01_START_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f))
#define BG_SPACE_01_MOVE_ANGLE		(D3DX_PI * 0.7f)

//****************************************
// 列挙型の定義
//****************************************

// 宇宙[01] の部品番号
typedef enum
{
	BG_SPACE_01_PARTS_SPACE_00 = 0,	// 宇宙[00]
	BG_SPACE_01_PARTS_SPACE_01,		// 宇宙[01]
	BG_SPACE_01_PARTS_SPACE_02,		// 宇宙[02]
	BG_SPACE_01_PARTS_SPACE_03,		// 宇宙[03]
	BG_SPACE_01_PARTS_MAX,
}BG_SPACE_01_PARTS;

//****************************************
// 構造体の定義
//****************************************

// 宇宙[01] のポリゴン毎の情報構造体
typedef struct
{
	char			aTexturePath	// テクスチャファイルの相対パス
					[TXT_MAX];		// 
	int				nPtnMaxX;		// パターンの上限(横)
	int				nPtnMaxY;		// パターンの上限(縦)
	int				nPtnMin;		// パターンNo.の下限
	int				nPtnMax;		// パターンNo.の上限
	int				nAnimTime;		// アニメーションにかかる時間
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	int				nPolygonNum;	// 必要ポリゴン数
	D3DXVECTOR3		textureMove;	// テクスチャの移動量

	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		texturePos;		// テクスチャの位置
	int				nPtn;			// パターンNo.
	int				nCounterAnim;	// アニメーションのカウンター
}Bg_space_01Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// 宇宙[01] のパラメーターの初期化処理
void InitParameterBg_space_01(void);

// 部品(宇宙[00])の更新処理
void UpdateBg_space_01Parts_space_00(void);

// 部品(宇宙[01])の更新処理
void UpdateBg_space_01Parts_space_01(void);

// 部品(宇宙[02])の更新処理
void UpdateBg_space_01Parts_space_02(void);

// 部品(宇宙[03])の更新処理
void UpdateBg_space_01Parts_space_03(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumBg_space_01(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureBg_space_01	// テクスチャへのポインタ
						[BG_SPACE_01_PARTS_MAX]	//
						= {};					//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_space_01	// 頂点バッファへのポインタ
						= NULL;					//

// 宇宙[01] の部品毎の情報
Bg_space_01Parts g_aBg_space_01Parts[BG_SPACE_01_PARTS_MAX] =
{
	{ "data\\TEXTURE\\bg_space_000_03.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.01f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.01f, 0.0f) },
	{ "data\\TEXTURE\\bg_space_000_02.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.16f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.16f, 0.0f) },
	{ "data\\TEXTURE\\bg_space_000_01.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.32f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.32f, 0.0f) },
	{ "data\\TEXTURE\\bg_space_000_00.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.64f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.64f, 0.0f) },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_space_01関数 - 宇宙[01] のパラメーターの初期化処理 -
//========================================
void InitParameterBg_space_01(void) 
{
	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX;nCntParts++) 
	{
		g_aBg_space_01Parts[nCntParts].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aBg_space_01Parts[nCntParts].texturePos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// テクスチャの位置
		g_aBg_space_01Parts[nCntParts].nPtn			= 0;								// パターンNo.
		g_aBg_space_01Parts[nCntParts].nCounterAnim = 0;								// アニメーションのカウンター
	}
}

//========================================
// UpdateBg_space_01Parts_space_00関数 - 部品(宇宙[00])の更新処理 -
//========================================
void UpdateBg_space_01Parts_space_00(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_00];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_00));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュでない時、
			// テクスチャの位置を更新
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fHeight;
		}

		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].texturePos);
	}
}

//========================================
// UpdateBg_space_01Parts_space_01関数 - 部品(宇宙[01])の更新処理 -
//========================================
void UpdateBg_space_01Parts_space_01(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_01];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_01));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュでない時、
			// テクスチャの位置を更新
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fHeight;
		}

		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].texturePos);
	}
}

//========================================
// UpdateBg_space_01Parts_space_02関数 - 部品(宇宙[02])の更新処理 -
//========================================
void UpdateBg_space_01Parts_space_02(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_02];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_02));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュでない時、
			// テクスチャの位置を更新
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fHeight;
		}

		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].texturePos);
	}
}

//========================================
// UpdateBg_space_01Parts_space_03関数 - 部品(宇宙[03])の更新処理 -
//========================================
void UpdateBg_space_01Parts_space_03(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_03];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_03));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュでない時、
			// テクスチャの位置を更新
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fHeight;
		}

		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].texturePos);
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumBg_space_01(int nParts) 
{
	int nPolygonNum	// 
		= 0;		// 必要ポリゴン数

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aBg_space_01Parts[nCntPolygon].nPolygonNum;
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
// InitBg_space_01関数 - 宇宙[01] の初期化処理 -
//========================================
void InitBg_space_01(void)
{
	// パラメーターの初期化処理
	InitParameterBg_space_01();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_space_01,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++)
	{
		// 位置を初期化
		g_aBg_space_01Parts[nCntParts].pos = BG_SPACE_01_START_POS;

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aBg_space_01Parts[nCntParts].aTexturePath, &g_aTextureBg_space_01[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aBg_space_01Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aBg_space_01Parts[nCntParts].fWidth,
				g_aBg_space_01Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx, 
				0, 
				g_aBg_space_01Parts[nCntParts].nPtnMaxX, 
				g_aBg_space_01Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffBg_space_01->Unlock();
}

//========================================
// UninitBg_space_01関数 - 宇宙[01] の終了処理 -
//========================================
void UninitBg_space_01(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureBg_space_01[nCntParts] != NULL)
		{
			g_aTextureBg_space_01[nCntParts]->Release();
			g_aTextureBg_space_01[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg_space_01 != NULL) 
	{
		g_pVtxBuffBg_space_01->Release();
		g_pVtxBuffBg_space_01 = NULL;
	}
}

//========================================
// UpdateBg_space_01関数 - 宇宙[01] の更新処理 -
//========================================
void UpdateBg_space_01(void) 
{
	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++) 
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** 宇宙[00] ***
		case BG_SPACE_01_PARTS_SPACE_00:
		{
			// 部品(宇宙[00])の更新処理
			UpdateBg_space_01Parts_space_00();
		}
			break;
			//========== *** 宇宙[01] ***
		case BG_SPACE_01_PARTS_SPACE_01:
		{
			// 部品(宇宙[01])の更新処理
			UpdateBg_space_01Parts_space_01();
		}
		break;
			//========== *** 宇宙[02] ***
		case BG_SPACE_01_PARTS_SPACE_02:
		{
			// 部品(宇宙[02])の更新処理
			UpdateBg_space_01Parts_space_02();
		}
		break;
			//========== *** 宇宙[03] ***
		case BG_SPACE_01_PARTS_SPACE_03:
		{
			// 部品(宇宙[03])の更新処理
			UpdateBg_space_01Parts_space_03();
		}
		break;
		}
	}
}

//========================================
// DrawBg_space_01関数 - 宇宙[01] の描画処理 -
//========================================
void DrawBg_space_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg_space_01, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureBg_space_01[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aBg_space_01Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// 宇宙[02] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}