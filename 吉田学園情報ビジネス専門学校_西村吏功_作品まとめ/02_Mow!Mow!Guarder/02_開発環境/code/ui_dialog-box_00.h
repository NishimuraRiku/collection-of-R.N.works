//========================================
// 
// UI:�_�C�A���O�{�b�N�X[00] �̃w�b�_�t�@�C��
// Author:���� ����
// 
//========================================
// *** ui_dialog-box_00.h ***
//========================================
#ifndef _UI_DIALOGBOX_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_DIALOGBOX_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �}�N����`
//****************************************
// UI:�_�C�A���O�{�b�N�X[00] �̈ʒu
#define UI_DIALOGBOX_00_POS	D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y+(PIXEL*64),0.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:�_�C�A���O�{�b�N�X[00] �̏��
typedef enum
{
	UI_DIALOGBOX_00_STATE_POP,		// �o��
	UI_DIALOGBOX_00_STATE_IN_POP,	// �o����
	UI_DIALOGBOX_00_STATE_CLEAR,	// �����Ă���
	UI_DIALOGBOX_00_STATE_IN_CLEAR,	// ������
	UI_DIALOGBOX_00_STATE_MAX,
}UI_DIALOGBOX_00_STATE;

// UI:�_�C�A���O�{�b�N�X[00] �̎��
typedef enum
{
	UI_DIALOGBOX_00_TYPE_NONE = -1,	// ����
	UI_DIALOGBOX_00_TYPE_00,		// 00
	UI_DIALOGBOX_00_TYPE_01,		// 01
	UI_DIALOGBOX_00_TYPE_02,		// 02
	UI_DIALOGBOX_00_TYPE_03,		// 03
	UI_DIALOGBOX_00_TYPE_04,		// 04
	UI_DIALOGBOX_00_TYPE_05,		// 05
	UI_DIALOGBOX_00_TYPE_06,		// 06
	UI_DIALOGBOX_00_TYPE_07,		// 07
	UI_DIALOGBOX_00_TYPE_MAX,
}UI_DIALOGBOX_00_TYPE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:�_�C�A���O�{�b�N�X[00] �̏��\����
typedef struct
{
	UI_DIALOGBOX_00_TYPE	type;			// ���
	UI_DIALOGBOX_00_STATE	state;			// ���
	int						nCounterState;	// ��ԃJ�E���^�[
	float					fAlpha;			// �S�̂̓����x
	Scale					scale;			// �g��{��
}Ui_dialogBox_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�_�C�A���O�{�b�N�X[00] �̏����擾
Ui_dialogBox_00 *GetUi_dialogBox_00(void);

// UI:�_�C�A���O�{�b�N�X[00] �̏���������
// UI:�_�C�A���O�{�b�N�X[00] �̏I������
// UI:�_�C�A���O�{�b�N�X[00] �̍X�V����
// UI:�_�C�A���O�{�b�N�X[00] �̕`�揈��
void InitUi_dialogBox_00(void);
void UninitUi_dialogBox_00(void);
void UpdateUi_dialogBox_00(void);
void DrawUi_dialogBox_00(void);

// UI:�_�C�A���O�{�b�N�X[00] �̏�Ԑݒ菈��
// UI:�_�C�A���O�{�b�N�X[00] �̐ݒ菈��
void SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE state);
void SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE type);

#endif