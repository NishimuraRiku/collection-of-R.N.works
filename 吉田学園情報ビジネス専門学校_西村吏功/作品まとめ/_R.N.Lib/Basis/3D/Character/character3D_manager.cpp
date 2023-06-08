//========================================
// 
// �L�����N�^(3D)�}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** character3D_manager.cpp ***
//========================================
#include "character3D_manager.h"
#include "../../Other/memory.h"

//****************************************
// �ÓI�����o�ϐ��錾
//****************************************
int CChr3DManager::m_nChrNum = 0;		// �L�����N�^�I�u�W�F�N�g��
CChr3D **CChr3DManager::m_ppChr = NULL;	// �L�����N�^�I�u�W�F�N�g�̃_�u���|�C���^

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CCharacter3DManager�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CChr3DManager::CChr3DManager(void) {
	m_nChrNum = 0;	// �L�����N�^�I�u�W�F�N�g��
	ReleaseMemory((void**)&m_ppChr);	// �L�����N�^�I�u�W�F�N�g�̃_�u���|�C���^
}

//========================================
// �f�X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CChr3DManager::~CChr3DManager(void) {

}

//========================================
// ��������
// Author:RIKU NISHIMURA
//========================================
CChr3D *CChr3DManager::Create(CChr3D::TYPE type, char *pBodyPath) {
	ReGetMemory((void**)&m_ppChr, sizeof(CChr3D*), m_nChrNum, m_nChrNum + 1);
	m_ppChr[m_nChrNum] = NULL;

	if (m_ppChr[m_nChrNum] != NULL) 
	{// ���������g�p����Ă��鎞�A���������������
		delete m_ppChr[m_nChrNum];
		m_ppChr[m_nChrNum] = NULL;
	}

	// ��ނɉ������^�̃������m��
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

	// ���������m�ۏo�������A�������������s��
	if (m_ppChr[m_nChrNum] != NULL) {
		m_ppChr[m_nChrNum]->Init(pBodyPath);
	}
	else {
		assert(false);
	}

	return m_ppChr[m_nChrNum++];
}