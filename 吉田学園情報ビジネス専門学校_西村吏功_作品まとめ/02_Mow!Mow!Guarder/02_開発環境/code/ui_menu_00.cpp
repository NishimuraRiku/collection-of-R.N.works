//========================================
// 
// UI:���j���[[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_menu_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_menu_00.h"	// UI:���j���[	[00]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// �}�N����`
//****************************************
// UI:���j���[[00] �̍ő吔
// UI:���j���[[00] �̕�
// UI:���j���[[00] �̍���
// UI:���j���[[00] �̃e�N�X�`���̃p�^�[����(��)
// UI:���j���[[00] �̃e�N�X�`���̃p�^�[����(�c)
// UI:���j���[[00] �̑I�����̐F
#define UI_MENU_00_MAX			(16)
#define UI_MENU_00_WIDTH		(PIXEL * 256)
#define UI_MENU_00_HEIGHT		(PIXEL * 16)
#define UI_MENU_00_SELECT_COLOR	(Color{243, 189, 63, 255})

// UI:���j���[[00] �̔z�u�ԊuX
// UI:���j���[[00] �̔z�u�ԊuY
// UI:���j���[[00] ��������̂ɂ����鎞��
#define UI_MENU_00_SPACE_X		(PIXEL * 0)
#define UI_MENU_00_SPACE_Y		(PIXEL * 16)
#define UI_MENU_00_CLEAR_TIME	(4)

// UI:���j���[[00] �̏o���ɂ����鎞��
// UI:���j���[[00] �̏o�����Ă��鎞�̑傫���ɂ�����{��
// UI:���j���[[00] �̏����Ă��鎞�̑傫���ɂ�����{��
#define UI_MENU_00_APPEAR_TIME		(10)
#define UI_MENU_00_APPEAR_SCALE		Scale{1.0f,1.0f,0.0f}
#define UI_MENU_00_DISAPPEAR_SCALE	Scale{1.0f,0.0f,0.0f}

// UI:���j���[[00] �̑I��SE
// UI:���j���[[00] �̌���SE
#define UI_MENU_00_SELECT_SE		(SOUND_LABEL_SE_SELECT_000)
#define UI_MENU_00_DETERMINATION_SE	(SOUND_LABEL_SE_DETERMINATION_001)

//****************************************
// �\���̂̒�`
//****************************************
// UI:���j���[[00] �̗\����
typedef struct 
{
	bool			bReservation;	// �\��t���O
	Ui_menu_00Set	*pSet;			// �ݒ���̃|�C���^
	int				nNum;			// ���j���[��
}Ui_menu_00Reservation;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���j���[[00] �̃p�����[�^�[����������
// UI:���j���[[00] �̊Ǘ����̃p�����[�^�[����������
// UI:���j���[[00] �̗\����̃p�����[�^�[����������
void InitParameterUi_menu_00(Ui_menu_00 *pUi);
void InitParameterUi_menu_00Control(void);
void InitParameterUi_menu_00Reservation(void);

// UI:���j���[[00] �̏�ԏ���
// UI:���j���[[00] �̍쐬����
void StateProcessUi_menu_00(void);
void CreateUi_menu_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_menu_00		// �e�N�X�`���ւ̃|�C���^
						[UI_MENU_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_menu_00		// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_menu_00				g_aUi_menu_00				// UI:���j���[[00] �̏��
						[UI_MENU_00_MAX];
Ui_menu_00Control		g_ui_menu_00Control;		// UI:���j���[[00] �̊Ǘ����
Ui_menu_00Reservation	g_ui_menu_00Reservation;	// UI:���j���[[00] �̗\����

// UI:���j���[[00] �̃e�N�X�`���t�@�C���̑��΃p�X
char g_aTexturePath[UI_MENU_00_TYPE_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_menu_000\\normal.png",
	"data\\TEXTURE\\UserInterface\\ui_menu_000\\select.png",
};

//========== *** UI:���j���[[00] �̏����擾 ***
Ui_menu_00 *GetUi_menu_00(void) 
{
	return g_aUi_menu_00;
}

//========== *** UI:���j���[[00] �̑S�̊Ǘ��̏����擾 ***
Ui_menu_00Control *GetUi_menu_00Control(void) 
{
	return &g_ui_menu_00Control;
}

//========== *** UI:���j���[[00] �̌��݂̑I��ԍ����擾 ***
int *GetSelect(void) 
{
	return &g_ui_menu_00Control.nSelect;
}


//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_menu_00�֐� - UI:���j���[[00] �̃p�����[�^�[���������� -
//========================================
void InitParameterUi_menu_00(Ui_menu_00 *pUi)
{
	pUi->pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pUi->type		= (UI_MENU_00_TYPE)0;				// ���
	pUi->bUse		= false;							// �g�p����Ă��邩�t���O
	pUi->bSelect	= false;							// �I������Ă��邩�t���O
	sprintf(pUi->aString, "");							// �\�����镶����
	pUi->nSelectNum = 0;								// ���݂̑I�����̔ԍ�
}

//========================================
// InitParameterUi_menu_00Control�֐� - UI:���j���[[00] �̊Ǘ����̃p�����[�^�[���������� -
//========================================
void InitParameterUi_menu_00Control(void)
{
	Ui_menu_00Control	*pUiCtl	// UI:���j���[[00] �̊Ǘ����̃|�C���^
						= &g_ui_menu_00Control;

	pUiCtl->state			= UI_MENU_00_STATE_NONE;			// ��� 
	pUiCtl->nCounterState	= 0;								// ��Ԃ̃J�E���^�[ 
	pUiCtl->scale			= { 0.0f,0.0f,0.0f };				// �g�嗦 
	pUiCtl->nNum			= 0;								// ���݂̎g�p�� 
	pUiCtl->nSelect			= 0;								// ���݂̑I��ԍ� 
	pUiCtl->centerPos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���S���W 
}

//========================================
// InitParameterUi_menu_00Reservation�֐� - UI:���j���[[00] �̗\����̃p�����[�^�[���������� -
//========================================
void InitParameterUi_menu_00Reservation(void)
{
	Ui_menu_00Reservation	*pUiRsv	// UI:���j���[[00] �̗\����̃|�C���^
							= &g_ui_menu_00Reservation;

	pUiRsv->bReservation	= false;	// �\��t���O
	pUiRsv->pSet			= NULL;		// �ݒ���̃|�C���^
	pUiRsv->nNum			= 0;		// ���j���[��
}

//========================================
// StateProcessUi_menu_00�֐� - UI:���j���[[00] �̏�ԏ��� -
//========================================
void StateProcessUi_menu_00(void)
{
	Ui_menu_00Control	*pUiCtl	// UI:���j���[[00] �̊Ǘ����̃|�C���^
						= &g_ui_menu_00Control;

	switch (pUiCtl->state)
	{
		//========== *** �o���� ***
	case UI_MENU_00_STATE_IN_POP: {
		if (--pUiCtl->nCounterState <= 0)
		{// ��Ԃ̃J�E���^�[�����Z��������0�ȉ��̎��A
			// �o����Ԃɂ���
			pUiCtl->state = UI_MENU_00_STATE_POP;
		}

		float	fRate	// ���� 
				= (float)pUiCtl->nCounterState / (float)UI_MENU_00_APPEAR_TIME;

		// �傫���ɂ�����{�����X�V
		pUiCtl->scale.fWidth =
			(UI_MENU_00_APPEAR_SCALE.fWidth * (1.0f - fRate)) +
			(UI_MENU_00_DISAPPEAR_SCALE.fWidth * fRate);
		pUiCtl->scale.fHeight =
			(UI_MENU_00_APPEAR_SCALE.fHeight * (1.0f - fRate)) +
			(UI_MENU_00_DISAPPEAR_SCALE.fHeight * fRate);
	}
		break;
		//========== *** �o�� ***
	case UI_MENU_00_STATE_POP: {
	}
		break;
		//========== *** ������ ***
	case UI_MENU_00_STATE_IN_CLEAR: {
		if (--pUiCtl->nCounterState <= 0)
		{// ��Ԃ̃J�E���^�[�����Z��������0�ȉ��̎��A
			// ��Ԃ𖳂��ɂ���
			pUiCtl->state = UI_MENU_00_STATE_NONE;

			// UI:���j���[[00] ��S�Ďg�p���Ă��Ȃ���Ԃɂ���
			for (int nCntUi = 0; nCntUi < UI_MENU_00_MAX; nCntUi++) {
				g_aUi_menu_00[nCntUi].bUse = false;
			}
		}

		float	fRate	// ���� 
				= (float)pUiCtl->nCounterState / (float)UI_MENU_00_CLEAR_TIME;

		// �傫���ɂ�����{�����X�V
		pUiCtl->scale.fWidth =
			(UI_MENU_00_APPEAR_SCALE.fWidth * fRate) +
			(UI_MENU_00_DISAPPEAR_SCALE.fWidth * (1.0f - fRate));
		pUiCtl->scale.fHeight =
			(UI_MENU_00_APPEAR_SCALE.fHeight * fRate) +
			(UI_MENU_00_DISAPPEAR_SCALE.fHeight * (1.0f - fRate));
	}
		break;
	}
}

//========================================
// CreateUi_menu_00�֐� - UI:���j���[[00] �̍쐬���� -
//========================================
void CreateUi_menu_00(void)
{
	Ui_menu_00Control		*pUiCtl	// UI:���j���[[00] �̊Ǘ����̃|�C���^
							= &g_ui_menu_00Control;
	Ui_menu_00Reservation	*pUiRsv	// UI:���j���[[00] �̗\����̃|�C���^
							= &g_ui_menu_00Reservation;

	if ((!pUiRsv->bReservation)
		||
		(pUiCtl->state != UI_MENU_00_STATE_NONE))
	{// ���j���[�̗\�񂪖����A�������͏�Ԃ������łȂ����A
		// �������I������
		return;
	}

	pUiRsv->bReservation	= false;					// �\��t���O���U�ɂ���
	pUiCtl->state			= UI_MENU_00_STATE_IN_POP;	// �o�����ɂ���
	pUiCtl->nCounterState	= UI_MENU_00_APPEAR_TIME;	// ��ԃJ�E���^�[��ݒ�
	pUiCtl->nSelect			= 0;						// ���݂̑I��ԍ�������������
	pUiCtl->nNum			= pUiRsv->nNum;				// ���݂̎g�p����\���񂩂���

	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_menu_00	*pUi	// UI:���j���[[00] �̏��̃|�C���^ 
				= g_aUi_menu_00;

	for (int nCntUi = 0; nCntUi < pUiRsv->nNum; nCntUi++, pVtx += 4, pUi++)
	{
		Ui_menu_00Set	*pSet	// UI:���j���[[00] �̐ݒ���̃|�C���^
						= &pUiRsv->pSet[nCntUi];

		pUi->type		= pSet->type;			// ��ނ���
		pUi->bUse		= true;					// �g�p����Ă����Ԃɂ���
		pUi->bSelect	= false;				// �I������Ă��Ȃ���Ԃɂ���
		sprintf(pUi->aString, pSet->aString);	// ���������

		// �ʒu��ݒ�
		pUi->pos =
			D3DXVECTOR3(
				pUiCtl->centerPos.x - (((pUiRsv->nNum - 1) * UI_MENU_00_SPACE_X) * 0.5f) + (nCntUi * UI_MENU_00_SPACE_X),
				pUiCtl->centerPos.y - (((pUiRsv->nNum - 1) * UI_MENU_00_SPACE_Y) * 0.5f) + (nCntUi * UI_MENU_00_SPACE_Y), 0.0f);

		// ���_���W�̐ݒ�
		SetVertexPos2D(pVtx,
			pUi->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_MENU_00_WIDTH * pUiCtl->scale.fWidth,
			UI_MENU_00_HEIGHT * pUiCtl->scale.fHeight,
			ANGLE_TYPE_FIXED);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_menu_00->Unlock();
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_menu_00�֐� - UI:���j���[[00] �̏��������� -
//========================================
void InitUi_menu_00(void)
{
	InitParameterUi_menu_00Control();		// �Ǘ����̃p�����[�^�[������
	InitParameterUi_menu_00Reservation();	// �\����̃p�����[�^�[������

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�ւ̃|�C���^���擾
						= GetDevice();
	VERTEX_2D			*pVtx;	// ���_���ւ̃|�C���^
	
	for (int nCntType = 0; nCntType < UI_MENU_00_TYPE_MAX; nCntType++) 
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aTexturePath[nCntType], &g_aTextureUi_menu_00[nCntType]);
	}
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_MENU_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_menu_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_menu_00			*pUi	// UI:���j���[[00] �̏��̃|�C���^
						= g_aUi_menu_00;
	Ui_menu_00Control	*pUiCtl	// UI:���j���[[00] �̊Ǘ����̃|�C���^
						= &g_ui_menu_00Control;

	for (int nCntUi = 0; nCntUi < UI_MENU_00_MAX; nCntUi++, pVtx += 4, pUi++)
	{
		InitParameterUi_menu_00(pUi);	// �p�����[�^�[�̏���������

		// ���_���W�̐ݒ�
		SetVertexPos2D(pVtx,
			pUi->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_MENU_00_WIDTH * pUiCtl->scale.fWidth,
			UI_MENU_00_HEIGHT * pUiCtl->scale.fHeight,
			ANGLE_TYPE_FIXED);

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx, 0, 1, 1, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// UninitUi_menu_00�֐� - UI:���j���[[00] �̏I������ -
//========================================
void UninitUi_menu_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < UI_MENU_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureUi_menu_00[nCntType] != NULL)
		{
			g_aTextureUi_menu_00[nCntType]->Release();
			g_aTextureUi_menu_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_menu_00 != NULL) 
	{
		g_pVtxBuffUi_menu_00->Release();
		g_pVtxBuffUi_menu_00 = NULL;
	}
}

//========================================
// UpdateUi_menu_00�֐� - UI:���j���[[00] �̍X�V���� -
//========================================
void UpdateUi_menu_00(void)
{
	StateProcessUi_menu_00();	// ��ԏ���
	CreateUi_menu_00();			// �쐬����

	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_menu_00			*pUi	// UI:���j���[[00] �̏��̃|�C���^
						= g_aUi_menu_00;
	Ui_menu_00Control	*pUiCtl	// UI:���j���[[00] �̊Ǘ����̃|�C���^
						= &g_ui_menu_00Control;

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pVtx += 4, pUi++)
	{
		if (!pUi->bUse)
		{// �g�p����Ă��Ȃ����A
			// �J��Ԃ����������Z�b�g
			continue;
		}

		// ���_���W�̐ݒ�
		SetVertexPos2D(pVtx,
			pUi->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_MENU_00_WIDTH * pUiCtl->scale.fWidth,
			UI_MENU_00_HEIGHT * pUiCtl->scale.fHeight,
			ANGLE_TYPE_FIXED);

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		Color setCol =	// �ݒ�F
			nCntUi_menu_00 == g_ui_menu_00Control.nSelect ?
			UI_MENU_00_SELECT_COLOR :
			Color{ 255,255,255,255 };

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, setCol);

		// �e�L�X�g�̐ݒ�
		SetText2D(
			pUi->aString,
			FONT_002, DISPLAY_CENTER,
			pUi->pos, setCol,
			pUiCtl->scale.fWidth,
			pUiCtl->scale.fHeight);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// Ui_menu_00Input�֐� - UI:���j���[[00] �̓��͏��� -
//========================================
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode)
{
	int					nSelected	// �I�������ԍ�(-1�͑I�𖳂�)
						= -1;
	Ui_menu_00Control	*pUiCtl		// UI:���j���[[00] �̊Ǘ����̃|�C���^
						= &g_ui_menu_00Control;

	if ((pUiCtl->state != UI_MENU_00_STATE_POP)
		||
		(GetFade() != FADE_NONE))
	{// �o����ԂłȂ� or �t�F�[�h���̎��A
		// �������I������
		return nSelected;
	}

	{
		bool	bInput	// ���̓t���O
				= false;

		if (
			(((GetKeyboardRepeat(DIK_UP))
			|| (GetKeyboardRepeat(DIK_W))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
			|| (GetButtonRepeat(BUTTON_UP)))
			&& (mode == UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
			||
			(((GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
			&& (mode == UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT)))
		{// ����͎��A
			g_ui_menu_00Control.nSelect--;	// ���݂̑I��ԍ������Z
			bInput = true;					// ���̓t���O��^�ɂ���
		}
		else if (
			(((GetKeyboardRepeat(DIK_DOWN))
			|| (GetKeyboardRepeat(DIK_S))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
			|| (GetButtonRepeat(BUTTON_DOWN)))
			&& (mode == UI_MENU_00_INPUT_MODE_UP_AND_DOWN))
			||
			(((GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
			&& (mode == UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT)))
		{// ��/�E���͎��A
			g_ui_menu_00Control.nSelect++;	// ���݂̑I��ԍ������Z
			bInput = true;					// ���̓t���O��^�ɂ���
		}

		if ((g_ui_menu_00Reservation.nNum != 1) && (bInput))
		{// ���j���[�̐���1�Ŗ��� & ���̓t���O���^�̎��A
			PlaySound(UI_MENU_00_SELECT_SE);	// �I��SE���Đ�
		}
	}

	// �I��ԍ������[�v����
	IntLoopControl(&pUiCtl->nSelect, pUiCtl->nNum, 0);

	if (g_aUi_menu_00[g_ui_menu_00Control.nSelect].type == UI_MENU_00_TYPE_SELECTION)
	{// ��ނ��I���̎��A
		if (
			(GetKeyboardRepeat(DIK_LEFT))
			|| (GetKeyboardRepeat(DIK_RIGHT))
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// ��/�E���͎��A
			nSelected = g_ui_menu_00Control.nSelect;	// �I�������ԍ��Ɍ��݂̑I��ԍ�����
			PlaySound(UI_MENU_00_SELECT_SE);			// �I��SE���Đ�
		}
	}
	else if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A) || (GetMouseTrigger(MOUSE_LEFT))))
	{// ENTER�L�[ or A�{�^�� or �}�E�X���{�^�������͂��ꂽ���A
		nSelected = g_ui_menu_00Control.nSelect;					// �I�������ԍ��Ɍ��݂̑I��ԍ�����
		g_ui_menu_00Control.state = UI_MENU_00_STATE_IN_CLEAR;		// �������ɂ���
		g_ui_menu_00Control.nCounterState = UI_MENU_00_CLEAR_TIME;	// ��ԃJ�E���^�[��ݒ肷��
		if (g_ui_menu_00Reservation.pSet[nSelected].bDecisionSE)
		{// ����SE�t���O���^�̎��A����SE���Đ�
			PlaySound(UI_MENU_00_DETERMINATION_SE);
		}
	}
	
	// �I�������ԍ���Ԃ�
	return nSelected;
}

//========================================
// DrawUi_menu_00�֐� - UI:���j���[[00] �̕`�揈�� -
//========================================
void DrawUi_menu_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_menu_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Ui_menu_00	*pUi	// UI:���j���[[00] �̏��̃|�C���^
				= g_aUi_menu_00;

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pUi++)
	{
		if (!pUi->bUse)
		{// �g�p����Ă��Ȃ����A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_menu_00[pUi->type]);

		// UI:���j���[[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_menu_00 * 4, 2);
	}
}

//========================================
// SetUi_menu_00Pos�֐� - UI:���j���[[00] �̒��S���W��ݒ� -
//========================================
void SetUi_menu_00Pos(D3DXVECTOR3 pos)
{
	Ui_menu_00Control	*pUiCtl		// UI:���j���[[00] �̊Ǘ����̃|�C���^
						= &g_ui_menu_00Control;

	// ���S���W����
	pUiCtl->centerPos = pos;
}

//========================================
// SetUi_menu_00�֐� - UI:���j���[[00] �̐ݒ菈�� -
//========================================
void SetUi_menu_00(Ui_menu_00Set *pSet, int nNum)
{
	g_ui_menu_00Reservation.bReservation	= true;	// �\��t���O��^�ɂ���
	g_ui_menu_00Reservation.pSet			= pSet;	// �ݒ���̃|�C���^����
	g_ui_menu_00Reservation.nNum			= nNum;	// ���j���[������
}