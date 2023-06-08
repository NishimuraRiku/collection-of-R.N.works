//========================================
// 
// R.N.Lib�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** R.N.Lib.cpp ***
//========================================
#include <time.h>
#include "R.N.Lib.h"

//****************************************
// �O���錾
//****************************************
// imgui_impl_win32.cpp�̃��b�Z�[�W�n���h��
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNLib�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
// Author:RIKU NISHIMURA
//========================================
RNLib::RNLib(void) {

}

//========================================
// �f�X�g���N�^
// Author:RIKU NISHIMURA
//========================================
RNLib::~RNLib(void) {

}

//========================================
// ����������
// Author:RIKU NISHIMURA
//========================================
void RNLib::Init(HINSTANCE hInstance) {
	// �����_�[�X�e�[�g�̐ݒ菈��
	SetRenderStateRN();

	// �����̎���擾
	srand((unsigned int)time(0));

	// 3D
	InitCamera3D();	// �J����(3D)
	InitLight();	// ��
	InitLine3D();	// ��(3D)
	InitModel();	// ���f��
	InitPolygon3D();// �|���S��(3D)
	// 2D
	InitPolygon2D();// �|���S��(2D)
	// Graphic
	InitText();		// �e�L�X�g
	// Other
	InitInput(		// ���͊֘A�̏���������
		hInstance,
		*GetWindowHandle());
	InitPhysics();	// �����֘A
	InitSound(		// �T�E���h
		*GetWindowHandle());
	InitSetting();	// �ݒ�

	// ImGui�̏���������
	ImGuiInit(*GetWindowHandle());
}

//========================================
// �I������
// Author:RIKU NISHIMURA
//========================================
void RNLib::Uninit(void) {
	// ImGui�̏I������
	ImGuiUninit();

	// 3D
	UninitHuman3DBase();// �q���[�}��(3D)�x�[�X
	UninitModelSetUp();	// ���f���Z�b�g�A�b�v
	UninitCamera3D();	// �J����(3D)
	UninitLight();		// ��
	UninitLine3D();		// ��(3D)
	UninitModel();		// ���f��
	UninitPolygon3D();	// �|���S��(3D)
	// 2D
	UninitPolygon2D();	// �|���S��(2D)
	// Other
	UninitInput();		// ���͊֘A
	UninitSetting();	// �ݒ�
	UninitSound();		// �T�E���h
	UninitWindow();		// �E�B���h�E
}

//========================================
// �X�V����
// Author:RIKU NISHIMURA
//========================================
void RNLib::Update(void) {
	// �����蔻��̐��̐���������
	InitHitTest3DLineNum();

	UpdateSetting();	// �ݒ�
	UpdateCamera3D();	// �J����(3D)
	UpdateLight();		// ��
	UpdateSound();		// �T�E���h
	UpdateWindowRN();	// �E�B���h�E
	DrawHitTest3DSetUp();	// �����蔻��̕`�揀������
	DrawHitTestSignOut();	// �����蔻��̓o�^��������
	UpdateInput();		// ���͊֘A

	// ImGui�̃t���O����������
	ImGuiInitFlag();
}

//========================================
// �`�揈��
// Author:RIKU NISHIMURA
//========================================
void RNLib::Draw(void) {
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ��ʃN���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DXCOLOR(GetColorInfo()->bgCol), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(pDevice->BeginScene()))
	{// �`��J�n�����������ꍇ�A
		SetCamera3D();		// �J����(3D)�̐ݒ菈��

		Draw3D();			// 3D:3D�I�u�W�F�N�g�S��
		DrawLine3D();		// 3D:��(3D)
		DrawHitTest3D();	// 3D:�����蔻��
		DrawPolygon2D();	// 2D:�|���S��(2D)

		// ImGui�̕`�揈��
		ImGuiDraw();

		// �`��I��
		pDevice->EndScene();
	}

	// �|���S��(2D & 3D)�̐ݒ萔��������
	GetPolygon2DControl()->nPolygonNum = 0;
	GetPolygon3DControl()->nPolygonNum = 0;
	// ��(3D)�̐���������
	GetLine3DControl()->nLineNum = 0;
	// ���f���̐ݒ萔��������
	GetModelInfo()->nModelSetNum = 0;

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�̓���ւ�
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// �E�B���h�E�v���V�[�W���̉�����
// Author:RIKU NISHIMURA
//========================================
void RNLib::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{// ImGui�̃E�B���h�E�v���V�[�W���̉�����
		return;
	}

	switch (uMsg)
	{
	case WM_MOUSEWHEEL: // �}�E�X�z�C�[�����񂳂ꂽ���̃��b�Z�[�W
		if (HIWORD(wParam) == WHEEL_DELTA)
		{// �}�E�X�z�C�[���̉�]��Ԃ�O��]�ɐݒ�
			GetInputInfo()->wheelSpin = WHEELSPIN_FRONT;
		}
		else
		{// �}�E�X�z�C�[���̉�]��Ԃ����]�ɐݒ�
			GetInputInfo()->wheelSpin = WHEELSPIN_BACK;
		}

		break;
	}
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================

//========================================
// �ǂݍ��ݏ���
// Author:RIKU NISHIMURA
//========================================
void RNLib::Load(void) {
	LoadFont();	// �t�H���g
}

//========================================
// �������ݏ���
// Author:RIKU NISHIMURA
//========================================
void RNLib::Save(void) {
	
}

//========================================
// �����_�[�X�e�[�g�̐ݒ菈��
// Author:RIKU NISHIMURA
//========================================
void RNLib::SetRenderStateRN(void) {
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// �ЖʃJ�����O�ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// �A���t�@�u�����h�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}