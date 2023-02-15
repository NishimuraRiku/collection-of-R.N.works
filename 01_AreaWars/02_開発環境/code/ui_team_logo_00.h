//========================================
// *** ui_team_logo_00.h ***
//========================================
#ifndef _UI_TEAM_LOGO_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_TEAM_LOGO_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �`�[�����S[00] �̏��
typedef enum
{
	UI_TEAM_LOGO_00_STATE_POP = 0,	// �o��
	UI_TEAM_LOGO_00_STATE_IN_POP,	// �o����
	UI_TEAM_LOGO_00_STATE_CLEAR,	// �����Ă���
	UI_TEAM_LOGO_00_STATE_IN_CLEAR,	// ������
	UI_TEAM_LOGO_00_STATE_MAX,
}UI_TEAM_LOGO_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// �`�[�����S[00] �̏��\����
typedef struct
{
	UI_TEAM_LOGO_00_STATE	state;			// ���
	int						nCounterState;	// ��ԃJ�E���^�[
	float					fAlpha;			// �S�̂̓����x
}Ui_team_logo_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �`�[�����S[00] �̏����擾
Ui_team_logo_00 *GetUi_team_logo_00(void);

// �`�[�����S[00] �̏���������
void InitUi_team_logo_00(void);

// �`�[�����S[00] �̏I������
void UninitUi_team_logo_00(void);

// �`�[�����S[00] �̍X�V����
void UpdateUi_team_logo_00(void);

// �`�[�����S[00] �̕`�揈��
void DrawUi_team_logo_00(void);

// �`�[�����S[00] �̏�Ԑݒ菈��
void SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE state);

#endif