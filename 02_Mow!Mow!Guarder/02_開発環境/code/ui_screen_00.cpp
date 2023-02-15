//========================================
// 
// UI:�X�N���[��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_screen_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "ui_screen_00.h"	// UI:�X�N���[��	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�X�N���[��[00] �̃e�N�X�`���t�@�C���̑��΃p�X
// UI:�X�N���[��[00] �̕�
// UI:�X�N���[��[00] �̍���
#define UI_SCREEN_00_TEXTURE_PATH	"data\\TEXTURE\\UserInterface\\ui_screen_000.png"
#define UI_SCREEN_00_WIDTH			(1280)
#define UI_SCREEN_00_HEIGHT			(720)

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_screen_00	// �e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_screen_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_screen_00�֐� - UI:�X�N���[��[00] �̏��������� -
//========================================
void InitUi_screen_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, UI_SCREEN_00_TEXTURE_PATH, &g_aTextureUi_screen_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_screen_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_screen_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_SCREEN_00_WIDTH,
		UI_SCREEN_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, INITCOLOR);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_screen_00->Unlock();
}

//========================================
// UninitUi_screen_00�֐� - UI:�X�N���[��[00] �̏I������ -
//========================================
void UninitUi_screen_00(void)
{
	// �e�N�X�`���̔j��
	if (g_aTextureUi_screen_00 != NULL)
	{
		g_aTextureUi_screen_00->Release();
		g_aTextureUi_screen_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_screen_00 != NULL) 
	{
		g_pVtxBuffUi_screen_00->Release();
		g_pVtxBuffUi_screen_00 = NULL;
	}
}

//========================================
// UpdateUi_screen_00�֐� - UI:�X�N���[��[00] �̍X�V���� -
//========================================
void UpdateUi_screen_00(void)
{
	
}

//========================================
// DrawUi_screen_00�֐� - UI:�X�N���[��[00] �̕`�揈�� -
//========================================
void DrawUi_screen_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_screen_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_screen_00);

	// UI:�X�N���[��[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}