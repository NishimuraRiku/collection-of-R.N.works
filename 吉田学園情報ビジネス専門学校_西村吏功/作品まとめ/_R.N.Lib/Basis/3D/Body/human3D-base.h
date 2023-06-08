//========================================
// 
// �q���[�}��(3D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** human3D-base.h ***
//========================================
#ifndef _HUMAN3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _HUMAN3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "../../../RNmain.h"

//****************************************
// �\���̒�`
//****************************************
// �q���[�}��(3D)�x�[�X�\����
typedef struct {
	int nSetUpNum = 0;	// ���f���̃Z�b�g�A�b�v�ԍ�
	//========== *** �� ***
	D3DXVECTOR3 eyePos = INITD3DXVECTOR3;	// �ڂ̈ʒu
	D3DXVECTOR3 eyeRot = INITD3DXVECTOR3;	// �ڂ̌���
	int nEyeParent = -1;	// �ڂ̐e���i�ԍ�
	float fEyeSpace = 0.0f;	// �ڂ̊Ԋu
	float fEyeRotDia = 0.0f;// �ڂ̉�]�{��
}Human3DBase;

// �q���[�}��(3D)�x�[�X�Ǘ����
typedef struct {
	char **ppLoadHumanBasePath = NULL;// �ǂݍ��ݍς̃q���[�}��(3D)�x�[�X�p�X
	int nHumanBaseNum = 0;	// �q���[�}��(3D)�x�[�X��
}Human3DBaseControl;

//****************************************
// �֐��錾
//****************************************
//========== *** ��{�ڑ� ***
// �q���[�}��(3D)�x�[�X�̏I������
void UninitHuman3DBase(void);
//========== *** ���o�� ***
// �q���[�}��(3D)�x�[�X���̓ǂݍ��ݏ���
int LoadHuman3DBase(char *pHumanBasePath,		// �q���[�}��(3D)�x�[�X�̃p�X
					bool bReload      = false,	// �ēǂݍ��݃t���O
					int nHumanBaseIdx = -1,		// �q���[�}��(3D)�x�[�X�ԍ�
					bool bSetUpLoad   = true);	// �Z�b�g�A�b�v�ǂݍ��݃t���O
// �q���[�}��(3D)�x�[�X���̏������ݏ���
void SaveHuman3DBase(char *pPath, int nHumanBaseIdx);
//========== *** �擾 ***
// �q���[�}��(3D)�x�[�X�����擾
Human3DBase GetHuman3DBaseInfo(int nSetUp);
// �q���[�}��(3D)�x�[�X�����擾(�|�C���^)
Human3DBase **GetHuman3DBase(void);
// �q���[�}��(3D)�x�[�X�Ǘ������擾
Human3DBaseControl *GetHuman3DBaseControl(void);
//========== *** ���̑� ***
// �q���[�}��(3D)�x�[�X���̃�������������
void InitHuman3DBaseMemory(int nSetUpNum);

#endif