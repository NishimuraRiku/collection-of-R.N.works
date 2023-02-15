//========================================
// 
// ���f���r���[���̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** model-viewer.h ***
//========================================
#ifndef _MODELVIEWER_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MODELVIEWER_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �\���̂̒�`
//****************************************
//========== *** �}�b�v�G�f�B�^�[��� ***
// �}�b�v�G�f�B�^�[�̏��\����
typedef struct
{
	// ��Ԋ֘A
	bool bPause;			// �|�[�Y�t���O
	bool bShowText;			// �e�L�X�g�\���t���O
	bool bShowHitTest;		// �����蔻��\���t���O
	char aNowFile[TXT_MAX];	// ���݂̃t�@�C����
}ModelViewer;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// ���f���r���[���̏���������
void InitModelViewer(void);
// ���f���r���[���̍X�V����
void UpdateModelViewer(void);
//========== *** ���o�� ***
// ���f���r���[���̃��f�����̏����ǂݍ��ݏ���
void InitLoadModelDataModelViewer(void);
//========== *** �擾 ***
// ���f���r���[���̃|�C���^���擾
ModelViewer *GetModelViewer(void);

#endif