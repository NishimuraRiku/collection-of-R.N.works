//========================================
// 
// UI:�A�C�e���A�C�R��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_item-icon_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "obj_pedestal_00.h"	// OBJ:���				[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W			[00]
#include "ui_item-icon_00.h"	// UI :�A�C�e���A�C�R��	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// UI:�A�C�e���A�C�R��[00] �̍ő吔
#define UI_ITEMICON_00_MAX	(OBJ_PEDESTAL_00_MAX)

// UI:�A�C�e���A�C�R��[00] �̕�
// UI:�A�C�e���A�C�R��[00] �̍���
#define UI_ITEMICON_00_WIDTH	(PIXEL3D * 16)
#define UI_ITEMICON_00_HEIGHT	(PIXEL3D * 16)

// UI:�A�C�e���A�C�R��[00] �̖@���x�N�g��
// UI:�A�C�e���A�C�R��[00] �̑��Έʒu
#define UI_ITEMICON_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define UI_ITEMICON_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 6.0f, 0.0f))

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_itemIcon_00	// UI:�A�C�e���A�C�R��[00] �̃e�N�X�`���ւ̃|�C���^
						[OBJ_PEDESTAL_00_ITEM_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_itemIcon_00	// UI:�A�C�e���A�C�R��[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldUi_itemIcon_00;	// UI:�A�C�e���A�C�R��[00] �̃��[���h�}�g���b�N�X

// UI:�A�C�e���A�C�R��[00] �̃e�N�X�`���̑��΃p�X
const char g_aTexturePathUi_itemIcon_00[OBJ_PEDESTAL_00_ITEM_MAX][TXT_MAX] = 
{
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\block.png",			// �u���b�N
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\turret.png",			// �^���b�g
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\reflector.png",		// ���ˑ��u
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\discharger.png",		// ���d���u
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\player-hp-prus.png",	// �v���C���[��HP����
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\unit-hp-prus.png",		// ���j�b�g��HP����
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\sword-atk-up.png",		// ���̍U���̓A�b�v
	"data\\TEXTURE\\UserInterface\\ui_item-icon_000\\bullet-atk-up.png",	// �e�̍U���̓A�b�v
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_itemIcon_00�֐� - UI:�A�C�e���A�C�R��[00] �̏��������� -
//========================================
void InitUi_itemIcon_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntItem = 0; nCntItem < OBJ_PEDESTAL_00_ITEM_MAX; nCntItem++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aTexturePathUi_itemIcon_00[nCntItem], &g_aTextureUi_itemIcon_00[nCntItem]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * UI_ITEMICON_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_itemIcon_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_itemIcon_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < UI_ITEMICON_00_MAX; nCntUi++, pVtx += 4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(UI_ITEMICON_00_WIDTH * 0.5f),  (UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (UI_ITEMICON_00_WIDTH * 0.5f),  (UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(UI_ITEMICON_00_WIDTH * 0.5f), -(UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (UI_ITEMICON_00_WIDTH * 0.5f), -(UI_ITEMICON_00_HEIGHT * 0.5f), 0.0f);

		// �@���x�N�g���̐ݒ�
		SetNormalLine3D(pVtx, UI_ITEMICON_00_NOR);

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_itemIcon_00->Unlock();
}

//========================================
// UninitUi_itemIcon_00�֐� - UI:�A�C�e���A�C�R��[00] �̏I������ -
//========================================
void UninitUi_itemIcon_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntItem = 0; nCntItem < OBJ_PEDESTAL_00_ITEM_MAX; nCntItem++)
	{
		if (g_aTextureUi_itemIcon_00[nCntItem] != NULL)
		{
			g_aTextureUi_itemIcon_00[nCntItem]->Release();
			g_aTextureUi_itemIcon_00[nCntItem] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_itemIcon_00 != NULL)
	{
		g_pVtxBuffUi_itemIcon_00->Release();
		g_pVtxBuffUi_itemIcon_00 = NULL;
	}
}

//========================================
// UpdateUi_itemIcon_00�֐� - UI:�A�C�e���A�C�R��[00] �̍X�V���� -
//========================================
void UpdateUi_itemIcon_00(void)
{
	VERTEX_3D	*pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_itemIcon_00->Lock(0, 0, (void**)&pVtx, 0);

	Obj_pedestal_00 *pObj	// OBJ:���[00] �̏��̃|�C���^���擾
					= GetObj_pedestal_00();

	float	fArrivalRate	// ������
			= (float)GetObj_pedestal_00Control()->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME;

	for (int nCntUi = 0; nCntUi < UI_ITEMICON_00_MAX; nCntUi++, pObj++, pVtx += 4)
	{
		if (!pObj->bUse)
		{// �J�E���g��OBJ:���[00] ���g�p����Ă��Ȃ����A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, { 255,255,255,(int)(255 * fArrivalRate) });
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_itemIcon_00->Unlock();
}

//========================================
// DrawUi_itemIcon_00�֐� - UI:�A�C�e���A�C�R��[00] �̕`�揈�� -
//========================================
void DrawUi_itemIcon_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX			mtxView;	// �r���[�}�g���b�N�X�擾�p

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_itemIcon_00, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	Obj_pedestal_00 *pObj = GetObj_pedestal_00();	// OBJ:���[00] �̏��̃|�C���^���擾

	for (int nCntUi = 0; nCntUi < UI_ITEMICON_00_MAX; nCntUi++, pObj++)
	{
		if (!pObj->bUse)
		{// �J�E���g��OBJ:���[00] ���g�p����Ă��Ȃ����A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldUi_itemIcon_00);

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_mtxWorldUi_itemIcon_00, NULL, &mtxView);
		g_mtxWorldUi_itemIcon_00._41 = 0.0f;
		g_mtxWorldUi_itemIcon_00._42 = 0.0f;
		g_mtxWorldUi_itemIcon_00._43 = 0.0f;
		{
			D3DXVECTOR3 setPos = pObj->pos + UI_ITEMICON_00_RELATIVE_POS;	// �ݒ�ʒu��ݒ�

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldUi_itemIcon_00, &g_mtxWorldUi_itemIcon_00, &mtxTrans);
		}
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldUi_itemIcon_00);

		Obj_stage_00Type *pObj = GetObj_stage_00Type();

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_itemIcon_00[
			GetObj_stage_00Type()[GetObj_stage_00()->nType].itemsSet.aItems[GetObj_pedestal_00Control()->nCntItems].aItem[nCntUi]]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUi, 2);
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}