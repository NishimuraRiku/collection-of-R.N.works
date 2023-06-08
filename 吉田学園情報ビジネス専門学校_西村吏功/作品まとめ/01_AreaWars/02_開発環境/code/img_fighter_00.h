//========================================
// *** img_fighter_00.h ***
//========================================
#ifndef _IMG_FIGHTER_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _IMG_FIGHTER_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �@��[00] �̏��
typedef enum
{
	IMG_FIGHTER_00_STATE_POP = 0,		// �o��
	IMG_FIGHTER_00_STATE_IN_POP,		// �o����
	IMG_FIGHTER_00_STATE_MAX,
}IMG_FIGHTER_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// �@��[00] �̏��\����
typedef struct
{
	IMG_FIGHTER_00_STATE	state;				// ���
	int						nCounterState;		// ��ԃJ�E���^�[
	float					fAlpha;				// �S�̂̓����x
	D3DXVECTOR3				pos;				// �ʒu
	int						nCounterFireBrink;	// ���̓_�ŃJ�E���^�[
	bool					bFireBrink;			// ���̓_�Ńt���O
}Img_fighter_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �@��[00] �̏����擾
Img_fighter_00 *GetImg_fighter_00(void);

// �@��[00] �̏���������
void InitImg_fighter_00(void);

// �@��[00] �̏I������
void UninitImg_fighter_00(void);

// �@��[00] �̍X�V����
void UpdateImg_fighter_00(void);

// �@��[00] �̕`�揈��
void DrawImg_fighter_00(void);

// �@��[00] �̏�Ԑݒ菈��
void SetStateImg_fighter_00(IMG_FIGHTER_00_STATE state);

#endif