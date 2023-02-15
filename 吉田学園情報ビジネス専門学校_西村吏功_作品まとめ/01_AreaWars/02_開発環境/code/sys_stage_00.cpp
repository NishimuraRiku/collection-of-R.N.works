//========================================
// 
// システム:ステージ[00] の処理
// Author:西村 吏功
// 
//========================================
// *** sys_stage_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "atk_bullet_00.h"
#include "bg_color.h"
#include "chr_fighter_00.h"
#include "chr_enemy_00.h"
#include "md_game_00.h"
#include "sys_stage_00.h"
#include "obj_plasma_block_00.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************

// ステージ[00] の情報のデータファイルの相対パス
// 配置間隔
// 変色にかかる時間
#define SYS_STAGE_00_STAGE_DATA_FILE_PATH	"data\\GAMEDATA\\STAGE_DATA.txt"
#define POSITIONING_SPACE					(PIXEL * 16)
#define CHANGE_COLOR_TIME					(60)

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterSys_stage_00(void);

// 配置情報のパラメーターの初期化処理
void InitSetParameterSys_stage_00(void);

// 敵の配置処理
void SetEnemy(void);

// サウンド切り替え処理
void SoundChange(void);

// ステージの情報の読み込み処理
void LoadStage(void);

// ウェーブの情報の読み込み処理
void LoadWave(void);

// ステージを検索
void StageSearch(FILE *pFile);

// ウェーブを検索
void WaveSearch(FILE *pFile);

//****************************************
// グローバル宣言
//****************************************
Sys_stage_00 g_sys_stage_00;	// ステージ[00] の情報

//========== *** システム:ステージ[00] の情報を取得 ***
Sys_stage_00 *GetSys_stage_00(void)
{
	return &g_sys_stage_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterSys_stage_00関数 - パラメーターの初期化処理 -
//========================================
void InitParameterSys_stage_00(void) 
{
	g_sys_stage_00.nStageNum		= 0;		// ステージ番号
	g_sys_stage_00.nWaveMax			= 0;		// ウェーブの最大数
	g_sys_stage_00.nWaveCount		= -1;		// ウェーブのカウント
	g_sys_stage_00.nDispWaveCount	= 0;		// 表示するウェーブのカウント
	g_sys_stage_00.nWaveTime		= 0;		// ウェーブの時間
	g_sys_stage_00.nWaveEndTime		= 0;		// ウェーブの終了時間
	g_sys_stage_00.bWaveUp			= false;	// ウェーブを増やすか否か
	g_sys_stage_00.nLastWave		= 0;		// 最終ウェーブ
	g_sys_stage_00.nLastDispWave	= 0;		// 最終表示ウェーブ
	g_sys_stage_00.nLastScore		= 0;		// 最終スコア
	g_sys_stage_00.nLastBomb		= 0;		// 最終ボム保持数
	g_sys_stage_00.nEnemySetNum		= 0;		// 敵の配置数

	// 配置情報のパラメーターの初期化処理
	InitSetParameterSys_stage_00();

	g_sys_stage_00.sound	= (SOUND_LABEL)-1;	// サウンド番号
	g_sys_stage_00.soundNow	= (SOUND_LABEL)-1;	// 現在再生中のサウンド番号
}

//========================================
// InitSetParameterSys_stage_00関数 - 配置情報のパラメーターの初期化処理 -
//========================================
void InitSetParameterSys_stage_00(void)
{
	for (int nCntSet = 0; nCntSet < ENEMY_SET_MAX; nCntSet++)
	{
		g_sys_stage_00.aEnemySet[nCntSet].nTime			= -1;
		g_sys_stage_00.aEnemySet[nCntSet].nType			= 0;
		g_sys_stage_00.aEnemySet[nCntSet].setPos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_sys_stage_00.aEnemySet[nCntSet].setRot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_sys_stage_00.aEnemySet[nCntSet].bAreaBoss		= false;
		g_sys_stage_00.aEnemySet[nCntSet].bBomb			= false;
		g_sys_stage_00.aEnemySet[nCntSet].nParentNum	= -1;
		g_sys_stage_00.aEnemySet[nCntSet].nChildNum		= -1;
	}
}

//========================================
// SetEnemy関数 - 敵の配置処理 -
//========================================
void SetEnemy(void)
{
	Sys_stage_00	*pSys	// ステージ[00] の情報
					= &g_sys_stage_00;

	for (int nCntSet = 0; nCntSet < pSys->nEnemySetNum; nCntSet++)
	{
		if (pSys->nWaveTime == pSys->aEnemySet[nCntSet].nTime)
		{// ウェーブの時間が敵を配置する時間と一致した時、
			// 敵[00] の配置処理
			SetChr_enemy_00(
				pSys->aEnemySet[nCntSet].setPos,
				pSys->aEnemySet[nCntSet].setRot,
				pSys->aEnemySet[nCntSet].nType,
				pSys->aEnemySet[nCntSet].bAreaBoss,
				pSys->aEnemySet[nCntSet].bBomb,
				pSys->aEnemySet[nCntSet].nParentNum,
				pSys->aEnemySet[nCntSet].nChildNum);
		}
	}

	// ウェーブの時間を加算
	pSys->nWaveTime++;
}

//========================================
// SoundChange関数 - サウンド切り替え処理 -
//========================================
void SoundChange(void) 
{
	Sys_stage_00	*pSys
					= &g_sys_stage_00;	// ステージ[00] の情報

	if (pSys->sound != pSys->soundNow)
	{// サウンドが再生中のサウンドと異なる時、
		if (pSys->soundNow != -1)
		{// 再生中のサウンド番号が-1でない時、
			// 再生中のサウンドを停止
			StopSound((SOUND_LABEL)pSys->soundNow);
		}
		if (pSys->sound != -1)
		{// サウンド番号が-1でない時、
			// サウンドを再生
			PlaySound((SOUND_LABEL)pSys->sound);
		}

		// 再生中のサウンドを設定
		pSys->soundNow = pSys->sound;
	}
}

//========================================
// LoadStage関数 - ステージの情報の読み込み処理 -
//========================================
void LoadStage(void)
{
	FILE			*pFile;				// ファイルポインタ
	char			aDataSearch			//
					[TXT_MAX];			// データ検索用
	Sys_stage_00	*pSys				// ステージ[00] の情報
					= &g_sys_stage_00;	//

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(SYS_STAGE_00_STAGE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ステージを検索
	StageSearch(pFile);

	// ウェーブの最大数を初期化
	pSys->nWaveMax = 0;

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (strcmp(aDataSearch, "STAGE_END") == 0)
		{// ファイルを閉じる
			fclose(pFile);
			// 繰り返し処理を抜ける
			break;
		}
		else if (strcmp(aDataSearch, "WAVE") == 0)
		{// ウェーブの最大数を加算
			pSys->nWaveMax++;
		}
	}
}

//========================================
// LoadWave関数 - ウェーブの情報の読み込み処理 -
//========================================
void LoadWave(void)
{
	FILE			*pFile;				// ファイルポインタ
	char			aDataSearch			// データ検索用
					[TXT_MAX];			//
	Sys_stage_00	*pSys				// ステージ[00] の情報
					= &g_sys_stage_00;	//

	// 配置情報のパラメーターの初期化処理
	InitSetParameterSys_stage_00();

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(SYS_STAGE_00_STAGE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ステージを検索
	StageSearch(pFile);

	// ウェーブを検索
	WaveSearch(pFile);

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (strcmp(aDataSearch, "WAVE_END") == 0)
		{// ファイルを閉じる
			fclose(pFile);
			// 繰り返し処理を抜ける
			break;
		}//====================//
		else if (strcmp(aDataSearch, "ウェーブタイム:") == 0)
		{// ウェーブの終了時間を読み込む
			fscanf(pFile, "%d", &pSys->nWaveEndTime);
		}//====================//
		else if (strcmp(aDataSearch, "プラズマウォールの上辺Y:") == 0)
		{// プラズマブロック[00] の上辺Yを読み込む
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetTop);

			// 位置を正しい値に更新
			GetObj_plasma_block_00_square()->fTargetTop *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "プラズマウォールの下辺Y:") == 0)
		{// プラズマブロック[00] の下辺Yを読み込む
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetBottom);

			// 位置を正しい値に更新
			GetObj_plasma_block_00_square()->fTargetBottom *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "プラズマウォールの左辺X:") == 0)
		{// プラズマブロック[00] の左辺Xを読み込む
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetLeft);

			// 位置を正しい値に更新
			GetObj_plasma_block_00_square()->fTargetLeft *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "プラズマウォールの右辺X:") == 0)
		{// プラズマブロック[00] の右辺Xを読み込む
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetRight);

			// 位置を正しい値に更新
			GetObj_plasma_block_00_square()->fTargetRight *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "ウェーブ数を増やすか否か:") == 0)
		{// ウェーブ数を増やすか否かを読み込む
			int nBTemp;	// 一時保存用

			fscanf(pFile, "%d", &nBTemp);

			pSys->bWaveUp = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "エリア時間の表示:") == 0)
		{// エリア時間の表示フラグを読み込む
			int nBTemp;	// 一時保存用

			fscanf(pFile, "%d", &nBTemp);

			pSys->bShowAreaTime = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "セーブ地点:") == 0)
		{// セーブ地点フラグを読み込む
			int nBTemp;	// 一時保存用

			fscanf(pFile, "%d", &nBTemp);

			pSys->bSavePoint = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "警告:") == 0)
		{// 警告フラグを読み込む
			int nBTemp;	// 一時保存用

			fscanf(pFile, "%d", &nBTemp);

			pSys->bWarning = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "サウンド:") == 0)
		{// サウンドを読み込む
			fscanf(pFile, "%s", aDataSearch); // 検索

			// サウンドの種類を文字列から読み込む
			StringLoadSound(aDataSearch, &pSys->sound);
		}//====================//
		else if (strcmp(aDataSearch, "背景色:") == 0)
		{// 背景色を読み込む
			Color bgColor;	// 背景色

			fscanf(pFile, "%d", &bgColor.nR);
			fscanf(pFile, "%d", &bgColor.nG);
			fscanf(pFile, "%d", &bgColor.nB);
			fscanf(pFile, "%d", &bgColor.nA);

			// 背景色の設定処理
			SetBg_color(bgColor, pSys->nWaveCount == 0 ? 0 : CHANGE_COLOR_TIME);
		}//====================//
		else if (strcmp(aDataSearch, "FLOW") == 0)
		{// 敵の配置数を初期化する
			pSys->nEnemySetNum = 0;

			// フローの文末識別子が見つかるまで敵の配置情報の読み込みを繰り返す
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (strcmp(aDataSearch, "FLOW_END") == 0)
				{// 繰り返し処理を抜ける
					break;
				}//====================//
				else if (strcmp(aDataSearch, "配置:") == 0)
				{// 配置する時間を読み込む
					fscanf(pFile, "%d", &pSys->aEnemySet[pSys->nEnemySetNum].nTime);

					fscanf(pFile, "%s", aDataSearch); // 検索

					if (strcmp(aDataSearch, "[") == 0)
					{// 配置情報の読み込みを開始する
						while (1)
						{
							fscanf(pFile, "%s", aDataSearch); // 検索

							if (strcmp(aDataSearch, "]") == 0)
							{// 繰り返し処理を抜ける
								break;
							}//====================//
							else if (strcmp(aDataSearch, "種類:") == 0)
							{// 種類を読み込む
								fscanf(pFile, "%s", aDataSearch); // 検索

								Chr_enemy_00Type	*pChrType	// 敵[00] の種類毎の情報
													= GetChr_enemy_00Type();

								for (int nCntChrType = 0; nCntChrType < CHR_ENEMY_00_TYPE_MAX; nCntChrType++, pChrType++)
								{
									if (strcmp(aDataSearch, pChrType->aLoadName) == 0)
									{// 読み込み名が一致した時
										// 種類を設定
										pSys->aEnemySet[pSys->nEnemySetNum].nType = nCntChrType;
										break;
									}
								}
							}//====================//
							else if (strcmp(aDataSearch, "X:") == 0)
							{// 設置する位置のX座標を読み込む
								fscanf(pFile, "%f", &pSys->aEnemySet[pSys->nEnemySetNum].setPos.x);

								// 位置を正しい値に更新
								pSys->aEnemySet[pSys->nEnemySetNum].setPos.x *= POSITIONING_SPACE;
							}//====================//
							else if (strcmp(aDataSearch, "Y:") == 0)
							{// 設置する位置のY座標を読み込む
								fscanf(pFile, "%f", &pSys->aEnemySet[pSys->nEnemySetNum].setPos.y);

								// 位置を正しい値に更新
								pSys->aEnemySet[pSys->nEnemySetNum].setPos.y *= POSITIONING_SPACE;
							}//====================//
							else if (strcmp(aDataSearch, "角度:") == 0)
							{// 設置する角度を読み込む
								fscanf(pFile, "%f", &pSys->aEnemySet[pSys->nEnemySetNum].setRot.z);

								// 角度を正しい値に変換
								pSys->aEnemySet[pSys->nEnemySetNum].setRot.z *= D3DX_PI;
							}//====================//
							else if (strcmp(aDataSearch, "エリアボス") == 0)
							{// エリアボスフラグを真にする
								pSys->aEnemySet[pSys->nEnemySetNum].bAreaBoss = true;
							}//====================//
							else if (strcmp(aDataSearch, "ボム") == 0)
							{// 爆弾所持フラグを真にする
								pSys->aEnemySet[pSys->nEnemySetNum].bBomb = true;
							}//====================//
							else if (strcmp(aDataSearch, "親:") == 0)
							{// 親番号を読み込む
								fscanf(pFile, "%d", &pSys->aEnemySet[pSys->nEnemySetNum].nParentNum);
							}//====================//
							else if (strcmp(aDataSearch, "子:") == 0)
							{// 子供番号を読み込む
								fscanf(pFile, "%d", &pSys->aEnemySet[pSys->nEnemySetNum].nChildNum);
							}//====================//
						}
					}

					// 敵の配置数を加算
					pSys->nEnemySetNum++;
				}
			}
		}//====================//
	}
}

//========================================
// StageSearch関数 - ステージを検索 -
//========================================
void StageSearch(FILE *pFile)
{
	int				nCntStage = 0;		// ステージのカウント
	char			aDataSearch			// データ検索用
					[TXT_MAX];			//
	Sys_stage_00	*pSys				// ステージ[00] の情報
					= &g_sys_stage_00;	//

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (strcmp(aDataSearch, "STAGE") == 0)
		{// 検索用の文字列が[ステージ] 識別子と一致した時、
			if (nCntStage == pSys->nStageNum)
			{// ステージのカウントがステージ番号と一致していた時、
				// 繰り返し処理を抜ける
				break;
			}
			else
			{// ステージのカウントがステージ番号と一致していない時、
				// ステージのカウントを加算
				nCntStage++;
			}
		}
	}
}

//========================================
// WaveSearch関数 - ウェーブを検索 -
//========================================
void WaveSearch(FILE *pFile)
{
	int				nCntWave			// ウェーブのカウント
					= 0;				//
	char			aDataSearch			// データ検索用
					[TXT_MAX];			//
	Sys_stage_00	*pSys				// ステージ[00] の情報
					= &g_sys_stage_00;	//

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (strcmp(aDataSearch, "WAVE") == 0)
		{// 検索用の文字列が[ウェーブ] 識別子と一致した時、
			if (nCntWave == pSys->nWaveCount)
			{// ウェーブのカウントがウェーブ番号と一致していた時、
				// 繰り返し処理を抜ける
				break;
			}
			else
			{// ウェーブのカウントがウェーブ番号と一致していない時、
				// ウェーブのカウントを加算
				nCntWave++;
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitSys_stage_00関数 - システム:ステージ[00] の初期化処理 -
//========================================
void InitSys_stage_00(int nStageNum)
{
	// パラメーターを初期化
	InitParameterSys_stage_00();

	// ステージ番号を代入
	g_sys_stage_00.nStageNum = nStageNum;

	// ステージの情報を読み込む
	LoadStage();

	// ウェーブの推移処理
	WaveTrendSys_stage_00();
}

//========================================
// UninitSys_stage_00関数 - システム:ステージ[00] の終了処理 -
//========================================
void UninitSys_stage_00(void) 
{

}

//========================================
// UpdateSys_stage_00関数 - システム:ステージ[00] の更新処理 -
//========================================
void UpdateSys_stage_00(void) 
{
	Sys_stage_00	*pSys	// ステージ[00] の情報
					= &g_sys_stage_00;

	if (GetGameState() == MD_GAME_00_STATE_NORMAL)
	{// ゲームの状態が通常の時、
		// 敵の配置処理
		SetEnemy();
	}

	if ((pSys->nWaveTime >= pSys->nWaveEndTime)
		&& (pSys->nWaveEndTime != -1))
	{// ウェーブの時間がウェーブの終了時間に達し、かつ無限でない時、
		// ウェーブの推移処理
		WaveTrendSys_stage_00();
	}
}

//========================================
// WaveTrendSys_stage_00関数 - ウェーブの推移処理 -
//========================================
void WaveTrendSys_stage_00(void) 
{
	// 敵[00] を全て消し去る
	RemoveAllChr_enemy_00();

	// 敵の弾[00] の全破壊処理
	AllDestructionAtk_bullet_00(ATK_PARENT_TYPE_ENEMY);

	Sys_stage_00	*pSys	// ステージ[00] の情報
					= &g_sys_stage_00;

	// ウェーブのカウントを加算
	pSys->nWaveCount++;

	// ウェーブの時間を初期化
	pSys->nWaveTime = 0;

	// ウェーブ数を増やすか否かを初期化
	pSys->bWaveUp = false;

	if (pSys->nWaveCount >= pSys->nWaveMax)
	{// ウェーブのカウントがウェーブの最大数に達した時、
		// ゲームクリアに移行
		SetMd_game_00State(MD_GAME_00_STATE_GAME_CLEAR);

		// 処理を終了
		return;
	}

	// ウェーブの情報を読み込み
	LoadWave();

	// サウンド切り替え処理
	SoundChange();

	if (pSys->bSavePoint)
	{// セーブ地点フラグが真の時、
		// 現在の状態を保存
		pSys->nLastWave		= pSys->nWaveCount;
		pSys->nLastDispWave = pSys->nDispWaveCount;
		pSys->nLastScore	= GetChr_fighter_00()->nScore;
		pSys->nLastBomb		= GetChr_fighter_00()->nBomb;
	}

	if (pSys->bWaveUp)
	{// ウェーブ数を増やすか否かが真の時、
		// 表示するウェーブのカウントを加算
		pSys->nDispWaveCount++;

		// ゲームの状態をウェーブ待ちに設定
		SetMd_game_00State(MD_GAME_00_STATE_WAVE_WAIT);

		// プラズマブロック[00] の移動量を設定
		SetMoveObj_plasma_block_00();
	}
	else if (pSys->bWarning) 
	{// 警告フラグが真の時、
		// ゲームの状態を警告に設定
		SetMd_game_00State(MD_GAME_00_STATE_WARNING);
	}
	else 
	{// ウェーブ数を増やさず警告フラグも偽の時、
		// プラズマブロック[00] の移動量を設定
		SetMoveObj_plasma_block_00();
	}
}

//========================================
// ContinueSys_stage_00関数 - システム:ステージ[00] のコンティニュー処理 -
//========================================
void ContinueSys_stage_00(void) 
{
	Sys_stage_00	*pSys					// ステージ[00] の情報のポインタ
					= &g_sys_stage_00;		// 
	Chr_fighter_00	*pChr					// 機体[00] の情報のポインタ
					= GetChr_fighter_00();	// 

	// 最終保存時の状態にする
	pSys->nWaveCount		= pSys->nLastWave - 1;
	pSys->nDispWaveCount	= pSys->nLastDispWave;
	pChr->nScore			= pSys->nLastScore;
	pChr->nBomb				= pSys->nLastBomb;

	// ウェーブの推移処理
	WaveTrendSys_stage_00();
}