//========================================
// 
// �{��[00]�̏���
// Author:���� ����
// 
//========================================
// *** ui_bomb_00.cpp ***
//========================================
#include "main.h"
#include "chr_fighter_00.h"
#include "ui_bomb_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// �}�N����`
//****************************************
#define UI_BOMB_00_MAX						(64)											// �{��[00] �̍ő�\����
#define UI_BOMB_00_START_POS				(D3DXVECTOR3(PIXEL * 8 * 32, PIXEL * 20, 0.0f))	// �{��[00] �̊J�n�ʒu
#define UI_BOMB_00_TEXTURE_X_PATTERN_MAX	(1)												// �{��[00] �̃e�N�X�`���̃p�^�[����(��)
#define UI_BOMB_00_TEXTURE_Y_PATTERN_MAX	(1)												// �{��[00] �̃e�N�X�`���̃p�^�[����(�c)

//****************************************
// �\���̂̒�`
//****************************************

// �{��[00]�\���̂̒�`
typedef struct
{
							//========== *** �ʒu�֘A ***
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	float fWidth;			// ��
	float fHeight;			// ����
							//========== *** �e�N�X�`���֘A ***
	int nCounterAnim;		// �A�j���[�V�����̃J�E���^�[
	int nAnimChangeTime;	// �A�j���[�V�����̐؂�ւ�����
	int nPattern;			// �p�^�[��No.
}Ui_bomb_00;

// �{��[00] �̎�ޖ��̏��
typedef struct
{
	char aTexturPath[TXT_MAX];	// �e�N�X�`���̑��΃p�X
	int nPtnMaxX;				// �p�^�[���̍ő吔(��)
	int nPtnMaxY;				// �p�^�[���̍ő吔(�c)
	float fWidth;				// ��
	float fHeight;				// ����
	Color col;
}Ui_bomb_00Type;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureUi_bomb_00 = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_bomb_00 = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Ui_bomb_00 g_aUi_bomb_00[UI_BOMB_00_MAX];				// �{��[00] �̏��

// �e�N�X�`���̑��΃p�X
Ui_bomb_00Type g_ui_bomb_00Type =
{
	"data\\TEXTURE\\ui_bomb_000_02.png", 1, 1, PIXEL * 8	, PIXEL * 8, {255,255,255,255}
};

//========================================
// InitUi_bomb_00�֐� - �{��[00]�̏��������� -
//========================================
void InitUi_bomb_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, g_ui_bomb_00Type.aTexturPath, &g_pTextureUi_bomb_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_BOMB_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_bomb_00,
		NULL);

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffUi_bomb_00->Lock(0, 0, (void**)&pVtx, 0);

	float fX = UI_BOMB_00_START_POS.x;	// X���W

	for (int nCntUi_bomb_00 = 0; nCntUi_bomb_00 < UI_BOMB_00_MAX; nCntUi_bomb_00++, pVtx += 4)
	{
		{
			// ���ƍ�����ݒ�
			g_aUi_bomb_00[nCntUi_bomb_00].fWidth	= g_ui_bomb_00Type.fWidth;
			g_aUi_bomb_00[nCntUi_bomb_00].fHeight	= g_ui_bomb_00Type.fHeight;

			D3DXVECTOR3 pos = g_aUi_bomb_00[nCntUi_bomb_00].pos;						// �ʒu
			D3DXVECTOR3 rot = g_aUi_bomb_00[nCntUi_bomb_00].rot;						// ����
			float fWidth	= g_aUi_bomb_00[nCntUi_bomb_00].fWidth;						// ��
			float fHeight	= g_aUi_bomb_00[nCntUi_bomb_00].fHeight;					// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// X���W���X�V
			fX += fWidth * 0.5f;

			// �ʒu��ݒ�
			g_aUi_bomb_00[nCntUi_bomb_00].pos = UI_BOMB_00_START_POS;
			g_aUi_bomb_00[nCntUi_bomb_00].pos.x = fX;

			// X���W���X�V
			fX += fWidth * 0.5f;

			// ���_���W ��ݒ�
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);
		pVtx[1].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);
		pVtx[2].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);
		pVtx[3].col = D3DCOLOR_RGBA(g_ui_bomb_00Type.col.nR, g_ui_bomb_00Type.col.nG, g_ui_bomb_00Type.col.nB, g_ui_bomb_00Type.col.nA);

		{
			int nPtn		= g_aUi_bomb_00[nCntUi_bomb_00].nPattern;	// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX	= g_ui_bomb_00Type.nPtnMaxX;				// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY	= g_ui_bomb_00Type.nPtnMaxY;				// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}
	
	// ���_���W ���A�����b�N����
	g_pVtxBuffUi_bomb_00->Unlock();
}

//========================================
// UninitUi_bomb_00�֐� - �{��[00]�̏I������ -
//========================================
void UninitUi_bomb_00(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureUi_bomb_00 != NULL)
	{
		g_pTextureUi_bomb_00->Release();
		g_pTextureUi_bomb_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_bomb_00 != NULL) 
	{
		g_pVtxBuffUi_bomb_00->Release();
		g_pVtxBuffUi_bomb_00 = NULL;
	}
}

//========================================
// UpdateUi_bomb_00�֐� - �{��[00]�̍X�V���� -
//========================================
void UpdateUi_bomb_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	POINT vertexPos[4];			// �l�p�`�̒��_�̍��W
	
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffUi_bomb_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_bomb_00 = 0; nCntUi_bomb_00 < UI_BOMB_00_MAX; nCntUi_bomb_00++, pVtx += 4)
	{
		D3DXVECTOR3 pos		= g_aUi_bomb_00[nCntUi_bomb_00].pos;		// �ʒu
		D3DXVECTOR3 rot		= g_aUi_bomb_00[nCntUi_bomb_00].rot;		// ����
		int nPtn			= g_aUi_bomb_00[nCntUi_bomb_00].nPattern;	// �e�N�X�`���̃p�^�[��No.
		float fWidth		= g_aUi_bomb_00[nCntUi_bomb_00].fWidth;		// ��
		float fHeight		= g_aUi_bomb_00[nCntUi_bomb_00].fHeight;	// ����
		int nPtnMaxX		= g_ui_bomb_00Type.nPtnMaxX;				// �e�N�X�`���̃p�^�[����(��)
		int nPtnMaxY		= g_ui_bomb_00Type.nPtnMaxY;				// �e�N�X�`���̃p�^�[����(�c)

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));

		{
			float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle = atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;

			// �e�N�X�`���̃p�^�[��No.��K�p
			g_aUi_bomb_00[nCntUi_bomb_00].nPattern = nPtn;
		}
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffUi_bomb_00->Unlock();
}

//========================================
// DrawUi_bomb_00�֐� - �{��[00]�̕`�揈�� -
//========================================
void DrawUi_bomb_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ ���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_bomb_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_bomb_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_bomb_00 = 0; nCntUi_bomb_00 < GetChr_fighter_00()->nBomb; nCntUi_bomb_00++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureUi_bomb_00);

		// �{��[00]�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_bomb_00 * 4, 2);
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffUi_bomb_00->Unlock();
}