//========================================
// 
// UI:�C���x���g��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_inventory_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "main.h"
#include "input.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "ui_inventory_00.h"	// UI :�C���x���g��	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�C���x���g��[00] �̕�
// UI:�C���x���g��[00] �̍���
// UI:�C���x���g��[00] �̔z�u�ԊuX
// UI:�C���x���g��[00] �̊�ʒu
#define UI_INVENTORY_00_WIDTH		(PIXEL * 32)
#define UI_INVENTORY_00_HEIGHT		(PIXEL * 32)
#define UI_INVENTORY_00_SPACE_X		(PIXEL * 32)
#define UI_INVENTORY_00_BASE_POS	(D3DXVECTOR3(BUFFER_WIDTH * 0.5f,BUFFER_HEIGHT - (PIXEL*21),0.0f))

// UI:�C���x���g��[00] �̑I�����̐F
// UI:�C���x���g��[00] �̔�I�����̐F
#define UI_INVENTORY_00_SELECTED_COLOR		{255,255,255,255}
#define UI_INVENTORY_00_UNSELECTED_COLOR	{100,100,100,200}

// UI:�C���x���g��[00] �̃J�E���g�e�L�X�g�̑��Έʒu
// UI:�C���x���g��[00] �̃J�E���g�e�L�X�g�̑I�����̐F
// UI:�C���x���g��[00] �̃J�E���g�e�L�X�g�̔�I�����̐F
#define UI_INVENTORY_00_COUNT_TEXT_RELATIVE_POS			(D3DXVECTOR3(0.0f,PIXEL*12,0.0f))
#define UI_INVENTORY_00_COUNT_TEXT_SELECTED_COLOR		{255,255,255,255}
#define UI_INVENTORY_00_COUNT_TEXT_UNSELECTED_COLOR		{100,100,100,255}

// UI:�C���x���g��[00] �̑I�����̈ړ��ɂ����鎞��
// UI:�C���x���g��[00] �̑I�����̈ړ���Y
#define UI_INVENTORY_00_SELECTED_MOVE_TIME	(5)
#define UI_INVENTORY_00_SELECTED_MOVE_Y		(PIXEL*-4)

// UI:�C���x���g��[00] �̃A�C�e�����e�L�X�g�̑��Έʒu
#define UI_INVENTORY_00_ITEMNAME_TEXT_RELATIVE_POS	D3DXVECTOR3(0.0f,PIXEL * -25,0.0f)

// UI:�C���x���g��[00] �̍��I���e�L�X�g�̑��Έʒu
// UI:�C���x���g��[00] �̉E�I���e�L�X�g�̑��Έʒu
// UI:�C���x���g��[00] �̓��͌��ʃe�L�X�g�̑��Έʒu
#define UI_INVENTORY_00_LEFTSELECT_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL * -42,PIXEL * -25,0.0f)
#define UI_INVENTORY_00_RIGHTSELECT_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL * 42,PIXEL * -25,0.0f)
#define UI_INVENTORY_00_INPUTRESULT_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL * 30,PIXEL *6,0.0f)

//****************************************
// �\���̂̒�`
//****************************************
// UI:�C���x���g��[00] �̏��\����
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu

	// ��Ԋ֘A
	int	nCounterSelect;	// �I���J�E���^�[
}Ui_inventory_00;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_inventory_00	// �e�N�X�`���ւ̃|�C���^
						[CHR_PLAYER_00_ITEM_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_inventory_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_inventory_00			g_aUi_inventory_00			// UI:�C���x���g��[00] �̏��
						[CHR_PLAYER_00_ITEM_MAX];

// UI:�C���x���g��[00] �̃e�N�X�`���̑��΃p�X
const char g_aTexturePathUi_inventory_00[CHR_PLAYER_00_ITEM_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\sword.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\gun.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\block.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\turret.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\reflector.png",
	"data\\TEXTURE\\UserInterface\\ui_inventory_000\\discharger.png",
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_inventory_00�֐� - UI:�C���x���g��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_inventory_00(Ui_inventory_00 *pUi)
{
	pUi->nCounterSelect = 0;	// �I���J�E���^�[
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_inventory_00�֐� - UI:�C���x���g��[00] �̏��������� -
//========================================
void InitUi_inventory_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X���擾
						= GetDevice();
	VERTEX_2D			*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_PLAYER_00_ITEM_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_inventory_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_inventory_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < CHR_PLAYER_00_ITEM_MAX; nCntUi++, pVtx += 4)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aTexturePathUi_inventory_00[nCntUi], &g_aTextureUi_inventory_00[nCntUi]);

		// ���_���W��ݒ�
		{
			D3DXVECTOR3 setPos = UI_INVENTORY_00_BASE_POS;	// �ݒ�ʒu��ݒ�
			setPos.x = setPos.x - (CHR_PLAYER_00_ITEM_MAX * UI_INVENTORY_00_SPACE_X * 0.5f) + (((float)nCntUi + 0.5f) * UI_INVENTORY_00_SPACE_X);
			SetVertexPos2D(pVtx,
				setPos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				UI_INVENTORY_00_WIDTH,
				UI_INVENTORY_00_HEIGHT,
				ANGLE_TYPE_FIXED);
		}

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			0,
			1,
			1,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_inventory_00->Unlock();
}

//========================================
// UninitUi_inventory_00�֐� - UI:�C���x���g��[00] �̏I������ -
//========================================
void UninitUi_inventory_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntUi = 0; nCntUi < CHR_PLAYER_00_ITEM_MAX; nCntUi++)
	{
		if (g_aTextureUi_inventory_00[nCntUi] != NULL)
		{
			g_aTextureUi_inventory_00[nCntUi]->Release();
			g_aTextureUi_inventory_00[nCntUi] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_inventory_00 != NULL)
	{
		g_pVtxBuffUi_inventory_00->Release();
		g_pVtxBuffUi_inventory_00 = NULL;
	}
}

//========================================
// UpdateUi_inventory_00�֐� - UI:�C���x���g��[00] �̍X�V���� -
//========================================
void UpdateUi_inventory_00(void)
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_inventory_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_inventory_00	*pUi	// UI:�C���x���g��[00] �̏��̃|�C���^
					= g_aUi_inventory_00;

	for (int nCntItem = 0; nCntItem < CHR_PLAYER_00_ITEM_MAX; nCntItem++, pVtx += 4, pUi++)
	{
		// �I���A�C�e���ԍ��ƈ�v���Ă��邩�ɉ����đI���J�E���^�[�����Zor���Z
		nCntItem == GetChr_player_00()->nSelectItem ?
			pUi->nCounterSelect++ :
			pUi->nCounterSelect--;

		// �I���J�E���^�[�̐���
		IntControl(&pUi->nCounterSelect, UI_INVENTORY_00_SELECTED_MOVE_TIME, 0);

		// ���_���W��ݒ�
		{
			D3DXVECTOR3 setPos = UI_INVENTORY_00_BASE_POS;	// �ݒ�ʒu��ݒ�
			setPos.x = setPos.x - (CHR_PLAYER_00_ITEM_MAX * UI_INVENTORY_00_SPACE_X * 0.5f) + (((float)nCntItem + 0.5f) * UI_INVENTORY_00_SPACE_X);
			setPos.y += UI_INVENTORY_00_SELECTED_MOVE_Y * ((float)pUi->nCounterSelect / (float)(UI_INVENTORY_00_SELECTED_MOVE_TIME));
			SetVertexPos2D(pVtx,
				setPos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				UI_INVENTORY_00_WIDTH,
				UI_INVENTORY_00_HEIGHT,
				ANGLE_TYPE_FIXED);

			if(GetChr_player_00Item()[nCntItem].bCount)
			{// �A�C�e���̃J�E���g�t���O���^�̎��A
				// �J�E���g�e�L�X�g�\��
				char aString[TXT_MAX];
				sprintf(aString, "*%02d", GetChr_player_00()->aItemCount[nCntItem]);
				Color SetColor;
				nCntItem == GetChr_player_00()->nSelectItem ?
					SetColor = UI_INVENTORY_00_COUNT_TEXT_SELECTED_COLOR :
					SetColor = UI_INVENTORY_00_COUNT_TEXT_UNSELECTED_COLOR;
				SetText2D(aString,
					FONT_002,
					DISPLAY_LEFT,
					setPos + UI_INVENTORY_00_COUNT_TEXT_RELATIVE_POS,
					SetColor,
					1.0f, 1.0f);
			}
		}

		// ���_�J���[�̐ݒ�
		{
			Color setColor;	// �ݒ�F��I���A�C�e���ԍ��ƈ�v���Ă��邩�ɉ����Đݒ�
			nCntItem == GetChr_player_00()->nSelectItem ?
				setColor = UI_INVENTORY_00_SELECTED_COLOR :
				setColor = UI_INVENTORY_00_UNSELECTED_COLOR;

			SetVertexColor2D(pVtx, setColor);
		}

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			0,
			1,
			1,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_inventory_00->Unlock();

	// �A�C�e�����e�L�X�g�\��
	{
		char aString[TXT_MAX];
		sprintf(aString, GetChr_player_00Item()[GetChr_player_00()->nSelectItem].aName);
		Color SetColor = { 255,255,255,255 };
		SetText2D(
			aString,
			FONT_002,
			DISPLAY_CENTER,
			UI_INVENTORY_00_BASE_POS + UI_INVENTORY_00_ITEMNAME_TEXT_RELATIVE_POS,
			SetColor,
			1.0f, 1.0f);
	}

	// ���I���e�L�X�g�\��
	{
		char aString[TXT_MAX];
		if (GetActiveInputType() == ACTIVE_INPUT_TYPE_KEYBOARD)
		{// ���I�ȓ��͂̎�ނ��L�[�{�[�h�̎��A
			sprintf(aString, "<[Q]");
		}
		else if (GetActiveInputType() == ACTIVE_INPUT_TYPE_CONTROLLER)
		{// ���I�ȓ��͂̎�ނ��R���g���[���[�̎��A
			sprintf(aString, "<[LB]");
		}
		Color SetColor = { 255,255,255,255 };
		SetText2D(
			aString,
			FONT_002,
			DISPLAY_RIGHT,
			UI_INVENTORY_00_BASE_POS + UI_INVENTORY_00_LEFTSELECT_TEXT_RELATIVE_POS,
			SetColor,
			1.0f, 1.0f);
	}

	// �E�I���e�L�X�g�\��
	{
		char aString[TXT_MAX];
		if (GetActiveInputType() == ACTIVE_INPUT_TYPE_KEYBOARD)
		{// ���I�ȓ��͂̎�ނ��L�[�{�[�h�̎��A
			sprintf(aString, "[E]>");
		}
		else if (GetActiveInputType() == ACTIVE_INPUT_TYPE_CONTROLLER)
		{// ���I�ȓ��͂̎�ނ��R���g���[���[�̎��A
			sprintf(aString, "[RB]>");
		}
		Color SetColor = { 255,255,255,255 };
		SetText2D(
			aString,
			FONT_002,
			DISPLAY_LEFT,
			UI_INVENTORY_00_BASE_POS + UI_INVENTORY_00_RIGHTSELECT_TEXT_RELATIVE_POS,
			SetColor,
			1.0f, 1.0f);
	}
}

//========================================
// DrawUi_inventory_00�֐� - UI:�C���x���g��[00] �̕`�揈�� -
//========================================
void DrawUi_inventory_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X���擾
						= GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_inventory_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUi = 0; nCntUi < CHR_PLAYER_00_ITEM_MAX; nCntUi++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_inventory_00[nCntUi]);

		// UI:�C���x���g��[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);
	}
}