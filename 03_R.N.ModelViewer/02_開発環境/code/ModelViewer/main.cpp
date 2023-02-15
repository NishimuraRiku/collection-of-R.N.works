//========================================
// 
// ���C������
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
// Character
#include "character.h"
// Object
#include "object.h"
// _R.N.Lib
#include "../R.N.Lib\RNmain.h"

//****************************************
// �}�N����`
//****************************************
// �E�C���h�E�̖��O
#define WINDOW_NAME	"R.N.ModelViewer"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
// �I������
void Uninit(void);
// �X�V����
void Update(void);
// �`�揈��
void Draw(void);
// �ǂݍ��ݏ���
void Load(void);
// �������ݏ���
void Save(void);
// �E�C���h�E�̕\��/��\����؂�ւ���
void ShowTaskBar(bool bShow);

//****************************************
// �O���[�o���ϐ��錾
//****************************************
Main g_main;	// ���C�������̏��\����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// WinMain�֐�
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	MSG msg;				// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS���v����������

	// �����̎���擾
	srand((unsigned int)time(0));

	// �E�C���h�E�N���X�̍\����
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						// �E�C���h�E�̃X�^�C��
		WindowProc,						// �E�C���h�E�v���V�[�W��
		0,								// 0�ɂ���
		0,								// 0�ɂ���
		hInstance,						// �C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		// �N���C�A���g�̈�̔w�i�F
		NULL,							// ���j���[�o�[
		CLASS_NAME,						// �E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	// �t�@�C���̃A�C�R��
	};

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�C���h�E�𐶐�
	g_main.hWnd = CreateWindowEx(
		0,								// �g���E�C���h�E�X�^�C��
		CLASS_NAME,						// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,					// �E�C���h�E�̖��O
		WS_SYSMENU | WS_MINIMIZEBOX,	// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,					// �E�C���h�E�̍���X���W
		CW_USEDEFAULT,					// �E�C���h�E�̍���Y���W
		SCREEN_WIDTH,					// �E�C���h�E�̕�
		SCREEN_HEIGHT,					// �E�C���h�E�̍���
		NULL,							// �e�E�C���h�E�̃n���h��
		NULL,							// ���j���[�n���h���܂���
		hInstance,						// �C���X�^���X�n���h��
		NULL);							// �E�C���h�E�쐬�f�[�^

	// ����������
	if (FAILED(Init(hInstance, g_main.hWnd, TRUE)))
	{// ���������������s�����ꍇ

		return -1;
	}

	// �E�C���h�E�̕\��
	WindowModeChange(false);

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0; // ���ݎ���������������
	dwExecLastTime = timeGetTime(); // ���ݎ������擾(�ۑ�)

	dwFrameCount = 0;	// �t���[���J�E���g������������
	dwFPSLastTime = timeGetTime();	// ���ݎ������擾(�ۑ�)

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯎�������A���b�Z�[�W���[�v�𔲂���
				break;
			}
			else {
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime(); // ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				// FPS���v��
				g_main.nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime; // FPS�𑪒肵��������ۑ�
				dwFrameCount = 0; // �t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60/1�b�o��
				dwExecLastTime = dwCurrentTime; // �����J�n�̎���[���ݎ���]��ۑ�

				{
					RECT rec;	// �傫�����󂯎��RECT�\����

					// �E�C���h�E�̏����擾
					GetWindowRect(g_main.hWnd, &rec);

					// �E�C���h�E�̈ʒu��ݒ�
					g_main.windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);
				}

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++; // �t���[���J�E���g�����Z
			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
// ShowTaskBar�֐� - �^�X�N�o�[�̕\��/��\����؂�ւ��� -
// �Q�l: https://dixq.net/forum/viewtopic.php?t=12505
//========================================
void ShowTaskBar(bool bShow)
{
	HWND hTask;	// �^�X�N�o�[�̃n���h��

	// �^�X�N�o�[�̃n���h�����擾
	hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow)
	{// �\��
		// �^�X�N�o�[��\��
		ShowWindow(hTask, SW_SHOW);

		// �E�C���h�E���A�N�e�B�u�ɂ���
		SetFocus(g_main.hWnd);
	}
	else
	{// ��\��
		// �^�X�N�o�[���\��
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// WindowProc�֐�
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	// �Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY: // �E�C���h�E�j���̃��b�Z�[�W

		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�𑗂�

		break;
	case WM_KEYDOWN: // �L�[�����̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]�L�[�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO); // �E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)

			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // �E�C���h�E��j������
			}

			break;
		}

		break;
	case WM_CLOSE: // �I���{�^���������ꂽ��

		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO); // �E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)

		if (nID == IDYES)
		{
			DestroyWindow(hWnd); // �E�C���h�E��j������
		}
		else
		{
			return 0; // (��0��Ԃ��Ȃ��ƏI�����Ă��܂�)
		}

		break;
	case WM_LBUTTONDOWN: // �}�E�X�N���b�N�̃��b�Z�[�W

		// �ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd); // �}�E�X�����N���b�N�����E�C���h�E���A�N�e�B�u�ɂ���
		break;
	case WM_MOUSEWHEEL: // �}�E�X�z�C�[�����񂳂ꂽ���̃��b�Z�[�W
		if (HIWORD(wParam) == WHEEL_DELTA)
		{
			GetCamera3D()->fLength -= CAMERA3D_ZOOM_LENGTH;
		}
		else
		{
			GetCamera3D()->fLength += CAMERA3D_ZOOM_LENGTH;
		}

		break;
	case WM_CREATE: //�E�C���h�E�����̃��b�Z�[�W

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // ����̏�����Ԃ�
}

//========================================
// Init�֐� - ���������� -
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �w�i�F��������
	g_main.bgCol = INITBGCOLOR;

	// Direct3D�I�u�W�F�N�g�̐���
	g_main.pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_main.pD3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;	// �f�B�X�v���C���[�h

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_main.pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[���̉�ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[���̉�ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_main.pD3DDevice)))
	{
		if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_main.pD3DDevice)))
		{
			if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_main.pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_main.pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�ݒ�
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �A���t�@�u�����h�̐ݒ�
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �ǂݍ��ݏ���
	Load();

	// R.N.Lib�̏���������
	RNInit(hInstance);

	return S_OK;
}

//========================================
// Uninit�֐� - �I������ -
//========================================
void Uninit(void)
{
	// �������ݏ���
	Save();

	// R.N.Lib�̏I������
	RNUninit();

	// Direct3D�f�o�C�X�̔j��
	if (g_main.pD3DDevice != NULL)
	{
		g_main.pD3DDevice->Release();
		g_main.pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_main.pD3D != NULL)
	{
		g_main.pD3D->Release();
		g_main.pD3D = NULL;
	}

	// �^�X�N�o�[��\��
	ShowTaskBar(true);
}

//========================================
// Update�֐� - �X�V���� - 
//========================================
void Update(void)
{
	// �X�V����
	RNUpdate();	// R.N.Lib
}

//========================================
// Draw�֐� - �`�揈�� -
//========================================
void Draw(void)
{
	// ��ʃN���A
	g_main.pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DCOLOR(g_main.bgCol), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_main.pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ�A
		SetCamera3D();	// �J����(3D)�̐ݒ菈��
		RNDraw();		// R.N.Lib

		// �`��I��
		g_main.pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�̓���ւ�
	g_main.pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// Load�֐� - �ǂݍ��ݏ��� -
//========================================
void Load(void)
{
	LoadCharacter();	// �L�����N�^�[
	LoadObject();		// �I�u�W�F�N�g
	RNLoad();			// R.N.Lib
}

//========================================
// Save�֐� - �������ݏ��� -
//========================================
void Save(void)
{
	SaveSetting();	// �ݒ�
	RNSave();		// R.N.Lib
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// GetMain�֐� - ���C�������̏��̎擾 -
//========================================
Main *GetMain(void)
{
	return &g_main;
}

//========================================
// GetDevice�֐� - �f�o�C�X�̎擾 -
//========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_main.pD3DDevice;
}

//========================================
// GetWindowHandle�֐� - �E�C���h�E�n���h�����擾 -
//========================================
HWND *GetWindowHandle(void)
{
	return &g_main.hWnd;
}

//========================================
// GetWindowPos�֐� - �E�C���h�E�̈ʒu���擾 -
//========================================
D3DXVECTOR3 GetWindowPos(void)
{
	return g_main.windowPos;
}

//========================================
// WindowModeChange�֐� - �E�C���h�E�̃��[�h��؂�ւ��� -
//========================================
void WindowModeChange(bool bFullScreen)
{
	HDC hDC = GetDC(GetDesktopWindow());			// �f�X�N�g�b�v�̃n���h��
	int nScrSizeX = GetDeviceCaps(hDC, HORZRES);	// �f�X�N�g�b�v�̉�ʃT�C�YX
	int nScrSizeY = GetDeviceCaps(hDC, VERTRES);	// �f�X�N�g�b�v�̉�ʃT�C�YY

	// �f�X�N�g�b�v�̃n���h���������
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bFullScreen)
	{// �S��ʃt���O���^�̎��A
		// �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// �S��ʃt���O���U�̎��A
		// �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// �E�C���h�E�̕\����Ԃ�ݒ�
	ShowWindow(g_main.hWnd, SW_NORMAL);

	// �N���C�A���g�̈���X�V
	UpdateWindow(g_main.hWnd);

	// �^�X�N�o�[��\��/��\��
	ShowTaskBar(bFullScreen ^ 1);
}

//========================================
// Message�֐� - ���b�Z�[�W���� -
//========================================
void Message(char *pText, char *pCaption)
{
	static int	bMessage = false;	// ���b�Z�[�W�\�������t���O(�������[�v�h�~)

	if (bMessage)
	{// ���b�Z�[�W�\�������t���O
		bMessage = false;	// ���b�Z�[�W�\�������t���O���U�ɂ���
		return;				// �������I������
	}

	if (MessageBox(*GetWindowHandle(), pText, pCaption, MB_OK) == IDOK)
	{// ���b�Z�[�W�{�b�N�X�̕Ԃ�l��OK�̎��A
		// ���b�Z�[�W�\�������t���O��^�ɂ���
		bMessage = true;
	}
}