//========================================
// *** ui_warning_00.h ***
//========================================
#ifndef _UI_WARNING_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_WARNING_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �x��[00] �̏��
typedef enum
{
	UI_WARNING_00_STATE_POP = 0,	// �o��
	UI_WARNING_00_STATE_IN_POP,		// �o����
	UI_WARNING_00_STATE_CLEAR,		// �����Ă���
	UI_WARNING_00_STATE_IN_CLEAR,	// ������
	UI_WARNING_00_STATE_MAX,
}UI_WARNING_00_STATE;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �x��[00] �̏���������
void InitUi_warning_00(void);

// �x��[00] �̏I������
void UninitUi_warning_00(void);

// �x��[00] �̍X�V����
void UpdateUi_warning_00(void);

// �x��[00] �̕`�揈��
void DrawUi_warning_00(void);

// �x��[00] �̏�Ԑݒ菈��
void SetStateUi_warning_00(UI_WARNING_00_STATE state);

#endif