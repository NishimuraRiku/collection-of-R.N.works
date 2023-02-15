//========================================
// 
// タイトルロゴ[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_title_logo_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "ui_title_logo_00.h"

//****************************************
// マクロ定義
//****************************************

// タイトルロゴ[00] の出現にかかる時間
// タイトルロゴ[00] のプラズマウォールの移動待ちにかかる時間
// タイトルロゴ[00] のプラズマウォールの移動にかかる時間
// タイトルロゴ[00] のプラズマウォールの四角形の辺の幅
// タイトルロゴ[00] のプラズマウォールの四角形の初期値
// タイトルロゴ[00] のプラズマウォールの四角形の最終値
// タイトルロゴ[00] のプラズマウォールの点滅パターン数
// タイトルロゴ[00] のプラズマウォールの点滅にかかる時間
// タイトルロゴ[00] の位置
// タイトルロゴ[00] のプラズマウォール[00] の位置(相対)
// タイトルロゴ[00] のプラズマウォール[01] の位置(相対)
// タイトルロゴ[00] の影時の色
#define UI_TITLE_LOGO_00_IN_POP_TIME					(80)
#define UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_WAIT_TIME		(40)
#define UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME			(MD_TITLE_00_TITLE_ANIM_TIME - UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_WAIT_TIME)
#define UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH				(PIXEL * 2)
#define UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE		Square{PIXEL * (-15 + 128),PIXEL * (15 + 128),PIXEL * -20 ,PIXEL * 20}
#define UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE			Square{PIXEL * -41,PIXEL * 45,PIXEL * -147,PIXEL * 146}
#define UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM	(2)
#define UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_TIME			(2)
#define UI_TITLE_LOGO_00_POS							(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, PIXEL * 72, 0.0f))
#define UI_TITLE_LOGO_00_SHADOW_COLOR					{0,0,0,255}

//****************************************
// 列挙型の定義
//****************************************

// タイトルロゴ[00] の部品番号
typedef enum
{
	UI_TITLE_LOGO_00_PARTS_BASE = 0,			// 基盤
	UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK,		// プラズマブロック
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00,		// プラズマウォール[00]
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01,		// プラズマウォール[01]
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP,		// プラズマウォール(上辺)
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM,	// プラズマウォール(下辺)
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT,	// プラズマウォール(左辺)
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT,	// プラズマウォール(右辺)
	UI_TITLE_LOGO_00_PARTS_MAX,
}UI_TITLE_LOGO_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// タイトルロゴ[00] のポリゴン毎の情報構造体
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
	D3DXVECTOR3		relativePos;	// 相対位置

	int				nPtn;			// パターンNo.
	int				nCounterAnim;	// アニメーションのカウンター
}Ui_title_logo_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterUi_title_logo_00(void);

// 部品(基盤)の更新処理
void UpdateUi_title_logo_00Parts_base(void);

// 部品(プラズマブロック)の更新処理
void UpdateUi_title_logo_00Parts_plasma_block(void);

// 部品(プラズマウォール[00])の更新処理
void UpdateUi_title_logo_00Parts_plasma_wall_00(void);

// 部品(プラズマウォール[01])の更新処理
void UpdateUi_title_logo_00Parts_plasma_wall_01(void);

// 部品(プラズマウォール(上))の更新処理
void UpdateUi_title_logo_00Parts_plasma_wall_top(void);

// 状態に応じた開始処理
void StartStateProcessUi_title_logo_00(void);

// 状態に応じた終了処理
void EndStateProcessUi_title_logo_00(void);

// 状態に応じた更新処理
void UpdateStateProcessUi_title_logo_00(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_title_logo_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_title_logo_00		// テクスチャへのポインタ
						[UI_TITLE_LOGO_00_PARTS_MAX]	//
						= {};							//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_title_logo_00		// 頂点バッファへのポインタ
						= NULL;							//
Ui_title_logo_00		g_ui_title_logo_00;				// タイトルロゴ[00] の情報

// タイトルロゴ[00] の部品毎の情報
Ui_title_logo_00Parts g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_title_logo_000_base.png"     , 1 , 1 , 0 , 0 , -1 , PIXEL * 512 , PIXEL * 128 , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ "data\\TEXTURE\\ui_title_logo_000_parts_00.png" , 4 , 2 , 0 , 7 , 4  , PIXEL * 16  , PIXEL * 16  , 1 , D3DXVECTOR3(PIXEL * -101, PIXEL * 45 , 0.0f) },
	{ "data\\TEXTURE\\ui_title_logo_000_parts_01.png" , 2 , 2 , 0 , 3 , 6  , PIXEL * 64  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * 133 , PIXEL * -34, 0.0f) },
	{ "data\\TEXTURE\\ui_title_logo_000_parts_02.png" , 2 , 2 , 0 , 3 , 6  , PIXEL * 64  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * -133, PIXEL * 38 , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
};

// タイトルロゴ[00] のプラズマウォールの点滅色
Color g_aUi_title_logo_00PlasmaWallBrinkColor[UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM] =
{
	{ 255,218,171,255 },
	{ 255,153,59 ,255 },
};

//========== *** タイトルロゴ[00] 情報を取得 ***
Ui_title_logo_00 *GetUi_title_logo_00(void)
{
	return &g_ui_title_logo_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_title_logo_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterUi_title_logo_00(void)
{
	g_ui_title_logo_00.nCounterState	= 0;											// 状態カウンター
	g_ui_title_logo_00.fAlpha			= 0.0f;											// 全体の透明度
	g_ui_title_logo_00.pos				= UI_TITLE_LOGO_00_POS;							// 位置
	g_ui_title_logo_00.square			= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE;	// 四角形の情報
}

//========================================
// UpdateUi_title_logo_00Parts_base関数 - 部品(基盤)の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_base(void)
{
	Ui_title_logo_00Parts	*pParts							// 
							= &g_aUi_title_logo_00Parts		//
							[UI_TITLE_LOGO_00_PARTS_BASE];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_BASE));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMaxY,
			NULL);

		if ((GetMd_title_00()->state == MD_TITLE_00_STATE_TEAM_LOGO)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_TITLE_ANIM)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_FLASH))
		{// タイトル画面[00] の状態がチームロゴ/タイトルアニメーション/フラッシュのいずれかの時、
			// 頂点カラーを影時のものに設定
			SetVertexColor(pVtx, UI_TITLE_LOGO_00_SHADOW_COLOR);
		}
		else
		{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュのいずれかでもない時、
			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });
		}
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_block関数 - 部品(プラズマブロック)の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_plasma_block(void)
{
	if (!g_ui_title_logo_00.bActive) 
	{// 非動的の時、
		// 処理を終了する
		return;
	}

	Ui_title_logo_00Parts	*pParts									// 
							= &g_aUi_title_logo_00Parts				//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;									// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_00関数 - 部品(プラズマウォール[00])の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_00(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;										// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nAnimTime,
				ANIM_TYPE_TURN),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_01関数 - 部品(プラズマウォール[01])の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_01(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;										// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nAnimTime,
				ANIM_TYPE_TURN),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_top関数 - 部品(プラズマウォール(上))の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_top(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;										// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3((g_ui_title_logo_00.square.fLeft + g_ui_title_logo_00.square.fRight) * 0.5f, g_ui_title_logo_00.square.fTop, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_ui_title_logo_00.square.fRight - g_ui_title_logo_00.square.fLeft,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_bottom関数 - 部品(プラズマウォール(下))の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_bottom(void)
{
	Ui_title_logo_00Parts	*pParts											// 
							= &g_aUi_title_logo_00Parts						//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;											// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3((g_ui_title_logo_00.square.fLeft + g_ui_title_logo_00.square.fRight) * 0.5f, g_ui_title_logo_00.square.fBottom, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_ui_title_logo_00.square.fRight - g_ui_title_logo_00.square.fLeft,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_left関数 - 部品(プラズマウォール(左))の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_left(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;										// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3(g_ui_title_logo_00.square.fLeft, (g_ui_title_logo_00.square.fTop + g_ui_title_logo_00.square.fBottom) * 0.5f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			g_ui_title_logo_00.square.fBottom - g_ui_title_logo_00.square.fTop,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_right関数 - 部品(プラズマウォール(右))の更新処理 -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_right(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT];	// 部品の情報のポインタ
	VERTEX_2D				*pVtx;										// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3(g_ui_title_logo_00.square.fRight, (g_ui_title_logo_00.square.fTop + g_ui_title_logo_00.square.fBottom) * 0.5f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			g_ui_title_logo_00.square.fBottom - g_ui_title_logo_00.square.fTop,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// StartStateProcessUi_title_logo_00関数 - 状態に応じた開始処理 -
//========================================
void StartStateProcessUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					// 
						= &g_ui_title_logo_00;	// タイトルロゴ[00] の情報のポインタ 

	// 状態カウンターを初期化
	pUi->nCounterState = 0;

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_TITLE_LOGO_00_STATE_POP:
	{
		// タイトルロゴ[00] の位置を代入
		pUi->pos = UI_TITLE_LOGO_00_POS;

		// 全ての部品のアニメーションのカウンターを初期化
		for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
		{
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_MAX].nCounterAnim = 0;
		}
	}
	break;
		//========== *** 出現中 ***
	case UI_TITLE_LOGO_00_STATE_IN_POP:
	{
		
	}
	break;
		//========== *** 消えている ***
	case UI_TITLE_LOGO_00_STATE_CLEAR:
	{
		
	}
	break;
		//========== *** プラズマウォールの移動 ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE:
	{
		// 四角形の情報を初期のものにする
		g_ui_title_logo_00.square = UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE;
	}
		break;
		//========== *** プラズマウォールの移動待ち ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT:
	{
		// 四角形の情報を初期のものにする
		g_ui_title_logo_00.square = UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE;

		// 点滅カウントと点滅カウンターを初期化する
		g_ui_title_logo_00.nCntBrink = 0;
		g_ui_title_logo_00.nCounterBrink = 0;
	}
	break;
	}
}

//========================================
// EndStateProcessUi_title_logo_00関数 - 状態に応じた終了処理 -
//========================================
void EndStateProcessUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					// 
						= &g_ui_title_logo_00;	// タイトルロゴ[00] の情報のポインタ 

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_TITLE_LOGO_00_STATE_POP:
	{

	}
	break;
		//========== *** 出現中 ***
	case UI_TITLE_LOGO_00_STATE_IN_POP:
	{

	}
	break;
		//========== *** 消えている ***
	case UI_TITLE_LOGO_00_STATE_CLEAR:
	{

	}
	break;
		//========== *** プラズマウォールの移動 ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE:
	{
		
	}
	break;
		//========== *** プラズマウォールの移動待ち ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT:
	{

	}
	break;
	}
}

//========================================
// UpdateStateProcessUi_title_logo_00関数 - 状態に応じた更新処理 -
//========================================
void UpdateStateProcessUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					// 
						= &g_ui_title_logo_00;	// タイトルロゴ[00] の情報のポインタ 

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_TITLE_LOGO_00_STATE_POP:
	{

	}
	break;
		//========== *** 出現中 ***
	case UI_TITLE_LOGO_00_STATE_IN_POP:
	{
		if (++pUi->nCounterState >= UI_TITLE_LOGO_00_IN_POP_TIME)
		{// 状態カウンターを加算した結果出現にかかる時間に達した時、
			// 出現状態にする
			SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_POP);
		}
	}
	break;
		//========== *** 消えている ***
	case UI_TITLE_LOGO_00_STATE_CLEAR:
	{

	}
	break;
		//========== *** プラズマウォールの移動 ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE:
	{
		// 四角形の情報にプラズマウォールの四角形の初期の情報をカウンターに応じた割合代入
		g_ui_title_logo_00.square.fTop		= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fTop	* (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));
		g_ui_title_logo_00.square.fBottom	= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fBottom * (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));
		g_ui_title_logo_00.square.fLeft		= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fLeft	* (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));
		g_ui_title_logo_00.square.fRight	= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fRight	* (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));

		// 四角形の情報にプラズマウォールの四角形の最終の情報をカウンターに応じた割合加算
		g_ui_title_logo_00.square.fTop		+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fTop		* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);
		g_ui_title_logo_00.square.fBottom	+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fBottom	* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);
		g_ui_title_logo_00.square.fLeft		+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fLeft	* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);
		g_ui_title_logo_00.square.fRight	+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fRight	* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);

		if (++pUi->nCounterBrink >= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_TIME)
		{// 点滅カウンターを加算した結果点滅にかかる時間に達した時、
			// 点滅カウンターを初期化し、点滅カウントを更新
			pUi->nCounterBrink = 0;
			++pUi->nCntBrink %= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM;
		}

		if (++pUi->nCounterState >= UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME)
		{// 状態カウンターを加算した結果プラズマウォールの移動にかかる時間に達した時、
			// 出現状態にする
			SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_POP);
		}
	}
	break;
		//========== *** プラズマウォールの移動待ち ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT:
	{
		if (++pUi->nCounterBrink >= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_TIME)
		{// 点滅カウンターを加算した結果点滅にかかる時間に達した時、
			// 点滅カウンターを初期化し、点滅カウントを更新
			pUi->nCounterBrink = 0;
			++pUi->nCntBrink %= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM;
		}

		if (++pUi->nCounterState >= UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_WAIT_TIME)
		{// 状態カウンターを加算した結果プラズマウォールの移動待ちにかかる時間に達した時、
			// プラズマウォールの移動状態にする
			SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE);
		}
	}
	break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_title_logo_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// 必要ポリゴン数

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_title_logo_00Parts[nCntPolygon].nPolygonNum;
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
// InitUi_title_logo_00関数 - タイトルロゴ[00] の初期化処理 -
//========================================
void InitUi_title_logo_00(void)
{
	// パラメーターの初期化処理
	InitParameterUi_title_logo_00();

	// 消えている状態にする
	SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_CLEAR);

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_title_logo_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aUi_title_logo_00Parts[nCntParts].aTexturePath, &g_aTextureUi_title_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_title_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_title_logo_00Parts[nCntParts].fWidth,
				g_aUi_title_logo_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx,
				0,
				g_aUi_title_logo_00Parts[nCntParts].nPtnMaxX,
				g_aUi_title_logo_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_title_logo_00->Unlock();
}

//========================================
// UninitUi_title_logo_00関数 - タイトルロゴ[00] の終了処理 -
//========================================
void UninitUi_title_logo_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_title_logo_00[nCntParts] != NULL)
		{
			g_aTextureUi_title_logo_00[nCntParts]->Release();
			g_aTextureUi_title_logo_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_title_logo_00 != NULL)
	{
		g_pVtxBuffUi_title_logo_00->Release();
		g_pVtxBuffUi_title_logo_00 = NULL;
	}
}

//========================================
// UpdateUi_title_logo_00関数 - タイトルロゴ[00] の更新処理 -
//========================================
void UpdateUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					//
						= &g_ui_title_logo_00;	// タイトルロゴ[00] の情報のポインタ

	if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
	{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュでない時、
		// 動的フラグを真にする
		pUi->bActive = true;
	}
	else 
	{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュのいずれかである時、
		// 動的フラグを偽にする
		pUi->bActive = false;
	}

	// 状態処理
	UpdateStateProcessUi_title_logo_00();

	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
		case UI_TITLE_LOGO_00_PARTS_BASE				:UpdateUi_title_logo_00Parts_base()					; break; // 部品(基盤)の更新処理
		case UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK		:UpdateUi_title_logo_00Parts_plasma_block()			; break; // 部品(プラズマブロック)の更新処理
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00		:UpdateUi_title_logo_00Parts_plasma_wall_00()		; break; // 部品(プラズマブロック[00])の更新処理
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01		:UpdateUi_title_logo_00Parts_plasma_wall_01()		; break; // 部品(プラズマブロック[01])の更新処理
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP		:UpdateUi_title_logo_00Parts_plasma_wall_top()		; break; // 部品(プラズマブロック(上))の更新処理
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM	:UpdateUi_title_logo_00Parts_plasma_wall_bottom()	; break; // 部品(プラズマブロック(下))の更新処理
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT	:UpdateUi_title_logo_00Parts_plasma_wall_left()		; break; // 部品(プラズマブロック(左))の更新処理
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT	:UpdateUi_title_logo_00Parts_plasma_wall_right()	; break; // 部品(プラズマブロック(右))の更新処理
		}
	}
}

//========================================
// DrawUi_title_logo_00関数 - タイトルロゴ[00] の描画処理 -
//========================================
void DrawUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					//
						= &g_ui_title_logo_00;	// タイトルロゴ[00] の情報のポインタ

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_title_logo_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_title_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_title_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			if (pUi->state != UI_TITLE_LOGO_00_STATE_POP)
			{// 出現状態でない時、
				if (((pUi->state == UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE)
					|| 
					(pUi->state == UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT))
					&&
					((nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT)))
				{// プラズマウォールの移動中もしくは移動待ちでかつカウントが移動しているプラズマウォールに該当する時、
					
				}
				else 
				{// プラズマウォールの移動中でない、もしくはカウントが移動しているプラズマウォールに該当しない時、
					// 処理を折り返す
					continue;
				}
			}
			else 
			{// 出現状態の時、
				if ((nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT))
				{// カウントが移動しているプラズマウォールに該当する時、
					// 処理を折り返す
					continue;
				}
			}

			// タイトルロゴ[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState関数 - タイトルロゴ[00] の状態設定処理 -
//========================================
void SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE state)
{
	Ui_title_logo_00	*pUi					//
						= &g_ui_title_logo_00;	// タイトルロゴ[00] の情報のポインタ 

	// 状態に応じた終了処理
	EndStateProcessUi_title_logo_00();

	// 状態を代入
	pUi->state = state;

	// 状態に応じた開始処理
	StartStateProcessUi_title_logo_00();

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}