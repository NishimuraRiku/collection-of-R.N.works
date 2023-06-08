#if 0
//========================================
// 
// マップ生成(3D)の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** map-generator3D.cpp ***
//========================================
// R.N.Lib
#include "../../../R.N.Lib.h"

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** マップ管理 ***
// マップクリア処理
void MapClear(void);

//****************************************
// グローバル変数宣言
//****************************************
MapSet3D g_mapSet3D;	// マップ設定情報(3D)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** マップ管理 *** |------------------
//============================================================

//========================================
// MapClear関数 - マップクリア処理 -
// Author:RIKU NISHIMURA
//========================================
void MapClear(void) 
{
	// クリア
	ClearMapChip3D();	// マップチップ(3D)

	// フォグを使用していない状態にする
	GetFog()->bUse = false;
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
// InitMapGenerator3D関数 - マップ生成(3D)の初期化 -
// Author:RIKU NISHIMURA
//========================================
void InitMapGenerator3D(void)
{
	// マップ設定情報(3D)の初期化
	g_mapSet3D = {};
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// GetMapSet関数 - マップ設定情報(3D)のポインタ取得 -
// Author:RIKU NISHIMURA
//========================================
MapSet3D *GetMapSet(void)
{
	return &g_mapSet3D;
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// LoadMapData関数 - マップ情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMapData(char *pPath)
{
	FILE *pFile;				// ファイルのポインタ
	char aDataSearch[TXT_MAX] = "";	// 検索用文字列
	
	// ファイルを開く
	pFile = fopen(pPath, "r");

	if (pFile != NULL)
	{// ファイルが開けた場合、
		// マップをクリア
		MapClear();

		// マップ設定情報(3D)の初期化
		g_mapSet3D = {};

		while (DataSearch(pFile,aDataSearch, "END"))
		{
			if (!strcmp(aDataSearch, "MAPCHIP3DSET"))	// マップチップ(3D)設定情報読み込み
			{
				// マップチップ(3D)設定情報のポインタ
				MapChip3DSet *pMapChip3DSet = g_mapSet3D.aMapChip3DSet;

				// マップチップ(3D)設定情報
				while (DataSearch(pFile, aDataSearch, "MAPCHIP3DSET_END"))
				{
					if (!strcmp(aDataSearch, "SET")) 
					{
						while (DataSearch(pFile, aDataSearch, "SET_END"))
						{
							if (false) {}
							else if (!strcmp(aDataSearch, "POS"))	{ fscanf_D3DXVECTOR3(pFile, &pMapChip3DSet->pos); }	// 色
							else if (!strcmp(aDataSearch, "ROT"))	{ fscanf_D3DXVECTOR3(pFile, &pMapChip3DSet->rot); }	// 開始距離
							else if (!strcmp(aDataSearch, "TYPE"))	{ fscanf_int(pFile, &pMapChip3DSet->nType); }		// 終了距離
						}

						SetMapChip3D(	// マップチップ(3D)を設定
							pMapChip3DSet->pos,
							pMapChip3DSet->rot,
							pMapChip3DSet->nType,
							0);
						pMapChip3DSet++;
						g_mapSet3D.nMapChip3DNum++;
					}
				}
			}
			else if (!strcmp(aDataSearch, "FOGSET"))	// フォグ設定情報読み込み
			{
				// フォグを使用している状態にする
				GetFog()->bUse = true;

				FogSet fogSet;	// フォグの設定情報

				while (DataSearch(pFile, aDataSearch, "FODSET_END"))
				{
					if (false) {}
					else if (!strcmp(aDataSearch, "COLOR")) { fscanf_Color(pFile, &fogSet.col); }	// 色
					else if (!strcmp(aDataSearch, "START")) { fscanf_int(pFile, &fogSet.nStart); }	// 開始距離
					else if (!strcmp(aDataSearch, "END"))	{ fscanf_int(pFile, &fogSet.nEnd); }	// 終了距離
				}

				// フォグを設定
				SetFog(fogSet);

				// フォグを使用している状態にする
				GetFog()->bUse = true;

				// フォグの設定情報を取得
				g_mapSet3D.fogSet = fogSet;
			}
		}
		
		 // ファイルを閉じる
		fclose(pFile);
	}
}

//========================================
// SaveMapData関数 - マップ情報の書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void SaveMapData(char *pPath)
{
	FILE *pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen(pPath, "w");

	if (pFile != NULL)
	{// ファイルが開けた場合、
		{// マップチップ(3D)設定情報を書き込み
			MapChip3DSet *pMapChip3DSet = g_mapSet3D.aMapChip3DSet;

			fprintf(pFile, "MAPCHIP3DSET\n");
			for (int nCntMapChip3D = 0; nCntMapChip3D < g_mapSet3D.nMapChip3DNum; nCntMapChip3D++, pMapChip3DSet++)
			{
				fprintf(pFile, "	SET\n");
				fprintf(pFile, "		POS %f %f %f\n", pMapChip3DSet->pos.x, pMapChip3DSet->pos.y, pMapChip3DSet->pos.z);
				fprintf(pFile, "		ROT %f %f %f\n", pMapChip3DSet->rot.x, pMapChip3DSet->rot.y, pMapChip3DSet->rot.z);
				fprintf(pFile, "		TYPE %d\n", pMapChip3DSet->nType);
				fprintf(pFile, "	SET_END\n");
			}
			fprintf(pFile, "MAPCHIP3DSET_END\n");

			fprintf(pFile, "END\n");
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}
#endif