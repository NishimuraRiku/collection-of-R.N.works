//========================================
// 
// EFF:�e[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_shadow_00.h ***
//========================================
#ifndef _EFF_SHADOW_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_SHADOW_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:�e[00] �̏���������
// EFF:�e[00] �̏I������
// EFF:�e[00] �̍X�V����
// EFF:�e[00] �̕`�揈��
void InitEff_shadow_00(void);
void UninitEff_shadow_00(void);
void UpdateEff_shadow_00(void);
void DrawEff_shadow_00(void);

// EFF:�e[00] �̐ݒ菈��
void SetEff_shadow_00(D3DXVECTOR3 pos, float fRadius);

#endif