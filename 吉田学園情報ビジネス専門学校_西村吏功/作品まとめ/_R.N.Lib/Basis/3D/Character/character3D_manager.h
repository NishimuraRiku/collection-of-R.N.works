//========================================
// 
// �L�����N�^(3D)�}�l�[�W���[�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** character3D_manager.h ***
//========================================
#ifndef _CHARACTER3D_MANAGER_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHARACTER3D_MANAGER_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "character3D.h"

//****************************************
// �N���X��`
//****************************************
// �L�����N�^(3D)�}�l�[�W���[�N���X
class CChr3DManager {
public:
	//========== *** �֐��錾 ***
	/* �R���X�g���N�^ */CChr3DManager();
	/* �f�X�g���N�^   */~CChr3DManager();
	/* ��������       */static CChr3D *Create(CChr3D::TYPE type, char *pBodyPath);

private:
	//========== *** �ϐ��錾 ***
	static int m_nChrNum;		// �L�����N�^�I�u�W�F�N�g��
	static CChr3D **m_ppChr;	// �L�����N�^�I�u�W�F�N�g�̃_�u���|�C���^

};

#endif