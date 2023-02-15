//========================================
// 
// 機体[00] の処理
// Author:西村 吏功
// 
//========================================
// *** chr_fighter_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "physics.h"
#include "sound.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "atk_explosion_01.h"
#include "chr_enemy_00.h"
#include "chr_fighter_00.h"
#include "eff_laser_pointer_00.h"
#include "eff_screen_00.h"
#include "md_game_00.h"
#include "md_title_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************

// 機体[00] の初期の爆弾所持数
// 機体[00] の出現にかかる時間
// 機体[00] の大きさに関与する出現のカウンターにかかる倍率
// 機体[00] のプレイヤーが音を拾う距離
// 機体[00] の回転量にかかる倍率
// 機体[00] の種類毎の情報のデータファイルの相対パス
// 機体[00] の無敵時間
// 機体[00] のダメージ状態の時間
// 機体[00] の被ダメージ時の振動
// 機体[00] の被ダメージ時の画面効果の色
#define CHR_FIGHTER_00_INIT_BOMB_NUM		(2)
#define CHR_FIGHTER_00_POP_TIME				(40)
#define CHR_FIGHTER_00_POP_SIZE				(0.08f)
#define CHR_FIGHTER_00_PLAYER_RANGE_SOUND	(800.0f)
#define CHR_FIGHTER_00_ROTATION_DIAMETER	(0.3f)
#define CHR_FIGHTER_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHR_FIGHTER_00_TYPE_DATA.txt"
#define CHR_FIGHTER_00_INVINCIBLE_TIME		(30)
#define CHR_FIGHTER_00_DAMAGE_TIME			(30)
#define CHR_FIGHTER_00_DAMAGE_VIBRATION		(64.0f)
#define CHR_FIGHTER_00_DAMAGE_COLOR			{167,0,0,0}

//****************************************
// 列挙型の定義
//****************************************

// 機体[00] のポリゴン番号
typedef enum 
{
	CHR_FIGHTER_00_POLYGON_NORMAL = 0,		// 通常
	CHR_FIGHTER_00_POLYGON_NUM				// ポリゴン数
}CHR_FIGHTER_00_POLYGON;

//****************************************
// プロトタイプ宣言
//****************************************

// 機体[00] のキー入力処理
void KeyInputChr_fighter_00(void);

// 機体[00] の状態処理
void StateProcessChr_fighter_00(void);

// 機体[00] のパラメーターを種類に応じて設定
void SetParameterChr_fighter_00(void);

// プラズマウォール[00] との当たり判定
void CollisionObj_plasma_wall_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_fighter_00	[CHR_FIGHTER_00_TYPE_MAX]	= {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChr_fighter_00								= NULL;	// 頂点バッファへのポインタ
Chr_fighter_00			g_chr_fighter_00;												// 機体[00] の情報
Chr_fighter_00Type		g_aChr_fighter_00Type		[CHR_FIGHTER_00_TYPE_MAX];			// 機体[00] の種類毎の情報

//========== *** 機体[00] の情報を取得 ***
Chr_fighter_00 *GetChr_fighter_00(void)
{
	return &g_chr_fighter_00;
}

//========== *** 機体[00] の種類毎の情報を取得 ***
Chr_fighter_00Type *GetChr_fighter_00Type(void)
{
	return g_aChr_fighter_00Type;
}

//========================================
// InitChr_fighter_00関数 - 機体[00] の初期化処理 -
//========================================
void InitChr_fighter_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntChr_fighter_00Type = 0; nCntChr_fighter_00Type < CHR_FIGHTER_00_TYPE_MAX; nCntChr_fighter_00Type++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aChr_fighter_00Type[nCntChr_fighter_00Type].aTexturPath, &g_aTextureChr_fighter_00[nCntChr_fighter_00Type]);
	}
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_FIGHTER_00_POLYGON_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChr_fighter_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	pChr->nCounterAnim	= 0;								// アニメーションカウンター を初期化する
	pChr->nPattern		= 0;								// パターンNo.				を初期化する
	pChr->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置						を初期化する
	pChr->move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量					を初期化する
	pChr->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き						を初期化する
	pChr->bUse			= false;							// 使用されているかフラグ	を初期化する
	pChr->nScore		= 0;								// スコア					を初期化する
	pChr->nType			= 0;								// 種類						を初期化する
	pChr->nShotCounter	= 0;								// 弾の発射カウンター		を初期化する
	pChr->nBomb			= CHR_FIGHTER_00_INIT_BOMB_NUM;		// 爆弾所持数				を初期化する
	pChr->nCntDamage	= 0;								// 被弾カウント				を初期化する

	// ステータスを種類に応じて設定
	SetParameterChr_fighter_00();

	// 状態関連の初期化
	for (int nCntState = 0; nCntState < CHR_FIGHTER_00_STATE_MAX; nCntState++)
	{
		pChr->aState[nCntState]			= false;	// 状態 を初期化する
		pChr->aCounterState[nCntState]	= 0;		// 状態管理のカウンター を初期化する
	}

	// ポリゴン毎の設定
	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++, pVtx += 4)
	{
		{
			D3DXVECTOR3 drawPos = pChr->pos;												// 描画位置
			D3DXVECTOR3 rot		= pChr->rot;												// 向き
			float fWidth		= g_aChr_fighter_00Type[pChr->nType].fWidth;				// 幅
			float fHeight		= g_aChr_fighter_00Type[pChr->nType].fHeight;				// 高さ
			float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle		= atan2f(fWidth, fHeight);									// 対角線の角度

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
			int nPtn		= pChr->nPattern;								// テクスチャのパターンNo.
			int nPtnMaxX	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxX;	// テクスチャのパターン数(横)
			int nPtnMaxY	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxY;	// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffChr_fighter_00->Unlock();
}

//========================================
// UninitChr_fighter_00関数 - 機体[00] の終了処理 -
//========================================
void UninitChr_fighter_00(void)
{
	// テクスチャの破棄
	for (int nCntChr_fighter_00 = 0; nCntChr_fighter_00 < CHR_FIGHTER_00_TYPE_MAX; nCntChr_fighter_00++)
	{
		if (g_aTextureChr_fighter_00[nCntChr_fighter_00] != NULL)
		{
			g_aTextureChr_fighter_00[nCntChr_fighter_00]->Release();
			g_aTextureChr_fighter_00[nCntChr_fighter_00] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffChr_fighter_00 != NULL) 
	{
		g_pVtxBuffChr_fighter_00->Release();
		g_pVtxBuffChr_fighter_00 = NULL;
	}
}

//========================================
// UpdateChr_fighter_00関数 - 機体[00] の更新処理 -
//========================================
void UpdateChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	// カメラの位置を機体[00] の座標とする
	GetCamera()->pos = pChr->pos;

	if (!pChr->bUse)
	{// 使用されている状態でない時、
		// 処理を終了する
		return;
	}

	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	// 状態処理
	StateProcessChr_fighter_00();

	if (pChr->nShotCounter < pChr->nShotTime)
	{// 弾の発射カウンターが弾の発射にかかる時間に達していない時、
		// 弾の発射カウンターを加算
		pChr->nShotCounter++;
	}

	// キー入力処理
	KeyInputChr_fighter_00();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// 位置/回転を更新
	pChr->pos.x += pChr->move.x;
	pChr->pos.y += pChr->move.y;
	
	// 移動量を更新(減衰)
	pChr->move.x += (0.0f - pChr->move.x) * 0.1f;
	pChr->move.y += (0.0f - pChr->move.y) * 0.1f;

	// プラズマウォール[00] との当たり判定
	CollisionObj_plasma_wall_00();

	// 角度を目標角度に向けて推移する
	pChr->rot.z += AngleDifference(pChr->rot.z, pChr->targetRot.z) * CHR_FIGHTER_00_ROTATION_DIAMETER;

	// 角度を制御する
	ControlAngle(&pChr->rot.z);

	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++, pVtx += 4)
	{
		int nPtn		= pChr->nPattern;								// パターンNo.
		int nPtnMaxX	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxX;	// テクスチャのパターン数(横)
		int nPtnMaxY	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxY;	// テクスチャのパターン数(縦)
		int nR, nG, nB;													// 色(RGB)

		{
			D3DXVECTOR3 drawPos = pChr->pos;																							// 描画位置
			float fWidth	= pChr->fWidth * (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));		// 幅
			float fHeight	= pChr->fHeight * (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));	// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;													// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);																					// 対角線の角度

			// 描画位置を設定
			MatchCameraPosition(&drawPos);

			// 頂点座標を設定
			pVtx[0].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) + fAngle) * fLength;
		}

		{
			float fRed		= 1.0f - ((float)pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE] / (float)CHR_FIGHTER_00_DAMAGE_TIME);	// 赤度
			float fAlpha	= 1.0f - ((float)pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] / (float)CHR_FIGHTER_00_POP_TIME);	// 透明度

			// 頂点カラーの設定																						   
			pVtx[0].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
			pVtx[1].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
			pVtx[2].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
			pVtx[3].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
	}

	// 頂点座標をアンロックする
	g_pVtxBuffChr_fighter_00->Unlock();
}

//========================================
// LoadChr_fighter_00関数 - 機体[00] の読み込み処理 -
//========================================
void LoadChr_fighter_00(void)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX];	// データ検索用

	Chr_fighter_00Type *pChrType
		= &g_aChr_fighter_00Type[0];	// 機体[00] の種類毎の情報

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(CHR_FIGHTER_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// 文末識別子が見つかるまで読み込みを繰り返す
	while (1)
	{
		// 検索用の文字列に現在の行の文字列を読み込む
		fscanf(pFile, "%s", aDataSearch);

		if (strcmp(aDataSearch, "END") == 0)
		{// 検索用の文字列が[文末] 識別子と一致した時、
			// ファイルを閉じる
			fclose(pFile);

			break;
		}

		if (aDataSearch[0] == '#')
		{// 検索用の文字列の最初の文字が[コメント] 識別子と一致した時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (strcmp(aDataSearch, "FIGHTER_TYPE") == 0)
		{// 検索用の文字列が[機体の種類] 識別子と一致した時、
			// 機体の種類毎の情報を読み込む
			while (1)
			{
				// 検索用の文字列に現在の行の文字列を読み込む
				fscanf(pFile, "%s", aDataSearch);
				if (strcmp(aDataSearch, "FIGHTER_TYPE_END") == 0)
				{// 検索用の文字列が[文末] 識別子と一致した時、
					pChrType++;
					break;
				}
				else if (strcmp(aDataSearch, "名称:") == 0)
				{// 検索用の文字列が[名称] 識別子と一致した時、
					// 名称を読み込む
					fscanf(pFile, "%s", pChrType->aName);
				}
				else if (strcmp(aDataSearch, "テクスチャの相対パス:") == 0)
				{// 検索用の文字列が[テクスチャの相対パス] 識別子と一致した時、
					// テクスチャの相対パスを読み込む
					fscanf(pFile, "%s", pChrType->aTexturPath);
				}
				else if (strcmp(aDataSearch, "パターンの上限X:") == 0)
				{// 検索用の文字列が[パターン上限X] 識別子と一致した時、
					// パターン上限Xを読み込む
					fscanf(pFile, "%d", &pChrType->nPtnMaxX);
				}
				else if (strcmp(aDataSearch, "パターンの上限Y:") == 0)
				{// 検索用の文字列が[パターン上限Y] 識別子と一致した時、
					// パターン上限Yを読み込む
					fscanf(pFile, "%d", &pChrType->nPtnMaxY);
				}
				else if (strcmp(aDataSearch, "変色部の段:") == 0)
				{// 検索用の文字列が[変色部の段] 識別子と一致した時、
					// 変色部の段を読み込む
					fscanf(pFile, "%d", &pChrType->nFillStep);
				}
				else if (strcmp(aDataSearch, "幅:") == 0)
				{// 検索用の文字列が[幅] 識別子と一致した時、
					// 幅を読み込む
					fscanf(pFile, "%f", &pChrType->fWidth);

					// 幅をピクセル単位で修正
					pChrType->fWidth *= PIXEL;
				}
				else if (strcmp(aDataSearch, "高さ:") == 0)
				{// 検索用の文字列が[高さ] 識別子と一致した時、
					// 高さを読み込む
					fscanf(pFile, "%f", &pChrType->fHeight);

					// 高さをピクセル単位で修正
					pChrType->fHeight *= PIXEL;
				}
				else if (strcmp(aDataSearch, "当たり判定の幅:") == 0)
				{// 当たり判定の幅を読み込む
					fscanf(pFile, "%f", &pChrType->fHitTestWidth);

					// 当たり判定をピクセル単位で修正
					pChrType->fHitTestWidth *= PIXEL;
				}
				else if (strcmp(aDataSearch, "当たり判定の高さ:") == 0)
				{// 当たり判定の幅を読み込む
					fscanf(pFile, "%f", &pChrType->fHitTestHeight);

					// 当たり判定をピクセル単位で修正
					pChrType->fHitTestHeight *= PIXEL;
				}
				else if (strcmp(aDataSearch, "当たり判定の種類:") == 0)
				{// 当たり判定の種類を読み込む
					fscanf(pFile, "%s", aDataSearch); // 検索

					// 当たり判定の種類を文字列から読み込む
					StringLoadHitTestType(aDataSearch, &pChrType->hitTestType);
				}
				else if (strcmp(aDataSearch, "体力:") == 0)
				{// 検索用の文字列が[体力] 識別子と一致した時、
					// 体力を読み込む
					fscanf(pFile, "%d", &pChrType->nLife);
				}
				else if (strcmp(aDataSearch, "移動力:") == 0)
				{// 検索用の文字列が[移動力] 識別子と一致した時、
					// 移動力を読み込む
					fscanf(pFile, "%f", &pChrType->fMove);
				}
				else if (strcmp(aDataSearch, "弾の発射にかかる時間:") == 0)
				{// 検索用の文字列が[弾の発射にかかる時間] 識別子と一致した時、
					// 弾の発射にかかる時間を読み込む
					fscanf(pFile, "%d", &pChrType->nShotTime);
				}
			}
		}			
	}
}

//========================================
// StateProcess関数 - 機体[00] の状態処理 -
//========================================
void StateProcessChr_fighter_00(void) 
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	// 状態処理
	for (int nCntState = 0; nCntState < CHR_FIGHTER_00_STATE_MAX; nCntState++)
	{
		if (pChr->aState[nCntState])
		{// その状態が真の時

			switch (nCntState)
			{
			case CHR_FIGHTER_00_STATE_IN_POP: // *** 出現中 ***

				if (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] > 0)
				{// 状態管理カウンター[出現中]が0を上回っている時

				 // 状態管理カウンター[出現中]を減算
					pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP]--;
				}
				else
				{// 状態管理カウンター[出現中]が0以下の時

				 // 状態[出現中]を偽にする
					pChr->aState[CHR_FIGHTER_00_STATE_IN_POP] = false;
				}

				break;
			case CHR_FIGHTER_00_STATE_DAMAGE: // *** ダメージ ***

				if (pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE] > 0)
				{// 状態管理カウンター[ダメージ]が0を上回っている時

					// 状態管理カウンター[ダメージ]を減算
					pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE]--;
				}
				else
				{// 状態管理カウンター[ダメージ]が0以下の時

					// 状態[ダメージ]を偽にする
					pChr->aState[CHR_FIGHTER_00_STATE_DAMAGE] = false;
				}

				break;
			case CHR_FIGHTER_00_STATE_INVINCIBLE: // *** 無敵 ***

				if (pChr->aCounterState[CHR_FIGHTER_00_STATE_INVINCIBLE] > 0)
				{// 状態管理カウンター[無敵]が0を上回っている時

					// 状態管理カウンター[無敵]を減算
					pChr->aCounterState[CHR_FIGHTER_00_STATE_INVINCIBLE]--;
				}
				else
				{// 状態管理カウンター[無敵]が0以下の時

					// 状態[無敵]を偽にする
					pChr->aState[CHR_FIGHTER_00_STATE_INVINCIBLE] = false;
				}

				break;
			}
		}
	}
}

//========================================
// KeyInput関数 - 機体[00] のキー入力処理 -
//========================================
void KeyInputChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	if ((pChr->aState[CHR_FIGHTER_00_STATE_IN_POP])
		||
		(GetGameState() == MD_GAME_00_STATE_RESULT)
		||
		(GetGameState() == MD_GAME_00_STATE_GAME_CLEAR)
		||
		(GetGameState() == MD_GAME_00_STATE_GAME_OVER))
	{// 出現中、もしくはリザルト/ゲームクリア/ゲームオーバーの時、
		// 処理を抜ける
		return;
	}

	bool	bShotInput	//
			= false;	// ショットの入力フラグ

	if ((GetMousePress(MOUSE_LEFT))
		||
		(GetStick().aTplDiameter[STICK_TYPE_RIGHT] > 0))
	{// ショットの入力がされている時、
		// ショットの入力フラグを真にする
		bShotInput = true;

		if (GetMousePress(MOUSE_LEFT))
		{// マウスの左が押されている時、
			D3DXVECTOR3 drawPos			//
						= pChr->pos;	// 描画位置

			// 描画位置を設定
			MatchCameraPosition(&drawPos);

			// 目標角度を描画位置からカーソルまでの角度に設定
			pChr->targetRot.z = FindAngle(drawPos, GetCursorPosOnScreen());
		}
		else if (GetStick().aTplDiameter[STICK_TYPE_RIGHT] > 0)
		{// 右スティックが入力されている時、
			// 機体[00] の目標角度を設定
			pChr->targetRot.z = GetStick().aAngle[STICK_TYPE_RIGHT];
		}

		// レーザーポインタ―[00] の状態を消え中にする
		SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE_IN_CLEAR);

		if (pChr->nShotCounter >= pChr->nShotTime)
		{// 弾の発射カウンターが弾の発射にかかる時間に達した時、
			// 弾の発射カウンターを初期化
			pChr->nShotCounter = 0;

			// 弾の設定処理
			SetAtk_bullet_00(
				D3DXVECTOR3((sinf(pChr->targetRot.z) * pChr->fHitTestWidth) + pChr->pos.x,
				(cosf(pChr->targetRot.z) * pChr->fHitTestWidth) + pChr->pos.y, 0.0f),
				pChr->targetRot,
				0,
				ATK_PARENT_TYPE_PLAYER);
		}
	}

	{
		D3DXVECTOR3 moveRot;	// 移動角度
		bool		bMoveInput	//
					= true;		// 移動の入力フラグ

		if (GetKeyboardPress(DIK_W))
		{// 上キーが押されている時、
			
			if (GetKeyboardPress(DIK_A))
			{// 左キーが押されている時、
				// 機体[00] の移動角度を設定
				moveRot.z = D3DX_PI * -0.75f;
			}
			else if (GetKeyboardPress(DIK_D))
			{// 右キーが押されている時、
				// 機体[00] の移動角度を設定
				moveRot.z = D3DX_PI * 0.75f;
			}
			else
			{// 上キーのみ押されている時
				// 機体[00] の移動角度を設定
				moveRot.z = D3DX_PI * 1.0f;
			}

			// 移動量を更新
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetKeyboardPress(DIK_S))
		{// 下キーが押されている時、
			if (GetKeyboardPress(DIK_A))
			{// 左キーが押されている時、
				// 機体[00] の移動角度を設定
				moveRot.z = D3DX_PI * -0.25f;
			}
			else if (GetKeyboardPress(DIK_D))
			{// 右キーが押されている時、
				// 機体[00] の移動角度を設定
				moveRot.z = D3DX_PI * 0.25f;
			}
			else
			{// 下キーのみ押されている時、
				// 機体[00] の移動角度を設定
				moveRot.z = D3DX_PI * 0.0f;
			}

			// 移動量を更新
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetKeyboardPress(DIK_A))
		{// 左キーが押されている時、
			// 機体[00] の移動角度を設定
			moveRot.z = D3DX_PI * -0.5f;

			// 移動量を更新
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetKeyboardPress(DIK_D))
		{// 右キーが押されている時、
			// 機体[00] の移動角度を設定
			moveRot.z = D3DX_PI * 0.5f;

			// 移動量を更新
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetStick().aTplDiameter[STICK_TYPE_LEFT] > 0)
		{// 左スティックが入力されている時、
			// 機体[00] の移動角度を設定
			moveRot.z = GetStick().aAngle[STICK_TYPE_LEFT];

			// 移動量を更新
			pChr->move.x += sinf(moveRot.z) * pChr->fMove * GetStick().aTplDiameter[STICK_TYPE_LEFT];
			pChr->move.y += cosf(moveRot.z) * pChr->fMove * GetStick().aTplDiameter[STICK_TYPE_LEFT];
		}
		else
		{// どの方向キーも押されていない時、
			// 移動の入力フラグを偽にする
			bMoveInput = false;

			// 移動角度を現在の角度に設定
			moveRot.z = pChr->rot.z;
		}

		if ((!bShotInput)
			&&
			(bMoveInput))
		{// ショットの入力フラグが偽、かつ移動の入力フラグが真の時、
			// 目標向きの設定
			pChr->targetRot = moveRot;
		}
	}

	if (GetMouseTrigger(MOUSE_RIGHT)
		|| GetButtonTrigger(BUTTON_RIGHT_SHOULDER)
		|| GetButtonTrigger(BUTTON_RIGHT_TRIGGER))
	{// 爆弾が入力された時、
		if ((pChr->nBomb > 0)
			&& GetGameState() == MD_GAME_00_STATE_NORMAL)
		{// 爆弾を所持していて、かつゲームの状態が通常の時、
			// 爆弾の所持数を減算
			pChr->nBomb--;

			// 爆発[01] を設定
			SetAtk_explosion_01(pChr->pos, pChr->rot, ATK_EXPLOSION_01_TYPE_00);
		}
		else 
		{// 爆弾を所持していなかった時、

		}
	}
}

//========================================
// DrawChr_fighter_00関数 - 機体[00] の描画処理 -
//========================================
void DrawChr_fighter_00(void)
{
	if (!g_chr_fighter_00.bUse)
	{// 使用されていない時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffChr_fighter_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureChr_fighter_00[g_chr_fighter_00.nType]);

	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++)
	{
		// 機体[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
	}
}

//========================================
// SetChr_fighter_00関数 - 機体[00] の設定処理 -
//========================================
void SetChr_fighter_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_chr_fighter_00.bUse)
	{// 使用されている状態の時、
		// 処理を終了する
		return;
	}

	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// ステータスを種類に応じて設定
	SetParameterChr_fighter_00();

	// 位置/角度を代入
	pChr->pos		= pos;
	pChr->rot		=
	pChr->targetRot = rot;

	// 出現中の状態にする
	pChr->aState		[CHR_FIGHTER_00_STATE_IN_POP]	= true;
	pChr->aCounterState	[CHR_FIGHTER_00_STATE_IN_POP]	= CHR_FIGHTER_00_POP_TIME;

	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++, pVtx += 4)
	{
		D3DXVECTOR3 drawPos = pChr->pos;												// 描画位置
		D3DXVECTOR3 rot		= pChr->rot;												// 向き
		float fWidth		= pChr->fWidth;												// 幅
		float fHeight		= pChr->fHeight;											// 高さ
		float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
		float fAngle		= atan2f(fWidth, fHeight);									// 対角線の角度

		// 描画位置を設定
		MatchCameraPosition(&drawPos);

		// 出現中のカウンターに応じて拡大
		fWidth	*= (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));
		fHeight *= (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));

		// 頂点座標を設定
		pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
		pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;

		{
			int nRed	= pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE];	// 赤度
			int nAlpha	= pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP];	// 透明度

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
			pVtx[1].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
			pVtx[2].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
			pVtx[3].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
		}
	}

	// 移動量を初期化
	pChr->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 使用している状態にする
	pChr->bUse = true;

	// 頂点座標をアンロックする
	g_pVtxBuffChr_fighter_00->Unlock();
}

//========================================
// BreakChr_fighter_00関数 - 機体[00] の破壊処理 -
//========================================
void BreakChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	// ゲームの状態を死亡後の余韻にする
	SetMd_game_00State(MD_GAME_00_STATE_DIED_AFTERGLOW);

	// 機体[00]を使用していない状態にする
	pChr->bUse = false;

	// 爆発[00] の設定処理
	SetAtk_explosion_00(pChr->pos, pChr->rot, ATK_EXPLOSION_00_TYPE_06,0);

	// 全ての敵[00] を消し去る
	RemoveAllChr_enemy_00();

	// 弾[00] の全破壊処理
	AllDestructionAtk_bullet_00(ATK_PARENT_TYPE_ENEMY);
	AllDestructionAtk_bullet_00(ATK_PARENT_TYPE_PLAYER);
}

//========================================
// SetParameter関数 - 機体[00] のステータスを種類に応じて設定 -
//========================================
void SetParameterChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	pChr->fWidth			= g_aChr_fighter_00Type[pChr->nType].fWidth;			// 幅
	pChr->fHeight			= g_aChr_fighter_00Type[pChr->nType].fHeight;			// 高さ
	pChr->fHitTestWidth		= g_aChr_fighter_00Type[pChr->nType].fHitTestWidth;		// 当たり判定の幅
	pChr->fHitTestHeight	= g_aChr_fighter_00Type[pChr->nType].fHitTestHeight;	// 当たり判定の高さ
	pChr->fMove				= g_aChr_fighter_00Type[pChr->nType].fMove;				// 移動力
	pChr->nLife				= g_aChr_fighter_00Type[pChr->nType].nLife;				// 体力
	pChr->nShotTime			= g_aChr_fighter_00Type[pChr->nType].nShotTime;			// 弾の発射にかかる時間
}

//========================================
// HitChr_fighter_00関数 - 機体[00] のヒット処理 -
//========================================
void HitChr_fighter_00(int nDamage)
{
	if (nDamage <= 0) 
	{// ダメージが0以下の時、
		// 処理を終了
		return;
	}

	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	// 体力からダメージを減算
	pChr->nLife -= nDamage;

	// 被弾カウントを加算
	pChr->nCntDamage++;

	// カメラの振動を設定
	SetCameraVibration(CHR_FIGHTER_00_DAMAGE_VIBRATION);

	// スクリーン[00] の設定処理
	SetEff_screen_00(CHR_FIGHTER_00_DAMAGE_TIME, CHR_FIGHTER_00_DAMAGE_COLOR);

	if (pChr->nLife <= 0)
	{// 体力が0以下の時、
		// 体力を0にする
		pChr->nLife = 0;

		// 機体[00] の破壊処理
		BreakChr_fighter_00();
	}
	else
	{// 体力が0を上回っている時、
		// ダメージ状態にする
		pChr->aState[CHR_FIGHTER_00_STATE_DAMAGE] = true;
		pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE] = CHR_FIGHTER_00_DAMAGE_TIME;

		// 無敵状態にする
		pChr->aState[CHR_FIGHTER_00_STATE_INVINCIBLE] = true;
		pChr->aCounterState[CHR_FIGHTER_00_STATE_INVINCIBLE] = CHR_FIGHTER_00_INVINCIBLE_TIME;

		// ダメージSE[000] を鳴らす
		PlaySound(SOUND_LABEL_SE_DAMAGE_000);
	}
}

//========================================
// CollisionObj_plasma_wall_00関数 - プラズマウォール[00]との当たり判定処理 -
//========================================
void CollisionObj_plasma_wall_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// 機体[00] の情報のポインタ

	if (!pChr->bUse)
	{// 機体[00] が使用されていない時、
		// 処理を終了する
		return;
	}

	Obj_plasma_wall_00	*pObj = GetObj_plasma_wall_00();					// プラズマウォール[00] の情報
	Obj_plasma_block_00_square	*pSquare = GetObj_plasma_block_00_square();	// プラズマブロック[00] の描く四角形の情報

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

		if ((nCntObj == OBJ_PLASMA_WALL_00_TOP) && (pChr->pos.y - pChr->fHitTestWidth < pSquare->fTop + (fHeight * 0.5f)))
		{// 上のプラズマウォールを越えていた時、
			// 位置を修正
			pChr->pos.y = pSquare->fTop + (fHeight * 0.5f) + pChr->fHitTestWidth;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_BOTTOM) && (pChr->pos.y + pChr->fHitTestWidth > pSquare->fBottom - (fHeight * 0.5f)))
		{// 下のプラズマウォールを越えていた時、
			// 位置を修正
			pChr->pos.y = pSquare->fBottom - (fHeight * 0.5f) - pChr->fHitTestWidth;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_LEFT) && (pChr->pos.x - pChr->fHitTestWidth < pSquare->fLeft + (fWidth * 0.5f)))
		{// 左のプラズマウォールを越えていた時、
			// 位置を修正
			pChr->pos.x = pSquare->fLeft + (fWidth * 0.5f) + pChr->fHitTestWidth;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_RIGHT) && (pChr->pos.x + pChr->fHitTestWidth > pObj->pos.x - (fWidth * 0.5f)))
		{// 右のプラズマウォールを越えていた時、
			// 位置を修正
			pChr->pos.x = pObj->pos.x - (fWidth * 0.5f) - pChr->fHitTestWidth;
		}
	}
}