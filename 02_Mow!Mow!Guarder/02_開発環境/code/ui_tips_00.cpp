//========================================
// 
// UI:TIPS[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_tips_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_tips_00.h"	// UI :TIPS	[00]

//****************************************
// �}�N����`
//****************************************
// UI:TIPS[00] �̕�
// UI:TIPS[00] �̍���
#define UI_TIPS_00_WIDTH	(PIXEL*256)
#define UI_TIPS_00_HEIGHT	(PIXEL*256)

// UI:TIPS[00] �̏o���ɂ����鎞��
// UI:TIPS[00] �̏�����̂ɂ����鎞��
// UI:TIPS[00] �̐؂�ւ��ɂ����鎞��
#define UI_TIPS_00_IN_POP_TIME		(10)
#define UI_TIPS_00_IN_CLEAR_TIME	(10)
#define UI_TIPS_00_SWITCH_TIME		(5)

// UI:TIPS[00] �̐؂�ւ����̍����̉���
// UI:TIPS[00] �̐؂�ւ����̍����̏��
#define UI_TIPS_00_SWITCH_HEIGHT_MIN	(0.95f)
#define UI_TIPS_00_SWITCH_HEIGHT_MAX	(1.0f)

// UI:TIPS[00] �̐؂�ւ�SE
// UI:TIPS[00] �̐ݒ�SE
#define UI_TIPS_00_SWITCH_SE	(SOUND_LABEL_SE_SWITCH_000)
#define UI_TIPS_00_SET_SE		(SOUND_LABEL_SE_DETERMINATION_001)

// UI:TIPS[00] �̃X�e�b�v�ő吔
#define UI_TIPS_00_STEP_MAX	(3)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:TIPS[00] �̃p�����[�^�[�̏���������
void InitParameterUi_tips_00(void);
// UI:TIPS[00] �̏�ԏ���
void StateProcessUi_tips_00(void);
// UI:TIPS[00] �̃L�[���͏���
void KeyInputUi_tips_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_tips_00	// �e�N�X�`���ւ̃|�C���^
						[UI_TIPS_00_STEP_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_tips_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_tips_00				g_ui_tips_00;			// UI:TIPS[00] �̏��

// UI:TIPS[00] �̃X�e�b�v���̃e�N�X�`���̑��΃p�X
const char c_aTexturePathUi_tips_00[UI_TIPS_00_STEP_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_tips_000\\00.png",
	"data\\TEXTURE\\UserInterface\\ui_tips_000\\01.png",
	"data\\TEXTURE\\UserInterface\\ui_tips_000\\02.png",
};

//========== *** UI:TIPS[00] �̏����擾 ***
Ui_tips_00 *GetUi_tips_00(void)
{
	return &g_ui_tips_00;
}

//========== *** UI:TIPS[00] �̐ؑւ��擾 ***
bool GetSwitchUi_tips_00(void)
{
	return (!g_ui_tips_00.bStepViewAllOld && g_ui_tips_00.bStepViewAll);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_tips_00�֐� - UI:TIPS[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_tips_00(void)
{
	Ui_tips_00	*pUi	// UI:TIPS[00] �̏��̃|�C���^ 
				= &g_ui_tips_00;

	pUi->nStep				= (ACTIVE_INPUT_TYPE)0;		// �X�e�b�v
	pUi->bStepViewAll		= false;					// �X�e�b�v��S�ĉ{�������t���O
	pUi->bStepViewAllOld	= false;					// �X�e�b�v��S�ĉ{�������t���O(�ߋ�)
	pUi->state				= UI_TIPS_00_STATE_CLEAR;	// ���
	pUi->nCounterState		= 0;						// ��ԃJ�E���^�[
	pUi->fAlpha				= 0.0f;						// �S�̂̓����x
	pUi->scale				= { 1.0f,0.0f,0.0f };		// �g��{��
	pUi->pos				= INITD3DXVECTOR3;			// �ʒu
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_tips_00(void)
{
	Ui_tips_00	*pUi	// UI:TIPS[00] �̏��̃|�C���^ 
				= &g_ui_tips_00;

	switch (pUi->state)
	{
	case /*/ �o�� /*/UI_TIPS_00_STATE_POP: {

		break;
	}
	case /*/ �o���� /*/UI_TIPS_00_STATE_IN_POP: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_TIPS_00_IN_POP_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= fRate;
		pUi->scale.fHeight	= fRate;

		if (pUi->nCounterState >= UI_TIPS_00_IN_POP_TIME)
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// ��Ԃ��o���ɂ���
			SetStateUi_tips_00(UI_TIPS_00_STATE_POP);
		}

		break;
	}
	case /*/ �����Ă��� /*/UI_TIPS_00_STATE_CLEAR: {

		break;
	}
	case /*/ ������ /*/UI_TIPS_00_STATE_IN_CLEAR: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_TIPS_00_IN_CLEAR_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= 1.0f - fRate;
		pUi->scale.fHeight	= 1.0f - fRate;

		if (pUi->nCounterState >= UI_TIPS_00_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_tips_00(UI_TIPS_00_STATE_CLEAR);
		}

		break;
	}
	case /*/ �؂�ւ� /*/UI_TIPS_00_STATE_SWITCH: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_TIPS_00_SWITCH_TIME;

		// �����̔{�����X�V
		pUi->scale.fHeight = 
			(UI_TIPS_00_SWITCH_HEIGHT_MAX * fRate) +
			(UI_TIPS_00_SWITCH_HEIGHT_MIN * (1.0f - fRate));

		if (pUi->nCounterState >= UI_TIPS_00_SWITCH_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// ��Ԃ��o���ɂ���
			SetStateUi_tips_00(UI_TIPS_00_STATE_POP);
		}

		break;
	}
	}
}

//========================================
// KeyInputUi_tips_00�֐� - UI:TIPS[00] �̃L�[���͏��� -
//========================================
void KeyInputUi_tips_00(void) 
{
	Ui_tips_00	*pUi	// UI:TIPS[00] �̏��̃|�C���^ 
					= &g_ui_tips_00;

	// �ߋ��̃X�e�b�v��S�ĉ{���t���O��ۑ�
	pUi->bStepViewAllOld = pUi->bStepViewAll;

	if (pUi->state == UI_TIPS_00_STATE_POP)
	{// ��Ԃ��o���̎��A
		bool	bStepSwitch	// �X�e�b�v�؂�ւ��t���O
				= false;

		if ((GetKeyboardTrigger(DIK_A))		||
			(GetKeyboardTrigger(DIK_LEFT))	||
			(GetButtonTrigger(BUTTON_LEFT))	||
			(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT]))
		{// ���ɓ��͂��ꂽ���A�X�e�b�v�����Z����
			int nStep = pUi->nStep;
			int nStepTemp = nStep;
			nStep--;
			IntControl(&nStep, UI_TIPS_00_STEP_MAX - 1, 0);
			pUi->nStep	= (ACTIVE_INPUT_TYPE)nStep;
			bStepSwitch = nStepTemp != nStep;	// �X�e�b�v�؂�ւ��t���O��ݒ肷��
		}
		else if (
			(GetKeyboardTrigger(DIK_D))			||
			(GetKeyboardTrigger(DIK_RIGHT))		||
			(GetButtonTrigger(BUTTON_RIGHT))	||
			(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT]))
		{// �E�ɓ��͂��ꂽ���A�X�e�b�v�����Z����
			int nStep = pUi->nStep;
			int nStepTemp = nStep;
			nStep++;
			IntControl(&nStep, UI_TIPS_00_STEP_MAX -1, 0);
			pUi->nStep	= (ACTIVE_INPUT_TYPE)nStep;
			bStepSwitch = nStepTemp != nStep;	// �X�e�b�v�؂�ւ��t���O��ݒ肷��
		}

		if (pUi->nStep >= UI_TIPS_00_STEP_MAX - 1)
		{// �X�e�b�v���X�e�b�v���ɒB���Ă��鎞�A
			pUi->bStepViewAll = true;	// �X�e�b�v��S�ĉ{�������t���O��^�ɂ���
		}

		if (bStepSwitch) 
		{// �X�e�b�v�؂�ւ��t���O���^�̎��A
			SetStateUi_tips_00(	// ��Ԃ�؂�ւ��ɐݒ�
				UI_TIPS_00_STATE_SWITCH);
			PlaySound(			// �؂�ւ�SE���Đ�
				UI_TIPS_00_SWITCH_SE);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_tips_00�֐� - UI:TIPS[00] �̏��������� -
//========================================
void InitUi_tips_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_tips_00();

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
		&g_pVtxBuffUi_tips_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_tips_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntStep = 0; nCntStep < UI_TIPS_00_STEP_MAX; nCntStep++)
	{
		D3DXCreateTextureFromFile(pDevice, c_aTexturePathUi_tips_00[nCntStep], &g_aTextureUi_tips_00[nCntStep]);
	}

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_TIPS_00_WIDTH,
		UI_TIPS_00_HEIGHT,
		ANGLE_TYPE_FREE);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,255 });

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_tips_00->Unlock();
}

//========================================
// UninitUi_tips_00�֐� - UI:TIPS[00] �̏I������ -
//========================================
void UninitUi_tips_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntStep = 0; nCntStep < UI_TIPS_00_STEP_MAX; nCntStep++)
	{
		if (g_aTextureUi_tips_00[nCntStep] != NULL)
		{
			g_aTextureUi_tips_00[nCntStep]->Release();
			g_aTextureUi_tips_00[nCntStep] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_tips_00 != NULL) 
	{
		g_pVtxBuffUi_tips_00->Release();
		g_pVtxBuffUi_tips_00 = NULL;
	}
}

//========================================
// UpdateUi_tips_00�֐� - UI:TIPS[00] �̍X�V���� -
//========================================
void UpdateUi_tips_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_tips_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_tips_00	*pUi	// UI:TIPS[00] �̏��̃|�C���^
					= &g_ui_tips_00;

	// ��ԏ���
	StateProcessUi_tips_00();

	// �L�[���͏���
	KeyInputUi_tips_00();

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		pUi->pos,
		INITD3DXVECTOR3,
		false,
		UI_TIPS_00_WIDTH
		* pUi->scale.fWidth,
		UI_TIPS_00_HEIGHT
		* pUi->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * g_ui_tips_00.fAlpha) });
}

//========================================
// DrawUi_tips_00�֐� - UI:TIPS[00] �̕`�揈�� -
//========================================
void DrawUi_tips_00(void)
{
	Ui_tips_00	*pUi	// UI:TIPS[00] �̏��̃|�C���^
					= &g_ui_tips_00;

	if (pUi->state == UI_TIPS_00_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_tips_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_tips_00[pUi->nStep]);

	// UI:TIPS[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetState�֐� - UI:TIPS[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_tips_00(UI_TIPS_00_STATE state) 
{
	Ui_tips_00	*pUi	// UI:TIPS[00] �̏��̃|�C���^
				= &g_ui_tips_00;

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}

//========================================
// SetUi_tips_00�֐� - UI:TIPS[00] �̐ݒ菈�� -
//========================================
void SetUi_tips_00(D3DXVECTOR3 pos)
{
	Ui_tips_00	*pUi	// UI:TIPS[00] �̏��̃|�C���^ 
				= &g_ui_tips_00;

	if (pUi->state != UI_TIPS_00_STATE_CLEAR)
	{// �����Ă����ԂłȂ����A
		// �������I������
		return;
	}

	// �Z�b�gSE���Đ�
	PlaySound(UI_TIPS_00_SET_SE);

	pUi->nStep = 0;				// �X�e�b�v��������
	pUi->pos = pos;				// �ʒu����
	pUi->bStepViewAll = false;	// �X�e�b�v��S�ĉ{�������t���O���U�ɂ���

	// �o�����̏�Ԃɂ���
	SetStateUi_tips_00(UI_TIPS_00_STATE_IN_POP);
}