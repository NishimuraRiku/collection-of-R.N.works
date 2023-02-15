//========================================
// *** ui_menu_00.h ***
//========================================
#ifndef UI_MENU_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define UI_MENU_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//========== �}�N����`			==========// 
#define UI_MENU_00_DEFAULT_SPACE_X	(PIXEL * 0)		// ���j���[[00] �̔z�u�ԊuX
#define UI_MENU_00_DEFAULT_SPACE_Y	(PIXEL * 16)	// ���j���[[00] �̔z�u�ԊuY
#define UI_MENU_00_CLEAR_TIME		(4)				// ���j���[[00] ��������̂ɂ����鎞��

//========== �񋓌^�̒�`		==========// 

// ���j���[[00] �̓��̓��[�h
typedef enum
{
	UI_MENU_00_INPUT_MODE_UP_AND_DOWN = 0,
	UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT,
	UI_MENU_00_INPUT_MODE_MAX
}UI_MENU_00_INPUT_MODE;

// ���j���[[00] �̎��
typedef enum
{
	UI_MENU_00_TYPE_NORMAL = 0,	// �ʏ�
	UI_MENU_00_TYPE_SELECTION,	// �I��
	UI_MENU_00_TYPE_MAX
}UI_MENU_00_TYPE;

// ���j���[[00] �̏��
typedef enum
{
	UI_MENU_00_STATE_NONE = 0,
	UI_MENU_00_STATE_IN_POP,	// �o����
	UI_MENU_00_STATE_POP,		// �o��
	UI_MENU_00_STATE_IN_CLEAR,	// ������
	UI_MENU_00_STATE_MAX
}UI_MENU_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// ���j���[[00] �̑S�̊Ǘ��\����
typedef struct
{
	UI_MENU_00_STATE	state;			// ���
	int					nCounterState;	// ��Ԃ̃J�E���^�[
	ScaleDiameter		scale;			// �g�嗦
}Ui_menu_00Control;

// ���j���[[00] �̏��\����
typedef struct
{
											//========== *** �ʒu�֘A ***
	D3DXVECTOR3			pos;				// �ʒu
	D3DXVECTOR3			rot;				// ����
	float				fWidth;				// ��
	float				fHeight;			// ����
											//========== *** ���ޏ�� ***
	UI_MENU_00_TYPE		type;				// ���
	bool				bUse;				// �g�p����Ă��邩�t���O
	bool				bSelect;			// �I������Ă��邩�t���O
											//========== *** ��Ԓl ***
	char				aString[TXT_MAX];	// �\�����镶����
	int					nSelectNum;			// ���݂̑I�����̔ԍ�
											//========== *** �e�N�X�`���֘A ***
	int					nCounterAnim;		// �A�j���[�V�����̃J�E���^�[
	int					nAnimChangeTime;	// �A�j���[�V�����̐؂�ւ�����
	int					nPattern;			// �p�^�[��No.
}Ui_menu_00;

// ���j���[[00] �̐ݒ�p�̏��\����
typedef struct 
{
	UI_MENU_00_TYPE type;	// ���
	char *aString;			// �\�����镶����
}Ui_menu_00_set;

//========== �v���g�^�C�v�錾	==========// 
void InitUi_menu_00(void);															// ���j���[[00] �̏���������
void UninitUi_menu_00(void);														// ���j���[[00] �̏I������
void UpdateUi_menu_00(void);														// ���j���[[00] �̍X�V����
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode);									// ���j���[[00] �̓��͏���
void DrawUi_menu_00(void);															// ���j���[[00] �̕`�揈��
void SetUi_menu_00Pos(D3DXVECTOR3 pos);												// ���j���[[00] �̒��S���W��ݒ�
void SetUi_menu_00(Ui_menu_00_set *set, int nMax, float fSpaseX, float fSpaceY);	// ���j���[[00] �̐ݒ菈��
Ui_menu_00 *GetUi_menu_00(void);													// ���j���[[00] �̏����擾
Ui_menu_00Control *GetUi_menu_00Control(void);										// ���j���[[00] �̑S�̊Ǘ��̏����擾
int *GetSelect(void);																// ���j���[[00] �̌��݂̑I��ԍ����擾

#endif