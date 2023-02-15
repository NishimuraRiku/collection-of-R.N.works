//========================================
// 
// UI:���[�U�[�K�C�h[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_user-guid_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_user-guid_00.h"	// UI :���[�U�[�K�C�h	[00]

//****************************************
// �}�N����`
//****************************************
// UI:���[�U�[�K�C�h[00] �̕�
// UI:���[�U�[�K�C�h[00] �̍���
#define UI_USERGUID_00_WIDTH	(PIXEL*256)
#define UI_USERGUID_00_HEIGHT	(PIXEL*256)

// UI:���[�U�[�K�C�h[00] �̏o���ɂ����鎞��
// UI:���[�U�[�K�C�h[00] �̏�����̂ɂ����鎞��
// UI:���[�U�[�K�C�h[00] �̐؂�ւ��ɂ����鎞��
#define UI_USERGUID_00_IN_POP_TIME		(10)
#define UI_USERGUID_00_IN_CLEAR_TIME	(10)
#define UI_USERGUID_00_SWITCH_TIME		(5)

// UI:���[�U�[�K�C�h[00] �̐؂�ւ����̍����̉���
// UI:���[�U�[�K�C�h[00] �̐؂�ւ����̍����̏��
#define UI_USERGUID_00_SWITCH_HEIGHT_MIN	(0.95f)
#define UI_USERGUID_00_SWITCH_HEIGHT_MAX	(1.0f)

// UI:���[�U�[�K�C�h[00] �̐؂�ւ�SE
#define UI_USERGUID_00_SWITCH_SE	(SOUND_LABEL_SE_SWITCH_000)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���[�U�[�K�C�h[00] �̃p�����[�^�[�̏���������
void InitParameterUi_userGuid_00(void);

// UI:���[�U�[�K�C�h[00] �̏�ԏ���
void StateProcessUi_userGuid_00(void);

// UI:���[�U�[�K�C�h[00] �̃L�[���͏���
void KeyInputUi_userGuid_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_userGuid_00	// �e�N�X�`���ւ̃|�C���^
						[ACTIVE_INPUT_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_userGuid_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_userGuid_00			g_ui_userGuid_00;			// UI:���[�U�[�K�C�h[00] �̏��

// UI:���[�U�[�K�C�h[00] �̎�ޖ��̃e�N�X�`���̑��΃p�X
const char c_aTexturePathUi_userGuid_00[ACTIVE_INPUT_TYPE_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_user-guid_000\\keyboard.png",
	"data\\TEXTURE\\UserInterface\\ui_user-guid_000\\controller.png",
};

//========== *** UI:���[�U�[�K�C�h[00] �̏����擾 ***
Ui_userGuid_00 *GetUi_userGuid_00(void)
{
	return &g_ui_userGuid_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_userGuid_00�֐� - UI:���[�U�[�K�C�h[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_userGuid_00(void)
{
	Ui_userGuid_00	*pUi	// UI:���[�U�[�K�C�h[00] �̏��̃|�C���^ 
					= &g_ui_userGuid_00;

	pUi->type			= (ACTIVE_INPUT_TYPE)0;			// ���
	pUi->state			= UI_USERGUID_00_STATE_CLEAR;	// ���
	pUi->nCounterState	= 0;							// ��ԃJ�E���^�[
	pUi->fAlpha			= 0.0f;							// �S�̂̓����x
	pUi->scale			= { 1.0f,0.0f,0.0f };			// �g��{��
	pUi->pos			= INITD3DXVECTOR3;				// �ʒu
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_userGuid_00(void)
{
	Ui_userGuid_00	*pUi	// UI:���[�U�[�K�C�h[00] �̏��̃|�C���^ 
					= &g_ui_userGuid_00;

	switch (pUi->state)
	{
	case /*/ �o�� /*/UI_USERGUID_00_STATE_POP: {

		break;
	}
	case /*/ �o���� /*/UI_USERGUID_00_STATE_IN_POP: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_USERGUID_00_IN_POP_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= fRate;
		pUi->scale.fHeight	= fRate;

		if (pUi->nCounterState >= UI_USERGUID_00_IN_POP_TIME)
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// ��Ԃ��o���ɂ���
			SetStateUi_userGuid_00(UI_USERGUID_00_STATE_POP);
		}

		break;
	}
	case /*/ �����Ă��� /*/UI_USERGUID_00_STATE_CLEAR: {

		break;
	}
	case /*/ ������ /*/UI_USERGUID_00_STATE_IN_CLEAR: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_USERGUID_00_IN_CLEAR_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= 1.0f - fRate;
		pUi->scale.fHeight	= 1.0f - fRate;

		if (pUi->nCounterState >= UI_USERGUID_00_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_userGuid_00(UI_USERGUID_00_STATE_CLEAR);
		}

		break;
	}
	case /*/ �؂�ւ� /*/UI_USERGUID_00_STATE_SWITCH: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_USERGUID_00_SWITCH_TIME;

		// �����̔{�����X�V
		pUi->scale.fHeight = 
			(UI_USERGUID_00_SWITCH_HEIGHT_MAX * fRate) +
			(UI_USERGUID_00_SWITCH_HEIGHT_MIN * (1.0f - fRate));

		if (pUi->nCounterState >= UI_USERGUID_00_SWITCH_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// ��Ԃ��o���ɂ���
			SetStateUi_userGuid_00(UI_USERGUID_00_STATE_POP);
		}

		break;
	}
	}
}

//========================================
// KeyInputUi_userGuid_00�֐� - UI:���[�U�[�K�C�h[00] �̃L�[���͏��� -
//========================================
void KeyInputUi_userGuid_00(void) 
{
	Ui_userGuid_00	*pUi	// UI:���[�U�[�K�C�h[00] �̏��̃|�C���^ 
					= &g_ui_userGuid_00;

	if (pUi->state == UI_USERGUID_00_STATE_POP)
	{// ��Ԃ��o���̎��A
		bool	bTypeSwitch	// ��ސ؂�ւ��t���O
				= false;

		if ((GetKeyboardTrigger(DIK_A))		||
			(GetKeyboardTrigger(DIK_LEFT))	||
			(GetButtonTrigger(BUTTON_LEFT))	||
			(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT]))
		{// ���ɓ��͂��ꂽ���A��ނ����Z����
			int nType	= pUi->type;
			--nType		%= ACTIVE_INPUT_TYPE_MAX;
			IntLoopControl(&nType, ACTIVE_INPUT_TYPE_MAX, 0);
			pUi->type	= (ACTIVE_INPUT_TYPE)nType;
			bTypeSwitch = true;	// ��ސ؂�ւ��t���O��^�ɂ���
		}
		else if (
			(GetKeyboardTrigger(DIK_D))			||
			(GetKeyboardTrigger(DIK_RIGHT))		||
			(GetButtonTrigger(BUTTON_RIGHT))	||
			(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT]))
		{// �E�ɓ��͂��ꂽ���A��ނ����Z����
			int nType	= pUi->type;
			++nType		%= ACTIVE_INPUT_TYPE_MAX;
			pUi->type	= (ACTIVE_INPUT_TYPE)nType;
			bTypeSwitch = true;	// ��ސ؂�ւ��t���O��^�ɂ���
		}

		if (bTypeSwitch) 
		{// ��ސ؂�ւ��t���O���^�̎��A
			SetStateUi_userGuid_00(	// ��Ԃ�؂�ւ��ɐݒ�
				UI_USERGUID_00_STATE_SWITCH);
			PlaySound(				// �؂�ւ�SE���Đ�
				UI_USERGUID_00_SWITCH_SE);
		}
	}
}

//================================================================================CONTROLLER
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_userGuid_00�֐� - UI:���[�U�[�K�C�h[00] �̏��������� -
//========================================
void InitUi_userGuid_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_userGuid_00();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_userGuid_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_userGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntType = 0; nCntType < ACTIVE_INPUT_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, c_aTexturePathUi_userGuid_00[nCntType], &g_aTextureUi_userGuid_00[nCntType]);
	}

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_USERGUID_00_WIDTH,
		UI_USERGUID_00_HEIGHT,
		ANGLE_TYPE_FREE);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,255 });

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_userGuid_00->Unlock();
}

//========================================
// UninitUi_userGuid_00�֐� - UI:���[�U�[�K�C�h[00] �̏I������ -
//========================================
void UninitUi_userGuid_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < ACTIVE_INPUT_TYPE_MAX; nCntType++)
	{
		if (g_aTextureUi_userGuid_00[nCntType] != NULL)
		{
			g_aTextureUi_userGuid_00[nCntType]->Release();
			g_aTextureUi_userGuid_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_userGuid_00 != NULL) 
	{
		g_pVtxBuffUi_userGuid_00->Release();
		g_pVtxBuffUi_userGuid_00 = NULL;
	}
}

//========================================
// UpdateUi_userGuid_00�֐� - UI:���[�U�[�K�C�h[00] �̍X�V���� -
//========================================
void UpdateUi_userGuid_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_userGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_userGuid_00	*pUi	// UI:���[�U�[�K�C�h[00] �̏��̃|�C���^
					= &g_ui_userGuid_00;

	// ��ԏ���
	StateProcessUi_userGuid_00();

	// �L�[���͏���
	KeyInputUi_userGuid_00();

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		pUi->pos,
		INITD3DXVECTOR3,
		false,
		UI_USERGUID_00_WIDTH
		* pUi->scale.fWidth,
		UI_USERGUID_00_HEIGHT
		* pUi->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * g_ui_userGuid_00.fAlpha) });
}

//========================================
// DrawUi_userGuid_00�֐� - UI:���[�U�[�K�C�h[00] �̕`�揈�� -
//========================================
void DrawUi_userGuid_00(void)
{
	Ui_userGuid_00	*pUi	// UI:���[�U�[�K�C�h[00] �̏��̃|�C���^
					= &g_ui_userGuid_00;

	if (pUi->state == UI_USERGUID_00_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_userGuid_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_userGuid_00[pUi->type]);

	// UI:���[�U�[�K�C�h[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetState�֐� - UI:���[�U�[�K�C�h[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_userGuid_00(UI_USERGUID_00_STATE state) 
{
	Ui_userGuid_00	*pUi	// UI:���[�U�[�K�C�h[00] �̏��̃|�C���^
					= &g_ui_userGuid_00;

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}

//========================================
// SetUi_userGuid_00�֐� - UI:���[�U�[�K�C�h[00] �̐ݒ菈�� -
//========================================
void SetUi_userGuid_00(D3DXVECTOR3 pos, ACTIVE_INPUT_TYPE type)
{
	Ui_userGuid_00	*pUi	// UI:���[�U�[�K�C�h[00] �̏��̃|�C���^ 
					= &g_ui_userGuid_00;

	if (pUi->state != UI_USERGUID_00_STATE_CLEAR)
	{// �����Ă����ԂłȂ����A
		// �������I������
		return;
	}

	pUi->type = type;	// ��ނ���
	pUi->pos = pos;		// �ʒu����

	// �o�����̏�Ԃɂ���
	SetStateUi_userGuid_00(UI_USERGUID_00_STATE_IN_POP);
}