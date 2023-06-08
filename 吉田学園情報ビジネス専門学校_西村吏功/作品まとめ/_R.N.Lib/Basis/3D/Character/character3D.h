//========================================
// 
// キャラクタ(3D)のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** character3D.h ***
//========================================
#ifndef _CHARACTER3D_H_	// このマクロ定義がされていなかった時
#define _CHARACTER3D_H_	// 二重インクルード防止のマクロを定義する

#include "../Body/parts3D_class.h"

//****************************************
// クラス定義
//****************************************
// キャラクタ(3D)クラス
class CChr3D {
public:
	//========== *** 列挙型定義 ***
	// 種類
	typedef enum {
		TYPE_STANDERD,	// スタンダード型
		TYPE_HUMAN,		// ヒューマン型
		TYPE_MAX,
	}TYPE;

	//========== *** 関数宣言 ***
	/* コンストラクタ */CChr3D();
	/* デストラクタ   */virtual ~CChr3D();
	/* 初期化処理     */virtual void Init(char *pBodyPath) = 0;

protected:
	//========== *** 変数宣言 ***
	int m_nModelSetUpIdx;	// モデルのセットアップ番号
	CParts3D m_parts3D;		// 部品(3D)のインスタンス
};

// スタンダード型キャラクタ(3D)クラス
class CChr3D_Standard :public CChr3D {
public:
	//========== *** 関数宣言 ***
	/* コンストラクタ */CChr3D_Standard();
	/* デストラクタ   */~CChr3D_Standard();
	/* 初期化処理     */void Init(char *pBodyPath);

};

// ヒューマン型キャラクタ(3D)クラス
class CChr3D_Human :public CChr3D {
public:
	//========== *** 関数宣言 ***
	/* コンストラクタ */CChr3D_Human();
	/* デストラクタ   */~CChr3D_Human();
	/* 初期化処理     */void Init(char *pBodyPath);

	//========== *** 変数宣言 ***
	int m_nIdxHumanBase;	// ヒューマンベース番号
};

#endif