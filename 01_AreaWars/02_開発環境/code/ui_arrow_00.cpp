//========================================
// 
// ���[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_arrow_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "ui_arrow_00.h"

//****************************************
// �񋓌^�̒�`
//****************************************

// ���[00] �̕��i�ԍ�
typedef enum
{
	UI_ARROW_00_PARTS_TOP_ARROW = 0,	// ���(��)
	UI_ARROW_00_PARTS_BOTTOM_ARROW,		// ���(��)
	UI_ARROW_00_PARTS_MAX,
}UI_ARROW_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// ���[00] �̃|���S�����̏��\����
typedef struct
{
	char			aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
					[TXT_MAX];		// 
	int				nPtnMaxX;		// �p�^�[���̏��(��)
	int				nPtnMaxY;		// �p�^�[���̏��(�c)
	int				nPtnMin;		// �p�^�[��No.�̉���
	int				nPtnMax;		// �p�^�[��No.�̏��
	int				nAnimTime;		// �A�j���[�V�����ɂ����鎞��
	float			fWidth;			// ��
	float			fHeight;		// ����
	int				nPolygonNum;	// �K�v�|���S����
	float			fAngle;			// ����
	D3DXVECTOR3		relativePos;	// ���Έʒu

	int				nPtn;			// �p�^�[��No.
	int				nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
}Ui_arrow_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���i(���(��))�̍X�V����
void UpdateUi_arrow_00Parts_top_arrow(void);

// ���i(���(��))�̍X�V����
void UpdateUi_arrow_00Parts_bottom_arrow(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_arrow_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_arrow_00	// �e�N�X�`���ւ̃|�C���^
						[UI_ARROW_00_PARTS_MAX]	//
						= {};					//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_arrow_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;					//
Ui_arrow_00				g_ui_arrow_00;			// ���[00] �̏��

// ���[00] �̕��i���̏��
Ui_arrow_00Parts g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_arrow_001_8x4.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 8 , PIXEL * 4 , 1 , D3DX_PI * 1.0f	, D3DXVECTOR3(PIXEL * 0, PIXEL * -7, 0.0f) },
	{ "data\\TEXTURE\\ui_arrow_001_8x4.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 8 , PIXEL * 4 , 1 , 0.0f				, D3DXVECTOR3(PIXEL * 0, PIXEL *  6, 0.0f) },
};

//========== *** ���[00] �����擾 ***
Ui_arrow_00 *GetUi_arrow_00(void)
{
	return &g_ui_arrow_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_arrow_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_arrow_00(void) 
{
	g_ui_arrow_00.bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���
}

//========================================
// UpdateUi_arrow_00Parts_top_arrow�֐� - ���i(���(��))�̍X�V���� -
//========================================
void UpdateUi_arrow_00Parts_top_arrow(void)
{
	Ui_arrow_00Parts	*pParts							// 
						= &g_aUi_arrow_00Parts			//
						[UI_ARROW_00_PARTS_TOP_ARROW];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_arrow_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_arrow_00(UI_ARROW_00_PARTS_TOP_ARROW));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_arrow_00.pos + g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].fAngle),
			false,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].fWidth,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].fHeight,
			ANGLE_TYPE_FREE);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtn,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMin,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMax,
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nCounterAnim,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMaxX,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_TOP_ARROW].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_arrow_00Parts_bottom_arrow�֐� - ���i(���(��))�̍X�V���� -
//========================================
void UpdateUi_arrow_00Parts_bottom_arrow(void)
{
	Ui_arrow_00Parts	*pParts								// 
						= &g_aUi_arrow_00Parts				//
						[UI_ARROW_00_PARTS_BOTTOM_ARROW];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;								// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_arrow_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_arrow_00(UI_ARROW_00_PARTS_BOTTOM_ARROW));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_arrow_00.pos + g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].fAngle),
			false,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].fWidth,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].fHeight,
			ANGLE_TYPE_FREE);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtn,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMin,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMax,
				&g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nCounterAnim,
				g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMaxX,
			g_aUi_arrow_00Parts[UI_ARROW_00_PARTS_BOTTOM_ARROW].nPtnMaxY,
			NULL);
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_arrow_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// �K�v�|���S����

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_arrow_00Parts[nCntPolygon].nPolygonNum;
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
// InitUi_arrow_00�֐� - ���[00] �̏��������� -
//========================================
void InitUi_arrow_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_arrow_00(UI_ARROW_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_arrow_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_arrow_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aUi_arrow_00Parts[nCntParts].aTexturePath, &g_aTextureUi_arrow_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_arrow_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_arrow_00Parts[nCntParts].fWidth,
				g_aUi_arrow_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx,
				0,
				g_aUi_arrow_00Parts[nCntParts].nPtnMaxX,
				g_aUi_arrow_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_arrow_00->Unlock();
}

//========================================
// UninitUi_arrow_00�֐� - ���[00] �̏I������ -
//========================================
void UninitUi_arrow_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_arrow_00[nCntParts] != NULL)
		{
			g_aTextureUi_arrow_00[nCntParts]->Release();
			g_aTextureUi_arrow_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_arrow_00 != NULL)
	{
		g_pVtxBuffUi_arrow_00->Release();
		g_pVtxBuffUi_arrow_00 = NULL;
	}
}

//========================================
// UpdateUi_arrow_00�֐� - ���[00] �̍X�V���� -
//========================================
void UpdateUi_arrow_00(void)
{
	Ui_arrow_00	*pUi				//
				= &g_ui_arrow_00;	// ���[00] �̏��̃|�C���^

	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** ���(��) ***
		case UI_ARROW_00_PARTS_TOP_ARROW:
		{
			// ���i(���(��))�̍X�V����
			UpdateUi_arrow_00Parts_top_arrow();
		}
		break;
			//========== *** ���(��) ***
		case UI_ARROW_00_PARTS_BOTTOM_ARROW:
		{
			// ���i(���(��))�̍X�V����
			UpdateUi_arrow_00Parts_bottom_arrow();
		}
		break;
		}
	}
}

//========================================
// DrawUi_arrow_00�֐� - ���[00] �̕`�揈�� -
//========================================
void DrawUi_arrow_00(void)
{
	if (!g_ui_arrow_00.bUse) 
	{// �g�p����Ă��Ȃ���Ԃ̎��A
		// ������܂�Ԃ�
		return;
	}

	Ui_arrow_00	*pUi				//
				= &g_ui_arrow_00;	// ���[00] �̏��̃|�C���^

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_arrow_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < UI_ARROW_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_arrow_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_arrow_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// ���[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}