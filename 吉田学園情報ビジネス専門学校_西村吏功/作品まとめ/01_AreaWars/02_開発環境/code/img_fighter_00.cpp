//========================================
// 
// 機体[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** img_fighter_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "img_fighter_00.h"

//****************************************
// マクロ定義
//****************************************

// 機体[00] の出現にかかる時間
// 機体[00] の位置
// 機体[00] の炎の点滅にかかる時間
// 機体[00] の影時の色
#define IMG_FIGHTER_00_IN_POP_TIME		(10)
#define IMG_FIGHTER_00_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 16), 0.0f))
#define IMG_FIGHTER_00_FIRE_BRINK_TIME	(1)
#define IMG_FIGHTER_00_SHADOW_COLOR		{0,0,0,255}

//****************************************
// 列挙型の定義
//****************************************

// 機体[00] の部品番号
typedef enum
{
	IMG_FIGHTER_00_PARTS_FIRE_LEFT= 0,	// 炎(左)
	IMG_FIGHTER_00_PARTS_FIRE_RIGHT,	// 炎(右)
	IMG_FIGHTER_00_PARTS_BODY,			// 本体
	IMG_FIGHTER_00_PARTS_MAX,
}IMG_FIGHTER_00_PARTS;

//****************************************
// 構造体の定義
//****************************************

// 機体[00] のポリゴン毎の情報構造体
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
}Img_fighter_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterImg_fighter_00(void);

// 部品(本体)の更新処理
void UpdateImg_fighter_00Parts_body(void);

// 部品(左)の更新処理
void UpdateImg_fighter_00Parts_fire_left(void);

// 部品(右)の更新処理
void UpdateImg_fighter_00Parts_fire_right(void);

// 状態に応じた開始処理
void StartStateProcessImg_fighter_00(void);

// 状態に応じた終了処理
void EndStateProcessImg_fighter_00(void);

// 状態に応じた更新処理
void UpdateStateProcessImg_fighter_00(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumImg_fighter_00(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureImg_fighter_00			// テクスチャへのポインタ
						[IMG_FIGHTER_00_PARTS_MAX]			//
						= {};								//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffImg_fighter_00			// 頂点バッファへのポインタ
						= NULL;								//
Img_fighter_00			g_img_fighter_00;					// 機体[00] の情報

// 機体[00] の部品毎の情報
Img_fighter_00Parts g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\img_fighter_000_fire.png" , 1 , 1 , 0 , 0 , -1, PIXEL * 32  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * -2, PIXEL * -74, 0.0f) },
	{ "data\\TEXTURE\\img_fighter_000_fire.png" , 1 , 1 , 0 , 0 , -1, PIXEL * 32  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * 88, PIXEL * -44, 0.0f) },
	{ "data\\TEXTURE\\img_fighter_000.png"      , 4 , 2 , 0 , 5 , 6 , PIXEL * 256 , PIXEL * 256 , 1 , D3DXVECTOR3(PIXEL * 0 , PIXEL * 0  , 0.0f) },
};

//========== *** 機体[00] 情報を取得 ***
Img_fighter_00 *GetImg_fighter_00(void)
{
	return &g_img_fighter_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterImg_fighter_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterImg_fighter_00(void)
{
	g_img_fighter_00.nCounterState		= 0;	// 状態カウンター
	g_img_fighter_00.fAlpha				= 0.0f;	// 全体の透明度
	g_img_fighter_00.nCounterFireBrink	= 0;	// 炎の点滅カウンター
	g_img_fighter_00.bFireBrink			= true;	// 炎の点滅フラグ
}

//========================================
// UpdateImg_fighter_00Parts_body関数 - 部品(本体)の更新処理 -
//========================================
void UpdateImg_fighter_00Parts_body(void)
{
	Img_fighter_00Parts	*pParts							// 
						= &g_aImg_fighter_00Parts		//
						[IMG_FIGHTER_00_PARTS_BODY];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_BODY));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_img_fighter_00.pos + g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].fWidth,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtn,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMin,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMax,
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nCounterAnim,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nAnimTime,
				ANIM_TYPE_TURN),
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMaxX,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMaxY,
			NULL);

		if ((GetMd_title_00()->state == MD_TITLE_00_STATE_TEAM_LOGO)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_TITLE_ANIM)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_FLASH))
		{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュのいずれかである時、
			// 頂点カラーを影時のものに設定
			SetVertexColor(pVtx, IMG_FIGHTER_00_SHADOW_COLOR);
		}
		else 
		{
			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });
		}
	}
}

//========================================
// UpdateImg_fighter_00Parts_fire_left関数 - 部品(炎(左))の更新処理 -
//========================================
void UpdateImg_fighter_00Parts_fire_left(void)
{
	Img_fighter_00Parts	*pParts								// 
						= &g_aImg_fighter_00Parts			//
						[IMG_FIGHTER_00_PARTS_FIRE_LEFT];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;								// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_FIRE_LEFT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_img_fighter_00.pos + g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].fWidth,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtn,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMin,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMax,
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nCounterAnim,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nAnimTime,
				ANIM_TYPE_TURN),
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMaxX,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMaxY,
			NULL);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,255 });
	}
}

//========================================
// UpdateImg_fighter_00Parts_fire_right関数 - 部品(炎(右))の更新処理 -
//========================================
void UpdateImg_fighter_00Parts_fire_right(void)
{
	Img_fighter_00Parts	*pParts								// 
						= &g_aImg_fighter_00Parts			//
						[IMG_FIGHTER_00_PARTS_FIRE_RIGHT];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;								// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_FIRE_RIGHT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_img_fighter_00.pos + g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].fWidth,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtn,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMin,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMax,
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nCounterAnim,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nAnimTime,
				ANIM_TYPE_TURN),
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMaxX,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMaxY,
			NULL);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,255 });
	}
}

//========================================
// StartStateProcessImg_fighter_00関数 - 状態に応じた開始処理 -
//========================================
void StartStateProcessImg_fighter_00(void)
{
	Img_fighter_00	*pUi					// 
					= &g_img_fighter_00;	// 機体[00] の情報のポインタ 

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case IMG_FIGHTER_00_STATE_POP:
	{
		// 機体[00] の位置を代入
		pUi->pos = IMG_FIGHTER_00_POS;
	}
	break;
		//========== *** 出現中 ***
	case IMG_FIGHTER_00_STATE_IN_POP:
	{
		// 全ての部品のアニメーションのカウンターを初期化
		for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
		{
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_MAX].nCounterAnim = 0;
		}
	}
	break;
	}
}

//========================================
// EndStateProcessImg_fighter_00関数 - 状態に応じた終了処理 -
//========================================
void EndStateProcessImg_fighter_00(void)
{
	Img_fighter_00	*pUi					// 
					= &g_img_fighter_00;	// 機体[00] の情報のポインタ 

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case IMG_FIGHTER_00_STATE_POP:
	{

	}
	break;
		//========== *** 出現中 ***
	case IMG_FIGHTER_00_STATE_IN_POP:
	{

	}
	break;
	}
}

//========================================
// UpdateStateProcessImg_fighter_00関数 - 状態に応じた更新処理 -
//========================================
void UpdateStateProcessImg_fighter_00(void)
{
	Img_fighter_00	*pUi					// 
					= &g_img_fighter_00;	// 機体[00] の情報のポインタ 

	if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
	{// タイトル画面[00] の状態がチームロゴ/オープニング/フラッシュでない時、
		if (++pUi->nCounterFireBrink >= IMG_FIGHTER_00_FIRE_BRINK_TIME)
		{// 炎の点滅カウンターが炎の点滅にかかる時間に達した時、
			// 炎の点滅カウンターを初期化する
			pUi->nCounterFireBrink = 0;

			// 炎の点滅フラグを切り替える
			pUi->bFireBrink ^= 1;
		}
	}

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case IMG_FIGHTER_00_STATE_POP:
	{

	}
	break;
		//========== *** 出現中 ***
	case IMG_FIGHTER_00_STATE_IN_POP:
	{
		// 状態カウンターを加算
		pUi->nCounterState++;

		if (pUi->nCounterState >= IMG_FIGHTER_00_IN_POP_TIME)
		{// 状態カウンターが出現にかかる時間に達した時、
			// 出現状態にする
			SetStateImg_fighter_00(IMG_FIGHTER_00_STATE_POP);
		}
	}
	break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumImg_fighter_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// 必要ポリゴン数

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aImg_fighter_00Parts[nCntPolygon].nPolygonNum;
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
// InitImg_fighter_00関数 - 機体[00] の初期化処理 -
//========================================
void InitImg_fighter_00(void)
{
	// パラメーターの初期化処理
	InitParameterImg_fighter_00();

	// 状態設定処理
	SetStateImg_fighter_00(IMG_FIGHTER_00_STATE_POP);

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffImg_fighter_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aImg_fighter_00Parts[nCntParts].aTexturePath, &g_aTextureImg_fighter_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aImg_fighter_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aImg_fighter_00Parts[nCntParts].fWidth,
				g_aImg_fighter_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx,
				0,
				g_aImg_fighter_00Parts[nCntParts].nPtnMaxX,
				g_aImg_fighter_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffImg_fighter_00->Unlock();
}

//========================================
// UninitImg_fighter_00関数 - 機体[00] の終了処理 -
//========================================
void UninitImg_fighter_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureImg_fighter_00[nCntParts] != NULL)
		{
			g_aTextureImg_fighter_00[nCntParts]->Release();
			g_aTextureImg_fighter_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffImg_fighter_00 != NULL)
	{
		g_pVtxBuffImg_fighter_00->Release();
		g_pVtxBuffImg_fighter_00 = NULL;
	}
}

//========================================
// UpdateImg_fighter_00関数 - 機体[00] の更新処理 -
//========================================
void UpdateImg_fighter_00(void)
{
	Img_fighter_00	*pUi					//
					= &g_img_fighter_00;	// 機体[00] の情報のポインタ

	// 状態処理
	UpdateStateProcessImg_fighter_00();

	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** 本体の説明 ***
		case IMG_FIGHTER_00_PARTS_BODY:
		{
			// 部品(本体の説明)の更新処理
			UpdateImg_fighter_00Parts_body();
		}
		break;
			//========== *** 炎(左) ***
		case IMG_FIGHTER_00_PARTS_FIRE_LEFT:
		{
			// 部品(炎(左))の更新処理
			UpdateImg_fighter_00Parts_fire_left();
		}
		break;
			//========== *** 炎(右) ***
		case IMG_FIGHTER_00_PARTS_FIRE_RIGHT:
		{
			// 部品(炎(右))の更新処理
			UpdateImg_fighter_00Parts_fire_right();
		}
		break;
		}
	}
}

//========================================
// DrawImg_fighter_00関数 - 機体[00] の描画処理 -
//========================================
void DrawImg_fighter_00(void)
{
	Img_fighter_00	*pUi					//
					= &g_img_fighter_00;	// 機体[00] の情報のポインタ

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffImg_fighter_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureImg_fighter_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aImg_fighter_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			if ((pUi->bFireBrink)
				&&
				((nCntParts == IMG_FIGHTER_00_PARTS_FIRE_LEFT)
				||
				(nCntParts == IMG_FIGHTER_00_PARTS_FIRE_RIGHT)))
			{// 炎の点滅フラグが真で、かつカウントが炎の部品に該当する時、
				// 繰り返し処理を折り返す
				continue;
			}

			// 機体[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState関数 - 機体[00] の状態設定処理 -
//========================================
void SetStateImg_fighter_00(IMG_FIGHTER_00_STATE state)
{
	Img_fighter_00	*pUi					//
					= &g_img_fighter_00;	// 機体[00] の情報のポインタ 

	// 状態に応じた終了処理
	EndStateProcessImg_fighter_00();

	// 状態を代入
	pUi->state = state;

	// 状態に応じた開始処理
	StartStateProcessImg_fighter_00();

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}