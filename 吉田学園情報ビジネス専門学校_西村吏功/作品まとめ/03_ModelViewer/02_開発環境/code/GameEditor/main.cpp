//========================================
// 
// ���C������
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "process.h"
#include "../../../../_R.N.Lib/RNmain.h"

//========================================
// WinMain�֐�
// Author:RIKU NISHIMURA
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow) {
	CProcess process;	// ���C�������̃C���X�^���X�𐶐�

	// ���C�����[�v
	return process.MainLoop(hInstance, hInstancePrev, IpCmdLine, nCmdShow);
}

//========================================
// WindowProc�֐�
// Author:RIKU NISHIMURA
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int nID;	// �Ԃ�l���i�[

	// R.N.Lib
	RNLib::WindowProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg) {
	case WM_DESTROY: // �E�C���h�E�j���̃��b�Z�[�W

		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�𑗂�

		break;
	case WM_KEYDOWN: // �L�[�����̃��b�Z�[�W

		switch (wParam) {
		case VK_ESCAPE: // [ESC]�L�[�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO); // �E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)

			if (nID == IDYES) {
				DestroyWindow(hWnd); // �E�C���h�E��j������
			}

			break;
		}

		break;
	case WM_CLOSE: // �I���{�^���������ꂽ��

		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO); // �E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)

		if (nID == IDYES) {
			DestroyWindow(hWnd); // �E�C���h�E��j������
		}
		else {
			return 0; // (��0��Ԃ��Ȃ��ƏI�����Ă��܂�)
		}

		break;
	case WM_LBUTTONDOWN: // �}�E�X�N���b�N�̃��b�Z�[�W

		// �ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd); // �}�E�X�����N���b�N�����E�C���h�E���A�N�e�B�u�ɂ���

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // ����̏�����Ԃ�
}