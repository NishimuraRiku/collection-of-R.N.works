//========================================
// 
// �^�C�g�����[00]�̏���
// Author:���� ����
// 
//========================================
// *** md_title_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "text.h"
#include "sound.h"
#include "setting.h"
#include "bg_color.h"
#include "bg_space_01.h"
#include "md_game_00.h"
#include "md_title_00.h"
#include "img_fighter_00.h"
#include "chr_fighter_00.h"
#include "ui_frame_02.h"
#include "ui_menu_00.h"
#include "ui_operation_00.h"
#include "ui_team_logo_00.h"
#include "ui_title_logo_00.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************

// �^�C�g�����[00] �̏����̔w�i�F
// �^�C�g�����[00] �̒ʏ펞�̔w�i�F
// �^�C�g�����[00] �̃t���b�V�����̔w�i�F
#define MD_TITLE_00_INIT_BG_COLOR	{0,0,0,255}
#define MD_TITLE_00_NORMAL_BG_COLOR	{36,36,56,255}
#define MD_TITLE_00_FLASH_BG_COLOR	{255,255,255,255}

// �^�C�g�����[00] �̃t���b�V���ɂ����鎞��
#define MD_TITLE_00_FLASH_TIME	(20)

// �^�C�g�����[00] �̃��C�����j���[�̈ʒu
// �^�C�g�����[00] �̑���������j���[�̈ʒu
// �^�C�g�����[00] �̃����L���O���j���[�̈ʒu
#define MD_TITLE_00_MAIN_MENU_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))
#define MD_TITLE_00_USER_GUIDE_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 32, 0.0f))
#define MD_TITLE_00_RANKING_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 32, 0.0f))

// �����L���O�e�L�X�g�̊�ʒu
// �����L���O�e�L�X�g�̔z�u�ԊuY
// �����L���O�t���[���̈ʒu
#define RANKING_TEXT_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (PIXEL * 8 * 9), PIXEL * 180, 0.0f))
#define RANKING_TEXT_SPACE_Y	(PIXEL * 16)
#define RANKING_FRAME_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 8 * 7), 0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************

// �^�C�g�����[00] �̃��j���[
typedef enum
{
	MD_TITLE_00_MENU_MAIN = 0,		// ���C��
	MD_TITLE_00_MENU_USER_GUIDE,	// �������
	MD_TITLE_00_MENU_RANKING,		// �����L���O
	MD_TITLE_00_MENU_MAX,
}MD_TITLE_00_MENU;

// �^�C�g�����[00] �̃��C�����j���[
typedef enum
{
	MD_TITLE_00_MAIN_MENU_START = 0,	// �J�n
	MD_TITLE_00_MAIN_MENU_USER_GUIDE,	// �������
	MD_TITLE_00_MAIN_MENU_RANKING,		// �����L���O
	MD_TITLE_00_MAIN_MENU_SETTINGS,		// �ݒ�
	MD_TITLE_00_MAIN_MENU_EXIT,			// �I��
	MD_TITLE_00_MAIN_MENU_MAX
}MD_TITLE_00_MAIN_MENU;

// �^�C�g�����[00] �̑���������j���[
typedef enum
{
	MD_TITLE_00_USER_GUIDE_MENU_OK = 0,	// ����
	MD_TITLE_00_USER_GUIDE_MENU_MAX
}MD_TITLE_00_USER_GUIDE_MENU;

// �^�C�g�����[00] �̃����L���O���j���[
typedef enum
{
	MD_TITLE_00_RANKING_MENU_OK = 0,	// ����
	MD_TITLE_00_RANKING_MENU_MAX
}MD_TITLE_00_RANKING_MENU;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �^�C�g�����[00] �̃p�����[�^�[�̏���������
void InitParameterMd_title_00(void);

// �^�C�g�����[00] �̏�Ԃɉ������X�V����
void UpdateMd_title_00State(void);

// �^�C�g�����[00] �̏�Ԃɉ������J�n����
void StartMd_title_00State(void);

// �^�C�g�����[00] �̏�Ԃɉ������I������
void EndMd_title_00State(void);

//****************************************
// �O���[�o���錾
//****************************************
Md_title_00	g_md_title_00;	// �^�C�g�����[00] �̏��

// ���C�����j���[�̐ݒ���
Ui_menu_00_set g_aUi_menu_00_set[MD_TITLE_00_MENU_MAX][MD_TITLE_00_MAIN_MENU_MAX] =
{
	{
		{ UI_MENU_00_TYPE_NORMAL	,"START" },
		{ UI_MENU_00_TYPE_NORMAL	,"USER GUIDE" },
		{ UI_MENU_00_TYPE_NORMAL	,"RANKING" },
		{ UI_MENU_00_TYPE_NORMAL	,"SETTINGS" },
		{ UI_MENU_00_TYPE_NORMAL	,"QUIT" },
	},
	{
		{ UI_MENU_00_TYPE_NORMAL	,"OK" },
	},
	{
		{ UI_MENU_00_TYPE_NORMAL	,"OK" },
	},
};

// �^�C�g�����[00] �̃e�L�X�g�̏��
Text g_aMd_title_00Text[MD_TITLE_00_TEXT_MAX] =
{
	{ "(C) 2022 STUDIO.D0DO"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - (PIXEL * 12), 0.0f)						,{ 255, 255, 255, 255 } , FONT_000 },	// ���쌠�\��
	{ "CREDIT : %02d"			, D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + PIXEL * 8 * 20, SCREEN_HEIGHT - (PIXEL * 12), 0.0f)	,{ 231, 0  , 91 , 255 } , FONT_000 },	// �N���W�b�g
	{ " %s  %-3s %8d"			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)																,{ 255, 255, 255, 255 } , FONT_000 },	// �����L���O�X�R�A
};

//========== *** �^�C�g�����[00] �̏����擾 ***
Md_title_00 *GetMd_title_00(void) 
{
	return &g_md_title_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterMd_title_00�֐� - �^�C�g�����[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterMd_title_00(void)
{
	g_md_title_00.bPlayBGM		= false;	// BGM�Đ��t���O
	g_md_title_00.bShowRanking	= false;	// �����L���O�\���t���O
}

//========================================
// UpdateTxtMd_title_00�֐� - �e�L�X�g���e�̍X�V���� -
//========================================
void UpdateTxtMd_title_00(void)
{
	char			aString					//
					[TXT_MAX];				// ���������p
	Chr_fighter_00	*pChr					//
					= GetChr_fighter_00();	// �@��[00] �̏��̃|�C���^

	//========== *** �e�L�X�g[���쌠�\��] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_TITLE_00_TEXT_COPYRIGHT,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].aDisplay,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].pos,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].col,
			TEXT_MODE_CENTER,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_COPYRIGHT].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�N���W�b�g] ***
	{
		sprintf(aString, g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].aDisplay, INIT_CREDIT_NUM);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_TITLE_00_TEXT_CREDIT,
			aString,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].pos,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].col,
			TEXT_MODE_CENTER,
			g_aMd_title_00Text[MD_TITLE_00_TEXT_CREDIT].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�����L���O] ***
	{
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			Sys_ranking_00	*pSys					//
							= GetSys_ranking_00();	// �����L���O[00] �̏��̃|�C���^
			char			aName					//
							[RANKING_NAME_NUM + 1];	// ���O
			D3DXVECTOR3		pos						//
							= RANKING_TEXT_POS;		// �ʒu

			// Y���W�������L���O�̃J�E���g�ɉ����Ă��炷
			pos.y += nCntRanking * RANKING_TEXT_SPACE_Y;

			sprintf(aString, g_aMd_title_00Text[MD_TITLE_00_TEXT_RANKING].aDisplay, GetRankText(nCntRanking), pSys[nCntRanking].aName, pSys[nCntRanking].nScore);
			UpdateTxt_00(
				TEXT_FIELD_UI,
				MD_TITLE_00_TEXT_RANKING + nCntRanking,
				aString,
				pos,
				g_aMd_title_00Text[MD_TITLE_00_TEXT_RANKING].col,
				TEXT_MODE_LEFT,
				g_aMd_title_00Text[MD_TITLE_00_TEXT_RANKING].font,
				1.0f,
				1.0f);
		}
	}
}

//========================================
// StartMd_title_00State�֐� - �^�C�g�����[00] �̏�Ԃɉ������J�n���� -
//========================================
void StartMd_title_00State(void)
{
	// ��ԃJ�E���^�[��������
	g_md_title_00.nCounterState = 0;

	switch (g_md_title_00.state)
	{
		//========== *** �`�[�����S ***
	case MD_TITLE_00_STATE_TEAM_LOGO:
	{
		// �w�i�F�������̂��̂ɐݒ�
		SetBg_color(MD_TITLE_00_INIT_BG_COLOR, 0);

		// �`�[�����S[00] ���o�����ɂ���
		SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_IN_POP);
	}
	break;
		//========== *** �^�C�g���A�j���[�V���� ***
	case MD_TITLE_00_STATE_TITLE_ANIM:
	{
		// �^�C�g�����S[00] ���v���Y�}�E�H�[���̈ړ��҂���Ԃɂ���
		SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT);
	}
		break;
		//========== *** �t���b�V�� ***
	case MD_TITLE_00_STATE_FLASH:
	{
		// �w�i�F���t���b�V�����̂��̂ɐݒ�
		SetBg_color(MD_TITLE_00_FLASH_BG_COLOR, 0);

		// �w�i�F��ʏ펞�̂��̂ɐ��ڐݒ�
		SetBg_color(MD_TITLE_00_NORMAL_BG_COLOR, MD_TITLE_00_FLASH_TIME);
	}
		break;
		//========== *** �ʏ� ***
	case MD_TITLE_00_STATE_NORMAL:
	{
		// �^�C�g�����S[00] ���o���ɂ���
		SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_POP);

		// �w�i�F��ʏ펞�̂��̂ɐݒ�
		SetBg_color(MD_TITLE_00_NORMAL_BG_COLOR, 0);

		// �`�[�����S[00] �������Ă����Ԃɂ���
		SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_CLEAR);

		if (!g_md_title_00.bPlayBGM) 
		{// BGM�Đ��t���O���U�̎��A
			// �^�C�g��BGM[000] ���Đ�
			PlaySound(SOUND_LABEL_BGM_TITLE_000);

			// BGM�Đ��t���O��^�ɂ���
			g_md_title_00.bPlayBGM = true;
		}

		// ���j���[�̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_TITLE_00_MAIN_MENU_POS);

		// ���j���[[00] �̐ݒ菈��
		SetUi_menu_00(
			g_aUi_menu_00_set[MD_TITLE_00_MENU_MAIN],
			MD_TITLE_00_MAIN_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);

		// �I��ԍ����J�n�ɐݒ�
		*GetSelect() = MD_TITLE_00_MAIN_MENU_START;

		// �e�L�X�g[���쌠�\��] ��\��
		// �e�L�X�g[�N���W�b�g] ��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_COPYRIGHT, true);
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_CREDIT, true);
	}
		break;
		//========== *** ������� ***
	case MD_TITLE_00_STATE_USER_GUIDE:
	{
		// ���j���[�̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_TITLE_00_USER_GUIDE_MENU_POS);

		// ���j���[[00] �̐ݒ菈��
		SetUi_menu_00(
			g_aUi_menu_00_set[MD_TITLE_00_MENU_USER_GUIDE],
			MD_TITLE_00_USER_GUIDE_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);

		// �I��ԍ��𗹉��ɐݒ�
		*GetSelect() = MD_TITLE_00_USER_GUIDE_MENU_OK;

		// �������[00] ���o�����̏�Ԃɂ���
		SetStateUi_operation_00(UI_OPERATION_00_STATE_IN_POP);
	}
		break;
		//========== *** �����L���O ***
	case MD_TITLE_00_STATE_RANKING:
	{
		// ���j���[�̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_TITLE_00_RANKING_MENU_POS);

		// ���j���[[00] �̐ݒ菈��
		SetUi_menu_00(
			g_aUi_menu_00_set[MD_TITLE_00_MENU_RANKING],
			MD_TITLE_00_RANKING_MENU_MAX,
			UI_MENU_00_DEFAULT_SPACE_X,
			UI_MENU_00_DEFAULT_SPACE_Y);

		// �I��ԍ��𗹉��ɐݒ�
		*GetSelect() = MD_TITLE_00_RANKING_MENU_OK;

		// �t���[��[02] �̐ݒ菈��
		SetUi_frame_02(RANKING_FRAME_POS);
	}
		break;
		//========== *** �ݒ� ***
	case MD_TITLE_00_STATE_SETTING:
	{
		// �ݒ胁�j���[�̐ݒ菈��
		SetSettingMenu(MODE_TITLE_00);
	}
	break;
	}
}

//========================================
// EndMd_title_00State�֐� - �^�C�g�����[00] �̏�Ԃɉ������I������ -
//========================================
void EndMd_title_00State(void) 
{
	switch (g_md_title_00.state)
	{
		//========== *** �`�[�����S ***
	case MD_TITLE_00_STATE_TEAM_LOGO:
	{

	}
	break;
		//========== *** �^�C�g���A�j���[�V���� ***
	case MD_TITLE_00_STATE_TITLE_ANIM:
	{
		
	}
	break;
		//========== *** �t���b�V�� ***
	case MD_TITLE_00_STATE_FLASH:
	{

	}
		//========== *** �ʏ� ***
	case MD_TITLE_00_STATE_NORMAL:
	{
		// �e�L�X�g[���쌠�\��] ���\��
		// �e�L�X�g[�N���W�b�g] ���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_COPYRIGHT, false);
		ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_CREDIT, false);
	}
		break;
		//========== *** ������� ***
	case MD_TITLE_00_STATE_USER_GUIDE:
	{
		// �������[00] ���������̏�Ԃɂ���
		SetStateUi_operation_00(UI_OPERATION_00_STATE_IN_CLEAR);
	}
		break;
		//========== *** �����L���O ***
	case MD_TITLE_00_STATE_RANKING:
	{
		// �e�L�X�g[�����L���O] ��S�Ĕ�\������
		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
		{
			ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_RANKING + nCntRanking, false);
		}

		// �������ɂ���
		SetStateUi_frame_02(UI_FRAME_02_STATE_IN_CLEAR);

		// �����L���O�\���t���O���U�ɂ���
		g_md_title_00.bShowRanking = false;
	}
		break;
		//========== *** �ݒ� ***
	case MD_TITLE_00_STATE_SETTING:
	{
		
	}
		break;
	}
}

//========================================
// UpdateMd_title_00State�֐� - �^�C�g�����[00] �̏�Ԃɉ������X�V���� -
//========================================
void UpdateMd_title_00State(void)
{
	switch (g_md_title_00.state)
	{
	case MD_TITLE_00_STATE_TEAM_LOGO:
	case MD_TITLE_00_STATE_TITLE_ANIM:
	case MD_TITLE_00_STATE_FLASH:
		// ��Ԃ��`�[�����S/�^�C�g���A�j���[�V����/�t���b�V���̂����ꂩ�̎��A

		if ((GetKeyboardTrigger(DIK_RETURN))
			|| 
			(GetButtonTrigger(BUTTON_A))
			||
			(GetButtonTrigger(BUTTON_START)))
		{// �X�L�b�v�̓��͎��A
			// ��Ԃ�ʏ�ɐݒ�
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);
		}

		break;
	}

	switch (g_md_title_00.state)
	{
		//========== *** �`�[�����S ***
	case MD_TITLE_00_STATE_TEAM_LOGO:
	{
		if (GetUi_team_logo_00()->state == UI_TEAM_LOGO_00_STATE_CLEAR) 
		{// �`�[�����S[00] �������Ă����Ԃ̎��A
			// ��Ԃ��^�C�g���A�j���[�V�����ɐݒ�
			SetMd_title_00State(MD_TITLE_00_STATE_TITLE_ANIM);
		}
	}
	break;
		//========== *** �^�C�g���A�j���[�V���� ***
	case MD_TITLE_00_STATE_TITLE_ANIM:
	{
		if (++g_md_title_00.nCounterState == MD_TITLE_00_TITLE_ANIM_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʃ^�C�g���A�j���[�V�����ɂ����鎞�ԂɒB�������A
			// ��Ԃ��t���b�V���ɂ���
			SetMd_title_00State(MD_TITLE_00_STATE_FLASH);
		}
	}
	break;
		//========== *** �t���b�V�� ***
	case MD_TITLE_00_STATE_FLASH:
	{
		if (++g_md_title_00.nCounterState == MD_TITLE_00_FLASH_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʃt���b�V���ɂ����鎞�ԂɒB�������A
			// ��Ԃ�ʏ�ɂ���
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);
		}
	}
		//========== *** �ʏ� ***
	case MD_TITLE_00_STATE_NORMAL:
	{
		// ���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** �J�n ***
		case MD_TITLE_00_MAIN_MENU_START:

			// ���ʉ����Đ�
			PlaySound(SOUND_LABEL_SE_DECITION_000);

			// ��ʂ��Q�[�����[00] �ɐݒ�
			SetFade(MODE_GAME_00);

			break;
			//========== *** ������� ***
		case MD_TITLE_00_MAIN_MENU_USER_GUIDE:

			// ��Ԃ𑀍�����ɐݒ�
			SetMd_title_00State(MD_TITLE_00_STATE_USER_GUIDE);

			break;
			//========== *** �����L���O ***
		case MD_TITLE_00_MAIN_MENU_RANKING:

			// ��Ԃ������L���O�ɐݒ�
			SetMd_title_00State(MD_TITLE_00_STATE_RANKING);

			break;
			//========== *** �ݒ� ***
		case MD_TITLE_00_MAIN_MENU_SETTINGS:

			// ��Ԃ�ݒ�ɐݒ�
			SetMd_title_00State(MD_TITLE_00_STATE_SETTING);

			break;
			//========== *** �I�� ***
		case MD_TITLE_00_MAIN_MENU_EXIT:

			// �E�C���h�E��j������
			DestroyWindow(*GetWindowHandle());

			break;
		}
	}
		break;
		//========== *** ������� ***
	case MD_TITLE_00_STATE_USER_GUIDE:
	{
		// ���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** ���� ***
		case MD_TITLE_00_USER_GUIDE_MENU_OK:

			// ��Ԃ�ʏ�ɐݒ�
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);

			break;
		}
	}
		break;
		//========== *** �����L���O ***
	case MD_TITLE_00_STATE_RANKING:
	{
		if ((GetUi_frame_02()->state == UI_FRAME_02_STATE_POP)
			&& (!g_md_title_00.bShowRanking))
		{// �t���[��[02] �̏�Ԃ��o�����̎��A�������L���O�\���t���O���U�̎��A
			// �����L���O�\���t���O��^�ɂ���
			g_md_title_00.bShowRanking = true;

			// �e�L�X�g[�����L���O] ��S�ĕ\������
			for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
			{
				ShowTxt_00(TEXT_FIELD_UI, MD_TITLE_00_TEXT_RANKING + nCntRanking, true);
			}

			// ���nSE[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_LANDING_000);
		}

		// ���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
			//========== *** ���� ***
		case MD_TITLE_00_USER_GUIDE_MENU_OK:

			// ��Ԃ�ʏ�ɐݒ�
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);

			break;
		}
	}
		break;
		//========== *** �ݒ� ***
	case MD_TITLE_00_STATE_SETTING:
	{
		// �ݒ胁�j���[�̍X�V����
		UpdateSettingMenu();
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
// InitMd_title_00�֐� - �^�C�g�����[00]�̏��������� -
//========================================
void InitMd_title_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterMd_title_00();

	// ����������
	InitTxt_00();			// �e�L�X�g
	InitBg_space_01();		// BG ;�F��			[01]
	InitImg_fighter_00();	// IMG:�@��			[00]
	InitUi_frame_02();		// UI :�t���[��		[00]
	InitUi_menu_00();		// UI :���j���[		[00]
	InitUi_operation_00();	// UI :�������		[00]
	InitUi_team_logo_00();	// UI :�`�[�����S	[00]
	InitUi_title_logo_00();	// UI :�^�C�g�����S	[00]

	// ��Ԃ��`�[�����S�ɐݒ�
	SetMd_title_00State(MD_TITLE_00_STATE_TEAM_LOGO);
}

//========================================
// UninitMd_title_00�֐� - �^�C�g�����[00]�̏I������ -
//========================================
void UninitMd_title_00(void)
{
	// �I������
	UninitTxt_00();				// �e�L�X�g
	UninitBg_space_01();		// BG ;�F��			[01]
	UninitImg_fighter_00();		// IMG:�@��			[00]
	UninitUi_frame_02();		// UI :�t���[��		[00]
	UninitUi_menu_00();			// UI :���j���[		[00]
	UninitUi_operation_00();	// UI :�������		[00]
	UninitUi_team_logo_00();	// UI :�`�[�����S	[00]
	UninitUi_title_logo_00();	// UI :�^�C�g�����S	[00]
}

//========================================
// UpdateMd_title_00�֐� - �^�C�g�����[00]�̍X�V���� -
//========================================
void UpdateMd_title_00(void)
{
	// �X�V����
	UpdateBg_space_01();		// BG ;�F��			[01]
	UpdateImg_fighter_00();		// IMG:�@��			[00]
	UpdateUi_frame_02();		// UI :�t���[��		[00]
	UpdateUi_menu_00();			// UI :���j���[		[00]
	UpdateUi_operation_00();	// UI :�������		[00]
	UpdateUi_team_logo_00();	// UI :�`�[�����S	[00]
	UpdateUi_title_logo_00();	// UI :�^�C�g�����S	[00]

	// �^�C�g�����[00] �̏�Ԃɉ������X�V����
	UpdateMd_title_00State();

	// �e�L�X�g���e�̍X�V����
	UpdateTxtMd_title_00();
}

//========================================
// DrawMd_title_00�֐� - �^�C�g�����[00]�̕`�揈�� -
//========================================
void DrawMd_title_00(void)
{
	// �`�揈��
	DrawBg_space_01();		// BG ;�F��			[01]
	DrawImg_fighter_00();	// IMG:�@��			[00]
	DrawUi_frame_02();		// UI :�t���[��		[00]
	DrawUi_menu_00();		// UI :���j���[		[00]
	DrawTxt_00();			// �e�L�X�g
	DrawUi_operation_00();	// UI :�������		[00]
	DrawUi_title_logo_00();	// UI :�^�C�g�����S	[00]
	DrawUi_team_logo_00();	// UI :�`�[�����S	[00]
}

//========================================
// SetMd_title_00State�֐� - �^�C�g�����[00] �̏�Ԃ�ݒ� -
//========================================
void SetMd_title_00State(MD_TITLE_00_STATE state)
{
	// �^�C�g�����[00] �̏�Ԃɉ������I������
	EndMd_title_00State();

	// �^�C�g�����[00] �̏�Ԃ���
	g_md_title_00.state = state;

	// �^�C�g�����[00] �̏�Ԃɉ������J�n����
	StartMd_title_00State();
}