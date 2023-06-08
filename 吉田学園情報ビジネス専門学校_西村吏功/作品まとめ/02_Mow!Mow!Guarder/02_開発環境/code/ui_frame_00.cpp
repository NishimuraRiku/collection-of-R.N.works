//========================================
// 
// UI:�t���[��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_frame_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "ui_frame_00.h"	// UI:�t���[��	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�t���[��[00] �̃e�N�X�`���t�@�C���̑��΃p�X
#define UI_FRAME_00_TEXTURE_PATH "data\\TEXTURE\\UserInterface\\ui_frame_000.png"
// UI:�t���[��[00] �̕�
#define UI_FRAME_00_WIDTH (PIXEL*128)
// UI:�t���[��[00] �̍���
#define UI_FRAME_00_HEIGHT (PIXEL*16)
// UI:�t���[��[00] �̈ʒu
#define UI_FRAME_00_POS D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y,0.0f)

//****************************************
// �\���̂̒�`
//****************************************
// UI:�t���[��[00] �̏��\����
typedef struct 
{
	bool bShow;	// �\���t���O
}Ui_frame_00;

//****************************************
// �O���[�o���錾
//****************************************
// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_aTextureUi_frame_00 = NULL;
// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_frame_00 = NULL;
Ui_frame_00 g_ui_frame_00;	// UI:�t���[��[00] �̏��\����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_frame_00�֐� - UI:�t���[��[00] �̏��������� -
//========================================
void InitUi_frame_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �\���t���O��������
	g_ui_frame_00.bShow = false;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, UI_FRAME_00_TEXTURE_PATH, &g_aTextureUi_frame_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_frame_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_frame_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		UI_FRAME_00_POS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_FRAME_00_WIDTH,
		UI_FRAME_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, INITCOLOR);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_frame_00->Unlock();
}

//========================================
// UninitUi_frame_00�֐� - UI:�t���[��[00] �̏I������ -
//========================================
void UninitUi_frame_00(void)
{
	// �e�N�X�`���̔j��
	if (g_aTextureUi_frame_00 != NULL)
	{
		g_aTextureUi_frame_00->Release();
		g_aTextureUi_frame_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_frame_00 != NULL) 
	{
		g_pVtxBuffUi_frame_00->Release();
		g_pVtxBuffUi_frame_00 = NULL;
	}
}

//========================================
// UpdateUi_frame_00�֐� - UI:�t���[��[00] �̍X�V���� -
//========================================
void UpdateUi_frame_00(void)
{
	// �\���t���O��������
	g_ui_frame_00.bShow = false;
}

//========================================
// DrawUi_frame_00�֐� - UI:�t���[��[00] �̕`�揈�� -
//========================================
void DrawUi_frame_00(void)
{
	if (!g_ui_frame_00.bShow) 
	{// �\���t���O���U�̎��A
		return;	// �������I������
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_frame_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_frame_00);

	// UI:�t���[��[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetUi_frame_00�֐� - UI:�t���[��[00] �̐ݒ菈�� -
//========================================
void SetUi_frame_00(void) 
{
	g_ui_frame_00.bShow = true;
}