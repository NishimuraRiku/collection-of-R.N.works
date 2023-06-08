//========================================
// 
// ���f���̃Z�b�g�A�b�v(3D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** model-setup3D.h ***
//========================================
#ifndef _MODELSETUP3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MODELSETUP3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "motion3D.h"
#include "../Draw/model.h"
#include "../../Other/physics.h"

//****************************************
// �񋓌^��`
//****************************************
// ���f���\��
typedef enum {
	MODELPLATFORM_NONE,				// ����
	MODELPLATFORM_BIPEDALISM_00,	// ����2�����s00
	MODELPLATFORM_BIPEDALISM_01,	// ����2�����s01
	MODELPLATFORM_MAX,
}MODELPLATFORM;

//****************************************
// �\���̒�`
//****************************************
// ���f���̃Z�b�g�A�b�v(3D)���\����
typedef struct {
	Parts3DSet partsSet;		// ���i�ݒ���(3D)
	HitTest3DSet hitTestSet;	// �����蔻��ݒ���
	MODELPLATFORM platform = MODELPLATFORM_NONE;	// �\��
}ModelSetUp3D;

// ���f���̃Z�b�g�A�b�v(3D)�Ǘ����
typedef struct {
	char **ppLoadModelSetUpPath = NULL;	// �ǂݍ��ݍς̃��f���Z�b�g�A�b�v�p�X
	int nModelSetUpNum = 0;	// ���f���̃Z�b�g�A�b�v(3D)��
}ModelSetUp3DControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// ���f���̃Z�b�g�A�b�v(3D)�̏I������
void UninitModelSetUp(void);
//========== *** ���o�� ***
// ���f���̃Z�b�g�A�b�v(3D)�̓ǂݍ��ݏ���
int LoadModelSetUp(char *pModelSetUpPath,	// ���f���̃Z�b�g�A�b�v(3D)�̃p�X
				   bool bReload = false,	// �ēǂݍ��݃t���O
				   int nSetUpIdx = -1);		// �Z�b�g�A�b�v�ԍ�
// ���f���̃Z�b�g�A�b�v(3D)�������ݏ���
void SaveModelSetUp(char *pPath, int nSetUpIdx);
//========== *** �擾 ***
// ���f���̃Z�b�g�A�b�v(3D)�����擾
ModelSetUp3D GetModelSetUpInfo(int nSetUp);
// ���f���̃Z�b�g�A�b�v(3D)�����擾(�|�C���^)
ModelSetUp3D **GetModelSetUp(void);
// ���f���̃Z�b�g�A�b�v(3D)�Ǘ������擾
ModelSetUp3DControl *GetModelSetUpControl(void);
//========== *** ���̑� ***
// ���f���̃Z�b�g�A�b�v(3D)���̃�������������
void InitModelSetUpMemory(int nNum);

#endif