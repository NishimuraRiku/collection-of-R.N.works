//========================================
// 
// �J����(3D)�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** camera3D.h ***
//========================================
#ifndef _CAMERA3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// R.N.Lib
#include "../../Other/physics.h"

//****************************************
// �}�N����`
//****************************************
//========== *** �����p�����[�^�[ ***
// �J����(3D)�̏����̎��_
#define CAMERA3D_INIT_POS_V (D3DXVECTOR3(0.0f,0.0f,0.0f))
// �J����(3D)�̏����̒����_
#define CAMERA3D_INIT_POS_R (D3DXVECTOR3(0.0f,0.0f,0.0f))
// �J����(3D)�̏����̏�����x�N�g��
#define CAMERA3D_INIT_POS_U (D3DXVECTOR3(0.0f,1.0f,0.0f))
// �J����(3D)�̏����̌���
#define CAMERA3D_INIT_ROT (D3DXVECTOR3(0.0f,0.0f,0.0f))
// �J����(3D)�̏����̋���
#define CAMERA3D_INIT_LENGTH (80.0f)
// �J����(3D)�̏����̍���
#define CAMERA3D_INIT_HEIGHT (0.25f)

//****************************************
// �񋓌^��`
//****************************************
// �J����(3D)�̏��
typedef enum {
	CAMERA3D_STATE_NONE,
	CAMERA3D_STATE_GRAB_AIR_MOUSE,	// �}�E�X�Œ���͂�
	CAMERA3D_STATE_MAX,
}CAMERA3D_STATE;

//****************************************
// �\���̒�`
//****************************************
// �J����(3D)�̏��\����
typedef struct {
	CAMERA3D_STATE state = CAMERA3D_STATE_NONE;	// ���
	D3DXVECTOR3 posV = CAMERA3D_INIT_POS_V;	// ���_
	D3DXVECTOR3 posR = CAMERA3D_INIT_POS_R;	// �����_
	D3DXVECTOR3 posU = CAMERA3D_INIT_POS_U;	// ������x�N�g��
	D3DXVECTOR3 move = INITD3DXVECTOR3;		// �ړ���
	D3DXVECTOR3 rot = CAMERA3D_INIT_ROT;	// ����
	D3DXVECTOR3 spin = INITD3DXVECTOR3;		// ��]��
	float fLength = CAMERA3D_INIT_LENGTH;	// ����
	float fHeight = CAMERA3D_INIT_HEIGHT;	// ����
	float fVerticalMove = 0.0f;	// �c�����̈ړ���
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	float fVibration = 0.0f;	// �U���̋���
	int nCounterVib = 0;		// �U���J�E���^�[
	D3DXVECTOR3 vibrationPos = INITD3DXVECTOR3;	// �U���ʒu
	D3DXVECTOR3 forward = INITD3DXVECTOR3;		// �O���x�N�g��
}Camera3D;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �J����(3D)�̏���������
void InitCamera3D(void);
// �J����(3D)�̏I������
void UninitCamera3D(void);
// �J����(3D)�̍X�V����
void UpdateCamera3D(void);
//========== *** �擾 ***
// �J����(3D)�̏����擾
Camera3D *GetCamera3D(void);
//========== *** �ݒ� ***
// �J����(3D)�̐ݒ菈��
void SetCamera3D(void);
// �J����(3D)�̏�Ԑݒ菈��
void SetCamera3DState(CAMERA3D_STATE state);
// �J����(3D)�̎��_�̐ݒ菈��
void SetCamera3DPosV(D3DXVECTOR3 pos);
// �J����(3D)�̒����_�̐ݒ菈��
void SetCamera3DPosR(D3DXVECTOR3 pos);
// �J����(3D)�̐U����ݒ�
void SetCamera3DVibration(float fVibration);
//========== *** ���� ***
// �J����(3D)�̈ړ�����
void MoveCamera3D(DIRECTION drct, float fMove);
// �J����(3D)�̎���]����
void AxisRotationCamera3D(DIRECTION drct, float fRot);
// �J����(3D)�̉�]����
void RotationCamera3D(DIRECTION drct, float fRot);

#endif