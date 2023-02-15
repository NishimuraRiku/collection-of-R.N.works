//========================================
// 
// テキストの処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** text.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon2D.h"
#include "polygon3D.h"
#include "text.h"

//****************************************
// マクロ定義
//****************************************
// テキストの文字数の最大数
#define TEXT_DIGIT_MAX	(128)

//****************************************
// 構造体の定義
//****************************************
// テキスト(2D)情報構造体を定義
typedef struct
{
	char	aString	// 文字列
			[TXT_MAX];
	FONT	font;	// フォント
}Text2D;

// テキスト(2D)の管理情報構造体を定義
typedef struct 
{
	int nUseNum;	// 使用されている数
}Text2DControl;

// テキスト(3D)情報構造体を定義
typedef struct
{
	char		aString		// 文字列
				[TXT_MAX];
	FONT		font;		// フォント
	D3DXVECTOR3 pos;		// 位置
	bool		bZtest;		// Zテストフラグ
	bool		bLighting;	// ライティングフラグ
	bool		bBillboard;	// ビルボードフラグ
}Text3D;

// テキスト(3D)の管理情報構造体を定義
typedef struct
{
	int nUseNum;	// 使用されている数
}Text3DControl;

//****************************************
// プロトタイプ宣言
//****************************************
// テキスト(2D)のパラメーターの初期化処理
// テキスト(2D)の管理情報のパラメーターの初期化処理
void InitParameterText2D(Text2D *pText);
void InitParameterText2DControl(void);

// テキスト(2D)の初期化処理
// テキスト(2D)の終了処理
void InitText2D(void);
void UninitText2D(void);

// テキスト(3D)のパラメーターの初期化処理
// テキスト(3D)の管理情報のパラメーターの初期化処理
void InitParameterText3D(Text3D *pText);
void InitParameterText3DControl(void);

// テキスト(3D)の初期化処理
// テキスト(3D)の終了処理
void InitText3D(void);
void UninitText3D(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureText		// テクスチャへのポインタ
						[FONT_MAX] 
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffText2D 	// テキスト(2D)の頂点バッファへのポインタ
						= NULL;
Text2D					g_text2D			// テキスト(2D)の情報
						[TEXT2D_MAX];
Text2DControl			g_text2DControl;	// テキスト(2D)の管理情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffText3D 	// テキスト(3D)の頂点バッファへのポインタ
						= NULL;
Text3D					g_text3D			// テキスト(3D)の情報
						[TEXT2D_MAX];
Text3DControl			g_text3DControl;	// テキスト(3D)の管理情報
D3DXMATRIX				g_mtxWorldText3D;	// UI:アイテムアイコン[00] のワールドマトリックス

// フォントの情報
Font g_aFont[FONT_MAX] =
{
	{ "data\\TEXTURE\\Font\\font_000.png",8 ,8 ,6 ,0,'!',8,8 },
	{ "data\\TEXTURE\\Font\\font_001.png",16,16,12,0,'!',8,8 },
	{ "data\\TEXTURE\\Font\\font_002.png",8 ,8 ,6 ,0,'!',8,8 },
	{ "data\\TEXTURE\\Font\\font_003.png",16,16,12,0,'!',8,8 },
	{ "data\\TEXTURE\\Font\\font_004.png",8 ,8 ,8 ,0,'!',8,8 },
};

//========== *** フォントの情報を取得 ***
Font *GetFont(void) 
{
	return g_aFont;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//-----|------------------------------------------------------
//=====| テキスト(2D)
//-----|------------------------------------------------------
//============================================================

//========================================
// InitParameterText2D関数 - テキスト(2D)のパラメーターの初期化処理 -
//========================================
void InitParameterText2D(Text2D *pText) 
{
	pText->font = (FONT)0;	// フォント
}

//========================================
// InitParameterText2DControl関数 - テキスト(2D)の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterText2DControl(void)
{
	Text2DControl	*pTxtCtl	// テキスト(2D)の管理情報のポインタ
					= &g_text2DControl;

	pTxtCtl->nUseNum = 0;	// 使用されている数
}

//========================================
// InitText2D関数 - テキスト(2D)の初期化処理 -
//========================================
void InitText2D(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TEXT2D_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffText2D,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffText2D->Lock(0, 0, (void**)&pVtx, 0);

	// 管理情報のパラメーターの初期化処理
	InitParameterText2DControl();

	Text2D	*pTxt	// テキスト(2D)の情報のポインタ
			= g_text2D;

	for (int nCntText = 0; nCntText < TEXT2D_MAX; nCntText++, pTxt++)
	{
		// パラメーターの初期化処理
		InitParameterText2D(pTxt);

		for (int nCntDigit = 0; nCntDigit < TEXT_DIGIT_MAX; nCntDigit++, pVtx += 4)
		{
			// rhwの設定
			SetRHW2D(pVtx);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffText2D->Unlock();
}

//========================================
// UninitText2D関数 - テキスト(2D)の終了処理 -
//========================================
void UninitText2D(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffText2D != NULL)
	{
		g_pVtxBuffText2D->Release();
		g_pVtxBuffText2D = NULL;
	}
}

//========================================
// DrawText2D関数 - テキスト2Dの描画処理 -
//========================================
void DrawText2D(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffText2D, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Text2D			*pTxt		// テキストの情報のポインタ
					= g_text2D;
	Text2DControl	*pTxtCtl	// テキスト(2D)の管理情報のポインタ
					= &g_text2DControl;

	for (int nCntTxt = 0; nCntTxt < pTxtCtl->nUseNum; nCntTxt++, pTxt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureText[pTxt->font]);

		for (int nCntDigit = 0; nCntDigit < strlen(pTxt->aString); nCntDigit++)
		{
			if ((nCntDigit < strlen(pTxt->aString)) && pTxt->aString[nCntDigit] != ' ')
			{// 桁のカウントが文字列の桁数を下回っている & その文字が空白で無い時、
				// テキストの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}
	}

	pTxtCtl->nUseNum = 0;	// 使用している数を初期化する
}

//============================================================
//-----|------------------------------------------------------
//=====| テキスト(3D)
//-----|------------------------------------------------------
//============================================================

//========================================
// InitParameterText3D関数 - テキスト(3D)のパラメーターの初期化処理 -
//========================================
void InitParameterText3D(Text3D *pText)
{
	pText->font		= (FONT)0;							// フォント
	pText->pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pText->bZtest	= false;							// Zテストフラグ
}

//========================================
// InitParameterText3DControl関数 - テキスト(3D)の管理情報のパラメーターの初期化処理 -
//========================================
void InitParameterText3DControl(void)
{
	Text3DControl	*pTxtCtl	// テキスト(3D)の管理情報のポインタ
					= &g_text3DControl;

	pTxtCtl->nUseNum = 0;	// 使用されている数
}

//========================================
// InitText3D関数 - テキスト(3D)の初期化処理 -
//========================================
void InitText3D(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * TEXT3D_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffText3D,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 管理情報のパラメーターの初期化処理
	InitParameterText3DControl();

	Text3D	*pTxt	// テキスト(3D)の情報のポインタ
		= g_text3D;

	for (int nCntText = 0; nCntText < TEXT3D_MAX; nCntText++, pTxt++)
	{
		// パラメーターの初期化処理
		InitParameterText3D(pTxt);
	}
}

//========================================
// UninitText3D関数 - テキスト(3D)の終了処理 -
//========================================
void UninitText3D(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffText3D != NULL)
	{
		g_pVtxBuffText3D->Release();
		g_pVtxBuffText3D = NULL;
	}
}

//========================================
// DrawText3D関数 - テキスト3Dの描画処理 -
//========================================
void DrawText3D(void)
{
	LPDIRECT3DDEVICE9	pDevice		// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot,		// 計算用マトリックス
						mtxTrans;
	D3DXMATRIX			mtxView;	// ビューマトリックス取得用

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffText3D, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	Text3D			*pTxt		// テキストの情報のポインタ
					= g_text3D;
	Text3DControl	*pTxtCtl	// テキスト(3D)の管理情報のポインタ
					= &g_text3DControl;

	for (int nCntTxt = 0; nCntTxt < pTxtCtl->nUseNum; nCntTxt++, pTxt++)
	{
		if (!pTxt->bZtest) 
		{// Zテストフラグが偽の時、
			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureText[pTxt->font]);

		for (int nCntDigit = 0; nCntDigit < strlen(pTxt->aString); nCntDigit++)
		{
			if ((nCntDigit < strlen(pTxt->aString)) && pTxt->aString[nCntDigit] != ' ')
			{// 桁のカウントが文字列の桁数を下回っている & その文字が空白で無い時、
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWorldText3D);

				// ビューマトリックスを取得
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				if (pTxt->bBillboard) 
				{// ビルボードフラグが真の時、
					// ポリゴンをカメラに対して正面に向ける
					D3DXMatrixInverse(&g_mtxWorldText3D, NULL, &mtxView);
					g_mtxWorldText3D._41 = 0.0f;
					g_mtxWorldText3D._42 = 0.0f;
					g_mtxWorldText3D._43 = 0.0f;
				}
				else
				{// ビルボードフラグが偽の時、
					// 向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRot, FindAngleLookDown(GetCamera3D()->posV, GetCamera3D()->posR), 0.0f, 0.0f);
					D3DXMatrixMultiply(&g_mtxWorldText3D, &g_mtxWorldText3D, &mtxRot);
				}

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, pTxt->pos.x, pTxt->pos.y, pTxt->pos.z);
				D3DXMatrixMultiply(&g_mtxWorldText3D, &g_mtxWorldText3D, &mtxTrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldText3D);

				// テキストの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}

		if (!pTxt->bZtest)
		{// Zテストフラグが偽の時、
			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}

	pTxtCtl->nUseNum = 0;	// 使用している数を初期化する

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitText関数 - テキストの初期化処理 -
//========================================
void InitText(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aFont[nCntFont].c_aPath, &g_pTextureText[nCntFont]);
	}

	// 初期化処理
	InitText3D();	// テキスト(3D)
	InitText2D();	// テキスト(2D)
}

//========================================
// UninitText関数 - テキストの終了処理 -
//========================================
void UninitText(void)
{
	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// テクスチャの破棄
		if (g_pTextureText[nCntFont] != NULL)
		{
			g_pTextureText[nCntFont]->Release();
			g_pTextureText[nCntFont] = NULL;
		}
	}

	// 終了処理
	UninitText3D();	// テキスト(3D)
	UninitText2D();	// テキスト(2D)
}

//========================================
// DrawText_関数 - テキストの描画処理 -
//========================================
void DrawText_(void)
{
	// 描画処理
	DrawText3D();	// テキスト(3D)
	DrawText2D();	// テキスト(2D)
}

//============================================================
//-----|------------------------------------------------------
//=====| テキスト(2D)
//-----|------------------------------------------------------
//============================================================

//========================================
// SetText2D関数 - テキスト(2D)の設定処理 -
//========================================
void SetText2D(char *pString, FONT font, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight)
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffText2D->Lock(0, 0, (void**)&pVtx, 0);

	Text2DControl	*pTxtCtl	// テキスト(2D)の管理情報のポインタ
					= &g_text2DControl;
	Text2D			*pTxt		// テキスト(2D)の情報のポインタ
					= &g_text2D[pTxtCtl->nUseNum];

	strcpy(pTxt->aString, pString);	// 文字列を代入
	pTxt->font = font;				// フォントを代入

	pVtx += 4 * pTxtCtl->nUseNum * TEXT_DIGIT_MAX;
	for (int nCntDigit = 0; nCntDigit < strlen(pString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 setPos = pos;	// 設定位置

		// 表示形式に応じた設定位置の設定
		switch (disp)
		{
		case DISPLAY_CENTER:	//========== *** 中央揃え ***
			setPos.x -= (((g_aFont[font].c_fSpaceX * PIXEL) * fWidth) * strlen(pString)) * 0.5f;	// 設定位置に文字の最終的な長さの半分を減算
			setPos.x += ((float)nCntDigit + 0.5f) * ((g_aFont[font].c_fSpaceX * PIXEL) * fWidth);	// 設定位置に配置間隔Xを適用(加算)
			break;
		case DISPLAY_LEFT:		//========== *** 左詰め ***
			setPos.x += nCntDigit * ((g_aFont[font].c_fSpaceX * PIXEL) * fWidth);					// 設定位置に配置間隔Xを適用(加算)
			break;
		case DISPLAY_RIGHT:		//========== *** 右詰め ***
			setPos.x -= ((g_aFont[font].c_fSpaceX * PIXEL) * fWidth) * (strlen(pString) - 1);			// 設定位置に文字の最終的な長さを減算
			setPos.x += nCntDigit * ((g_aFont[font].c_fSpaceX * PIXEL) * fWidth);					// 設定位置に配置間隔Xを適用(加算)
			break;
		}

		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			setPos,
			{},
			false,
			(g_aFont[font].c_fWidth * PIXEL) * fWidth,
			(g_aFont[font].c_fHeight * PIXEL) * fHeight,
			ANGLE_TYPE_FIXED);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, col);

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx,
			pString[nCntDigit] - g_aFont[font].c_cStartChar,
			g_aFont[font].c_nPtnMaxX,
			g_aFont[font].c_nPtnMaxY,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffText2D->Unlock();

	pTxtCtl->nUseNum++;	// 使用されている数を加算
}

//============================================================
//-----|------------------------------------------------------
//=====| テキスト(3D)
//-----|------------------------------------------------------
//============================================================

//========================================
// SetText3D関数 - テキスト(3D)の設定処理 -
//========================================
void SetText3D(char *pString, FONT font, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard)
{
	VERTEX_3D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffText3D->Lock(0, 0, (void**)&pVtx, 0);

	Text3DControl	*pTxtCtl	// テキスト(3D)の管理情報のポインタ
					= &g_text3DControl;
	Text3D			*pTxt		// テキスト(3D)の情報のポインタ
					= &g_text3D[pTxtCtl->nUseNum];

	strcpy(pTxt->aString, pString);	// 文字列を代入
	pTxt->font = font;				// フォントを代入
	pTxt->pos = pos;				// 位置を代入
	pTxt->bZtest = bZtest;			// Zテストフラグを代入
	pTxt->bLighting = bLighting;	// ライティングフラグを代入
	pTxt->bBillboard = bBillboard;	// ビルボードフラグを代入

	pVtx += 4 * pTxtCtl->nUseNum * TEXT_DIGIT_MAX;
	for (int nCntDigit = 0; nCntDigit < strlen(pString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 setPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 設定位置

		// 表示形式に応じた設定位置の設定
		switch (disp)
		{
		case DISPLAY_CENTER:	//========== *** 中央揃え ***
			setPos.x -= (((g_aFont[font].c_fSpaceX * PIXEL3D) * fWidth) * strlen(pString)) * 0.5f;	// 設定位置に文字の最終的な長さの半分を減算
			setPos.x += ((float)nCntDigit + 0.5f) * ((g_aFont[font].c_fSpaceX * PIXEL3D) * fWidth);	// 設定位置に配置間隔Xを適用(加算)
			break;
		case DISPLAY_LEFT:		//========== *** 左詰め ***
			setPos.x += nCntDigit * ((g_aFont[font].c_fSpaceX * PIXEL3D) * fWidth);					// 設定位置に配置間隔Xを適用(加算)
			break;
		case DISPLAY_RIGHT:		//========== *** 右詰め ***
			setPos.x -= ((g_aFont[font].c_fSpaceX * PIXEL3D) * fWidth) * (strlen(pString) - 1);			// 設定位置に文字の最終的な長さを減算
			setPos.x += nCntDigit * ((g_aFont[font].c_fSpaceX * PIXEL3D) * fWidth);					// 設定位置に配置間隔Xを適用(加算)
			break;
		}

		float fCWidth	= (g_aFont[font].c_fWidth * PIXEL3D) * fWidth;
		float fCHeight	= (g_aFont[font].c_fHeight * PIXEL3D) * fHeight;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(setPos.x - (fCWidth * 0.5f), fCHeight + (fCHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(setPos.x + (fCWidth * 0.5f), fCHeight + (fCHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(setPos.x - (fCWidth * 0.5f), fCHeight - (fCHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(setPos.x + (fCWidth * 0.5f), fCHeight - (fCHeight * 0.5f), 0.0f);

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, col);

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx,
			pString[nCntDigit] - g_aFont[font].c_cStartChar,
			g_aFont[font].c_nPtnMaxX,
			g_aFont[font].c_nPtnMaxY,
			NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffText3D->Unlock();

	pTxtCtl->nUseNum++;	// 使用されている数を加算
}