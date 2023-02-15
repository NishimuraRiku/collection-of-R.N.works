//========================================
// 
// �F��[00]�̏���
// Author:���� ����
// 
//========================================
//  *** bg_space_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "bg_space_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//========== �}�N����`			==========// 
#define BG_SPACE_00_NUM				(4)										// �F��[00] �̐�
#define BG_SPACE_00_UV_MOVE_0		(0.025f)								// �F��[00] ��UV�̈ړ��� [0]
#define BG_SPACE_00_UV_MOVE_1		(0.2f)									// �F��[00] ��UV�̈ړ��� [1]
#define BG_SPACE_00_UV_MOVE_2		(0.4f)									// �F��[00] ��UV�̈ړ��� [2]
#define BG_SPACE_00_UV_MOVE_3		(0.6f)									// �F��[00] ��UV�̈ړ��� [3]
#define BG_SPACE_00_TEXTURE_PATH_0	"data\\TEXTURE\\bg_space_000_03.png"	// �F��[00] �̃e�N�X�`���t�@�C���̑��΃p�X [0]
#define BG_SPACE_00_TEXTURE_PATH_1	"data\\TEXTURE\\bg_space_000_02.png"	// �F��[00] �̃e�N�X�`���t�@�C���̑��΃p�X [1]
#define BG_SPACE_00_TEXTURE_PATH_2	"data\\TEXTURE\\bg_space_000_01.png"	// �F��[00] �̃e�N�X�`���t�@�C���̑��΃p�X [2]
#define BG_SPACE_00_TEXTURE_PATH_3	"data\\TEXTURE\\bg_space_000_00.png"	// �F��[00] �̃e�N�X�`���t�@�C���̑��΃p�X [3]
#define BG_SPACE_00_WIDTH			(PIXEL * 1024)							// �F��[00] �̕�
#define BG_SPACE_00_HEIGHT			(PIXEL * 1024)							// �F��[00] �̍���

//========== �O���[�o���錾		==========// 
LPDIRECT3DTEXTURE9 g_aTextureBg_space_00[BG_SPACE_00_NUM] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_space_00 = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
POINT g_aTex[BG_SPACE_00_NUM];									// �e�N�X�`�����W�̊J�n�ʒu(UV�l)

//========================================
// InitBg_space_00�֐� - �F��[00]�̏��������� -
//========================================
void InitBg_space_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	POINT vertexPos[4];			// �l�p�`�̒��_�̍��W
	
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_0, &g_aTextureBg_space_00[0]);
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_1, &g_aTextureBg_space_00[1]);
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_2, &g_aTextureBg_space_00[2]);
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH_3, &g_aTextureBg_space_00[3]);

	// �e�N�X�`�����W�̊J�n�ʒu�̏�����
	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++) 
	{
		g_aTex[nCntBg_space_00].x = 0.0f;
		g_aTex[nCntBg_space_00].y = 0.0f;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * BG_SPACE_00_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_space_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++, pVtx += 4)
	{
		{
			D3DXVECTOR3 pos	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);	// �ʒu
			D3DXVECTOR3 rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// ����
			float fWidth	= BG_SPACE_00_WIDTH;											// ��
			float fHeight	= BG_SPACE_00_HEIGHT;											// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;		// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);										// �Ίp���̊p�x

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)	) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)	) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)	) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)	) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle				) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle				) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle				) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle				) * fLength;
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x			, g_aTex[nCntBg_space_00].y			);
		pVtx[1].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x + 1.0f	, g_aTex[nCntBg_space_00].y			);
		pVtx[2].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x			, g_aTex[nCntBg_space_00].y + 1.0f	);
		pVtx[3].tex = D3DXVECTOR2(g_aTex[nCntBg_space_00].x + 1.0f	, g_aTex[nCntBg_space_00].y + 1.0f	);
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffBg_space_00->Unlock();
}

//========================================
// UninitBg_space_00�֐� - �F��[00]�̏I������ -
//========================================
void UninitBg_space_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++)
	{
		if (g_aTextureBg_space_00[nCntBg_space_00] != NULL)
		{
			g_aTextureBg_space_00[nCntBg_space_00]->Release();
			g_aTextureBg_space_00[nCntBg_space_00] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg_space_00 != NULL) 
	{
		g_pVtxBuffBg_space_00->Release();
		g_pVtxBuffBg_space_00 = NULL;
	}
}

//========================================
// UpdateBg_space_00�֐� - �F��[00]�̍X�V���� -
//========================================
void UpdateBg_space_00(void)
{
	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^
	Camera *pCamera;	// �@��[00]�̏��

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_00->Lock(0, 0, (void**)&pVtx, 0);

	// �@��[00]�̏����擾
	pCamera = GetCamera();

	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++, pVtx += 4)
	{
		// �e�N�X�`�����W���X�V
		g_aTex[nCntBg_space_00].y = ((int)g_aTex[nCntBg_space_00].y % 100);

		float fMove;	// �ړ���

		if (nCntBg_space_00 == 0) { fMove = BG_SPACE_00_UV_MOVE_0; }
		if (nCntBg_space_00 == 1) { fMove = BG_SPACE_00_UV_MOVE_1; }
		if (nCntBg_space_00 == 2) { fMove = BG_SPACE_00_UV_MOVE_2; }
		if (nCntBg_space_00 == 3) { fMove = BG_SPACE_00_UV_MOVE_3; }

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove)			, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove)));
		pVtx[1].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f	, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove)));
		pVtx[2].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove)			, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f));
		pVtx[3].tex = D3DXVECTOR2(((pCamera->pos.x + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f	, (g_aTex[nCntBg_space_00].y + (((pCamera->pos.y + pCamera->vibrationPos.y) + pCamera->vibrationPos.x) * 0.001f * fMove) + 1.0f));
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffBg_space_00->Unlock();
}

//========================================
// DrawBg_space_00�֐� - �F��[00]�̕`�揈�� -
//========================================
void DrawBg_space_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg_space_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBg_space_00 = 0; nCntBg_space_00 < BG_SPACE_00_NUM; nCntBg_space_00++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureBg_space_00[nCntBg_space_00]);

		// �F��[00]�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg_space_00 * 4, 2);
	}
}