//========================================
// 
// UI:HP�o�[[01] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_hp-bar_01.h ***
//========================================
#ifndef _UI_HPBAR_01_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_HPBAR_01_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:HP�o�[[01] �̏���������
// UI:HP�o�[[01] �̏I������
// UI:HP�o�[[01] �̍X�V����
// UI:HP�o�[[01] �̕`�揈��
void InitUi_hpBar_01(void);
void UninitUi_hpBar_01(void);
void UpdateUi_hpBar_01(void);
void DrawUi_hpBar_01(void);

// UI:HP�o�[[01] �̓ǂݍ��ݏ���
void LoadUi_hpBar_01(void);

// UI:HP�o�[[01] �̐ݒ菈��
void SetUi_hpBar_01(D3DXVECTOR3 pos, int nType);

#endif