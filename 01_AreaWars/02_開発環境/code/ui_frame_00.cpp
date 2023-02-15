//========================================
// 
// �t���[��[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_frame_00.cpp ***
//========================================
#include "main.h"
#include "polygon.h"
#include "ui_frame_00.h"

//****************************************
// �}�N����`
//****************************************

// �t���[��[00] �̃e�N�X�`���t�@�C���̑��΃p�X
// �t���[��[00] �̕�
// �t���[��[00] �̍���
// �t���[��[00] �̈ʒu
#define UI_FRAME_00_TEXTURE_PATH	"data\\TEXTURE\\ui_frame_000.png"
#define UI_FRAME_00_WIDTH			(SCREEN_WIDTH)
#define UI_FRAME_00_HEIGHT			(SCREEN_HEIGHT)
#define UI_FRAME_00_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f))

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_frame_00	//
						= NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_frame_00	//
						= NULL;					// ���_�o�b�t�@�ւ̃|�C���^

//========================================
// InitUi_frame_00�֐� - �t���[��[00] �̏��������� -
//========================================
void InitUi_frame_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

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
	SetVertexPosition(pVtx,
		UI_FRAME_00_POS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_FRAME_00_WIDTH,
		UI_FRAME_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor(pVtx, { 255,255,255,255 });

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePosition(pVtx, 0, 1, 1, NULL);

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_frame_00->Unlock();
}

//========================================
// UninitUi_frame_00�֐� - �t���[��[00] �̏I������ -
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
// DrawUi_frame_00�֐� - �t���[��[00] �̕`�揈�� -
//========================================
void DrawUi_frame_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_frame_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_frame_00);

	// �t���[��[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}