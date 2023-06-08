//========================================
// 
// R.N.Lib�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** R.N.Lib.h ***
//========================================
#ifndef _RNLIB_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _RNLIB_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//---------- Basis ----------
// 2D
#include "Basis\2D\polygon2D.h"
// 3D
// > Body
#include "Basis\3D\Body\human3D-base.h"
#include "Basis\3D\Body\model-setup3D.h"
#include "Basis\3D\Body\motion3D.h"
#include "Basis\3D\Body\parts3D.h"
#include "Basis\3D\Body\parts3D_class.h"
// > Camera
#include "Basis\3D\Camera\camera3D.h"
#include "Basis\3D\Camera\camera3D_control.h"
// > Character
#include "Basis\3D\Character\character3D.h"
#include "Basis\3D\Character\character3D_manager.h"
// > Draw
#include "Basis\3D\Draw\draw3D.h"
#include "Basis\3D\Draw\line3D.h"
#include "Basis\3D\Draw\material.h"
#include "Basis\3D\Draw\model.h"
#include "Basis\3D\Draw\polygon3D.h"
// > Object
#include "Basis\3D\Object\map-chip3D.h"
#include "Basis\3D\Object\map-generator3D.h"
#include "Basis\3D\Object\text3D.h"
// > Space
#include "Basis\3D\Space\fog.h"
#include "Basis\3D\Space\light.h"
// Calculation
#include "Basis\Calculation\easing.h"
#include "Basis\Calculation\geometry.h"
#include "Basis\Calculation\matrix.h"
#include "Basis\Calculation\number.h"
// Graphic
#include "Basis\Graphic\text.h"
#include "Basis\Graphic\texture.h"
// Other
#include "Basis\Other\color.h"
#include "Basis\Other\file.h"
#include "Basis\Other\input.h"
#include "Basis\Other\memory.h"
#include "Basis\Other\physics.h"
#include "Basis\Other\print.h"
#include "Basis\Other\setting.h"
#include "Basis\Other\sound.h"
#include "Basis\Other\window.h"
//---------- ImGui ----------
#include "ImGui\imconfig.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_crt-wgt.h"
#include "ImGui\imgui_impl_dx9.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_internal.h"
#include "ImGui\imgui_main.h"
#include "ImGui\imstb_rectpack.h"
#include "ImGui\imstb_textedit.h"
#include "ImGui\imstb_truetype.h"

//****************************************
// �N���X��`
//****************************************
// R.N.Lib�̃N���X
class RNLib {
public:
	//========== *** �֐��錾 ***
	/* �R���X�g���N�^   */RNLib();
	/* �f�X�g���N�^     */~RNLib();
	/* ����������       */static void Init(HINSTANCE hInstance);
	/* �I������         */static void Uninit(void);
	/* �X�V����         */static void Update(void);
	/* �`�揈��         */static void Draw(void);
	/* WindowProc������ */static void WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	/* �ǂݍ��ݏ���     */static void Load(void);
	/* �������ݏ���     */static void Save(void);

private:
	//========== *** �֐��錾 ***
	/* �����_�[�X�e�[�g�̐ݒ菈�� */static void SetRenderStateRN(void);

	//========== *** �ϐ��錾 ***
	CChr3DManager Chr3DManager;	// �L�����N�^(3D)�}�l�[�W���[
};

#endif