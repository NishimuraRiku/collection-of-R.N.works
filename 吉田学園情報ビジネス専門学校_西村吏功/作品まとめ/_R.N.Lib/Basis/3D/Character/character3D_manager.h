//========================================
// 
// キャラクタ(3D)マネージャーのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** character3D_manager.h ***
//========================================
#ifndef _CHARACTER3D_MANAGER_H_	// このマクロ定義がされていなかった時
#define _CHARACTER3D_MANAGER_H_	// 二重インクルード防止のマクロを定義する

#include "character3D.h"

//****************************************
// クラス定義
//****************************************
// キャラクタ(3D)マネージャークラス
class CChr3DManager {
public:
	//========== *** 関数宣言 ***
	/* コンストラクタ */CChr3DManager();
	/* デストラクタ   */~CChr3DManager();
	/* 生成処理       */static CChr3D *Create(CChr3D::TYPE type, char *pBodyPath);

private:
	//========== *** 変数宣言 ***
	static int m_nChrNum;		// キャラクタオブジェクト数
	static CChr3D **m_ppChr;	// キャラクタオブジェクトのダブルポインタ

};

#endif