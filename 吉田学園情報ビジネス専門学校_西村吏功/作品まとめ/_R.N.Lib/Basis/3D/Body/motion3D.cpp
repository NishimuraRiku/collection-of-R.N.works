//========================================
// 
// モーション(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** motion3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//****************************************
// 構造体定義
//****************************************
// モーション(3D)の管理情報
typedef struct {
	char **ppLoadMotionPath = NULL;	// 読み込み済のモーション(3D)パス
	int nMotionNum = 0;	// モーション(3D)の数
}Motion3DInfo;

//****************************************
// グローバル変数宣言
//****************************************
// モーション(3D)の管理情報
static Motion3DInfo g_motion3DInfo;
// モーション情報
static Motion3D *g_pMotion3D;

//****************************************
// グローバル定数宣言
//****************************************
// モーションコマンドのラベル毎の情報
const Motion3DCommandInfo g_aMotionCommandInfo[MOTION3D_COMMAND_LABEL_MAX] = {
	{ "MOVE",5 },	// 移動
	{ "SPIN",5 },	// 回転
	{ "STEP",3 },	// 踏む
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** モーション(3D)関連 *** |----------
//============================================================

//========================================
// LoadMotion3D関数 - モーション(3D)の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
int LoadMotion3D(Motion3DLoad motionLoad) {
	char *pMotionPath = motionLoad.pMotionPath;	// モーションのパス
	int nMotionIdx = g_motion3DInfo.nMotionNum;	// モーション番号
	bool bLoadDone = false;	// 読み込み済フラグ

	if (motionLoad.nMotionIdx != -1)
	{// 読み込むモーション番号が指定されている時、
		nMotionIdx = motionLoad.nMotionIdx;	// モーション番号を設定
		bLoadDone = true;	// 読み込み済
	}

	// 既に読み込まれているモーションか確認
	for (int nCntType = 0; nCntType < g_motion3DInfo.nMotionNum; nCntType++) {
		if (!strcmp(pMotionPath, g_motion3DInfo.ppLoadMotionPath[nCntType]))
		{// 既に同じパスのモーションを読み込んでいた時、
			// 読み込み済フラグを真にする
			bLoadDone = true;

			if (motionLoad.nMotionIdx != -1)
			{// 読み込むセットアップ番号が指定されている時、
				break;
			}
			else if (motionLoad.bReload)
			{// 再読み込みフラグが真の時、
				nMotionIdx = nCntType;
				break;
			}
			else
			{// 再読み込みフラグが偽の時、
				return nCntType;	// 種類番号を返す
			}
		}
	}

	if (!bLoadDone)
	{// 読み込み済みでない時、
		// モーション数を加算
		g_motion3DInfo.nMotionNum++;

		// 文字列のダブルポインタのメモリ再確保
		ReGetMemory((void**)&g_motion3DInfo.ppLoadMotionPath, sizeof(char*), g_motion3DInfo.nMotionNum - 1, g_motion3DInfo.nMotionNum);

		// 文字列のポインタのメモリ確保
		GetMemory((void**)&g_motion3DInfo.ppLoadMotionPath[g_motion3DInfo.nMotionNum - 1], sizeof(char) * TXT_MAX, 1);

		// モーション情報のメモリ再確保
		ReGetMemory((void**)g_pMotion3D, sizeof(Motion3D), g_motion3DInfo.nMotionNum - 1, g_motion3DInfo.nMotionNum);
	}

	// モーション(3D)のパスを保存
	sprintf(g_motion3DInfo.ppLoadMotionPath[nMotionIdx], "%s", pMotionPath);

	// 種類毎の情報のデータファイルを開く
	FILE *pFile = fopen(pMotionPath, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、処理を終了する
		assert(false);
		return 0;
	}

	// モーション(3D)情報のポインタ
	Motion3D *pMotion = &g_pMotion3D[nMotionIdx];
	char aDataSearch[TXT_MAX] = "";	// データ検索用文字列
	int nCntParts = 0;	// 部品のカウント

	// モーション設定情報の読み込みを開始
	while (DataSearch(pFile, aDataSearch, "END")) {
		if (false) {}
		else if (!strcmp(aDataSearch, "LOOP_TIME")) { fscanf_int(pFile, &pMotion->nLoopTime); }	// ループ時間
		else if (!strcmp(aDataSearch, "LOOP"))      { fscanf_bool(pFile, &pMotion->bLoop); }	// ループするかフラグ

		if (!strcmp(aDataSearch, "PARTSMOTION")) {
			fscanf_int(pFile, &pMotion->nPartsMotionNum);	// 部品毎のモーションの数取得

			// 部品の種類毎の情報のメモリ確保
			pMotion->pPartsMotion = NULL;
			GetMemory((void**)&pMotion->pPartsMotion, sizeof(PartsMotion3D), pMotion->nPartsMotionNum);

			// モーション情報の読み込みを開始
			while (DataSearch(pFile, aDataSearch, "PARTSMOTION_END")) {
				if (false) {}
				else if (!strcmp(aDataSearch, "PARTS")) {
					// 部品毎のモーションの情報のポインタ
					pMotion->pPartsMotion[nCntParts] = {};
					PartsMotion3D *pPartsMotion = &pMotion->pPartsMotion[nCntParts];

					fscanf_int(pFile, &pPartsMotion->nCommandNum);	// コマンド数取得

					// コマンド情報のメモリ確保
					if (pPartsMotion->nCommandNum > 0) {
						GetMemory((void**)&pPartsMotion->pCommand, sizeof(Motion3DCommand), pPartsMotion->nCommandNum);

						// コマンドのカウント
						int nCntCmd = 0;

						// 部品毎のモーションの情報の読み込みを開始
						while (DataSearch(pFile, aDataSearch, "PARTS_END")) {
							if (false) {}
							else if (!strcmp(aDataSearch, "TIME")) {
								// コマンド開始時間
								fscanf_int(pFile, &pPartsMotion->pCommand[nCntCmd].nTime);

								// コマンド番号
								int nCmd = 0;
								// コマンドラベルを読み取る
								fscanf_string(pFile, aDataSearch);

								// 総当たりでラベルの判定を行う
								for (int nCntLabel = 0; nCntLabel < MOTION3D_COMMAND_LABEL_MAX; nCntLabel++) {
									if (!strcmp(aDataSearch, g_aMotionCommandInfo[nCntLabel].pLabelName))
									{// 文字列がラベル名と一致している時、
										// ラベルを設定
										pPartsMotion->pCommand[nCntCmd].command = (MOTION3D_COMMAND_LABEL)nCntLabel;
										// コマンド番号を指定
										nCmd = nCntLabel;
										break;
									}
								}

								// 引数情報のメモリを確保
								pPartsMotion->pCommand[nCntCmd].pData = NULL;
								GetMemory((void**)&pPartsMotion->pCommand[nCntCmd].pData, sizeof(float), g_aMotionCommandInfo[nCmd].nDataNum);

								// 引数の読み込みを引数の数分行う
								for (int nCntData = 0; nCntData < g_aMotionCommandInfo[nCmd].nDataNum; nCntData++) {
									fscanf_float(pFile, &pPartsMotion->pCommand[nCntCmd].pData[nCntData]);
								}

								// コマンドのカウントを加算
								nCntCmd++;
							}
						}
					}

					// 部品のカウントを加算
					nCntParts++;
				}
			}
		}
	}

	// データファイルを閉じる
	fclose(pFile);

	// モーション(3D)番号を返す
	return nMotionIdx;
}

//========================================
// SaveMotion3D関数 - モーション(3D)書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void SaveMotion3D(char *pPath, int nMotionIdx)
{
	FILE *pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen(pPath, "w");

	if (pFile != NULL)
	{// ファイルが開けた場合、
		// 書き込むモーション(3D)情報のポインタ
		Motion3D *pMotion = &g_pMotion3D[nMotionIdx];

		fprintf(pFile, "LOOP_TIME %d\n", pMotion->nLoopTime);
		fprintf(pFile, "LOOP %d\n", (int)pMotion->bLoop);

		{// 部品(3D)毎のモーション情報を書き込み
			fprintf(pFile, "PARTSMOTION %d\n", pMotion->nPartsMotionNum);

			for (int nCntParts = 0; nCntParts < pMotion->nPartsMotionNum; nCntParts++) {
				// 部品(3D)設定情報のポインタ
				PartsMotion3D *pPartsMotion = &pMotion->pPartsMotion[nCntParts];

				fprintf(pFile, "	PARTS %d\n", pPartsMotion->nCommandNum);
				for (int nCntCmd = 0; nCntCmd < pPartsMotion->nCommandNum; nCntCmd++) {
					Motion3DCommand cmd = pPartsMotion->pCommand[nCntCmd];
					fprintf(pFile, "		TIME %d ", cmd.nTime);
					switch (cmd.command) {
					case MOTION3D_COMMAND_LABEL_MOVE:
						fprintf(pFile, "MOVE %f %f %f %d %d\n", cmd.pData[0], cmd.pData[1], cmd.pData[2], (int)cmd.pData[3], (int)cmd.pData[4]);
						break;
					case MOTION3D_COMMAND_LABEL_SPIN:
						fprintf(pFile, "SPIN %f %f %f %d %d\n", cmd.pData[0], cmd.pData[1], cmd.pData[2], (int)cmd.pData[3], (int)cmd.pData[4]);
						break;
					case MOTION3D_COMMAND_LABEL_STEP:
						fprintf(pFile, "STEP %d %d %d\n", (int)cmd.pData[0], (int)cmd.pData[1], (int)cmd.pData[2]);
						break;
					default:
						assert(false);
						break;
					}
				}
				fprintf(pFile, "	PARTS_END\n");
			}
			fprintf(pFile, "PARTSMOTION_END\n");
		}

		fprintf(pFile, "END\n");

		// ファイルを閉じる
		fclose(pFile);
	}
}

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// UninitMotion3D関数 - モーション(3D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitMotion3D(void) {
	for (int nCntPath = 0; nCntPath < g_motion3DInfo.nMotionNum; nCntPath++) {
		// 読み込み済モーション(3D)パスのポインタ
		ReleaseMemory((void**)&g_motion3DInfo.ppLoadMotionPath[nCntPath]);
	}
	// 読み込み済モーション(3D)パスのダブルポインタ
	ReleaseMemory((void**)&g_motion3DInfo.ppLoadMotionPath);
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// GetMotion3D関数 - モーション(3D)の情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Motion3D *GetMotion3D(void) {
	return g_pMotion3D;
}

//========================================
// GetMotion3DCommandInfo関数 - モーション(3D)のコマンド毎の情報を取得 -
// Author:RIKU NISHIMURA
//========================================
Motion3DCommandInfo GetMotion3DCommandInfo(MOTION3D_COMMAND_LABEL cmd) {
	return g_aMotionCommandInfo[cmd];
}

//============================================================
//--------------------| *** その他 *** |----------------------
//============================================================

//========================================
// InitMotion3DMemory関数 - モーション(3D)の情報のメモリを初期化 -
// Author:RIKU NISHIMURA
//========================================
void InitMotion3DMemory(int nMotion) {
	// 現在のモーション数を保存
	int nMotionNumTemp = g_motion3DInfo.nMotionNum;

	// セットアップ数を代入
	g_motion3DInfo.nMotionNum = nMotion;

	// 文字列のダブルポインタのメモリ再確保
	ReGetMemory((void**)&g_motion3DInfo.ppLoadMotionPath, sizeof(char*), nMotionNumTemp, g_motion3DInfo.nMotionNum);

	// 文字列のポインタのメモリ確保
	g_motion3DInfo.ppLoadMotionPath[g_motion3DInfo.nMotionNum - 1] = NULL;
	GetMemory((void**)&g_motion3DInfo.ppLoadMotionPath[g_motion3DInfo.nMotionNum - 1], sizeof(char) * TXT_MAX, 1);

	// モーション(3D)の情報のメモリ再確保
	ReGetMemory((void**)&g_pMotion3D, sizeof(Motion3D), nMotionNumTemp, g_motion3DInfo.nMotionNum);

	// 確保した分の情報を全て初期化する
	for (int nCntMotion = 0; nCntMotion < nMotion; nCntMotion++) {
		g_pMotion3D[nCntMotion] = {};
	}
}