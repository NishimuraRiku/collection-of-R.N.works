//========================================
// 
// �J����(3D)�̑��쏈��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** camera3D_control.cpp ***
//========================================
// R.N.Lib
#include "../../../R.N.Lib.h"

//****************************************
// �}�N����`
//****************************************
// �J����(3D)�̃J�[�\���̉�]��
#define CAMERA3D_ROT_FORCE (D3DXVECTOR3(0.004f,-0.006f,0.0f))
// �J����(3D)�̉�]��
#define CAMERA3D_ROT_FORCE_BY_CURSOR (D3DXVECTOR3(0.0005f,-0.00075f,0.0f))
// �J����(3D)�̉�]�̌����l
#define CAMERA3D_SPIN_DAMP (0.9f)
// �J����(3D)�̏����̍����̏��
#define CAMERA3D_HEIGHT_MAX (0.99f)
// �J����(3D)�̏����̍����̉���
#define CAMERA3D_HEIGHT_MIN (-0.99f)
// �J����(3D)�̈ړ���
#define CAMERA3D_MOVEFORCE (0.1f)
// �J����(3D)�̃Y�[����
#define CAMERA3D_ZOOMFORCE (8.0f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------| *** �J����(3D)�̏�Ԃɉ��������쏈�� *** |--------
//============================================================

//========================================
// GRAB_AIR_MOUSE
//========================================
void Camera3DControl_GRAB_AIR_MOUSE(Camera3D *pCamera3D)
{
	if (ImGui_GetWindowFocused()) 
	{// ImGui�̃E�B���h�E�Ƀt�H�[�J�X���������Ă��鎞�A
		return;	// �������I������
	}

	if (GetMousePress(MOUSEBUTTON_RIGHT))
	{// �}�E�X�̉E�{�^����������Ă��鎞�A
		// �J�[�\���̈ړ��ʂɉ����ĉ�]
		AxisRotationCamera3D(DIRECTION_UP, GetCursorMove().y * CAMERA3D_ROT_FORCE_BY_CURSOR.x);
		AxisRotationCamera3D(DIRECTION_LEFT, GetCursorMove().x * CAMERA3D_ROT_FORCE_BY_CURSOR.y);
	}
	else
	{// ��������Y�����Ȃ����A
		if (GetKeyboardPress(DIK_UP)) { AxisRotationCamera3D(DIRECTION_UP, CAMERA3D_ROT_FORCE.x); }	// �㎲��]
		if (GetKeyboardPress(DIK_DOWN)) { AxisRotationCamera3D(DIRECTION_DOWN, CAMERA3D_ROT_FORCE.x); }	// ������]
		if (GetKeyboardPress(DIK_LEFT)) { AxisRotationCamera3D(DIRECTION_LEFT, CAMERA3D_ROT_FORCE.y); }	// ������]
		if (GetKeyboardPress(DIK_RIGHT)) { AxisRotationCamera3D(DIRECTION_RIGHT, CAMERA3D_ROT_FORCE.y); }	// �E����]
	}

	pCamera3D->rot += pCamera3D->spin;				// �������X�V
	pCamera3D->spin *= CAMERA3D_SPIN_DAMP;			// ��]�ʂ�����
	pCamera3D->fHeight += pCamera3D->fVerticalMove;	// �������X�V
	pCamera3D->fVerticalMove *= CAMERA3D_SPIN_DAMP;	// �c�����̈ړ��ʂ�����

	// �����𐧌�
	RotControl(&pCamera3D->rot);

	// �����𐧌�
	FloatControl(&pCamera3D->fHeight, CAMERA3D_HEIGHT_MAX, CAMERA3D_HEIGHT_MIN);

	if (GetMousePress(MOUSEBUTTON_LEFT))
	{// �}�E�X�̍��{�^����������Ă��鎞�A
		D3DXVECTOR3 move;	// �ړ���
							// �J�[�\���̈ړ���
		D3DXVECTOR3 cursorMove = GetCursorMove();
		// �ړ�����X
		float fMoveAngleX = pCamera3D->rot.y - (D3DX_PI * 0.5f);

		// XZ
		move.x = sinf(fMoveAngleX) * cursorMove.x;
		move.z = cosf(fMoveAngleX) * cursorMove.x;
		// Y
		move.y = cursorMove.y * (1.0f - fabsf(pCamera3D->fHeight));

		// �c�������XZ��Y�̈ړ��ʉ��Z
		move.x += sinf(pCamera3D->rot.y) * cursorMove.y * pCamera3D->fHeight;
		move.z += cosf(pCamera3D->rot.y) * cursorMove.y * pCamera3D->fHeight;

		// �����_�Ɉړ��ʂ𔽉f
		pCamera3D->posR += move * CAMERA3D_MOVEFORCE;
	}

	if (GetWheelSpin() == WHEELSPIN_FRONT)
	{
		pCamera3D->posR.x += sinf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.z += cosf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.y += CAMERA3D_ZOOMFORCE * -pCamera3D->fHeight;
	}
	else if (GetWheelSpin() == WHEELSPIN_BACK)
	{
		pCamera3D->posR.x -= sinf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.z -= cosf(pCamera3D->rot.y) * CAMERA3D_ZOOMFORCE * (1.0f - fabsf(pCamera3D->fHeight));
		pCamera3D->posR.y -= CAMERA3D_ZOOMFORCE * -pCamera3D->fHeight;
	}

	// ���_�̈ʒu��ݒ�
	pCamera3D->posV.x = pCamera3D->posR.x + (sinf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
	pCamera3D->posV.y = pCamera3D->posR.y + (pCamera3D->fLength * pCamera3D->fHeight);
	pCamera3D->posV.z = pCamera3D->posR.z + (cosf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
}