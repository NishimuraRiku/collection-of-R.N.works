//========================================
// 
// UI:TIPS[00] �̃w�b�_�t�@�C��
// Author:���� ����
// 
//========================================
// *** ui_tips_00.h ***
//========================================
#ifndef _UI_TIPS_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_TIPS_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "input.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:TIPS[00] �̏��
typedef enum
{
	UI_TIPS_00_STATE_POP,		// �o��
	UI_TIPS_00_STATE_IN_POP,	// �o����
	UI_TIPS_00_STATE_CLEAR,		// �����Ă���
	UI_TIPS_00_STATE_IN_CLEAR,	// ������
	UI_TIPS_00_STATE_SWITCH,	// �؂�ւ�
	UI_TIPS_00_STATE_MAX,
}UI_TIPS_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:TIPS[00] �̏��\����
typedef struct
{
	int nType;				// ���
	int nStep;				// �X�e�b�v
	bool bStepViewAll;		// �X�e�b�v��S�ĉ{�������t���O
	bool bStepViewAllOld;	// �X�e�b�v��S�ĉ{�������t���O(�ߋ�)
	UI_TIPS_00_STATE state;	// ���
	int nCounterState;		// ��ԃJ�E���^�[
	float fAlpha;			// �S�̂̓����x
	Scale scale;			// �g��{��
}Ui_tips_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:TIPS[00] �̏����擾
Ui_tips_00 *GetUi_tips_00(void);
// UI:TIPS[00] �̐ؑւ��擾
bool GetSwitchUi_tips_00(void);

// UI:TIPS[00] �̏���������
void InitUi_tips_00(void);
// UI:TIPS[00] �̏I������
void UninitUi_tips_00(void);
// UI:TIPS[00] �̍X�V����
void UpdateUi_tips_00(void);
// UI:TIPS[00] �̕`�揈��
void DrawUi_tips_00(void);

// UI:TIPS[00] �̏�Ԑݒ菈��
void SetStateUi_tips_00(UI_TIPS_00_STATE state);
// UI:TIPS[00] �̐ݒ菈��
void SetUi_tips_00(int nType);

#endif