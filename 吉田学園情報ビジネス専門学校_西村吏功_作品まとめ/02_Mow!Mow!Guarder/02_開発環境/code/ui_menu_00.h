//========================================
// 
// UI:���j���[[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_menu_00.h ***
//========================================
#ifndef UI_MENU_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define UI_MENU_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:���j���[[00] �̓��̓��[�h
typedef enum
{
	UI_MENU_00_INPUT_MODE_UP_AND_DOWN,		// �㉺
	UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT,	// ���E
	UI_MENU_00_INPUT_MODE_MAX
}UI_MENU_00_INPUT_MODE;

// UI:���j���[[00] �̎��
typedef enum
{
	UI_MENU_00_TYPE_NORMAL,		// �ʏ�
	UI_MENU_00_TYPE_SELECTION,	// �I��
	UI_MENU_00_TYPE_MAX
}UI_MENU_00_TYPE;

// UI:���j���[[00] �̏��
typedef enum
{
	UI_MENU_00_STATE_NONE,
	UI_MENU_00_STATE_IN_POP,	// �o����
	UI_MENU_00_STATE_POP,		// �o��
	UI_MENU_00_STATE_IN_CLEAR,	// ������
	UI_MENU_00_STATE_MAX
}UI_MENU_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:���j���[[00] �̊Ǘ����\����
typedef struct
{
	UI_MENU_00_STATE	state;			// ���
	int					nCounterState;	// ��Ԃ̃J�E���^�[
	Scale				scale;			// �g�嗦
	int					nNum;			// ���݂̎g�p��
	int					nSelect;		// ���݂̑I��ԍ�
	D3DXVECTOR3			centerPos;		// ���S���W
}Ui_menu_00Control;

// UI:���j���[[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3	pos;	// �ʒu

	// ���ޏ��
	UI_MENU_00_TYPE	type;	// ���
	
	// ��Ԓl
	bool	bUse;		// �g�p����Ă��邩�t���O
	bool	bSelect;	// �I������Ă��邩�t���O
	char	aString
			[TXT_MAX];	// �\�����镶����
	int		nSelectNum;	// ���݂̑I�����̔ԍ�
}Ui_menu_00;

// UI:���j���[[00] �̐ݒ���\����
typedef struct 
{
	UI_MENU_00_TYPE type;			// ���
	char			*aString;		// �\�����镶����
	bool			bDecisionSE;	// ����SE�t���O
}Ui_menu_00Set;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���j���[[00] �̏����擾
// UI:���j���[[00] �̑S�̊Ǘ��̏����擾
// UI:���j���[[00] �̌��݂̑I��ԍ����擾
Ui_menu_00 *GetUi_menu_00(void);
Ui_menu_00Control *GetUi_menu_00Control(void);
int *GetSelect(void);

// UI:���j���[[00] �̏���������
// UI:���j���[[00] �̏I������
// UI:���j���[[00] �̍X�V����
// UI:���j���[[00] �̕`�揈��
void InitUi_menu_00(void);
void UninitUi_menu_00(void);
void UpdateUi_menu_00(void);
void DrawUi_menu_00(void);

// UI:���j���[[00] �̓��͏���
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode);

// UI:���j���[[00] �̒��S���W��ݒ�
// UI:���j���[[00] �̐ݒ菈��
void SetUi_menu_00Pos(D3DXVECTOR3 pos);
void SetUi_menu_00(Ui_menu_00Set *pSet, int nNum);

#endif