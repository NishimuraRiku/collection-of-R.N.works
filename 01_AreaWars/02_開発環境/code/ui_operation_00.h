//========================================
// *** ui_operation_00.h ***
//========================================
#ifndef _UI_OPERATION_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_OPERATION_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �������[00] �̏��
typedef enum
{
	UI_OPERATION_00_STATE_POP = 0,	// �o��
	UI_OPERATION_00_STATE_IN_POP,	// �o����
	UI_OPERATION_00_STATE_CLEAR,	// �����Ă���
	UI_OPERATION_00_STATE_IN_CLEAR,	// ������
	UI_OPERATION_00_STATE_MAX,
}UI_OPERATION_00_STATE;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �������[00] �̏���������
void InitUi_operation_00(void);

// �������[00] �̏I������
void UninitUi_operation_00(void);

// �������[00] �̍X�V����
void UpdateUi_operation_00(void);

// �������[00] �̕`�揈��
void DrawUi_operation_00(void);

// �������[00] �̏�Ԑݒ菈��
void SetStateUi_operation_00(UI_OPERATION_00_STATE state);

#endif