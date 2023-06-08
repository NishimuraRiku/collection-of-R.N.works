//========================================
// 
// �e�L�X�g(3D)�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** text3D.h ***
//========================================
#ifndef _TEXT3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _TEXT3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "../../Graphic/text.h"

//****************************************
// �\���̒�`
//****************************************
// �e�L�X�g(3D)�̐ݒ���
typedef struct {
	char aString[TXT_MAX] = "";	// ������̃|�C���^
	int nFont = 0;			// �t�H���g�ԍ�
	TEXT_DISP disp = TEXT_DISP_CENTER;	// �\���`��
	D3DXMATRIX mtx = {};	// �}�g���b�N�X
	Color col = INITCOLOR;	// �F
	float fWidth = 0.0f;	// ��
	float fHeight = 0.0f;	// ����
	bool bZtest = false;		// Z�e�X�g�t���O
	bool bLighting = false;		// ���C�e�B���O�t���O
	bool bBillboard = false;	// �r���{�[�h�t���O
}Text3DSet;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �ݒ� ***
// �e�L�X�g(3D)�̐ݒ菈��
void SetText3D(char *pString   = NULL,
			   int nFont       = 0,
			   TEXT_DISP disp  = TEXT_DISP_CENTER,
			   D3DXMATRIX mtx  = {},
			   Color col       = INITCOLOR,
			   float fWidth    = 0.0f,
			   float fHeight   = 0.0f,
			   bool bZtest     = false,
			   bool bLighting  = false,
			   bool bBillboard = false);

#endif