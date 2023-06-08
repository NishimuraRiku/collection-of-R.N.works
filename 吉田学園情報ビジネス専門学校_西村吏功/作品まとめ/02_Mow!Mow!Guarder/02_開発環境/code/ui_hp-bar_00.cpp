//========================================
// 
// HP�o�[[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_hp-bar_00.cpp ***
//========================================
#include "main.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"	// CHR:�v���C���[	[00]
#include "ui_hp-bar_00.h"	// UI :HP�o�[		[00]

//****************************************
// �}�N����`
//****************************************
// HP�o�[[00] �̈ʒu
// HP�o�[[00] �̃o�[�����̑��Έʒu
// HP�o�[[00] �̃e�L�X�g�̑��Έʒu
#define UI_HPBAR_00_POS					(D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*64),PIXEL*16,0.0f))
#define UI_HPBAR_00_BAR_RELATIVE_POS	(D3DXVECTOR3(PIXEL*-55,PIXEL*-4,0.0f))
#define UI_HPBAR_00_TEXT_RELATIVE_POS	(D3DXVECTOR3(PIXEL*13,PIXEL*11,0.0f))

// HP�o�[[00] �̃o�[������0%�J���[
// HP�o�[[00] �̃o�[������100%�J���[
#define UI_HPBAR_00_BAR_COLOR_0		Color{88,248,152,255}
#define UI_HPBAR_00_BAR_COLOR_100	Color{88,248,152,255}

//****************************************
// �񋓌^�̒�`
//****************************************
// HP�o�[[00] �̕��i�ԍ�
typedef enum
{
	UI_HPBAR_00_PARTS_FRAME,		// �t���[��
	UI_HPBAR_00_PARTS_BAR,			// �o�[
	UI_HPBAR_00_PARTS_BAR_RIGHT,	// �o�[�̉E�[
	UI_HPBAR_00_PARTS_MAX,
}UI_HPBAR_00_PARTS;

// HP�o�[[00] �̃e�N�X�`���ԍ�
typedef enum 
{
	UI_HPBAR_00_TEXTURE_FRAME,		// �t���[��
	UI_HPBAR_00_TEXTURE_BAR,		// �o�[
	UI_HPBAR_00_TEXTURE_MAX,
}UI_HPBAR_00_TEXTURE;

//****************************************
// �\���̂̒�`
//****************************************
// HP�o�[[00] �̕��i���̏��\����
typedef struct
{
	UI_HPBAR_00_TEXTURE texture;	// �e�N�X�`��
	int					nPattern;	// �p�^�[��No.
	float				fWidth;		// ��
	float				fHeight;	// ����
}Ui_hpBar_00Parts;

// HP�o�[[00] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	int		nPtnWidth;				// �p�^�[����
	int		nPtnHeight;				// �p�^�[������
}Ui_hpBar_00Texture;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// HP�o�[[00] �̕��i���̒��_���W�ݒ菈��
void UpdatePartsUi_hpBar_00(VERTEX_2D *pVtx, UI_HPBAR_00_PARTS parts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_hpBar_00	// �e�N�X�`���ւ̃|�C���^
						[UI_HPBAR_00_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_hpBar_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;

// HP�o�[[00] �̃e�N�X�`�����̏��
const Ui_hpBar_00Texture g_aUi_hpBar_00Texture[UI_HPBAR_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_hp-bar_000\\frame.png" , 1, 1 },	// �t���[��
	{ "data\\TEXTURE\\UserInterface\\ui_hp-bar_000\\bar.png"   , 2, 1 },	// �o�[
};

// HP�o�[[00] �̕��i���̏��
const Ui_hpBar_00Parts g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_MAX] =
{
	{ UI_HPBAR_00_TEXTURE_FRAME, 0, PIXEL * 128, PIXEL * 32 },	// �t���[��
	{ UI_HPBAR_00_TEXTURE_BAR  , 0, PIXEL * 108, PIXEL * 16 },	// �o�[
	{ UI_HPBAR_00_TEXTURE_BAR  , 1, PIXEL * 1  , PIXEL * 16 },	// �o�[�̉E�[
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_hpBar_00�֐� - HP�o�[[00] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void UpdatePartsUi_hpBar_00(VERTEX_2D *pVtx, UI_HPBAR_00_PARTS parts) 
{
	float	fBarLengthRate	// �o�[�̒����̔{��
			= ((float)GetChr_player_00()->nHP / (float)GetChr_player_00()->nHPMax);

	switch (parts)
	{
		//========== *** �t���[�� ***
	case UI_HPBAR_00_PARTS_FRAME:
	{
		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_HPBAR_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_hpBar_00Parts[parts].fWidth,
			g_aUi_hpBar_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
	break;
		//========== *** �o�[ ***
	case UI_HPBAR_00_PARTS_BAR:
	{
		D3DXVECTOR3 basePos	// ��ʒu
			= UI_HPBAR_00_POS + UI_HPBAR_00_BAR_RELATIVE_POS;

		// ���_���W��ݒ�
		pVtx[0].pos.x = basePos.x;
		pVtx[0].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[1].pos.x = basePos.x + (g_aUi_hpBar_00Parts[parts].fWidth * fBarLengthRate);
		pVtx[1].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[2].pos.x = basePos.x;
		pVtx[2].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[3].pos.x = basePos.x + (g_aUi_hpBar_00Parts[parts].fWidth * fBarLengthRate);
		pVtx[3].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, 
		{ 
			(int)((UI_HPBAR_00_BAR_COLOR_100.r * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.r * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.g * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.g * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.b * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.b * (1.0f - fBarLengthRate))),
			255
		});
	}
	break;
		//========== *** �o�[�̉E�[ ***
	case UI_HPBAR_00_PARTS_BAR_RIGHT:
	{
		D3DXVECTOR3 basePos	// ��ʒu
			= UI_HPBAR_00_POS + UI_HPBAR_00_BAR_RELATIVE_POS;

		// ���_���W��ݒ�
		pVtx[0].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate);
		pVtx[0].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[1].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate) + g_aUi_hpBar_00Parts[parts].fWidth;
		pVtx[1].pos.y = basePos.y - (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[2].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate);
		pVtx[2].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);
		pVtx[3].pos.x = basePos.x + (g_aUi_hpBar_00Parts[UI_HPBAR_00_PARTS_BAR].fWidth * fBarLengthRate) + g_aUi_hpBar_00Parts[parts].fWidth;
		pVtx[3].pos.y = basePos.y + (g_aUi_hpBar_00Parts[parts].fHeight * 0.5f);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx,
		{
			(int)((UI_HPBAR_00_BAR_COLOR_100.r * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.r * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.g * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.g * (1.0f - fBarLengthRate))),
			(int)((UI_HPBAR_00_BAR_COLOR_100.b * fBarLengthRate) + (UI_HPBAR_00_BAR_COLOR_0.b * (1.0f - fBarLengthRate))),
			255
		});
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
// InitUi_hpBar_00�֐� - HP�o�[[00] �̏��������� -
//========================================
void InitUi_hpBar_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_HPBAR_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_hpBar_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_hpBar_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < UI_HPBAR_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_hpBar_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_hpBar_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_HPBAR_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			g_aUi_hpBar_00Parts[nCntParts].nPattern,
			g_aUi_hpBar_00Texture[g_aUi_hpBar_00Parts[nCntParts].texture].nPtnWidth,
			g_aUi_hpBar_00Texture[g_aUi_hpBar_00Parts[nCntParts].texture].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_hpBar_00->Unlock();
}

//========================================
// UninitUi_hpBar_00�֐� - HP�o�[[00] �̏I������ -
//========================================
void UninitUi_hpBar_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < UI_HPBAR_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_hpBar_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_hpBar_00[nCntTexture]->Release();
			g_aTextureUi_hpBar_00[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_hpBar_00 != NULL) 
	{
		g_pVtxBuffUi_hpBar_00->Release();
		g_pVtxBuffUi_hpBar_00 = NULL;
	}
}

//========================================
// UpdateUi_hpBar_00�֐� - HP�o�[[00] �̍X�V���� -
//========================================
void UpdateUi_hpBar_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_hpBar_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���i���̍X�V����
		UpdatePartsUi_hpBar_00(pVtx, (UI_HPBAR_00_PARTS)nCntParts);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_hpBar_00->Unlock();

	{// �e�L�X�g�̐ݒ�
		char aString[TXT_MAX];	// ������
		sprintf(aString, "%3d/%3d", GetChr_player_00()->nHP, GetChr_player_00()->nHPMax);
		SetText2D(aString, FONT_000, DISPLAY_LEFT, UI_HPBAR_00_POS + UI_HPBAR_00_TEXT_RELATIVE_POS, { 255,255,255,255 }, 1.0f, 1.0f);
	}
}

//========================================
// DrawUi_hpBar_00�֐� - HP�o�[[00] �̕`�揈�� -
//========================================
void DrawUi_hpBar_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_hpBar_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_hpBar_00[g_aUi_hpBar_00Parts[nCntParts].texture]);

		// HP�o�[[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}