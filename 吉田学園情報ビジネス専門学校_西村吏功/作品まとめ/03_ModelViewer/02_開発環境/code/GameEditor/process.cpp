//========================================
// 
// ��{�����N���X
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "process.h"
#include "../../../../_R.N.Lib/RNmain.h"

//****************************************
// �}�N����`
//****************************************
#define WINDOW_NAME	"R.N.ModelViewer"		// �E�C���h�E�̖��O
#define CLASS_NAME  "WindowClass"			// �E�C���h�E�N���X
#define INIT_MODE   (CProcess::MODE_MAIN)	// �����̃��[�h

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Process�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CProcess::CProcess(void) {
	m_nCountFPS = 0;	// FPS�̃J�E���^�[
	m_mode = MODENONE;	// ���[�h
}

//========================================
// �f�X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CProcess::~CProcess(void) {

}

//========================================
// ModeInit�֐� - ���[�h�ɉ��������������� -
// Author:RIKU NISHIMURA
//========================================
void CProcess::ModeInit(void) {
	switch (m_mode) {
	case MODE_MAIN:mode_gameEditor.Init(); break;
	}
}

//========================================
// ModeUninit�֐� - ���[�h�ɉ������I������ -
// Author:RIKU NISHIMURA
//========================================
void CProcess::ModeUninit(void) {
	switch (m_mode) {
	case MODE_MAIN:mode_gameEditor.Uninit(); break;
	}
}

//========================================
// ModeUpdate�֐� - ���[�h�ɉ������X�V���� -
// Author:RIKU NISHIMURA
//========================================
void CProcess::ModeUpdate(void) {
	switch (m_mode) {
	case MODE_MAIN:mode_gameEditor.Update(); break;
	}
}

//========================================
// MainLoop�֐� - ���C�����[�v -
// Author:RIKU NISHIMURA
//========================================
int CProcess::MainLoop(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow) {
	MSG msg;				// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS���v����������

	// �E�B���h�E�������
	CreateWindowInfo cwi = {
		WindowProc,		// �E�B���h�E�v���V�[�W��
		hInstance,		// �C���X�^���X�n���h��
		CLASS_NAME,		// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,	// �E�B���h�E�̖��O
		SCREEN_WIDTH,	// �E�B���h�E�̕�
		SCREEN_HEIGHT,	// �E�B���h�E�̍���
		true,			// �E�B���h�E���[�h
	};
	CreateWindowRN(cwi);

	// ����������
	if (FAILED(Init(hInstance))) {
		return -1;	// ���������������s�����ꍇ
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0; // ���ݎ���������������
	dwExecLastTime = timeGetTime(); // ���ݎ������擾(�ۑ�)

	dwFrameCount = 0;	// �t���[���J�E���g������������
	dwFPSLastTime = timeGetTime();	// ���ݎ������擾(�ۑ�)

	// ���b�Z�[�W���[�v
	while (1) {
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
				m_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// FPS�𑪒肵��������ۑ�
				dwFPSLastTime = dwCurrentTime;

				// �t���[���J�E���g���N���A
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60/1�b�o��
				Update();	// �X�V����
				Draw();		// �`�揈��

				// �����J�n�̎���(���ݎ���)��ۑ�
				dwExecLastTime = dwCurrentTime;

				// �t���[���J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//========================================
// Init�֐� - ���������� -
// Author:RIKU NISHIMURA
//========================================
HRESULT CProcess::Init(HINSTANCE hInstance) {
	Load();	// �ǂݍ��ݏ���
	RNLib::Init(hInstance);	// R.N.Lib�̏���������
	SetMode(INIT_MODE);	// ���[�h��ݒ肷��
	SetBGColor(DEFAULTBGCOLOR);	// �w�i�F��ݒ�

	return S_OK;
}

//========================================
// Uninit�֐� - �I������ -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Uninit(void) {
	Save();	// �������ݏ���
	ShowTaskBar(true);	// �^�X�N�o�[��\��

	// �I������
	RNLib::Uninit();		// R.N.Lib
	ModeUninit();	// ���[�h
}

//========================================
// Update�֐� - �X�V���� - 
// Author:RIKU NISHIMURA
//========================================
void CProcess::Update(void) {
	RNLib::Update();		// R.N.Lib
	ModeUpdate();	// ���[�h
}

//========================================
// Draw�֐� - �`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Draw(void) {
	RNLib::Draw();	// R.N.Lib
}

//========================================
// Load�֐� - �ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Load(void) {
	RNLib::Load();	// R.N.Lib
}

//========================================
// Save�֐� - �������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Save(void) {
	RNLib::Save();		// R.N.Lib
	SaveSetting();	// �ݒ�
}

//========================================
// SetMode�֐� - ���[�h�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void CProcess::SetMode(MODE newMode) {
	// ���݂̃��[�h�̏I������
	ModeUninit();

	// ���݂̃��[�h��؂�ւ���
	m_mode = newMode;

	// �V�������[�h�̏���������
	ModeInit();
}
