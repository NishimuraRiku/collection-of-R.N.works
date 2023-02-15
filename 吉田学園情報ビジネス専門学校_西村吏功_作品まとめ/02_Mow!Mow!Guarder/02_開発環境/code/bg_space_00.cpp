//========================================
// 
// BG:�F��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_space_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon3D.h"
#include "sound.h"
#include "bg_space_00.h"	// BG :�F��		[00]
#include "md_game_00.h"		// MD :�Q�[��	[00]
#include "obj_stage_00.h"	// OBJ:�X�e�[�W	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// BG:�F��[00] �̃e�N�X�`���̑��΃p�X
#define BG_SPACE_00_TEXTURE_PATH	"data\\TEXTURE\\Background\\bg_space_000.png"

// BG:�F��[00] �̔��a
// BG:�F��[00] �̍���
// BG:�F��[00] �̕�����
// BG:�F��[00] �̕�������
// BG:�F��[00] �̈ʒu
// BG:�F��[00] �̖@���x�N�g��
// BG:�F��[00] �̉�]��
#define BG_SPACE_00_RADIUS			(475.0f)
#define BG_SPACE_00_HEIGHT			(475.0f)
#define BG_SPACE_00_DIVISION_WIDTH	(16)
#define BG_SPACE_00_DIVISION_HEIGHT	(8)
#define BG_SPACE_00_POS				(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define BG_SPACE_00_NOR				(D3DXVECTOR3(0.0f,0.0f,1.0f))
#define BG_SPACE_00_SPIN			(D3DXVECTOR3(0.0f,0.001f,0.0f))

//****************************************
// �\���̂̒�`
//****************************************
// BG:�F��[00] �̏��\����
typedef struct
{
	D3DXVECTOR3 rot;	// ����
}Bg_space_00;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureBg_space_00		// BG:�F��[00] �̃e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_space_00		// BG:�F��[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBg_space_00		// BG:�F��[00] �̃C���f�b�N�X�o�b�t�@�̃|�C���^
						= NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBg_space_00Lid	// BG:�F��[00] �̊W�̃C���f�b�N�X�o�b�t�@�̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldBg_space_00;		// BG:�F��[00] �̃��[���h�}�g���b�N�X
Bg_space_00				g_bg_space_00;				// BG:�F��[00] �̏��

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_space_00�֐� - BG:�F��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterBg_space_00(void)
{
	Bg_space_00	*pBg	// BG:�F��[00] �̏��̃|�C���^
				= &g_bg_space_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitBg_space_00�֐� - BG:�F��[00] �̏��������� -
//========================================
void InitBg_space_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_SPACE_00_TEXTURE_PATH, &g_pTextureBg_space_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (((BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT) + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_space_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_00->Lock(0, 0, (void**)&pVtx, 0);

	// �p�����[�^�[�̏���������
	InitParameterBg_space_00();

	// �^��̒��_���W/�e�N�X�`�����W�̐ݒ�
	pVtx[(BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT].pos = D3DXVECTOR3(0.0f, BG_SPACE_00_HEIGHT, 0.0f);
	pVtx[(BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT].tex = D3DXVECTOR2(0.0f, 0.0f);

	// ���_���W/�@���x�N�g��/�e�N�X�`�����W�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < BG_SPACE_00_DIVISION_HEIGHT; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_SPACE_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			// ���_���W
			pVtx[(nCntHeight * (BG_SPACE_00_DIVISION_WIDTH + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))) * (BG_SPACE_00_RADIUS * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_SPACE_00_DIVISION_HEIGHT)))),
					BG_SPACE_00_HEIGHT - (((nCntHeight + 1) * (BG_SPACE_00_HEIGHT / BG_SPACE_00_DIVISION_HEIGHT)) * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_SPACE_00_DIVISION_HEIGHT)))),
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))) * (BG_SPACE_00_RADIUS * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_SPACE_00_DIVISION_HEIGHT)))));

			// �@���x�N�g��
			pVtx[(nCntHeight * (BG_SPACE_00_DIVISION_WIDTH + 1)) + nCntWidth].nor
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))),
					0.0f,
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_SPACE_00_DIVISION_WIDTH))));;

			// �e�N�X�`�����W
			pVtx[(nCntHeight * (BG_SPACE_00_DIVISION_WIDTH + 1)) + nCntWidth].tex
				= D3DXVECTOR2(
					nCntWidth * (1.0f / (float)BG_SPACE_00_DIVISION_WIDTH),
					(nCntHeight + 1) * (1.0f / (float)BG_SPACE_00_DIVISION_HEIGHT));
		}
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((((BG_SPACE_00_DIVISION_HEIGHT - 1) * 2 * (BG_SPACE_00_DIVISION_WIDTH + 1)) + ((BG_SPACE_00_DIVISION_HEIGHT - 2) * 2)) + 1),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBg_space_00,
		NULL);

	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffBg_space_00->Lock(0, 0, (void**)&pIdx, 0);

	// ���_�ԍ��f�[�^�̐ݒ�
	int nCntIdx = 0;	// �C���f�b�N�X�̃J�E���g

	for (int nCntHeight = 0; nCntHeight < BG_SPACE_00_DIVISION_HEIGHT - 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_SPACE_00_DIVISION_WIDTH + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((BG_SPACE_00_DIVISION_WIDTH + 1) * (nCntHeight + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((BG_SPACE_00_DIVISION_WIDTH + 1) * nCntHeight);
			nCntIdx += 2;
		}

		if (nCntHeight < BG_SPACE_00_DIVISION_HEIGHT - 2)
		{
			pIdx[nCntIdx] = ((BG_SPACE_00_DIVISION_WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (BG_SPACE_00_DIVISION_WIDTH + 1) * (nCntHeight + 2);
			nCntIdx += 2;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffBg_space_00->Unlock();

	{
		// �W�̃C���f�b�N�X�o�b�t�@�̐���
		pDevice->CreateIndexBuffer(sizeof(WORD) * (BG_SPACE_00_DIVISION_WIDTH + 2),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_pIdxBuffBg_space_00Lid,
			NULL);

		WORD *pIdx;	// �W�̃C���f�b�N�X���ւ̃|�C���^

		// �W�̃C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
		g_pIdxBuffBg_space_00Lid->Lock(0, 0, (void**)&pIdx, 0);

		pIdx[0] = (BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT;
		for (int nCntIdx = 0; nCntIdx < BG_SPACE_00_DIVISION_WIDTH + 1; nCntIdx++) 
		{
			pIdx[nCntIdx + 1] = BG_SPACE_00_DIVISION_WIDTH - nCntIdx;
		}

		// �W�̃C���f�b�N�X�o�b�t�@���A�����b�N����
		g_pIdxBuffBg_space_00Lid->Unlock();
	}

	for (int nCntVtx = 0; nCntVtx < ((BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT) + 1; nCntVtx++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg_space_00->Unlock();
}

//========================================
// UninitBg_space_00�֐� - BG:�F��[00] �̏I������ -
//========================================
void UninitBg_space_00(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureBg_space_00 != NULL)
	{
		g_pTextureBg_space_00->Release();
		g_pTextureBg_space_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg_space_00 != NULL)
	{
		g_pVtxBuffBg_space_00->Release();
		g_pVtxBuffBg_space_00 = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffBg_space_00 != NULL)
	{
		g_pIdxBuffBg_space_00->Release();
		g_pIdxBuffBg_space_00 = NULL;
	}

	// �W�̃C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffBg_space_00Lid != NULL)
	{
		g_pIdxBuffBg_space_00Lid->Release();
		g_pIdxBuffBg_space_00Lid = NULL;
	}
}

//========================================
// UpdateBg_space_00�֐� - BG:�F��[00] �̍X�V���� -
//========================================
void UpdateBg_space_00(void)
{
	Bg_space_00	*pBg	// BG:�F��[00] �̏��̃|�C���^
				= &g_bg_space_00;

	pBg->rot += BG_SPACE_00_SPIN;	// ��]��������
	RotControl(&pBg->rot);			// �����𐧌�
}

//========================================
// DrawBg_space_00�֐� - BG:�F��[00] �̕`�揈�� -
//========================================
void DrawBg_space_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	Bg_space_00			*pBg				// BG:�F��[00] �̏��̃|�C���^
						= &g_bg_space_00;

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBg_space_00);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetObj_stage_00()->rot.y + pBg->rot.y, GetObj_stage_00()->rot.x + pBg->rot.x, GetObj_stage_00()->rot.z + pBg->rot.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_space_00, &g_mtxWorldBg_space_00, &mtxRot);

	{
		D3DXVECTOR3 setPos;	// �ݒ�ʒu

		if (GetMode() == MODE_TITLE_00)
		{// ���[�h��MD:�^�C�g�����[00] �̎��A
			setPos = GetObj_stage_00()->pos;	// �ݒ�ʒu��OBJ:�X�e�[�W[00] �̈ʒu����
		}
		else if (GetMode() == MODE_GAME_00)
		{// ���[�h��MD:�Q�[�����[00] �̎��A
			setPos = GetCamera3D()->posV;	// �ݒ�ʒu�ɃJ����(3D)�̎��_�̈ʒu����
			setPos.y = MD_GAME_00_BG_POS_Y;	// Y���W������
		}

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
		D3DXMatrixMultiply(&g_mtxWorldBg_space_00, &g_mtxWorldBg_space_00, &mtxTrans);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg_space_00);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg_space_00, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg_space_00);

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffBg_space_00);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		(BG_SPACE_00_DIVISION_WIDTH + 1) * BG_SPACE_00_DIVISION_HEIGHT,
		0,
		((BG_SPACE_00_DIVISION_WIDTH * (BG_SPACE_00_DIVISION_HEIGHT - 1)) * 2) + (4 * ((BG_SPACE_00_DIVISION_HEIGHT - 1) - 1)));

	// �W�̃C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffBg_space_00Lid);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,
		0,
		0,
		(BG_SPACE_00_DIVISION_WIDTH + 2),
		0,
		BG_SPACE_00_DIVISION_WIDTH);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}