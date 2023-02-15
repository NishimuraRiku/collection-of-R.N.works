//========================================
// 
// ���j���[[00]�̏���
// Author:���� ����
// 
//========================================
//  *** ui_menu_00.cpp ***
//========================================
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "text.h"
#include "ui_menu_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// �}�N����`
//****************************************
#define UI_MENU_00_MAX						(16)						// ���j���[[00] �̍ő吔
#define UI_MENU_00_WIDTH					(PIXEL * 256)				// ���j���[[00] �̕�
#define UI_MENU_00_HEIGHT					(PIXEL * 16)				// ���j���[[00] �̍���
#define UI_MENU_00_TEXTURE_X_PATTERN_MAX	(1)							// ���j���[[00] �̃e�N�X�`���̃p�^�[����(��)
#define UI_MENU_00_TEXTURE_Y_PATTERN_MAX	(1)							// ���j���[[00] �̃e�N�X�`���̃p�^�[����(�c)
#define UI_MENU_00_SELECT_COLOR				(Color{243, 189, 63, 255})	// ���j���[[00] �̑I�����̐F

#define UI_MENU_00_POP_TIME					(10)						// ���j���[[00] �̏o���ɂ����鎞��
#define UI_MENU_00_POP_SCALE				(ScaleDiameter{1.0f, 1.0f})	// ���j���[[00] �̏o�����Ă��鎞�̑傫���ɂ�����{��

#define UI_MENU_00_CLEAR_SCALE				(ScaleDiameter{1.0f, 0.0f})	// ���j���[[00] �̏����Ă��鎞�̑傫���ɂ�����{��

//****************************************
// �\���̂̒�`
//****************************************

// ���j���[�̗\����
typedef struct 
{
	bool bReservation;
	Ui_menu_00_set *set;
	int nMax; 
	float fSpaceX; 
	float fSpaceY;
}MenuReservation;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void StateProcessUi_menu_00(void);	// ���j���[[00] �̏�ԏ���
void CreateUi_menu_00(void);		// ���j���[[00] �̍쐬����

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_menu_00[UI_MENU_00_TYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_menu_00 = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Ui_menu_00				g_aUi_menu_00[UI_MENU_00_MAX];					// ���j���[[00] �̏��
int						g_nUi_menu_00Num;								// ���j���[[00] �̌��݂̎g�p��
int						g_nSelect;										// ���݂̑I��ԍ�
D3DXVECTOR3				g_centerPos;									// ���S���W
Ui_menu_00Control		g_control;										// �S�̊Ǘ�
MenuReservation			g_menuReservation;								// ���j���[�̗\����

// ���j���[[00] �̃e�N�X�`���t�@�C���̑��΃p�X
char g_aTexturePath[UI_MENU_00_TYPE_MAX][TXT_MAX] =
{
	"data\\TEXTURE\\ui_menu_000.png",
	"data\\TEXTURE\\ui_menu_000_select.png",
};

//========== *** ���j���[[00] �̏����擾 ***
Ui_menu_00 *GetUi_menu_00(void) 
{
	return &g_aUi_menu_00[0];
}

//========== *** ���j���[[00] �̑S�̊Ǘ��̏����擾 ***
Ui_menu_00Control *GetUi_menu_00Control(void) 
{
	return &g_control;
}

//========== *** ���j���[[00] �̌��݂̑I��ԍ����擾 ***
int *GetSelect(void) 
{
	return &g_nSelect;
}

//========================================
// CreateUi_menu_00�֐� - ���j���[[00] �̍쐬���� -
//========================================
void CreateUi_menu_00(void)
{
	if ((!g_menuReservation.bReservation)
		|| (g_control.state != UI_MENU_00_STATE_NONE))
	{// ���j���[�̗\�񂪖����A�������͏�Ԃ������łȂ����A
		// �������I������
		return;
	}

	// �\��t���O���U�ɂ���
	g_menuReservation.bReservation = false;

	// ���j���[[00] ���o�����ɂ���
	g_control.state = UI_MENU_00_STATE_IN_POP;
	g_control.nCounterState = UI_MENU_00_POP_TIME;

	// ���݂̑I��ԍ�������������
	g_nSelect = 0;

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

								// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���݂̎g�p������
	g_nUi_menu_00Num = g_menuReservation.nMax;

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < g_menuReservation.nMax; nCntUi_menu_00++, g_menuReservation.set++, pVtx += 4)
	{
		// ��ނ���
		g_aUi_menu_00[nCntUi_menu_00].type = g_menuReservation.set->type;

		// ���������
		sprintf(g_aUi_menu_00[nCntUi_menu_00].aString, g_menuReservation.set->aString);

		// �g�p����Ă����Ԃɂ���
		g_aUi_menu_00[nCntUi_menu_00].bUse = true;

		// �I������Ă��Ȃ���Ԃɂ���
		g_aUi_menu_00[nCntUi_menu_00].bSelect = false;

		// �ʒu��ݒ�
		g_aUi_menu_00[nCntUi_menu_00].pos = D3DXVECTOR3(g_centerPos.x - (((g_menuReservation.nMax - 1) * g_menuReservation.fSpaceX) * 0.5f) + (nCntUi_menu_00 * g_menuReservation.fSpaceX),
			g_centerPos.y - (((g_menuReservation.nMax - 1) * g_menuReservation.fSpaceY) * 0.5f) + (nCntUi_menu_00 * g_menuReservation.fSpaceY),
			0.0f);

		{
			D3DXVECTOR3 pos = g_aUi_menu_00[nCntUi_menu_00].pos;						// �ʒu
			D3DXVECTOR3 rot = g_aUi_menu_00[nCntUi_menu_00].rot;						// ����
			float fWidth = g_aUi_menu_00[nCntUi_menu_00].fWidth;						// ��
			float fHeight = g_aUi_menu_00[nCntUi_menu_00].fHeight;					// ����
			float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle = atan2f(fWidth, fHeight);									// �Ίp���̊p�x

																					// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// InitUi_menu_00�֐� - ���j���[[00] �̏��������� -
//========================================
void InitUi_menu_00(void)
{
	// ���j���[�̗\��t���O��������
	g_menuReservation.bReservation = false;

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	
	// ���S���W��������
	g_centerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���݂̎g�p����������
	g_nUi_menu_00Num = 0;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntUi_menu_00_type = 0; nCntUi_menu_00_type < UI_MENU_00_TYPE_MAX; nCntUi_menu_00_type++) 
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aTexturePath[nCntUi_menu_00_type], &g_aTextureUi_menu_00[nCntUi_menu_00_type]);
	}
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UI_MENU_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_menu_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pVtx += 4)
	{
		g_aUi_menu_00[nCntUi_menu_00].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu��������
		g_aUi_menu_00[nCntUi_menu_00].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ������������
		g_aUi_menu_00[nCntUi_menu_00].fWidth	= UI_MENU_00_WIDTH;					// ����������
		g_aUi_menu_00[nCntUi_menu_00].fHeight	= UI_MENU_00_HEIGHT;				// ������������
		g_aUi_menu_00[nCntUi_menu_00].nPattern	= 0;								// �p�^�[��No.��������
		g_aUi_menu_00[nCntUi_menu_00].bUse		= false;							// �g�p����Ă��Ȃ���Ԃɂ���
		g_aUi_menu_00[nCntUi_menu_00].bSelect	= false;							// �I������Ă��Ȃ���Ԃɂ���

		{
			D3DXVECTOR3 pos = g_aUi_menu_00[nCntUi_menu_00].pos;						// �ʒu
			D3DXVECTOR3 rot = g_aUi_menu_00[nCntUi_menu_00].rot;						// ����
			float fWidth	= g_aUi_menu_00[nCntUi_menu_00].fWidth;						// ��
			float fHeight	= g_aUi_menu_00[nCntUi_menu_00].fHeight;					// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		{
			int nPtn		= g_aUi_menu_00[nCntUi_menu_00].nPattern;	// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX	= UI_MENU_00_TEXTURE_X_PATTERN_MAX;			// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY	= UI_MENU_00_TEXTURE_Y_PATTERN_MAX;			// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// UninitUi_menu_00�֐� - ���j���[[00] �̏I������ -
//========================================
void UninitUi_menu_00(void)
{
	for (int nCntUi_menu_00_type = 0; nCntUi_menu_00_type < UI_MENU_00_TYPE_MAX; nCntUi_menu_00_type++)
	{
		// �e�N�X�`���̔j��
		if (g_aTextureUi_menu_00[nCntUi_menu_00_type] != NULL)
		{
			g_aTextureUi_menu_00[nCntUi_menu_00_type]->Release();
			g_aTextureUi_menu_00[nCntUi_menu_00_type] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_menu_00 != NULL) 
	{
		g_pVtxBuffUi_menu_00->Release();
		g_pVtxBuffUi_menu_00 = NULL;
	}
}

//========================================
// UpdateUi_menu_00�֐� - ���j���[[00] �̍X�V���� -
//========================================
void UpdateUi_menu_00(void)
{
	// ��ԏ���
	StateProcessUi_menu_00();

	// �쐬����
	CreateUi_menu_00();

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	Ui_menu_00 *pUi
		= g_aUi_menu_00;		// ���j���[[00] �̏��̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_menu_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pVtx += 4, pUi++)
	{
		if (!pUi->bUse)
		{// �g�p����Ă��Ȃ����A
			// �J��Ԃ����������Z�b�g
			continue;
		}

		{
			float fWidth	= pUi->fWidth * g_control.scale.fWidth;						// ��
			float fHeight	= pUi->fHeight * g_control.scale.fHeight;					// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// ���_���W��ݒ�
			pVtx[0].pos.x = pUi->pos.x + sinf(pUi->rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pUi->pos.y + cosf(pUi->rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pUi->pos.x + sinf(pUi->rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pUi->pos.y + cosf(pUi->rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pUi->pos.x + sinf(pUi->rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pUi->pos.y + cosf(pUi->rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pUi->pos.x + sinf(pUi->rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pUi->pos.y + cosf(pUi->rot.z + fAngle) * fLength;
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		{
			Color col = { 255, 255, 255, 255 };	// �F

			if (nCntUi_menu_00 == g_nSelect) 
			{// �J�E���g���I��ԍ��ƈ�v���Ă������A
				// �F��I�����̐F�ɂ���
				col = UI_MENU_00_SELECT_COLOR;
			}

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
			pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
			pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
			pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);

			// �e�L�X�g[���j���[] ��\��
			ShowTxt_00(TEXT_FIELD_MENU, nCntUi_menu_00, true);

			// �e�L�X�g[���j���[] �̍X�V����
			UpdateTxt_00(TEXT_FIELD_MENU, nCntUi_menu_00, pUi->aString, pUi->pos, col, TEXT_MODE_CENTER, FONT_000, g_control.scale.fWidth, g_control.scale.fHeight);
		}

		{
			int nPtn = pUi->nPattern;	// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX = UI_MENU_00_TEXTURE_X_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY = UI_MENU_00_TEXTURE_Y_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_menu_00->Unlock();
}

//========================================
// StateProcessUi_menu_00�֐� - ���j���[[00] �̏�ԏ��� -
//========================================
void StateProcessUi_menu_00(void)
{
	switch (g_control.state)
	{
		//========== *** �o���� ***
	case UI_MENU_00_STATE_IN_POP:

		// ��Ԃ̃J�E���^�[�����Z
		g_control.nCounterState--;

		if (g_control.nCounterState <= 0) 
		{// ��Ԃ̃J�E���^�[��0�ȉ��̎��A
			// �o����Ԃɂ���
			g_control.state = UI_MENU_00_STATE_POP;
		}

		// �傫���ɂ�����{�����X�V
		g_control.scale.fWidth = (UI_MENU_00_POP_SCALE.fWidth * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME)))
			+ (UI_MENU_00_CLEAR_SCALE.fWidth * ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME));
		g_control.scale.fHeight = (UI_MENU_00_POP_SCALE.fHeight * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME)))
			+ (UI_MENU_00_CLEAR_SCALE.fHeight * ((float)g_control.nCounterState / (float)UI_MENU_00_POP_TIME));

		break;
		//========== *** �o�� ***
	case UI_MENU_00_STATE_POP:

		break;
		//========== *** ������ ***
	case UI_MENU_00_STATE_IN_CLEAR:

		// ��Ԃ̃J�E���^�[�����Z
		g_control.nCounterState--;

		if (g_control.nCounterState <= 0)
		{// ��Ԃ̃J�E���^�[��0�ȉ��̎��A
			// ��Ԃ𖳂��ɂ���
			g_control.state = UI_MENU_00_STATE_NONE;

			Ui_menu_00 *pUi
				= g_aUi_menu_00;	// ���j���[[00] �̏��̃|�C���^

			for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++, pUi++)
			{
				// �g�p���Ă��Ȃ���Ԃɂ���
				pUi->bUse = false;

				// �e�L�X�g[���j���[] ���\��
				ShowTxt_00(TEXT_FIELD_MENU, nCntUi_menu_00, false);
			}
		}

		// �傫���ɂ�����{�����X�V
		g_control.scale.fWidth = (UI_MENU_00_CLEAR_SCALE.fWidth * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME)))
			+ (UI_MENU_00_POP_SCALE.fWidth * ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME));
		g_control.scale.fHeight = (UI_MENU_00_CLEAR_SCALE.fHeight * (1.0f - ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME)))
			+ (UI_MENU_00_POP_SCALE.fHeight * ((float)g_control.nCounterState / (float)UI_MENU_00_CLEAR_TIME));

		break;
	}
}

//========================================
// Ui_menu_00Input�֐� - ���j���[[00] �̓��͏��� -
//========================================
int Ui_menu_00Input(UI_MENU_00_INPUT_MODE mode)
{
	int nSelected = -1;	// �I�������ԍ�(-1�͑I�𖳂�)

	if ((g_control.state != UI_MENU_00_STATE_POP)
		||(GetFade() != FADE_NONE))
	{// �o����ԂłȂ����A�������̓t�F�[�h���̎��A
		// �������I������
		return nSelected;
	}

	switch (mode)
	{
	//========== *** �㉺ ***
	case UI_MENU_00_INPUT_MODE_UP_AND_DOWN:

		if ((GetKeyboardRepeat(DIK_UP)) 
			|| (GetKeyboardRepeat(DIK_W))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
			|| (GetButtonRepeat(BUTTON_UP)))
		{// ��L�[�������ꂽ���A
			// ���݂̑I��ԍ������Z
			g_nSelect--;

			if (g_menuReservation.nMax != 1)
			{// ���j���[�̐���1�Ŗ������A
			 // ���ʉ�[�I��] ���Đ�
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}
		else if ((GetKeyboardRepeat(DIK_DOWN))
			|| (GetKeyboardRepeat(DIK_S))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
			|| (GetButtonRepeat(BUTTON_DOWN)))
		{// ���L�[�������ꂽ���A
			// ���݂̑I��ԍ������Z
			g_nSelect++;

			if (g_menuReservation.nMax != 1)
			{// ���j���[�̐���1�Ŗ������A
			 // ���ʉ�[�I��] ���Đ�
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}

		break;
	//========== *** ���E ***
	case UI_MENU_00_INPUT_MODE_LEFT_AND_RIGHT:

		if ((GetKeyboardRepeat(DIK_LEFT)) 
			|| (GetKeyboardRepeat(DIK_A))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetButtonRepeat(BUTTON_LEFT)))
		{// ���L�[�������ꂽ���A
			// ���݂̑I��ԍ������Z
			g_nSelect--;

			if (g_menuReservation.nMax != 1)
			{// ���j���[�̐���1�Ŗ������A
				// ���ʉ�[�I��] ���Đ�
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}
		else if ((GetKeyboardRepeat(DIK_RIGHT)) 
			|| (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// �E�L�[�������ꂽ���A
			// ���݂̑I��ԍ������Z
			g_nSelect++;

			if (g_menuReservation.nMax != 1)
			{// ���j���[�̐���1�Ŗ������A
				// ���ʉ�[�I��] ���Đ�
				PlaySound(SOUND_LABEL_SE_SELECT_000);
			}
		}

		break;
	}

	if (g_nUi_menu_00Num > 0)
	{// ���ݎg�p����0�������Ă��鎞�A
		// �I��ԍ��𐧌�
		g_nSelect %= g_nUi_menu_00Num;
	}

	if (g_nSelect < 0) 
	{// ���݂̑I��ԍ���0��������Ă��鎞

		g_nSelect = g_nUi_menu_00Num - 1;
	}

	if (g_aUi_menu_00[g_nSelect].type == UI_MENU_00_TYPE_SELECTION)
	{// ��ނ��I����������

		if ((GetKeyboardRepeat(DIK_LEFT)) || (GetKeyboardRepeat(DIK_A))
			|| (GetKeyboardRepeat(DIK_RIGHT)) || (GetKeyboardRepeat(DIK_D))
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_LEFT])
			|| (GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_RIGHT])
			|| (GetButtonRepeat(BUTTON_LEFT))
			|| (GetButtonRepeat(BUTTON_RIGHT)))
		{// ���L�[���E�L�[�������ꂽ���A
			// �I�������ԍ��Ɍ��݂̑I��ԍ�����
			nSelected = g_nSelect;

			// �I����[000] ���Đ�
			PlaySound(SOUND_LABEL_SE_SELECT_000);
		}
	}
	else if ((GetKeyboardTrigger(DIK_RETURN))
		|| (GetButtonTrigger(BUTTON_A)))
	{// �G���^�[�L�[�������ꂽ���A
		// �I�������ԍ��Ɍ��݂̑I��ԍ�����
		nSelected = g_nSelect;

		// ���j���[[00] ���������ɂ���
		g_control.state = UI_MENU_00_STATE_IN_CLEAR;
		g_control.nCounterState = UI_MENU_00_CLEAR_TIME;
	}
	
	return nSelected;
}

//========================================
// DrawUi_menu_00�֐� - ���j���[[00] �̕`�揈�� -
//========================================
void DrawUi_menu_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_menu_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUi_menu_00 = 0; nCntUi_menu_00 < UI_MENU_00_MAX; nCntUi_menu_00++)
	{
		if (g_aUi_menu_00[nCntUi_menu_00].bUse) 
		{// �g�p����Ă��鎞�A
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureUi_menu_00[g_aUi_menu_00[nCntUi_menu_00].type]);

			// ���j���[[00]�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_menu_00 * 4, 2);
		}
	}
}

//========================================
// SetUi_menu_00Pos�֐� - ���j���[[00] �̒��S���W��ݒ� -
//========================================
void SetUi_menu_00Pos(D3DXVECTOR3 pos)
{
	// ���S���W����
	g_centerPos = pos;
}

//========================================
// SetUi_menu_00�֐� - ���j���[[00] �̐ݒ菈�� -
//========================================
void SetUi_menu_00(Ui_menu_00_set *set, int nMax, float fSpaceX, float fSpaceY)
{
	// �\��t���O��^�ɂ���
	g_menuReservation.bReservation = true;

	g_menuReservation.set = set;
	g_menuReservation.nMax = nMax;
	g_menuReservation.fSpaceX = fSpaceX;
	g_menuReservation.fSpaceY = fSpaceY;
}