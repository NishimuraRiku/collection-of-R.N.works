//========================================
// 
// MD:�`���[�g���A�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_tutorial_00.h ***
//========================================
#ifndef _MD_TUTORIAL_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_TUTORIAL_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
// �I�u�W�F�N�g�̔z�u�Ԋu
#define OBJECT_SET_SPACE	(8)

// MD:�`���[�g���A�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̊J�n����
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH	(280.0f)
// MD:�`���[�g���A�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̏I������
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_END_LENGTH	(240.0f)
// MD:�`���[�g���A�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̊g��ɂ����鎞��
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME		(60)
// MD:�`���[�g���A�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̒����_�̍���
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_POS_R_HEIGHT	(4.0f)
// MD:�`���[�g���A�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̎��_�̍���
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_POS_V_HEIGHT	(0.35f)
// MD:�`���[�g���A�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̌���
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_ROT			(D3DX_PI * -0.85f)

// MD:�`���[�g���A�����[00] �̈ʒuY
#define MD_TUTORIAL_00_BG_POS_Y	(-40.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// MD:�`���[�g���A�����[00] �̏��
typedef enum 
{
	MD_TUTORIAL_00_STATE_NONE,			// ����
	MD_TUTORIAL_00_STATE_NORMAL,		// �ʏ�
	MD_TUTORIAL_00_STATE_PAUSE,			// �|�[�Y
	MD_TUTORIAL_00_STATE_USERGUID,		// ���[�U�[�K�C�h
	MD_TUTORIAL_00_STATE_SETTING,		// �ݒ�
	MD_TUTORIAL_00_STATE_RANKING,		// �����L���O
	MD_TUTORIAL_00_STATE_RESULT,		// ���U���g
	MD_TUTORIAL_00_STATE_TIPS,			// TIPS�҂�
	MD_TUTORIAL_00_STATE_TUTORIAL_00,	// �`���[�g���A��[00]
	MD_TUTORIAL_00_STATE_MAX,
}MD_TUTORIAL_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// MD:�`���[�g���A�����[00] �̏��\����
typedef struct 
{
	// ��Ԋ֘A
	MD_TUTORIAL_00_STATE	state;			// MD:�`���[�g���A�����[00] �̏��
	int					nCounterState;	// ��Ԃ̃J�E���^�[
	D3DXVECTOR3			previousPosR;	// �O�̒����_�̈ʒu
}Md_tutorial_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�`���[�g���A�����[00] �̏����擾
Md_tutorial_00 *GetMd_tutorial_00(void);

// MD:�`���[�g���A�����[00] �̏���������
// MD:�`���[�g���A�����[00] �̏I������
// MD:�`���[�g���A�����[00] �̍X�V����
// MD:�`���[�g���A�����[00] �̕`�揈��
void InitMd_tutorial_00(void);
void UninitMd_tutorial_00(void);
void UpdateMd_tutorial_00(void);
void DrawMd_tutorial_00(void);

// MD:�`���[�g���A�����[00] �̏�Ԃ�ݒ�
void SetMd_tutorial_00State(MD_TUTORIAL_00_STATE state);

#endif