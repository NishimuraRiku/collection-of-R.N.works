//========================================
// 
// キャラクタ(3D)マネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** character3D_manager.cpp ***
//========================================
#include "character3D_manager.h"
#include "../../Other/memory.h"

//****************************************
// 静的メンバ変数宣言
//****************************************
int CChr3DManager::m_nChrNum = 0;		// キャラクタオブジェクト数
CChr3D **CChr3DManager::m_ppChr = NULL;	// キャラクタオブジェクトのダブルポインタ

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CCharacter3DManagerクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CChr3DManager::CChr3DManager(void) {
	m_nChrNum = 0;	// キャラクタオブジェクト数
	ReleaseMemory((void**)&m_ppChr);	// キャラクタオブジェクトのダブルポインタ
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CChr3DManager::~CChr3DManager(void) {

}

//========================================
// 生成処理
// Author:RIKU NISHIMURA
//========================================
CChr3D *CChr3DManager::Create(CChr3D::TYPE type, char *pBodyPath) {
	ReGetMemory((void**)&m_ppChr, sizeof(CChr3D*), m_nChrNum, m_nChrNum + 1);
	m_ppChr[m_nChrNum] = NULL;

	if (m_ppChr[m_nChrNum] != NULL) 
	{// メモリが使用されている時、メモリを解放する
		delete m_ppChr[m_nChrNum];
		m_ppChr[m_nChrNum] = NULL;
	}

	// 種類に応じた型のメモリ確保
	switch (type) {
	case CChr3D::TYPE_STANDERD:
		m_ppChr[m_nChrNum] = new CChr3D_Standard;
		break;
	case CChr3D::TYPE_HUMAN:
		m_ppChr[m_nChrNum] = new CChr3D_Human;
		break;
	default:
		assert(false);
		break;
	}

	// メモリが確保出来た時、初期化処理を行う
	if (m_ppChr[m_nChrNum] != NULL) {
		m_ppChr[m_nChrNum]->Init(pBodyPath);
	}
	else {
		assert(false);
	}

	return m_ppChr[m_nChrNum++];
}