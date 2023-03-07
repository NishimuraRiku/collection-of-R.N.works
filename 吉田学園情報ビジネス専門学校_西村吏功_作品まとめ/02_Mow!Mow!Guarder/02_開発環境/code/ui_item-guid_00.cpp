//========================================
// 
// UI:���i�K�C�h[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_item-guid_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "item.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include "ui_item-guid_00.h"	// UI :���i�K�C�h	[00]

//****************************************
// �}�N����`
//****************************************
// UI:���i�K�C�h[00] �̈ʒu
#define UI_ITEMGUID_00_POS	(D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*64),(BUFFER_HEIGHT*0.5f),0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:���i�K�C�h[00] �̕��i�ԍ�
typedef enum
{
	UI_ITEMGUID_00_PARTS_ITEM_GUID,	// ���i����
	UI_ITEMGUID_00_PARTS_MAX,
}UI_ITEMGUID_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************
// UI:���i�K�C�h[00] �̕��i���̏��\����
typedef struct
{
	float	fWidth;		// ��
	float	fHeight;	// ����
}Ui_itemGuid_00Parts;

// UI:���i�K�C�h[00] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
			[TXT_MAX];
}Ui_itemGuid_00Texture;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���i�K�C�h[00] �̃p�����[�^�[����������
void InitParameterUi_itemGuid_00(void);

// UI:���i�K�C�h[00] �̕��i���̒��_���W�ݒ菈��
void UpdatePartsUi_itemGuid_00(VERTEX_2D *pVtx, UI_ITEMGUID_00_PARTS parts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_itemGuid_00	// �e�N�X�`���ւ̃|�C���^
						[OBJ_PEDESTAL_00_ITEM_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_itemGuid_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_itemGuid_00			g_ui_itemGuid_00;			// UI:���i����[00] �̏��

// UI:���i�K�C�h[00] �̃e�N�X�`�����̏��
const Ui_itemGuid_00Texture g_aUi_itemGuid_00Texture[OBJ_PEDESTAL_00_ITEM_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\block.png" },			// �u���b�N
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\turret.png" },			// �^���b�g
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\reflector.png" },		// ���ˑ��u
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\discharger.png" },		// ���d���u
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\repair-you.png" },		// �v���C���[��HP����
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\repair-unit.png" },		// ���j�b�g��HP����
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\slash-atk-up.png" },		// ���̍U���̓A�b�v
	{ "data\\TEXTURE\\UserInterface\\ui_item-guid_000\\bullet-atk-up.png" },	// �e�̍U���̓A�b�v
};

// UI:���i�K�C�h[00] �̕��i���̏��
const Ui_itemGuid_00Parts g_aUi_itemGuid_00Parts[UI_ITEMGUID_00_PARTS_MAX] =
{
	{ PIXEL * 128, PIXEL * 64 },	// ���i����
};

//========== *** UI:���i�K�C�h[00] �̏����擾 ***
Ui_itemGuid_00 *GetUi_itemGuid_00(void) 
{
	return &g_ui_itemGuid_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_itemGuid_00�֐� - UI:���i�K�C�h[00] �̃p�����[�^�[���������� -
//========================================
void InitParameterUi_itemGuid_00(void) 
{
	Ui_itemGuid_00	*pUi	// UI:���i�K�C�h[00] �̏��̃|�C���^
					= &g_ui_itemGuid_00;
	
	pUi->nTex				= 0;	// �e�N�X�`���ԍ�
	pUi->nCounterArrival	= 0;	// �����J�E���^�[
}

//========================================
// SetVertexPositionUi_itemGuid_00�֐� - UI:���i�K�C�h[00] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void UpdatePartsUi_itemGuid_00(VERTEX_2D *pVtx, UI_ITEMGUID_00_PARTS parts) 
{
	Ui_itemGuid_00	*pUi	// UI:���i�K�C�h[00] �̏��̃|�C���^
					= &g_ui_itemGuid_00;

	switch (parts)
	{
	case /*/ ���i���� /*/UI_ITEMGUID_00_PARTS_ITEM_GUID: {
		float	fRate	// �J�E���^�[�̐i�s��
				= (float)pUi->nCounterArrival / (float)UI_ITEMGUID_00_ARRIVAL_TIME;

		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_ITEMGUID_00_POS + D3DXVECTOR3(0.0f, (UI_ITEMGUID_00_NON_ARRIVAL_RELATIVE_POS_Y * fRate), 0),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_itemGuid_00Parts[parts].fWidth,
			g_aUi_itemGuid_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * fRate) });
	}
	break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_itemGuid_00�֐� - UI:���i�K�C�h[00] �̏��������� -
//========================================
void InitUi_itemGuid_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_ITEMGUID_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_itemGuid_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_itemGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < OBJ_PEDESTAL_00_ITEM_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_itemGuid_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_itemGuid_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_ITEMGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_ITEMGUID_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_itemGuid_00Parts[nCntParts].fWidth,
			g_aUi_itemGuid_00Parts[nCntParts].fHeight,
			ANGLE_TYPE_FIXED);

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx, 0, 1, 1, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_itemGuid_00->Unlock();

	// �p�����[�^�[�̏���������
	InitParameterUi_itemGuid_00();
}

//========================================
// UninitUi_itemGuid_00�֐� - UI:���i�K�C�h[00] �̏I������ -
//========================================
void UninitUi_itemGuid_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < UI_ITEMGUID_00_PARTS_MAX; nCntTexture++)
	{
		if (g_aTextureUi_itemGuid_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_itemGuid_00[nCntTexture]->Release();
			g_aTextureUi_itemGuid_00[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_itemGuid_00 != NULL) 
	{
		g_pVtxBuffUi_itemGuid_00->Release();
		g_pVtxBuffUi_itemGuid_00 = NULL;
	}
}

//========================================
// UpdateUi_itemGuid_00�֐� - UI:���i�K�C�h[00] �̍X�V���� -
//========================================
void UpdateUi_itemGuid_00(void) 
{
	Ui_itemGuid_00	*pUi	// UI:���i�K�C�h[00] �̏��̃|�C���^
					= &g_ui_itemGuid_00;

	if (GetChr_player_00()->nPedestalIndex != -1)
	{// CHR:�v���C���[[00] �̑���̔ԍ���-1�łȂ����A
		pUi->nCounterArrival++;	// �����J�E���^�[�����Z
		pUi->nTex				// �e�N�X�`���ԍ���ݒ�
			= GetObj_stage_00Type()[GetObj_stage_00()->nType].itemsSet.aItems[GetObj_pedestal_00Control()->nCntItems].aItem[GetChr_player_00()->nPedestalIndex];
	}
	else
	{// CHR:�v���C���[[00] �̑���̔ԍ���-1�̎��A
		pUi->nCounterArrival--;	// �����J�E���^�[�����Z
	}

	// �����J�E���^�[�𐧌�
	IntControl(&pUi->nCounterArrival, UI_ITEMGUID_00_ARRIVAL_TIME, 0);

	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_itemGuid_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_ITEMGUID_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���i���̍X�V����
		UpdatePartsUi_itemGuid_00(pVtx, (UI_ITEMGUID_00_PARTS)nCntParts);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_itemGuid_00->Unlock();
}

//========================================
// DrawUi_itemGuid_00�֐� - UI:���i�K�C�h[00] �̕`�揈�� -
//========================================
void DrawUi_itemGuid_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_itemGuid_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Ui_itemGuid_00	*pUi	// UI:���i�K�C�h[00] �̏��̃|�C���^
					= &g_ui_itemGuid_00;

	for (int nCntParts = 0; nCntParts < UI_ITEMGUID_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_itemGuid_00[pUi->nTex]);

		// UI:���i�K�C�h[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}