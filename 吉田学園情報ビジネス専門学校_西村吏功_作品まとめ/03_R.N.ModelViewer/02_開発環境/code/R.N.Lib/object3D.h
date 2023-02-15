//========================================
// 
// �I�u�W�F�N�g3D�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** object3D.h ***
//========================================
#ifndef _OBJECT3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJECT3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// _R.N.Lib
#include "RNmain.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"

//****************************************
// �}�N����`
//****************************************
// �I�u�W�F�N�g3D�̍ő吔
#define OBJECT3D_MAX (128)

//****************************************
// �\���̂̒�`
//****************************************
// �I�u�W�F�N�g3D�̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// ���݂̈ʒu
	D3DXVECTOR3 rot;	// ����

	// ���ފ֘A
	int nType;	// ���

	// ��Ԋ֘A
	bool bUse;	// �g�p����Ă���t���O

	// ���i�֘A
	Parts3DInfo partsInfo;	// ���i�Ǘ�
	int	 nMotion;			// ���[�V����
}Object3D;
// �I�u�W�F�N�g3D�̊Ǘ����\����
typedef struct
{
	int nTypeNum;	// ��ސ�
}Object3DControl;
// �I�u�W�F�N�g3D�̎�ޖ��̏��\����
typedef struct
{
	Parts3DSet partsSet;	// ���i�ݒ���(3D)
	Motion3DSet motionSet;	// ���[�V�����ݒ���
}Object3DType;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �I�u�W�F�N�g3D�̏���������
void InitObject3D(void);
// �I�u�W�F�N�g3D�̏I������
void UninitObject3D(void);
// �I�u�W�F�N�g3D�̍X�V����
void UpdateObject3D(void);
// �I�u�W�F�N�g3D�̕`�揈��
void DrawObject3D(void);
//========== *** �`�� ***
// �I�u�W�F�N�g3D�̗�O�`�揈��
void ExcDrawObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, Color col);
//========== *** ���o�� ***
// �I�u�W�F�N�g3D�̓ǂݍ��ݏ���
void LoadObject3D(void);
//========== *** �擾 ***
// �I�u�W�F�N�g3D�̏����擾
Object3D *GetObject3D(void);
// �I�u�W�F�N�g3D�̊Ǘ������擾
Object3DControl *GetObject3DControl(void);
// �I�u�W�F�N�g3D�̎�ޖ��̏����擾
Object3DType *GetObject3DType(void);
//========== *** �ݒ� ***
// �I�u�W�F�N�g3D�̐ݒ菈��
void SetObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
//========== *** ��� ***
// �I�u�W�F�N�g3D�̃N���A����
void ClearObject3D(void);

#endif