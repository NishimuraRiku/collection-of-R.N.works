//========================================
// 
// MD:�Q�[�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game_00.h ***
//========================================
#ifndef _MD_GAME_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_GAME_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
// �I�u�W�F�N�g�̔z�u�Ԋu
#define OBJECT_SET_SPACE	(8)

// MD:�Q�[�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̊J�n����
// MD:�Q�[�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̏I������
// MD:�Q�[�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̊g��ɂ����鎞��
// MD:�Q�[�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̒����_�̍���
// MD:�Q�[�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̎��_�̍���
// MD:�Q�[�����[00] �̃`���[�g���A��[00] �̃J����(3D)�̌���
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH	(160.0f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_END_LENGTH	(120.0f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME		(60)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_R_HEIGHT	(4.0f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_V_HEIGHT	(0.25f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ROT			(D3DX_PI)

// MD:�Q�[�����[00] �̃`���[�g���A��[01] �ɂ����鎞��
// MD:�Q�[�����[00] �̃`���[�g���A��[01] �̃J����(3D)�̊J�n����
// MD:�Q�[�����[00] �̃`���[�g���A��[01] �̃J����(3D)�̏I������
// MD:�Q�[�����[00] �̃`���[�g���A��[01] �̃J����(3D)�̒����_�̍���
// MD:�Q�[�����[00] �̃`���[�g���A��[01] �̃J����(3D)�̎��_�̍���
// MD:�Q�[�����[00] �̃`���[�g���A��[01] �̃J����(3D)�̊J�n����Y
// MD:�Q�[�����[00] �̃`���[�g���A��[01] �̃J����(3D)�̏I������Y
#define MD_GAME_00_STATE_TUTORIAL_01_TIME					(3000)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_LENGTH		(160.0f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_R_HEIGHT	(4.0f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_V_HEIGHT	(0.25f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_START_ROT_Y	(D3DX_PI*-0.5f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_END_ROT_Y		(D3DX_PI*0.5f)

// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(�g��)�ɂ����鎞��
// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(�g��)�̃J����(3D)�̊J�n����
// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(�g��)�̃J����(3D)�̏I������
// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(�g��)�̃J����(3D)�̒����_�̍���
// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(�g��)�̃J����(3D)�̎��_�̍���
// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(�g��)�̃J����(3D)�̊J�n����Y
// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(�g��)�̃J����(3D)�̏I������Y
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_TIME					(180)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_START_LENGTH	(200.0f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_END_LENGTH	(80.0f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_R_HEIGHT	(4.0f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_V_HEIGHT	(0.5f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_START_ROT_Y	(D3DX_PI * -1.25f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_END_ROT_Y		(D3DX_PI * -0.75f)

// MD:�Q�[�����[00] �̈ʒuY
#define MD_GAME_00_BG_POS_Y	(-40.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// MD:�Q�[�����[00] �̏��
typedef enum 
{
	MD_GAME_00_STATE_NONE,								// ����
	MD_GAME_00_STATE_NORMAL,							// �ʏ�
	MD_GAME_00_STATE_PAUSE,								// �|�[�Y
	MD_GAME_00_STATE_USERGUID,							// ���[�U�[�K�C�h
	MD_GAME_00_STATE_SETTING,							// �ݒ�
	MD_GAME_00_STATE_CLEAR,								// �N���A���o
	MD_GAME_00_STATE_RANKING,							// �����L���O
	MD_GAME_00_STATE_RESULT,							// ���U���g
	MD_GAME_00_STATE_TUTORIAL_00,						// �`���[�g���A��[00]
	MD_GAME_00_STATE_TUTORIAL_01,						// �`���[�g���A��[01]
	MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE,		// �{�X�o�����[�r�[(���_�ړ�)
	MD_GAME_00_STATE_BOSS_POP_MOVIE_POP,				// �{�X�o�����[�r�[(�o��)
	MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET,	// �{�X�o�����[�r�[(���_���Z�b�g)
	MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM,			// �R�A�j�󃀁[�r�[(�g��)
	MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION,		// �R�A�j�󃀁[�r�[(����)
	MD_GAME_00_STATE_MAX,
}MD_GAME_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// MD:�Q�[�����[00] �̏��\����
typedef struct 
{
	// ��Ԋ֘A
	MD_GAME_00_STATE	state;			// MD:�Q�[�����[00] �̏��
	int					nCounterState;	// ��Ԃ̃J�E���^�[
	D3DXVECTOR3			previousPosR;	// �O�̒����_�̈ʒu
}Md_game_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�Q�[�����[00] �̏����擾
Md_game_00 *GetMd_game_00(void);

// MD:�Q�[�����[00] �̏���������
// MD:�Q�[�����[00] �̏I������
// MD:�Q�[�����[00] �̍X�V����
// MD:�Q�[�����[00] �̕`�揈��
void InitMd_game_00(void);
void UninitMd_game_00(void);
void UpdateMd_game_00(void);
void DrawMd_game_00(void);

// MD:�Q�[�����[00] �̏�Ԃ�ݒ�
void SetMd_game_00State(MD_GAME_00_STATE state);

#endif