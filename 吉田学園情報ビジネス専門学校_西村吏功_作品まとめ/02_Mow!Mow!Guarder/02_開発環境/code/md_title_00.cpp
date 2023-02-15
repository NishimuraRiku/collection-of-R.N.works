//========================================
// 
// MD:�^�C�g�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_title_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "setting.h"
#include "sound.h"
#include "text.h"
#include "bg_mountain_00.h"			// BG :�R					[00]
#include "bg_space_00.h"			// BG :�F��					[00]
#include "md_title_00.h"			// MD :�^�C�g�����			[00]
#include "obj_stage_00.h"			// OBJ:�X�e�[�W				[00]
#include "ui_menu_00.h"				// UI :���j���[				[00]
#include "ui_ranking-frame_00.h"	// UI :�����L���O�t���[��	[00]
#include "ui_title-logo_00.h"		// UI :�^�C�g�����S			[00]
#include "ui_user-guid_00.h"		// UI :���[�U�[�K�C�h		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// �^�C�g�����[00] �̃��C�����j���[�̈ʒu
// �^�C�g�����[00] �̃��[�U�[�K�C�h���j���[�̈ʒu
// �^�C�g�����[00] �̃��[�U�[�K�C�h�̈ʒu
// �^�C�g�����[00] �̃����L���O���j���[�̈ʒu
// �^�C�g�����[00] �̃����L���O�̈ʒu
// �^�C�g�����[00] �̐ݒ胁�j���[�̈ʒu
#define MD_TITLE_00_MAIN_MENU_POS		D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*64,0.0f)
#define MD_TITLE_00_USERGUID_MENU_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*32,0.0f)
#define MD_TITLE_00_USERGUID_POS		D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT*0.5f,0.0f)
#define MD_TITLE_00_RANKING_MENU_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*32,0.0f)
#define MD_TITLE_00_RANKING_POS			D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT*0.5f,0.0f)
#define MD_TITLE_00_SETTING_MENU_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*80),SCREEN_HEIGHT-PIXEL*64,0.0f)

// �^�C�g�����[00] �̒��쌠�̈ʒu
// �^�C�g�����[00] �̒��쌠�̕\���ɂ����鎞��
#define MD_TITLE_00_COPYRIGHT_POS		D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_HEIGHT-PIXEL*8,0.0f)
#define MD_TITLE_00_COPYRIGHT_SHOW_TIME	(10)

// �^�C�g�����[00] �̌���SE
#define MD_TITLE_00_DETERMINATION_SE	(SOUND_LABEL_SE_DETERMINATION_000)

//****************************************
// �񋓌^�̒�`
//****************************************
// �^�C�g�����[00] �̃��C�����j���[
typedef enum
{
	MD_TITLE_00_MAIN_MENU_START,	// �J�n
	MD_TITLE_00_MAIN_MENU_USERGUID,	// ���[�U�[�K�C�h
	MD_TITLE_00_MAIN_MENU_RANKING,	// �����L���O
	MD_TITLE_00_MAIN_MENU_SETTINGS,	// �ݒ�
	MD_TITLE_00_MAIN_MENU_EXIT,		// �I��
	MD_TITLE_00_MAIN_MENU_MAX,
}MD_TITLE_00_MAIN_MENU;

// �^�C�g�����[00] �̃��[�U�[�K�C�h���j���[
typedef enum
{
	MD_TITLE_00_USERGUID_MENU_OK,	// OK
	MD_TITLE_00_USERGUID_MENU_MAX,
}MD_TITLE_00_USERGUID_MENU;

// �^�C�g�����[00] �̃����L���O���j���[
typedef enum
{
	MD_TITLE_00_RANKING_MENU_OK,	// OK
	MD_TITLE_00_RANKING_MENU_MAX,
}MD_TITLE_00_RANKING_MENU;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�^�C�g�����[00] �̃p�����[�^�[�̏���������
// MD:�^�C�g�����[00] �̏�Ԃɉ������X�V����
// MD:�^�C�g�����[00] �̏�Ԃɉ������J�n����
// MD:�^�C�g�����[00] �̏�Ԃɉ������I������
void InitParameterMd_title_00(void);
void UpdateMd_title_00State(void);
void StartMd_title_00State(void);
void EndMd_title_00State(void);

//****************************************
// �O���[�o���錾
//****************************************
Md_title_00	g_md_title_00;	// MD:�^�C�g�����[00] �̏��

// MD:�^�C�g�����[00] �̃��C�����j���[�ݒ���
Ui_menu_00Set g_aMd_title_00MainMenuSet[MD_TITLE_00_MAIN_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"START"    ,false },
	{ UI_MENU_00_TYPE_NORMAL,"USER GUID",true  },
	{ UI_MENU_00_TYPE_NORMAL,"RANKING"  ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"SETTINGS" ,true },
	{ UI_MENU_00_TYPE_NORMAL,"QUIT"     ,false },
};

// MD:�^�C�g�����[00] �̃��[�U�[�K�C�h���j���[�ݒ���
Ui_menu_00Set g_aMd_title_00UserGuidSet[MD_TITLE_00_USERGUID_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"OK",true },
};

// MD:�^�C�g�����[00] �̃����L���O���j���[�ݒ���
Ui_menu_00Set g_aMd_title_00RankingMenuSet[MD_TITLE_00_RANKING_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"OK",true },
};

//========== *** MD:�^�C�g�����[00] �̏����擾 ***
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
// InitParameterMd_title_00�֐� - MD:�^�C�g�����[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterMd_title_00(void)
{
	Md_title_00	*pMd =	// MD:�^�C�g�����[00] �̏��
				&g_md_title_00;

	pMd->state				= MD_TITLE_00_STATE_NONE;	// ���
	pMd->nCounterState		= 0;						// ��ԃJ�E���^�[
	pMd->bShowCopyRight		= false;					// ���쌠�\���t���O
	pMd->nCounterCopyRight	= 0;						// ���쌠�\���J�E���^�[
}

//========================================
// StartMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃɉ������J�n���� -
//========================================
void StartMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	// ��ԃJ�E���^�[��������
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TITLE_00_STATE_NORMAL: {
		// UI:���j���[[00] �̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_TITLE_00_MAIN_MENU_POS);

		// UI:���j���[[00] �̐ݒ菈��(���C�����j���[)
		SetUi_menu_00(
			g_aMd_title_00MainMenuSet,
			MD_TITLE_00_MAIN_MENU_MAX);

		// ���쌠�\���t���O��^�ɂ���
		pMd->bShowCopyRight = true;

		break;
	}
	case /*/ ���[�U�[�K�C�h /*/MD_TITLE_00_STATE_USERGUID: {
		// UI:���j���[[00] �̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_TITLE_00_USERGUID_MENU_POS);

		// UI:���j���[[00] �̐ݒ菈��(���[�U�[�K�C�h���j���[)
		SetUi_menu_00(
			g_aMd_title_00RankingMenuSet,
			MD_TITLE_00_USERGUID_MENU_MAX);

		break;
	}
	case /*/ �����L���O /*/MD_TITLE_00_STATE_RANKING: {
		// UI:���j���[[00] �̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_TITLE_00_RANKING_MENU_POS);

		// UI:���j���[[00] �̐ݒ菈��(�����L���O���j���[)
		SetUi_menu_00(
			g_aMd_title_00RankingMenuSet,
			MD_TITLE_00_RANKING_MENU_MAX);

		break;
	}
	}
}

//========================================
// EndMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃɉ������I������ -
//========================================
void EndMd_title_00State(void) 
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TITLE_00_STATE_NORMAL: {
		break;
	}
	case /*/ ���[�U�[�K�C�h /*/MD_TITLE_00_STATE_USERGUID: {
		break;
	}
	case /*/ �����L���O /*/MD_TITLE_00_STATE_RANKING: {
		break;
	}
	}
}

//========================================
// UpdateMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃɉ������X�V���� -
//========================================
void UpdateMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TITLE_00_STATE_NORMAL: {
		// ���C�����j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ �J�n /*/MD_TITLE_00_MAIN_MENU_START: {
			// ��ʂ��Q�[�����[00] �ɐݒ�
			SetFade(MODE_GAME_00);

			// ����SE���Đ�
			PlaySound(MD_TITLE_00_DETERMINATION_SE);

			// ���쌠�\���t���O���U�ɂ���
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ ���[�U�[�K�C�h /*/MD_TITLE_00_MAIN_MENU_USERGUID: {
			// UI:���[�U�[�K�C�h[00] �̐ݒ菈��
			SetUi_userGuid_00(MD_TITLE_00_USERGUID_POS, GetActiveInputType());

			// ��Ԃ����[�U�[�K�C�h�ɂ���
			SetMd_title_00State(MD_TITLE_00_STATE_USERGUID);

			// ���쌠�\���t���O���U�ɂ���
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ �����L���O /*/MD_TITLE_00_MAIN_MENU_RANKING: {
			// UI:�����L���O�t���[��[00] �̐ݒ菈��
			SetUi_rankingFrame_00(MD_TITLE_00_RANKING_POS);

			// ���쌠�\���t���O���U�ɂ���
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ �ݒ� /*/MD_TITLE_00_MAIN_MENU_SETTINGS: {
			// UI:���j���[[00] �̒��S���W��ݒ�
			SetUi_menu_00Pos(MD_TITLE_00_SETTING_MENU_POS);

			// �ݒ胁�j���[�̐ݒ菈��
			SetSettingMenu();

			// ��Ԃ�ݒ�ɂ���
			SetMd_title_00State(MD_TITLE_00_STATE_SETTINGS);

			// ���쌠�\���t���O���U�ɂ���
			pMd->bShowCopyRight = false;
			break;
		}
		case /*/ �I�� /*/MD_TITLE_00_MAIN_MENU_EXIT: {
			// �E�C���h�E��j������
			DestroyWindow(*GetWindowHandle());
			break;
		}
		}

		if (GetUi_rankingFrame_00()->state == UI_RANKINGFRAME_00_STATE_POP) 
		{// UI:�����L���O�t���[��[00] �̏�Ԃ��o���̎��A
			SetMd_title_00State(MD_TITLE_00_STATE_RANKING);	// ��Ԃ������L���O�ɂ���
		}

		break;
	}
	case /*/ ���[�U�[�K�C�h /*/MD_TITLE_00_STATE_USERGUID: {
		// ���[�U�[�K�C�h���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ OK /*/MD_TITLE_00_USERGUID_MENU_OK: {
			// UI:���[�U�[�K�C�h[00] �̏�Ԃ��������ɂ���
			SetStateUi_userGuid_00(UI_USERGUID_00_STATE_IN_CLEAR);

			break;
		}
		}

		if (GetUi_userGuid_00()->state == UI_USERGUID_00_STATE_CLEAR)
		{// UI:���[�U�[�K�C�h[00] �̏�Ԃ������Ă��鎞�A
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// ��Ԃ�ʏ�ɂ���
		}

		break;
	}
	case /*/ �����L���O /*/MD_TITLE_00_STATE_RANKING: {
		// �����L���O���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ OK /*/MD_TITLE_00_RANKING_MENU_OK: {
			// UI:�����L���O�t���[��[00] �̏�Ԃ��������ɂ���
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_IN_CLEAR);

			break;
		}
		}

		if (GetUi_rankingFrame_00()->state == UI_RANKINGFRAME_00_STATE_CLEAR)
		{// UI:�����L���O�t���[��[00] �̏�Ԃ������Ă��鎞�A
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// ��Ԃ�ʏ�ɂ���
		}

		break;
	}
	case /*/ �ݒ� /*/MD_TITLE_00_STATE_SETTINGS: {
		// �ݒ胁�j���[�̍X�V����
		UpdateSettingMenu();

		if (!*GetSettingNow())
		{// �ݒ蒆�t���O���U�̎��A
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// ��Ԃ�ʏ�ɂ���
		}

		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_title_00�֐� - MD:�^�C�g�����[00]�̏��������� -
//========================================
void InitMd_title_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterMd_title_00();

	// ����������
	InitParameterCamera3D();	// �J����(3D)�̃p�����[�^�[
	InitBg_mountain_00();		// BG :�R					[00]
	InitBg_space_00();			// BG :�F��					[00]
	InitObj_stage_00();			// OBJ:�X�e�[�W				[00]
	InitUi_menu_00();			// UI :���j���[				[00]
	InitUi_rankingFrame_00();	// UI :�����L���O�t���[��	[00]
	InitUi_titleLogo_00();		// UI :�^�C�g�����S			[00]
	InitUi_userGuid_00();		// UI :���[�U�[�K�C�h		[00]

	// ��Ԃ�ʏ�ɐݒ�
	SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);

	// �J����(3D)�̒����_��OBJ:�X�e�[�W[00] �ɐݒ�
	SetCamera3DPosR(
		GetObj_stage_00()->pos +
		D3DXVECTOR3(0.0f, MD_TITLE_00_CAMERA3D_POS_V_HEIGHT, 0.0f));

	// �J����(3D)�̍���/������ݒ�
	GetCamera3D()->fHeight = MD_TITLE_00_CAMERA3D_POS_R_HEIGHT;
	GetCamera3D()->fLength = MD_TITLE_00_CAMERA3D_LENGTH;

	// �^�C�g��BGM���Đ�
	PlaySound(SOUND_LABEL_BGM_TITLE_000);
}

//========================================
// UninitMd_title_00�֐� - MD:�^�C�g�����[00]�̏I������ -
//========================================
void UninitMd_title_00(void)
{
	// �I������
	UninitBg_mountain_00();		// BG :�R					[00]
	UninitBg_space_00();		// BG :�F��					[00]
	UninitObj_stage_00();		// OBJ:�X�e�[�W				[00]
	UninitUi_menu_00();			// UI :���j���[				[00]
	UninitUi_rankingFrame_00();	// UI :�����L���O�t���[��	[00]
	UninitUi_titleLogo_00();	// UI :�^�C�g�����S			[00]
	UninitUi_userGuid_00();		// UI :���[�U�[�K�C�h		[00]
}

//========================================
// UpdateMd_title_00�֐� - MD:�^�C�g�����[00]�̍X�V���� -
//========================================
void UpdateMd_title_00(void)
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	// MD:�^�C�g�����[00] �̏�Ԃɉ������X�V����
	UpdateMd_title_00State();

	// �X�V����
	UpdateBg_mountain_00();		// BG :�R					[00]
	UpdateBg_space_00();		// BG :�F��					[00]
	UpdateObj_stage_00();		// OBJ:�X�e�[�W				[00]
	UpdateUi_menu_00();			// UI :���j���[				[00]
	UpdateUi_rankingFrame_00();	// UI :�����L���O�t���[��	[00]
	UpdateUi_titleLogo_00();	// UI :�^�C�g�����S			[00]
	UpdateUi_userGuid_00();		// UI :���[�U�[�K�C�h		[00]

	if (pMd->bShowCopyRight) {		// ���쌠�\���t���O���^�̎��A
		pMd->nCounterCopyRight++;	// ���쌠�\���J�E���^�[�����Z
	}
	else {							// ���쌠�\���t���O���U�̎��A
		pMd->nCounterCopyRight--;	// ���쌠�\���J�E���^�[�����Z
	}
	// ���쌠�\���J�E���^�[�𐧌�
	IntControl(&pMd->nCounterCopyRight, MD_TITLE_00_COPYRIGHT_SHOW_TIME, 0);
	{
		float	fRate	// �J�E���^�[�̐i�s��
				= (float)pMd->nCounterCopyRight / (float)MD_TITLE_00_COPYRIGHT_SHOW_TIME;

		// ���쌠�\���̃e�L�X�g�ݒ�
		SetText2D(
			"(C) 2022 STUDIO.D0DO",
			FONT_002,
			DISPLAY_CENTER,
			MD_TITLE_00_COPYRIGHT_POS,
			{ 255,255,255,(int)(255 * fRate) },
			fRate,
			fRate);
	}
}

//========================================
// DrawMd_title_00�֐� - MD:�^�C�g�����[00]�̕`�揈�� -
//========================================
void DrawMd_title_00(void)
{
	// �`�揈��
	DrawBg_mountain_00();		// BG :�R					[00]
	DrawBg_space_00();			// BG :�F��					[00]
	DrawObj_stage_00();			// OBJ:�X�e�[�W				[00]
	DrawUi_menu_00();			// UI :���j���[				[00]
	DrawUi_rankingFrame_00();	// UI :�����L���O�t���[��	[00]
	DrawUi_titleLogo_00();		// UI :�^�C�g�����S			[00]
	DrawUi_userGuid_00();		// UI :���[�U�[�K�C�h		[00]
	DrawText_();				// �e�L�X�g
}

//========================================
// SetMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃ�ݒ� -
//========================================
void SetMd_title_00State(MD_TITLE_00_STATE state)
{
	// MD:�^�C�g�����[00] �̏�Ԃɉ������I������
	EndMd_title_00State();

	// MD:�^�C�g�����[00] �̏�Ԃ���
	g_md_title_00.state = state;

	// MD:�^�C�g�����[00] �̏�Ԃɉ������J�n����
	StartMd_title_00State();
}