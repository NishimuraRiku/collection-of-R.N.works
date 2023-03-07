//========================================
// 
// UI:���j���[�R�����g[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_menu-comment_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "md_title_00.h"		// UI:�^�C�g�����		[00]
#include "ui_menu_00.h"			// UI:���j���[			[00]
#include "ui_menu-comment_00.h"	// UI:���j���[�R�����g	[00]

//****************************************
// �}�N����`
//****************************************
// UI:���j���[�R�����g[00] �̕�
#define UI_MENUCOMMENT_00_WIDTH (PIXEL*128)
// UI:���j���[�R�����g[00] �̍���
#define UI_MENUCOMMENT_00_HEIGHT (PIXEL*16)
// UI:���j���[�R�����g[00] �̊J�n���Έʒu
#define UI_MENUCOMMENT_00_START_RELATIVE_POS (D3DXVECTOR3(PIXEL*64,0.0f,0.0f))
// UI:���j���[�R�����g[00] �̏I�����Έʒu
#define UI_MENUCOMMENT_00_END_RELATIVE_POS (D3DXVECTOR3(PIXEL*128,0.0f,0.0f))
// UI:���j���[�R�����g[00] �̃X���C�h�ɂ����鎞��
#define UI_MENUCOMMENT_00_SLIDE_TIME (10)

//****************************************
// �\���̂̒�`
//****************************************
// UI:���j���[�R�����g[00] �̏��\����
typedef struct 
{
	int nCounterSlide;	// �X���C�h�J�E���^�[
	int nSelectTemp;	// �I��ԍ���ۑ�
}Ui_menuComment_00;

//****************************************
// �O���[�o���錾
//****************************************
// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_aTextureUi_menuComment_00[MD_TITLE_00_MAIN_MENU_MAX] = {};
// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_menuComment_00 = NULL;
Ui_menuComment_00 g_ui_menuComment_00;	// UI:���j���[�R�����g[00] �̏��\����

// �e�N�X�`���̑��΃p�X
const char g_aTexturePathUi_menuComment_00[MD_TITLE_00_MAIN_MENU_MAX][TXT_MAX] = {
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\00.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\01.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\02.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\03.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\04.png",
	"data\\TEXTURE\\UserInterface\\Ui_menu-comment_000\\05.png",
};

// �z�u�ԊuX
const float g_aSpaceXUi_menuComment_00[MD_TITLE_00_MAIN_MENU_MAX] = {
	PIXEL*-16,
	PIXEL*-8,
	PIXEL*-24,
	PIXEL*-16,
	PIXEL*-40,
	PIXEL*-32,
};// 12 13 11 12 8 10

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_menuComment_00�֐� - UI:���j���[�R�����g[00] �̏��������� -
//========================================
void InitUi_menuComment_00(void)
{
	// �X���C�h�J�E���^�[��������
	g_ui_menuComment_00.nCounterSlide = 0;
	// �I��ԍ��̕ۑ���������
	g_ui_menuComment_00.nSelectTemp = 0;

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntMenu = 0; nCntMenu < MD_TITLE_00_MAIN_MENU_MAX; nCntMenu++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aTexturePathUi_menuComment_00[nCntMenu], &g_aTextureUi_menuComment_00[nCntMenu]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_menuComment_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menuComment_00->Lock(0, 0, (void**)&pVtx, 0);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, INITCOLOR);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_menuComment_00->Unlock();
}

//========================================
// UninitUi_menuComment_00�֐� - UI:���j���[�R�����g[00] �̏I������ -
//========================================
void UninitUi_menuComment_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMenu = 0; nCntMenu < MD_TITLE_00_MAIN_MENU_MAX; nCntMenu++)
	{
		if (g_aTextureUi_menuComment_00[nCntMenu] != NULL)
		{
			g_aTextureUi_menuComment_00[nCntMenu]->Release();
			g_aTextureUi_menuComment_00[nCntMenu] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_menuComment_00 != NULL) 
	{
		g_pVtxBuffUi_menuComment_00->Release();
		g_pVtxBuffUi_menuComment_00 = NULL;
	}
}

//========================================
// UpdateUi_menuComment_00�֐� - UI:���j���[�R�����g[00] �̍X�V���� -
//========================================
void UpdateUi_menuComment_00(void)
{
	if ((GetMode() != MODE_TITLE_00) || (GetMd_title_00()->state != MD_TITLE_00_STATE_NORMAL))
	{// ���[�h���^�C�g����ʂłȂ� or ��Ԃ��ʏ�łȂ����A
		return;	// �������I������
	}

	// �X���C�h�J�E���^�[�����Z
	g_ui_menuComment_00.nCounterSlide++;

	// �X���C�h�J�E���^�[�̒l�𐧌�
	IntControl(&g_ui_menuComment_00.nCounterSlide, UI_MENUCOMMENT_00_SLIDE_TIME, 0);

	// �I��ԍ����؂�ւ�������A�X���C�h�J�E���^�[��������
	g_ui_menuComment_00.nCounterSlide *= !(GetUi_menu_00Control()->nSelect != g_ui_menuComment_00.nSelectTemp);

	// �I��ԍ���ۑ�
	g_ui_menuComment_00.nSelectTemp = GetUi_menu_00Control()->nSelect;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menuComment_00->Lock(0, 0, (void**)&pVtx, 0);

	float fRate = (float)g_ui_menuComment_00.nCounterSlide / (float)UI_MENUCOMMENT_00_SLIDE_TIME;
	D3DXVECTOR3 addPos =
		(UI_MENUCOMMENT_00_START_RELATIVE_POS * (1.0f - fRate)) +
		(UI_MENUCOMMENT_00_END_RELATIVE_POS * fRate);
	addPos.x += g_aSpaceXUi_menuComment_00[GetUi_menu_00Control()->nSelect];

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		GetUi_menu_00()[GetUi_menu_00Control()->nSelect].pos + addPos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_MENUCOMMENT_00_WIDTH,
		UI_MENUCOMMENT_00_HEIGHT * GetUi_menu_00Control()->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_menuComment_00->Unlock();
}

//========================================
// DrawUi_menuComment_00�֐� - UI:���j���[�R�����g[00] �̕`�揈�� -
//========================================
void DrawUi_menuComment_00(void)
{
	if ((GetMode() != MODE_TITLE_00) || (GetMd_title_00()->state != MD_TITLE_00_STATE_NORMAL))
	{// ���[�h���^�C�g����ʂłȂ� or ��Ԃ��ʏ�łȂ����A
		return;	// �������I������
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_menuComment_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_menuComment_00[GetUi_menu_00Control()->nSelect]);

	// UI:���j���[�R�����g[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}