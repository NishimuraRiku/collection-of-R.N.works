//========================================
// 
// �t�H�O�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** fog.h ***
//========================================
#ifndef _FOG_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _FOG_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �\���̂̐錾
//****************************************
// �t�H�O�̐ݒ���\����
typedef struct {
	Color col = INITCOLOR;	// �F
	int nStart = 0;	// �J�n����
	int nEnd = 0;	// �I������
#if /* �ݒ�t�H�[�}�b�g */0
	// �t�H�O�̐ݒ���
	FogSet fogSet;
	/* �F       */fogSet.col = ;
	/* �J�n���� */fogSet.nStart = ;
	/* �I������ */fogSet.nEnd = ;
	// �t�H�O�̐ݒ菈��
	SetFog(fogSet);
#endif
}FogSet;

// �t�H�O�̏��\����
typedef struct {
	bool bUse = false;	// �g�p����Ă��邩�t���O
}Fog;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �擾 ***
// �t�H�O�̏����擾
Fog *GetFog(void);
//========== *** �ݒ� ***
// �t�H�O�̐ݒ菈��
void SetFog(FogSet fogSet);
// �t�H�O�̗L�������ݒ�
void EnableFog(bool bEnable);

#endif