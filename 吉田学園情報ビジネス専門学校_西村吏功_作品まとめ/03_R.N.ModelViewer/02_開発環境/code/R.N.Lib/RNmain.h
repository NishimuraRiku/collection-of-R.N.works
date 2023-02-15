//========================================
// 
// R.N.Lib�̃��C�������̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** RNmain.h ***
//========================================
#ifndef _RNMAIN_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _RNMAIN_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// _R.N.Lib
#include "../R.N.Writable/RNsetup.h"	// �Z�b�g�A�b�v
#include "camera2D.h"
#include "camera3D.h"
#include "chunk.h"
#include "color.h"
#include "fade.h"
#include "file.h"
#include "input.h"
#include "light.h"
#include "map-generator3D.h"
#include "object3D.h"
#include "physics.h"
#include "polygon2D.h"
#include "polygon3D.h"
#include "setting.h"
#include "sound.h"
#include "text.h"

//****************************************
// �\���̂̒�`
//****************************************
// R.N.Lib�̃��C�������̏��\����
typedef struct 
{
	MODE mode;	// ���[�h
}RNmain;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// ����������
void RNInit(HINSTANCE hInstance);
// �I������
void RNUninit(void);
// �X�V����
void RNUpdate(void);
// �`�揈��
void RNDraw(void);
//========== *** ���o�� ***
// �ǂݍ��ݏ���
void RNLoad(void);
// �������ݏ���
void RNSave(void);
//========== *** �擾 ***
// ���[�h�̎擾
MODE GetMode(void);
//========== *** �ݒ� ***
// ���[�h�̐ݒ菈��
void SetMode(MODE mode);

#endif