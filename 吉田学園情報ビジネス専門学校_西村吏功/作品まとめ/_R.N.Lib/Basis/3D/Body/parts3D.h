//========================================
// 
// ���i(3D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** parts3D.h ***
//========================================
#ifndef _PARTS3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _PARTS3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "../../../RNmain.h"

//****************************************
// �\���̒�`
//****************************************
// ���i(3D)�̎�ޖ��̏��\����
typedef struct {
	// �ǂݍ��ݏ��
	int nIdx                     = 0;				// �����̔ԍ�
	int nModelIdx                = DATANONE;		// ���f���̔ԍ�
	int nParent                  = DATANONE;		// �e�ԍ�
	D3DXVECTOR3 fixedRelativePos = INITD3DXVECTOR3;	// �Œ�̑��Έʒu
	D3DXVECTOR3 fixedRelativeRot = INITD3DXVECTOR3;	// �Œ�̑��Ίp�x
	D3DXVECTOR3 rotRelativePos   = INITD3DXVECTOR3;	// ��]���̑��Έʒu
	bool bLighting               = true;			// ���C�e�B���O�t���O
}Parts3DType;

// ���i�ݒ���\����
typedef struct {
	float fScale            = 1.0f;	// �g��{��
	int nPartsNum           = 0;	// ���i��
	Parts3DType *pPartsType = NULL;	// ���i�̎�ޖ��̏��
}Parts3DSet;

//****************************************
// �v���g�^�C�v�錾
//****************************************
/* ���i�ݒ���(3D)�̓ǂݍ��ݏ��� */void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);
/* ���i�ݒ���(3D)�̏������ݏ��� */void SaveParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);

#endif