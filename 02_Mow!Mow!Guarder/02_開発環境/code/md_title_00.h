//========================================
// 
// MD:�^�C�g�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_title_00.h ***
//========================================
#ifndef _MD_TITLE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_TITLE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
// �I�u�W�F�N�g�̔z�u�Ԋu
#define OBJECT_SET_SPACE	(8)

// MD:�^�C�g�����[00] �̃J����(3D)�̋���
// MD:�^�C�g�����[00] �̎��_�̍���
// MD:�^�C�g�����[00] �̒����_�̍���
// MD:�^�C�g�����[00] �̊J�n����Y
#define MD_TITLE_00_CAMERA3D_LENGTH			(120.0f)
#define MD_TITLE_00_CAMERA3D_POS_R_HEIGHT	(-0.2f)
#define MD_TITLE_00_CAMERA3D_POS_V_HEIGHT	(40.0f)
#define MD_TITLE_00_CAMERA3D_BG_SPIN		D3DXVECTOR3(0.0f,0.001f,0.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// MD:�^�C�g�����[00] �̏��
typedef enum 
{
	MD_TITLE_00_STATE_NONE,		// ����
	MD_TITLE_00_STATE_NORMAL,	// �ʏ�
	MD_TITLE_00_STATE_USERGUID,	// ���[�U�[�K�C�h
	MD_TITLE_00_STATE_RANKING,	// �����L���O
	MD_TITLE_00_STATE_SETTINGS,	// �ݒ�
	MD_TITLE_00_STATE_MAX,
}MD_TITLE_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// MD:�^�C�g�����[00] �̏��\����
typedef struct 
{
	// ��Ԋ֘A
	MD_TITLE_00_STATE	state;				// MD:�^�C�g�����[00] �̏��
	int					nCounterState;		// ��Ԃ̃J�E���^�[
	bool				bShowCopyRight;		// ���쌠�\���t���O
	int					nCounterCopyRight;	// ���쌠�\���K�E���^�[
}Md_title_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�^�C�g�����[00] �̏����擾
Md_title_00 *GetMd_title_00(void);

// MD:�^�C�g�����[00] �̏���������
// MD:�^�C�g�����[00] �̏I������
// MD:�^�C�g�����[00] �̍X�V����
// MD:�^�C�g�����[00] �̕`�揈��
void InitMd_title_00(void);
void UninitMd_title_00(void);
void UpdateMd_title_00(void);
void DrawMd_title_00(void);

// MD:�^�C�g�����[00] �̏�Ԃ�ݒ�
void SetMd_title_00State(MD_TITLE_00_STATE state);

#endif