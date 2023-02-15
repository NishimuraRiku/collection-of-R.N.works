//========================================
// 
// メダル[00]の処理
// Author:西村 吏功
// 
//========================================
// *** item_medal_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon.h"
#include "sound.h"
#include "chr_fighter_00.h"
#include "item_medal_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include "ui_score_00.h"
#include <stdio.h>
#include <float.h>

//****************************************
// マクロ定義
//****************************************
// メダル[00] のテクスチャの相対パス
// メダル[00] のパターン上限(横)
// メダル[00] のパターン上限(縦)
// メダル[00] のアニメーションにかかる時間
// メダル[00] の幅
// メダル[00] の高さ
// メダル[00] の当たり判定の半径
// メダル[00] の追尾距離
// メダル[00] の追尾力
// メダル[00] の追尾を開始する寿命
// メダル[00] の寿命
// メダル[00] の点滅にかかる時間
// メダル[00] の点滅を開始する寿命
// メダル[00] のスコア
#define ITEM_MEDAL_00_TEXTURE_PATH			"data\\TEXTURE\\item_medal_000_border.png"
#define ITEM_MEDAL_00_PATTERN_MAX_X			(4)
#define ITEM_MEDAL_00_PATTERN_MAX_Y			(1)
#define ITEM_MEDAL_00_ANIMATION_TIME		(4)
#define ITEM_MEDAL_00_WIDTH					(PIXEL * 16)
#define ITEM_MEDAL_00_HEIGHT				(PIXEL * 16)
#define ITEM_MEDAL_00_HIT_RADIUS			(PIXEL * 8)
#define ITEM_MEDAL_00_TRACKING_DISTANCE		(160.0f)
#define ITEM_MEDAL_00_TRACKING				(8.0f)
#define ITEM_MEDAL_00_TRACKING_START_LIFE	(160)
#define ITEM_MEDAL_00_LIFE					(200)
#define ITEM_MEDAL_00_BRINK_TIME			(2)
#define ITEM_MEDAL_00_BRINK_START_LIFE		(80)
#define ITEM_MEDAL_00_SCORE					(5000)

//****************************************
// プロトタイプ宣言
//****************************************

// 機体[00] との当たり判定処理
void CollisionChr_fighter_00(Item_medal_00 *pItem);

// プラズマウォール[00] との当たり判定処理
void CollisionObj_plasma_wall_00(Item_medal_00 *pItem);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureItem_medal_00	//
						= NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem_medal_00	//
						= NULL;					// 頂点バッファへのポインタ
Item_medal_00			g_aItem_medal_00			//
						[ITEM_MEDAL_00_MAX];		// メダル[00] の情報

//========== *** メダル[00] の情報を取得 ***
Item_medal_00 *GetItem_medal_00(void)
{
	return g_aItem_medal_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterItem_medal_00関数 - メダル[00] のパラメーターの初期化処理 -
//========================================
void InitParameterItem_medal_00(Item_medal_00 *pItem) 
{
	pItem->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pItem->bUse				= false;							// 使用されているフラグ
	pItem->nLife			= 0;								// 寿命
	pItem->nCounterBrink	= 0;								// 点滅のカウンター
	pItem->bBrink			= false;							// 点滅フラグ
	pItem->nCounterAnim		= 0;								// アニメーションのカウンター
	pItem->nPattern			= 0;								// パターンNo.
}

//========================================
// TrackingItem_medal_00関数 - メダル[00] の追尾処理 -
//========================================
void TrackingItem_medal_00(Item_medal_00 *pItem) 
{
	if ((!GetChr_fighter_00()->bUse)
		|| (pItem->nLife > ITEM_MEDAL_00_TRACKING_START_LIFE))
	{// 機体[00] が使用されていない状態の時、もしくは寿命が追尾を開始する寿命を上回っていた時、
		// 処理を終了する
		return;
	}

	Chr_fighter_00	*pChr												//
					= GetChr_fighter_00();								// 機体[00] の情報のポインタ
	float			fDistance											//
					= FindDistance(pItem->pos, pChr->pos);				// 機体[00] との距離
	float			fMove												//
					= ITEM_MEDAL_00_TRACKING								//
					* (1.0f												//
					- (fDistance / ITEM_MEDAL_00_TRACKING_DISTANCE));	// 移動量
	float			fAngle												//
					= FindAngle(pItem->pos, pChr->pos);					// 向き

	if (fDistance > ITEM_MEDAL_00_TRACKING_DISTANCE) 
	{// 距離が追尾距離を上回っていた時、
		// 処理を終了する
		return;
	}
	
	// 位置を更新
	pItem->pos.x += sinf(fAngle) * fMove;
	pItem->pos.y += cosf(fAngle) * fMove;
}

//========================================
// CollisionChr_fighter_00関数 - 機体[00]との当たり判定処理 -
//========================================
void CollisionChr_fighter_00(Item_medal_00 *pItem)
{
	if (!GetChr_fighter_00()->bUse)
	{// 機体[00] が使用されていない状態の時、
		// 処理を終了する
		return;
	}

	Chr_fighter_00	*pChr					//
					= GetChr_fighter_00();	// 機体[00] の情報のポインタ

	if (CheckHit(
		pItem->pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		ITEM_MEDAL_00_HIT_RADIUS,
		ITEM_MEDAL_00_HIT_RADIUS,
		HIT_TEST_TYPE_CIRCLE,
		pChr->pos,
		pChr->rot,
		pChr->fHitTestWidth,
		pChr->fHitTestHeight,
		GetChr_fighter_00Type()[pChr->nType].hitTestType))
	{// メダル[00] が機体[00] の衝突した時、
		// ボーナスSE[000] を再生
		PlaySound(SOUND_LABEL_SE_BONUS_000);

		// 使用していない状態にする
		pItem->bUse = false;

		// スコアを加算
		pChr->nScore += ITEM_MEDAL_00_SCORE;

		// スコア[00] の設定処理
		SetUi_score_00(pChr->pos, ITEM_MEDAL_00_SCORE);
	}
}

//========================================
// CollisionObj_plasma_wall_00関数 - プラズマウォール[00]との当たり判定処理 -
//========================================
void CollisionObj_plasma_wall_00(Item_medal_00 *pItem)
{
	if (!pItem->bUse)
	{// メダル[00] が使用されていない時、
		// 処理を終了する
		return;
	}

	Obj_plasma_wall_00			*pObj								//
								= GetObj_plasma_wall_00();			// プラズマウォール[00] の情報
	Obj_plasma_block_00_square	*pSquare							//
								= GetObj_plasma_block_00_square();	// プラズマブロック[00] の描く四角形の情報

	for (int nCntObj = 0; nCntObj < OBJ_PLASMA_WALL_00_MAX; nCntObj++, pObj++)
	{
		float fWidth, fHeight;	// 幅と高さ

		if ((int)((pObj->rot.z / D3DX_PI) * 10) % 10 == 0)
		{// プラズマウォールの向きが縦の時、
			// 幅と高さを設定
			fWidth = OBJ_PLASMA_WALL_00_WIDTH;
			fHeight = pObj->fHeight;
		}
		else
		{// プラズマウォールの向きが横の時、
			// 幅と高さを設定
			fHeight = OBJ_PLASMA_WALL_00_WIDTH;
			fWidth = pObj->fHeight;
		}

		if ((nCntObj == OBJ_PLASMA_WALL_00_TOP) && (pItem->pos.y - ITEM_MEDAL_00_HIT_RADIUS < pSquare->fTop + (fHeight * 0.5f)))
		{// 上のプラズマウォールを越えていた時、
			// 位置を修正
			pItem->pos.y = pSquare->fTop + (fHeight * 0.5f) + ITEM_MEDAL_00_HIT_RADIUS;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_BOTTOM) && (pItem->pos.y + ITEM_MEDAL_00_HIT_RADIUS > pSquare->fBottom - (fHeight * 0.5f)))
		{// 下のプラズマウォールを越えていた時、
			// 位置を修正
			pItem->pos.y = pSquare->fBottom - (fHeight * 0.5f) - ITEM_MEDAL_00_HIT_RADIUS;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_LEFT) && (pItem->pos.x - ITEM_MEDAL_00_HIT_RADIUS < pSquare->fLeft + (fWidth * 0.5f)))
		{// 左のプラズマウォールを越えていた時、
			// 位置を修正
			pItem->pos.x = pSquare->fLeft + (fWidth * 0.5f) + ITEM_MEDAL_00_HIT_RADIUS;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_RIGHT) && (pItem->pos.x + ITEM_MEDAL_00_HIT_RADIUS > pObj->pos.x - (fWidth * 0.5f)))
		{// 右のプラズマウォールを越えていた時、
			// 位置を修正
			pItem->pos.x = pObj->pos.x - (fWidth * 0.5f) - ITEM_MEDAL_00_HIT_RADIUS;
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitItem_medal_00関数 - メダル[00]の初期化処理 -
//========================================
void InitItem_medal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ITEM_MEDAL_00_TEXTURE_PATH, &g_aTextureItem_medal_00);

	for (int nCntItem_medal_00 = 0; nCntItem_medal_00 < ITEM_MEDAL_00_MAX; nCntItem_medal_00++) 
	{
		g_aItem_medal_00[nCntItem_medal_00].bUse	= false;					// 使用されているかフラグ を初期化する
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ITEM_MEDAL_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem_medal_00,
		NULL);

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	Item_medal_00	*pItem				//
					= g_aItem_medal_00;	// メダル[00] の情報のポインタ

	for (int nCntItem = 0; nCntItem < ITEM_MEDAL_00_MAX; nCntItem++, pVtx += 4, pItem++)
	{
		// パラメーターの初期化処理
		InitParameterItem_medal_00(pItem);

		// 頂点座標 を設定
		SetVertexPosition(pVtx,
			pItem->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			true,
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			ANGLE_TYPE_FIXED);

		// rhwの設定
		SetRHW(pVtx);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&pItem->nPattern,
				0,
				ITEM_MEDAL_00_PATTERN_MAX_X
				* ITEM_MEDAL_00_PATTERN_MAX_Y,
				&pItem->nCounterAnim,
				ITEM_MEDAL_00_ANIMATION_TIME,
				ANIM_TYPE_NORMAL),
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			NULL);
	}
	
	// 頂点座標 をアンロックする
	g_pVtxBuffItem_medal_00->Unlock();
}

//========================================
// UninitItem_medal_00関数 - メダル[00]の終了処理 -
//========================================
void UninitItem_medal_00(void)
{
	// テクスチャの破棄
	if (g_aTextureItem_medal_00 != NULL)
	{
		g_aTextureItem_medal_00->Release();
		g_aTextureItem_medal_00 = NULL;
	}
	
	// 頂点バッファの破棄
	if (g_pVtxBuffItem_medal_00 != NULL) 
	{
		g_pVtxBuffItem_medal_00->Release();
		g_pVtxBuffItem_medal_00 = NULL;
	}
}

//========================================
// UpdateItem_medal_00関数 - メダル[00]の更新処理 -
//========================================
void UpdateItem_medal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ
	
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	Item_medal_00	*pItem				//
					= g_aItem_medal_00;	// メダル[00] の情報のポインタ

	for (int nCntItem_medal_00 = 0; nCntItem_medal_00 < ITEM_MEDAL_00_MAX; nCntItem_medal_00++, pVtx += 4, pItem++)
	{
		if (!pItem->bUse)
		{// メダル[00]が使用されている状態でない時、
			// 処理を折り返す
			continue;
		}
		else if (--pItem->nLife <= 0)
		{// 寿命を減算した結果尽きた時、
			// 使用していない状態にする
			pItem->bUse = false;

			continue;
		}
		else if (pItem->nLife <= ITEM_MEDAL_00_BRINK_START_LIFE) 
		{// 寿命が点滅を開始する寿命以下の時、
			// 点滅カウンターを加算
			pItem->nCounterBrink++;

			if (pItem->nCounterBrink >= ITEM_MEDAL_00_BRINK_TIME) 
			{// 点滅カウンターが点滅にかかる時間に達した時、
				// 点滅カウンターを初期化
				pItem->nCounterBrink = 0;

				// 点滅フラグを切り替える
				pItem->bBrink ^= 1;
			}
		}

		// 追尾処理
		TrackingItem_medal_00(pItem);

		// 機体[00] との当たり判定処理
		CollisionChr_fighter_00(pItem);

		// プラズマウォール[00] との当たり判定処理
		CollisionObj_plasma_wall_00(pItem);

		// 頂点座標 を設定
		SetVertexPosition(pVtx,
			pItem->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			true,
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&pItem->nPattern,
				0,
				(ITEM_MEDAL_00_PATTERN_MAX_X
					* ITEM_MEDAL_00_PATTERN_MAX_Y) - 1,
				&pItem->nCounterAnim,
				ITEM_MEDAL_00_ANIMATION_TIME,
				ANIM_TYPE_NORMAL),
			ITEM_MEDAL_00_PATTERN_MAX_X,
			ITEM_MEDAL_00_PATTERN_MAX_Y,
			NULL);
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffItem_medal_00->Unlock();
}

//========================================
// DrawItem_medal_00関数 - メダル[00]の描画処理 -
//========================================
void DrawItem_medal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem_medal_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureItem_medal_00);

	for (int nCntItem = 0; nCntItem < ITEM_MEDAL_00_MAX; nCntItem++)
	{
		if ((g_aItem_medal_00[nCntItem].bUse)
			&& (!g_aItem_medal_00[nCntItem].bBrink))
		{// メダル[00]が使用されている時、もしくは点滅フラグが偽の時、
			// メダル[00]の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffItem_medal_00->Unlock();
}

//========================================
// SetItem_medal_00関数 - メダル[00]の設定処理 -
//========================================
void SetItem_medal_00(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	Item_medal_00	*pItem				//
					= g_aItem_medal_00;	// メダル[00] の情報のポインタ

	for (int nCntItem = 0; nCntItem < ITEM_MEDAL_00_MAX; nCntItem++, pVtx += 4, pItem++)
	{
		if (pItem->bUse)
		{// メダル[00]が使用されている時
			// 繰り返し処理を折り返す
			continue;
		}

		// パラメーターの初期化処理
		InitParameterItem_medal_00(pItem);

		// 位置/角度 を代入
		pItem->pos = pos;

		// 頂点座標 を設定
		SetVertexPosition(pVtx,
			pItem->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			true,
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			ANGLE_TYPE_FIXED);

		// 寿命を設定
		pItem->nLife = ITEM_MEDAL_00_LIFE;

		// 使用している状態にする
		pItem->bUse = true;

		break;
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffItem_medal_00->Unlock();
}