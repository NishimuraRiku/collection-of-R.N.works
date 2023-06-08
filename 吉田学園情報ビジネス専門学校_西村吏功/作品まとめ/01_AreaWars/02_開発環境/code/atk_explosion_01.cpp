//========================================
// 
// 爆発[01] の処理
// Author:西村 吏功
// 
//========================================
// *** atk_explosion_01.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "sound.h"
#include "chr_fighter_00.h"
#include "chr_enemy_00.h"
#include "eff_light_00.h"
#include "eff_particle_00.h"
#include "atk_bullet_00.h"
#include "atk_explosion_01.h"
#include "input.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
#define ATK_EXPLOSION_01_MAX		(256)	// 爆発[01] の最大数
#define SCALE_DAMPING_MAGNIFICATION	(0.1f)	// 拡大量の減衰倍率

//****************************************
// 列挙型の定義
//****************************************

// 標的タイプ
typedef enum
{
	TARGET_TYPE_ENEMY_ONRY
}TARGET_TYPE;

//****************************************
// 構造体の定義
//****************************************

// 爆発[01] 構造体の定義
typedef struct 
{
								//========== *** 位置関連 ***
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	float fWidth;				// 幅
	float fHeight;				// 高さ
	float fHitTestRadius;		// 当たり判定の半径
	float fScale;				// 拡大量
								//========== *** 分類情報 ***
	ATK_EXPLOSION_01_TYPE type;	// 種類
	bool bUse;					// 使用されているかフラグ
								//========== *** 状態値 ***
	int nLife;					// 寿命
	int nDamage;				// ダメージ
	int nPattern;				// パターンNo.
	int nCounterDamage;			// ダメージカウンター
}Atk_explosion_01;

// 爆発[01] の種類毎の情報
typedef struct
{
	char					aTexturPath[TXT_MAX];	// テクスチャの相対パス
	int						nPtnMaxX;				// パターンの最大数(横)
	int						nPtnMaxY;				// パターンの最大数(縦)
	int						nDamage;				// ダメージ
	int						nDamageTime;			// ダメージの発生間隔
	int						nLife;					// 寿命
	float					fScale;					// 拡大量
	float					fHitRadiusDiameter;		// 当たり判定の半径倍率
	SOUND_LABEL				soundSet;				// 設定時のサウンド
	Color					col;					// 色
	EFF_PARTICLE_00_TYPE	ptclType;				// パーティクル[00] の種類
	int						nPtclNum;				// パーティクルの発生数
	float					fVibration;				// 振動の強さ
	TARGET_TYPE				targetType;				// 目標タイプ
}Atk_explosion_01Type;

//****************************************
// プロトタイプ宣言
//****************************************

// 機体[00] との当たり判定処理
void CollisionChr_fighter_00(Atk_explosion_01 *pAtk);

// 敵[00] との当たり判定
void CollisionChr_enemy_00(Atk_explosion_01 *pAtk);

// 弾[00] との当たり判定
void CollisionAtk_bullet_00(Atk_explosion_01 *pAtk);

// 爆発[01] のステータスを種類に応じて設定
void SetState(Atk_explosion_01 *pAtk);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9 g_aTextureAtk_explosion_01[ATK_EXPLOSION_01_TYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_explosion_01 = NULL;						// 頂点バッファへのポインタ
Atk_explosion_01 g_aAtk_explosion_01[ATK_EXPLOSION_01_MAX];						// 爆発[01] の情報

// 爆発[01] の種類の情報
Atk_explosion_01Type g_aAtk_explosion_01Type[ATK_EXPLOSION_01_TYPE_MAX] =
{//   テクスチャの相対パス             , パターンの最大X, Y, ダメージ, 発生間隔, 寿命, 拡大量, 当たり判定の半径倍率, SE                           , 色                    , パーティクルの種類           , パーティクルの発生数 , 振動の強さ, 標的タイプ
	{ "data\\TEXTURE\\eff_ring_002.jpg", 1              , 1, 5       , 0       , 32  , 120.0f, 0.8f                , SOUND_LABEL_SE_EXPLOSION_005 , { 255, 155, 59 , 255 }, EFF_PARTICLE_00_TYPE_NONE    , 0                    , 16.0f     , TARGET_TYPE_ENEMY_ONRY},
};

//========================================
// InitAtk_explosion_01関数 - 爆発[01] の初期化処理 -
//========================================
void InitAtk_explosion_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntAtk_explosion_01_type = 0; nCntAtk_explosion_01_type < ATK_EXPLOSION_01_TYPE_MAX; nCntAtk_explosion_01_type++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aAtk_explosion_01Type[nCntAtk_explosion_01_type].aTexturPath, &g_aTextureAtk_explosion_01[nCntAtk_explosion_01_type]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ATK_EXPLOSION_01_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_explosion_01,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_explosion_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++, pVtx += 4)
	{
		Atk_explosion_01 *pAtk = &g_aAtk_explosion_01[nCntAtk_explosion_01];	// 爆発[01] の情報のポインタ

		pAtk->nPattern		= 0;								// パターンNo.を初期化する
		pAtk->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置を初期化する
		pAtk->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きを初期化する
		pAtk->bUse			= false;							// 使用されているかフラグを初期化する
		pAtk->type			= ATK_EXPLOSION_01_TYPE_00;			// 種類を初期化する			

		// ステータスを種類に応じて設定
		SetState(pAtk);

		{
			D3DXVECTOR3 pos = pAtk->pos;												// 位置
			D3DXVECTOR3 rot = pAtk->rot;												// 向き
			float fWidth	= pAtk->fWidth;												// 幅
			float fHeight	= pAtk->fHeight;											// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		{
			int nPtn		= pAtk->nPattern;								// テクスチャのパターンNo.
			int nPtnMaxX	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxX;	// テクスチャのパターン数(横)
			int nPtnMaxY	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxY;	// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffAtk_explosion_01->Unlock();
}

//========================================
// UninitAtk_explosion_01関数 - 爆発[01] の終了処理 -
//========================================
void UninitAtk_explosion_01(void)
{
	// テクスチャの破棄
	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_TYPE_MAX; nCntAtk_explosion_01++)
	{
		if (g_aTextureAtk_explosion_01[nCntAtk_explosion_01] != NULL)
		{
			g_aTextureAtk_explosion_01[nCntAtk_explosion_01]->Release();
			g_aTextureAtk_explosion_01[nCntAtk_explosion_01] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffAtk_explosion_01 != NULL) 
	{
		g_pVtxBuffAtk_explosion_01->Release();
		g_pVtxBuffAtk_explosion_01 = NULL;
	}
}

//========================================
// UpdateAtk_explosion_01関数 - 爆発[01] の更新処理 -
//========================================
void UpdateAtk_explosion_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_explosion_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++, pVtx += 4)
	{
		if (!g_aAtk_explosion_01[nCntAtk_explosion_01].bUse)
		{// 爆発[01] が使用されている状態でない時、
			// 繰り返し処理を折り返す
			continue;
		}

		Atk_explosion_01 *pAtk = &g_aAtk_explosion_01[nCntAtk_explosion_01];	// 爆発[01] の情報のポインタ

		// 寿命を減算
		pAtk->nLife--;

		if (pAtk->nLife <= 0) 
		{// 寿命が尽きた時、
			// 使用していない状態にする
			pAtk->bUse = false;

			// 繰り返し処理を折り返す
			continue;
		}

		D3DXVECTOR3 pos		= pAtk->pos;											// 位置
		D3DXVECTOR3 rot		= pAtk->rot;											// 向き
		int nPtn			= pAtk->nPattern;										// テクスチャのパターンNo.
		int nPtnMaxX		= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxX;			// テクスチャのパターン数(横)
		int nPtnMaxY		= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxY;			// テクスチャのパターン数(縦)

		if (pAtk->nPattern == nPtnMaxX * nPtnMaxY)
		{// 爆発[01] のパターンNo.がパターンの上限を上回った時、
			pAtk->bUse = false; // 使用していない状態にする
		}

		// 幅/高さを更新
		pAtk->fWidth += pAtk->fScale;
		pAtk->fHeight += pAtk->fScale;
		pAtk->fHitTestRadius = pAtk->fWidth*0.5f;

		// 拡大量を更新(減衰)
		pAtk->fScale += (0.0f - pAtk->fScale) * SCALE_DAMPING_MAGNIFICATION;

		SetEff_light_00(pAtk->pos, pAtk->rot, 0.0f, 
			g_aAtk_explosion_01Type[pAtk->type].col,
			pAtk->fWidth, 
			pAtk->fHeight, 
			g_aAtk_explosion_01Type[pAtk->type].nLife-pAtk->nLife,
			EFF_LIGHT_00_TYPE_01);

		if (++pAtk->nCounterDamage >= g_aAtk_explosion_01Type[pAtk->type].nDamageTime) 
		{// ダメージカウンターがダメージの発生間隔に達した時、
			// ダメージカウンターを初期化
			pAtk->nCounterDamage = 0;

			// 目標タイプに応じた当たり判定処理
			switch (g_aAtk_explosion_01Type[pAtk->type].targetType)
			{
				//========== *** 敵のみ ***
			case TARGET_TYPE_ENEMY_ONRY:

				// 敵[00]との当たり判定処理
				CollisionChr_enemy_00(pAtk);

				break;
			}
		}

		// 弾[00] との当たり判定処理
		CollisionAtk_bullet_00(pAtk);

		{
			D3DXVECTOR3 drawPos = pos;																			// 描画位置
			float fLength	= sqrtf((pAtk->fWidth * pAtk->fWidth) + (pAtk->fHeight * pAtk->fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(pAtk->fWidth, pAtk->fHeight);												// 対角線の角度

			// 描画位置を設定
			MatchCameraPosition(&drawPos);

			// 頂点座標を設定
			pVtx[0].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		{
			float fAlpha = ((float)pAtk->nLife / (float)g_aAtk_explosion_01Type[pAtk->type].nLife);	// 透明度
			Color col = g_aAtk_explosion_01Type[pAtk->type].col;									// 色

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
			pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
			pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
			pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
	}

	// 頂点座標をアンロックする
	g_pVtxBuffAtk_explosion_01->Unlock();
}

//========================================
// DrawAtk_explosion_01関数 - 爆発[01] の描画処理 -
//========================================
void DrawAtk_explosion_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_explosion_01, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++)
	{
		if (g_aAtk_explosion_01[nCntAtk_explosion_01].bUse)
		{// 使用されている時、
			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureAtk_explosion_01[g_aAtk_explosion_01[nCntAtk_explosion_01].type]);

			// 機体[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAtk_explosion_01 * 4, 2);
		}
	}

	// アルファブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// Atk_explosion_01関数 - 爆発[01] の設定処理 -
//========================================
void SetAtk_explosion_01(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ATK_EXPLOSION_01_TYPE type)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_explosion_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++, pVtx += 4)
	{
		if (g_aAtk_explosion_01[nCntAtk_explosion_01].bUse)
		{// 爆発[01] が使用されている時、
			// 繰り返し処理を折り返す
			continue;
		}

		Atk_explosion_01 *pAtk = &g_aAtk_explosion_01[nCntAtk_explosion_01];	// 爆発[01] の情報のポインタ

		// 振動を設定
		SetCameraVibration(g_aAtk_explosion_01Type[type].fVibration);

		// 爆発[01] の種類を設定する
		pAtk->type = type;

		// ステータスを種類に応じて設定
		SetState(pAtk);

		// 効果音を再生
		PlaySound(g_aAtk_explosion_01Type[type].soundSet);

		// 位置/角度を代入
		pAtk->pos = pos;
		pAtk->rot = rot;

		// 幅/高さ/当たり判定の半径を初期化
		pAtk->fWidth = 0.0f;
		pAtk->fHeight = 0.0f;
		pAtk->fHitTestRadius = 0.0f;

		// ダメージカウンターを初期化
		pAtk->nCounterDamage = g_aAtk_explosion_01Type[type].nDamageTime;

		{
			D3DXVECTOR3 drawPos = pAtk->pos;												// 描画位置
			D3DXVECTOR3 rot		= pAtk->rot;												// 向き
			float fWidth		= pAtk->fWidth;												// 幅
			float fHeight		= pAtk->fHeight;											// 高さ
			float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle		= atan2f(fWidth, fHeight);									// 対角線の角度

			// 描画位置を設定
			MatchCameraPosition(&drawPos);

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		pAtk->nPattern		= 0;	// パターンNo.を初期化する

		{
			int nPtn		= pAtk->nPattern;								// テクスチャのパターンNo.
			int nPtnMaxX	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxX;	// テクスチャのパターン数(横)
			int nPtnMaxY	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxY;	// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}

		// 使用している状態にする
		pAtk->bUse = true;

		// パーティクルの設定処理
		for (int nCntPtcl = 0; nCntPtcl < g_aAtk_explosion_01Type[pAtk->type].nPtclNum; nCntPtcl++) 
		{
			D3DXVECTOR3 setRot;	// 設定する向きを設定
			setRot.z = -D3DX_PI + fRand(D3DX_PI * 2);

			// パーティクル[00] を設定
			SetEff_particle_00(pos, setRot, g_aAtk_explosion_01Type[pAtk->type].ptclType);
		}

		break;
	}

	// 頂点座標をアンロックする
	g_pVtxBuffAtk_explosion_01->Unlock();
}

//========================================
// SetState関数 - 爆発[01] のステータスを種類に応じて設定 -
//========================================
void SetState(Atk_explosion_01 *pAtk_explosion_01)
{
	pAtk_explosion_01->nLife	= g_aAtk_explosion_01Type[pAtk_explosion_01->type].nLife;	// 寿命
	pAtk_explosion_01->nDamage	= g_aAtk_explosion_01Type[pAtk_explosion_01->type].nDamage;	// ダメージ
	pAtk_explosion_01->fScale	= g_aAtk_explosion_01Type[pAtk_explosion_01->type].fScale;	// 拡大量
}

//========================================
// CollisionChr_fighter_00関数 - 機体[00]との当たり判定処理 -
//========================================
void CollisionChr_fighter_00(Atk_explosion_01 *pAtk)
{
	if (!GetChr_fighter_00()->bUse) 
	{// 機体[00] が使用されていない状態の時、
		// 処理を終了する
		return;
	}

	if (FindDistance(pAtk->pos, GetChr_fighter_00()->pos) < pAtk->fHitTestRadius + GetChr_fighter_00()->fHitTestWidth)
	{// 弾[00] の座標が機体[00] の当たり判定内に入った時、
		// 機体[00] のヒット処理
		HitChr_fighter_00(pAtk->nDamage);
	}
}

//========================================
// CollisionChr_enemy_00関数 - 敵[00]との当たり判定処理 -
//========================================
void CollisionChr_enemy_00(Atk_explosion_01 *pAtk)
{
	Chr_enemy_00 *pChr = GetChr_enemy_00();	// 敵[00] の情報のポインタ

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if ((!pChr->bUse)
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_POP])
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
			|| (pChr->aState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_GHOST]))
		{// 使用されていない状態、もしくは出現中/消え中/爆発耐性/膨らみ/膨らみ待ち/すり抜けの時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (FindDistance(pAtk->pos, pChr->pos) < (pAtk->fHitTestRadius * g_aAtk_explosion_01Type[pAtk->type].fHitRadiusDiameter) + pChr->fWidth)
		{// 爆発[01] が敵[00] の当たり判定内に入った時、
			// 爆発を受けたフラグを真にする
			pChr->bReceivedExplosion = true;

			// 膨らみ待ちにかかる時間を設定
			pChr->nSwellWaitTime = FindDistance(pAtk->pos, pChr->pos) * CHR_ENEMY_00_SWELL_WAIT_TIME_DIAMETER_EXPLOSION;

			// 敵[00] のヒット処理
			HitChr_enemy_00(pChr, pAtk->nDamage);
		}
	}
}

//========================================
// CollisionAtk_bullet_00関数 - 弾[00] との当たり判定処理 -
//========================================
void CollisionAtk_bullet_00(Atk_explosion_01 *pAtk) 
{
	Atk_bullet_00	*pAtk_b					//
					= GetAtk_bullet_00();	// 弾[00] の情報のポインタ

	for (int nCntAtk_b = 0; nCntAtk_b < ATK_BULLET_00_MAX; nCntAtk_b++,pAtk_b++) 
	{
		if (!pAtk_b->bUse)
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (FindDistance(pAtk->pos, pAtk_b->pos) < pAtk->fHitTestRadius + pAtk_b->fWidth)
		{// 爆発[01] が弾[00] の当たり判定内に入った時、
			// 弾[00] の破壊処理
			DestructionAtk_bullet_00(pAtk_b);
		}
	}
}