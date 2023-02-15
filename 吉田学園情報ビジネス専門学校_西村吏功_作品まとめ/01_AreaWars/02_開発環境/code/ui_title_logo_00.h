//========================================
// *** ui_title_logo_00.h ***
//========================================
#ifndef _UI_TITLE_LOGO_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_TITLE_LOGO_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �^�C�g�����S[00] �̏��
typedef enum
{
	UI_TITLE_LOGO_00_STATE_POP = 0,					// �o��
	UI_TITLE_LOGO_00_STATE_IN_POP,					// �o����
	UI_TITLE_LOGO_00_STATE_CLEAR,					// �����Ă���
	UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE,		// �v���Y�}�E�H�[���̈ړ�
	UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT,	// �v���Y�}�E�H�[���̈ړ��҂�
	UI_TITLE_LOGO_00_STATE_MAX,
}UI_TITLE_LOGO_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// �l�p�`�̏��\����
typedef struct
{
	float fTop;
	float fBottom;
	float fLeft;
	float fRight;
}Square;

// �^�C�g�����S[00] �̏��\����
typedef struct
{
	UI_TITLE_LOGO_00_STATE	state;			// ���
	int						nCounterState;	// ��ԃJ�E���^�[
	float					fAlpha;			// �S�̂̓����x
	D3DXVECTOR3				pos;			// �ʒu
	bool					bActive;		// ���I�t���O
	Square					square;			// �l�p�`�̏��
	int						nCntBrink;		// �_�ŃJ�E���g
	int						nCounterBrink;	// �_�ŃJ�E���^�[
}Ui_title_logo_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �^�C�g�����S[00] �̏����擾
Ui_title_logo_00 *GetUi_title_logo_00(void);

// �^�C�g�����S[00] �̏���������
void InitUi_title_logo_00(void);

// �^�C�g�����S[00] �̏I������
void UninitUi_title_logo_00(void);

// �^�C�g�����S[00] �̍X�V����
void UpdateUi_title_logo_00(void);

// �^�C�g�����S[00] �̕`�揈��
void DrawUi_title_logo_00(void);

// �^�C�g�����S[00] �̏�Ԑݒ菈��
void SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE state);

#endif