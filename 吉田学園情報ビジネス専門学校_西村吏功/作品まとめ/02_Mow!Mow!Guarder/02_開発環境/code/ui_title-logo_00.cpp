//========================================
// 
// UI:�^�C�g�����S[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_title-logo_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "ui_title-logo_00.h"	// UI :�^�C�g�����S	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�^�C�g�����S[00] �̈ʒu
// UI:�^�C�g�����S[00] �̃e�L�X�g�̑��Έʒu
#define UI_TITLELOGO_00_POS					(D3DXVECTOR3((BUFFER_WIDTH*0.5f)+PIXEL*64,(BUFFER_HEIGHT*0.5f)+PIXEL*-64,0.0f))
#define UI_TITLELOGO_00_TEXT_RELATIVE_POS	(D3DXVECTOR3(0,PIXEL*-13,0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:�^�C�g�����S[00] �̕��i�ԍ�
typedef enum
{
	UI_TITLELOGO_00_PARTS_TITLELOGO,	// �^�C�g�����S
	UI_TITLELOGO_00_PARTS_MAX,
}UI_TITLELOGO_00_PARTS;

// UI:�^�C�g�����S[00] �̃e�N�X�`���ԍ�
typedef enum 
{
	UI_TITLELOGO_00_TEXTURE_TITLELOGO,	// �^�C�g�����S
	UI_TITLELOGO_00_TEXTURE_MAX,
}UI_TITLELOGO_00_TEXTURE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:�^�C�g�����S[00] �̕��i���̏��\����
typedef struct
{
	UI_TITLELOGO_00_TEXTURE	tex;		// �e�N�X�`��
	float					fWidth;		// ��
	float					fHeight;	// ����
}Ui_titleLogo_00Parts;

// UI:�^�C�g�����S[00] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	int		nPtnWidth;				// �p�^�[����
	int		nPtnHeight;				// �p�^�[������
}Ui_titleLogo_00Texture;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�^�C�g�����S[00] �̕��i���̒��_���W�ݒ菈��
void UpdatePartsUi_titleLogo_00(VERTEX_2D *pVtx, UI_TITLELOGO_00_PARTS parts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_titleLogo_00	// �e�N�X�`���ւ̃|�C���^
						[UI_TITLELOGO_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_titleLogo_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;

// UI:�^�C�g�����S[00] �̃e�N�X�`�����̏��
const Ui_titleLogo_00Texture g_aUi_titleLogo_00Texture[UI_TITLELOGO_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_title-logo_000.png" , 1, 1 },	// �^�C�g�����S
};

// UI:�^�C�g�����S[00] �̕��i���̏��
const Ui_titleLogo_00Parts g_aUi_titleLogo_00Parts[UI_TITLELOGO_00_PARTS_MAX] =
{
	{ UI_TITLELOGO_00_TEXTURE_TITLELOGO, PIXEL * 256, PIXEL * 128 },	// �^�C�g�����S
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void UpdatePartsUi_titleLogo_00(VERTEX_2D *pVtx, UI_TITLELOGO_00_PARTS parts) 
{
	switch (parts)
	{
	case /*/ �^�C�g�����S /*/UI_TITLELOGO_00_PARTS_TITLELOGO:
	{
		
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
// InitUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̏��������� -
//========================================
void InitUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_TITLELOGO_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_titleLogo_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < UI_TITLELOGO_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_titleLogo_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_titleLogo_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_TITLELOGO_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_TITLELOGO_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_titleLogo_00Parts[nCntParts].fWidth,
			g_aUi_titleLogo_00Parts[nCntParts].fHeight,
			ANGLE_TYPE_FIXED);

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			0,
			g_aUi_titleLogo_00Texture[g_aUi_titleLogo_00Parts[nCntParts].tex].nPtnWidth,
			g_aUi_titleLogo_00Texture[g_aUi_titleLogo_00Parts[nCntParts].tex].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// UninitUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̏I������ -
//========================================
void UninitUi_titleLogo_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < UI_TITLELOGO_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_titleLogo_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_titleLogo_00[nCntTexture]->Release();
			g_aTextureUi_titleLogo_00[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_titleLogo_00 != NULL) 
	{
		g_pVtxBuffUi_titleLogo_00->Release();
		g_pVtxBuffUi_titleLogo_00 = NULL;
	}
}

//========================================
// UpdateUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̍X�V���� -
//========================================
void UpdateUi_titleLogo_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_TITLELOGO_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���i���̍X�V����
		UpdatePartsUi_titleLogo_00(pVtx, (UI_TITLELOGO_00_PARTS)nCntParts);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// DrawUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̕`�揈�� -
//========================================
void DrawUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_titleLogo_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_TITLELOGO_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_titleLogo_00[g_aUi_titleLogo_00Parts[nCntParts].tex]);

		// UI:�^�C�g�����S[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}