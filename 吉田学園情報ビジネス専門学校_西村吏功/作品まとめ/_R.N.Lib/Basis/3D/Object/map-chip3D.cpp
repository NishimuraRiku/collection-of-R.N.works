#if 0
//========================================
// 
// マップチップ(3D)の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** map-chip3D.cpp ***
//========================================
// R.N.Lib
#include "../../../R.N.Lib.h"

//****************************************
// マクロ定義
//****************************************
// マップチップ(3D)リストの相対パス
#define MAPCHIP3DLIST_PATH "data\\GAMEOBJECT\\3D\\MAPCHIP3D_LIST.txt"
// マップチップ(3D)の注視点の相対位置の移動倍率
#define MAPCHIP3D_RELATIVE_POS_R_MOVE_DIAMETER (0.25f)

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// マップチップ(3D)のパラメーターの初期値を返す
void InitParameterMapChip3D(MapChip3D *pMapChip);
//========== *** 入出力 ***
// マップチップ(3D)の読み込み処理
void LoadMapChip3D(char *pPath, int nType);

//****************************************
// グローバル変数宣言
//****************************************
// マップチップ(3D)の情報
MapChip3D g_aMapChip3D[MAPCHIP3D_MAX];
// マップチップ(3D)の管理情報
MapChip3DControl g_mapChip3DControl;
// マップチップ(3D)の種類毎の情報
MapChip3DType g_aMapChip3DType[MAPCHIP3D_TYPE_MAX][MAPCHIP3D_SUBTYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// InitParameterMapChip3D関数 - マップチップ(3D)のパラメーターの初期化 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMapChip3D(MapChip3D *pMapChip)
{
	pMapChip->nType = 0;	// 種類
	pMapChip->bUse = false;	// 使用されているかフラグ

	// マップチップ(3D)の種類毎の情報のポインタ
	MapChip3DType *pType = &g_aMapChip3DType[pMapChip->nType][pMapChip->nSubType];
	
	// 部品管理の初期化処理
	//InitParts3DInfo(&pMapChip->partsInfo, pType->nSetUp);
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// LoadMapChip3D関数 - マップチップ(3D)の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMapChip3D(char *pPath,int nType)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX] = "";	// データ検索用
	// マップチップ(3D)の種類毎の情報のポインタ
	MapChip3DType *pMapChipType = g_aMapChip3DType[nType];
	// マップチップ(3D)の管理情報のポインタ
	MapChip3DControl *pMapChipCtl = &g_mapChip3DControl;

	// マップチップ(3D)の種類の中の種類数を初期化
	pMapChipCtl->aSubTypeNum[nType] = 0;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(pPath, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))
		{// ENDだった時、読み込みを終了
			fclose(pFile);	// データファイルを閉じる
			break; 
		}

		if (!strcmp(aDataSearch, "TYPE"))
		{
			// マップチップ(3D)の種類毎の情報の読み込みループ
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "TYPE_END"))
				{// 読み込みを終了
					pMapChipType++;	// 種類のポインタを進める

					// 種類の中の種類数を加算
					pMapChipCtl->aSubTypeNum[nType]++;
					break;
				}
				else if (!strcmp(aDataSearch, "MODEL_SETUP:"))
				{// モデルのセットアップ(3D)番号
					fscanf(pFile, "%d", &pMapChipType->nSetUp);
				}
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// InitMapChip3D関数 - マップチップ(3D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitMapChip3D(void)
{
	// クリア処理
	ClearMapChip3D();
}

//========================================
// UninitMapChip3D関数 - マップチップ(3D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitMapChip3D(void)
{

}

//========================================
// UpdateMapChip3D関数 - マップチップ(3D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMapChip3D(void)
{
	// マップチップ(3D)の情報のポインタ
	MapChip3D *pMapChip = g_aMapChip3D;

	for (int nCntMapChip = 0; nCntMapChip < MAPCHIP3D_MAX; nCntMapChip++, pMapChip++)
	{
		if (!pMapChip->bUse) 
		{// 使用されていない状態の時、
			continue;	// 繰り返し処理を折り返す
		}

		// 部品(3D)の更新処理
		//UpdateParts3DInfo(&pMapChip->partsInfo);
	}
}

//========================================
// DrawMapChip3D関数 - マップチップ(3D)の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawMapChip3D(void)
{
	
}

//============================================================
//--------------------| *** 描画 *** |------------------------
//============================================================
//========================================
// RenderMapChip3D関数 - マップチップ(3D)の表示処理 -
// Author:RIKU NISHIMURA
//========================================
void RenderMapChip3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType,int nSubType, Color col)
{
	//// 部品(3D)管理情報
	//Parts3DInfo parts3DInfo;
	//InitParts3DInfo(&parts3DInfo, g_aMapChip3DType[nType][nSubType].nSetUp);
	///* 位置 */parts3DInfo.pos = pos;
	///* 向き */parts3DInfo.rot = rot;
	///* 停止 */parts3DInfo.bStop = true;
	///* 色   */parts3DInfo.col = col;
	//// 部品(3D)の更新処理
	//UpdateParts3DInfo(&parts3DInfo);
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================
//========================================
// LoadMapChip3DList関数 - マップチップ(3D)リストの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMapChip3DList(void)
{
	return;

	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX] = "";	// データ検索用
	// マップチップ(3D)の管理情報のポインタ
	MapChip3DControl *pMapChipCtl = &g_mapChip3DControl;
	
	// 種類毎の情報のデータファイルを開く
	pFile = fopen(MAPCHIP3DLIST_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		return;	//処理を終了する
	}

	// マップチップ(3D)の種類カウント
	int nCntType = 0;

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))
		{// ENDだった時、読み込みを終了
			fclose(pFile);	// データファイルを閉じる

			// 余白のパスをNULL指定する
			for (; nCntType < MAPCHIP3D_TYPE_MAX; nCntType++) 
			{
				sprintf(pMapChipCtl->aMapChipPath[nCntType], "NULL");
			}

			break;
		}
		else if (!strcmp(aDataSearch, "MAPCHIP3D"))
		{// ENDで無かった時、
			fscanf(pFile, "%s", aDataSearch);	// 検索

			// 読み込んだ文字列をパスとして格納
			sprintf(pMapChipCtl->aMapChipPath[nCntType], aDataSearch);

			// カウントを加算
			nCntType++;
		}
	}

	// 読み込んだパスを元にマップチップ(3D)を読み込む
	for (nCntType = 0; nCntType < MAPCHIP3D_TYPE_MAX; nCntType++)
	{
		if (strcmp(pMapChipCtl->aMapChipPath[nCntType], "NULL"))
		{// 現カウントのパスがNULLでない時、マップチップ(3D)の読み込み処理
			LoadMapChip3D(pMapChipCtl->aMapChipPath[nCntType], nCntType);
		}
	}
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// GetMapChip3D関数 - マップチップ(3D)の情報を取得 -
// Author:RIKU NISHIMURA
//========================================
MapChip3D *GetMapChip3D(void)
{
	return g_aMapChip3D;
}

//========================================
// GetMapChip3DControl関数 - マップチップ(3D)の管理情報を取得 -
// Author:RIKU NISHIMURA
//========================================
MapChip3DControl *GetMapChip3DControl(void)
{
	return &g_mapChip3DControl;
}

//========================================
// GetMapChip3DType関数 - マップチップ(3D)の種類毎の情報を取得 -
// Author:RIKU NISHIMURA
//========================================
MapChip3DType *GetMapChip3DType(void)
{
	return &g_aMapChip3DType[0][0];
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================
//========================================
// SetMapChip3D関数 - マップチップ(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetMapChip3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nSubType)
{
	return;

	// マップチップ(3D)の情報のポインタ
	MapChip3D *pMapChip = g_aMapChip3D;

	for (int nCntMapChip = 0; nCntMapChip < MAPCHIP3D_MAX; nCntMapChip++, pMapChip++)
	{
		if (pMapChip->bUse)
		{// 使用されている時、処理を折り返す
			continue;
		}

		// マップチップ(3D)の種類毎の情報のポインタ
		MapChip3DType *pType = &g_aMapChip3DType[nType][nSubType];

		//// 部品管理の初期化処理
		//InitParts3DInfo(&pMapChip->partsInfo, pType->nSetUp);

		//pMapChip->partsInfo.pos = pos;	// 位置代入
		//pMapChip->partsInfo.rot = rot;	// 向き代入
		pMapChip->bUse = true;	// 使用されている状態にする

		break;
	}
}

//============================================================
//--------------------| *** 状態 *** |------------------------
//============================================================
//========================================
// ClearMapChip3D関数 - マップチップ(3D)のクリア処理 -
// Author:RIKU NISHIMURA
//========================================
void ClearMapChip3D(void) 
{
	return;

	// マップチップ(3D)の情報のポインタ
	MapChip3D *pMapChip = g_aMapChip3D;

	for (int nCntMapChip = 0; nCntMapChip < MAPCHIP3D_MAX; nCntMapChip++, pMapChip++)
	{
		// パラメーターの初期化処理
		InitParameterMapChip3D(pMapChip);
	}
}
#endif