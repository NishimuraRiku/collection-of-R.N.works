//========================================
// 
// EFF:�p�[�e�B�N��[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_particle_00.h ***
//========================================
#ifndef _EFF_PARTICLE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_PARTICLE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:�p�[�e�B�N��[00] �̏���������
// EFF:�p�[�e�B�N��[00] �̏I������
// EFF:�p�[�e�B�N��[00] �̍X�V����
void InitEff_particle_00(void);
void UninitEff_particle_00(void);
void UpdateEff_particle_00(void);

// EFF:�p�[�e�B�N��[00] �̓ǂݍ��ݏ���
void LoadEff_particle_00(void);

// EFF:�p�[�e�B�N��[00] �̐ݒ菈��
void SetEff_particle_00(D3DXVECTOR3 pos, int nType);

#endif