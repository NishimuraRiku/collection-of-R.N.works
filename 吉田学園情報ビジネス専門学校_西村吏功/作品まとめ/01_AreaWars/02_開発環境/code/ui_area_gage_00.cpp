//========================================
// 
// �G���A�Q�[�W[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_area_gage_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "sys_stage_00.h"
#include "ui_area_gage_00.h"

//****************************************
// �}�N����`
//****************************************

// �G���A�Q�[�W[00] �̈ʒu
// �G���A�Q�[�W[00] �̕�
// �G���A�Q�[�W[00] �̖_�����̕�
#define UI_AREA_GAGE_00_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT - PIXEL * 6), 0.0f))
#define UI_AREA_GAGE_00_WIDTH		(960)
#define UI_AREA_GAGE_00_BAR_WIDTH	(960 - PIXEL * 6)

//****************************************
// �񋓌^�̒�`
//****************************************

// �G���A�Q�[�W[00] �̕��i�ԍ�
typedef enum
{
	UI_AREA_GAGE_00_PARTS_BACK_FRAME = 0,	// ���̘g
	UI_AREA_GAGE_00_PARTS_BAR,				// �_
	UI_AREA_GAGE_00_PARTS_BAR_RIGHT,		// �_�̉E�[
	UI_AREA_GAGE_00_PARTS_FRAME,			// �g
	UI_AREA_GAGE_00_PARTS_FRAME_LEFT,		// �g�̍��[
	UI_AREA_GAGE_00_PARTS_FRAME_RIGHT,		// �g�̉E�[
	UI_AREA_GAGE_00_PARTS_MAX,
}UI_AREA_GAGE_00_PARTS;

// �G���A�Q�[�W[00] �̃e�N�X�`���ԍ�
typedef enum 
{
	UI_AREA_GAGE_00_TEXTURE_BAR = 0,	// �_
	UI_AREA_GAGE_00_TEXTURE_FRAME,		// �g
	UI_AREA_GAGE_00_TEXTURE_MAX,
}UI_AREA_GAGE_00_TEXTURE;

//****************************************
// �\���̂̒�`
//****************************************

// �G���A�Q�[�W[00] �̕��i���̏��\����
typedef struct
{
	UI_AREA_GAGE_00_TEXTURE texture;		// �e�N�X�`���ԍ�
	int						nPattern;		// �p�^�[��No.
	float					fWidth;			// ��
	float					fHeight;		// ����
	int						nPolygonNum;	// �K�v�|���S����
}Ui_area_gage_00Parts;

// �G���A�Q�[�W[00] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	int		nPtnMaxX;				// �p�^�[�����X
	int		nPtnMaxY;				// �p�^�[�����Y
}Ui_area_gage_00Texture;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �G���A�Q�[�W[00] �̕��i���̒��_���W�ݒ菈��
void SetVertexPositionUi_area_gage_00(VERTEX_2D *pVtx, UI_AREA_GAGE_00_PARTS parts);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_area_gage_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_area_gage_00		// �e�N�X�`���ւ̃|�C���^
						[UI_AREA_GAGE_00_TEXTURE_MAX]	//
						= {};							//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_area_gage_00		// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;							//

// �G���A�Q�[�W[00] �̃e�N�X�`�����̏��
Ui_area_gage_00Texture g_aUi_area_gage_00Texture[UI_AREA_GAGE_00_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\ui_gage_003_h12_bar_orange.png", 2, 1 },
	{ "data\\TEXTURE\\ui_gage_003_h12_frame.png"     , 4, 1 },
};

// �G���A�Q�[�W[00] �̕��i���̏��
Ui_area_gage_00Parts g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_MAX] =
{
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 3, PIXEL * 0, PIXEL * 16, 1 },
	{ UI_AREA_GAGE_00_TEXTURE_BAR  , 0, PIXEL * 0, PIXEL * 8 , 1 },
	{ UI_AREA_GAGE_00_TEXTURE_BAR  , 1, PIXEL * 1, PIXEL * 8 , 1 },
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 1, PIXEL * 0, PIXEL * 16, 1 },
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 0, PIXEL * 4, PIXEL * 16, 1 },
	{ UI_AREA_GAGE_00_TEXTURE_FRAME, 2, PIXEL * 4, PIXEL * 16, 1 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_area_gage_00�֐� - �G���A�Q�[�W[00] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void SetVertexPositionUi_area_gage_00(VERTEX_2D *pVtx, UI_AREA_GAGE_00_PARTS parts) 
{
	float	fBarLengthDiameter	// �_�̒����̔{��
			= ((float)GetSys_stage_00()->nWaveTime / (float)GetSys_stage_00()->nWaveEndTime);

	if (!GetSys_stage_00()->bShowAreaTime)
	{// �G���A���Ԃ̕\���t���O���U�̎��A
		// �_�̒����̔{����0�ɂ���
		fBarLengthDiameter = 0.0f;
	}

	switch (parts)
	{
		//========== *** ���̘g ***
	case UI_AREA_GAGE_00_PARTS_BACK_FRAME:
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_AREA_GAGE_00_BAR_WIDTH,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** �_ ***
	case UI_AREA_GAGE_00_PARTS_BAR:
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS
			+ D3DXVECTOR3(
				- (UI_AREA_GAGE_00_BAR_WIDTH * fBarLengthDiameter) * 0.5f,
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			(UI_AREA_GAGE_00_BAR_WIDTH * (1.0f - fBarLengthDiameter))
			- (g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_BAR_RIGHT].fWidth),
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** �_�̉E�[ ***
	case UI_AREA_GAGE_00_PARTS_BAR_RIGHT:
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS
			+ D3DXVECTOR3(
				+ (UI_AREA_GAGE_00_BAR_WIDTH * 0.5f)
				- (UI_AREA_GAGE_00_BAR_WIDTH * fBarLengthDiameter)
				- (g_aUi_area_gage_00Parts[parts].fWidth * 0.5f),
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_area_gage_00Parts[parts].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** �g ***
	case UI_AREA_GAGE_00_PARTS_FRAME:
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_AREA_GAGE_00_WIDTH
			- g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_FRAME_LEFT].fWidth
			- g_aUi_area_gage_00Parts[UI_AREA_GAGE_00_PARTS_FRAME_RIGHT].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** �g�̍��[ ***
	case UI_AREA_GAGE_00_PARTS_FRAME_LEFT:
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS 
			+ D3DXVECTOR3(- (UI_AREA_GAGE_00_WIDTH * 0.5f) + (g_aUi_area_gage_00Parts[parts].fWidth * 0.5f), 
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_area_gage_00Parts[parts].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
		//========== *** �g�̉E�[ ***
	case UI_AREA_GAGE_00_PARTS_FRAME_RIGHT:
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_AREA_GAGE_00_POS
			+ D3DXVECTOR3((UI_AREA_GAGE_00_WIDTH * 0.5f) - (g_aUi_area_gage_00Parts[parts].fWidth * 0.5f),
				0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_area_gage_00Parts[parts].fWidth,
			g_aUi_area_gage_00Parts[parts].fHeight,
			ANGLE_TYPE_FIXED);
	}
		break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_area_gage_00(int nParts) 
{
	int nPolygonNum	// �K�v�|���S����
		= 0;		//

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_area_gage_00Parts[nCntPolygon].nPolygonNum;
	}

	// �K�v�|���S������Ԃ�
	return nPolygonNum;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_area_gage_00�֐� - �G���A�Q�[�W[00] �̏��������� -
//========================================
void InitUi_area_gage_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_area_gage_00(UI_AREA_GAGE_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_area_gage_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_area_gage_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < UI_AREA_GAGE_00_TEXTURE_MAX; nCntTexture++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_area_gage_00Texture[nCntTexture].aTexturePath, &g_aTextureUi_area_gage_00[nCntTexture]);
	}

	for (int nCntParts = 0; nCntParts < UI_AREA_GAGE_00_PARTS_MAX; nCntParts++)
	{
		for (int nCntPolygon = 0; nCntPolygon < g_aUi_area_gage_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���i���̒��_���W�ݒ菈��
			SetVertexPositionUi_area_gage_00(pVtx, (UI_AREA_GAGE_00_PARTS)nCntParts);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx, 
				g_aUi_area_gage_00Parts[nCntParts].nPattern, 
				g_aUi_area_gage_00Texture[g_aUi_area_gage_00Parts[nCntParts].texture].nPtnMaxX,
				g_aUi_area_gage_00Texture[g_aUi_area_gage_00Parts[nCntParts].texture].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_area_gage_00->Unlock();
}

//========================================
// UninitUi_area_gage_00�֐� - �G���A�Q�[�W[00] �̏I������ -
//========================================
void UninitUi_area_gage_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < UI_AREA_GAGE_00_TEXTURE_MAX; nCntTexture++)
	{
		if (g_aTextureUi_area_gage_00[nCntTexture] != NULL) 
		{
			g_aTextureUi_area_gage_00[nCntTexture]->Release();
			g_aTextureUi_area_gage_00[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_area_gage_00 != NULL) 
	{
		g_pVtxBuffUi_area_gage_00->Release();
		g_pVtxBuffUi_area_gage_00 = NULL;
	}
}

//========================================
// UpdateUi_area_gage_00�֐� - �G���A�Q�[�W[00] �̍X�V���� -
//========================================
void UpdateUi_area_gage_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_area_gage_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_AREA_GAGE_00_PARTS_MAX; nCntParts++)
	{
		for (int nCntPolygon = 0; nCntPolygon < g_aUi_area_gage_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���i���̒��_���W�ݒ菈��
			SetVertexPositionUi_area_gage_00(pVtx, (UI_AREA_GAGE_00_PARTS)nCntParts);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_area_gage_00->Unlock();
}

//========================================
// DrawUi_area_gage_00�֐� - �G���A�Q�[�W[00] �̕`�揈�� -
//========================================
void DrawUi_area_gage_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_area_gage_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < UI_AREA_GAGE_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_area_gage_00[g_aUi_area_gage_00Parts[nCntParts].texture]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_area_gage_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// �G���A�Q�[�W[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}