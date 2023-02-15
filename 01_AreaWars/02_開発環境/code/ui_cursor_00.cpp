//========================================
// 
// �J�[�\��[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_cursor_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "setting.h"
#include "polygon.h"
#include "md_title_00.h"
#include "ui_cursor_00.h"

//****************************************
// �}�N���̒�`
//****************************************

// �J�[�\��[00] �̔�N���b�N���̃p�^�[��No.
// �J�[�\��[00] �̃N���b�N���̃p�^�[��No.
#define UI_CURSOR_00_NOT_CLICK_PATTERN	(0)
#define UI_CURSOR_00_CLICK_PATTERN		(1)

//****************************************
// �񋓌^�̒�`
//****************************************

// �J�[�\��[00] �̕��i�ԍ�
typedef enum
{
	UI_CURSOR_00_PARTS_CURSOR = 0,	// �J�[�\��
	UI_CURSOR_00_PARTS_MAX,
}UI_CURSOR_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �J�[�\��[00] �̃|���S�����̏��\����
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

	int				nPtn;			// �p�^�[��No.
	int				nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
}Ui_cursor_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���i(�J�[�\��)�̍X�V����
void UpdateUi_cursor_00Parts_cursor(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_cursor_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_cursor_00		// �e�N�X�`���ւ̃|�C���^
						[UI_CURSOR_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_cursor_00		// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;						//
Ui_cursor_00			g_ui_cursor_00;				// �J�[�\��[00] �̏��

// �J�[�\��[00] �̕��i���̏��
Ui_cursor_00Parts g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_cursor_000.png" , 2 , 1 , 0 , 0 , -1 , PIXEL * 16 , PIXEL * 16 , 1 },
};

//========== *** �J�[�\��[00] �����擾 ***
Ui_cursor_00 *GetUi_cursor_00(void)
{
	return &g_ui_cursor_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_cursor_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_cursor_00(void) 
{
	
}

//========================================
// UpdateUi_cursor_00Parts_cursor�֐� - ���i(�J�[�\��)�̍X�V���� -
//========================================
void UpdateUi_cursor_00Parts_cursor(void)
{
	Ui_cursor_00Parts	*pParts							// 
						= &g_aUi_cursor_00Parts			//
						[UI_CURSOR_00_PARTS_CURSOR];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_cursor_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_cursor_00(UI_CURSOR_00_PARTS_CURSOR));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			GetCursorPosOnScreen(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].fWidth,
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].fHeight,
			ANGLE_TYPE_FIXED);

		if ((GetMousePress(MOUSE_LEFT))
			||
			(GetMousePress(MOUSE_RIGHT))) 
		{// �N���b�N���A
			// �p�^�[��No.���N���b�N���̂��̂ɂ���
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtn = UI_CURSOR_00_CLICK_PATTERN;
		}
		else 
		{// ��N���b�N���A
			// �p�^�[��No.���N���b�N���̂��̂ɂ���
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtn = UI_CURSOR_00_NOT_CLICK_PATTERN;
		}

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtn,
				g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMin,
				g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMax,
				&g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nCounterAnim,
				g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMaxX,
			g_aUi_cursor_00Parts[UI_CURSOR_00_PARTS_CURSOR].nPtnMaxY,
			NULL);
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_cursor_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// �K�v�|���S����

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_cursor_00Parts[nCntPolygon].nPolygonNum;
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
// InitUi_cursor_00�֐� - �J�[�\��[00] �̏��������� -
//========================================
void InitUi_cursor_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_cursor_00(UI_CURSOR_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_cursor_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_cursor_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aUi_cursor_00Parts[nCntParts].aTexturePath, &g_aTextureUi_cursor_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_cursor_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_cursor_00Parts[nCntParts].fWidth,
				g_aUi_cursor_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FIXED);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx,
				0,
				g_aUi_cursor_00Parts[nCntParts].nPtnMaxX,
				g_aUi_cursor_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_cursor_00->Unlock();
}

//========================================
// UninitUi_cursor_00�֐� - �J�[�\��[00] �̏I������ -
//========================================
void UninitUi_cursor_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_cursor_00[nCntParts] != NULL)
		{
			g_aTextureUi_cursor_00[nCntParts]->Release();
			g_aTextureUi_cursor_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_cursor_00 != NULL)
	{
		g_pVtxBuffUi_cursor_00->Release();
		g_pVtxBuffUi_cursor_00 = NULL;
	}
}

//========================================
// UpdateUi_cursor_00�֐� - �J�[�\��[00] �̍X�V���� -
//========================================
void UpdateUi_cursor_00(void)
{
	Ui_cursor_00	*pUi				//
					= &g_ui_cursor_00;	// �J�[�\��[00] �̏��̃|�C���^

	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
		case UI_CURSOR_00_PARTS_CURSOR:UpdateUi_cursor_00Parts_cursor(); break; // ���i(�J�[�\��)�̍X�V����
		}
	}
}

//========================================
// DrawUi_cursor_00�֐� - �J�[�\��[00] �̕`�揈�� -
//========================================
void DrawUi_cursor_00(void)
{
	if ((GetActiveInputType() != ACTIVE_INPUT_TYPE_KEYBOARD) ||
		(!GetSetting()->bFullScreen)) 
	{// ���I�ȓ��͂̎�ނ��L�[�{�[�h�Ŗ��� or �S��ʃt���O���^�̎��A
		// �������I��
		return;
	}

	Ui_cursor_00	*pUi				//
					= &g_ui_cursor_00;	// �J�[�\��[00] �̏��̃|�C���^

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_cursor_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < UI_CURSOR_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_cursor_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_cursor_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// �J�[�\��[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}