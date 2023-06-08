//========================================
// 
// ���[�V����(3D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** motion3D.h ***
//========================================
#ifndef _MOTION3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MOTION3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "parts3D.h"

//****************************************
// �񋓌^��`
//****************************************
// ���[�V����(3D)�R�}���h
typedef enum {
	MOTION3D_COMMAND_LABEL_MOVE,	// �ړ�
	MOTION3D_COMMAND_LABEL_SPIN,	// ��]
	MOTION3D_COMMAND_LABEL_STEP,	// ����
	MOTION3D_COMMAND_LABEL_MAX,
}MOTION3D_COMMAND_LABEL;

//****************************************
// �\���̒�`
//****************************************
// ���[�V����(3D)�R�}���h���
typedef struct {
	char *pLabelName;	// ���x����
	int nDataNum;		// �����̐�
}Motion3DCommandInfo;

// ���[�V�����R�}���h���\����
typedef struct {
	int nTime = 0;	// ����
	MOTION3D_COMMAND_LABEL command = (MOTION3D_COMMAND_LABEL)0;	// �R�}���h�̎��
	float *pData = NULL;	// ����
}Motion3DCommand;

// ���i���̃��[�V����(3D)���\����
typedef struct {
	int nCommandNum = 0;	// �R�}���h��
	Motion3DCommand *pCommand = NULL;	// �R�}���h���
}PartsMotion3D;

// ���[�V����(3D)���\����
typedef struct {
	int nLoopTime = 0;	// ���[�v����
	bool bLoop = false;	// ���[�v���邩�t���O
	int nPartsMotionNum = 0;	// ���i���̃��[�V�����̐�
	PartsMotion3D *pPartsMotion = NULL;	// ���i���̃��[�V�������
}Motion3D;

// ���[�V����(3D)�̓ǂݍ��ݏ��\����
typedef struct {
	char *pMotionPath = NULL;	// ���[�V����(3D)�̃p�X
	bool bReload = false;	// �ēǂݍ��݃t���O
	int nMotionIdx = -1;	// ���[�V����(3D)�ԍ�
#if /* �ݒ�t�H�[�}�b�g */0
	// ���[�V����(3D)�̓ǂݍ��ݏ��
	Motion3DLoad motion3DLoad;
	/* ���[�V�����̃p�X */motion3DLoad.pMotionPath = ;
	/* �ēǂݍ��݃t���O */motion3DLoad.bReload = ;
	/* ���[�V�����ԍ�   */motion3DLoad.nMotionIdx = ;
	// ���[�V����(3D)��ǂݍ���
	= LoadMotion3D(motion3DLoad);
#endif
}Motion3DLoad;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ���[�V����(3D)�֘A ***
// ���[�V����(3D)�̓ǂݍ��ݏ���
int LoadMotion3D(Motion3DLoad motionLoad);
// ���[�V����(3D)�������ݏ���
void SaveMotion3D(char *pPath, int nMotionIdx);
//========== *** ��{�ڑ� ***
// ���[�V����(3D)�̏I������
void UninitMotion3D(void);
//========== *** �擾 ***
// ���[�V����(3D)�̏����擾
Motion3D *GetMotion3D(void);
// ���[�V����(3D)�̃R�}���h���̏����擾
Motion3DCommandInfo GetMotion3DCommandInfo(MOTION3D_COMMAND_LABEL cmd);
//========== *** ���̑� ***
// ���[�V����(3D)�̏��̃�������������
void InitMotion3DMemory(int nMotion);

#endif