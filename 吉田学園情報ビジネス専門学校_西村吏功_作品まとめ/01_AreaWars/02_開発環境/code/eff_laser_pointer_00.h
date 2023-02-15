//========================================
// *** eff_laser_pointer_00.h ***
//========================================
#ifndef _EFF_LASER_POINTER_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_LASER_POINTER_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

typedef enum
{
	EFF_LASER_POINTER_00_STATE_POP = 0,		// �o��
	EFF_LASER_POINTER_00_STATE_IN_CLEAR,	// ������
	EFF_LASER_POINTER_00_STATE_CLEAR,		// �����Ă���
}EFF_LASER_POINTER_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// ���[�U�[�|�C���^�\[00] �̏��\����
typedef struct
{
	D3DXVECTOR3					rot;			// ����
	EFF_LASER_POINTER_00_STATE	state;			// ���
	int							nCounterState;	// ��ԃJ�E���^�[
	float						fAlpha;			// �S�̂̓����x
}Eff_laser_pointer_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���[�U�[�|�C���^�\[00] �̏����擾
Eff_laser_pointer_00 *GetEff_laser_pointer_00(void);

// ���[�U�[�|�C���^�\[00] �̏���������
void InitEff_laser_pointer_00(void);

// ���[�U�[�|�C���^�\[00] �̏I������
void UninitEff_laser_pointer_00(void);

// ���[�U�[�|�C���^�\[00] �̍X�V����
void UpdateEff_laser_pointer_00(void);

// ���[�U�[�|�C���^�\[00] �̕`�揈��
void DrawEff_laser_pointer_00(void);

// ���[�U�[�|�C���^�\[00] �̏�Ԑݒ菈��
void SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE state);

#endif