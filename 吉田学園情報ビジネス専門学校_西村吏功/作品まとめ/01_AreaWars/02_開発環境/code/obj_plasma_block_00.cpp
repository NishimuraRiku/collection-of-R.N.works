//========================================
// 
// �v���Y�}�u���b�N[00]�̏���
// Author:���� ����
// 
//========================================
// *** obj_plasma_block_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "obj_plasma_block_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// �}�N����`
//****************************************
#define OBJ_PLASMA_BLOCK_00_NUM						(4)											// �v���Y�}�u���b�N[00] �̐�
#define OBJ_PLASMA_BLOCK_00_WIDTH					(PIXEL * 16)								// �v���Y�}�u���b�N[00] �̕�
#define OBJ_PLASMA_BLOCK_00_HEIGHT					(PIXEL * 16)								// �v���Y�}�u���b�N[00] �̍���
#define OBJ_PLASMA_BLOCK_00_TEXTURE_PATH			"data\\TEXTURE\\obj_plasma_block_000.png"	// �v���Y�}�u���b�N[00] �̃e�N�X�`���t�@�C���̑��΃p�X
#define OBJ_PLASMA_BLOCK_00_TEXTURE_X_PATTERN_MAX	(4)											// �v���Y�}�u���b�N[00] �̃e�N�X�`���̃p�^�[����(��)
#define OBJ_PLASMA_BLOCK_00_TEXTURE_Y_PATTERN_MAX	(1)											// �v���Y�}�u���b�N[00] �̃e�N�X�`���̃p�^�[����(�c)
#define OBJ_PLASMA_BLOCK_00_ANIM_CHANGE_TIME		(8)											// �v���Y�}�u���b�N[00] �̃A�j���[�V�����̐؂�ւ�����(�c)
#define OBJ_PLASMA_BLOCK_00_SQUARE_WIDTH			(80.0f)										// �v���Y�}�u���b�N[00] ���`���l�p�`�̕�(����)
#define OBJ_PLASMA_BLOCK_00_SQUARE_HEIGHT			(80.0f)										// �v���Y�}�u���b�N[00] ���`���l�p�`�̍���(����)

//****************************************
// �\���̂̒�`
//****************************************

// �v���Y�}�u���b�N[00]�\���̂̒�`
typedef struct
{
							//========== *** �ʒu�֘A ***
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ����
							//========== *** �e�N�X�`���֘A ***
	int nCounterAnim;		// �A�j���[�V�����̃J�E���^�[
	int nAnimChangeTime;	// �A�j���[�V�����̐؂�ւ�����
	int nPattern;			// �p�^�[��No.
}Obj_plasma_block_00;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureObj_plasma_block_00 = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObj_plasma_block_00 = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
Obj_plasma_block_00 g_aObj_plasma_block_00[OBJ_PLASMA_BLOCK_00_NUM];	// �v���Y�}�u���b�N[00] �̏��
Obj_plasma_block_00_square g_square;									// �v���Y�}�u���b�N[00] ���`���l�p�`�̏��

//========== *** �v���Y�}�u���b�N[00] ���`���l�p�`�̏����擾 ***
Obj_plasma_block_00_square *GetObj_plasma_block_00_square(void) 
{
	return &g_square;
}

//========================================
// InitObj_plasma_block_00�֐� - �v���Y�}�u���b�N[00]�̏��������� -
//========================================
void InitObj_plasma_block_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	g_square.nMoveCounter = OBJ_PLASMA_BLOCK_00_MOVE_TIME;	// �ړ��̃J�E���^�[��������

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, OBJ_PLASMA_BLOCK_00_TEXTURE_PATH, &g_pTextureObj_plasma_block_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * OBJ_PLASMA_BLOCK_00_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObj_plasma_block_00,
		NULL);

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffObj_plasma_block_00->Lock(0, 0, (void**)&pVtx, 0);

	//�l�p�`�̕ӂ̍��W��ݒ�
	g_square.fTop		= -OBJ_PLASMA_BLOCK_00_SQUARE_HEIGHT	* 0.5f;
	g_square.fBottom	= OBJ_PLASMA_BLOCK_00_SQUARE_HEIGHT		* 0.5f;
	g_square.fLeft		= -OBJ_PLASMA_BLOCK_00_SQUARE_WIDTH		* 0.5f;
	g_square.fRight		= OBJ_PLASMA_BLOCK_00_SQUARE_WIDTH		* 0.5f;

	// ���W��ݒ�
	g_aObj_plasma_block_00[0].pos = D3DXVECTOR3(g_square.fLeft	, g_square.fTop		, 0.0f);
	g_aObj_plasma_block_00[1].pos = D3DXVECTOR3(g_square.fRight	, g_square.fTop		, 0.0f);
	g_aObj_plasma_block_00[2].pos = D3DXVECTOR3(g_square.fLeft	, g_square.fBottom	, 0.0f);
	g_aObj_plasma_block_00[3].pos = D3DXVECTOR3(g_square.fRight	, g_square.fBottom	, 0.0f);

	for (int nCntObj_plasma_block_00 = 0; nCntObj_plasma_block_00 < OBJ_PLASMA_BLOCK_00_NUM; nCntObj_plasma_block_00++, pVtx += 4)
	{
		{
			D3DXVECTOR3 pos = g_aObj_plasma_block_00[nCntObj_plasma_block_00].pos;		// �ʒu
			D3DXVECTOR3 rot = g_aObj_plasma_block_00[nCntObj_plasma_block_00].rot;		// ����
			float fWidth	= OBJ_PLASMA_BLOCK_00_WIDTH;								// ��
			float fHeight	= OBJ_PLASMA_BLOCK_00_HEIGHT;								// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// ���_���W ��ݒ�
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
			int nPtn		= g_aObj_plasma_block_00[nCntObj_plasma_block_00].nPattern;	// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX	= OBJ_PLASMA_BLOCK_00_TEXTURE_X_PATTERN_MAX;				// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY	= OBJ_PLASMA_BLOCK_00_TEXTURE_Y_PATTERN_MAX;				// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}
	
	// ���_���W ���A�����b�N����
	g_pVtxBuffObj_plasma_block_00->Unlock();
}

//========================================
// UninitObj_plasma_block_00�֐� - �v���Y�}�u���b�N[00]�̏I������ -
//========================================
void UninitObj_plasma_block_00(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureObj_plasma_block_00 != NULL)
	{
		g_pTextureObj_plasma_block_00->Release();
		g_pTextureObj_plasma_block_00 = NULL;
	}
	
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffObj_plasma_block_00 != NULL) 
	{
		g_pVtxBuffObj_plasma_block_00->Release();
		g_pVtxBuffObj_plasma_block_00 = NULL;
	}
}

//========================================
// UpdateObj_plasma_block_00�֐� - �v���Y�}�u���b�N[00]�̍X�V���� -
//========================================
void UpdateObj_plasma_block_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���W��ݒ�
	g_aObj_plasma_block_00[0].pos = D3DXVECTOR3(g_square.fLeft, g_square.fTop, 0.0f);
	g_aObj_plasma_block_00[1].pos = D3DXVECTOR3(g_square.fRight, g_square.fTop, 0.0f);
	g_aObj_plasma_block_00[2].pos = D3DXVECTOR3(g_square.fLeft, g_square.fBottom, 0.0f);
	g_aObj_plasma_block_00[3].pos = D3DXVECTOR3(g_square.fRight, g_square.fBottom, 0.0f);
	
	// ���S�ʒu��ݒ�
	g_square.centerPos.x = (g_square.fTargetLeft + g_square.fTargetRight) * 0.5f;
	g_square.centerPos.y = (g_square.fTargetTop + g_square.fTargetBottom) * 0.5f;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffObj_plasma_block_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntObj_plasma_block_00 = 0; nCntObj_plasma_block_00 < OBJ_PLASMA_BLOCK_00_NUM; nCntObj_plasma_block_00++, pVtx += 4)
	{
		Obj_plasma_block_00 *pObj = &g_aObj_plasma_block_00[nCntObj_plasma_block_00];	// �v���Y�}�u���b�N[00] �̏��̃|�C���^

		float fWidth	= OBJ_PLASMA_BLOCK_00_WIDTH;	// ��
		float fHeight	= OBJ_PLASMA_BLOCK_00_HEIGHT;	// ����

		{
			int nAnimChangeTime = OBJ_PLASMA_BLOCK_00_ANIM_CHANGE_TIME;			// �A�j���[�V�����̐؂�ւ�����
			int nPtnMaxX		= OBJ_PLASMA_BLOCK_00_TEXTURE_X_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY		= OBJ_PLASMA_BLOCK_00_TEXTURE_Y_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(�c)

			// �A�j���[�V�����̃J�E���^�[�����Z
			pObj->nCounterAnim++;

			if (pObj->nCounterAnim >= nAnimChangeTime)
			{// �A�j���[�V�����̃J�E���^�[���A�j���[�V�����ɂ����鎞�ԂɒB������

				// �A�j���[�V�����̃J�E���^�[��������
				pObj->nCounterAnim = 0;

				// �p�^�[��No.���X�V
				pObj->nPattern = ++pObj->nPattern % (nPtnMaxX * nPtnMaxY);
			}

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((pObj->nPattern % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((pObj->nPattern / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}

		{
			D3DXVECTOR3 drawPos = pObj->pos;										// �`��ʒu
			float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle = atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// ���_���W��ݒ�
			pVtx[0].pos.x = drawPos.x + sinf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf(pObj->rot.z - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf(pObj->rot.z - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf(pObj->rot.z + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf(pObj->rot.z + fAngle) * fLength;
		}
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffObj_plasma_block_00->Unlock();
}

//========================================
// UpdateObj_plasma_block_00_square�֐� - �v���Y�}�u���b�N[00] ���`���l�p�`�̍X�V���� -
//========================================
void UpdateObj_plasma_block_00_square(void)
{
	if (g_square.nMoveCounter > 0)
	{// �ړ��̃J�E���^�[��0�������Ă��鎞�A
		// �ړ��̃J�E���^�[�����Z
		g_square.nMoveCounter--;

		if (g_square.nMoveCounter > 0)
		{// �ړ��̃J�E���^�[��0�������Ă��鎞�A
			// �e�ӂ̈ʒu���X�V
			g_square.fTop		+= (g_square.fMoveTop / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
			g_square.fBottom	+= (g_square.fMoveBottom / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
			g_square.fLeft		+= (g_square.fMoveLeft / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
			g_square.fRight		+= (g_square.fMoveRight / (float)OBJ_PLASMA_BLOCK_00_MOVE_TIME);
		}
		else
		{// �ړ��̃J�E���^�[��0�ȉ��̎��A
			// �e�ӂ̈ʒu��ڕW�ʒu�ɐݒ�
			g_square.fTop = g_square.fTargetTop;
			g_square.fBottom = g_square.fTargetBottom;
			g_square.fLeft = g_square.fTargetLeft;
			g_square.fRight = g_square.fTargetRight;
		}
	}
}

//========================================
// DrawObj_plasma_block_00�֐� - �v���Y�}�u���b�N[00]�̕`�揈�� -
//========================================
void DrawObj_plasma_block_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ ���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffObj_plasma_block_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureObj_plasma_block_00);

	for (int nCntObj_plasma_block_00 = 0; nCntObj_plasma_block_00 < OBJ_PLASMA_BLOCK_00_NUM; nCntObj_plasma_block_00++)
	{
		// �v���Y�}�u���b�N[00]�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntObj_plasma_block_00 * 4, 2);
	}
}

//========================================
// SetMoveObj_plasma_block_00�֐� - �v���Y�}�u���b�N[00] �̎l�p�`�̈ړ��ʂ�ݒ� -
//========================================
void SetMoveObj_plasma_block_00(void) 
{
	Obj_plasma_block_00_square	*pObj	// �v���Y�}�u���b�N[00] �̕`���l�p�`�̏��
								= GetObj_plasma_block_00_square();

	// �ړ��̃J�E���^�[��ݒ�
	pObj->nMoveCounter = OBJ_PLASMA_BLOCK_00_MOVE_TIME;

	// �l�p�`�̊e�ӂ̈ړ��ʂ�ݒ�
	pObj->fMoveTop		= (pObj->fTargetTop - pObj->fTop);
	pObj->fMoveBottom	= (pObj->fTargetBottom - pObj->fBottom);
	pObj->fMoveLeft		= (pObj->fTargetLeft - pObj->fLeft);
	pObj->fMoveRight	= (pObj->fTargetRight - pObj->fRight);
}