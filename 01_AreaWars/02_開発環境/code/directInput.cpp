//========================================
// 
// DirectInputの処理
// Author:西村 吏功
// 
//========================================
// *** directInput.cpp ***
//========================================
#include "main.h"
#include "directInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// プロトタイプ宣言
//****************************************
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);	// デバイス発見時に実行される

//****************************************
// グローバル宣言
//****************************************
		LPVOID					*g_pInputInterface;	// IDirectInput8インターフェースポインタ受け取り用
static	LPDIRECTINPUT8			g_InputInterface;	// DIRECTINPUT8のポインタ
		LPDIRECTINPUTDEVICE8	g_gamePadDevice;	// ゲームパッドのデバイス

//========================================
// InitDirectInput関数 - DirectInputの初期化処理 -
//========================================
bool InitDirectInput(HINSTANCE hInstance, HWND hWnd)
{
	// IDirectInput8インターフェイスの作成
	HRESULT hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputInterface,
		NULL);

	if (FAILED(hr))
	{
		// 失敗
	}

	LPVOID	*parameter;	// データのポインタ

	// デバイスの列挙
	if (FAILED(g_InputInterface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	)))
	{
		return false;
	}

	// IDirectInputDevice8の作成
	LPCDIDEVICEINSTANCE	lpddi;

	hr = g_InputInterface->CreateDevice(
		//lpddi->guidInstance,
		GUID_Joystick,
		&g_gamePadDevice,
		NULL
	);

	if (FAILED(hr))
	{
		// 失敗
	}

	// デバイスのフォーマットの設定
	hr = g_gamePadDevice->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

	// 軸モードを変更
	if (FAILED(g_gamePadDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(g_gamePadDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(g_gamePadDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// 協調モードの設定
	if (FAILED(g_gamePadDevice->SetCooperativeLevel(
		FindWindow(CLASS_NAME, nullptr),
		DISCL_EXCLUSIVE | DISCL_FOREGROUND)
	))
	{
		// 失敗
	}

	// 制御開始
	hr = g_gamePadDevice->Acquire();

	if (FAILED(hr))
	{
		// 制御開始失敗
	}

	// ポーリング開始
	hr = g_gamePadDevice->Poll();
	if (FAILED(hr))
	{
		// 失敗
	}
}

//========================================
// DeviceFindCallBack関数 - デバイス発見時に実行される -
//========================================
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

//========================================
// UninitDirectInput関数 - DirectInputの終了処理 -
//========================================
void UninitDirectInput(void) 
{
	// デバイス制御の停止
	g_gamePadDevice->Unacquire();

	// IDirectInputDevice8の解放
	g_gamePadDevice->Release();

	// IDirectInput8の解放
	g_gamePadDevice->Release();
}