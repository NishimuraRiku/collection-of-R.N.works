//========================================
// 
// MD:�Q�[�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "light.h"
#include "setting.h"
#include "sound.h"
#include "text.h"
#include "atk_bullet_00.h"			// ATK:�e					[00]
#include "atk_impact_00.h"			// ATK:�Ռ�					[00]
#include "atk_sword_00.h"			// ATK:��					[00]
#include "bg_mountain_00.h"			// BG :�R					[00]
#include "bg_space_00.h"			// BG :�F��					[00]
#include "chr_player_00.h"			// CHR:�v���C���[			[00]
#include "chr_enemy_00.h"			// CHR:�G					[00]
#include "eff_explosion_00.h"		// EFF:����					[00]
#include "eff_light_00.h"			// EFF:��					[00]
#include "eff_particle_00.h"		// EFF:�p�[�e�B�N��			[00]
#include "eff_screen_00.h"			// EFF:�X�N���[��			[00]
#include "eff_shadow_00.h"			// EFF:�e					[00]
#include "eff_shock-wave_00.h"		// EFF:�Ռ��g				[00]
#include "md_game_00.h"				// MD :�Q�[�����			[00]
#include "itm_coin_00.h"			// ITM:�R�C��				[00]
#include "obj_block_00.h"			// OBJ:�u���b�N				[00]
#include "obj_core_00.h"			// OBJ:�R�A					[00]
#include "obj_discharger_00.h"		// OBJ:���d���u				[00]
#include "obj_mirror_00.h"			// OBJ:�~���[				[00]
#include "obj_stage_00.h"			// OBJ:�X�e�[�W				[00]
#include "obj_switch_00.h"			// OBJ:�X�C�b�`				[00]
#include "obj_target_00.h"			// OBJ:�^�[�Q�b�g			[00]
#include "obj_turret_00.h"			// OBJ:�^���b�g				[00]
#include "sys_ranking_00.h"			// SYS:�����L���O			[00]
#include "ui_attention-mark_00.h"	// UI :���Ӄ}�[�N			[00]
#include "ui_input-guid_00.h"		// UI :���̓K�C�h			[00]
#include "ui_coin-frame_00.h"		// UI :�R�C���t���[��		[00]
#include "ui_damage_00.h"			// UI :�_���[�W				[00]
#include "ui_dialog-box_00.h"		// UI :�_�C�A���O�{�b�N�X	[00]
#include "ui_frame_00.h"			// UI :�t���[��				[00]
#include "ui_hp-bar_00.h"			// UI :HP�o�[				[00]
#include "ui_hp-bar_01.h"			// UI :HP�o�[				[01]
#include "ui_inventory_00.h"		// UI :�C���x���g��			[00]
#include "ui_item-icon_00.h"		// UI :�A�C�e���A�C�R��		[00]
#include "ui_item-guid_00.h"		// UI :���i����				[00]
#include "ui_map_00.h"				// UI :�}�b�v				[00]
#include "ui_menu_00.h"				// UI :���j���[				[00]
#include "ui_parameter_00.h"		// UI :�p�����[�^�[			[00]
#include "ui_ranking-frame_00.h"	// UI :�����L���O�t���[��	[00]
#include "ui_tips_00.h"				// UI :TIPS					[00]
#include "ui_user-guid_00.h"		// UI :���[�U�[�K�C�h		[00]
#include "ui_warning-text_00.h"		// UI :���Ӄe�L�X�g			[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// MD:�Q�[�����[00] �̃|�[�Y���j���[�̈ʒu
// MD:�Q�[�����[00] �̃��U���g���j���[�̈ʒu
// MD:�Q�[�����[00] �̃����L���O�̈ʒu
// MD:�Q�[�����[00] �̃��[�U�[�K�C�h���j���[�̈ʒu
// MD:�Q�[�����[00] �̃��[�U�[�K�C�h�̈ʒu
// MD:�Q�[�����[00] �̐ݒ胁�j���[�̈ʒu
#define MD_GAME_00_PAUSE_MENU_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT-PIXEL*64,0.0f)
#define MD_GAME_00_RESULT_MENU_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT+(PIXEL*-32),0.0f)
#define MD_GAME_00_RANKING_POS			D3DXVECTOR3(BUFFER_WIDTH*0.5f,(BUFFER_HEIGHT*0.5f)-PIXEL*8,0.0f)
#define MD_GAME_00_USERGUID_MENU_POS	D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT-PIXEL*32,0.0f)
#define MD_GAME_00_USERGUID_POS			D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT*0.5f,0.0f)
#define MD_GAME_00_SETTING_MENU_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT-PIXEL*64,0.0f)

// MD:�Q�[�����[00] �̃{�X�o�����[�r�[(���_�ړ�)�ɂ����鎞��
// MD:�Q�[�����[00] �̃{�X�o�����[�r�[(���_���Z�b�g)�ɂ����鎞��
#define MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE_TIME		(60)
#define MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET_TIME	(60)

// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(����)�ɂ����鎞��
// MD:�Q�[�����[00] �̃R�A�j�󃀁[�r�[(����)�̖��邭�Ȃ�n�߂鎞��
#define MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_TIME		(120)
#define MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_BRIGHT_TIME	(60)

// MD:�Q�[�����[00] �̃J����(3D)�̏�������
#define MD_GAME_00_CAMERA3D_INIT_ROT	D3DXVECTOR3(0.0f,D3DX_PI,0.0f)

// MD:�Q�[�����[00] �̃|�[�YSE
// MD:�Q�[�����[00] �̌���SE
// MD:�Q�[�����[00] �̃N���ASE
// MD:�Q�[�����[00] �̃e�L�X�g�Z�b�gSE
#define MD_GAME_00_PAUSE_SE		(SOUND_LABEL_SE_PAUSE_000)
#define MD_GAME_00_RETRY_SE		(SOUND_LABEL_SE_DETERMINATION_000)
#define MD_GAME_00_CLEAR_SE		(SOUND_LABEL_BGM_CLEAR_000)
#define MD_GAME_00_TEXT_SET_SE	(SOUND_LABEL_SE_SELECT_000)

// MD:�Q�[�����[00] �̃N���A��̗]�C
// MD:�Q�[�����[00] �̃N���A���o�ɂ����鎞��
// MD:�Q�[�����[00] �̃N���A�e�L�X�g�̕\���ɂ����鎞��
// MD:�Q�[�����[00] �̃N���A�e�L�X�g�̈ʒu
// MD:�Q�[�����[00] �̃N���A�e�L�X�g�̐F
// MD:�Q�[�����[00] �̃N���A�e�L�X�g�̕\���`��
// MD:�Q�[�����[00] �̃N���A�e�L�X�g�̊g��{��
#define MD_GAME_00_CLEAR_AFTERGLOW		(120)
#define MD_GAME_00_CLEAR_TIME			(420)
#define MD_GAME_00_CLEAR_TEXT_SHOW_TIME	(150)
#define MD_GAME_00_CLEAR_TEXT_POS		D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y,0.0f)
#define MD_GAME_00_CLEAR_TEXT_COLOR		Color{243,191,63,255}
#define MD_GAME_00_CLEAR_TEXT_DISPLAY	"STAGE CLEAR"
#define MD_GAME_00_CLEAR_TEXT_SCALE		(1.5f)

// MD:�Q�[�����[00] �̃N���A�e�L�X�g(�e)�̏o������
// MD:�Q�[�����[00] �̃N���A�e�L�X�g(�e)�̏�����̂ɂ����鎞��
// MD:�Q�[�����[00] �̃N���A�e�L�X�g(�e)�̊g��{��
#define MD_GAME_00_CLEAR_TEXT_SHADOW_POP_TIME	(340)
#define MD_GAME_00_CLEAR_TEXT_SHADOW_CLEAR_TIME	(60)
#define MD_GAME_00_CLEAR_TEXT_SHADOW_SCALE		(3.0f)

// MD:�Q�[�����[00] ��TIPS�҂��ɂ����鎞��
#define MD_GAME_00_TIPS_WAIT_TIME	(60)

// MD;�Q�[�����[00] �̃X�e�[�W�̎��
#define MD_GAME_00_STAGE_TYPE	(0)

//****************************************
// �񋓌^�̒�`
//****************************************
// �Q�[�����[00] �̃��j���[
typedef enum
{
	MD_GAME_00_MENU_RESULT,	// ���C��
	MD_GAME_00_MENU_MAX,
}MD_GAME_00_MENU;

// �Q�[�����[00] �̃��U���g���j���[
typedef enum
{
	MD_GAME_00_RESULT_MENU_RETRY,			// ���g���C
	MD_GAME_00_RESULT_MENU_BACK_TO_TITLE,	// �^�C�g���ɖ߂�
	MD_GAME_00_RESULT_MENU_MAX,
}MD_GAME_00_RESULT_MENU;

// �Q�[�����[00] �̃|�[�Y���j���[
typedef enum
{
	MD_GAME_00_PAUSE_MENU_RESUME,			// �ĊJ
	MD_GAME_00_PAUSE_MENU_RETRY,			// ���g���C
	MD_GAME_00_PAUSE_MENU_USERGUID,			// ���[�U�[�K�C�h
	MD_GAME_00_PAUSE_MENU_SETTINGS,			// �ݒ�
	MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE,	// �^�C�g���ɖ߂�
	MD_GAME_00_PAUSE_MENU_MAX,
}MD_GAME_00_PAUSE_MENU;

// �Q�[�����[00] �̃��[�U�[�K�C�h���j���[
typedef enum
{
	MD_GAME_00_USERGUID_MENU_OK,	// OK
	MD_GAME_00_USERGUID_MENU_MAX,
}MD_GAME_00_USERGUID_MENU;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�Q�[�����[00] �̃p�����[�^�[�̏���������
// MD:�Q�[�����[00] �̏�Ԃɉ������X�V����
// MD:�Q�[�����[00] �̏�Ԃɉ������J�n����
// MD:�Q�[�����[00] �̏�Ԃɉ������I������
void InitParameterMd_game_00(void);
void UpdateMd_game_00State(void);
void StartMd_game_00State(void);
void EndMd_game_00State(void);

//****************************************
// �O���[�o���錾
//****************************************
Md_game_00	g_md_game_00;	// MD:�Q�[�����[00] �̏��

// MD:�Q�[�����[00] �̃��U���g���j���[�ݒ���
Ui_menu_00Set g_aMd_game_00ResultMenuSet[MD_GAME_00_RESULT_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RETRY"        ,true },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE",true },
};

// MD:�Q�[�����[00] �̃|�[�Y���j���[�ݒ���
Ui_menu_00Set g_aMd_game_00PauseMenuSet[MD_GAME_00_PAUSE_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"RESUME"       ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"RETRY"        ,false },
	{ UI_MENU_00_TYPE_NORMAL,"CONTROLS"     ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"SETTINGS"     ,true  },
	{ UI_MENU_00_TYPE_NORMAL,"BACK TO TITLE",true  },
};

// MD:�Q�[�����[00] �̃��[�U�[�K�C�h���j���[�ݒ���
Ui_menu_00Set g_aMd_game_00UserGuidSet[MD_GAME_00_USERGUID_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_NORMAL,"OK",true },
};

//========== *** MD:�Q�[�����[00] �̏����擾 ***
Md_game_00 *GetMd_game_00(void) 
{
	return &g_md_game_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterMd_game_00�֐� - MD:�Q�[�����[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterMd_game_00(void)
{
	Md_game_00	*pMd	// MD:�Q�[�����[00] �̏��
				= &g_md_game_00;

	pMd->state			= MD_GAME_00_STATE_NONE;	// ���
	pMd->previousPosR	= INITD3DXVECTOR3;			// �O�̒����_�̈ʒu
}

//========================================
// StartMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃɉ������J�n���� -
//========================================
void StartMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:�Q�[�����[00] �̏��
				= &g_md_game_00;

	// ��ԃJ�E���^�[��������
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	case /*/ �|�[�Y /*/MD_GAME_00_STATE_PAUSE: {
		// UI:���j���[[00] �̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_GAME_00_PAUSE_MENU_POS);

		// UI:���j���[[00] �̐ݒ菈��(�|�[�Y���j���[)
		SetUi_menu_00(
			g_aMd_game_00PauseMenuSet,
			MD_GAME_00_PAUSE_MENU_MAX);

		break;
	}
	case /*/ ���[�U�[�K�C�h /*/MD_GAME_00_STATE_USERGUID: {
		// UI:���[�U�[�K�C�h[00] �̐ݒ菈��
		SetUi_userGuid_00(MD_GAME_00_USERGUID_POS, GetActiveInputType());

		// UI:���j���[[00] �̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_GAME_00_USERGUID_MENU_POS);

		// UI:���j���[[00] �̐ݒ菈��(���[�U�[�K�C�h���j���[)
		SetUi_menu_00(
			g_aMd_game_00UserGuidSet,
			MD_GAME_00_USERGUID_MENU_MAX);

		break;
	}
	case /*/ �N���A���o /*/MD_GAME_00_STATE_CLEAR: {
		
		break;
	}
	case /*/ �����L���O /*/MD_GAME_00_STATE_RANKING: {
		// UI:�����L���O�t���[��[00] �̖��O���͐ݒ菈��
		SetNameEntryUi_rankingFrame_00(SetScore(GetChr_player_00()->nScore));

		// UI:�����L���O�t���[��[00] �̐ݒ菈��
		SetUi_rankingFrame_00(MD_GAME_00_RANKING_POS);

		break;
	}
	case /*/ ���U���g /*/MD_GAME_00_STATE_RESULT: {
		SetStateObj_stage_00(			// OBJ:�X�e�[�W[00] �̏�Ԃ��~�ɂ���
			OBJ_STAGE_00_STATE_STOP);
		SetChr_enemy_00ControlState(	// CHR:�G[00] �̊Ǘ���Ԃ�ÓI�ɂ���
			CHR_ENEMY_00_CONTROL_STATE_STATIC);
		SetChr_player_00ControlState(	// CHR:�v���C���[[00] �̏�Ԃ�ÓI�ɂ���
			CHR_PLAYER_00_CONTROL_STATE_STATIC);

		// UI:���j���[[00] �̒��S���W��ݒ�
		SetUi_menu_00Pos(MD_GAME_00_RESULT_MENU_POS);

		// UI:���j���[[00] �̐ݒ菈��(���U���g���j���[)
		SetUi_menu_00(
			g_aMd_game_00ResultMenuSet,
			MD_GAME_00_RESULT_MENU_MAX);

		break;
	}
	case /*/ �`���[�g���A��[00] /*/MD_GAME_00_STATE_TUTORIAL_00: {
		SetChr_player_00ControlState(	// CHR:�v���C���[[00] �̊Ǘ���Ԃ���͒�~�ɂ���
			CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP);

		// �J����(3D)�̒����_��OBJ:�R�A[00] �ɐݒ�
		SetCamera3DPosR(
			GetObj_core_00()->pos +
			D3DXVECTOR3(0.0f, MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_R_HEIGHT, 0.0f));

		// �J����(3D)�̍���/����/������ݒ�
		GetCamera3D()->fHeight	= MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_V_HEIGHT;
		GetCamera3D()->rot.y	= MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ROT;
		GetCamera3D()->fLength	= MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH;

		break;
	}
	case /*/ �`���[�g���A��[01] /*/MD_GAME_00_STATE_TUTORIAL_01: {
		// UI:�_�C�A���O�{�b�N�X[00] �̐ݒ菈��(���:02)
		SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_02);

		// �J����(3D)�̒����_��CHR:�v���C���[[00] �ɐݒ�
		SetCamera3DPosR(
			GetChr_player_00()->pos +
			D3DXVECTOR3(0.0f, MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_R_HEIGHT, 0.0f));

		// �J����(3D)�̍���/������ݒ�
		GetCamera3D()->fHeight = MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_V_HEIGHT;
		GetCamera3D()->fLength = MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_LENGTH;

		break;
	}
	case /*/ �{�X�o�����[�r�[(���_�ړ�) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE: {
		SetChr_player_00ControlState(	// CHR:�v���C���[[00] �̊Ǘ���Ԃ�ÓI�ɂ���
			CHR_PLAYER_00_CONTROL_STATE_STATIC);
		SetChr_enemy_00ControlState(	// CHR:�G[00] �̊Ǘ���Ԃ��{�X�A�s�[���ɂ���
			CHR_ENEMY_00_CONTROL_STATE_BOSS_APPEAL);

		// �O�̒����_�̈ʒu��ۑ�
		pMd->previousPosR = GetCamera3D()->posR;

		break;
	}
	case /*/ �{�X�o�����[�r�[(�o��) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_POP: {
		// OBJ:�X�e�[�W[00] �̃{�X�ݒ菈��
		BossSetObj_stage_00();

		break;
	}
	case /*/ �{�X�o�����[�r�[(���_���Z�b�g) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET: {

		break;
	}
	case /*/ �R�A�j�󃀁[�r�[(�g��) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM: {
		SetStateObj_stage_00(				// OBJ:�X�e�[�W[00] �̏�Ԃ��~�ɂ���
			OBJ_STAGE_00_STATE_STOP);
		SetObj_turret_00ControlState(		// OBJ:�^���b�g[00] �̊Ǘ���Ԃ�ÓI�ɂ���
			OBJ_TURRET_00_CONTROL_STATE_STATIC);
		SetObj_discharger_00ControlState(	// OBJ:���d���u[00] �̊Ǘ���Ԃ�ÓI�ɂ���
			OBJ_DISCHARGER_00_CONTROL_STATE_STATIC);
		SetChr_enemy_00ControlState(		// CHR:�G[00] �̊Ǘ���Ԃ�ÓI�ɂ���
			CHR_ENEMY_00_CONTROL_STATE_STATIC);
		SetChr_player_00ControlState(		// CHR:�v���C���[[00] �̊Ǘ���Ԃ�ÓI�ɂ���
			CHR_PLAYER_00_CONTROL_STATE_STATIC);

		// �J����(3D)�̒����_��OBJ:�R�A[00] �ɐݒ�
		SetCamera3DPosR(
			GetObj_core_00()->pos + 
			D3DXVECTOR3(0.0f, MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_R_HEIGHT, 0.0f));

		// �J����(3D)�̍�����ݒ�
		GetCamera3D()->fHeight = MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_V_HEIGHT;

		break;
	}
	case /*/ �R�A�j�󃀁[�r�[(����) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION: {
		DestroyObj_core_00();		// OBJ:�R�A[00] �̔j�󏈗�
		SetColorLight3D(INITCOLOR);	// OBJ:�R�A[00] �̐F��������

		break;
	}
	}
}

//========================================
// EndMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃɉ������I������ -
//========================================
void EndMd_game_00State(void) 
{
	Md_game_00	*pMd	// MD:�Q�[�����[00] �̏��
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	case /*/ �|�[�Y /*/MD_GAME_00_STATE_PAUSE: {

		break;
	}
	case /*/ ���[�U�[�K�C�h /*/MD_GAME_00_STATE_USERGUID: {
		
		break;
	}
	case /*/ �N���A���o /*/MD_GAME_00_STATE_CLEAR: {
		SetChr_player_00ControlState(	// CHR:�v���C���[[00] �̊Ǘ���Ԃ�ÓI�ɂ���
			CHR_PLAYER_00_CONTROL_STATE_STATIC);

		break;
	}
	case /*/ �����L���O /*/MD_GAME_00_STATE_RANKING: {

		break;
	}
	case /*/ ���U���g /*/MD_GAME_00_STATE_RESULT: {

		break;
	}
	case /*/ �`���[�g���A��[00] /*/MD_GAME_00_STATE_TUTORIAL_00: {

		break;
	}
	case /*/ �`���[�g���A��[01] /*/MD_GAME_00_STATE_TUTORIAL_01: {
		SetChr_player_00ControlState(	// CHR:�v���C���[[00] �̊Ǘ���Ԃ𓮓I�ɂ���
			CHR_PLAYER_00_CONTROL_STATE_DYNAMIC);

		// �J����(3D)�̃p�����[�^�[������
		InitParameterCamera3D();

		break;
	}
	case /*/ �{�X�o�����[�r�[(���_�ړ�) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE: {

		break;
	}
	case /*/ �{�X�o�����[�r�[(�o��) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_POP: {

		break;
	}
	case /*/ �{�X�o�����[�r�[(���_���Z�b�g) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET: {
		SetChr_player_00ControlState(	// CHR:�v���C���[[00] �̏�Ԃ𓮓I�ɂ���
			CHR_PLAYER_00_CONTROL_STATE_DYNAMIC);
		SetChr_enemy_00ControlState(	// CHR:�G[00] �̊Ǘ���Ԃ𓮓I�ɂ���
			CHR_ENEMY_00_CONTROL_STATE_DYNAMIC);

		break;
	}
	case /*/ �R�A�j�󃀁[�r�[(�g��) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM: {
		// BGM���ꊇ��~
		StopSoundType(SOUND_TYPE_BGM);

		// BGM�̉��ʂ����ɖ߂�
		SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f);
		
		break;
	}
	case /*/ �R�A�j�󃀁[�r�[(����) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION: {

		break;
	}
	}
}

//========================================
// UpdateMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃɉ������X�V���� -
//========================================
void UpdateMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:�Q�[�����[00] �̏��
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_GAME_00_STATE_NORMAL: {
		if (GetObj_core_00()->nHP <= 0)
		{// OBJ:�R�A[00] ��HP��0�ȉ��̎��A
			SetMd_game_00State(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM);	// ��Ԃ��R�A�j�󃀁[�r�[(�g��)�ɐݒ�
			break;
		}

		if ((GetKeyboardTrigger(DIK_P)) || (GetButtonTrigger(BUTTON_START))) 
		{// �|�[�Y�L�[�������ꂽ���A
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);	// ��Ԃ��|�[�Y�ɐݒ�
			PlaySound(MD_GAME_00_PAUSE_SE);				// �|�[�YSE�Đ�
		}

		break;
	}
	case /*/ �|�[�Y /*/MD_GAME_00_STATE_PAUSE: {
		// �|�[�Y���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ �ĊJ /*/MD_GAME_00_PAUSE_MENU_RESUME: {
			// ��Ԃ�ʏ�ɐݒ�
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
			break;
		}
		case /*/ ���g���C /*/MD_GAME_00_PAUSE_MENU_RETRY: {
			// ��ʂ��Q�[�����[00] �ɐݒ�
			SetFade(MODE_GAME_00);

			// ���g���CSE���Đ�
			PlaySound(MD_GAME_00_RETRY_SE);
			break;
		}
		case /*/ ���[�U�[�K�C�h /*/MD_GAME_00_PAUSE_MENU_USERGUID: {
			// ��Ԃ����[�U�[�K�C�h�ɂ���
			SetMd_game_00State(MD_GAME_00_STATE_USERGUID);
			break;
		}
		case /*/ �ݒ� /*/MD_GAME_00_PAUSE_MENU_SETTINGS: {
			// UI:���j���[[00] �̒��S���W��ݒ�
			SetUi_menu_00Pos(MD_GAME_00_SETTING_MENU_POS);

			// �ݒ胁�j���[�̐ݒ菈��
			SetSettingMenu();

			// ��Ԃ�ݒ�ɂ���
			SetMd_game_00State(MD_GAME_00_STATE_SETTING);
			break;
		}
		case /*/ �^�C�g���ɖ߂� /*/MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE: {
			// ��ʂ��^�C�g�����[00] �ɐݒ�
			SetFade(MODE_TITLE_00);
			break;
		}
		}

		break;
	}
	case /*/ ���[�U�[�K�C�h /*/MD_GAME_00_STATE_USERGUID: {
		// ���[�U�[�K�C�h���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ OK /*/MD_GAME_00_USERGUID_MENU_OK:
			// UI:���[�U�[�K�C�h[00] �̏�Ԃ��������ɂ���
			SetStateUi_userGuid_00(UI_USERGUID_00_STATE_IN_CLEAR);
			break;
		}

		if (GetUi_userGuid_00()->state == UI_USERGUID_00_STATE_CLEAR)
		{// UI:���[�U�[�K�C�h[00] �̏�Ԃ������Ă��鎞�A
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);	// ��Ԃ��|�[�Y�ɂ���
		}

		break;
	}
	case /*/ �ݒ� /*/MD_GAME_00_STATE_SETTING: {
		// �ݒ胁�j���[�̍X�V����
		UpdateSettingMenu();

		if (!*GetSettingNow())
		{// �ݒ蒆�t���O���U�̎��A
			SetMd_game_00State(MD_GAME_00_STATE_PAUSE);	// ��Ԃ��|�[�Y�ɂ���
		}

		break;
	}
	case /*/ �N���A���o /*/MD_GAME_00_STATE_CLEAR: {
		pMd->nCounterState++;	// ��ԃJ�E���^�[�����Z

		if (pMd->nCounterState == MD_GAME_00_CLEAR_AFTERGLOW) 
		{// ��Ԃ̃J�E���^�[���N���A��̗]�C�ɒB�������A
			PlaySound(MD_GAME_00_CLEAR_SE);	// �N���ASE���Đ�
		}

		if (pMd->nCounterState >= MD_GAME_00_CLEAR_TIME) 
		{// ��Ԃ̃J�E���^�[���N���A���o�ɂ����鎞�ԂɒB�������A
			SetMd_game_00State(MD_GAME_00_STATE_RANKING);	// ��Ԃ������L���O�ɐݒ�
		}

		if (pMd->nCounterState >= MD_GAME_00_CLEAR_AFTERGLOW)
		{// ��Ԃ̃J�E���^�[���N���A��̗]�C�ȏ�̎��A
			{//========== *** �N���A�e�L�X�g ***
				int nTextCounter = IntControlReturn(pMd->nCounterState - MD_GAME_00_CLEAR_AFTERGLOW, MD_GAME_00_CLEAR_TEXT_SHOW_TIME, 0);
				int nStrLen = strlen(MD_GAME_00_CLEAR_TEXT_DISPLAY);
				int nTextTime = MD_GAME_00_CLEAR_TEXT_SHOW_TIME / nStrLen;

				if ((nTextCounter % nTextTime == 0) &&
					(nTextCounter / nTextTime < nStrLen))
				{// �e�L�X�g�J�E���^�[���N���A�e�L�X�g�ɂ����鎞�ԂŊ���؂�鎞�A
					PlaySound(MD_GAME_00_TEXT_SET_SE);	// �e�L�X�g�Z�b�gSE���Đ�
				}

				char aString[TXT_MAX];
				sprintf(aString, MD_GAME_00_CLEAR_TEXT_DISPLAY);
				int nText = (nTextCounter / nTextTime) + 1;
				IntControl(&nText, nStrLen, 0);
				aString[(nTextCounter / nTextTime) + 1] = '\0';
				SetText2D(
					aString,
					FONT_003,
					DISPLAY_CENTER,
					MD_GAME_00_CLEAR_TEXT_POS,
					MD_GAME_00_CLEAR_TEXT_COLOR,
					MD_GAME_00_CLEAR_TEXT_SCALE,
					MD_GAME_00_CLEAR_TEXT_SCALE);
			}
			if(pMd->nCounterState >= MD_GAME_00_CLEAR_TEXT_SHADOW_POP_TIME)
			{//========== *** �N���A�e�L�X�g(�e) ***
				int nShadowCounter = IntControlReturn(pMd->nCounterState - MD_GAME_00_CLEAR_TEXT_SHADOW_POP_TIME, MD_GAME_00_CLEAR_TEXT_SHADOW_CLEAR_TIME, 0);
				float fRate = (float)nShadowCounter / (float)MD_GAME_00_CLEAR_TEXT_SHADOW_CLEAR_TIME;
				Color setCol = MD_GAME_00_CLEAR_TEXT_COLOR; setCol.a *= (1.0f - fRate);
				float fScale = 
					(MD_GAME_00_CLEAR_TEXT_SCALE * (1.0f - fRate)) +
					(MD_GAME_00_CLEAR_TEXT_SHADOW_SCALE * fRate);

				SetText2D(
					MD_GAME_00_CLEAR_TEXT_DISPLAY,
					FONT_001,
					DISPLAY_CENTER,
					MD_GAME_00_CLEAR_TEXT_POS,
					setCol,
					fScale,
					fScale);
			}
		}

		break;
	}
	case /*/ �����L���O /*/MD_GAME_00_STATE_RANKING: {
		if (GetUi_rankingFrame_00()->state == UI_RANKINGFRAME_00_STATE_POP)
		{// UI:�����L���O�t���[��[00] �̏�Ԃ��o���̎��A
			SetMd_game_00State(MD_GAME_00_STATE_RESULT);	// ��Ԃ����U���g�ɂ���
		}

		break;
	}
	case /*/ ���U���g /*/MD_GAME_00_STATE_RESULT: {
		// ���U���g���j���[�̑I������
		switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
		{
		case /*/ ���g���C /*/MD_GAME_00_RESULT_MENU_RETRY: {
			// ��ʂ��Q�[�����[00] �ɐݒ�
			SetFade(MODE_GAME_00);
			break;
		}
		case /*/ �^�C�g���ɖ߂� /*/MD_GAME_00_RESULT_MENU_BACK_TO_TITLE: {
			// ��ʂ��^�C�g�����[00] �ɐݒ�
			SetFade(MODE_TITLE_00);
			break;
		}
		}
		break;
	}
	case /*/ �`���[�g���A��[00] /*/MD_GAME_00_STATE_TUTORIAL_00: {
		float	fRate	// �J�E���^�[�̐i�s��
				= (float)pMd->nCounterState / (float)MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME;

		// �J����(3D)�̋������X�V
		GetCamera3D()->fLength =
			(MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH * (1.0f - fRate)) +
			(MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_END_LENGTH * fRate);

		pMd->nCounterState++;	// ��ԃJ�E���^�[�����Z
		IntControl(				// ��ԃJ�E���^�[�𐧌�
			&pMd->nCounterState,
			MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME, 0);

		if (pMd->nCounterState == MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME)
		{// ��ԃJ�E���^�[���g��ɂ����鎞�ԂɒB���Ă��鎞�A
			if (GetUi_dialogBox_00()->state == UI_DIALOGBOX_00_STATE_CLEAR)
			{//  UI:�_�C�A���O�{�b�N�X[00] �̏�Ԃ������Ă��鎞
				switch (GetUi_dialogBox_00()->type)
				{
				case /*/ ���� /*/UI_DIALOGBOX_00_TYPE_NONE:
					// UI:�_�C�A���O�{�b�N�X[00] �̐ݒ菈��(���:00)
					SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_00);
					break;
				case /*/ 00 /*/UI_DIALOGBOX_00_TYPE_00:
					// UI:�_�C�A���O�{�b�N�X[00] �̐ݒ菈��(���:01)
					SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_01);
					break;
				case /*/ 01 /*/UI_DIALOGBOX_00_TYPE_01:
					// ��Ԃ��`���[�g���A��[01]�ɐݒ�
					SetMd_game_00State(MD_GAME_00_STATE_TUTORIAL_01);
					break;
				}
			}
		}

		break;
	}
	case /*/ �`���[�g���A��[01] /*/MD_GAME_00_STATE_TUTORIAL_01: {
		if ((GetUi_dialogBox_00()->state == UI_DIALOGBOX_00_STATE_CLEAR)
			&&
			(GetFade() == FADE_NONE))
		{//  UI:�_�C�A���O�{�b�N�X[00] �̏�Ԃ������Ă��� & �t�F�[�h�����̎��A
			switch (GetUi_dialogBox_00()->type)
			{
			case /*/ 02 /*/UI_DIALOGBOX_00_TYPE_02:
				// UI:�_�C�A���O�{�b�N�X[00] �̐ݒ菈��(���:03)
				SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_03);
				break;
			case /*/ 03 /*/UI_DIALOGBOX_00_TYPE_03:
				// UI:�_�C�A���O�{�b�N�X[00] �̐ݒ菈��(���:04)
				SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE_04);
				break;
			case /*/ 04 /*/UI_DIALOGBOX_00_TYPE_04:
				SetFade(MODE_NONE);	// �t�F�[�h�ݒ�
				break;
			}
		}

		pMd->nCounterState++;	// ��ԃJ�E���^�[�����Z
		IntControl(				// ��ԃJ�E���^�[�𐧌�
			&pMd->nCounterState,
			MD_GAME_00_STATE_TUTORIAL_01_TIME, 0);

		if (GetFade() == FADE_IN) 
		{// �t�F�[�h�C����Ԃ̎��A
			// ��Ԃ�ʏ�ɐݒ�
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

			// �J����(3D)�̌�����ݒ�
			GetCamera3D()->rot = MD_GAME_00_CAMERA3D_INIT_ROT;
		}

		break;
	}
	case /*/ �{�X�o�����[�r�[(���_�ړ�) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE: {
		float	fRate	// �J�E���^�[�̐i�s��
				= ((float)pMd->nCounterState / (float)MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE_TIME);

		// �J����(3D)�̒����_�����݂̒����_����{�X�ݒ�ʒu�ɐ��ڂ�����
		GetCamera3D()->posR =
			(GetObj_stage_00()->setBoss.pos * fRate) +
			(GetCamera3D()->posR * (1.0f - fRate));

		if (++pMd->nCounterState >= MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʃ{�X�o�����[�r�[(���_�ړ�)�ɂ����鎞�ԂɒB�������A
			SetMd_game_00State(MD_GAME_00_STATE_BOSS_POP_MOVIE_POP);	// ��Ԃ��{�X�o�����[�r�[(�o��)�ɐݒ�
		}

		break;
	}
	case /*/ �{�X�o�����[�r�[(�o��) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_POP: {
		
		break;
	}
	case /*/ �{�X�o�����[�r�[(���_���Z�b�g) /*/MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET: {
		float	fRate	// �J�E���^�[�̐i�s��
				= ((float)pMd->nCounterState / (float)MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET_TIME);

		// �J����(3D)�̒����_�����݂̒����_����O�̒����_�̈ʒu�ɐ��ڂ�����
		GetCamera3D()->posR =
			(pMd->previousPosR * fRate) +
			(GetCamera3D()->posR * (1.0f - fRate));

		if (++pMd->nCounterState >= MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʃ{�X�o�����[�r�[(���_���Z�b�g)�ɂ����鎞�ԂɒB�������A
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);	// ��Ԃ�ʏ�ɐݒ�
		}

		break;
	}
	case /*/ �R�A�j�󃀁[�r�[(�g��) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM: {
		float	fRate	// ����
				= ((float)pMd->nCounterState / (float)MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_TIME);

		// ��(3D)�̖��邳��ݒ�
		SetBrightnessLight3D(1.0f - fRate);

		if (++pMd->nCounterState >= MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_TIME) 
		{// ��ԃJ�E���^�[�����Z�������ʃR�A�j�󃀁[�r�[(�g��)�ɂ����鎞�ԂɒB�������A
			SetMd_game_00State(MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION);	// ��Ԃ��R�A�j�󃀁[�r�[(����)�ɐݒ�
		}

		// BGM�̉��ʂ��X�V
		SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - fRate);

		break;
	}
	case /*/ �R�A�j�󃀁[�r�[(����) /*/MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION: {
		float	fRate	// ����
				= Prus(
				(float)(pMd->nCounterState - MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_BRIGHT_TIME) /
				(float)(MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_TIME - MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_BRIGHT_TIME));

		// ��(3D)�̖��邳��ݒ�
		SetBrightnessLight3D(fRate);

		if (++pMd->nCounterState >= MD_GAME_00_CORE_DESTROY_MOVIE_EXPLOSION_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʃR�A�j�󃀁[�r�[(����)�ɂ����鎞�ԂɒB�������A
			SetMd_game_00State(MD_GAME_00_STATE_RANKING);	// ��Ԃ������L���O�ɐݒ�
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
// InitMd_game_00�֐� - MD:�Q�[�����[00]�̏��������� -
//========================================
void InitMd_game_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterMd_game_00();

	// ����������
	InitParameterCamera3D();	// �J����(3D)�̃p�����[�^�[
	InitAtk_bullet_00();		// ATK:�e					[00]
	InitAtk_impact_00();		// ATK:�Ռ�					[00]
	InitAtk_sword_00();			// ATK:��					[00]
	InitBg_mountain_00();		// BG :�R					[00]
	InitBg_space_00();			// BG :�F��					[00]
	InitChr_player_00();		// CHR:�v���C���[			[00]
	InitChr_enemy_00();			// CHR:�G					[00]
	InitEff_explosion_00();		// EFF:����					[00]
	InitEff_light_00();			// EFF:��					[00]
	InitEff_particle_00();		// EFF:�p�[�e�B�N��			[00]
	InitEff_screen_00();		// EFF:�X�N���[��			[00]
	InitEff_shadow_00();		// EFF:�e					[00]
	InitEff_shockWave_00();		// EFF:�Ռ��g				[00]
	InitItm_coin_00();			// ITM:�R�C��				[00]
	InitObj_block_00();			// OBJ:�u���b�N				[00]
	InitObj_core_00();			// OBJ:�R�A					[00]
	InitObj_discharger_00();	// OBJ:���d���u				[00]
	InitObj_mirror_00();		// OBJ:�~���[				[00]
	InitObj_pedestal_00();		// OBJ:���					[00]
	InitUi_attentionMark_00();	// UI :���Ӄ}�[�N			[00]
	InitObj_turret_00();		// OBJ:�^���b�g				[00]
	InitObj_switch_00();		// OBJ:�X�C�b�`				[00]
	InitObj_stage_00(MD_GAME_00_STAGE_TYPE);	// OBJ:�X�e�[�W				[00]
	InitObj_target_00();		// OBJ:�^�[�Q�b�g			[00]
	InitUi_inputGuid_00();		// UI :���̓K�C�h			[00]
	InitUi_coinFrame_00();		// UI :�R�C���t���[��		[00]
	InitUi_damage_00();			// UI :�_���[�W				[00]
	InitUi_dialogBox_00();		// UI :�_�C�A���O�{�b�N�X	[00]
	InitUi_frame_00();			// UI :�t���[��				[00]
	InitUi_hpBar_00();			// UI :HP�o�[				[00]
	InitUi_hpBar_01();			// UI :HP�o�[				[01]
	InitUi_inventory_00();		// UI :�C���x���g��			[00]
	InitUi_itemGuid_00();		// UI :���i����				[00]
	InitUi_itemIcon_00();		// UI :�A�C�e���A�C�R��		[00]
	InitUi_map_00();			// UI :�}�b�v				[00]
	InitUi_parameter_00();		// UI :�p�����[�^�[			[00]
	InitUi_menu_00();			// UI :���j���[				[00]
	InitUi_rankingFrame_00();	// UI :�����L���O�t���[��	[00]
	InitUi_tips_00();			// UI :TIPS					[00]
	InitUi_userGuid_00();		// UI :���[�U�[�K�C�h		[00]
	InitUi_warningText_00();	// UI :���Ӄe�L�X�g			[00]

	// ��Ԃ��`���[�g���A��[00]�ɐݒ�
	SetMd_game_00State(MD_GAME_00_STATE_TUTORIAL_00);

	SetColorLight3D(INITCOLOR);	// OBJ:�R�A[00] �̐F��������
}

//========================================
// UninitMd_game_00�֐� - MD:�Q�[�����[00]�̏I������ -
//========================================
void UninitMd_game_00(void)
{
	// �I������
	UninitAtk_bullet_00();			// ATK:�e					[00]
	UninitAtk_impact_00();			// ATK:�Ռ�					[00]
	UninitAtk_sword_00();			// ATK:��					[00]
	UninitBg_mountain_00();			// BG :�R					[00]
	UninitBg_space_00();			// BG :�F��					[00]
	UninitChr_player_00();			// CHR:�v���C���[			[00]
	UninitChr_enemy_00();			// CHR:�G					[00]
	UninitEff_explosion_00();		// EFF:����					[00]
	UninitEff_light_00();			// EFF:��					[00]
	UninitEff_particle_00();		// EFF:�p�[�e�B�N��			[00]
	UninitEff_screen_00();			// EFF:�X�N���[��			[00]
	UninitEff_shadow_00();			// EFF:�e					[00]
	UninitEff_shockWave_00();		// EFF:�Ռ��g				[00]
	UninitItm_coin_00();			// ITM:�R�C��				[00]
	UninitObj_block_00();			// OBJ:�u���b�N				[00]
	UninitObj_core_00();			// OBJ:�R�A					[00]
	UninitObj_discharger_00();		// OBJ:���d���u				[00]
	UninitObj_mirror_00();			// OBJ:�~���[				[00]
	UninitObj_pedestal_00();		// OBJ:���					[00]
	UninitObj_stage_00();			// OBJ:�X�e�[�W				[00]
	UninitObj_switch_00();			// OBJ:�X�C�b�`				[00]
	UninitObj_target_00();			// OBJ:�^�[�Q�b�g			[00]
	UninitObj_turret_00();			// OBJ:�^���b�g				[00]
	UninitUi_attentionMark_00();	// UI :���Ӄ}�[�N			[00]
	UninitUi_inputGuid_00();		// UI :���̓K�C�h			[00]
	UninitUi_coinFrame_00();		// UI :�R�C���t���[��		[00]
	UninitUi_damage_00();			// UI :�_���[�W				[00]
	UninitUi_dialogBox_00();		// UI :�_�C�A���O�{�b�N�X	[00]
	UninitUi_frame_00();			// UI :�t���[��				[00]
	UninitUi_hpBar_00();			// UI :HP�o�[				[00]
	UninitUi_hpBar_01();			// UI :HP�o�[				[01]
	UninitUi_inventory_00();		// UI :�C���x���g��			[00]
	UninitUi_itemGuid_00();			// UI :���i����				[00]
	UninitUi_itemIcon_00();			// UI :�A�C�e���A�C�R��		[00]
	UninitUi_map_00();				// UI :�}�b�v				[00]
	UninitUi_parameter_00();		// UI :�p�����[�^�[			[00]
	UninitUi_menu_00();				// UI :���j���[				[00]
	UninitUi_rankingFrame_00();		// UI :�����L���O�t���[��	[00]
	UninitUi_tips_00();				// UI :TIPS					[00]
	UninitUi_userGuid_00();			// UI :���[�U�[�K�C�h		[00]
	UninitUi_warningText_00();		// UI :���Ӄe�L�X�g			[00]
}

//========================================
// UpdateMd_game_00�֐� - MD:�Q�[�����[00]�̍X�V���� -
//========================================
void UpdateMd_game_00(void)
{
	UpdateUi_frame_00();				// UI :�t���[��				[00]
	UpdateUi_menu_00();					// UI :���j���[				[00]
	if ((g_md_game_00.state != MD_GAME_00_STATE_PAUSE) &&
		(g_md_game_00.state != MD_GAME_00_STATE_USERGUID) && 
		(g_md_game_00.state != MD_GAME_00_STATE_SETTING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RANKING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RESULT))
	{// ��Ԃ��|�[�Y/���[�U�[�K�C�h/�ݒ�/�����L���O/���U���g�Ŗ������A
		// �X�V����
		UpdateBg_mountain_00();			// BG :�R					[00]
		UpdateBg_space_00();			// BG :�F��					[00]
		UpdateItm_coin_00();			// ITM:�R�C��				[00]
		UpdateChr_enemy_00();			// CHR:�G					[00]
		UpdateChr_player_00();			// CHR:�v���C���[			[00]
		UpdateAtk_sword_00();			// ATK:��					[00]
		UpdateEff_explosion_00();		// EFF:����					[00]
		UpdateEff_light_00();			// EFF:��					[00]
		UpdateEff_particle_00();		// EFF:�p�[�e�B�N��			[00]
		UpdateEff_screen_00();			// EFF:�X�N���[��			[00]
		UpdateEff_shadow_00();			// EFF:�e					[00]
		UpdateEff_shockWave_00();		// EFF:�Ռ��g				[00]
		UpdateAtk_bullet_00();			// ATK:�e					[00]
		UpdateAtk_impact_00();			// ATK:�Ռ�					[00]
		UpdateObj_block_00();			// OBJ:�u���b�N				[00]
		UpdateObj_core_00();			// OBJ:�R�A					[00]
		UpdateObj_discharger_00();		// OBJ:���d���u				[00]
		UpdateObj_mirror_00();			// OBJ:�~���[				[00]
		UpdateObj_stage_00();			// OBJ:�X�e�[�W				[00]
		UpdateObj_target_00();			// OBJ:�^�[�Q�b�g			[00]
		UpdateObj_turret_00();			// OBJ:�^���b�g				[00]
		UpdateObj_pedestal_00();		// OBJ:���					[00]
		UpdateUi_attentionMark_00();	// UI :���Ӄ}�[�N			[00]
		UpdateUi_inputGuid_00();		// UI :���̓K�C�h			[00]
		UpdateUi_itemGuid_00();			// UI :���i����				[00]
		UpdateUi_itemIcon_00();			// UI :�A�C�e���A�C�R��		[00]
		UpdateObj_switch_00();			// OBJ:�X�C�b�`				[00]
		if (g_md_game_00.state == MD_GAME_00_STATE_NORMAL)
		{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�̎��A
			UpdateUi_coinFrame_00();	// UI :�R�C���t���[��		[00]
			UpdateUi_damage_00();		// UI :�_���[�W				[00]
			UpdateUi_hpBar_00();		// UI :HP�o�[				[00]
			UpdateUi_hpBar_01();		// UI :HP�o�[				[01]
			UpdateUi_inventory_00();	// UI :�C���x���g��			[00]
			UpdateUi_map_00();			// UI :�}�b�v				[00]
			UpdateUi_parameter_00();	// UI :�p�����[�^�[			[00]
		}
		UpdateUi_dialogBox_00();		// UI :�_�C�A���O�{�b�N�X	[00]
		UpdateUi_warningText_00();		// UI :���Ӄe�L�X�g			[00]
	}
	UpdateUi_rankingFrame_00();			// UI :�����L���O�t���[��	[00]
	UpdateUi_tips_00();					// UI :TIPS					[00]
	UpdateUi_userGuid_00();				// UI :���[�U�[�K�C�h		[00]

	// MD:�Q�[�����[00] �̏�Ԃɉ������X�V����
	UpdateMd_game_00State();
}

//========================================
// DrawMd_game_00�֐� - MD:�Q�[�����[00]�̕`�揈�� -
//========================================
void DrawMd_game_00(void)
{
	// �`�揈��
	DrawAtk_bullet_00();			// ATK:�e					[00]
	DrawAtk_sword_00();				// ATK:��					[00]
	DrawBg_space_00();				// BG :�F��					[00]
	DrawBg_mountain_00();			// BG :�R					[00]
	DrawChr_enemy_00();				// CHR:�G					[00]
	DrawObj_block_00();				// OBJ:�u���b�N				[00]
	DrawObj_core_00();				// OBJ:�R�A					[00]
	DrawObj_discharger_00();		// OBJ:���d���u				[00]
	DrawObj_pedestal_00();			// OBJ:���					[00]
	DrawObj_stage_00();				// OBJ:�X�e�[�W				[00]
	DrawObj_switch_00();			// OBJ:�X�C�b�`				[00]
	DrawObj_target_00();			// OBJ:�^�[�Q�b�g			[00]
	DrawObj_turret_00();			// OBJ:�^���b�g				[00]
	DrawChr_player_00();			// CHR:�v���C���[			[00]
	DrawEff_explosion_00();			// EFF:����					[00]
	DrawEff_light_00();				// EFF:��					[00]
	DrawEff_shadow_00();			// EFF:�e					[00]
	DrawEff_shockWave_00();			// EFF:�Ռ��g				[00]
	DrawItm_coin_00();				// ITM:�R�C��				[00]
	DrawObj_mirror_00();			// OBJ:�~���[				[00]
	DrawUi_attentionMark_00();		// UI :���Ӄ}�[�N			[00]
	DrawUi_menu_00();				// UI :���j���[				[00]
	if ((g_md_game_00.state != MD_GAME_00_STATE_PAUSE) &&
		(g_md_game_00.state != MD_GAME_00_STATE_USERGUID) &&
		(g_md_game_00.state != MD_GAME_00_STATE_SETTING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RANKING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RESULT))
	{// ��Ԃ��|�[�Y/���[�U�[�K�C�h/�ݒ�/�����L���O/���U���g�Ŗ������A
		DrawUi_itemIcon_00();		// UI :�A�C�e���A�C�R��		[00]
	}
	DrawText3D();					// �e�L�X�g(3D)
	if (g_md_game_00.state == MD_GAME_00_STATE_NORMAL)
	{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�̎��A
		DrawUi_hpBar_01();			// UI :HP�o�[				[01]
		DrawUi_hpBar_00();			// UI :HP�o�[				[00]
		DrawUi_coinFrame_00();		// UI :�R�C���t���[��		[00]
		DrawUi_inventory_00();		// UI :�C���x���g��			[00]
		DrawUi_map_00();			// UI :�}�b�v				[00]
		DrawUi_parameter_00();		// UI :�p�����[�^�[			[00]
	}
	DrawUi_dialogBox_00();			// UI :�_�C�A���O�{�b�N�X	[00]
	DrawUi_rankingFrame_00();		// UI :�����L���O�t���[��	[00]
	if (g_md_game_00.state == MD_GAME_00_STATE_NORMAL)
	{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�̎��A
		DrawUi_warningText_00();	// UI :���Ӄe�L�X�g			[00]
		DrawUi_itemGuid_00();		// UI :���i����				[00]
	}
	DrawUi_frame_00();				// UI :�t���[��				[00]
	if ((g_md_game_00.state != MD_GAME_00_STATE_PAUSE) &&
		(g_md_game_00.state != MD_GAME_00_STATE_USERGUID) &&
		(g_md_game_00.state != MD_GAME_00_STATE_SETTING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RANKING) &&
		(g_md_game_00.state != MD_GAME_00_STATE_RESULT))
	{// ��Ԃ��|�[�Y/���[�U�[�K�C�h/�ݒ�/�����L���O/���U���g�Ŗ������A
		DrawUi_inputGuid_00();		// UI :���̓K�C�h			[00]
	}
	DrawUi_tips_00();				// UI :TIPS					[00]
	DrawUi_userGuid_00();			// UI :���[�U�[�K�C�h		[00]
	DrawText2D();					// �e�L�X�g(2D)
	DrawEff_screen_00();			// EFF:�X�N���[��			[00]
}

//========================================
// SetMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃ�ݒ� -
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state)
{
	// ��Ԃ̃J�E���^�[��������
	g_md_game_00.nCounterState = 0;

	// MD:�Q�[�����[00] �̏�Ԃɉ������I������
	EndMd_game_00State();

	// MD:�Q�[�����[00] �̏�Ԃ���
	g_md_game_00.state = state;

	// MD:�Q�[�����[00] �̏�Ԃɉ������J�n����
	StartMd_game_00State();
}