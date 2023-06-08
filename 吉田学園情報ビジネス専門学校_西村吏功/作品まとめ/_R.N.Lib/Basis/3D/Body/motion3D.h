//========================================
// 
// モーション(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** motion3D.h ***
//========================================
#ifndef _MOTION3D_H_	// このマクロ定義がされていなかった時
#define _MOTION3D_H_	// 二重インクルード防止のマクロを定義する

#include "parts3D.h"

//****************************************
// 列挙型定義
//****************************************
// モーション(3D)コマンド
typedef enum {
	MOTION3D_COMMAND_LABEL_MOVE,	// 移動
	MOTION3D_COMMAND_LABEL_SPIN,	// 回転
	MOTION3D_COMMAND_LABEL_STEP,	// 踏む
	MOTION3D_COMMAND_LABEL_MAX,
}MOTION3D_COMMAND_LABEL;

//****************************************
// 構造体定義
//****************************************
// モーション(3D)コマンド情報
typedef struct {
	char *pLabelName;	// ラベル名
	int nDataNum;		// 引数の数
}Motion3DCommandInfo;

// モーションコマンド情報構造体
typedef struct {
	int nTime = 0;	// 時間
	MOTION3D_COMMAND_LABEL command = (MOTION3D_COMMAND_LABEL)0;	// コマンドの種類
	float *pData = NULL;	// 引数
}Motion3DCommand;

// 部品毎のモーション(3D)情報構造体
typedef struct {
	int nCommandNum = 0;	// コマンド数
	Motion3DCommand *pCommand = NULL;	// コマンド情報
}PartsMotion3D;

// モーション(3D)情報構造体
typedef struct {
	int nLoopTime = 0;	// ループ時間
	bool bLoop = false;	// ループするかフラグ
	int nPartsMotionNum = 0;	// 部品毎のモーションの数
	PartsMotion3D *pPartsMotion = NULL;	// 部品毎のモーション情報
}Motion3D;

// モーション(3D)の読み込み情報構造体
typedef struct {
	char *pMotionPath = NULL;	// モーション(3D)のパス
	bool bReload = false;	// 再読み込みフラグ
	int nMotionIdx = -1;	// モーション(3D)番号
#if /* 設定フォーマット */0
	// モーション(3D)の読み込み情報
	Motion3DLoad motion3DLoad;
	/* モーションのパス */motion3DLoad.pMotionPath = ;
	/* 再読み込みフラグ */motion3DLoad.bReload = ;
	/* モーション番号   */motion3DLoad.nMotionIdx = ;
	// モーション(3D)を読み込み
	= LoadMotion3D(motion3DLoad);
#endif
}Motion3DLoad;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** モーション(3D)関連 ***
// モーション(3D)の読み込み処理
int LoadMotion3D(Motion3DLoad motionLoad);
// モーション(3D)書き込み処理
void SaveMotion3D(char *pPath, int nMotionIdx);
//========== *** 基本接続 ***
// モーション(3D)の終了処理
void UninitMotion3D(void);
//========== *** 取得 ***
// モーション(3D)の情報を取得
Motion3D *GetMotion3D(void);
// モーション(3D)のコマンド毎の情報を取得
Motion3DCommandInfo GetMotion3DCommandInfo(MOTION3D_COMMAND_LABEL cmd);
//========== *** その他 ***
// モーション(3D)の情報のメモリを初期化
void InitMotion3DMemory(int nMotion);

#endif