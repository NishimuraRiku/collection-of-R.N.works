//========================================
// 
// CHR:���f���̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_model.h ***
//========================================
#ifndef _CHR_MODEL_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHR_MODEL_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// _R.N.Lib
#include "../R.N.Lib/RNmain.h"

//****************************************
// �\���̂̒�`
//****************************************
// CHR:���f���̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// ���݂̈ʒu
	D3DXVECTOR3 rot;	// ����

	// ���ފ֘A
	int nType;	// ���

	// ���i�֘A
	Parts3DInfo partsInfo;	// ���i�Ǘ�
	int nMotion;			// ���[�V����
}Chr_model;
// CHR:���f���̊Ǘ����\����
typedef struct
{
	int nTypeNum;		// ��ސ�
	bool bLoadModel;	// ���f����ǂݍ��߂����t���O
}Chr_modelControl;
// CHR:���f���̎�ޖ��̏��\����
typedef struct
{
	Parts3DSet partsSet;	// ���i�ݒ���(3D)
	HitTestSet hitTestSet;	// �����蔻��ݒ���
	Motion3DSet motionSet;	// ���[�V�����ݒ���
}Chr_modelType;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// CHR:���f���̏���������
void InitChr_model(void);
// CHR:���f���̏I������
void UninitChr_model(void);
// CHR:���f���̍X�V����
void UpdateChr_model(void);
// CHR:���f���̕`�揈��
void DrawChr_model(void);
//========== *** ���o�� ***
// CHR:���f���̓ǂݍ��ݏ���
void LoadChr_model(char *pPath);
//========== *** �擾 ***
// CHR:���f���̏��̃|�C���^���擾
Chr_model *GetChr_model(void);
// CHR:���f���̊Ǘ����̃|�C���^���擾
Chr_modelControl *GetChr_modelControl(void);
// CHR:���f���̎�ޖ��̏����擾
Chr_modelType *GetChr_modelType(void);

#endif