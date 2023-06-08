//========================================
// 
// �`�[�����S[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_team_logo_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_team_logo_00.h"

//****************************************
// �}�N����`
//****************************************

// �`�[�����S[00] �̏o���ɂ����鎞��
// �`�[�����S[00] �̏o�����鎞��
// �`�[�����S[00] �̏�����̂ɂ����鎞��
// �`�[�����S[00] �̓����x�̍ő�l
#define UI_TEAM_LOGO_00_IN_POP_TIME		(40)
#define UI_TEAM_LOGO_00_POP_TIME		(80)
#define UI_TEAM_LOGO_00_IN_CLEAR_TIME	(40)
#define UI_TEAM_LOGO_00_ALPHA_MAX		(1.0f)

// �`�[�����S[00] �̈ʒu
#define UI_TEAM_LOGO_00_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************

// �`�[�����S[00] �̕��i�ԍ�
typedef enum
{
	UI_TEAM_LOGO_00_PARTS_TEAM_LOGO = 0,		// �`�[�����S
	UI_TEAM_LOGO_00_PARTS_MAX,
}UI_TEAM_LOGO_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �`�[�����S[00] �̃|���S�����̏��\����
typedef struct
{
	char	aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
			[TXT_MAX];		// 
	float	fWidth;			// ��
	float	fHeight;		// ����
	int		nPolygonNum;	// �K�v�|���S����
}Ui_team_logo_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterUi_team_logo_00(void);

// ���i(�`�[�����S)�̍X�V����
void UpdateUi_team_logo_00Parts_team_logo(void);

// ��ԏ���
void StateProcessUi_team_logo_00(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_team_logo_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_team_logo_00	// �e�N�X�`���ւ̃|�C���^
						[UI_TEAM_LOGO_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_team_logo_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;						//
Ui_team_logo_00			g_ui_team_logo_00;			// �`�[�����S[00] �̏��

// �`�[�����S[00] �̕��i���̏��
Ui_team_logo_00Parts g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\tmlg_STUDIO.D0DO.png", PIXEL * 512, PIXEL * 128, 1 },
};

//========== *** �`�[�����S[00] �̏����擾 ***
Ui_team_logo_00 *GetUi_team_logo_00(void) 
{
	return &g_ui_team_logo_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_team_logo_00Parts_team_logo�֐� - ���i(�`�[�����S)�̍X�V���� -
//========================================
void UpdateUi_team_logo_00Parts_team_logo(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_team_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_team_logo_00(UI_TEAM_LOGO_00_PARTS_TEAM_LOGO));

	for (int nCntPolygon = 0; nCntPolygon < g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_TEAM_LOGO].nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			UI_TEAM_LOGO_00_POS,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_TEAM_LOGO].fWidth,
			g_aUi_team_logo_00Parts[UI_TEAM_LOGO_00_PARTS_TEAM_LOGO].fHeight,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_team_logo_00.fAlpha) });
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_team_logo_00->Unlock();
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_team_logo_00(void)
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// �`�[�����S[00] �̏��̃|�C���^ 

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_TEAM_LOGO_00_STATE_POP:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		if (pUi->nCounterState >= UI_TEAM_LOGO_00_POP_TIME)
		{// ��ԃJ�E���^�[���o�����鎞�ԂɒB�������A
			// ��������Ԃɂ���
			SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_IN_CLEAR);
		}
		
		break;
		//========== *** �o���� ***
	case UI_TEAM_LOGO_00_STATE_IN_POP:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �����x���X�V
		pUi->fAlpha = UI_TEAM_LOGO_00_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_TEAM_LOGO_00_IN_POP_TIME);

		if (pUi->nCounterState >= UI_TEAM_LOGO_00_IN_POP_TIME) 
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// �o����Ԃɂ���
			SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_POP);

			// �����x���X�V
			pUi->fAlpha = UI_TEAM_LOGO_00_ALPHA_MAX;
		}
		
		break;
		//========== *** �����Ă��� ***
	case UI_TEAM_LOGO_00_STATE_CLEAR:
		
		break;
		//========== *** ������ ***
	case UI_TEAM_LOGO_00_STATE_IN_CLEAR:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �����x���X�V
		pUi->fAlpha = UI_TEAM_LOGO_00_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_TEAM_LOGO_00_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_TEAM_LOGO_00_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE_CLEAR);

			// �Q�[���̏�Ԃ�ʏ�ɐݒ�
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}

		break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_team_logo_00(int nParts) 
{
	int nPolygonNum	// �K�v�|���S����
		= 0;		//

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_team_logo_00Parts[nCntPolygon].nPolygonNum;
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
// InitParameterUi_team_logo_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_team_logo_00(void)
{
	g_ui_team_logo_00.state				= UI_TEAM_LOGO_00_STATE_CLEAR;	// ���
	g_ui_team_logo_00.nCounterState		= 0;							// ��ԃJ�E���^�[
	g_ui_team_logo_00.fAlpha			= 0.0f;							// �S�̂̓����x
}

//========================================
// InitUi_team_logo_00�֐� - �`�[�����S[00] �̏��������� -
//========================================
void InitUi_team_logo_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_team_logo_00();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_team_logo_00(UI_TEAM_LOGO_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_team_logo_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_team_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aUi_team_logo_00Parts[nCntParts].aTexturePath, &g_aTextureUi_team_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_team_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_team_logo_00Parts[nCntParts].fWidth,
				g_aUi_team_logo_00Parts[nCntParts].fHeight,
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
	g_pVtxBuffUi_team_logo_00->Unlock();
}

//========================================
// UninitUi_team_logo_00�֐� - �`�[�����S[00] �̏I������ -
//=====================================d===
void UninitUi_team_logo_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_team_logo_00[nCntParts] != NULL)
		{
			g_aTextureUi_team_logo_00[nCntParts]->Release();
			g_aTextureUi_team_logo_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_team_logo_00 != NULL) 
	{
		g_pVtxBuffUi_team_logo_00->Release();
		g_pVtxBuffUi_team_logo_00 = NULL;
	}
}

//========================================
// UpdateUi_team_logo_00�֐� - �`�[�����S[00] �̍X�V���� -
//========================================
void UpdateUi_team_logo_00(void) 
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// �`�[�����S[00] �̏��̃|�C���^ 

	// ��ԏ���
	StateProcessUi_team_logo_00();

	if (pUi->state == UI_TEAM_LOGO_00_STATE_CLEAR) 
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++) 
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** �`�[�����S ***
		case UI_TEAM_LOGO_00_PARTS_TEAM_LOGO:

			// ���i(�`�[�����S)�̍X�V����
			UpdateUi_team_logo_00Parts_team_logo();

			break;
		}
	}
}

//========================================
// DrawUi_team_logo_00�֐� - �`�[�����S[00] �̕`�揈�� -
//========================================
void DrawUi_team_logo_00(void)
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// �`�[�����S[00] �̏��̃|�C���^ 

	if (pUi->state == UI_TEAM_LOGO_00_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_team_logo_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	// �|���S���ԍ�
		= 0;

	for (int nCntParts = 0; nCntParts < UI_TEAM_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_team_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_team_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// �`�[�����S[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState�֐� - �`�[�����S[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_team_logo_00(UI_TEAM_LOGO_00_STATE state) 
{
	Ui_team_logo_00	*pUi					//
					= &g_ui_team_logo_00;	// �`�[�����S[00] �̏��̃|�C���^ 

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}