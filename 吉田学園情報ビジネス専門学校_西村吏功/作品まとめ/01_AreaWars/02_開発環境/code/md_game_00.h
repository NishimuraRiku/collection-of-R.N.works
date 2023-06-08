//========================================
// *** md_game_00.h ***
//========================================
#ifndef _MD_GAME_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_GAME_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "text.h"
#include "sys_ranking_00.h"
#include "ui_score_00.h"

//****************************************
// �}�N����` 
//****************************************

// �����̃N���W�b�g��
#define INIT_CREDIT_NUM	(2)

//****************************************
// �񋓌^�̒�`
//****************************************

// �Q�[�����[00] �̏��
typedef enum 
{
	MD_GAME_00_STATE_NONE = 0,				// �������Ă��Ȃ�
	MD_GAME_00_STATE_PAUSE,					// �|�[�Y
	MD_GAME_00_STATE_SETTING,				// �ݒ�
	MD_GAME_00_STATE_CONTINUE,				// �R���e�B�j���[
	MD_GAME_00_STATE_CONTINUE_AFTERGLOW,	// �R���e�B�j���[�̗]�C
	MD_GAME_00_STATE_POP_WAIT,				// �o���҂�
	MD_GAME_00_STATE_WAVE_WAIT,				// �E�F�[�u�҂�
	MD_GAME_00_STATE_START_WAIT,			// �X�^�[�g�҂�
	MD_GAME_00_STATE_CLEAR_WAIT,			// �N���A�҂�
	MD_GAME_00_STATE_NORMAL,				// �ʏ�
	MD_GAME_00_STATE_WARNING,				// �x��
	MD_GAME_00_STATE_GAME_CLEAR,			// �Q�[���N���A
	MD_GAME_00_STATE_GAME_OVER,				// �Q�[���I�[�o�[
	MD_GAME_00_STATE_RESULT,				// ���U���g
	MD_GAME_00_STATE_DIED_AFTERGLOW,		// ���S��̗]�C
	MD_GAME_00_STATE_END,					// �I��
	MD_GAME_00_STATE_MAX
}MD_GAME_00_STATE;

// �Q�[�����[00] �̃e�L�X�g�ԍ�
typedef enum
{
	MD_GAME_00_TEXT_SCORE = 0,		// �X�R�A
	MD_GAME_00_TEXT_SIGNAL,			// ���}
	MD_GAME_00_TEXT_PAUSE,			// �|�[�Y
	MD_GAME_00_TEXT_CONTINUE,		// �R���e�B�j���[
	MD_GAME_00_TEXT_CONTINUE_COUNT,	// �R���e�B�j���[�J�E���g
	MD_GAME_00_TEXT_PUSH_CONTINUE,	// �R���e�B�j���[����
	MD_GAME_00_TEXT_CREDIT,			// �N���W�b�g
	MD_GAME_00_TEXT_RESULT_SCORE,	// ���U���g�X�R�A
	MD_GAME_00_TEXT_LIFE_BONUS,		// ���C�t�{�[�i�X
	MD_GAME_00_TEXT_LIFE,			// ���C�t
	MD_GAME_00_TEXT_BOMB,			// �{��
	MD_GAME_00_TEXT_BOMB_MAX,		// �{���ő�
	MD_GAME_00_TEXT_BOMB_GET,		// �{���l��
	MD_GAME_00_TEXT_AREA_TIME,		// �G���A����
	MD_GAME_00_TEXT_GAME_CLEAR,		// �Q�[���N���A
	MD_GAME_00_TEXT_GAME_OVER,		// �Q�[���I�[�o�[
	MD_GAME_00_TEXT_RANKING_NAME,	// �����L���O��
	MD_GAME_00_TEXT_RANKING,		// �����L���O
	MD_GAME_00_TEXT_RANKING_MAX		// �����L���O�̍ő�
	= MD_GAME_00_TEXT_RANKING 
	+ RANKING_NUM - 1,
	MD_GAME_00_TEXT_SCORE_DISP,		// �X�R�A�\��
	MD_GAME_00_TEXT_SCORE_DISP_MAX	// �X�R�A�\���̍ő�
	= MD_GAME_00_TEXT_SCORE_DISP 
	+ UI_SCORE_00_MAX -1,
	MD_GAME_00_TEXT_MAX,
}MD_GAME_00_TEXT;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �Q�[�����[00]�̏���������
void InitMd_game_00(void);

// �Q�[�����[00]�̏I������
void UninitMd_game_00(void);

// �Q�[�����[00]�̍X�V����
void UpdateMd_game_00(void);

// �Q�[�����[00]�̕`�揈��
void DrawMd_game_00(void);

// �Q�[�����[00]�̃e�L�X�g�̏����擾
Text GetMd_game_00Text(MD_GAME_00_TEXT text);

// �Q�[���̏�Ԃ̐ݒ�
void SetMd_game_00State(MD_GAME_00_STATE state);

// �Q�[���̏�Ԃ̎擾
MD_GAME_00_STATE GetGameState(void);

// �N���W�b�g�̎擾
int *GetCredit(void);

#endif