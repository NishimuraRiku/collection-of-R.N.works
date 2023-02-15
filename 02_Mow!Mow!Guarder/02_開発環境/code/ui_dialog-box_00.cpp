//========================================
// 
// UI:�_�C�A���O�{�b�N�X[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_dialog-box_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_dialog-box_00.h"	// UI :�_�C�A���O�{�b�N�X	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�_�C�A���O�{�b�N�X[00] �̕�
// UI:�_�C�A���O�{�b�N�X[00] �̍���
#define UI_DIALOGBOX_00_WIDTH	(PIXEL*256)
#define UI_DIALOGBOX_00_HEIGHT	(PIXEL*64)

// UI:�_�C�A���O�{�b�N�X[00] �̏o���ɂ����鎞��
// UI:�_�C�A���O�{�b�N�X[00] �̏�����̂ɂ����鎞��
#define UI_DIALOGBOX_00_IN_POP_TIME		(10)
#define UI_DIALOGBOX_00_IN_CLEAR_TIME	(10)

// UI:�_�C�A���O�{�b�N�X[00] �̐ݒ�SE
#define UI_DIALOGBOX_00_SET_SE	(SOUND_LABEL_SE_CRY_001)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�_�C�A���O�{�b�N�X[00] �̃p�����[�^�[�̏���������
void InitParameterUi_dialogBox_00(void);

// UI:�_�C�A���O�{�b�N�X[00] �̏�ԏ���
void StateProcessUi_dialogBox_00(void);

// UI:�_�C�A���O�{�b�N�X[00] �̃L�[���͏���
void KeyInputUi_dialogBox_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_dialogBox_00	// �e�N�X�`���ւ̃|�C���^
						[UI_DIALOGBOX_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_dialogBox_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_dialogBox_00			g_ui_dialogBox_00;			// UI:�_�C�A���O�{�b�N�X[00] �̏��

// UI:�_�C�A���O�{�b�N�X[00] �̎�ޖ��̃e�N�X�`���̑��΃p�X
const char c_aTexturePathUi_dialogBox_00[UI_DIALOGBOX_00_TYPE_MAX][TXT_MAX] = 
{
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\00.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\01.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\02.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\03.png",
	"data\\TEXTURE\\UserInterface\\ui_dialog-box_000\\04.png",
};

//========== *** UI:�_�C�A���O�{�b�N�X[00] �̏����擾 ***
Ui_dialogBox_00 *GetUi_dialogBox_00(void)
{
	return &g_ui_dialogBox_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_dialogBox_00�֐� - UI:�_�C�A���O�{�b�N�X[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_dialogBox_00(void)
{
	Ui_dialogBox_00	*pUi	// UI:�_�C�A���O�{�b�N�X[00] �̏��̃|�C���^ 
						= &g_ui_dialogBox_00;

	pUi->type			= UI_DIALOGBOX_00_TYPE_NONE;	// ���
	pUi->state			= UI_DIALOGBOX_00_STATE_CLEAR;	// ���
	pUi->nCounterState	= 0;							// ��ԃJ�E���^�[
	pUi->fAlpha			= 0.0f;							// �S�̂̓����x
	pUi->scale			= { 1.0f,0.0f,0.0f };			// �g��{��
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_dialogBox_00(void)
{
	Ui_dialogBox_00	*pUi	// UI:�_�C�A���O�{�b�N�X[00] �̏��̃|�C���^ 
					= &g_ui_dialogBox_00;

	switch (pUi->state)
	{
	case /*/ �o�� /*/UI_DIALOGBOX_00_STATE_POP: {
		break;
	}
	case /*/ �o���� /*/UI_DIALOGBOX_00_STATE_IN_POP: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_DIALOGBOX_00_IN_POP_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= fRate;
		pUi->scale.fHeight	= fRate;

		if (pUi->nCounterState >= UI_DIALOGBOX_00_IN_POP_TIME)
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// ��Ԃ��o���ɂ���
			SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_POP);
		}

		break;
	}
	case /*/ �����Ă��� /*/UI_DIALOGBOX_00_STATE_CLEAR: {
		break;
	}
	case /*/ ������ /*/UI_DIALOGBOX_00_STATE_IN_CLEAR: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_DIALOGBOX_00_IN_POP_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= 1.0f - fRate;
		pUi->scale.fHeight	= 1.0f - fRate;

		if (pUi->nCounterState >= UI_DIALOGBOX_00_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_CLEAR);
		}

		break;
	}
	}
}

//========================================
// KeyInputUi_dialogBox_00�֐� - UI:�_�C�A���O�{�b�N�X[00] �̃L�[���͏��� -
//========================================
void KeyInputUi_dialogBox_00(void) 
{
	Ui_dialogBox_00	*pUi	// UI:�_�C�A���O�{�b�N�X[00] �̏��̃|�C���^ 
					= &g_ui_dialogBox_00;

	if (pUi->state == UI_DIALOGBOX_00_STATE_POP)
	{// ��Ԃ��o���̎��A
		if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A) || (GetMouseTrigger(MOUSE_LEFT))))
		{// ENTER�L�[ or A�{�^�� or �}�E�X���{�^�������͂��ꂽ���A
			// ��Ԃ��������ɂ���
			SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_IN_CLEAR);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_dialogBox_00�֐� - UI:�_�C�A���O�{�b�N�X[00] �̏��������� -
//========================================
void InitUi_dialogBox_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_dialogBox_00();

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
		&g_pVtxBuffUi_dialogBox_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_dialogBox_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntType = 0; nCntType < UI_DIALOGBOX_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, c_aTexturePathUi_dialogBox_00[nCntType], &g_aTextureUi_dialogBox_00[nCntType]);
	}

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_DIALOGBOX_00_WIDTH,
		UI_DIALOGBOX_00_HEIGHT,
		ANGLE_TYPE_FREE);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,255 });

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_dialogBox_00->Unlock();
}

//========================================
// UninitUi_dialogBox_00�֐� - UI:�_�C�A���O�{�b�N�X[00] �̏I������ -
//========================================
void UninitUi_dialogBox_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < UI_DIALOGBOX_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureUi_dialogBox_00[nCntType] != NULL)
		{
			g_aTextureUi_dialogBox_00[nCntType]->Release();
			g_aTextureUi_dialogBox_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_dialogBox_00 != NULL) 
	{
		g_pVtxBuffUi_dialogBox_00->Release();
		g_pVtxBuffUi_dialogBox_00 = NULL;
	}
}

//========================================
// UpdateUi_dialogBox_00�֐� - UI:�_�C�A���O�{�b�N�X[00] �̍X�V���� -
//========================================
void UpdateUi_dialogBox_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_dialogBox_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_dialogBox_00	*pUi	// UI:�_�C�A���O�{�b�N�X[00] �̏��̃|�C���^
						= &g_ui_dialogBox_00;

	// ��ԏ���
	StateProcessUi_dialogBox_00();

	// �L�[���͏���
	KeyInputUi_dialogBox_00();

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		UI_DIALOGBOX_00_POS,
		INITD3DXVECTOR3,
		false,
		UI_DIALOGBOX_00_WIDTH
		* pUi->scale.fWidth,
		UI_DIALOGBOX_00_HEIGHT
		* pUi->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * g_ui_dialogBox_00.fAlpha) });
}

//========================================
// DrawUi_dialogBox_00�֐� - UI:�_�C�A���O�{�b�N�X[00] �̕`�揈�� -
//========================================
void DrawUi_dialogBox_00(void)
{
	Ui_dialogBox_00	*pUi	// UI:�_�C�A���O�{�b�N�X[00] �̏��̃|�C���^
					= &g_ui_dialogBox_00;

	if (pUi->state == UI_DIALOGBOX_00_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_dialogBox_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_dialogBox_00[pUi->type]);

	// UI:�_�C�A���O�{�b�N�X[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetState�֐� - UI:�_�C�A���O�{�b�N�X[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE state) 
{
	Ui_dialogBox_00	*pUi	// UI:�_�C�A���O�{�b�N�X[00] �̏��̃|�C���^
					= &g_ui_dialogBox_00;

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}

//========================================
// SetUi_dialogBox_00�֐� - UI:�_�C�A���O�{�b�N�X[00] �̐ݒ菈�� -
//========================================
void SetUi_dialogBox_00(UI_DIALOGBOX_00_TYPE type)
{
	Ui_dialogBox_00	*pUi	// UI:�_�C�A���O�{�b�N�X[00] �̏��̃|�C���^ 
					= &g_ui_dialogBox_00;

	if (pUi->state != UI_DIALOGBOX_00_STATE_CLEAR) 
	{// �����Ă����ԂłȂ����A
		// �������I������
		return;
	}

	// �ݒ�SE���Đ�
	PlaySound(UI_DIALOGBOX_00_SET_SE);

	// ��ނ���
	pUi->type = type;

	// �o�����̏�Ԃɂ���
	SetStateUi_dialogBox_00(UI_DIALOGBOX_00_STATE_IN_POP);
}