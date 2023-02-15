//========================================
// 
// �x��[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_warning_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_warning_00.h"

//****************************************
// �}�N����`
//****************************************

// �x��[00] �̏o���ɂ����鎞��
// �x��[00] �̏o�����鎞��
// �x��[00] �̏�����̂ɂ����鎞��
// �x��[00] �̓����x�̍ő�l
#define UI_WARNING_00_IN_POP_TIME		(40)
#define UI_WARNING_00_POP_TIME			(80)
#define UI_WARNING_00_IN_CLEAR_TIME		(40)
#define UI_WARNING_00_ALPHA_MAX			(0.8f)

// �x��[00] �̕����̈ʒu
#define UI_WARNING_00_TEXT_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))

// �x��[00] �̑я�̊�ʒu
// �x��[00] �̑щ��̊�ʒu
// �x��[00] �̑т̔z�u�ԊuX
// �x��[00] �̑т̈ړ��ɂ����鎞��
#define UI_WARNING_00_BELT_TOP_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - PIXEL * 24 * 2, 0.0f))
#define UI_WARNING_00_BELT_BOTTOM_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + PIXEL * 24 * 2, 0.0f))
#define UI_WARNING_00_BELT_SPACE_X		(PIXEL * 24 * 2)
#define UI_WARNING_00_MOVE_TIME			(12)

//****************************************
// �񋓌^�̒�`
//****************************************

// �x��[00] �̕��i�ԍ�
typedef enum
{
	UI_WARNING_00_PARTS_TEXT = 0,		// ����
	UI_WARNING_00_PARTS_BELT_TOP,		// �я�
	UI_WARNING_00_PARTS_BELT_BOTTOM,	// �щ�
	UI_WARNING_00_PARTS_MAX,
}UI_WARNING_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �x��[00] �̏��\����
typedef struct 
{
	UI_WARNING_00_STATE state;				// ���
	int					nCounterState;		// ��ԃJ�E���^�[
	int					nCounterBeltMove;	// �т̈ړ��J�E���^�[
	float				fAlpha;				// �S�̂̓����x
}Ui_warning_00;

// �x��[00] �̃|���S�����̏��\����
typedef struct
{
	char	aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
			[TXT_MAX];		// 
	float	fWidth;			// ��
	float	fHeight;		// ����
	int		nPolygonNum;	// �K�v�|���S����
}Ui_warning_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterUi_warning_00(void);

// ���i(����)�̍X�V����
void UpdateUi_warning_00Parts_text(void);

// ���i(�я�)�̍X�V����
void UpdateUi_warning_00Parts_belt_top(void);

// ���i(�щ�)�̍X�V����
void UpdateUi_warning_00Parts_belt_bottom(void);

// ��ԏ���
void StateProcessUi_warning_00(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_warning_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_warning_00		// �e�N�X�`���ւ̃|�C���^
						[UI_WARNING_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_warning_00		// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;						//
Ui_warning_00			g_ui_warning_00;			// �x��[00] �̏��

// �x��[00] �̕��i���̏��
Ui_warning_00Parts g_aUi_warning_00Parts[UI_WARNING_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_warning_000_text.png", PIXEL * 128 * 2, PIXEL * 32 * 2, 1  },
	{ "data\\TEXTURE\\ui_warning_000_belt.png", PIXEL * 32  * 2, PIXEL * 8  * 2, 16 },
	{ "data\\TEXTURE\\ui_warning_000_belt.png", PIXEL * 32  * 2, PIXEL * 8  * 2, 16 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_warning_00Parts_text�֐� - ���i(����)�̍X�V���� -
//========================================
void UpdateUi_warning_00Parts_text(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_TEXT));

	for (int nCntPolygon = 0; nCntPolygon < g_aUi_warning_00Parts[UI_WARNING_00_PARTS_TEXT].nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_WARNING_00_TEXT_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_warning_00Parts[UI_WARNING_00_PARTS_TEXT].fWidth,
			g_aUi_warning_00Parts[UI_WARNING_00_PARTS_TEXT].fHeight,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_warning_00.fAlpha) });
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_warning_00->Unlock();
}

//========================================
// UpdateUi_warning_00Parts_belt_top�֐� - ���i(�я�)�̍X�V���� -
//========================================
void UpdateUi_warning_00Parts_belt_top(void)
{
	Ui_warning_00Parts	*pParts							// ���i�̏��̃|�C���^ 
						= &g_aUi_warning_00Parts		//
						[UI_WARNING_00_PARTS_BELT_TOP];	//
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_BELT_TOP));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			D3DXVECTOR3(
			(UI_WARNING_00_BELT_TOP_POS.x - ((UI_WARNING_00_BELT_SPACE_X * pParts->nPolygonNum) * 0.5f))
				+ (UI_WARNING_00_BELT_SPACE_X * nCntPolygon)
				- (((float)g_ui_warning_00.nCounterBeltMove / (float)UI_WARNING_00_MOVE_TIME) * UI_WARNING_00_BELT_SPACE_X),
				UI_WARNING_00_BELT_TOP_POS.y,
				0.0f),
			D3DXVECTOR3(0.0f,0.0f,D3DX_PI),
			false,
			pParts->fWidth,
			pParts->fHeight,
			ANGLE_TYPE_FREE);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_warning_00.fAlpha) });
	}
}

//========================================
// UpdateUi_warning_00Parts_belt_bottom�֐� - ���i(�щ�)�̍X�V���� -
//========================================
void UpdateUi_warning_00Parts_belt_bottom(void)
{
	Ui_warning_00Parts	*pParts								// ���i�̏��̃|�C���^ 
						= &g_aUi_warning_00Parts			//
						[UI_WARNING_00_PARTS_BELT_BOTTOM];	//
	VERTEX_2D			*pVtx;								// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_BELT_BOTTOM));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			D3DXVECTOR3(
			(UI_WARNING_00_BELT_BOTTOM_POS.x + ((UI_WARNING_00_BELT_SPACE_X * pParts->nPolygonNum) * 0.5f))
				- (UI_WARNING_00_BELT_SPACE_X * nCntPolygon)
				+ (((float)g_ui_warning_00.nCounterBeltMove / (float)UI_WARNING_00_MOVE_TIME) * UI_WARNING_00_BELT_SPACE_X),
				UI_WARNING_00_BELT_BOTTOM_POS.y,
				0.0f),
			D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
			false,
			pParts->fWidth,
			pParts->fHeight,
			ANGLE_TYPE_FREE);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_warning_00.fAlpha) });
	}
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_warning_00(void)
{
	Ui_warning_00	*pUi	// �x��[00] �̏��̃|�C���^ 
					= &g_ui_warning_00;

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_WARNING_00_STATE_POP:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		if (pUi->nCounterState >= UI_WARNING_00_POP_TIME)
		{// ��ԃJ�E���^�[���o�����鎞�ԂɒB�������A
			// ��������Ԃɂ���
			SetStateUi_warning_00(UI_WARNING_00_STATE_IN_CLEAR);
		}
		
		break;
		//========== *** �o���� ***
	case UI_WARNING_00_STATE_IN_POP:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �����x���X�V
		pUi->fAlpha = UI_WARNING_00_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_WARNING_00_IN_POP_TIME);

		if (pUi->nCounterState >= UI_WARNING_00_IN_POP_TIME) 
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// �o����Ԃɂ���
			SetStateUi_warning_00(UI_WARNING_00_STATE_POP);

			// �����x���X�V
			pUi->fAlpha = UI_WARNING_00_ALPHA_MAX;
		}
		
		break;
		//========== *** �����Ă��� ***
	case UI_WARNING_00_STATE_CLEAR:
		
		break;
		//========== *** ������ ***
	case UI_WARNING_00_STATE_IN_CLEAR:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �����x���X�V
		pUi->fAlpha = UI_WARNING_00_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_WARNING_00_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_WARNING_00_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_warning_00(UI_WARNING_00_STATE_CLEAR);

			// �Q�[���̏�Ԃ�ʏ�ɐݒ�
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}

		break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_warning_00(int nParts) 
{
	int nPolygonNum	// �K�v�|���S����
		= 0;		//

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_warning_00Parts[nCntPolygon].nPolygonNum;
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
// InitParameterUi_warning_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_warning_00(void)
{
	g_ui_warning_00.state				= UI_WARNING_00_STATE_CLEAR;	// ���
	g_ui_warning_00.nCounterState		= 0;							// ��ԃJ�E���^�[
	g_ui_warning_00.nCounterBeltMove	= 0;							// �т̈ړ��J�E���^�[
	g_ui_warning_00.fAlpha				= 0.0f;							// �S�̂̓����x
}

//========================================
// InitUi_warning_00�֐� - �x��[00] �̏��������� -
//========================================
void InitUi_warning_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_warning_00();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_warning_00(UI_WARNING_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_warning_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_warning_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aUi_warning_00Parts[nCntParts].aTexturePath, &g_aTextureUi_warning_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_warning_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_warning_00Parts[nCntParts].fWidth,
				g_aUi_warning_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx, 0, 1, 1, NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_warning_00->Unlock();
}

//========================================
// UninitUi_warning_00�֐� - �x��[00] �̏I������ -
//=====================================d===
void UninitUi_warning_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_warning_00[nCntParts] != NULL)
		{
			g_aTextureUi_warning_00[nCntParts]->Release();
			g_aTextureUi_warning_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_warning_00 != NULL) 
	{
		g_pVtxBuffUi_warning_00->Release();
		g_pVtxBuffUi_warning_00 = NULL;
	}
}

//========================================
// UpdateUi_warning_00�֐� - �x��[00] �̍X�V���� -
//========================================
void UpdateUi_warning_00(void) 
{
	Ui_warning_00	*pUi	// �x��[00] �̏��̃|�C���^ 
					= &g_ui_warning_00;

	// ��ԏ���
	StateProcessUi_warning_00();

	if (pUi->state == UI_WARNING_00_STATE_CLEAR) 
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	// �т̈ړ��J�E���^�[���X�V
	pUi->nCounterBeltMove = ++pUi->nCounterBeltMove % UI_WARNING_00_MOVE_TIME;

	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++) 
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** ���� ***
		case UI_WARNING_00_PARTS_TEXT:

			// ���i(����)�̍X�V����
			UpdateUi_warning_00Parts_text();

			break;
			//========== *** �я� ***
		case UI_WARNING_00_PARTS_BELT_TOP:

			// ���i(�я�)�̍X�V����
			UpdateUi_warning_00Parts_belt_top();

			break;
			//========== *** �щ� ***
		case UI_WARNING_00_PARTS_BELT_BOTTOM:

			// ���i(�щ�)�̍X�V����
			UpdateUi_warning_00Parts_belt_bottom();

			break;
		}
	}
}

//========================================
// DrawUi_warning_00�֐� - �x��[00] �̕`�揈�� -
//========================================
void DrawUi_warning_00(void)
{
	Ui_warning_00	*pUi	// �x��[00] �̏��̃|�C���^ 
					= &g_ui_warning_00;

	if (pUi->state == UI_WARNING_00_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_warning_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	// �|���S���ԍ�
		= 0;

	for (int nCntParts = 0; nCntParts < UI_WARNING_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_warning_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_warning_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// �x��[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState�֐� - �x��[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_warning_00(UI_WARNING_00_STATE state) 
{
	Ui_warning_00	*pUi	// �x��[00] �̏��̃|�C���^ 
					= &g_ui_warning_00;

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}