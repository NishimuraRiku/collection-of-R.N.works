//========================================
// 
// EFF:��[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_light_00.h ***
//========================================
#ifndef _EFF_LIGHT_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_LIGHT_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:��[00] �̏���������
// EFF:��[00] �̏I������
// EFF:��[00] �̍X�V����
// EFF:��[00] �̕`�揈��
void InitEff_light_00(void);
void UninitEff_light_00(void);
void UpdateEff_light_00(void);
void DrawEff_light_00(void);

// EFF:��[00] �̓ǂݍ��ݏ���
void LoadEff_light_00(void);

// EFF:��[00] �̐ݒ菈��
void SetEff_light_00(D3DXVECTOR3 pos, int nType, float fScale);

#endif