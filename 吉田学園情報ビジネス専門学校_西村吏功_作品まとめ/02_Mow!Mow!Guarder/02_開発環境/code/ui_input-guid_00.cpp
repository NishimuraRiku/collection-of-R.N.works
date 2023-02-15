//========================================
// 
// UI:���̓K�C�h[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_input-guid_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "color.h"
#include "input.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:�v���C���[			[00]
#include "ui_dialog-box_00.h"	// UI :�_�C�A���O�{�b�N�X	[00]
#include "ui_input-guid_00.h"	// UI :���̓K�C�h			[00]
#include "ui_item-guid_00.h"	// UI :���i�K�C�h			[00]

//****************************************
// �}�N����`
//****************************************
// UI:���̓K�C�h[00] �̈ʒu(�w����)
// UI:���̓K�C�h[00] �̈ʒu(�X�L�b�v��)
// UI:���̓K�C�h[00] �̃e�L�X�g�̑��Έʒu(�w����)
// UI:���̓K�C�h[00] �̃e�L�X�g�̑��Έʒu(�X�L�b�v��)
// UI:���̓K�C�h[00] �̃_�C�A���O�{�b�N�X�Ƃ̑��Έʒu
#define UI_INPUTGUID_00_POS_BUY						(D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*33)+(PIXEL*6*3),(SCREEN_HEIGHT*0.5f)-(PIXEL*14)+(PIXEL*8*4),0.0f))
#define UI_INPUTGUID_00_POS_SKIP					(D3DXVECTOR3((SCREEN_WIDTH*0.5f),(SCREEN_HEIGHT*0.5f)+(PIXEL*12),0.0f))
#define UI_INPUTGUID_00_TEXT_RELATIVE_POS_BUY		(D3DXVECTOR3(PIXEL*-24,0.0f,0.0f))
#define UI_INPUTGUID_00_TEXT_RELATIVE_POS_SKIP		(D3DXVECTOR3(0.0f,PIXEL*-16,0.0f))
#define UI_INPUTGUID_00_RELATIVE_POS_WITH_DIALOGBOX	(D3DXVECTOR3(PIXEL*113,PIXEL*21,0.0f))

// UI:���̓K�C�h[00] �̃A�j���[�V�����ɂ����鎞��
// UI:���̓K�C�h[00] �̃e�L�X�g�̓_�ŃJ�E���^�[
#define UI_INPUTGUID_00_ANIM_TIME			(20)
#define UI_INPUTGUID_00_TEXT_BRINK_COLOR	Color{243,189,63,255}

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:���̓K�C�h[00] �̕��i�ԍ�
typedef enum
{
	UI_INPUTGUID_00_PARTS_BUTTON,	// �{�^��
	UI_INPUTGUID_00_PARTS_MAX,
}UI_INPUTGUID_00_PARTS;

// UI:���̓K�C�h[00] �̃e�N�X�`���ԍ�
typedef enum 
{
	UI_INPUTGUID_00_TEXTURE_BUTTON_ENTER,	// �{�^��(ENTER)
	UI_INPUTGUID_00_TEXTURE_BUTTON_A,		// �{�^��(A)
	UI_INPUTGUID_00_TEXTURE_MAX,
}UI_INPUTGUID_00_TEXTURE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:���̓K�C�h[00] �̕��i���̏��\����
typedef struct
{
	float	fWidth;		// ��
	float	fHeight;	// ����
}Ui_inputGuid_00Parts;

// UI:���̓K�C�h[00] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	int		nPtnWidth;				// �p�^�[����
	int		nPtnHeight;				// �p�^�[������
}Ui_inputGuid_00Texture;

// UI:���̓K�C�h[00] �̊Ǘ����\����
typedef struct
{
	int						nPattern;		// �p�^�[��No.
	int						nCounterAnim;	// �A�j���[�V�����J�E���^�[
	UI_INPUTGUID_00_TEXTURE btnTex;			// �{�^���̃e�N�X�`��
	D3DXVECTOR3				setPos;			// �ݒ�ʒu
}Ui_inputGuid_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���̓K�C�h[00] �̊Ǘ����̃p�����[�^�[��������
void InitParameterUi_inputGuid_00Control(void);

// UI:���̓K�C�h[00] �̕��i���̒��_���W�ݒ菈��
void UpdatePartsUi_inputGuid_00(VERTEX_2D *pVtx, UI_INPUTGUID_00_PARTS parts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_inputGuid_00	// �e�N�X�`���ւ̃|�C���^
						[UI_INPUTGUID_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_inputGuid_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_inputGuid_00Control	g_ui_inputGuid_00Control;	// UI:���̓K�C�h[00] �̊Ǘ����

// UI:���̓K�C�h[00] �̃e�N�X�`�����̏��
const Ui_inputGuid_00Texture g_aUi_inputGuid_00Texture[UI_INPUTGUID_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_button-enter_000.png" , 2, 1 },	// �{�^��(ENTER)
	{ "data\\TEXTURE\\UserInterface\\ui_button-a_000.png"     , 2, 1 },	// �{�^��(A)
};

// UI:���̓K�C�h[00] �̕��i���̏��
const Ui_inputGuid_00Parts g_aUi_inputGuid_00Parts[UI_INPUTGUID_00_PARTS_MAX] =
{
	{ PIXEL * 16, PIXEL * 16 },	// �{�^��
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_inputGuid_00�֐� - UI:���̓K�C�h[00] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void UpdatePartsUi_inputGuid_00(VERTEX_2D *pVtx, UI_INPUTGUID_00_PARTS parts) 
{
	Ui_inputGuid_00Control	*pUiCtl	// UI:���̓K�C�h[00] �̊Ǘ����̃|�C���^
							= &g_ui_inputGuid_00Control;

	switch (parts)
	{
	case /*/ �{�^�� /*/UI_INPUTGUID_00_PARTS_BUTTON: {
		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			pUiCtl->setPos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_inputGuid_00Parts[parts].fWidth,
			g_aUi_inputGuid_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			Count(
				&pUiCtl->nPattern,
				0,
				(g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnWidth * g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnHeight) - 1,
				&pUiCtl->nCounterAnim,
				UI_INPUTGUID_00_ANIM_TIME,
				COUNT_TYPE_NORMAL),
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnWidth,
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnHeight,
			NULL);
	}
	break;
	}
}

//========================================
// InitParameterUi_inputGuid_00Control�֐� - UI:���̓K�C�h[00] �̊Ǘ����̃p�����[�^�[�������� -
//========================================
void InitParameterUi_inputGuid_00Control(void) 
{
	Ui_inputGuid_00Control	*pUiCtl	// UI:���̓K�C�h[00] �̊Ǘ����̃|�C���^
							= &g_ui_inputGuid_00Control;

	pUiCtl->nPattern		= 0;									// �p�^�[��No.
	pUiCtl->nCounterAnim	= 0;									// �A�j���[�V�����J�E���^�[
	pUiCtl->btnTex			= UI_INPUTGUID_00_TEXTURE_BUTTON_ENTER;	// �{�^���̃e�N�X�`��
	pUiCtl->setPos			= INITD3DXVECTOR3;						// �ݒ�ʒu
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_inputGuid_00�֐� - UI:���̓K�C�h[00] �̏��������� -
//========================================
void InitUi_inputGuid_00(void)
{
	// �Ǘ����̃p�����[�^�[�̏�����
	InitParameterUi_inputGuid_00Control();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_INPUTGUID_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_inputGuid_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_inputGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < UI_INPUTGUID_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_inputGuid_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_inputGuid_00[nCntTexture]);
	}

	Ui_inputGuid_00Control	*pUiCtl	// UI:���̓K�C�h[00] �̊Ǘ����̃|�C���^
							= &g_ui_inputGuid_00Control;

	for (int nCntParts = 0; nCntParts < UI_INPUTGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			0,
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnWidth,
			g_aUi_inputGuid_00Texture[pUiCtl->btnTex].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_inputGuid_00->Unlock();
}

//========================================
// UninitUi_inputGuid_00�֐� - UI:���̓K�C�h[00] �̏I������ -
//========================================
void UninitUi_inputGuid_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < UI_INPUTGUID_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_inputGuid_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_inputGuid_00[nCntTexture]->Release();
			g_aTextureUi_inputGuid_00[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_inputGuid_00 != NULL) 
	{
		g_pVtxBuffUi_inputGuid_00->Release();
		g_pVtxBuffUi_inputGuid_00 = NULL;
	}
}

//========================================
// UpdateUi_inputGuid_00�֐� - UI:���̓K�C�h[00] �̍X�V���� -
//========================================
void UpdateUi_inputGuid_00(void) 
{
	Ui_inputGuid_00Control	*pUiCtl	// UI:���̓K�C�h[00] �̊Ǘ����̃|�C���^
							= &g_ui_inputGuid_00Control;

	if (GetActiveInputType() == ACTIVE_INPUT_TYPE_KEYBOARD) 
	{// �L�[�{�[�h�����I�Ȏ��A�e�N�X�`�����{�^��(ENTER)�ɐݒ�
		pUiCtl->btnTex = UI_INPUTGUID_00_TEXTURE_BUTTON_ENTER;
	}
	else if (GetActiveInputType() == ACTIVE_INPUT_TYPE_CONTROLLER)
	{// �R���g���[���[�����I�Ȏ��A�e�N�X�`�����{�^��(A)�ɐݒ�
		pUiCtl->btnTex = UI_INPUTGUID_00_TEXTURE_BUTTON_A;
	}

	float	fAlpha;	// �s�����x

	if (GetUi_dialogBox_00()->state == UI_DIALOGBOX_00_STATE_POP) 
	{// UI:�_�C�A���O�{�b�N�X[00] ���o�����Ă����Ԃ̎��A
		fAlpha = 1.0f;	// �s�����x��ݒ�

		// �ݒ�ʒu��UI:�_�C�A���O�{�b�N�X[00]�Ƃ̑��Έʒu�Őݒ�
		pUiCtl->setPos = UI_DIALOGBOX_00_POS + UI_INPUTGUID_00_RELATIVE_POS_WITH_DIALOGBOX;
	}
	else if (GetChr_player_00()->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00].bHit) 
	{// CHR:�v���C���[[00] ��OBJ:�X�C�b�`[00] �Ƃ̓�����t���O���^�̎��A
		fAlpha = 1.0f;	// �s�����x��ݒ�

		// �ݒ�ʒu���X�L�b�v���̂��̂ɐݒ�
		pUiCtl->setPos = UI_INPUTGUID_00_POS_SKIP;

		// �e�L�X�g�̐ݒ�
		SetText2D(
			"ARE YOU READY?",
			FONT_002, DISPLAY_CENTER,
			UI_INPUTGUID_00_POS_SKIP + UI_INPUTGUID_00_TEXT_RELATIVE_POS_SKIP,
			!pUiCtl->nPattern ?
			Color{ 255,255,255,(int)(255 * fAlpha) } :
			ColorMix(UI_INPUTGUID_00_TEXT_BRINK_COLOR, { 255,255,255,(int)(255 * fAlpha) }), 1.0f, 1.0f);
	}
	else
	{// CHR:�v���C���[[00] ��OBJ:�X�C�b�`[00] �Ƃ̓�����t���O���U & OBJ:���[00] �Ƃ̎��A
		// UI:���i�K�C�h�̓����J�E���^�[�ɉ����ĕs�����x��ݒ�
		fAlpha = (float)GetUi_itemGuid_00()->nCounterArrival / (float)UI_ITEMGUID_00_ARRIVAL_TIME;

		// �ݒ�ʒu���w�����̂��̂ɐݒ�
		pUiCtl->setPos = UI_INPUTGUID_00_POS_BUY;

		// �e�L�X�g�̐ݒ�
		SetText2D(
			"BUY?",
			FONT_002, DISPLAY_CENTER,
			UI_INPUTGUID_00_POS_BUY + UI_INPUTGUID_00_TEXT_RELATIVE_POS_BUY,
			!pUiCtl->nPattern ?
			Color{ 255,255,255,(int)(255 * fAlpha) } :
			ColorMix(UI_INPUTGUID_00_TEXT_BRINK_COLOR, { 255,255,255,(int)(255 * fAlpha) }), 1.0f, 1.0f);
	}

	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_inputGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_INPUTGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * fAlpha) });

		// ���i���̍X�V����
		UpdatePartsUi_inputGuid_00(pVtx, (UI_INPUTGUID_00_PARTS)nCntParts);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_inputGuid_00->Unlock();
}

//========================================
// DrawUi_inputGuid_00�֐� - UI:���̓K�C�h[00] �̕`�揈�� -
//========================================
void DrawUi_inputGuid_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_inputGuid_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Ui_inputGuid_00Control	*pUiCtl	// UI:���̓K�C�h[00] �̊Ǘ����̃|�C���^
							= &g_ui_inputGuid_00Control;

	for (int nCntParts = 0; nCntParts < UI_INPUTGUID_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_inputGuid_00[pUiCtl->btnTex]);

		// UI:���̓K�C�h[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}