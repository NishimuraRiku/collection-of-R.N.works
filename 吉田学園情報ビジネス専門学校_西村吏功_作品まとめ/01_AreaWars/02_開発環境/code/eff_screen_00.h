//========================================
// *** eff_screen_00.h ***
//========================================
#ifndef _EFF_SCREEN_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_SCREEN_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �X�N���[��[00] �̏���������
void InitEff_screen_00(void);

// �X�N���[��[00] �̏I������
void UninitEff_screen_00(void);

// �X�N���[��[00] �̍X�V����
void UpdateEff_screen_00(void);

// �X�N���[��[00] �̕`�揈��
void DrawEff_screen_00(void);

// �X�N���[��[00] �̐ݒ菈��
void SetEff_screen_00(int nTime, Color col);

#endif