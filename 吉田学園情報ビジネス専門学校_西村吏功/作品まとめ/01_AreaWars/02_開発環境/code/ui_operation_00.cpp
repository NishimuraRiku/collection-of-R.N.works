//========================================
// 
// �������[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_operation_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_operation_00.h"

//****************************************
// �}�N����`
//****************************************

// �������[00] �̏o���ɂ����鎞��
// �������[00] �̏�����̂ɂ����鎞��
// �������[00] �̓����x�̍ő�l
#define UI_OPERATION_00_IN_POP_TIME		(10)
#define UI_OPERATION_00_IN_CLEAR_TIME	(10)
#define UI_OPERATION_00_ALPHA_MAX		(1.0f)

// �������[00] �̃L�[�{�[�h�̐����̊J�n�ʒu
// �������[00] �̃L�[�{�[�h�̐����̏I���ʒu
#define UI_OPERATION_00_KEYBOARD_START_POS	(D3DXVECTOR3((SCREEN_WIDTH * 0.3f), (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))
#define UI_OPERATION_00_KEYBOARD_END_POS	(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - PIXEL * 76, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))

// �������[00] �̃R���g���[���[�̐����̊J�n�ʒu
// �������[00] �̃R���g���[���[�̐����̏I���ʒu
#define UI_OPERATION_00_CONTROLLER_START_POS	(D3DXVECTOR3((SCREEN_WIDTH * 0.7f), (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))
#define UI_OPERATION_00_CONTROLLER_END_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + PIXEL * 76, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 56), 0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************

// �������[00] �̕��i�ԍ�
typedef enum
{
	UI_OPERATION_00_PARTS_KEYBOARD = 0,	// �L�[�{�[�h�̐���
	UI_OPERATION_00_PARTS_CONTROLLER,	// �R���g���[���[�̐���
	UI_OPERATION_00_PARTS_MAX,
}UI_OPERATION_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �������[00] �̏��\����
typedef struct 
{
	UI_OPERATION_00_STATE	state;				// ���
	int						nCounterState;		// ��ԃJ�E���^�[
	float					fAlpha;				// �S�̂̓����x
}Ui_operation_00;

// �������[00] �̃|���S�����̏��\����
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

	D3DXVECTOR3		pos;			// �ʒu
	int				nPtn;			// �p�^�[��No.
	int				nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
}Ui_operation_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterUi_operation_00(void);

// ���i(�L�[�{�[�h�̐���)�̍X�V����
void UpdateUi_operation_00Parts_keyboard(void);

// ���i(�R���g���[���[�̐���)�̍X�V����
void UpdateUi_operation_00Parts_controller(void);

// ��ԏ���
void StateProcessUi_operation_00(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_operation_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_operation_00	// �e�N�X�`���ւ̃|�C���^
						[UI_OPERATION_00_PARTS_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_operation_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;						//
Ui_operation_00			g_ui_operation_00;			// �������[00] �̏��

// �������[00] �̕��i���̏��
Ui_operation_00Parts g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_operation_000_keyboard.png"   , 4 , 4 , 0 , 15 , 10 , PIXEL * 256 , PIXEL * 256 , 1 },
	{ "data\\TEXTURE\\ui_operation_000_controller.png" , 4 , 4 , 0 , 15 , 10 , PIXEL * 256 , PIXEL * 256 , 1 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_operation_00Parts_keyboard�֐� - ���i(�L�[�{�[�h�̐���)�̍X�V���� -
//========================================
void UpdateUi_operation_00Parts_keyboard(void)
{
	Ui_operation_00Parts	*pParts								// ���i�̏��̃|�C���^ 
							= &g_aUi_operation_00Parts			//
							[UI_OPERATION_00_PARTS_KEYBOARD];	//
	VERTEX_2D				*pVtx;								// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_operation_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_operation_00(UI_OPERATION_00_PARTS_KEYBOARD));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].fWidth,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtn,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMin,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMax,
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nCounterAnim,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMaxX,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].nPtnMaxY,
			NULL);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_operation_00.fAlpha) });
	}
}

//========================================
// UpdateUi_operation_00Parts_controller�֐� - ���i(�R���g���[���[)�̍X�V���� -
//========================================
void UpdateUi_operation_00Parts_controller(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_operation_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_operation_00(UI_OPERATION_00_PARTS_CONTROLLER));

	for (int nCntPolygon = 0; nCntPolygon < g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].fWidth,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtn,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMin,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMax,
				&g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nCounterAnim,
				g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMaxX,
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].nPtnMaxY,
			NULL);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_operation_00.fAlpha) });
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_operation_00->Unlock();
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_operation_00(void)
{
	Ui_operation_00	*pUi	// �������[00] �̏��̃|�C���^ 
					= &g_ui_operation_00;

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_OPERATION_00_STATE_POP:

		break;
		//========== *** �o���� ***
	case UI_OPERATION_00_STATE_IN_POP:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �L�[�{�[�h�̐����̈ʒu���X�V
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].pos
			= (UI_OPERATION_00_KEYBOARD_START_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME)))
			+ (UI_OPERATION_00_KEYBOARD_END_POS * ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME));

		// �R���g���[���[�̐����̈ʒu���X�V
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].pos
			= (UI_OPERATION_00_CONTROLLER_START_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME)))
			+ (UI_OPERATION_00_CONTROLLER_END_POS * ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_POP_TIME));

		// �����x���X�V
		pUi->fAlpha = UI_OPERATION_00_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_OPERATION_00_IN_POP_TIME);

		if (pUi->nCounterState >= UI_OPERATION_00_IN_POP_TIME) 
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// �o����Ԃɂ���
			SetStateUi_operation_00(UI_OPERATION_00_STATE_POP);

			// �����x���X�V
			pUi->fAlpha = UI_OPERATION_00_ALPHA_MAX;

			// ���nSE[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_LANDING_000);
		}
		
		break;
		//========== *** �����Ă��� ***
	case UI_OPERATION_00_STATE_CLEAR:
		
		break;
		//========== *** ������ ***
	case UI_OPERATION_00_STATE_IN_CLEAR:

		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �L�[�{�[�h�̐����̈ʒu���X�V
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_KEYBOARD].pos
			= (UI_OPERATION_00_KEYBOARD_START_POS * ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME))
			+ (UI_OPERATION_00_KEYBOARD_END_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME)));

		// �R���g���[���[�̐����̈ʒu���X�V
		g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_CONTROLLER].pos
			= (UI_OPERATION_00_CONTROLLER_START_POS * (((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME)))
			+ (UI_OPERATION_00_CONTROLLER_END_POS * (1.0f - ((float)g_ui_operation_00.nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME)));

		// �����x���X�V
		pUi->fAlpha = UI_OPERATION_00_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_OPERATION_00_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_OPERATION_00_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_operation_00(UI_OPERATION_00_STATE_CLEAR);

			// �Q�[���̏�Ԃ�ʏ�ɐݒ�
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}

		break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_operation_00(int nParts) 
{
	int nPolygonNum	// �K�v�|���S����
		= 0;		//

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_operation_00Parts[nCntPolygon].nPolygonNum;
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
// InitParameterUi_operation_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_operation_00(void)
{
	g_ui_operation_00.state				= UI_OPERATION_00_STATE_CLEAR;	// ���
	g_ui_operation_00.nCounterState		= 0;							// ��ԃJ�E���^�[
	g_ui_operation_00.fAlpha			= 0.0f;							// �S�̂̓����x
}

//========================================
// InitUi_operation_00�֐� - �������[00] �̏��������� -
//========================================
void InitUi_operation_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_operation_00();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_operation_00(UI_OPERATION_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_operation_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_operation_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aUi_operation_00Parts[nCntParts].aTexturePath, &g_aTextureUi_operation_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_operation_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_operation_00Parts[nCntParts].fWidth,
				g_aUi_operation_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx, 
				0, 
				g_aUi_operation_00Parts[nCntParts].nPtnMaxX, 
				g_aUi_operation_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_operation_00->Unlock();
}

//========================================
// UninitUi_operation_00�֐� - �������[00] �̏I������ -
//========================================
void UninitUi_operation_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_operation_00[nCntParts] != NULL)
		{
			g_aTextureUi_operation_00[nCntParts]->Release();
			g_aTextureUi_operation_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_operation_00 != NULL) 
	{
		g_pVtxBuffUi_operation_00->Release();
		g_pVtxBuffUi_operation_00 = NULL;
	}
}

//========================================
// UpdateUi_operation_00�֐� - �������[00] �̍X�V���� -
//========================================
void UpdateUi_operation_00(void) 
{
	Ui_operation_00	*pUi	// �������[00] �̏��̃|�C���^ 
					= &g_ui_operation_00;

	// ��ԏ���
	StateProcessUi_operation_00();

	if (pUi->state == UI_OPERATION_00_STATE_CLEAR) 
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++) 
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** �L�[�{�[�h�̐��� ***
		case UI_OPERATION_00_PARTS_KEYBOARD:

			// ���i(�L�[�{�[�h�̐���)�̍X�V����
			UpdateUi_operation_00Parts_keyboard();

			break;
			//========== *** �R���g���[���[�̐��� ***
		case UI_OPERATION_00_PARTS_CONTROLLER:

			// ���i(�R���g���[���[�̐���)�̍X�V����
			UpdateUi_operation_00Parts_controller();

			break;
		}
	}
}

//========================================
// DrawUi_operation_00�֐� - �������[00] �̕`�揈�� -
//========================================
void DrawUi_operation_00(void)
{
	Ui_operation_00	*pUi	// �������[00] �̏��̃|�C���^ 
					= &g_ui_operation_00;

	if (pUi->state == UI_OPERATION_00_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_operation_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	// �|���S���ԍ�
		= 0;

	for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_operation_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_operation_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// �������[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState�֐� - �������[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_operation_00(UI_OPERATION_00_STATE state) 
{
	Ui_operation_00	*pUi	// �������[00] �̏��̃|�C���^ 
					= &g_ui_operation_00;

	switch (state)
	{
		//========== *** �o�� ***
	case UI_OPERATION_00_STATE_POP:

		break;
		//========== *** �o���� ***
	case UI_OPERATION_00_STATE_IN_POP:
		
		// �S�Ă̕��i�̃A�j���[�V�����̃J�E���^�[��������
		for (int nCntParts = 0; nCntParts < UI_OPERATION_00_PARTS_MAX; nCntParts++)
		{
			g_aUi_operation_00Parts[UI_OPERATION_00_PARTS_MAX].nCounterAnim = 0;
		}
		
		break;
		//========== *** �����Ă��� ***
	case UI_OPERATION_00_STATE_CLEAR:

		break;
		//========== *** ������ ***
	case UI_OPERATION_00_STATE_IN_CLEAR:

		break;
	}

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}