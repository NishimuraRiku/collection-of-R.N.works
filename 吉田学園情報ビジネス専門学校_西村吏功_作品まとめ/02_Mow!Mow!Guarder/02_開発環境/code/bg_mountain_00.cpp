//========================================
// 
// BG:�R[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_mountain_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon3D.h"
#include "sound.h"
#include "bg_mountain_00.h"	// BG :�R		[00]
#include "md_game_00.h"		// MD :�Q�[��	[00]
#include "obj_stage_00.h"	// OBJ:�X�e�[�W	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// BG:�R[00] �̃e�N�X�`���̑��΃p�X
#define BG_MOUNTAIN_00_TEXTURE_PATH	"data\\TEXTURE\\Background\\bg_mountain_000.png"

// BG:�R[00] �̔��a
// BG:�R[00] �̍���
// BG:�R[00] �̕�����
// BG:�R[00] �̕�������
// BG:�R[00] �̈ʒu
// BG:�R[00] �̖@���x�N�g��
#define BG_MOUNTAIN_00_HEIGHT			(256.0f)
#define BG_MOUNTAIN_00_RADIUS			(400.0f)
#define BG_MOUNTAIN_00_DIVISION_WIDTH	(16)
#define BG_MOUNTAIN_00_DIVISION_HEIGHT	(1)
#define BG_MOUNTAIN_00_POS				(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define BG_MOUNTAIN_00_NOR				(D3DXVECTOR3(0.0f,0.0f,1.0f))

//****************************************
// �\���̂̒�`
//****************************************
// BG:�R[00] �̏��\����
typedef struct
{
	D3DXVECTOR3 rot;	// ����
}Bg_mountain_00;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureBg_mountain_00	// BG:�R[00] �̃e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_mountain_00	// BG:�R[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBg_mountain_00	// BG:�R[00] �̃C���f�b�N�X�o�b�t�@�̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldBg_mountain_00;	// BG:�R[00] �̃��[���h�}�g���b�N�X
Bg_mountain_00			g_bg_mountain_00;			// BG:�R[00] �̏��

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_mountain_00�֐� - BG:�R[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterBg_mountain_00(void) 
{
	Bg_mountain_00		*pBg				// BG:�R[00] �̏��̃|�C���^
						= &g_bg_mountain_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitBg_mountain_00�֐� - BG:�R[00] �̏��������� -
//========================================
void InitBg_mountain_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_MOUNTAIN_00_TEXTURE_PATH, &g_pTextureBg_mountain_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (BG_MOUNTAIN_00_DIVISION_HEIGHT + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_mountain_00,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_mountain_00->Lock(0, 0, (void**)&pVtx, 0);

	// �p�����[�^�[�̏���������
	InitParameterBg_mountain_00();
	
	// ���_���W/�@���x�N�g��/�e�N�X�`�����W�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < BG_MOUNTAIN_00_DIVISION_HEIGHT + 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_MOUNTAIN_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			pVtx[(nCntHeight * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))) * BG_MOUNTAIN_00_RADIUS,
					BG_MOUNTAIN_00_HEIGHT - (nCntHeight * (BG_MOUNTAIN_00_HEIGHT / BG_MOUNTAIN_00_DIVISION_HEIGHT)),
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))) * BG_MOUNTAIN_00_RADIUS);

			pVtx[(nCntHeight * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + nCntWidth].nor 
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))),
					0.0f,
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_MOUNTAIN_00_DIVISION_WIDTH))));;

			pVtx[(nCntHeight * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + nCntWidth].tex 
				= D3DXVECTOR2(
					nCntWidth * (1.0f / (float)BG_MOUNTAIN_00_DIVISION_WIDTH),
					nCntHeight * (1.0f / (float)BG_MOUNTAIN_00_DIVISION_HEIGHT));
		}
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((BG_MOUNTAIN_00_DIVISION_HEIGHT * 2 * (BG_MOUNTAIN_00_DIVISION_WIDTH + 1)) + ((BG_MOUNTAIN_00_DIVISION_HEIGHT - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBg_mountain_00,
		NULL);

	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffBg_mountain_00->Lock(0, 0, (void**)&pIdx, 0);

	// ���_�ԍ��f�[�^�̐ݒ�
	int nCntIdx = 0;	// �C���f�b�N�X�̃J�E���g

	for (int nCntDepth = 0; nCntDepth < BG_MOUNTAIN_00_DIVISION_HEIGHT; nCntDepth++) 
	{
		for (int nCntWidth = 0; nCntWidth < BG_MOUNTAIN_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (nCntDepth + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * nCntDepth);
			nCntIdx += 2;
		}

		if (nCntDepth < BG_MOUNTAIN_00_DIVISION_HEIGHT - 1)
		{
			pIdx[nCntIdx] = ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (nCntDepth + 1)) - 1;
			pIdx[nCntIdx + 1] = (BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (nCntDepth + 2);
			nCntIdx += 2;
		}
	}
	
	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffBg_mountain_00->Unlock();

	for (int nCntVtx = 0; nCntVtx < ((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (BG_MOUNTAIN_00_DIVISION_HEIGHT + 1)); nCntVtx++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg_mountain_00->Unlock();
}

//========================================
// UninitBg_mountain_00�֐� - BG:�R[00] �̏I������ -
//========================================
void UninitBg_mountain_00(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureBg_mountain_00 != NULL)
	{
		g_pTextureBg_mountain_00->Release();
		g_pTextureBg_mountain_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg_mountain_00 != NULL)
	{
		g_pVtxBuffBg_mountain_00->Release();
		g_pVtxBuffBg_mountain_00 = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffBg_mountain_00 != NULL) 
	{
		g_pIdxBuffBg_mountain_00->Release();
		g_pIdxBuffBg_mountain_00 = NULL;
	}
}

//========================================
// UpdateBg_mountain_00�֐� - BG:�R[00] �̍X�V���� -
//========================================
void UpdateBg_mountain_00(void)
{

}

//========================================
// DrawBg_mountain_00�֐� - BG:�R[00] �̕`�揈�� -
//========================================
void DrawBg_mountain_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	Bg_mountain_00		*pBg				// BG:�R[00] �̏��̃|�C���^
						= &g_bg_mountain_00;

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBg_mountain_00);
	
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetObj_stage_00()->rot.y, GetObj_stage_00()->rot.x, GetObj_stage_00()->rot.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_mountain_00, &g_mtxWorldBg_mountain_00, &mtxRot);

	{
		D3DXVECTOR3 setPos;	// �ݒ�ʒu

		if (GetMode() == MODE_TITLE_00) 
		{// ���[�h��MD:�^�C�g�����[00] �̎��A
			setPos = GetObj_stage_00()->pos;	// �ݒ�ʒu��OBJ:�X�e�[�W[00] �̈ʒu����
		}
		if ((GetMode() == MODE_GAME_00) || (GetMode() == MODE_TUTORIAL_00))
		{// ���[�h��MD:�Q�[�����or�`���[�g���A����ʂ̎��A
			setPos = GetCamera3D()->posV;	// �ݒ�ʒu�ɃJ����(3D)�̎��_�̈ʒu����
			setPos.y = MD_GAME_00_BG_POS_Y;	// Y���W������
		}

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
		D3DXMatrixMultiply(&g_mtxWorldBg_mountain_00, &g_mtxWorldBg_mountain_00, &mtxTrans);
	}
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg_mountain_00);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg_mountain_00, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffBg_mountain_00);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg_mountain_00);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0, 
		0,
		((BG_MOUNTAIN_00_DIVISION_WIDTH + 1) * (BG_MOUNTAIN_00_DIVISION_HEIGHT + 1)),
		0, 
		((BG_MOUNTAIN_00_DIVISION_WIDTH * BG_MOUNTAIN_00_DIVISION_HEIGHT) * 2) + (4 * (BG_MOUNTAIN_00_DIVISION_HEIGHT - 1)));

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}