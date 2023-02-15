//========================================
// 
// �ݒ�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** setting.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "setting.h"
#include "ui_menu_00.h"	// UI:���j���[[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// �ݒ�̏��̃f�[�^�t�@�C���̑��΃p�X
#define SETTING_DATA_FILE_PATH	"data\\SAVEDATA\\setting.bin"

// ���ʂ̕\���{��
#define VOLUME_DISPLAY_DIAMETER	(5)

// ��ʃ��[�h�̐؂�ւ��ɂ����鎞��
#define WINDOW_MODE_CHANGE_TIME	(20)

//****************************************
// �񋓌^�̒�`
//****************************************
// �ݒ胁�j���[
typedef enum
{
	SETTING_MENU_BGM_VOLUME = 0,	// BGM����
	SETTING_MENU_SE_VOLUME,			// SE����
	SETTING_MENU_SCREEN_MODE,		// ��ʃ��[�h
	SETTING_MENU_VIBRATION,			// �U��
	SETTING_MENU_OK,				// ����
	SETTING_MENU_MAX,
}SETTING_MENU;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �ݒ胁�j���[�̃e�L�X�g��ݒ�
void SetSettingMenuText(void);

//****************************************
// �O���[�o���錾
//****************************************
int		g_nCounterScreenModeChange	// ��ʃ��[�h�؂�ւ��J�E���^�[
		= -1;
bool	g_bSettingNow;				// �ݒ蒆�t���O

// �ݒ�̏��
Setting g_setting =
{// �����ݒ�
	VOLUME_STAGE / 2,
	VOLUME_STAGE / 2,
	true,
};

// �ݒ胁�j���[�̐ݒ���
Ui_menu_00Set g_aSettingMenuSet[SETTING_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_NORMAL	,"OK" , true },
};

// ��ʃ��[�h�̕\���`��
char g_aScreenModeText[2][TXT_MAX] =
{
	"FULL: OFF ",
	"FULL: ON  ",
};


// �U���̕\���`��
char g_aVibrationText[2][TXT_MAX] =
{
	"VIB : OFF ",
	"VIB : ON  ",
};

//========== *** �ݒ�̏����擾 ***
Setting *GetSetting(void) 
{
	return &g_setting;
}

//========== *** �ݒ蒆�t���O���擾 ***
bool *GetSettingNow(void) 
{
	return &g_bSettingNow;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetSettingMenuText�֐� - �ݒ胁�j���[�̃e�L�X�g��ݒ� -
//========================================
void SetSettingMenuText(void)
{
	Ui_menu_00	*pUi	// ���j���[[00] �̏��̃|�C���^ 
				= GetUi_menu_00();

	/*/ BGM����    /*/sprintf(pUi[SETTING_MENU_BGM_VOLUME].aString, "BGM : %-3d ", g_setting.nBGMVolume * VOLUME_DISPLAY_DIAMETER);
	/*/ SE����     /*/sprintf(pUi[SETTING_MENU_SE_VOLUME].aString, "SE  : %-3d ", g_setting.nSEVolume * VOLUME_DISPLAY_DIAMETER);
	/*/ ��ʃ��[�h /*/sprintf(pUi[SETTING_MENU_SCREEN_MODE].aString, g_aScreenModeText[g_setting.bFullScreen]);
	/*/ �U��       /*/sprintf(pUi[SETTING_MENU_VIBRATION].aString, g_aVibrationText[g_setting.bVibration]);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitSetting�֐� - �ݒ�̏��������� -
//========================================
void InitSetting(void)
{
	g_bSettingNow = false;	// �ݒ蒆�t���O��������
}

//========================================
// UninitSetting�֐� - �ݒ�̏I������ -
//========================================
void UninitSetting(void)
{

}

//========================================
// UpdateSetting�֐� - �ݒ�̍X�V���� -
//========================================
void UpdateSetting(void)
{
	if (g_nCounterScreenModeChange >= 0)
	{// ��ʃ��[�h�؂�ւ��J�E���^�[��0�ȏ�̎��A
		// ��ʃ��[�h�؂�ւ��J�E���^�[�����Z
		g_nCounterScreenModeChange--;

		if (g_nCounterScreenModeChange == 0)
		{// ��ʃ��[�h�؂�ւ��J�E���^�[��0�̎��A
			// �E�C���h�E�̃��[�h��؂�ւ���
			WindowModeChange(g_setting.bFullScreen);
		}
	}

	if (GetKeyboardTrigger(DIK_F11))
	{// F11�L�[�����͂��ꂽ���A
		// �S��ʃt���O��؂�ւ���
		g_setting.bFullScreen ^= 1;

		// ��ʃ��[�h�؂�ւ��J�E���^�[��ݒ�
		g_nCounterScreenModeChange = WINDOW_MODE_CHANGE_TIME;
	}
}

//========================================
// UpdateSettingMenu�֐� - �ݒ胁�j���[�̍X�V���� -
//========================================
void UpdateSettingMenu(void)
{
	switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
	{
	case /*/ BGM���� /*/SETTING_MENU_BGM_VOLUME: {
		if ((GetKeyboardRepeat(DIK_LEFT)) || (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
		{// ���ɓ��͂��ꂽ���A
			// BGM���ʂ����Z
			g_setting.nBGMVolume--;
		}
		else if ((GetKeyboardRepeat(DIK_RIGHT)) || (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// �E�ɓ��͂��ꂽ���A
			// BGM���ʂ����Z
			g_setting.nBGMVolume++;
		}

		// BGM���ʂ𐧌�
		IntControl(&g_setting.nBGMVolume, VOLUME_STAGE, 0);

		// �ݒ艹�ʂ�ݒ�
		SetSettingVolume(SOUND_TYPE_BGM);

		break;
	}
	case /*/ SE���� /*/SETTING_MENU_SE_VOLUME: {
		if ((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
		{// ���ɓ��͂��ꂽ���A
			// SE���ʂ����Z
			g_setting.nSEVolume--;
		}
		else if ((GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// �E�ɓ��͂��ꂽ���A
			// SE���ʂ����Z
			g_setting.nSEVolume++;
		}

		// SE���ʂ𐧌�
		IntControl(&g_setting.nSEVolume, VOLUME_STAGE, 0);

		// �ݒ艹�ʂ�ݒ�
		SetSettingVolume(SOUND_TYPE_SE);
	}
	break;
	case /*/ ��ʃ��[�h /*/SETTING_MENU_SCREEN_MODE: {
		if ((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// �����E�ɓ��͂��ꂽ���A
			// �S��ʃt���O��؂�ւ���
			g_setting.bFullScreen ^= 1;

			// ��ʃ��[�h�؂�ւ��J�E���^�[��ݒ�
			g_nCounterScreenModeChange = WINDOW_MODE_CHANGE_TIME;
		}

		break;
	}
	case /*/ �U�� /*/SETTING_MENU_VIBRATION: {
		if ((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// �����E�ɓ��͂��ꂽ���A
			 // �U���t���O��؂�ւ���
			g_setting.bVibration ^= 1;
		}

		break;
	}
	case /*/ ���� /*/SETTING_MENU_OK: {
		// �ݒ蒆�t���O���U�ɂ���
		g_bSettingNow = false;
		break;
	}
	}

	// �ݒ胁�j���[�̃e�L�X�g��ݒ�
	SetSettingMenuText();
}

//========================================
// SetSettingMenu�֐� - �ݒ胁�j���[�̐ݒ菈�� -
//========================================
void SetSettingMenu(void) 
{
	// ���j���[[00] �̐ݒ菈��
	SetUi_menu_00(
		g_aSettingMenuSet,
		SETTING_MENU_MAX);

	// �I��ԍ���BGM���ʂɐݒ�
	*GetSelect() = SETTING_MENU_BGM_VOLUME;

	// �ݒ蒆�t���O��^�ɂ���
	g_bSettingNow = true;
}

//========================================
// LoadSetting�֐� - �ݒ�̓ǂݍ��ݏ��� -
//========================================
void LoadSetting(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

	//�t�@�C�����J��
	pFile = fopen(SETTING_DATA_FILE_PATH, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�������A
		//�t�@�C������ݒ��ǂݍ���
		fread(&g_setting, sizeof(Setting), 1, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}

//========================================
// SaveSetting�֐� - �ݒ�̏������ݏ��� -
//========================================
void SaveSetting(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

	//�t�@�C�����J��
	pFile = fopen(SETTING_DATA_FILE_PATH, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�������A
		//�t�@�C���ɐݒ�������o��
		fwrite(&g_setting, sizeof(Setting), 1, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}