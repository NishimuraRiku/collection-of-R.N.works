//========================================
// 
// DirectInput�̏���
// Author:���� ����
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
// �v���g�^�C�v�錾
//****************************************
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);	// �f�o�C�X�������Ɏ��s�����

//****************************************
// �O���[�o���錾
//****************************************
		LPVOID					*g_pInputInterface;	// IDirectInput8�C���^�[�t�F�[�X�|�C���^�󂯎��p
static	LPDIRECTINPUT8			g_InputInterface;	// DIRECTINPUT8�̃|�C���^
		LPDIRECTINPUTDEVICE8	g_gamePadDevice;	// �Q�[���p�b�h�̃f�o�C�X

//========================================
// InitDirectInput�֐� - DirectInput�̏��������� -
//========================================
bool InitDirectInput(HINSTANCE hInstance, HWND hWnd)
{
	// IDirectInput8�C���^�[�t�F�C�X�̍쐬
	HRESULT hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputInterface,
		NULL);

	if (FAILED(hr))
	{
		// ���s
	}

	LPVOID	*parameter;	// �f�[�^�̃|�C���^

	// �f�o�C�X�̗�
	if (FAILED(g_InputInterface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	)))
	{
		return false;
	}

	// IDirectInputDevice8�̍쐬
	LPCDIDEVICEINSTANCE	lpddi;

	hr = g_InputInterface->CreateDevice(
		//lpddi->guidInstance,
		GUID_Joystick,
		&g_gamePadDevice,
		NULL
	);

	if (FAILED(hr))
	{
		// ���s
	}

	// �f�o�C�X�̃t�H�[�}�b�g�̐ݒ�
	hr = g_gamePadDevice->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// �����[�h���Βl���[�h�Ƃ��Đݒ�
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// ��Βl���[�h�̎w��(DIPROPAXISMODE_REL�ɂ����瑊�Βl)

	// �����[�h��ύX
	if (FAILED(g_gamePadDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X���̒l�͈̔͐ݒ�
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

	// Y���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(g_gamePadDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// �������[�h�̐ݒ�
	if (FAILED(g_gamePadDevice->SetCooperativeLevel(
		FindWindow(CLASS_NAME, nullptr),
		DISCL_EXCLUSIVE | DISCL_FOREGROUND)
	))
	{
		// ���s
	}

	// ����J�n
	hr = g_gamePadDevice->Acquire();

	if (FAILED(hr))
	{
		// ����J�n���s
	}

	// �|�[�����O�J�n
	hr = g_gamePadDevice->Poll();
	if (FAILED(hr))
	{
		// ���s
	}
}

//========================================
// DeviceFindCallBack�֐� - �f�o�C�X�������Ɏ��s����� -
//========================================
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

//========================================
// UninitDirectInput�֐� - DirectInput�̏I������ -
//========================================
void UninitDirectInput(void) 
{
	// �f�o�C�X����̒�~
	g_gamePadDevice->Unacquire();

	// IDirectInputDevice8�̉��
	g_gamePadDevice->Release();

	// IDirectInput8�̉��
	g_gamePadDevice->Release();
}