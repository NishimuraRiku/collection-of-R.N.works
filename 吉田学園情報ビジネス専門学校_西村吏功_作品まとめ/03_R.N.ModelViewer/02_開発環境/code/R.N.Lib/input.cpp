//========================================
// 
// 入力関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** input.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// マクロ定義
//****************************************
//========== *** 調整パラメーター ***
// リピートの間隔
#define REPEATE_INTERVAL (160)
// 振動の減算量
#define VIBRATION_SUBTRACT (0.02f)

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// スティックの入力情報のパラメーター初期値を返す
StickInput InitParameterStickInput(void);
// 入力情報のパラメーター初期値を返す
InputInfo InitParameterInputInfo(void);
//========== *** 更新 ***
// キーボードの更新処理
void UpdateKeyboard(void);
// マウスの更新処理
void UpdateMouse(void);
// カーソルの更新処理
void UpdateCursor(void);
// ボタンの更新処理
void UpdateButton(void);
// スティックの更新処理
void UpdateStick(void);
// コントローラーの振動の更新処理
void UpdateVibration(void);

//****************************************
// グローバル変数宣言
//****************************************
LPDIRECTINPUT8 g_pInput = NULL;				// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	// 入力デバイス(キーボード)へのポインタ
InputInfo g_inputInfo;						// 入力情報
ACTIVE_DEVICE_TYPE g_activeInputType;		// 動的なデバイス

//****************************************
// グローバル定数宣言
//****************************************
// マウスのマスク
const int g_aMouseMask[MOUSEBUTTON_MAX] = {
	VK_LBUTTON,
	VK_RBUTTON,
};
// ボタンのマスク
const int g_aButtonMask[BUTTON_MAX] = {
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	-1,
	-1,
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// InitParameterStickInput関数 - スティックの入力情報のパラメーター初期値を返す -
// Author:RIKU NISHIMURA
//========================================
StickInput InitParameterStickInput(void)
{
	return{
		{ 0.0f },
		{ 0.0f },
		{ { false } },
		{ { false } },
		{ { false } },
		{ { false } },
	};
}

//========================================
// InitParameterInputInfo関数 - 入力情報のパラメーター初期値を返す -
// Author:RIKU NISHIMURA
//========================================
InputInfo InitParameterInputInfo(void) 
{
	return{
		{ 0 },						// キーボードのプレス情報
		{ 0 },						// キーボードのトリガー情報
		{ 0 },						// キーボードのリリース情報
		{ 0 },						// キーボードのリピート情報
		{ 0 },						// キーボードの現在の時間
		{ 0 },						// キーボードの最後に真を返した時間
		{ 0 },						// マウスのプレス情報
		{ 0 },						// マウスのトリガー情報
		{ 0 },						// マウスのリリース情報
		{ 0 },						// マウスのリピート情報
		{ 0 },						// マウスの現在の時間
		{ 0 },						// マウスの最後に真を返した時間
		GetCursorPosOnScreen(),		// カーソルの位置を保存
		INITD3DXVECTOR3,			// カーソルの移動量
		WHEELSPIN_NONE,				// マウスホイールの回転状態
		{ false },					// ボタンのプレス情報
		{ false },					// ボタンのトリガー情報
		{ false },					// ボタンのリリース情報
		{ false },					// ボタンのリピート情報
		{ 0 },						// ボタンの現在の時間
		{ 0 },						// ボタンの最後に真を返した時間
		InitParameterStickInput(),	// スティックの入力情報
		{ 0 },						// スティックの現在の時間
		{ 0 },						// スティックの最後に真を返した時間
		0,							// コントローラーの振動カウンター
		0,							// コントローラーの振動カウンターの最大値を保存
		0.0f,						// 振動倍率
		{},							// XInputの状態
	};
}

//============================================================
//--------------------| *** 更新 *** |----------------------
//============================================================
//========================================
// UpdateKeyboard関数 - キーボードの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateKeyboard(void)
{
	// キーボードの入力情報
	BYTE aKeyState[NUM_KEY_MAX];

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// キーボードのトリガー情報を保存
			g_inputInfo.aKeyStateTrigger[nCntKey] = (g_inputInfo.aKeyStatePress[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

			// キーボードのリリース情報を保存
			g_inputInfo.aKeyStateRelease[nCntKey] = (g_inputInfo.aKeyStatePress[nCntKey] ^ aKeyState[nCntKey])&~aKeyState[nCntKey];

			// 現在の時間を取得
			g_inputInfo.aKeyCurrentTime[nCntKey] = timeGetTime();

			if (aKeyState[nCntKey] && ((g_inputInfo.aKeyCurrentTime[nCntKey] - g_inputInfo.aKeyExecLastTime[nCntKey]) > REPEATE_INTERVAL))
			{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
				// 最後に真を返した時間を保存
				g_inputInfo.aKeyExecLastTime[nCntKey] = g_inputInfo.aKeyCurrentTime[nCntKey];

				// キーボードのリピート情報を保存
				g_inputInfo.aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}
			else
			{
				// キーボードのリピート情報を保存
				g_inputInfo.aKeyStateRepeat[nCntKey] = 0;
			}

			// キーボードのプレス情報を保存
			g_inputInfo.aKeyStatePress[nCntKey] = aKeyState[nCntKey];

			if (aKeyState[nCntKey])
			{// 入力が行われた時、
				// 動的なデバイスをキーボードにする
				g_activeInputType = ACTIVE_DEVICE_KEYBOARD;
			}
		}
	}
	else
	{// 入力デバイスからデータを取得できなかった時、
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//========================================
// UpdateMouse関数 - マウスの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMouse(void)
{
	for (int nCntMouse = 0; nCntMouse < MOUSEBUTTON_MAX; nCntMouse++)
	{
		// マウスの入力情報
		BYTE mouseState = GetKeyState(g_aMouseMask[nCntMouse]) & (0x80) ? true : false;;

		// マウスのトリガー情報を保存
		g_inputInfo.aMouseStateTrigger[nCntMouse] = (g_inputInfo.aMouseState[nCntMouse] ^ mouseState)&mouseState;

		// マウスのリリース情報を保存
		g_inputInfo.aMouseStateRelease[nCntMouse] = (g_inputInfo.aMouseState[nCntMouse] ^ mouseState)&~mouseState;

		// 現在の時間を取得
		g_inputInfo.aMouseCurrentTime[nCntMouse] = timeGetTime();

		if (mouseState && ((g_inputInfo.aMouseCurrentTime[nCntMouse] - g_inputInfo.aMouseExecLastTime[nCntMouse]) > REPEATE_INTERVAL))
		{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
			// 最後に真を返した時間を保存
			g_inputInfo.aMouseExecLastTime[nCntMouse] = g_inputInfo.aMouseCurrentTime[nCntMouse];

			// マウスのリピート情報を保存
			g_inputInfo.aMouseStateRepeat[nCntMouse] = mouseState;
		}
		else
		{// キーが入力されていない、もしくは現在の時間と最後に真を返した時間の差がリピートの間隔を越えていない時、
			// マウスのリピート情報を保存
			g_inputInfo.aMouseStateRepeat[nCntMouse] = 0;
		}

		// マウスのプレス情報を保存
		g_inputInfo.aMouseState[nCntMouse] = mouseState;

		if (mouseState)
		{// 入力が行われた時、
			// 動的なデバイスをキーボードにする
			g_activeInputType = ACTIVE_DEVICE_KEYBOARD;
		}
	}
}

//========================================
// UpdateCursor関数 - カーソルの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateCursor(void)
{
	if (g_inputInfo.cursorPosTemp != GetCursorPosOnScreen()) 
	{// カーソルの現在位置が保存位置を異なる時、
		// 動的なデバイスをキーボードにする
		g_activeInputType = ACTIVE_DEVICE_KEYBOARD;
	}

	// カーソルの移動量を設定
	g_inputInfo.cursorMove = GetCursorPosOnScreen() - g_inputInfo.cursorPosTemp;

	// カーソル位置を保存
	g_inputInfo.cursorPosTemp = GetCursorPosOnScreen();
}

//========================================
// UpdateButton関数 - ボタンの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateButton(void)
{
	// ボタンの入力情報
	bool aButton[BUTTON_MAX] = { false };

	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		if (nCntButton == BUTTON_LEFT_TRIGGER)
		{// カウントが左トリガーの時、
			if (GetXInputState()->Gamepad.bLeftTrigger > 0)
			{// 押し込まれている
				aButton[nCntButton] = true;
			}
		}
		else if (nCntButton == BUTTON_RIGHT_TRIGGER)
		{// カウントが右トリガーの時、
			if (GetXInputState()->Gamepad.bRightTrigger > 0)
			{// 押し込まれている
				aButton[nCntButton] = true;
			}
		}
		else if (GetXInputState()->Gamepad.wButtons & g_aButtonMask[nCntButton])
		{// 入力情報を更新
			aButton[nCntButton] = true;
		}

		// ボタンのトリガー情報を保存
		g_inputInfo.aButtonTrigger[nCntButton] = (g_inputInfo.aButtonPress[nCntButton] ^ aButton[nCntButton])&aButton[nCntButton];

		// ボタンのリリース情報を保存
		g_inputInfo.aButtonRelease[nCntButton] = (g_inputInfo.aButtonPress[nCntButton] ^ aButton[nCntButton])&~aButton[nCntButton];

		// 現在の時間を取得
		g_inputInfo.aButtonCurrentTime[nCntButton] = timeGetTime();

		if (aButton[nCntButton] && ((g_inputInfo.aButtonCurrentTime[nCntButton] - g_inputInfo.aButtonExecLastTime[nCntButton]) > REPEATE_INTERVAL))
		{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
			// 最後に真を返した時間を保存
			g_inputInfo.aButtonExecLastTime[nCntButton] = g_inputInfo.aButtonCurrentTime[nCntButton];

			// ボタンのリピート情報を保存
			g_inputInfo.aButtonRepeat[nCntButton] = aButton[nCntButton];
		}
		else
		{
			// ボタンのリピート情報を保存
			g_inputInfo.aButtonRepeat[nCntButton] = 0;
		}

		// ボタンのプレス情報を保存
		g_inputInfo.aButtonPress[nCntButton] = aButton[nCntButton];

		if (aButton[nCntButton])
		{// 入力が行われた時、
			// 動的なデバイスをコントローラーにする
			g_activeInputType = ACTIVE_DEVICE_CONTROLLER;
		}
	}
}

//========================================
// UpdateStick関数 - スティックの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateStick(void)
{
	// スティックの入力情報
	bool bAngle[STICK_TYPE_MAX][STICK_ANGLE_MAX];

	// XInputの状態を取得
	g_inputInfo.xInputState.dwPacketNumber = XInputGetState(g_inputInfo.xInputState.dwPacketNumber, &g_inputInfo.xInputState);

	for (int nCntStick = 0; nCntStick < STICK_TYPE_MAX; nCntStick++)
	{
		float X;	// スティックのX軸
		float Y;	// スティックのY軸

		// 種類に応じたスティックの軸の値を取得
		switch (nCntStick)
		{
		case STICK_TYPE_LEFT:
			X = GetXInputState()->Gamepad.sThumbLX;
			Y = GetXInputState()->Gamepad.sThumbLY;
			break;
		case STICK_TYPE_RIGHT:
			X = GetXInputState()->Gamepad.sThumbRX;
			Y = GetXInputState()->Gamepad.sThumbRY;
			break;
		}

		// 角度を取得
		g_inputInfo.stick.aAngle[nCntStick] = FindAngle(D3DXVECTOR3(X, Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * -1;

		// スティックの倒し具合を取得
		g_inputInfo.stick.aTplDiameter[nCntStick] = fabsf(X);
		if (g_inputInfo.stick.aTplDiameter[nCntStick] < fabsf(Y))
		{
			g_inputInfo.stick.aTplDiameter[nCntStick] = fabsf(Y);
		}
		g_inputInfo.stick.aTplDiameter[nCntStick] /= 32768.0f;

		// 方向入力フラグを初期化
		for (int nCntAngle = 0; nCntAngle < STICK_ANGLE_MAX; nCntAngle++)
		{
			bAngle[nCntStick][nCntAngle] = false;
		}

		if (g_inputInfo.stick.aTplDiameter[nCntStick] > 0)
		{// スティックが倒されている時、
			// 動的なデバイスをコントローラーにする
			g_activeInputType = ACTIVE_DEVICE_CONTROLLER;

			if (false) {}
			else if ((g_inputInfo.stick.aAngle[nCntStick] < D3DX_PI * -0.75)
				|| (g_inputInfo.stick.aAngle[nCntStick] > D3DX_PI * 0.75))
			{// 角度が四分割で上に位置する時、上フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_UP] = true;
			}
			else if ((g_inputInfo.stick.aAngle[nCntStick] > D3DX_PI * -0.25)
				&& (g_inputInfo.stick.aAngle[nCntStick] < D3DX_PI * 0.25))
			{// 角度が四分割で下に位置する時、下フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_DOWN] = true;
			}
			else if ((g_inputInfo.stick.aAngle[nCntStick] > D3DX_PI * -0.75)
				&& (g_inputInfo.stick.aAngle[nCntStick] < D3DX_PI * -0.25))
			{// 角度が四分割で左に位置する時、左フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_LEFT] = true;
			}
			else if ((g_inputInfo.stick.aAngle[nCntStick] > D3DX_PI * 0.25)
				&& (g_inputInfo.stick.aAngle[nCntStick] < D3DX_PI * 0.75))
			{// 角度が四分割で右に位置する時、右フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_RIGHT] = true;
			}
		}

		for (int nCntAngle = 0; nCntAngle < STICK_ANGLE_MAX; nCntAngle++)
		{
			// スティックのトリガー情報を保存
			g_inputInfo.stick.aAngleTrigger[nCntStick][nCntAngle] = (g_inputInfo.stick.aAnglePress[nCntStick][nCntAngle] ^ bAngle[nCntStick][nCntAngle])&bAngle[nCntStick][nCntAngle];

			// スティックのリリース情報を保存
			g_inputInfo.stick.aAngleRelease[nCntStick][nCntAngle] = (g_inputInfo.stick.aAnglePress[nCntStick][nCntAngle] ^ bAngle[nCntStick][nCntAngle])&~bAngle[nCntStick][nCntAngle];

			// 現在の時間を取得
			g_inputInfo.aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

			if (bAngle[nCntStick][nCntAngle] && ((g_inputInfo.aStickCurrentTime[nCntStick][nCntAngle] - g_inputInfo.aStickExecLastTime[nCntStick][nCntAngle]) > REPEATE_INTERVAL))
			{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
				// 最後に真を返した時間を保存
				g_inputInfo.aStickExecLastTime[nCntStick][nCntAngle] = g_inputInfo.aStickCurrentTime[nCntStick][nCntAngle];

				// スティックのリピート情報を保存
				g_inputInfo.stick.aAngleRepeat[nCntStick][nCntAngle] = bAngle[nCntStick][nCntAngle];
			}
			else {
				// スティックのリピート情報を保存
				g_inputInfo.stick.aAngleRepeat[nCntStick][nCntAngle] = 0;
			}

			// スティックのプレス情報を保存
			g_inputInfo.stick.aAnglePress[nCntStick][nCntAngle] = bAngle[nCntStick][nCntAngle];

			if (bAngle[nCntStick][nCntAngle])
			{// 入力が行われた時、
				// 動的なデバイスをコントローラーにする
				g_activeInputType = ACTIVE_DEVICE_CONTROLLER;
			}
		}
	}
}

//========================================
// UpdateVibration関数 - コントローラーの振動更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateVibration(void) 
{
	if (g_inputInfo.fVibration > 0)
	{// 振動倍率が0を上回っている時、
		// 振動倍率を減算
		g_inputInfo.fVibration -= VIBRATION_SUBTRACT;

		// 振動倍率を制御
		FloatControl(&g_inputInfo.fVibration, 1.0f, 0);

		{// 振動フラグが真の時、
			XINPUT_STATE state;			//XINPUT_STATE型 stateを作成
			XInputGetState(0, &state);	//stateに習得したコントローラの状態を格納
			XINPUT_VIBRATION vibration;	//XINPUT_VIBRATION vibrationを作成

			//vibrationメンバーwLeftMotorSpeedに値を代入 モータースピード100%
			vibration.wLeftMotorSpeed = 65535 * g_inputInfo.fVibration;

			//vibrationメンバーwRightMotorSpeedに値を代入 モータースピード0%
			vibration.wRightMotorSpeed = 65535 * g_inputInfo.fVibration;

			//ジョイスティックにバイブレーターデータを送信
			XInputSetState(0, &vibration);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// InitInput関数 - 入力関連の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// マウスカーソルを表示
	ShowCursor(true);

	// 入力情報をクリア
	ClearInputInfo();

	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) 
	{
		return E_FAIL;
	}
	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// 協調フォーマット
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//========================================
// UninitInput関数 - 入力関連の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitInput(void)
{
	// XInputを閉じる
	XInputEnable(false);

	// 入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL) 
	{
		g_pDevKeyboard->Unacquire();	// キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL) 
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//========================================
// UpdateInput関数 - 入力関連の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateInput(void)
{
	UpdateKeyboard();	// キーボード
	UpdateButton();		// ボタン
	UpdateCursor();		// カーソル
	UpdateMouse();		// マウス
	UpdateStick();		// スティック
	UpdateVibration();	// コントローラーの振動

	// マウスホイールの回転状態を無しに設定
	g_inputInfo.wheelSpin = WHEELSPIN_NONE;
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//=======================================
// GetInputInfo関数 - 入力情報を取得 -
//=======================================
InputInfo *GetInputInfo(void) 
{
	return &g_inputInfo;
}

//========================================
// GetXInputState関数 - XInputの状態を取得 -
// Author:RIKU NISHIMURA
//=======================================
XINPUT_STATE *GetXInputState(void)
{
	return &g_inputInfo.xInputState;
}

//========================================
// GetCursorPosOnScreen関数 - 画面上のカーソルの位置を取得 -
// Author:RIKU NISHIMURA
//=======================================
D3DXVECTOR3 GetCursorPosOnScreen(void)
{
	// カーソルの現在位置を取得
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	// クライアント領域のサイズを取得
	RECT rc;
	GetClientRect(*GetWindowHandle(), &rc);

	return D3DXVECTOR3(
		(cursorPos.x - GetWindowPos().x) * (SCREEN_WIDTH / (float)rc.right),
		(cursorPos.y - GetWindowPos().y) * (SCREEN_HEIGHT / (float)rc.bottom),
		0.0f);
}

//========================================
// GetCursorMove関数 - カーソルの移動量を取得 -
// Author:RIKU NISHIMURA
//=======================================
D3DXVECTOR3 GetCursorMove(void)
{
	return g_inputInfo.cursorMove;
}

//========================================
// GetStick関数 - スティックの入力情報を取得 -
// Author:RIKU NISHIMURA
//=======================================
StickInput GetStick(void)
{
	return g_inputInfo.stick;
}

//========================================
// GetActiveInputType関数 - 動的なデバイスを取得 -
// Author:RIKU NISHIMURA
//========================================
ACTIVE_DEVICE_TYPE GetActiveInputType()
{
	return g_activeInputType;
}

//============================================================
//--------------------| *** キーボード *** |------------------
//============================================================
//========================================
// GetKeyboardPress関数 - キーボードのプレス情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardPress(int nKey)
{
	return g_inputInfo.aKeyStatePress[nKey] != 0;
}

//========================================
// GetKeyboardTrigger関数 - キーボードのトリガー情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardTrigger(int nKey)
{
	return g_inputInfo.aKeyStateTrigger[nKey] != 0;
}

//========================================
// GetKeyboardRepeat関数 - キーボードのリピート情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardRepeat(int nKey)
{
	return g_inputInfo.aKeyStateRepeat[nKey] != 0;
}

//========================================
// GetKeyboardRelease関数 - キーボードのリリース情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardRelease(int nKey)
{
	return g_inputInfo.aKeyStateRelease[nKey] != 0;
}

//============================================================
//--------------------| *** マウス *** |----------------------
//============================================================
//========================================
// GetMouse関数 - マウスのプレス情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMousePress(int nMouse)
{
	return g_inputInfo.aMouseState[nMouse];
}

//========================================
// GetMouseTrigger関数 - マウスのトリガー情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseTrigger(int nMouse)
{
	return g_inputInfo.aMouseStateTrigger[nMouse];
}

//========================================
// GetMouseRepeat関数 - マウスのリピート情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseRepeat(int nMouse)
{
	return g_inputInfo.aMouseStateRepeat[nMouse];
}

//========================================
// GetMouseRelease関数 - マウスのリリース情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseRelease(int nMouse)
{
	return g_inputInfo.aMouseStateRelease[nMouse];
}

//========================================
// GetWheelSpin関数 - マウスホイールの回転状態を取得 -
// Author:RIKU NISHIMURA
//========================================
WHEELSPIN GetWheelSpin(void) 
{
	return g_inputInfo.wheelSpin;
}

//============================================================
//--------------------| *** ボタン *** |----------------------
//============================================================
//========================================
// GetButtonPress関数 - ボタンのプレス情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonPress(int nButton)
{
	return g_inputInfo.aButtonPress[nButton];
}

//========================================
// GetButtonTrigger関数 - ボタンのトリガー情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonTrigger(int nButton)
{
	return g_inputInfo.aButtonTrigger[nButton];
}

//========================================
// GetButtonRepeat関数 - ボタンのリピート情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonRepeat(int nButton)
{
	return g_inputInfo.aButtonRepeat[nButton];
}

//========================================
// GetButtonRelease関数 - ボタンのリリース情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonRelease(int nButton)
{
	return g_inputInfo.aButtonRelease[nButton];
}

//============================================================
//--------------------| *** コントローラー *** |--------------
//============================================================
//========================================
// SetVibration関数 - コントローラーの振動設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetVibration(float fVibration, int nTime)
{
	if (g_activeInputType != ACTIVE_DEVICE_CONTROLLER) 
	{// 動的なデバイスがコントローラーで無い時、
		// 処理を終了
		return;
	}

	if (fVibration > g_inputInfo.fVibration)
	{// 振動倍率が元の振動倍率を上回っている時、
		// 振動倍率を代入
		g_inputInfo.fVibration = fVibration;
	}

	// 振動倍率を制御
	FloatControl(&g_inputInfo.fVibration, 1.0f, 0);
}

//========================================
// ClearInputInfo関数 - 入力情報のクリア処理 -
// Author:RIKU NISHIMURA
//========================================
void ClearInputInfo(void)
{
	// 入力情報を初期化
	g_inputInfo = InitParameterInputInfo();
}