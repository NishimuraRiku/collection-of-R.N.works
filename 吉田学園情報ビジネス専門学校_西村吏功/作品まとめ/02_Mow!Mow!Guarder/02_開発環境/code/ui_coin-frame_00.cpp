//========================================
// 
// UI:�R�C���t���[��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_coin-frame_00.cpp ***
//========================================
#include "main.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:�v���C���[		[00]
#include "ui_coin-frame_00.h"	// UI :�R�C���t���[��	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�R�C���t���[��[00] �̈ʒu
// UI:�R�C���t���[��[00] �̃R�C���e�L�X�g�̑��Έʒu
// UI:�R�C���t���[��[00] �̃X�R�A�e�L�X�g�̑��Έʒu
#define UI_COINFRAME_00_POS						(D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*64),PIXEL*16,0.0f))
#define UI_COINFRAME_00_COIN_TEXT_RELATIVE_POS	(D3DXVECTOR3(PIXEL*-23,PIXEL*-4,0.0f))
#define UI_COINFRAME_00_SCORE_TEXT_RELATIVE_POS	(D3DXVECTOR3(PIXEL*-10,PIXEL*11,0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:�R�C���t���[��[00] �̕��i�ԍ�
typedef enum
{
	UI_COINFRAME_00_PARTS_FRAME,	// �t���[��
	UI_COINFRAME_00_PARTS_MAX,
}UI_COINFRAME_00_PARTS;

// UI:�R�C���t���[��[00] �̃e�N�X�`���ԍ�
typedef enum 
{
	UI_COINFRAME_00_TEXTURE_FRAME,	// �t���[��
	UI_COINFRAME_00_TEXTURE_MAX,
}UI_COINFRAME_00_TEXTURE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:�R�C���t���[��[00] �̕��i���̏��\����
typedef struct
{
	UI_COINFRAME_00_TEXTURE texture;	// �e�N�X�`��
	int						nPattern;	// �p�^�[��No.
	float					fWidth;		// ��
	float					fHeight;	// ����
}Ui_coinFrame_00Parts;

// UI:�R�C���t���[��[00] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	int		nPtnWidth;				// �p�^�[����
	int		nPtnHeight;				// �p�^�[������
}Ui_coinFrame_00Texture;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�R�C���t���[��[00] �̕��i���̒��_���W�ݒ菈��
void UpdatePartsUi_coinFrame_00(VERTEX_2D *pVtx, UI_COINFRAME_00_PARTS parts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_coinFrame_00	// �e�N�X�`���ւ̃|�C���^
						[UI_COINFRAME_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_coinFrame_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;

// UI:�R�C���t���[��[00] �̃e�N�X�`�����̏��
const Ui_coinFrame_00Texture g_aUi_coinFrame_00Texture[UI_COINFRAME_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_coin-frame_000.png" , 1, 1 },	// �t���[��
};

// UI:�R�C���t���[��[00] �̕��i���̏��
const Ui_coinFrame_00Parts g_aUi_coinFrame_00Parts[UI_COINFRAME_00_PARTS_MAX] =
{
	{ UI_COINFRAME_00_TEXTURE_FRAME, 0, PIXEL * 128, PIXEL * 32 },	// �t���[��
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_coinFrame_00�֐� - UI:�R�C���t���[��[00] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void UpdatePartsUi_coinFrame_00(VERTEX_2D *pVtx, UI_COINFRAME_00_PARTS parts) 
{
	switch (parts)
	{
		//========== *** �t���[�� ***
	case UI_COINFRAME_00_PARTS_FRAME:
	{
		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_COINFRAME_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_coinFrame_00Parts[parts].fWidth,
			g_aUi_coinFrame_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
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
// InitUi_coinFrame_00�֐� - UI:�R�C���t���[��[00] �̏��������� -
//========================================
void InitUi_coinFrame_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_COINFRAME_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_coinFrame_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_coinFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < UI_COINFRAME_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_coinFrame_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_coinFrame_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_COINFRAME_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			g_aUi_coinFrame_00Parts[nCntParts].nPattern,
			g_aUi_coinFrame_00Texture[g_aUi_coinFrame_00Parts[nCntParts].texture].nPtnWidth,
			g_aUi_coinFrame_00Texture[g_aUi_coinFrame_00Parts[nCntParts].texture].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_coinFrame_00->Unlock();
}

//========================================
// UninitUi_coinFrame_00�֐� - UI:�R�C���t���[��[00] �̏I������ -
//========================================
void UninitUi_coinFrame_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < UI_COINFRAME_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_coinFrame_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_coinFrame_00[nCntTexture]->Release();
			g_aTextureUi_coinFrame_00[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_coinFrame_00 != NULL) 
	{
		g_pVtxBuffUi_coinFrame_00->Release();
		g_pVtxBuffUi_coinFrame_00 = NULL;
	}
}

//========================================
// UpdateUi_coinFrame_00�֐� - UI:�R�C���t���[��[00] �̍X�V���� -
//========================================
void UpdateUi_coinFrame_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_coinFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_COINFRAME_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���i���̍X�V����
		UpdatePartsUi_coinFrame_00(pVtx, (UI_COINFRAME_00_PARTS)nCntParts);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_coinFrame_00->Unlock();

	{// �R�C���e�L�X�g�̐ݒ�
		char aString[TXT_MAX];	// ������
		sprintf(aString, "%6d", GetChr_player_00()->nCoin);
		SetText2D(aString, FONT_001, DISPLAY_LEFT, UI_COINFRAME_00_POS + UI_COINFRAME_00_COIN_TEXT_RELATIVE_POS, { 255,255,255,255 }, 1.0f, 1.0f);
	}

	{// �X�R�A�e�L�X�g�̐ݒ�
		char aString[TXT_MAX];	// ������
		if (GetMode() == MODE_TUTORIAL_00)
		{// ���݂̃��[�h���`���[�g���A���̎��A
			sprintf(aString, "      ?????");
		}
		else
		{// ���݂̃��[�h���`���[�g���A���łȂ����A
			sprintf(aString, "%11d", GetChr_player_00()->nScore);
		}
		SetText2D(aString, FONT_000, DISPLAY_LEFT, UI_COINFRAME_00_POS + UI_COINFRAME_00_SCORE_TEXT_RELATIVE_POS, { 255,255,255,255 }, 1.0f, 1.0f);
	}
}

//========================================
// DrawUi_coinFrame_00�֐� - UI:�R�C���t���[��[00] �̕`�揈�� -
//========================================
void DrawUi_coinFrame_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_coinFrame_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_COINFRAME_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_coinFrame_00[g_aUi_coinFrame_00Parts[nCntParts].texture]);

		// UI:�R�C���t���[��[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}