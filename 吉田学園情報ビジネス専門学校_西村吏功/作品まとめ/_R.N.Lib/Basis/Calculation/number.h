//========================================
// 
// 数値処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** number.h ***
//========================================
#ifndef _NUMBER_H_	// このマクロ定義がされていなかった時
#define _NUMBER_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

//****************************************
// 列挙型定義
//****************************************
// カウントの種類
typedef enum
{
	COUNTTYPE_NORMAL,	// 通常
	COUNTTYPE_TURN,		// 折り返し
	COUNTTYPE_MAX,
}COUNTTYPE;

//****************************************
// プロトタイプ宣言
//****************************************
/* カウント処理                     */int Counter(int *pData, int nMin, int nMax, int *pCounter, int nCountTime, COUNTTYPE countType);
/* 整数型の消費処理                 */bool IntConsumption(int *pData, int nSubtract);
/* 浮動小数型の値の制御処理         */void FloatControl(float *pData, float fMax, float fMin);
/* 浮動小数型の値のループ制御処理   */void FloatLoopControl(float *pData, float fMax, float fMin);
/* 整数値の制御処理                 */void IntControl(int *pData, int nMax, int nMin);
/* 整数値のループ制御処理           */void IntLoopControl(int *pData, int nMax, int nMin);
/* 整数値の制御処理結果を返す       */int IntControlReturn(int nData, int nMax, int nMin);
/* 整数値のループ制御処理結果を返す */int IntLoopControlReturn(int nData, int nMax, int nMin);
/* 抽選処理                         */bool Lottery(float fProb);
/* 浮動小数型の乱数を返す           */float fRand(float fMax);
/* 整数型乱数を返す                 */int Rand(int nMin, int nMax);
/* 値を間隔刻みで返す               */float Tick(float fData, float fSpace);
/* (与えられた値の符号)1を返す      */int Sign(float fData);
/* 与えられた値を正の数で返す       */float MaxOfZero(float fData);

#endif