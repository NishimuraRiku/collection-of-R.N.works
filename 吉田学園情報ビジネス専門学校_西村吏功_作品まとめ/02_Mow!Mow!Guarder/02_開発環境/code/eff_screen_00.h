//========================================
// 
// EFF:�X�N���[��[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_screen_00.h ***
//========================================
#ifndef _EFF_SCREEN_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_SCREEN_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "eff_screen_00.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:�X�N���[��[00] �̏���������
// EFF:�X�N���[��[00] �̏I������
// EFF:�X�N���[��[00] �̍X�V����
// EFF:�X�N���[��[00] �̕`�揈��
void InitEff_screen_00(void);
void UninitEff_screen_00(void);
void UpdateEff_screen_00(void);
void DrawEff_screen_00(void);

// EFF:�X�N���[��[00] �̐ݒ菈��
void SetEff_screen_00(int nTime, Color col);

#endif