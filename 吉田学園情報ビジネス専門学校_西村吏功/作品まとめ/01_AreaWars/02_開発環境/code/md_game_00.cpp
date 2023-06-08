//========================================
// 
// �Q�[�����[00]�̏���
// Author:���� ����
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "setting.h"
#include "sound.h"
#include "text.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "atk_explosion_01.h"
#include "bg_space_00.h"
#include "chr_enemy_00.h"
#include "chr_fighter_00.h"
#include "eff_general_00.h"
#include "eff_laser_pointer_00.h"
#include "eff_light_00.h"
#include "eff_particle_00.h"
#include "eff_screen_00.h"
#include "md_game_00.h"
#include "item_bomb_00.h"
#include "item_medal_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include "sys_ranking_00.h"
#include "sys_stage_00.h"
#include "ui_area_gage_00.h"
#include "ui_arrow_00.h"
#include "ui_bomb_00.h"
#include "ui_bomb_get_00.h"
#include "ui_frame_01.h"
#include "ui_frame_02.h"
#include "ui_life_00.h"
#include "ui_life_bonus_00.h"
#include "ui_menu_00.h"
#include "ui_score_00.h"
#include "ui_warning_00.h"
#include <stdio.h>

//****************************************
// �}�N����` 
//****************************************

// �B���R�}���h�̗L��
#define SECRET_COMMAND	(true)

// �o���҂��̎���
// ���S��̗]�C�̎���
#define POP_WAIT_TIME		(120)
#define DIED_AFTERGLOW_TIME	(120)

// �E�F�[�u�҂��̎���
// �E�F�[�u�҂��̗]�C
// �E�F�[�u�e�L�X�g�̒��Ԃ܂ł̈ړ��ɂ����鎞��
// �E�F�[�u�e�L�X�g�̏I���܂ł̈ړ����J�n���鎞��
// �E�F�[�u�e�L�X�g�̏����̈ʒu
// �E�F�[�u�e�L�X�g�̒��Ԃ̈ʒu
// �E�F�[�u�e�L�X�g�̍Ō�̈ʒu
// �E�F�[�u�e�L�X�g�̑傫���ɂ�����{��
// �E�F�[�u�e�L�X�g�̐F
#define WAVE_WAIT_TIME						(60)
#define WAVE_WAIT_AFTERGLOW					(20)
#define WAVE_TEXT_INTERMEDIATE_MOVE_TIME	(20)
#define WAVE_TEXT_END_MOVE_START_TIME		(40)
#define WAVE_TEXT_FIRST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_INTERMEDIATE_POS			(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_LAST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_SCALE						(ScaleDiameter{1.6f, 1.6f})
#define WAVE_TEXT_COLOR						(Color{255, 255, 255, 255})

// �X�^�[�g�҂��̎���
// �X�^�[�g�e�L�X�g�̈ʒu
// �X�^�[�g�e�L�X�g�̒��Ԃ܂łɂ����鎞��
// �X�^�[�g�e�L�X�g�̏I���܂ł̐��ڂ��J�n���鎞��
// �X�^�[�g�e�L�X�g�̏����̑傫���ɂ�����{��
// �X�^�[�g�e�L�X�g�̒��Ԃ̑傫���ɂ�����{��
// �X�^�[�g�e�L�X�g�̍Ō�̑傫���ɂ�����{��
// �X�^�[�g�e�L�X�g�̐F
#define START_WAIT_TIME					(60)
#define START_TEXT_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define START_TEXT_INTERMEDIATE_TIME	(20)
#define START_TEXT_END_START_TIME		(40)
#define START_TEXT_FIRST_SCALE			(ScaleDiameter{8.0f, 8.0f})
#define START_TEXT_INTERMEDIATE_SCALE	(ScaleDiameter{2.0f, 2.0f})
#define START_TEXT_LAST_SCALE			(ScaleDiameter{8.0f, 0.0f})
#define START_TEXT_COLOR				(Color{243, 191, 63, 255})

// ���U���g�X�R�A�̉��Z�ɂ����鎞��
// ���U���g�X�R�A�̉��Z��
// ���U���g�X�R�A�̕\���ɂ����鎞��
// ���U���g�X�R�A�̕\���������̐F
#define RESULT_SCORE_ADD_TIME			(3)
#define RESULT_SCORE_ADD_NUM			(17)
#define RESULT_SCORE_SHOW_TIME			(8)
#define RESULT_SCORE_SHOW_COMP_COLOR	(Color{243, 191, 63, 255})

// �����L���O�e�L�X�g�̊J�n�ʒu
// �����L���O�e�L�X�g�̏I���ʒu
// �����L���O�e�L�X�g�̈ړ��ɂ����鎞��
// �����L���O�e�L�X�g�̔z�u�ԊuX
// �����L���O�e�L�X�g�̓_�ŐF
// �����L���O�e�L�X�g�̓_�łɂ����鎞��
// �����L���O�t���[���̈ʒu
#define RANKING_TEXT_FIRST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.4f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_LAST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_MOVE_TIME		(10)
#define RANKING_TEXT_SPACE_Y		(PIXEL * 16)
#define RANKING_TEXT_BRINK_COLOR	(Color{243, 191, 63, 255})
#define RANKING_TEXT_BRINK_TIME		(4)
#define RANKING_FRAME_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 16), 0.0f))

// �|�[�Y�e�L�X�g�̓_�łɂ����鎞��
#define PAUSE_TEXT_BRINK_TIME	(16)

// �|�[�Y���j���[�̈ʒu
// ���U���g���j���[�̈ʒu
#define MD_GAME_00_PAUSE_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 80, 0.0f))
#define MD_GAME_00_RESULT_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))

// �R���e�B�j���[�̃J�E���g��
// �R���e�B�j���[�̃J�E���g�ɂ����鎞��
// �R���e�B�j���[�̗]�C�ɂ����鎞��
// �R���e�B�j���[�����Ñ��̓_�łɂ����鎞��
#define CONTINUE_COUNT_NUM			(10)
#define CONTINUE_COUNT_TIME			(60)
#define CONTINUE_AFTERGLOW_TIME		(60)
#define PUSH_CONTINUE_BRINK_TIME	(30)

// �Q�[���N���A�ɂ����鎞��
// �Q�[���I�[�o�[�ɂ����鎞��
// �Q�[���N���A�̃e�L�X�g�ɂ����鎞��
// �Q�[���I�[�o�[�̃e�L�X�g�ɂ����鎞��
#define GAME_CLEAR_TIME				(300)
#define GAME_OVER_TIME				(300)
#define GAME_CLEAR_TEXT_TIME		(12)
#define GAME_OVER_TEXT_TIME			(12)

// �m�[�_���[�W�{�[�i�X�X�R�A
// �m�[�_���[�W�{�[�i�X�X�R�A�̔�e���̌�����
#define NO_DAMAGE_BONUS_SCORE			(100000)
#define NO_DAMAGE_BONUS_SCORE_DECREASE	(20000)

// ���̑��Έʒu
#define ALLOW_RELATIVE_POSITION	(D3DXVECTOR3(PIXEL * 8 * 6.5f,PIXEL * 0,0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************

// �^�C�g�����[00] �̃��U���g���j���[
typedef enum
{
	MD_GAME_00_RESULT_MENU_RETRY = 0,		// ���g���C
	MD_GAME_00_RESULT_MENU_BACK_TO_TITLE,	// �^�C�g���ɖ߂�
	MD_GAME_00_RESULT_MENU_MAX,
}MD_GAME_00_RESULT_MENU;

// �^�C�g�����[00] �̃|�[�Y���j���[
typedef enum
{
	MD_GAME_00_PAUSE_MENU_RESUME = 0,		// �ĊJ
	MD_GAME_00_PAUSE_MENU_RETRY,			// ���g���C
	MD_GAME_00_PAUSE_MENU_SETTING,			// �ݒ�
	MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE,	// �^�C�g���ɖ߂�
	MD_GAME_00_PAUSE_MENU_MAX,
}MD_GAME_00_PAUSE_MENU;

//****************************************
// �\���̂̒�`
//****************************************

// ���}�e�L�X�g�̏��\����
typedef struct 
{
	char			aString				// 
					[TXT_MAX];			// ������
	D3DXVECTOR3		pos;				// �ʒu
	ScaleDiameter	scaleDiameter;		// �傫���ɂ�����{��
	Color			col;				// �F
}SignalText;

// �����L���O�e�L�X�g�̏��\����
typedef struct 
{
	D3DXVECTOR3 pos;			// �ʒu
	int			nCounterMove;	// �ړ��J�E���^�[
	Color		col;			// �F
}RankingText;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �e�L�X�g���e�̍X�V����
void UpdateTxtMd_game_00(void);

// �Q�[���̏�Ԃɉ������J�n����
void StartGameState(MD_GAME_00_STATE state);

// �Q�[���̏�Ԃɉ������I������
void EndGameState(MD_GAME_00_STATE state);

// �Q�[���̏�Ԃɉ������X�V����
void UpdateGameState(void);

//****************************************
// �O���[�o���錾
//****************************************
MD_GAME_00_STATE	g_gameState					//
					= MD_GAME_00_STATE_NONE;	// �Q�[���̏��
int					g_nStateCounter;			// ��Ԃ̃J�E���^�[
SignalText			g_signalText;				// ���}�e�L�X�g�̏��
int					g_nCounterReScoreAdd;		// ���U���g�X�R�A�̉��Z�J�E���^�[
int					g_nCntReScoreAdd;			// ���U���g�X�R�A�̉��Z���̃J�E���g
int					g_nCounterShow;				// �\���J�E���^�[
bool				g_bBrink;					// �_�Ńt���O
int					g_nCounterBrink;			// �_�ŃJ�E���^�[
RankingText			g_aRankingText				//
					[RANKING_NUM];				// �����L���O�̈ʒu
int					g_nUpdateRank;				// �X�V��������
int					g_nShowRanking;				// �\����������
int					g_nCredit;					// �N���W�b�g
int					g_nCntContinueEnd;			// �R���e�B�j���[�I���܂ł̃J�E���g
bool				g_bContinue;				// �R���e�B�j���[�t���O
int					g_nCntInputName;			// ���O���͂̃J�E���g
int					g_nCntText;					// �e�L�X�g�̃J�E���g
bool				g_bResultMenu;				// ���U���g���j���[�̏o���t���O

// ���U���g�̃��j���[���
Ui_menu_00_set md_game_00_result_menu_set[MD_GAME_00_RESULT_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RETRY" },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE" }
};

// �|�[�Y�̃��j���[���
Ui_menu_00_set md_game_00_pause_menu_set[MD_GAME_00_PAUSE_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RESUME" },
	{ UI_MENU_00_TYPE_NORMAL,"RETRY" },
	{ UI_MENU_00_TYPE_NORMAL,"SETTINGS" },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE" }
};

// �Q�[�����[00] �̃e�L�X�g�̏��
Text g_aMd_game_00Text[MD_GAME_00_TEXT_MAX] =
{
	{ "SCORE: %08d"			, D3DXVECTOR3(SCREEN_WIDTH - (PIXEL * 8 * 28), PIXEL * 20, 0.0f)				, {255, 255, 255, 255} , FONT_000 },	// �X�R�A
	{ ""					, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_001 },	// ���}
	{ "- P A U S E -"		, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {63 , 191, 255, 255} , FONT_000 },	// �|�[�Y
	{ "C O N T I N U E ?"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - PIXEL * 64, 0.0f)	, {63 , 191, 255, 255} , FONT_000 },	// �R���e�B�j���[
	{ "%d"					, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {63 , 191, 255, 255} , FONT_004 },	// �R���e�B�j���[�̃J�E���g
	{ ""					, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + PIXEL * 80, 0.0f)	, {231, 0  , 91 , 255} , FONT_000 },	// �R���e�B�j���[����
	{ "CREDIT : %02d"		, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + PIXEL * 64, 0.0f)	, {63 , 191, 255, 255} , FONT_000 },	// �N���W�b�g
	{ "SCORE %08d"			, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, PIXEL * 100, 0.0f)							, {255, 255, 255, 255} , FONT_001 },	// ���U���g�X�R�A
	{ "LIFE BONUS +%d"		, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_000 },	// ���C�t�{�[�i�X
	{ "LIFE:"				, D3DXVECTOR3(PIXEL * 8 * 13, PIXEL * 20, 0.0f)									, {255, 255, 255, 255} , FONT_000 },	// ���C�t
	{ "BOMB:"				, D3DXVECTOR3(PIXEL * 8 * 26, PIXEL * 20, 0.0f)									, {255, 255, 255, 255} , FONT_000 },	// �{��
	{ "MAX!"				, D3DXVECTOR3(PIXEL * 8 * (32 + CHR_FIGHTER_00_BOMB_MAX), PIXEL * 20, 0.0f)		, {243, 191, 63 , 255} , FONT_002 },	// �{���ő�
	{ "BOMB UP!"			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_002 },	// �{���l��
	{ "AREA TIME: %2d.%1d"	, D3DXVECTOR3(PIXEL * 8 * 11, (SCREEN_HEIGHT - PIXEL * 20), 0.0f)				, {255, 255, 255, 255} , FONT_000 },	// �G���A����
	{ "STAGE CLEAR"			, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {243, 191, 63 , 255} , FONT_001 },	// �Q�[���N���A
	{ "GAME OVER"			, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)					, {88 , 248, 152, 255} , FONT_001 },	// �Q�[���I�[�o�[
	{ "      %-3s"			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {243, 191, 63 , 255} , FONT_000 },	// �����L���O��
	{ " %s  %-3s %8d"		, D3DXVECTOR3(0.0f, 0.0f, 0.0f)													, {255, 255, 255, 255} , FONT_000 },	// �����L���O�X�R�A
};

// �R���e�B�j���[�̓��͍Ñ��e�L�X�g
char aContinueButtonText[ACTIVE_INPUT_TYPE_MAX][TXT_MAX] = 
{
	"- PUSH ENTER KEY -",
	"- PUSH START BUTTON -",
};

//========== *** �Q�[�����[00]�̃e�L�X�g�̏����擾 ***
Text GetMd_game_00Text(MD_GAME_00_TEXT text)
{
	return g_aMd_game_00Text[text];
}

//========== *** �Q�[���̏�Ԃ̎擾	***
MD_GAME_00_STATE GetGameState(void) 
{
	return g_gameState;
}

//========== *** �N���W�b�g�̎擾 ***
int *GetCredit(void)
{
	return &g_nCredit;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateTxtMd_game_00�֐� - �e�L�X�g���e�̍X�V���� -
//========================================
void UpdateTxtMd_game_00(void)
{
	char			aString					//
					[TXT_MAX];				// ���������p
	Chr_fighter_00	*pChr					//
					= GetChr_fighter_00();	// �@��[00] �̏��̃|�C���^

	//========== *** �e�L�X�g[���}] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SIGNAL,
			g_signalText.aString,
			g_signalText.pos,
			g_signalText.col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SIGNAL].font,
			g_signalText.scaleDiameter.fWidth,
			g_signalText.scaleDiameter.fHeight);
	}
	//========== *** �e�L�X�g[�X�R�A] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].aDisplay, pChr->nScore);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SCORE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�|�[�Y] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_PAUSE,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].aDisplay,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PAUSE].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�R���e�B�j���[] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_CONTINUE,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].aDisplay,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�R���e�B�j���[�̃J�E���g] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].aDisplay, g_nCntContinueEnd - 1);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_CONTINUE_COUNT,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CONTINUE_COUNT].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�N���W�b�g] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].aDisplay, g_nCredit);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_CREDIT,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_CREDIT].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�R���e�B�j���[����] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_PUSH_CONTINUE,
			aContinueButtonText[GetActiveInputType()],
			g_aMd_game_00Text[MD_GAME_00_TEXT_PUSH_CONTINUE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PUSH_CONTINUE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_PUSH_CONTINUE].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[���U���g�X�R�A] ***
	{
		g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col.nA = 255 * ((float)g_nCounterShow / (float)RESULT_SCORE_SHOW_TIME);
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].aDisplay, (int)(pChr->nScore * ((float)g_nCntReScoreAdd / (float)RESULT_SCORE_ADD_NUM)));
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_RESULT_SCORE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[���C�t] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].aDisplay, pChr->nLife);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_LIFE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_LIFE].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�{��] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].aDisplay, pChr->nLife);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_BOMB,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�{���ő�] ***
	{
		if ((GetChr_fighter_00()->nBomb >= CHR_FIGHTER_00_BOMB_MAX)
			&&
			((g_gameState != MD_GAME_00_STATE_PAUSE)
				&& (g_gameState != MD_GAME_00_STATE_NONE)
				&& (g_gameState != MD_GAME_00_STATE_SETTING)
				&& (g_gameState != MD_GAME_00_STATE_RESULT)))
		{// ���e�����������e�̍ő及�����ɒB���Ă������A����Ԃ��|�[�Y/����/�ݒ�/���U���g�łȂ����A
			// �e�L�X�g[�{���ő�] ��\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_MAX, true);
		}
		else 
		{// ���e�����������e�̍ő及�����ɒB���Ă��Ȃ����A
			// �e�L�X�g[�{���ő�] ���\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_MAX, false);
		}
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].aDisplay, pChr->nLife);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_BOMB_MAX,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_BOMB_MAX].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�G���A����] ***
	{
		if (GetSys_stage_00()->bShowAreaTime)
		{// �G���A���Ԃ̕\���t���O���^�̎��A
			sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].aDisplay,
				(GetSys_stage_00()->nWaveEndTime - GetSys_stage_00()->nWaveTime) / 60,
				((GetSys_stage_00()->nWaveEndTime - GetSys_stage_00()->nWaveTime) / 6)
				- (((GetSys_stage_00()->nWaveEndTime - GetSys_stage_00()->nWaveTime) / 60) * 10));
		}
		else
		{// �G���A���Ԃ̕\���t���O���U�̎��A
			sprintf(aString, "AREA TIME: --.-");
		}
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_AREA_TIME,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_AREA_TIME].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�Q�[���N���A] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay);
		int nText = (g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay), 0);
		aString[(g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_CLEAR,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�Q�[���I�[�o�[] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay);
		int nText = (g_nStateCounter / GAME_OVER_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay), 0);
		aString[(g_nStateCounter / GAME_OVER_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_OVER,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�����L���O��] ***
	{
		Sys_ranking_00	*pSys		//
			= GetSys_ranking_00();	// �����L���O[00] �̏��̃|�C���^
		char			aName		//
						[TXT_MAX];	// ���O

		// ���O����
		sprintf(aName, pSys[g_nUpdateRank].aName);

		if (g_bBrink)
		{// �_�Ńt���O���^�̎��A
 			aName[g_nCntInputName] = '\0';
		}

		g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].pos = g_aRankingText[g_nUpdateRank].pos;
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].aDisplay, aName);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_RANKING_NAME,
			aString,
			g_aRankingText[g_nUpdateRank].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING_NAME].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�����L���O] ***
	{
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			Sys_ranking_00	*pSys								//
							= GetSys_ranking_00();				// �����L���O[00] �̏��̃|�C���^
			Color			col									//
							= g_aRankingText[nCntRanking].col;	// �F
			char			aName								//
							[RANKING_NAME_NUM + 1];				// ���O

			if ((!g_bResultMenu)
				&& (nCntRanking == g_nUpdateRank))
			{// ���U���g���j���[���o�����Ă��Ȃ����A���X�V���ʂƏ��ʂ̃J�E���g����v�������A
				// �F��ݒ�
				col.nR = 255;
				col.nG = 255;
				col.nB = 255;

				// �󔒖���
				strinit(aName, RANKING_NAME_NUM);
			}
			else 
			{// ���U���g���j���[���o�����Ă��鎞�A
				// ���O����
				sprintf(aName, pSys[nCntRanking].aName);
			}

			sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].aDisplay, GetRankText(nCntRanking), aName, pSys[nCntRanking].nScore);
			UpdateTxt_00(
				TEXT_FIELD_UI,
				MD_GAME_00_TEXT_RANKING + nCntRanking,
				aString,
				g_aRankingText[nCntRanking].pos,
				col,
				TEXT_MODE_LEFT,
				g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].font,
				1.0f,
				1.0f);
		}
	}
}

//========================================
// StartGameState�֐� - �Q�[���̏�Ԃɉ������J�n���� -
//========================================
void StartGameState(MD_GAME_00_STATE state)
{
	// ��Ԃ̃J�E���^�[������������
	g_nStateCounter = 0;

	// �J�n����
	switch (g_gameState)
	{
		//========== *** �|�[�Y ***
	case MD_GAME_00_STATE_PAUSE:
	{
		// �e�L�X�g[�X�R�A]		���\��
		// �e�L�X�g[���C�t]		���\��
		// �e�L�X�g[�{��]		���\��
		// �e�L�X�g[�G���A����]	���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE		, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE		, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB		, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME	, false);

		// �e�L�X�g[�X�R�A�\��] ��S�Ĕ�\��
		for (int nCntUi = 0; nCntUi < UI_SCORE_00_MAX; nCntUi++)
		{
			if (GetUi_score_00()[nCntUi].bUse)
			{// �g�p����Ă��鎞�A
				// ��\���ɂ���
				ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi, false);
			}
		}

		if (state != MD_GAME_00_STATE_SETTING)
		{// ���̏�Ԃ��ݒ�Ŗ������A
			// �e�L�X�g[�|�[�Y]	��\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, true);

			// �_�Ńt���O�Ɠ_�ŃJ�E���^�[��������
			g_bBrink = true;
			g_nCounterBrink = 0;

			// �|�[�Y�J�nSE[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_PAUSE_START_000);
		}

		// ���j���[[00] �̐ݒ菈��
		SetUi_menu_00Pos(MD_GAME_00_PAUSE_MENU_POS);
		SetUi_menu_00(
			md_game_00_pause_menu_set,
			MD_GAME_00_PAUSE_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);
	}
	break;
		//========== *** �ݒ� ***
	case MD_GAME_00_STATE_SETTING:
	{
		// �ݒ胁�j���[�̐ݒ菈��
		SetSettingMenu(MODE_GAME_00);
	}
	break;
		//========== *** �R���e�B�j���[ ***
	case MD_GAME_00_STATE_CONTINUE:
	{
		// �e�L�X�g[�R���e�B�j���[]				��\��
		// �e�L�X�g[�R���e�B�j���[�̃J�E���g]	��\��
		// �e�L�X�g[�N���W�b�g]					��\��
		// �e�L�X�g[�R���e�B�j���[����]			��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE_COUNT, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CREDIT, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PUSH_CONTINUE, true);

		// �_�Ńt���O�Ɠ_�ŃJ�E���^�[��������
		g_bBrink = true;
		g_nCounterBrink = 0;

		// �R���e�B�j���[�I���܂ł̃J�E���g��������
		g_nCntContinueEnd = CONTINUE_COUNT_NUM;

		// �J�E���g�_�E��SE[000] ���Đ�
		PlaySound(SOUND_LABEL_SE_COUNT_DOWN_000);
	}
	break;
		//========== *** �R���e�B�j���[�̗]�C ***
	case MD_GAME_00_STATE_CONTINUE_AFTERGLOW:
	{
		if (!GetSys_stage_00()->bWarning) 
		{// �x���t���O���U�̎��A
			// �R���e�B�j���[�t���O��^�ɂ���
			g_bContinue = true;
		}
	}
	break;
		//========== *** �o���҂� ***
	case MD_GAME_00_STATE_POP_WAIT:
	{
		// �@��[00] �̐ݒ菈��
		SetChr_fighter_00(
			D3DXVECTOR3(
			(GetObj_plasma_block_00_square()->fTargetLeft + GetObj_plasma_block_00_square()->fTargetRight) * 0.5f,
				(GetObj_plasma_block_00_square()->fTargetTop + GetObj_plasma_block_00_square()->fTargetBottom) * 0.5f,
				0.0f),
			D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));

		// �e�L�X�g[�X�R�A]		��\��
		// �e�L�X�g[���C�t]		��\��
		// �e�L�X�g[�{��]		��\��
		// �e�L�X�g[�G���A����]	��\��
		// �e�L�X�g[�|�[�Y]		���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, false);
	}
	break;
		//========== *** �E�F�[�u�҂� ***
	case MD_GAME_00_STATE_WAVE_WAIT:
	{
		// �e�L�X�g[���}] ��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SIGNAL, true);

		// ���}�e�L�X�g�̐F��ݒ�
		g_signalText.col = WAVE_TEXT_COLOR;

		// �����x��0�ɂ���
		g_signalText.col.nA = 0;

		// �ʒu��ݒ�
		g_signalText.pos = WAVE_TEXT_FIRST_POS;

		// �傫���ɂ�����{����ݒ�
		g_signalText.scaleDiameter = WAVE_TEXT_SCALE;

		// �������ݒ肷��
		sprintf(g_signalText.aString, "WAVE %d", GetSys_stage_00()->nDispWaveCount);
	}
	break;
		//========== *** �X�^�[�g�҂� ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// ���}�e�L�X�g�̐F��ݒ�
		g_signalText.col = START_TEXT_COLOR;

		// �����x��0�ɂ���
		g_signalText.col.nA = 0;

		// �ʒu��ݒ�
		g_signalText.pos = START_TEXT_POS;

		// �傫���ɂ�����{����ݒ�
		g_signalText.scaleDiameter = START_TEXT_FIRST_SCALE;

		if (g_bContinue) 
		{// �R���e�B�j���[�t���O���^�̎��A
			// �R���e�B�j���[�t���O���U�ɂ���
			g_bContinue = false;

			// �������ݒ肷��
			sprintf(g_signalText.aString, "RESTART");
		}
		else 
		{// �R���e�B�j���[�t���O���U�̎��A
			// �������ݒ肷��
			sprintf(g_signalText.aString, "START");
		}
	}
	break;
		//========== *** �ʏ� ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** �x�� ***
	case MD_GAME_00_STATE_WARNING:
	{
		// �x��[00] ���o�����ɂ���
		SetStateUi_warning_00(UI_WARNING_00_STATE_IN_POP);
	}
	break;
		//========== *** �Q�[���N���A ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// �e�L�X�g[�Q�[���N���A]	��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, true);

		// �Q�[���N���ASE[000] ���Đ�
		PlaySound(SOUND_LABEL_SE_GAME_CLEAR_000);
	}
	break;
		//========== *** �Q�[���I�[�o�[ ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// �e�L�X�g[�Q�[���I�[�o�[]	��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, true);

		// �Q�[���I�[�o�[SE[000] ���Đ�
		PlaySound(SOUND_LABEL_SE_GAME_OVER_000);
	}
		break;
		//========== *** ���U���g ***
	case MD_GAME_00_STATE_RESULT:
	{
		// �\���J�E���^�[��������
		g_nCounterShow = 0;

		// ���U���g�X�R�A�̐F��������
		g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col = { 255,255,255,0 };

		// ���U���g�X�R�A�̉��Z�J�E���^�[��
		// ���U���g�X�R�A�̉��Z���̃J�E���g������������
		g_nCounterReScoreAdd = 0;
		g_nCntReScoreAdd = 0;

		if (GetSys_stage_00()->soundNow != -1)
		{// �Đ����̃T�E���h�ԍ���-1�łȂ����A
			// �Đ����̃T�E���h���~
			StopSound((SOUND_LABEL)GetSys_stage_00()->soundNow);
		}

		// �e�L�X�g[�X�R�A]			���\��
		// �e�L�X�g[���C�t]			���\��
		// �e�L�X�g[�{��]			���\��
		// �e�L�X�g[�G���A����]		���\��
		// �e�L�X�g[���U���g�X�R�A]	��\��
		// �e�L�X�g[�����L���O��]	��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RESULT_SCORE, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RANKING_NAME, true);
		
		{
			int nNoDamageBonus				//
				= NO_DAMAGE_BONUS_SCORE;	// �m�[�_���[�W�{�[�i�X

			// �m�[�_���[�W�{�[�i�X���e�J�E���g�ɉ����Č���������
			nNoDamageBonus -= GetChr_fighter_00()->nCntDamage * NO_DAMAGE_BONUS_SCORE_DECREASE;

			// �m�[�_���[�W�{�[�i�X�𐧌�
			IntControl(&nNoDamageBonus, INT_MAX, 0);

			// �X�R�A�Ƀm�[�_���[�W�{�[�i�X�����Z
			GetChr_fighter_00()->nScore += nNoDamageBonus;

			if (nNoDamageBonus > 0)
			{// �m�[�_���[�W�{�[�i�X��0�������Ă������A
				// ���C�t�{�[�i�X[00] �̐ݒ菈��
				SetUi_life_bonus_00(nNoDamageBonus);
			}
		}

		// �X�R�A��}�����A�X�V�������ʂ��擾
		g_nUpdateRank = SetScore(GetChr_fighter_00()->nScore);

		// ���̏��ʂ̖��O���󔒖��߂���
		strinit(GetSys_ranking_00()[g_nUpdateRank].aName, RANKING_NAME_NUM);

		// �����L���O�e�L�X�g�̏���������
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			// �ړ��J�E���^�[��������
			g_aRankingText[nCntRanking].nCounterMove = 0;

			// �ʒu��������
			g_aRankingText[nCntRanking].pos = RANKING_TEXT_FIRST_POS;
			g_aRankingText[nCntRanking].pos.y += RANKING_TEXT_SPACE_Y * nCntRanking;

			// �F��ݒ�
			g_aRankingText[nCntRanking].col = g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].col;
			g_aRankingText[nCntRanking].col.nA = 0;

			// �e�L�X�g[�����L���O] ��\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RANKING + nCntRanking, true);
		}

		// �\���������ʂ�������
		g_nShowRanking = 0;

		// �_�Ńt���O�Ɠ_�ŃJ�E���^�[��������
		g_bBrink = false;
		g_nCounterBrink = 0;
	}
	break;
		//========== *** ���S��̗]�C ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		if (GetSys_stage_00()->soundNow != -1)
		{// �Đ����̃T�E���h�ԍ���-1�łȂ����A
		 // �Đ����̃T�E���h���~
			StopSound((SOUND_LABEL)GetSys_stage_00()->soundNow);
		}

		// �Đ����̃T�E���h�ԍ���������
		GetSys_stage_00()->soundNow = (SOUND_LABEL)-1;
	}
	break;
		//========= *** �I�� ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
	}
}

//========================================
// EndGameState�֐� - �Q�[���̏�Ԃɉ������I������ -
//========================================
void EndGameState(MD_GAME_00_STATE state)
{
	switch (g_gameState)
	{
		//========== *** �|�[�Y ***
	case MD_GAME_00_STATE_PAUSE:
	{
		if (state != MD_GAME_00_STATE_SETTING)
		{// ���̏�Ԃ��ݒ�Ŗ������A
			// �e�L�X�g[�X�R�A]		��\��
			// �e�L�X�g[���C�t]		��\��
			// �e�L�X�g[�{��]		��\��
			// �e�L�X�g[�G���A����]	��\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, true);
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE, true);
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB, true);
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_AREA_TIME, true);

			// �e�L�X�g[�X�R�A�\��] ��S�ĕ\��
			for (int nCntUi = 0; nCntUi < UI_SCORE_00_MAX; nCntUi++)
			{
				if (GetUi_score_00()[nCntUi].bUse)
				{// �g�p����Ă��鎞�A
					ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi, true);
				}
			}

			// �e�L�X�g[�|�[�Y] ���\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, false);
		}
	}
	break;
		//========== *** �R���e�B�j���[ ***
	case MD_GAME_00_STATE_CONTINUE:
	{
		// �e�L�X�g[�R���e�B�j���[] ���\��
		// �e�L�X�g[�R���e�B�j���[�̃J�E���g] ���\��
		// �e�L�X�g[�R���e�B�j���[����] ���\��
		// �e�L�X�g[�N���W�b�g] ���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CONTINUE_COUNT, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PUSH_CONTINUE, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_CREDIT, false);
	}
	break;
		//========== *** �R���e�B�j���[�̗]�C ***
	case MD_GAME_00_STATE_CONTINUE_AFTERGLOW:
	{

	}
	break;
		//========== *** �o���҂� ***
	case MD_GAME_00_STATE_POP_WAIT:
	{

	}
	break;
		//========== *** �E�F�[�u�҂� ***
	case MD_GAME_00_STATE_WAVE_WAIT:
	{

	}
	break;
		//========== *** �X�^�[�g�҂� ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// �e�L�X�g[���}] ���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SIGNAL, false);
	}
	break;
		//========== *** �ʏ� ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** �x�� ***
	case MD_GAME_00_STATE_WARNING:
	{
		// �v���Y�}�u���b�N[00] �̈ړ��ʂ�ݒ�
		SetMoveObj_plasma_block_00();
	}
	break;
		//========== *** �Q�[���N���A ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// �e�L�X�g[�Q�[���N���A]	���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, false);
	}
	break;
		//========== *** �Q�[���I�[�o�[ ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// �e�L�X�g[�Q�[���I�[�o�[]	���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, false);
	}
	break;
		//========== *** ���U���g ***
	case MD_GAME_00_STATE_RESULT:
	{
		// �e�L�X�g[���U���g�X�R�A] ���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RESULT_SCORE, false);
	}
	break;
		//========== *** �I�� ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
		//========== *** ���S��̗]�C ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{

	}
	break;
	}
}

//========================================
// UpdateGameState�֐� - �Q�[���̏�Ԃɉ������X�V���� -
//========================================
void UpdateGameState(void)
{
	switch (g_gameState)
	{
		//========== *** �ݒ� ***
	case MD_GAME_00_STATE_SETTING:
	{
		// �ݒ胁�j���[�̍X�V����
		UpdateSettingMenu();
	}
		//========== *** �|�[�Y ***
	case MD_GAME_00_STATE_PAUSE:
	{
		// �_�ŃJ�E���^�[�����Z
		g_nCounterBrink++;

		if (g_nCounterBrink >= PAUSE_TEXT_BRINK_TIME)
		{// �_�ŃJ�E���^�[���_�łɂ����鎞�ԂɒB�������A
			// �_�ŃJ�E���^�[��������
			g_nCounterBrink = 0;

			// �_�Ńt���O��؂�ւ�
			g_bBrink ^= 1;

			// �e�L�X�g[�|�[�Y] ��\��/��\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PAUSE, g_bBrink);
		}

		if ((GetKeyboardTrigger(DIK_P))
			|| (GetButtonTrigger(BUTTON_START)))
		{// �|�[�Y�̓��͎��A
			if (GetUi_menu_00Control()->state == UI_MENU_00_STATE_POP)
			{// �o����Ԃ̎��A
				g_gameState = MD_GAME_00_STATE_PAUSE;

				// �ʏ��Ԃɂ���
				SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

				// �|�[�Y�I��SE[000] ���Đ�
				PlaySound(SOUND_LABEL_SE_PAUSE_END_000);

				// �������ɂ���
				GetUi_menu_00Control()->state = UI_MENU_00_STATE_IN_CLEAR;
				GetUi_menu_00Control()->nCounterState = UI_MENU_00_CLEAR_TIME;
			}
		}

		if (g_gameState == MD_GAME_00_STATE_SETTING) 
		{// ��Ԃ��ݒ�̎��A
			// �����𔲂���
			break;
		}

		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** �Q�[���ɖ߂� ***
		case MD_GAME_00_PAUSE_MENU_RESUME:
		{
			// �ʏ��Ԃɂ���
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

			// �|�[�Y�I��SE[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_PAUSE_END_000);
		}
		break;
			//========== *** ���g���C ***
		case MD_GAME_00_PAUSE_MENU_RETRY:
		{
			// ����SE[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_DECITION_000);

			// ��Ԃ𖳂��ɂ���
			SetMd_game_00State(MD_GAME_00_STATE_NONE);

			// ���[�h���Q�[���ɐݒ�
			SetFade(MODE_GAME_00);
		}
		break;
			//========== *** �ݒ� ***
		case MD_GAME_00_PAUSE_MENU_SETTING:
		{
			// ��Ԃ�ݒ�ɐݒ�
			SetMd_game_00State(MD_GAME_00_STATE_SETTING);
		}
		break;
			//========== *** �^�C�g���ɖ߂� ***
		case MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE:
		{
			// ��Ԃ𖳂��ɂ���
			SetMd_game_00State(MD_GAME_00_STATE_NONE);

			// ���[�h���^�C�g���ɐݒ�
			SetFade(MODE_TITLE_00);
		}
		break;
		}
	}
	break;
		//========== *** �R���e�B�j���[ ***
	case MD_GAME_00_STATE_CONTINUE:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if ((g_nStateCounter >= CONTINUE_COUNT_TIME)
			|| (GetKeyboardTrigger(DIK_SPACE))
			|| (GetButtonTrigger(BUTTON_B)))
		{// ��Ԃ̃J�E���^�[���R���e�B�j���[�̃J�E���g�ɂ����鎞�ԂɒB�������A�������̓X�L�b�v���͎��A
			// ��Ԃ̃J�E���^�[��������
			g_nStateCounter = 0;

			// �R���e�B�j���[�I���܂ł̃J�E���g�����Z
			g_nCntContinueEnd--;

			if (g_nCntContinueEnd == 0)
			{// �R���e�B�j���[�I���܂ł̃J�E���g��0�ɂȂ������A
				// �Q�[���I�[�o�[�Ɉڍs
				SetMd_game_00State(MD_GAME_00_STATE_GAME_OVER);

				// �������I��
				break;
			}
			else
			{// �R���e�B�j���[�I���܂ł̃J�E���g��0�ɂȂ��Ă��Ȃ����A
				// ���ʉ�[�J�E���g�_�E��] ���Đ�
				PlaySound(SOUND_LABEL_SE_COUNT_DOWN_000);
			}
		}

		// �_�ŃJ�E���^�[�����Z
		g_nCounterBrink++;

		if (g_nCounterBrink >= PUSH_CONTINUE_BRINK_TIME)
		{// �_�ŃJ�E���^�[���_�łɂ����鎞�ԂɒB�������A
			// �_�ŃJ�E���^�[��������
			g_nCounterBrink = 0;

			// �_�Ńt���O��؂�ւ�
			g_bBrink ^= 1;

			// �e�L�X�g[�R���e�B�j���[����] ��\��/��\��
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_PUSH_CONTINUE, g_bBrink);
		}

		if ((GetKeyboardTrigger(DIK_RETURN))
			|| GetButtonTrigger(BUTTON_START))
		{// �R���e�B�j���[���͎��A
			if (g_nCredit > 0)
			{// �N���W�b�g���c���Ă��鎞�A
				// �N���W�b�g�����Z
				g_nCredit--;

				// ���ʉ�[����] ���Đ�
				PlaySound(SOUND_LABEL_SE_DECITION_000);

				// �R���e�B�j���[����
				ContinueSys_stage_00();

				// �R���e�B�j���[�̗]�C��Ԃɂ���
				SetMd_game_00State(MD_GAME_00_STATE_CONTINUE_AFTERGLOW);
			}
			else
			{// �N���W�b�g���c���Ă��Ȃ����A
				// ���ʉ�[�Փ�] ���Đ�
				PlaySound(SOUND_LABEL_SE_COLLISION_000);
			}
		}

		if (GetKeyboardTrigger(DIK_C)
			&& GetKeyboardPress(DIK_U)
			&& GetKeyboardPress(DIK_P)
			&& SECRET_COMMAND)
		{// �B���R�}���h
			// �N���W�b�g�����Z
			g_nCredit++;

			// ����SE[000]���Đ�
			PlaySound(SOUND_LABEL_SE_METAL_000);
		}
	}
	break;
		//========== *** �R���e�B�j���[�̗]�C ***
	case MD_GAME_00_STATE_CONTINUE_AFTERGLOW:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if (g_nStateCounter >= CONTINUE_AFTERGLOW_TIME)
		{// ��Ԃ̃J�E���^�[���R���e�B�j���[�̗]�C�ɂ����鎞�ԂɒB�������A
			// �o���҂��̏�Ԃɂ���
			SetMd_game_00State(MD_GAME_00_STATE_POP_WAIT);
		}

		// �v���C���[�̈ʒu���X�V
		GetChr_fighter_00()->pos
			= (D3DXVECTOR3(
			(GetObj_plasma_block_00_square()->fTargetLeft + GetObj_plasma_block_00_square()->fTargetRight) * 0.5f,
				(GetObj_plasma_block_00_square()->fTargetTop + GetObj_plasma_block_00_square()->fTargetBottom) * 0.5f, 0.0f)
				* ((float)g_nStateCounter / (float)CONTINUE_AFTERGLOW_TIME))
			+ (GetChr_fighter_00()->pos * (1.0f - ((float)g_nStateCounter / (float)CONTINUE_AFTERGLOW_TIME)));
	}
	break;
		//========== *** �o���҂� ***
	case MD_GAME_00_STATE_POP_WAIT:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if ((g_nStateCounter >= POP_WAIT_TIME))
		{//	��Ԃ̃J�E���^�[���o���҂��̎��ԂɒB�������A
			// �Q�[���̏�Ԃ��E�F�[�u�҂��Ɉڍs
			SetMd_game_00State(MD_GAME_00_STATE_WAVE_WAIT);
		}
	}
	break;
		//========== *** �ʏ� ***
	case MD_GAME_00_STATE_NORMAL:
	{
		if ((GetKeyboardTrigger(DIK_P))
			|| (GetButtonTrigger(BUTTON_START)))
		{// �|�[�Y���͎��A
		 // �|�[�Y��Ԃɂ���
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);
		}
	}
	break;
		//========== *** �E�F�[�u�҂� ***
	case MD_GAME_00_STATE_WAVE_WAIT:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if (g_nStateCounter <= WAVE_TEXT_INTERMEDIATE_MOVE_TIME)
		{// ��Ԃ̃J�E���^�[�����Ԃ܂ł̈ړ��ɂ����鎞�Ԉȉ��̎��A
			// �ʒu���X�V
			g_signalText.pos = (WAVE_TEXT_FIRST_POS * (1.0f - ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME)))
				+ (WAVE_TEXT_INTERMEDIATE_POS * ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME));

			// �����x���X�V
			g_signalText.col.nA = 255 * ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME);
		}

		if ((g_nStateCounter >= WAVE_TEXT_END_MOVE_START_TIME)
			&& (g_nStateCounter <= WAVE_WAIT_TIME))
		{// ��Ԃ̃J�E���^�[���I���܂ł̈ړ����J�n���鎞�Ԉȏ�̎��A
			// �ʒu���X�V
			g_signalText.pos = (WAVE_TEXT_INTERMEDIATE_POS
				* (1.0f - ((float)(g_nStateCounter - WAVE_TEXT_END_MOVE_START_TIME) / (float)(WAVE_WAIT_TIME - WAVE_TEXT_END_MOVE_START_TIME))))
				+ (WAVE_TEXT_LAST_POS * ((float)(g_nStateCounter - WAVE_TEXT_END_MOVE_START_TIME) / (float)(WAVE_WAIT_TIME - WAVE_TEXT_END_MOVE_START_TIME)));

			// �����x���X�V
			g_signalText.col.nA = 255 * (1.0f - ((float)(g_nStateCounter - WAVE_TEXT_END_MOVE_START_TIME) / (float)(WAVE_WAIT_TIME - WAVE_TEXT_END_MOVE_START_TIME)));
		}

		if ((g_nStateCounter >= WAVE_WAIT_TIME + WAVE_WAIT_AFTERGLOW))
		{//	��Ԃ̃J�E���^�[���E�F�[�u�҂��̎��ԂɒB�������A
			// �Q�[���̏�Ԃ��X�^�[�g�҂��Ɉڍs
			SetMd_game_00State(MD_GAME_00_STATE_START_WAIT);
		}
	}
	break;
		//========== *** �X�^�[�g�҂� ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if (g_nStateCounter <= START_TEXT_INTERMEDIATE_TIME)
		{// ��Ԃ̃J�E���^�[�����Ԃ܂łɂ����鎞�Ԉȉ��̎��A
			// �傫���ɂ�����{�����X�V
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_FIRST_SCALE.fWidth * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fWidth * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_FIRST_SCALE.fHeight * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fHeight * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));

			// �����x���X�V
			g_signalText.col.nA = 255
				* ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME);
		}

		if (g_nStateCounter >= START_TEXT_END_START_TIME)
		{// ��Ԃ̃J�E���^�[���I���܂ł̈ړ����J�n���鎞�Ԉȏ�̎��A
			// �傫���ɂ�����{�����X�V
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_INTERMEDIATE_SCALE.fWidth
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fWidth * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_INTERMEDIATE_SCALE.fHeight
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fHeight * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));

			// �����x���X�V
			g_signalText.col.nA = 255
				* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
		}

		if ((g_nStateCounter >= START_WAIT_TIME))
		{//	��Ԃ̃J�E���^�[���X�^�[�g�҂��̎��ԂɒB�������A
			// �Q�[���̏�Ԃ�ʏ�Ɉڍs
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}
	}
	break;
		//========== *** �Q�[���N���A ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		if (g_nStateCounter >= GAME_CLEAR_TIME) 
		{// ��Ԃ̃J�E���^�[���Q�[���N���A�ɂ����鎞�ԂɒB�������A
			// ��Ԃ����U���g�Ɉڍs
			SetMd_game_00State(MD_GAME_00_STATE_RESULT);
		}
		else if ((g_nStateCounter % GAME_CLEAR_TEXT_TIME == 0)
			&&((g_nStateCounter / GAME_CLEAR_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay)))
		{// ��Ԃ̃J�E���^�[���Q�[���N���A�̃e�L�X�g�ɂ����鎞�ԂŊ���؂�鎞�A
			// �e�L�X�g�J�E���gSE[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_TEXT_COUNT_000);
		}

		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;
	}
	break;
		//========== *** �Q�[���I�[�o�[ ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		if (g_nStateCounter >= GAME_OVER_TIME)
		{// ��Ԃ̃J�E���^�[���Q�[���I�[�o�[�ɂ����鎞�ԂɒB�������A
			// ��Ԃ����U���g�Ɉڍs
			SetMd_game_00State(MD_GAME_00_STATE_RESULT);
		}
		else if ((g_nStateCounter % GAME_OVER_TEXT_TIME == 0)
			&& ((g_nStateCounter / GAME_OVER_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay)))
		{// ��Ԃ̃J�E���^�[���Q�[���I�[�o�[�̃e�L�X�g�ɂ����鎞�ԂŊ���؂�鎞�A
			// �e�L�X�g�J�E���gSE[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_TEXT_COUNT_000);
		}

		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;
	}
	break;
		//========== *** ���U���g ***
	case MD_GAME_00_STATE_RESULT:
	{
		if (g_nCounterShow < RESULT_SCORE_SHOW_TIME)
		{// �\���J�E���^�[�����U���g�X�R�A�̕\���ɂ����鎞�Ԃ�������Ă��鎞�A
			// �\���J�E���^�[�����Z
			g_nCounterShow++;
		}

		if (g_nCntReScoreAdd < RESULT_SCORE_ADD_NUM)
		{// ���U���g�X�R�A�̉��Z���̃J�E���g�����U���g�X�R�A�̉��Z����������Ă��鎞�A
			// ���U���g�X�R�A�̉��Z�J�E���^�[�����Z
			g_nCounterReScoreAdd++;

			if (g_nCounterReScoreAdd >= RESULT_SCORE_ADD_TIME)
			{// ���U���g�X�R�A�̉��Z�J�E���^�[�����U���g�X�R�A�̉��Z�ɂ����鎞�ԂɒB�������A
				// ���U���g�X�R�A�̉��Z�J�E���^�[��������
				g_nCounterReScoreAdd = 0;

				if (GetChr_fighter_00()->nScore != 0)
				{// �X�R�A��0�łȂ����A
					// ���U���g�X�R�A�̉��Z���̃J�E���g�����Z
					g_nCntReScoreAdd++;

					// �X�R�A�J�E���gSE[000] ���Đ�
					PlaySound(SOUND_LABEL_SE_SCORE_COUNT_000);
				}
				else
				{// �X�R�A��0�̎��A
					// ���U���g�X�R�A�̉��Z���̃J�E���g���ő�ɂ���
					g_nCntReScoreAdd = RESULT_SCORE_ADD_NUM;
				}

				if (g_nCntReScoreAdd >= RESULT_SCORE_ADD_NUM)
				{// ���U���g�X�R�A�̉��Z���̃J�E���g�����U���g�X�R�A�̉��Z���ɒB�������A
					// ���U���g�X�R�A�̐F��\���������̐F�ɐݒ肷��
					g_aMd_game_00Text[MD_GAME_00_TEXT_RESULT_SCORE].col = RESULT_SCORE_SHOW_COMP_COLOR;

					// �t���[��[02] �̐ݒ菈��
					SetUi_frame_02(RANKING_FRAME_POS);
				}
			}
		}
		else
		{// ���U���g�X�R�A�̉��Z���̃J�E���g�����U���g�X�R�A�̉��Z���ɒB�������A
			if (g_nUpdateRank != -1)
			{// ���ʂ��X�V����Ă��鎞�A
				// �_�ł̃J�E���^�[�����Z
				g_nCounterBrink++;

				if (g_nCounterBrink >= RANKING_TEXT_BRINK_TIME)
				{// �_�ł̃J�E���^�[���_�łɂ����鎞�ԂɒB�������A
					// �_�ł̃J�E���^�[��������
					g_nCounterBrink = 0;

					// �_�Ńt���O���X�V
					g_bBrink ^= 1;

					if (g_bBrink)
					{// �_�Ńt���O���^�̎��A
						// �_�Ŏ��̐F�ɐݒ�
						int nTempAlpha = g_aRankingText[g_nUpdateRank].col.nA;
						g_aRankingText[g_nUpdateRank].col = RANKING_TEXT_BRINK_COLOR;
						g_aRankingText[g_nUpdateRank].col.nA = nTempAlpha;
					}
					else
					{// �_�Ńt���O���U�̎��A
						// �ʏ펞�̐F�ɐݒ�
						int nTempAlpha = g_aRankingText[g_nUpdateRank].col.nA;
						g_aRankingText[g_nUpdateRank].col = g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].col;
						g_aRankingText[g_nUpdateRank].col.nA = nTempAlpha;
					}
				}
			}

			if (g_nShowRanking < RANKING_NUM)
			{// �S�Ẵ����L���O��\�����I���Ă��Ȃ����A
				// �ړ��̃J�E���^�[�����Z
				g_aRankingText[g_nShowRanking].nCounterMove++;

				// �����x��ݒ�
				g_aRankingText[g_nShowRanking].col.nA =
					255 * ((float)g_aRankingText[g_nShowRanking].nCounterMove / RANKING_TEXT_MOVE_TIME);

				// �ʒu���X�V
				g_aRankingText[g_nShowRanking].pos =
					(RANKING_TEXT_FIRST_POS * (1.0f - ((float)g_aRankingText[g_nShowRanking].nCounterMove / (float)RANKING_TEXT_MOVE_TIME)))
					+ (RANKING_TEXT_LAST_POS * ((float)g_aRankingText[g_nShowRanking].nCounterMove / (float)RANKING_TEXT_MOVE_TIME));
				g_aRankingText[g_nShowRanking].pos.y += g_nShowRanking * RANKING_TEXT_SPACE_Y;

				if (g_aRankingText[g_nShowRanking].nCounterMove >= RANKING_TEXT_MOVE_TIME)
				{// �ړ��̃J�E���^�[���ړ��ɂ����鎞�ԂɒB�������A
					// �\���������ʂ����Z
					g_nShowRanking++;

					// �����L���O�Z�b�gSE[000] ���Đ�
					PlaySound(SOUND_LABEL_SE_RANKING_SET_000);
				}
			}
			else if ((g_nCntInputName < RANKING_NAME_NUM)
				&& (g_nUpdateRank != -1))
			{// ���O���͂̃J�E���g�������L���O���̕�������������Ă��鎞�A
				if ((GetKeyboardRepeat(DIK_DOWN))
					|| (GetKeyboardRepeat(DIK_S))
					|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
					|| (GetButtonRepeat(BUTTON_DOWN)))
				{// ���ɓ��͂��ꂽ���A
					// �e�L�X�g�̃J�E���g�����Z
					g_nCntText--;

					// �I��SE[001] ���Đ�
					PlaySound(SOUND_LABEL_SE_SELECT_001);
				}
				else if ((GetKeyboardRepeat(DIK_UP))
					|| (GetKeyboardRepeat(DIK_W))
					|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
					|| (GetButtonRepeat(BUTTON_UP)))
				{// ��ɓ��͂��ꂽ���A
					// �e�L�X�g�̃J�E���g�����Z
					g_nCntText++;

					// �I��SE[001] ���Đ�
					PlaySound(SOUND_LABEL_SE_SELECT_001);
				}

				// �e�L�X�g�̃J�E���g�����[�v����
				IntLoopControl(&g_nCntText, 'Z' + 1, '0');
				
				if (((GetKeyboardTrigger(DIK_BACKSPACE))
					||
					(GetButtonTrigger(BUTTON_B)))
					&&
					(g_nCntInputName > 0))
				{// �폜�����͂��ꂽ���A�����O���͂̃J�E���g��0�������Ă��鎞�A
					// ���O���͂̃J�E���g�����Z
					g_nCntInputName--;

					// �폜SE[000] ���Đ�
					PlaySound(SOUND_LABEL_SE_DELETION_000);

					// ���݂̃J�E���g�ȍ~�̕������󔒂ɂ���
					GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName + 1] = '\0';

					// �e�L�X�g�̃J�E���g��ݒ�
					g_nCntText = GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName];
				}
				else if ((GetKeyboardTrigger(DIK_RETURN))
					|| (GetButtonTrigger(BUTTON_A)))
				{// ���肪���͂��ꂽ���A
					// ���O���͂̃J�E���g�����Z
					g_nCntInputName++;

					// ����SE[001] ���Đ�
					PlaySound(SOUND_LABEL_SE_DECITION_001);

					// �e�L�X�g�̃J�E���g��������
					g_nCntText = 'A';
				}
				else
				{// ���肪���͂���Ă��Ȃ����A

					if (g_nCntText == 'Z' + 1) 
					{// �e�L�X�g�̃J�E���g��Z+���������A
						// ���O�ɋ󔒂���
						GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName] = ' ';
					}
					else 
					{// �e�L�X�g�̃J�E���g��Z+�łȂ����A
						// ���O�ɕ�������
						GetSys_ranking_00()[g_nUpdateRank].aName[g_nCntInputName] = g_nCntText;
					}
				}

				// ���[00] ���g�p���Ă����Ԃɂ���
				GetUi_arrow_00()->bUse = true;

				// ���[00] �̈ʒu��ݒ�
				GetUi_arrow_00()->pos = D3DXVECTOR3(
					RANKING_TEXT_LAST_POS.x + g_nCntInputName * GetFont()[g_aMd_game_00Text[MD_GAME_00_TEXT_RANKING].font].fSpaceX,
					RANKING_TEXT_LAST_POS.y + g_nUpdateRank * RANKING_TEXT_SPACE_Y,
					0.0f) + ALLOW_RELATIVE_POSITION;
			}
			else if(!g_bResultMenu)
			{// ���U���g���j���[�o���t���O���U�̎��A
				// ���U���g���j���[�o���t���O��^�ɂ���
				g_bResultMenu = true;

				// ���[00] ���g�p���Ă��Ȃ���Ԃɂ���
				GetUi_arrow_00()->bUse = false;

				// �e�L�X�g[�����L���O��] ���\���ɂ���
				ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_RANKING_NAME, false);

				// ���j���[�̒��S���W��ݒ�
				SetUi_menu_00Pos(MD_GAME_00_RESULT_MENU_POS);

				// ���j���[[00] �̐ݒ菈��
				SetUi_menu_00(
					md_game_00_result_menu_set,
					MD_GAME_00_RESULT_MENU_MAX,
					UI_MENU_00_DEFAULT_SPACE_X,
					UI_MENU_00_DEFAULT_SPACE_Y);
			}
			else if (GetFade() == FADE_NONE)
			{// �S�Ẵ����L���O��\�����I���Ă��āA���t�F�[�h���������A
				// ���j���[�̓��͏���
				switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
				{
					//========== *** ���g���C ***
				case MD_GAME_00_RESULT_MENU_RETRY:
				{
					// ����SE[000] ���Đ�
					PlaySound(SOUND_LABEL_SE_DECITION_000);

					// ���[�h���Q�[���ɐݒ�
					SetFade(MODE_GAME_00);
				}
				break;
					//========== *** �^�C�g���ɖ߂� ***
				case MD_GAME_00_RESULT_MENU_BACK_TO_TITLE:
				{
					// ���[�h���^�C�g���ɐݒ�
					SetFade(MODE_TITLE_00);
				}
				break;
				}
			}
		}
	}
	break;
		//========== *** ���S��̗]�C ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if (g_nStateCounter >= DIED_AFTERGLOW_TIME)
		{// ��Ԃ̃J�E���^�[�����S��̗]�C�ɂ����鎞�ԂɒB�������A
		 // �R���e�B�j���[�Ɉڍs
			SetMd_game_00State(MD_GAME_00_STATE_CONTINUE);
		}
	}
	break;
		//========== *** �I�� ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_game_00�֐� - �Q�[�����[00]�̏��������� -
//========================================
void InitMd_game_00(void)
{
	g_gameState		= MD_GAME_00_STATE_NONE;	// �Q�[���̏�Ԃ�������
	g_nCredit		= INIT_CREDIT_NUM;			// �N���W�b�g��������
	g_bContinue		= false;					// �R���e�B�j���[�t���O���U�ɂ���
	g_nCntInputName = 0;						// ���O���͂̃J�E���g��������
	g_nCntText		= 'A';						// �e�L�X�g�̃J�E���g��������
	g_bResultMenu	= false;					// ���U���g���j���[�o���t���O���U�ɂ���

	// ����������
	InitTxt_00();				// �e�L�X�g
	InitAtk_bullet_00();		// ATK:�e					[00]
	InitAtk_explosion_00();		// ATK:����					[00]
	InitAtk_explosion_01();		// ATK:����					[01]
	InitBg_space_00();			// BG :�F��					[00]
	InitChr_enemy_00();			// CHR:�G					[00]
	InitChr_fighter_00();		// CHR:�@��					[00]
	InitEff_general_00();		// EFF:�G�t�F�N�g�S��		[00]
	InitEff_laser_pointer_00();	// EFF:���[�U�[�|�C���^�\	[00]
	InitEff_light_00();			// EFF:���C�g				[00]
	InitEff_particle_00();		// EFF:�p�[�e�B�N��			[00]
	InitEff_screen_00();		// EFF:�X�N���[��			[00]
	InitItem_bomb_00();			// ITM:���e					[00]
	InitItem_medal_00();		// ITM:���_��				[00]
	InitObj_plasma_block_00();	// OBJ:�v���Y�}�u���b�N		[00]
	InitSys_stage_00(0);		// SYS;�X�e�[�W				[00]
	InitUi_area_gage_00();		// UI :�G���A�Q�[�W			[00]
	InitUi_arrow_00();			// UI :���					[00]
	InitUi_bomb_00();			// UI :�{��					[00]
	InitUi_bomb_get_00();		// UI :�{���l��				[00]
	InitUi_frame_01();			// UI :�t���[��				[01]
	InitUi_frame_02();			// UI :�t���[��				[02]
	InitUi_life_00();			// UI :�̗�					[00]
	InitUi_life_bonus_00();		// UI :���C�t�{�[�i�X		[00]
	InitUi_menu_00();			// UI :���j���[				[00]
	InitUi_score_00();			// UI :�X�R�A				[00]
	InitUi_warning_00();		// UI :�x��					[00]
	InitObj_plasma_wall_00();	// OBJ:�v���Y�}�E�H�[��		[00]

	// �Q�[���̏�Ԃ��o���҂��ɐݒ�
	SetMd_game_00State(MD_GAME_00_STATE_POP_WAIT);
}

//========================================
// UninitMd_game_00�֐� - �Q�[�����[00]�̏I������ -
//========================================
void UninitMd_game_00(void)
{
	// �I������
	UninitTxt_00();					// �e�L�X�g
	UninitAtk_bullet_00();			// ATK:�e					[00]
	UninitAtk_explosion_00();		// ATK:����					[00]
	UninitAtk_explosion_01();		// ATK:����					[01]
	UninitBg_space_00();			// BG :�F��					[00]
	UninitChr_enemy_00();			// CHR:�G					[00]
	UninitChr_fighter_00();			// CHR:�@��					[00]
	UninitEff_general_00();			// EFF:�G�t�F�N�g�S��		[00]
	UninitEff_laser_pointer_00();	// EFF:���[�U�[�|�C���^�\	[00]
	UninitEff_light_00();			// EFF:���C�g				[00]
	UninitEff_screen_00();			// EFF:�X�N���[��			[00]
	UninitItem_bomb_00();			// ITM:���e					[00]
	UninitItem_medal_00();			// ITM:���_��				[00]
	UninitSys_stage_00();			// SYS:�X�e�[�W				[00]
	UninitUi_area_gage_00();		// UI :�G���A�Q�[�W			[00]
	UninitUi_arrow_00();			// UI :���					[00]
	UninitUi_bomb_00();				// UI :�{��					[00]
	UninitUi_bomb_get_00();			// UI :�{���l��				[00]
	UninitUi_frame_01();			// UI :�t���[��				[01]
	UninitUi_frame_02();			// UI :�t���[��				[02]
	UninitUi_life_00();				// UI :�̗�					[00]
	UninitUi_life_bonus_00();		// UI :���C�t�{�[�i�X		[00]
	UninitUi_menu_00();				// UI :���j���[				[00]
	UninitUi_score_00();			// UI :�X�R�A				[00]
	UninitUi_warning_00();			// UI :�x��					[00]
	UninitObj_plasma_block_00();	// OBJ:�v���Y�}�u���b�N		[00]
	UninitObj_plasma_wall_00();		// OBJ:�v���Y�}�E�H�[��		[00]
}

//========================================
// UpdateMd_game_00�֐� - �Q�[�����[00]�̍X�V���� -
//========================================
void UpdateMd_game_00(void)
{
	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_NONE)
		&& (g_gameState != MD_GAME_00_STATE_SETTING))
	{// ��Ԃ��|�[�Y/����/�ݒ�łȂ����A
		// �X�V����
		UpdateObj_plasma_block_00_square();	// OBJ:�v���Y�}�u���b�N		[00] ���`���l�p�`
		UpdateChr_fighter_00();				// CHR:�@��					[00]
		UpdateObj_plasma_block_00();		// OBJ:�v���Y�}�u���b�N		[00]
		UpdateObj_plasma_wall_00();			// OBJ:�v���Y�}�E�H�[��		[00]
		UpdateChr_enemy_00();				// CHR:�G					[00]
		UpdateAtk_bullet_00();				// ATK:�e					[00]
		UpdateAtk_explosion_00();			// ATK:����					[00]
		UpdateAtk_explosion_01();			// ATK:����					[01]
		UpdateBg_space_00();				// BG :�F��					[00]
		UpdateEff_general_00();				// EFF:�G�t�F�N�g�S��		[00]
		UpdateEff_particle_00();			// EFF:�p�[�e�B�N��			[00]
		UpdateEff_laser_pointer_00();		// EFF:���[�U�[�|�C���^�\	[00]
		UpdateEff_light_00();				// EFF:���C�g				[00]
		UpdateEff_screen_00();				// EFF:�X�N���[��			[00]
		UpdateItem_bomb_00();				// ITM:���e					[00]
		UpdateItem_medal_00();				// ITM:���_��				[00]
		UpdateSys_stage_00();				// SYS:�X�e�[�W				[00]
		UpdateUi_area_gage_00();			// UI :�G���A�Q�[�W			[00]
		UpdateUi_arrow_00();				// UI :���					[00]
		UpdateUi_bomb_00();					// UI :�{��					[00]
		UpdateUi_bomb_get_00();				// UI :�{���l��				[00]
		UpdateUi_frame_02();				// UI :�t���[��				[02]
		UpdateUi_life_00();					// UI :�̗�					[00]
		UpdateUi_life_bonus_00();			// UI :���C�t�{�[�i�X		[00]
		UpdateUi_score_00();				// UI :�X�R�A				[00]
		UpdateUi_warning_00();				// UI :�x��					[00]
	}

	// ���j���[[00] �̍X�V����
	UpdateUi_menu_00();

	// �Q�[���̏�Ԃɉ������X�V����
	UpdateGameState();

	// �e�L�X�g���e�̍X�V����
	UpdateTxtMd_game_00();
}

//========================================
// DrawMd_game_00�֐� - �Q�[�����[00]�̕`�揈�� -
//========================================
void DrawMd_game_00(void)
{
	// �`�揈��
	if (g_gameState != MD_GAME_00_STATE_RESULT)
	{// ��Ԃ����U���g�łȂ����A
		DrawUi_frame_01();		// UI :�t���[��					[01]
	}
	DrawBg_space_00();			// BG :�F��						[00]
	DrawObj_plasma_wall_00();	// OBJ:�v���Y�}�E�H�[��			[00]
	DrawObj_plasma_block_00();	// OBJ:�v���Y�}�u���b�N			[00]
	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_NONE)
		&& (g_gameState != MD_GAME_00_STATE_SETTING))
	{// ��Ԃ��|�[�Y/����/�ݒ�łȂ����A
		DrawChr_enemy_00();			// CHR:�G					[00]
		DrawEff_laser_pointer_00();	// EFF:���[�U�[�|�C���^�\	[00]
		DrawChr_fighter_00();		// CHR:�@��					[00]
		DrawEff_light_00();			// EFF:���C�g				[00]
		DrawEff_general_00();		// EFF:�G�t�F�N�g�S��		[00]
		DrawAtk_bullet_00();		// ATK:�e					[00]
		DrawAtk_explosion_00();		// ATK:����					[00]
		DrawAtk_explosion_01();		// ATK:����					[01]
		DrawItem_bomb_00();			// ITM:���e					[00]
		DrawItem_medal_00();		// ITM:���_��				[00]
	}
	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_NONE)
		&& (g_gameState != MD_GAME_00_STATE_SETTING)
		&& (g_gameState != MD_GAME_00_STATE_RESULT))
	{// ��Ԃ��|�[�Y/����/�ݒ�/���U���g�łȂ����A
		DrawUi_bomb_00();		// UI :�{��						[00]
		DrawUi_life_00();		// UI :�̗�						[00]
		DrawUi_area_gage_00();	// UI :�G���A�Q�[�W				[00]
	}
	DrawUi_warning_00();		// UI :�x��						[00]
	DrawUi_frame_02();			// UI :�t���[��					[02]
	DrawUi_menu_00();			// UI :���j���[					[00]
	DrawTxt_00();				// �e�L�X�g
	DrawUi_arrow_00();			// UI :���						[00]
	DrawEff_screen_00();		// EFF:�X�N���[��				[00]
}

//========================================
// SetGameState�֐� - �Q�[���̏�Ԃ̐ݒ� -
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state) 
{
	MD_GAME_00_STATE	stateTemp		// 
						= g_gameState;	// ���̏�Ԃ��ꎞ�I�Ɋi�[

	// �Q�[���̏�Ԃɉ������I������
	EndGameState(state);

	// �Q�[���̏�Ԃ𔽉f
	g_gameState = state;

	// �Q�[���̏�Ԃɉ������J�n����
	StartGameState(stateTemp);
}
