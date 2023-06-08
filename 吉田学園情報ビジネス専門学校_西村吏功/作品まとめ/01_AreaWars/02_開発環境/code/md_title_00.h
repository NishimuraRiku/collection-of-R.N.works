//========================================
// *** md_title_00.h ***
//========================================
#ifndef _MD_TITLE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_TITLE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "sys_ranking_00.h"

//****************************************
// �}�N����`
//****************************************

// �^�C�g�����[00] �̃^�C�g���A�j���[�V�����ɂ����鎞��
#define MD_TITLE_00_TITLE_ANIM_TIME	(80)

//****************************************
// �񋓌^�̒�`
//****************************************

// �^�C�g�����[00] �̏��
typedef enum 
{
	MD_TITLE_00_STATE_NONE = 0,		// �������Ă��Ȃ�
	MD_TITLE_00_STATE_TEAM_LOGO,	// �`�[�����S
	MD_TITLE_00_STATE_TITLE_ANIM,	// �^�C�g���A�j���[�V����
	MD_TITLE_00_STATE_FLASH,		// �t���b�V��
	MD_TITLE_00_STATE_NORMAL,		// �ʏ�
	MD_TITLE_00_STATE_USER_GUIDE,	// �������
	MD_TITLE_00_STATE_RANKING,		// �����L���O
	MD_TITLE_00_STATE_SETTING,		// �ݒ�
	MD_TITLE_00_STATE_MAX,
}MD_TITLE_00_STATE;

// �Q�[�����[00] �̃e�L�X�g�ԍ�
typedef enum
{
	MD_TITLE_00_TEXT_COPYRIGHT = 0,	// ���쌠�\��
	MD_TITLE_00_TEXT_CREDIT,		// �N���W�b�g
	MD_TITLE_00_TEXT_RANKING,		// �����L���O
	MD_TITLE_00_TEXT_RANKING_MAX	// �����L���O�̍ő�
	= MD_TITLE_00_TEXT_RANKING
	+ RANKING_NUM - 1,
	MD_TITLE_00_TEXT_MAX,
}MD_TITLE_00_TEXT;

//****************************************
// �\���̂̒�`
//****************************************

// �^�C�g�����[00] �̏��\����
typedef struct 
{
	// ��Ԋ֘A
	MD_TITLE_00_STATE	state;			// �^�C�g�����[00] �̏��
	int					nCounterState;	// �^�C�g�����[00] �̏�ԃJ�E���^�[
	bool				bPlayBGM;		// BGM�Đ��t���O
	bool				bShowRanking;	// �����L���O�\���t���O
}Md_title_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �^�C�g�����[00] �̏����擾
Md_title_00 *GetMd_title_00(void);

// �^�C�g�����[00] �̏���������
void InitMd_title_00(void);

// �^�C�g�����[00] �̏I������
void UninitMd_title_00(void);

// �^�C�g�����[00] �̍X�V����
void UpdateMd_title_00(void);

// �^�C�g�����[00] �̕`�揈��
void DrawMd_title_00(void);

// �^�C�g�����[00] �̏�Ԃ�ݒ�
void SetMd_title_00State(MD_TITLE_00_STATE state);

#endif