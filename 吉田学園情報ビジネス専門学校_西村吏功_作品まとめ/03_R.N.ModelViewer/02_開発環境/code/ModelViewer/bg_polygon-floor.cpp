//========================================
// 
// BG:�|���S�����̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_polygon-floor.cpp ***
//========================================
#include <stdio.h>
#include "main.h"
// BackGround
#include "bg_polygon-floor.h"
// R.N.Lib
#include "../R.N.Lib/RNmain.h"

//****************************************
// �}�N����`
//****************************************
// BG:�|���S�����̃e�N�X�`���̑��΃p�X
#define BG_POLYGONFLOOR_TEXTURE_PATH "data\\APPDATA\\TEXTURE\\BackGround\\bg_plaid-tile_000.png"
// BG:�|���S�����̕�
#define BG_POLYGONFLOOR_WIDTH (100.0f)
// BG:�|���S�����̍���
#define BG_POLYGONFLOOR_HEIGHT (100.0f)
// BG:�|���S�����̖@���x�N�g��
#define BG_POLYGONFLOOR_NOR (D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//****************************************
// �\���̂̒�`
//****************************************
// BG:�|���S�����̏��\����
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
}Bg_polygonFloor;

//****************************************
// �O���[�o���錾
//****************************************
// BG:�|���S�����̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBg_polygonFloor = NULL;
// BG:�|���S�����̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_polygonFloor = NULL;
D3DXMATRIX g_mtxWorldBg_polygonFloor;		// BG:�|���S�����̃��[���h�}�g���b�N�X
Bg_polygonFloor g_bg_polygonFloor_00;		// BG:�|���S�����̏��

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================
//========================================
// InitBg_polygonFloor�֐� - BG:�|���S�����̏��������� -
//========================================
void InitBg_polygonFloor(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_POLYGONFLOOR_TEXTURE_PATH, &g_pTextureBg_polygonFloor);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_polygonFloor,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_polygonFloor->Lock(0, 0, (void**)&pVtx, 0);
	
	// ���_���W�̐ݒ�
	SetVertexPos3D(pVtx, BG_POLYGONFLOOR_WIDTH, BG_POLYGONFLOOR_HEIGHT);
	
	// �@���x�N�g���̐ݒ�
	SetNormalLine3D(pVtx, BG_POLYGONFLOOR_NOR);
	
	// ���_�J���[�̐ݒ�
	SetVertexColor3D(pVtx, { 255,255,255,255 });
	
	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	
	// ���_���W���A�����b�N����
	g_pVtxBuffBg_polygonFloor->Unlock();
}

//========================================
// UninitBg_polygonFloor�֐� - BG:�|���S�����̏I������ -
//========================================
void UninitBg_polygonFloor(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureBg_polygonFloor != NULL)
	{
		g_pTextureBg_polygonFloor->Release();
		g_pTextureBg_polygonFloor = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg_polygonFloor != NULL)
	{
		g_pVtxBuffBg_polygonFloor->Release();
		g_pVtxBuffBg_polygonFloor = NULL;
	}
}

//========================================
// UpdateBg_polygonFloor�֐� - BG:�|���S�����̍X�V���� -
//========================================
void UpdateBg_polygonFloor(void)
{

}

//========================================
// DrawBg_polygonFloor�֐� - BG:�|���S�����̕`�揈�� -
//========================================
void DrawBg_polygonFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans;					// �v�Z�p�}�g���b�N�X
	Bg_polygonFloor *pBg = &g_bg_polygonFloor_00;	// BG:�|���S�����̏��̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBg_polygonFloor);
	
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pBg->rot.y, pBg->rot.x, pBg->rot.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_polygonFloor, &g_mtxWorldBg_polygonFloor, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pBg->pos.x, pBg->pos.y, pBg->pos.z);
	D3DXMatrixMultiply(&g_mtxWorldBg_polygonFloor, &g_mtxWorldBg_polygonFloor, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg_polygonFloor);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg_polygonFloor, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg_polygonFloor);

	// �|���S�����̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}