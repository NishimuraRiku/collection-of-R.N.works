//========================================
// 
// EFF:光[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_light_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "eff_light_00.h"	// EFF:光	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// EFF:光[00] の種類毎の情報のデータファイルの相対パス
#define EFF_LIGHT_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\EFFECT\\EFF_LIGHT_00_TYPE_DATA.txt"

// EFF:光[00] の最大数
// EFF:光[00] の種類の最大数
#define EFF_LIGHT_00_MAX		(2048)
#define EFF_LIGHT_00_TYPE_MAX	(8)

// EFF:光[00] の法線ベクトル
// EFF:光[00] の相対位置
#define EFF_LIGHT_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define EFF_LIGHT_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 4.0f, 0.0f))

//****************************************
// 構造体の定義
//****************************************
// EFF:光[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	
	// 分類関連
	int	nType;	// 種類
	
	// ポリゴン関連
	int nPtn;			// パターンNo.
	int nCounterAnim;	// アニメーションカウンター

	// 状態関連
	bool	bUse;	// 使用されているかフラグ
	int		nLife;	// 寿命
}Eff_light_00;

// EFF:光[00] の種類毎の情報構造体
typedef struct
{
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	char		aTexturePath	// テクスチャの相対パス
				[TXT_MAX];
	int			nPtnWidth;		// パターン幅
	int			nPtnHeight;		// パターン高さ
	int			nPtnMax;		// パターン上限
	int			nAnimTime;		// アニメーションにかかる時間
	int			nLife;			// 寿命
	Color		col;			// 色
}Eff_light_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// EFF:光[00] のパラメーターの初期化処理
void InitParameterEff_light_00(Eff_light_00 *pEff);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_light_00	// EFF:光[00] のテクスチャへのポインタ
						[EFF_LIGHT_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_light_00	// EFF:光[00] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldEff_light_00;	// EFF:光[00] のワールドマトリックス
Eff_light_00			g_aEff_light_00			// EFF:光[00] の情報
						[EFF_LIGHT_00_MAX];
Eff_light_00Type		g_aEff_light_00Type		// EFF:光[00] の種類毎の情報
						[EFF_LIGHT_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_light_00関数 - EFF:光[00] のパラメーターの初期化処理 -
//========================================
void InitParameterEff_light_00(Eff_light_00 *pEff)
{
	pEff->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pEff->nType			= 0;								// 種類
	pEff->nPtn			= 0;								// パターンNo.
	pEff->nCounterAnim	= 0;								// アニメーションカウンター
	pEff->bUse			= false;							// 使用されているかフラグ
	pEff->nLife			= 0;
}

//========================================
// DestroyEff_light_00関数 - EFF:光[00] の破壊処理 -
//========================================
void DestroyEff_light_00(Eff_light_00 *pEff) 
{
	pEff->bUse = false;	// 使用されていない状態にする
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadEff_light_00関数 - EFF:光[00] の読み込み処理 -
//========================================
void LoadEff_light_00(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Eff_light_00Type	*pEffType	// EFF:光[00] の種類毎の情報
						= g_aEff_light_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(EFF_LIGHT_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// 読み込みを終了
		if (aDataSearch[0] == '#')			{ continue; }				// 折り返す

		if (!strcmp(aDataSearch, "TYPE")) {
			// EFF:光[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pEffType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pEffType->fWidth); }			// 幅
				else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pEffType->fHeight); }		// 高さ
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))	{ fscanf(pFile, "%s", &pEffType->aTexturePath); }	// テクスチャの相対パス
				else if (!strcmp(aDataSearch, "PTN_WIDTH:"))	{ fscanf(pFile, "%d", &pEffType->nPtnWidth); }		// パターン幅
				else if (!strcmp(aDataSearch, "PTN_HEIGHT:"))	{ fscanf(pFile, "%d", &pEffType->nPtnHeight); }		// パターン高さ
				else if (!strcmp(aDataSearch, "PTN_MAX:"))		{ fscanf(pFile, "%d", &pEffType->nPtnMax); }		// パターン上限
				else if (!strcmp(aDataSearch, "ANIM_TIME:"))	{ fscanf(pFile, "%d", &pEffType->nAnimTime); }		// アニメーションにかかる時間
				else if (!strcmp(aDataSearch, "LIFE:"))			{ fscanf(pFile, "%d", &pEffType->nLife); }			// 寿命
				else if (!strcmp(aDataSearch, "COLOR:"))		{	// 色
					fscanf(pFile, "%d", &pEffType->col.r);			// R値
					fscanf(pFile, "%d", &pEffType->col.g);			// G値
					fscanf(pFile, "%d", &pEffType->col.b);			// B値
					fscanf(pFile, "%d", &pEffType->col.a);			// A値
				}
			}
		}
	}
}

//========================================
// InitEff_light_00関数 - EFF:光[00] の初期化処理 -
//========================================
void InitEff_light_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	for (int nCntType = 0; nCntType < EFF_LIGHT_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aEff_light_00Type[nCntType].aTexturePath, &g_aTextureEff_light_00[nCntType]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFF_LIGHT_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_light_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_light_00	*pEff	// EFF:光[00] の情報のポインタ
					= g_aEff_light_00;

	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++, pEff++, pVtx += 4)
	{
		Eff_light_00Type	*pType	// EFF:光[00] の種類毎の情報のポインタ
							= &g_aEff_light_00Type[pEff->nType];

		// パラメーターの初期化処理
		InitParameterEff_light_00(&g_aEff_light_00[nCntEff]);

		// 法線ベクトルの設定
		SetNormalLine3D(pVtx, EFF_LIGHT_00_NOR);

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, pType->col);

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx, 0, pType->nPtnWidth, pType->nPtnHeight, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// UninitEff_light_00関数 - EFF:光[00] の終了処理 -
//========================================
void UninitEff_light_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < EFF_LIGHT_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureEff_light_00[nCntType] != NULL)
		{
			g_aTextureEff_light_00[nCntType]->Release();
			g_aTextureEff_light_00[nCntType] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEff_light_00 != NULL)
	{
		g_pVtxBuffEff_light_00->Release();
		g_pVtxBuffEff_light_00 = NULL;
	}
}

//========================================
// UpdateEff_light_00関数 - EFF:光[00] の更新処理 -
//========================================
void UpdateEff_light_00(void)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_light_00	*pEff	// EFF:光[00] の情報のポインタ
					= g_aEff_light_00;
	
	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++, pEff++, pVtx += 4)
	{
		if (!pEff->bUse)
		{// 使用されている状態でない時、
			// 繰り返し処理を折り返す
			continue;
		}
		else if (--pEff->nLife <= 0) 
		{// 寿命を減算した結果0以下の時、
			pEff->bUse = false;	// 使用されていない状態にする
			continue;			// 繰り返し処理を折り返す
		}

		Eff_light_00Type	*pType	// EFF:光[00] の種類毎の情報のポインタ
							= &g_aEff_light_00Type[pEff->nType];

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx,
			Count(
				&pEff->nPtn,
				0,
				g_aEff_light_00Type[pEff->nType].nPtnMax,
				&pEff->nCounterAnim,
				g_aEff_light_00Type[pEff->nType].nAnimTime,
				COUNT_TYPE_NORMAL),
			g_aEff_light_00Type[pEff->nType].nPtnWidth,
			g_aEff_light_00Type[pEff->nType].nPtnHeight, NULL);

		// 頂点カラーの設定
		{
			Color col = pType->col;	// 色
			col.a *= ((float)pEff->nLife / (float)pType->nLife);
			SetVertexColor3D(pVtx, col);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// DrawEff_light_00関数 - EFF:光[00] の描画処理 -
//========================================
void DrawEff_light_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// 計算用マトリックス
	D3DXMATRIX			mtxView;	// ビューマトリックス取得用
	Eff_light_00		*pEff		// EFF:光[00] の情報のポインタ
						= g_aEff_light_00;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEff_light_00, 0, sizeof(VERTEX_3D));

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

	// アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++,pEff++)
	{
		if (!pEff->bUse)
		{// 使用されている状態でない時、
			// 処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldEff_light_00);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_mtxWorldEff_light_00, NULL, &mtxView);
		g_mtxWorldEff_light_00._41 = 0.0f;
		g_mtxWorldEff_light_00._42 = 0.0f;
		g_mtxWorldEff_light_00._43 = 0.0f;
		{
			D3DXVECTOR3 setPos = pEff->pos + EFF_LIGHT_00_RELATIVE_POS;	// 設定位置を設定

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldEff_light_00, &g_mtxWorldEff_light_00, &mtxTrans);
		}
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEff_light_00);

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureEff_light_00[pEff->nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEff, 2);
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	// アルファブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// SetEff_light_00関数 - EFF:光[00] の設定処理 -
//========================================
void SetEff_light_00(D3DXVECTOR3 pos,int nType)
{
	if (nType == -1)
	{// 種類が-1の時、
		return;	// 処理を終了
	}

	Eff_light_00	*pEff	// EFF:光[00] の情報のポインタ
					= g_aEff_light_00;
	VERTEX_3D		*pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++, pEff++, pVtx += 4)
	{
		if (pEff->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Eff_light_00Type	*pType	// 種類毎の情報のポインタ
							= &g_aEff_light_00Type[nType];

		pEff->pos = pos;			// 位置を代入
		pEff->nType = nType;		// 種類を代入
		pEff->nPtn = 0;				// パターンNo.を初期化
		pEff->nCounterAnim = 0;		// アニメーションカウンターを初期化
		pEff->bUse = true;			// 使用されている状態にする
		pEff->nLife = pType->nLife;	// 寿命を設定する

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f),  (pType->fHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (pType->fWidth * 0.5f),  (pType->fHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f), -(pType->fHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (pType->fWidth * 0.5f), -(pType->fHeight * 0.5f), 0.0f);

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, pType->col);

		// 繰り返し処理を抜ける
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEff_light_00->Unlock();
}