//========================================
// 
// 設定の処理
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
#include "ui_menu_00.h"	// UI:メニュー[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// 設定の情報のデータファイルの相対パス
#define SETTING_DATA_FILE_PATH	"data\\SAVEDATA\\setting.bin"

// 音量の表示倍率
#define VOLUME_DISPLAY_DIAMETER	(5)

// 画面モードの切り替えにかかる時間
#define WINDOW_MODE_CHANGE_TIME	(20)

//****************************************
// 列挙型の定義
//****************************************
// 設定メニュー
typedef enum
{
	SETTING_MENU_BGM_VOLUME = 0,	// BGM音量
	SETTING_MENU_SE_VOLUME,			// SE音量
	SETTING_MENU_SCREEN_MODE,		// 画面モード
	SETTING_MENU_VIBRATION,			// 振動
	SETTING_MENU_OK,				// 了解
	SETTING_MENU_MAX,
}SETTING_MENU;

//****************************************
// プロトタイプ宣言
//****************************************
// 設定メニューのテキストを設定
void SetSettingMenuText(void);

//****************************************
// グローバル宣言
//****************************************
int		g_nCounterScreenModeChange	// 画面モード切り替えカウンター
		= -1;
bool	g_bSettingNow;				// 設定中フラグ

// 設定の情報
Setting g_setting =
{// 初期設定
	VOLUME_STAGE / 2,
	VOLUME_STAGE / 2,
	true,
};

// 設定メニューの設定情報
Ui_menu_00Set g_aSettingMenuSet[SETTING_MENU_MAX] =
{
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_SELECTION	,""   , false },
	{ UI_MENU_00_TYPE_NORMAL	,"OK" , true },
};

// 画面モードの表示形式
char g_aScreenModeText[2][TXT_MAX] =
{
	"FULL: OFF ",
	"FULL: ON  ",
};


// 振動の表示形式
char g_aVibrationText[2][TXT_MAX] =
{
	"VIB : OFF ",
	"VIB : ON  ",
};

//========== *** 設定の情報を取得 ***
Setting *GetSetting(void) 
{
	return &g_setting;
}

//========== *** 設定中フラグを取得 ***
bool *GetSettingNow(void) 
{
	return &g_bSettingNow;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetSettingMenuText関数 - 設定メニューのテキストを設定 -
//========================================
void SetSettingMenuText(void)
{
	Ui_menu_00	*pUi	// メニュー[00] の情報のポインタ 
				= GetUi_menu_00();

	/*/ BGM音量    /*/sprintf(pUi[SETTING_MENU_BGM_VOLUME].aString, "BGM : %-3d ", g_setting.nBGMVolume * VOLUME_DISPLAY_DIAMETER);
	/*/ SE音量     /*/sprintf(pUi[SETTING_MENU_SE_VOLUME].aString, "SE  : %-3d ", g_setting.nSEVolume * VOLUME_DISPLAY_DIAMETER);
	/*/ 画面モード /*/sprintf(pUi[SETTING_MENU_SCREEN_MODE].aString, g_aScreenModeText[g_setting.bFullScreen]);
	/*/ 振動       /*/sprintf(pUi[SETTING_MENU_VIBRATION].aString, g_aVibrationText[g_setting.bVibration]);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitSetting関数 - 設定の初期化処理 -
//========================================
void InitSetting(void)
{
	g_bSettingNow = false;	// 設定中フラグを初期化
}

//========================================
// UninitSetting関数 - 設定の終了処理 -
//========================================
void UninitSetting(void)
{

}

//========================================
// UpdateSetting関数 - 設定の更新処理 -
//========================================
void UpdateSetting(void)
{
	if (g_nCounterScreenModeChange >= 0)
	{// 画面モード切り替えカウンターが0以上の時、
		// 画面モード切り替えカウンターを減算
		g_nCounterScreenModeChange--;

		if (g_nCounterScreenModeChange == 0)
		{// 画面モード切り替えカウンターが0の時、
			// ウインドウのモードを切り替える
			WindowModeChange(g_setting.bFullScreen);
		}
	}

	if (GetKeyboardTrigger(DIK_F11))
	{// F11キーが入力された時、
		// 全画面フラグを切り替える
		g_setting.bFullScreen ^= 1;

		// 画面モード切り替えカウンターを設定
		g_nCounterScreenModeChange = WINDOW_MODE_CHANGE_TIME;
	}
}

//========================================
// UpdateSettingMenu関数 - 設定メニューの更新処理 -
//========================================
void UpdateSettingMenu(void)
{
	switch (Ui_menu_00Input(UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
	{
	case /*/ BGM音量 /*/SETTING_MENU_BGM_VOLUME: {
		if ((GetKeyboardRepeat(DIK_LEFT)) || (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
		{// 左に入力された時、
			// BGM音量を減算
			g_setting.nBGMVolume--;
		}
		else if ((GetKeyboardRepeat(DIK_RIGHT)) || (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// 右に入力された時、
			// BGM音量を加算
			g_setting.nBGMVolume++;
		}

		// BGM音量を制御
		IntControl(&g_setting.nBGMVolume, VOLUME_STAGE, 0);

		// 設定音量を設定
		SetSettingVolume(SOUND_TYPE_BGM);

		break;
	}
	case /*/ SE音量 /*/SETTING_MENU_SE_VOLUME: {
		if ((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
		{// 左に入力された時、
			// SE音量を減算
			g_setting.nSEVolume--;
		}
		else if ((GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// 右に入力された時、
			// SE音量を加算
			g_setting.nSEVolume++;
		}

		// SE音量を制御
		IntControl(&g_setting.nSEVolume, VOLUME_STAGE, 0);

		// 設定音量を設定
		SetSettingVolume(SOUND_TYPE_SE);
	}
	break;
	case /*/ 画面モード /*/SETTING_MENU_SCREEN_MODE: {
		if ((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// 左か右に入力された時、
			// 全画面フラグを切り替える
			g_setting.bFullScreen ^= 1;

			// 画面モード切り替えカウンターを設定
			g_nCounterScreenModeChange = WINDOW_MODE_CHANGE_TIME;
		}

		break;
	}
	case /*/ 振動 /*/SETTING_MENU_VIBRATION: {
		if ((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// 左か右に入力された時、
			 // 振動フラグを切り替える
			g_setting.bVibration ^= 1;
		}

		break;
	}
	case /*/ 了解 /*/SETTING_MENU_OK: {
		// 設定中フラグを偽にする
		g_bSettingNow = false;
		break;
	}
	}

	// 設定メニューのテキストを設定
	SetSettingMenuText();
}

//========================================
// SetSettingMenu関数 - 設定メニューの設定処理 -
//========================================
void SetSettingMenu(void) 
{
	// メニュー[00] の設定処理
	SetUi_menu_00(
		g_aSettingMenuSet,
		SETTING_MENU_MAX);

	// 選択番号をBGM音量に設定
	*GetSelect() = SETTING_MENU_BGM_VOLUME;

	// 設定中フラグを真にする
	g_bSettingNow = true;
}

//========================================
// LoadSetting関数 - 設定の読み込み処理 -
//========================================
void LoadSetting(void)
{
	FILE *pFile;	//ファイルのポインタ

	//ファイルを開く
	pFile = fopen(SETTING_DATA_FILE_PATH, "rb");

	if (pFile != NULL)
	{//ファイルが開けた時、
		//ファイルから設定を読み込む
		fread(&g_setting, sizeof(Setting), 1, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
}

//========================================
// SaveSetting関数 - 設定の書き込み処理 -
//========================================
void SaveSetting(void)
{
	FILE *pFile;	//ファイルのポインタ

	//ファイルを開く
	pFile = fopen(SETTING_DATA_FILE_PATH, "wb");

	if (pFile != NULL)
	{//ファイルが開けた時、
		//ファイルに設定を書き出し
		fwrite(&g_setting, sizeof(Setting), 1, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
}