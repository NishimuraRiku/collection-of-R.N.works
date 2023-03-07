//========================================
// 
// UI:���Ӄe�L�X�g[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_warning-text_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include "ui_warning-text_00.h"	// UI :���Ӄe�L�X�g	[00]

//****************************************
// �}�N����`
//****************************************
// UI:���Ӄe�L�X�g[00] �̈ʒu
// UI:���Ӄe�L�X�g[00] �̃e�L�X�g�̑��Έʒu
#define UI_WARNINGTEXT_00_POS				(D3DXVECTOR3((BUFFER_WIDTH*0.5f),PIXEL*64,0.0f))
#define UI_WARNINGTEXT_00_TEXT_RELATIVE_POS	(D3DXVECTOR3(0,PIXEL*-8,0.0f))

// UI:���Ӄe�L�X�g[00] �̓_�łɂ����鎞��
#define UI_WARNINGTEXT_00_BRINK_TIME		(60)

// UI:���Ӄe�L�X�g[00] �̉e�̊g����
#define UI_WARNINGTEXT_00_SHADOW_SCALE_MAX	(1.1f)

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:���Ӄe�L�X�g[00] �̕��i�ԍ�
typedef enum
{
	UI_WARNINGTEXT_00_PARTS_CURTAIN,		// �J�[�e��
	UI_WARNINGTEXT_00_PARTS_MARK,			// �}�[�N
	UI_WARNINGTEXT_00_PARTS_SHADOW,			// �e
	UI_WARNINGTEXT_00_PARTS_MAX,
}UI_WARNINGTEXT_00_PARTS;

// UI:���Ӄe�L�X�g[00] �̃e�N�X�`���ԍ�
typedef enum 
{
	UI_WARNINGTEXT_00_TEXTURE_CURTAIN,	// �J�[�e��
	UI_WARNINGTEXT_00_TEXTURE_NORMAL,	// �ʏ�
	UI_WARNINGTEXT_00_TEXTURE_WARNING,	// ����
	UI_WARNINGTEXT_00_TEXTURE_MAX,
}UI_WARNINGTEXT_00_TEXTURE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:���Ӄe�L�X�g[00] �̕��i���̏��\����
typedef struct
{
	float	fWidth;		// ��
	float	fHeight;	// ����
}Ui_warningText_00Parts;

// UI:���Ӄe�L�X�g[00] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	int		nPtnWidth;				// �p�^�[����
	int		nPtnHeight;				// �p�^�[������
}Ui_warningText_00Texture;

// UI:���Ӄe�L�X�g[00] �̊Ǘ����
typedef struct
{
	int	nCounterBlink;	// �_�ŃJ�E���^�[
}Ui_warningText_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���Ӄe�L�X�g[00] �̊Ǘ����̃p�����[�^�[����������
void InitParameterUi_warningText_00Control(void);

// UI:���Ӄe�L�X�g[00] �̕��i���̒��_���W�ݒ菈��
void UpdatePartsUi_warningText_00(VERTEX_2D *pVtx, UI_WARNINGTEXT_00_PARTS parts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9			g_aTextureUi_warningText_00	// �e�N�X�`���ւ̃|�C���^
							[UI_WARNINGTEXT_00_TEXTURE_MAX]
							= {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffUi_warningText_00	// ���_�o�b�t�@�ւ̃|�C���^
							= NULL;
Ui_warningText_00Control	g_ui_warningTextControl;	// UI:���Ӄe�L�X�g[00] �̊Ǘ����

// UI:���Ӄe�L�X�g[00] �̃e�N�X�`�����̏��
const char c_aUi_warningText_00TexturePath[UI_WARNINGTEXT_00_TEXTURE_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\UserInterface\\ui_curtain_000.png",			// �J�[�e��
	"data\\TEXTURE\\UserInterface\\ui_monster-mark_000.png",	// �}�[�N
	"data\\TEXTURE\\UserInterface\\ui_boss-mark_000.png",		// ����
};

// UI:���Ӄe�L�X�g[00] �̕��i���̏��
const Ui_warningText_00Parts c_aUi_warningText_00Parts[UI_WARNINGTEXT_00_PARTS_MAX] =
{
	{ PIXEL * 64, PIXEL * 16 },	// �J�[�e��
	{ PIXEL * 16, PIXEL * 16 },	// ���Ӄe�L�X�g
	{ PIXEL * 16, PIXEL * 16 },	// �e
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_warningText_00Control�֐� - UI:���Ӄe�L�X�g[00] �̊Ǘ����̃p�����[�^�[���������� -
//========================================
void InitParameterUi_warningText_00Control(void) 
{
	Ui_warningText_00Control	*pUiCtl	// UI:���Ӄe�L�X�g[00] �̊Ǘ����̃|�C���^
								= &g_ui_warningTextControl;

	pUiCtl->nCounterBlink = 0;	// �_�ŃJ�E���^�[
}

//========================================
// SetVertexPositionUi_warningText_00�֐� - UI:���Ӄe�L�X�g[00] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void UpdatePartsUi_warningText_00(VERTEX_2D *pVtx, UI_WARNINGTEXT_00_PARTS parts) 
{
	Ui_warningText_00Control	*pUiCtl	// UI:���Ӄe�L�X�g[00] �̊Ǘ����̃|�C���^
								= &g_ui_warningTextControl;

	switch (parts)
	{
	case /*/ �}�[�N /*/UI_WARNINGTEXT_00_PARTS_MARK: {
		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_WARNINGTEXT_00_POS + UI_WARNINGTEXT_00_TEXT_RELATIVE_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			c_aUi_warningText_00Parts[parts].fWidth,
			c_aUi_warningText_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);

		break;
	}
	case /*/ �e /*/UI_WARNINGTEXT_00_PARTS_SHADOW: {
		float	fRate	= (float)pUiCtl->nCounterBlink / (float)UI_WARNINGTEXT_00_BRINK_TIME;
		float	fScale	= (1.0f + ((UI_WARNINGTEXT_00_SHADOW_SCALE_MAX - 1.0f) * fRate));

		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_WARNINGTEXT_00_POS + UI_WARNINGTEXT_00_TEXT_RELATIVE_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			c_aUi_warningText_00Parts[parts].fWidth * fScale,
			c_aUi_warningText_00Parts[parts].fHeight * fScale,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * (1.0f - fRate)) });

		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_warningText_00�֐� - UI:���Ӄe�L�X�g[00] �̏��������� -
//========================================
void InitUi_warningText_00(void)
{
	// �Ǘ����̃p�����[�^�[�̏���������
	InitParameterUi_warningText_00Control();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_WARNINGTEXT_00_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_warningText_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_warningText_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < UI_WARNINGTEXT_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, c_aUi_warningText_00TexturePath[nCntTexture], &g_aTextureUi_warningText_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_WARNINGTEXT_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			UI_WARNINGTEXT_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			c_aUi_warningText_00Parts[nCntParts].fWidth,
			c_aUi_warningText_00Parts[nCntParts].fHeight,
			ANGLE_TYPE_FIXED);

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			0,
			1,
			1,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_warningText_00->Unlock();
}

//========================================
// UninitUi_warningText_00�֐� - UI:���Ӄe�L�X�g[00] �̏I������ -
//========================================
void UninitUi_warningText_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < UI_WARNINGTEXT_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_warningText_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_warningText_00[nCntTexture]->Release();
			g_aTextureUi_warningText_00[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_warningText_00 != NULL) 
	{
		g_pVtxBuffUi_warningText_00->Release();
		g_pVtxBuffUi_warningText_00 = NULL;
	}
}

//========================================
// UpdateUi_warningText_00�֐� - UI:���Ӄe�L�X�g[00] �̍X�V���� -
//========================================
void UpdateUi_warningText_00(void) 
{
	Ui_warningText_00Control	*pUiCtl	// UI:���Ӄe�L�X�g[00] �̊Ǘ����̃|�C���^
								= &g_ui_warningTextControl;

	// �_�ŃJ�E���^�[��OBJ:�X�e�[�W�̃J�E���^�[��������
	pUiCtl->nCounterBlink = GetObj_stage_00()->nCounter % UI_WARNINGTEXT_00_BRINK_TIME;

	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_warningText_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_WARNINGTEXT_00_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���i���̍X�V����
		UpdatePartsUi_warningText_00(pVtx, (UI_WARNINGTEXT_00_PARTS)nCntParts);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_warningText_00->Unlock();
}

//========================================
// DrawUi_warningText_00�֐� - UI:���Ӄe�L�X�g[00] �̕`�揈�� -
//========================================
void DrawUi_warningText_00(void)
{
	if (GetObj_stage_00()->state != OBJ_STAGE_00_STATE_INTERVAL)
	{// OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���łȂ����A
		return;	// �������I������
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_warningText_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < UI_WARNINGTEXT_00_PARTS_MAX; nCntParts++)
	{
		UI_WARNINGTEXT_00_TEXTURE tex;

		if (nCntParts != UI_WARNINGTEXT_00_PARTS_CURTAIN) 
		{// �J�E���g�̕��i���J�[�e���łȂ����A
			tex = !GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
				UI_WARNINGTEXT_00_TEXTURE_NORMAL : UI_WARNINGTEXT_00_TEXTURE_WARNING;
		}
		else 
		{// �J�E���g�̕��i���J�[�e���̎��A
			tex = UI_WARNINGTEXT_00_TEXTURE_CURTAIN;
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0,g_aTextureUi_warningText_00[tex]);

		// UI:���Ӄe�L�X�g[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParts * 4, 2);
	}
}