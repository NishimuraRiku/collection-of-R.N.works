//========================================
// 
// EFF:����[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_explosion_00.h ***
//========================================
#ifndef _EFF_EXPLOSION_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_EXPLOSION_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:����[00] �̏���������
// EFF:����[00] �̏I������
// EFF:����[00] �̍X�V����
// EFF:����[00] �̕`�揈��
void InitEff_explosion_00(void);
void UninitEff_explosion_00(void);
void UpdateEff_explosion_00(void);
void DrawEff_explosion_00(void);

// EFF:����[00] �̓ǂݍ��ݏ���
void LoadEff_explosion_00(void);

// EFF:����[00] �̐ݒ菈��
void SetEff_explosion_00(D3DXVECTOR3 pos, int nType);

#endif