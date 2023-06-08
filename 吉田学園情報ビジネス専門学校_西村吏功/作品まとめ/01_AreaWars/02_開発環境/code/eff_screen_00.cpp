//========================================
// 
// �X�N���[��[00] �̏���
// Author:���� ����
// 
//========================================
//  *** eff_screen_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon.h"
#include "eff_screen_00.h"
#include <limits.h>

//****************************************
// �}�N����`
//****************************************

// �X�N���[��[00] �̃e�N�X�`���t�@�C���̑��΃p�X
// �X�N���[��[00] �̕�
// �X�N���[��[00] �̍���
#define EFF_SCREEN_00_TEXTURE_PATH	"data\\TEXTURE\\eff_screen_000_960x720.png"
#define EFF_SCREEN_00_WIDTH			(960)
#define EFF_SCREEN_00_HEIGHT		(720)

//****************************************
// �\���̂̒�`
//****************************************

// �X�N���[��[00] �̏��\����
typedef struct 
{
	Color	col;				// �F
	int		nCounterAlpha;		// �����x�J�E���^�[
	int		nCounterAlphaMax;	// �����x�J�E���^�[�̍ő�l(�J�n�l)
}Eff_screen_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterEff_screen_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_screen_00 //
						= NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_screen_00	//
						= NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Eff_screen_00			g_eff_screen_00;		// �X�N���[��[00] �̏��\����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_screen_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterEff_screen_00(void) 
{
	g_eff_screen_00.col					= { 0,0,0,0 };	// �F
	g_eff_screen_00.nCounterAlpha		= 0;			// �����x�J�E���^�[
	g_eff_screen_00.nCounterAlphaMax	= 0;			// �����x�J�E���^�[�̍ő�l(�J�n�l)
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitEff_screen_00�֐� - �X�N���[��[00] �̏��������� -
//========================================
void InitEff_screen_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterEff_screen_00();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EFF_SCREEN_00_TEXTURE_PATH, &g_aTextureEff_screen_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_screen_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_screen_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	SetVertexPosition(pVtx,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		EFF_SCREEN_00_WIDTH,
		EFF_SCREEN_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor(pVtx, g_eff_screen_00.col);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePosition(pVtx, 0, 1, 1, NULL);

	// ���_���W���A�����b�N����
	g_pVtxBuffEff_screen_00->Unlock();
}

//========================================
// UninitEff_screen_00�֐� - �X�N���[��[00] �̏I������ -
//========================================
void UninitEff_screen_00(void)
{
	// �e�N�X�`���̔j��
	if (g_aTextureEff_screen_00 != NULL)
	{
		g_aTextureEff_screen_00->Release();
		g_aTextureEff_screen_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEff_screen_00 != NULL) 
	{
		g_pVtxBuffEff_screen_00->Release();
		g_pVtxBuffEff_screen_00 = NULL;
	}
}

//========================================
// UpdateEff_screen_00�֐� - �X�N���[��[00] �̍X�V���� -
//========================================
void UpdateEff_screen_00(void)
{
	Eff_screen_00	*pEff	// �X�N���[��[00] �̏��̃|�C���^
					= &g_eff_screen_00;

	if (pEff->nCounterAlpha > 0)
	{// �����x�J�E���^�[��0�������Ă��鎞�A
		// �����x�J�E���^�[�����Z
		pEff->nCounterAlpha--;

		// �����x��ݒ�
		pEff->col.nA = 255 * ((float)pEff->nCounterAlpha / (float)pEff->nCounterAlphaMax);

		VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffEff_screen_00->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, g_eff_screen_00.col);

		// ���_���W���A�����b�N����
		g_pVtxBuffEff_screen_00->Unlock();
	}
}

//========================================
// DrawEff_screen_00�֐� - �X�N���[��[00] �̕`�揈�� -
//========================================
void DrawEff_screen_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEff_screen_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureEff_screen_00);

	// �X�N���[��[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetEff_screen_00�֐� - �X�N���[��[00] �̐ݒ菈�� -
//========================================
void SetEff_screen_00(int nTime, Color col) 
{
	// �F����
	g_eff_screen_00.col = col;

	// �����x�J�E���^�[�Ɏ��Ԃ���
	g_eff_screen_00.nCounterAlpha = nTime;
	g_eff_screen_00.nCounterAlphaMax = nTime;

	// �����x�J�E���^�[�̍ő�l�𐧌�
	IntControl(&g_eff_screen_00.nCounterAlphaMax, INT_MAX, 1);
}