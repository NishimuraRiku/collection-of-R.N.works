//========================================
// 
// ITM:コイン[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** itm_coin_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "chr_player_00.h"		// CHR:プレイヤー	[00]
#include "itm_coin_00.h"		// ITM:コイン		[00]
#include "obj_block_00.h"		// OBJ:ブロック		[00]
#include "obj_pedestal_00.h"	// OBJ:台座			[00]
#include "obj_stage_00.h"		// OBJ:ステージ		[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// ITM:コイン[00] の種類毎の情報のデータファイルの相対パス
#define ITM_COIN_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ITEM\\ITM_COIN_00_TYPE_DATA.txt"

// ITM:コイン[00] の最大数
// ITM:コイン[00] の種類数
#define ITM_COIN_00_MAX			(256)
#define ITM_COIN_00_TYPE_NUM	(1)

// ITM:コイン[00] の法線ベクトル
#define ITM_COIN_00_NOR	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

// ITM:コイン[00] の移動力の下限
// ITM:コイン[00] の移動力の上限
#define ITM_COIN_00_MOVE_FORCE_MIN	(1.0f)
#define ITM_COIN_00_MOVE_FORCE_MAX	(2.0f)

// ITM:コイン[00] の移動力の減衰倍率
#define ITM_COIN_00_MOVE_FORCE_DAMP	(0.9f)

// ITM:コイン[00] の回転量
#define ITM_COIN_00_SPIN	D3DXVECTOR3(0.0f,0.1f,0.0f)

// ITM:コイン[00] の追尾距離
// ITM:コイン[00] の追尾力
#define ITM_COIN_00_HOMING_DISTANCE	(80.0f)
#define ITM_COIN_00_HOMING_POWER	(2.0f)

// ITM:コイン[00] の点滅する寿命のライン
// ITM:コイン[00] の点滅にかかる時間
#define ITM_COIN_00_BLINK_LIFE_LINE	(120)
#define ITM_COIN_00_BLINK_TIME		(2)

//****************************************
// 列挙型の定義
//****************************************
// ITM:コイン[00] の衝突オブジェクト番号
typedef enum
{
	ITM_COIN_00_HITOBJ_OBJ_BLOCK_00,	// OBJ:ブロック	[00]
	ITM_COIN_00_HITOBJ_OBJ_PEDESTAL_00,	// OBJ:台座		[00]
	ITM_COIN_00_HITOBJ_OBJ_STAGE_00,	// OBJ:ステージ	[00]
	ITM_COIN_00_HITOBJ_MAX,
}ITM_COIN_00_HITOBJ;

//****************************************
// 構造体の定義
//****************************************
// ITM:コイン[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 posOld;		// 過去の位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 dispRot;	// 表示上の向き

	// 分類関連
	int nType;	// 種類

	// 状態関連
	bool	bUse;		// 使用されているかフラグ
	int		nLife;		// 寿命
	float	fMoveForce;	// 移動力

	// 運動関連
	HitTestInfo hitTestInfo;	// 当たり判定の管理情報
	Collision	cmnCollision; 	// 共通の衝突情報
	Collision	aObjCollision	// オブジェクト毎の衝突情報
				[ITM_COIN_00_HITOBJ_MAX];
}Itm_coin_00;

// ITM:コイン[00] の種類毎の情報構造体
typedef struct
{
	int			nLife;			// 寿命
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	char		aTexturePath	// テクスチャの相対パス
				[TXT_MAX];
	SOUND_LABEL	destroySE;		// 破壊SE
	HitTestSet	hitTestSet;		// 当たり判定設定情報
}Itm_coin_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
// ITM:コイン[00] のパラメーターの初期化処理
void InitParameterItm_coin_00(Itm_coin_00 *pItm);

// ITM:コイン[00] の位置更新処理
// ITM:コイン[00] の衝突チェック処理
void UpdatePosItm_coin_00(Itm_coin_00 *pItm);
void CheckCollisionItm_coin_00(VECTOR vector, Itm_coin_00 *pItm);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureItm_coin_00	// ITM:コイン[00] のテクスチャへのポインタ
						[ITM_COIN_00_TYPE_NUM]
						= {};
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffItm_coin_00	// ITM:コイン[00] の頂点バッファへのポインタ
						= NULL;
D3DXMATRIX				g_mtxWorldItm_coin_00;	// ITM:コイン[00] のワールドマトリックス
Itm_coin_00				g_aItm_coin_00			// ITM:コイン[00] の情報
						[ITM_COIN_00_MAX];
Itm_coin_00Type			g_aItm_coin_00Type		// ITM:コイン[00] の種類毎の情報
						[ITM_COIN_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterItm_coin_00関数 - ITM:コイン[00] のパラメーターの初期化処理 -
//========================================
void InitParameterItm_coin_00(Itm_coin_00 *pItm)
{
	pItm->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の位置
	pItm->posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 過去の位置
	pItm->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pItm->dispRot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 表示上の向き
	pItm->nType			= 0;								// 種類
	pItm->bUse			= false;							// 使用されているかフラグ
	pItm->nLife			= 0;								// 寿命
	pItm->fMoveForce	= 0.0f;								// 移動力
	pItm->hitTestInfo	= {};								// 当たり判定の管理情報
	pItm->cmnCollision	= {};								// 共通の衝突情報
	for (int nCntHitObj = 0; nCntHitObj < ITM_COIN_00_HITOBJ_MAX; nCntHitObj++)
	{
		pItm->aObjCollision[nCntHitObj] = {};				// オブジェクト毎の衝突情報
	}
}

//========================================
// UpdatePosItm_coin_00関数 - ITM:コイン[00] の位置更新処理 -
//========================================
void UpdatePosItm_coin_00(Itm_coin_00 *pItm)
{
	// 共通の衝突情報を初期化
	pItm->cmnCollision = {};

	// オブジェクト毎の衝突情報を初期化
	for (int nCntHitObj = 0; nCntHitObj < ITM_COIN_00_HITOBJ_MAX; nCntHitObj++)
	{
		pItm->aObjCollision[nCntHitObj] = {};
	}

	pItm->posOld = pItm->pos;	// 過去の位置を保存

	pItm->pos.x += sinf(pItm->rot.y) * pItm->fMoveForce;	// X座標更新
	CheckCollisionItm_coin_00(VECTOR_X, pItm);				// X方向の衝突チェック処理

	pItm->pos.z += cosf(pItm->rot.y) * pItm->fMoveForce;	// Z座標更新
	CheckCollisionItm_coin_00(VECTOR_Z, pItm);				// Z方向の衝突チェック処理
}


//========================================
// CheckCollisionItm_coin_00関数 - ITM:プレイヤー[00] の衝突チェック処理 -
//========================================
void CheckCollisionItm_coin_00(VECTOR vector, Itm_coin_00 *pItm)
{
	Itm_coin_00Type	*pType	// ITM:プレイヤー[00] の種類毎の情報のポインタ
					= &g_aItm_coin_00Type[pItm->nType];

	// 衝突判定に必要な情報
	CollisionInfo	myCollInfo =
	{
		&pItm->pos,
		pItm->posOld,
		NULL,
		&pItm->rot,
		pItm->rot,
		pType->hitTestSet.aHitTest[pItm->hitTestInfo.nHitTest]
	};

	// 衝突判定
	/*/ OBJ:ブロック	[00] /*/CollisionObj_block_00	(vector, &pItm->aObjCollision[ITM_COIN_00_HITOBJ_OBJ_BLOCK_00]		, &pItm->cmnCollision, myCollInfo);
	/*/ OBJ:台座		[00] /*/CollisionObj_pedestal_00(vector, &pItm->aObjCollision[ITM_COIN_00_HITOBJ_OBJ_PEDESTAL_00]	, &pItm->cmnCollision, myCollInfo);
	/*/ OBJ:ステージ	[00] /*/CollisionObj_stage_00	(vector, &pItm->aObjCollision[ITM_COIN_00_HITOBJ_OBJ_STAGE_00]		, &pItm->cmnCollision, myCollInfo);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadItm_coin_00関数 - ITM:コイン[00] の読み込み処理 -
//========================================
void LoadItm_coin_00(void)
{
	FILE			*pFile;		// ファイルポインタ
	char			aDataSearch	// データ検索用
					[TXT_MAX];
	Itm_coin_00Type	*pItmType	// ITM:コイン[00] の種類毎の情報
					= g_aItm_coin_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(ITM_COIN_00_TYPE_DATA_FILE_PATH, "r");

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
			// ITM:コイン[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pItmType++; break; }								// 読み込みを終了
				else if (!strcmp(aDataSearch, "LIFE:"))			{ fscanf(pFile, "%d", &pItmType->nLife); }			// 寿命
				else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pItmType->fWidth); }			// 幅
				else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pItmType->fHeight); }		// 高さ
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:")) { fscanf(pFile, "%s", &pItmType->aTexturePath); }	// テクスチャの相対パス
				else if (!strcmp(aDataSearch, "DESTROY_SE:"))	{ fscanf(pFile, "%d", &pItmType->destroySE); }		// 破壊SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pItmType->hitTestSet); }	// 当たり判定設定情報
			}
		}
	}
}

//========================================
// InitItm_coin_00関数 - ITM:コイン[00] の初期化処理 -
//========================================
void InitItm_coin_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();

	// テクスチャの読み込み
	for (int nCntType = 0; nCntType < ITM_COIN_00_TYPE_NUM; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aItm_coin_00Type[nCntType].aTexturePath, &g_pTextureItm_coin_00[nCntType]);
	}
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * ITM_COIN_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItm_coin_00,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItm_coin_00->Lock(0, 0, (void**)&pVtx, 0);

	Itm_coin_00	*pItm	// ITM:コイン[00] の情報のポインタ
				= g_aItm_coin_00;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pItm++, pVtx += 4)
	{
		// パラメーターの初期化処理
		InitParameterItm_coin_00(&g_aItm_coin_00[nCntItm]);

		// 法線ベクトルの設定
		SetNormalLine3D(pVtx, ITM_COIN_00_NOR);

		// 頂点カラーの設定
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItm_coin_00->Unlock();
}

//========================================
// UninitItm_coin_00関数 - ITM:コイン[00] の終了処理 -
//========================================
void UninitItm_coin_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < ITM_COIN_00_TYPE_NUM; nCntType++)
	{
		if (g_pTextureItm_coin_00[nCntType] != NULL)
		{
			g_pTextureItm_coin_00[nCntType]->Release();
			g_pTextureItm_coin_00[nCntType] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffItm_coin_00 != NULL)
	{
		g_pVtxBuffItm_coin_00->Release();
		g_pVtxBuffItm_coin_00 = NULL;
	}
}

//========================================
// UpdateItm_coin_00関数 - ITM:コイン[00] の更新処理 -
//========================================
void UpdateItm_coin_00(void)
{
	Itm_coin_00	*pItm	// ITM:コイン[00] の情報のポインタ
				= g_aItm_coin_00;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pItm++)
	{
		if (!pItm->bUse)
		{// 使用されている状態でない時、
			// 処理を折り返す
			continue;
		}
		else if (--pItm->nLife <= 0) 
		{// 寿命を減算した結果0以下の時、
			pItm->bUse = false;	// 使用していない状態にする
			continue;			// 処理を折り返す
		}

		// 位置更新処理
		UpdatePosItm_coin_00(pItm);

		if (GetChr_player_00()->state != CHR_PLAYER_00_STATE_DOWN)
		{// プレイヤーの状態がダウンでない時、
			float	fDistance	// プレイヤーとの距離
					= FindDistanceLookDown(pItm->pos, GetChr_player_00()->pos);
			float	fAngle		// プレイヤーまでの向き
					= FindAngleLookDown(pItm->pos, GetChr_player_00()->pos);

			// 距離に応じてプレイヤーまでの向きに位置を加算
			pItm->pos.x += sinf(fAngle) * Prus(1.0f - ((float)fDistance / (float)ITM_COIN_00_HOMING_DISTANCE)) * ITM_COIN_00_HOMING_POWER;
			pItm->pos.z += cosf(fAngle) * Prus(1.0f - ((float)fDistance / (float)ITM_COIN_00_HOMING_DISTANCE)) * ITM_COIN_00_HOMING_POWER;
		}

		// 移動力を減衰
		pItm->fMoveForce *= ITM_COIN_00_MOVE_FORCE_DAMP;

		// 表示向きに回転量を加算
		pItm->dispRot += ITM_COIN_00_SPIN;

		// 表示向きを制御
		RotControl(&pItm->dispRot);
	}
}

//========================================
// DrawItm_coin_00関数 - ITM:コイン[00] の描画処理 -
//========================================
void DrawItm_coin_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// デバイス取得
						= GetDevice();
	D3DXMATRIX			mtxRot,		// 計算用マトリックス
						mtxTrans;
	D3DXMATRIX			mtxView;	// ビューマトリックス取得用
	Itm_coin_00			*pItm		// ITM:コイン[00] の情報のポインタ
						= g_aItm_coin_00;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItm_coin_00, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++,pItm++)
	{
		if ((!pItm->bUse)
			||
			(pItm->nLife <= ITM_COIN_00_BLINK_LIFE_LINE) && (pItm->nLife % (ITM_COIN_00_BLINK_TIME * 2) < ITM_COIN_00_BLINK_TIME))
		{// 使用されている状態でない時、
			// 処理を折り返す
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldItm_coin_00);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		{
			D3DXVECTOR3 setRot = pItm->dispRot;	// 設定向き
			float fAngleDifference				// 設定向きと視点から現在位置までの向きの差
				= AngleDifference(
					FindAngleLookDown(GetCamera3D()->posV, pItm->pos),
					setRot.y);
			if ((fAngleDifference >= D3DX_PI * 0.5f)
				||
				(fAngleDifference <= D3DX_PI * -0.5f))
			{// 設定向きと視点から現在位置までの向きの差が90度以上の時、
				setRot.y += D3DX_PI;	// 向きに円周率(半周分)を加算
			}

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, setRot.y, setRot.x, setRot.z);
			D3DXMatrixMultiply(&g_mtxWorldItm_coin_00, &g_mtxWorldItm_coin_00, &mtxRot);
		}
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pItm->pos.x, pItm->pos.y, pItm->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldItm_coin_00, &g_mtxWorldItm_coin_00, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItm_coin_00);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureItm_coin_00[pItm->nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItm, 2);
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//========================================
// SetItm_coin_00関数 - ITM:コイン[00] の設定処理 -
//========================================
void SetItm_coin_00(D3DXVECTOR3 pos, int nSetNum)
{
	if (nSetNum <= 0) 
	{// 設定数が0以下の時、
		return;	// 処理を終了する
	}

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItm_coin_00->Lock(0, 0, (void**)&pVtx, 0);

	Itm_coin_00	*pItm	// ITM:コイン[00] の情報のポインタ
				= g_aItm_coin_00;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pVtx += 4, pItm++)
	{
		if (pItm->bUse)
		{// 使用されている状態の時、
			// 処理を折り返す
			continue;
		}

		Itm_coin_00Type *pType	// ITM:コイン[00] の種類毎の情報
						= &g_aItm_coin_00Type[pItm->nType];

		pItm->pos = pos;	// 位置を代入
		pItm->rot.y =		// 向きを乱数で設定
			-D3DX_PI + fRand(D3DX_PI * 2);
		pItm->dispRot.y =	// 表示上の向きを乱数で設定
			-D3DX_PI + fRand(D3DX_PI * 2);
		pItm->bUse = true;	// 使用されている状態にする
		pItm->fMoveForce =	// 移動力を乱数で設定
			ITM_COIN_00_MOVE_FORCE_MIN + 
			fRand(ITM_COIN_00_MOVE_FORCE_MAX - ITM_COIN_00_MOVE_FORCE_MIN);
		pItm->nLife = pType->nLife;	// 寿命を設定する

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f), pType->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (pType->fWidth * 0.5f), pType->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f), 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (pType->fWidth * 0.5f), 0.0f, 0.0f);

		if (--nSetNum <= 0) 
		{// 設定数を減算した結果0以下の時、
			break;	// 繰り返し処理を抜ける
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItm_coin_00->Unlock();
}

//========================================
// CollisionItm_coin_00関数 - ITM:コイン[00] との当たり判定処理 -
//========================================
int CollisionItm_coin_00(VECTOR vector, Collision *pItmCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool		bGhostTemp	// 当たりフラグを保存
				= pItmCollision->bGhost;
	Itm_coin_00	*pItm		// ITM:コイン[00] の情報のポインタ
				= g_aItm_coin_00;
	int			nIndex		// CHR:敵[00] の返しの番号
				= -1;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pItm++)
	{
		if (!pItm->bUse)
		{// 使用されていない状態の時、
			// 処理を折り返す
			continue;
		}

		// 衝突判定に必要な情報
		CollisionCheck(
			vector,
			pItmCollision,
			myCollInfo,
			{
				&pItm->pos,
				pItm->pos,
				NULL,
				&pItm->rot,
				pItm->rot,
				g_aItm_coin_00Type[pItm->nType].hitTestSet.aHitTest[pItm->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		if ((pItmCollision->bGhost) && (!bGhostTemp))
		{// 当たりフラグが真の時、
			nIndex = nCntItm;	// 返しの番号を設定
			bGhostTemp = true;	// 当たりフラグの保存を真にする
		}
	}

	// 共通の衝突情報を上書き
	CollisionOverwrite(pCmnCollision, *pItmCollision);

	// 番号を返す
	return nIndex;
}

//========================================
// DestroyItm_coin_00関数 - ITM:コイン[00] の破壊処理 -
//========================================
void DestroyItm_coin_00(int nIndex) 
{
	Itm_coin_00		*pItm	// ITM:コイン[00] の情報のポインタ
					= &g_aItm_coin_00[nIndex];
	Itm_coin_00Type *pType	// ITM:コイン[00] の種類毎の情報
					= &g_aItm_coin_00Type[pItm->nType];

	pItm->bUse = false;				// 使用されていない状態にする
	PlaySound(pType->destroySE);	// 破壊SE
}