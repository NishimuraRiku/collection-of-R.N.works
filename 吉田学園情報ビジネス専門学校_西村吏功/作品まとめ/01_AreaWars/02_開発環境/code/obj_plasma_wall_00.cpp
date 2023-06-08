//========================================
// 
// �v���Y�}�E�H�[��[00]�̏���
// Author:���� ����
// 
//========================================
// *** obj_plasma_wall_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "sound.h"
#include "md_game_00.h"
#include "chr_fighter_00.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// �}�N����`
//****************************************
#define OBJ_PLASMA_WALL_00_HEIGHT					(PIXEL * 32)						// �v���Y�}�E�H�[��[00] �̍���
#define OBJ_PLASMA_WALL_00_UV_MOVE					(0.015f)							// �v���Y�}�E�H�[��[00] ��UV�̈ړ���
#define OBJ_PLASMA_WALL_00_TEXTURE_PATH				"data\\TEXTURE\\eff_plasma_000.png"	// �v���Y�}�E�H�[��[00] �̃e�N�X�`���t�@�C���̑��΃p�X
#define OBJ_PLASMA_WALL_00_TEXTURE_X_PATTERN_MAX	(4)									// �v���Y�}�E�H�[��[00] �̃e�N�X�`���̃p�^�[����(��)
#define OBJ_PLASMA_WALL_00_TEXTURE_Y_PATTERN_MAX	(1)									// �v���Y�}�E�H�[��[00] �̃e�N�X�`���̃p�^�[����(�c)
#define OBJ_PLASMA_WALL_00_ANIM_CHANGE_TIME			(6)									// �v���Y�}�E�H�[��[00] �̃A�j���[�V�����̐؂�ւ�����
#define OBJ_PLASMA_WALL_00_ALPHA_MAX				(255)								// �v���Y�}�E�H�[��[00] �̕s�����x�̏���l
#define OBJ_PLASMA_WALL_00_ALPHA_MIN				(70)								// �v���Y�}�E�H�[��[00] �̕s�����x�̉����l
#define OBJ_PLASMA_WALL_00_ALPHA_CHANGE				(2)									// �v���Y�}�E�H�[��[00] �̕ϓ��l
#define OBJ_PLASMA_WALL_00_DAMAGE					(1.0f)								// �v���Y�}�E�H�[��[00] �̃_���[�W
#define OBJ_PLASMA_WALL_00_REBOUND					(0.0f)								// �v���Y�}�E�H�[��[00] �̔�����

//****************************************
// �v���g�^�C�v�錾
//****************************************
void SetPosition	(void);	// �v���Y�}�u���b�N[00] �̍��W�ɉ������ʒu/����/�p�x���擾

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureObj_plasma_wall_00 = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObj_plasma_wall_00 = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Obj_plasma_wall_00 g_aObj_plasma_wall_00[4];					// �v���Y�}�E�H�[��[00] �̏��
int g_nAlpha;													// �s�����x

//========== *** �v���Y�}�E�H�[��[00] �̏����擾 ***
Obj_plasma_wall_00 *GetObj_plasma_wall_00(void)
{
	return g_aObj_plasma_wall_00;
}

//========================================
// InitObj_plasma_wall_00�֐� - �v���Y�}�E�H�[��[00]�̏��������� -
//========================================
void InitObj_plasma_wall_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �s�����x��������
	int g_nAlpha = OBJ_PLASMA_WALL_00_ALPHA_MAX;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, OBJ_PLASMA_WALL_00_TEXTURE_PATH, &g_pTextureObj_plasma_wall_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * OBJ_PLASMA_WALL_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObj_plasma_wall_00,
		NULL);

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffObj_plasma_wall_00->Lock(0, 0, (void**)&pVtx, 0);

	// �v���Y�}�u���b�N[00] �̍��W�ɉ������ʒu/����/�p�x���擾
	SetPosition();

	for (int nCntObj_plasma_wall_00 = 0; nCntObj_plasma_wall_00 < OBJ_PLASMA_WALL_00_MAX; nCntObj_plasma_wall_00++, pVtx += 4)
	{
		
		Obj_plasma_wall_00 *pObj = &g_aObj_plasma_wall_00[nCntObj_plasma_wall_00];	// �v���Y�}�E�H�[��

		{
			float fHeight = pObj->fHeight;	// ����

			{
				D3DXVECTOR3 drawPos = pObj->pos;											// �`��ʒu
				float fWidth	= OBJ_PLASMA_WALL_00_WIDTH;									// ��
				float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
				float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

				// ���_���W ��ݒ�
				pVtx[0].pos.x = drawPos.x + sinf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
				pVtx[0].pos.y = drawPos.y + cosf(pObj->rot.z - (D3DX_PI - fAngle)) * fLength;
				pVtx[1].pos.x = drawPos.x + sinf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
				pVtx[1].pos.y = drawPos.y + cosf(pObj->rot.z + (D3DX_PI - fAngle)) * fLength;
				pVtx[2].pos.x = drawPos.x + sinf(pObj->rot.z - fAngle) * fLength;
				pVtx[2].pos.y = drawPos.y + cosf(pObj->rot.z - fAngle) * fLength;
				pVtx[3].pos.x = drawPos.x + sinf(pObj->rot.z + fAngle) * fLength;
				pVtx[3].pos.y = drawPos.y + cosf(pObj->rot.z + fAngle) * fLength;
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
				int nPtn		= pObj->nPattern;							// �e�N�X�`���̃p�^�[��No.
				int nPtnMaxX	= OBJ_PLASMA_WALL_00_TEXTURE_X_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(��)
				int nPtnMaxY	= OBJ_PLASMA_WALL_00_TEXTURE_Y_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(�c)
				D3DXVECTOR3 tex = pObj->tex;								// �e�N�X�`�����W�̊J�n�ʒu(UV�l)

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y);
				pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y);
				pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));
				pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));
			}
		}
	}
	
	// ���_���W ���A�����b�N����
	g_pVtxBuffObj_plasma_wall_00->Unlock();
}

//========================================
// UninitObj_plasma_wall_00�֐� - �v���Y�}�E�H�[��[00]�̏I������ -
//========================================
void UninitObj_plasma_wall_00(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureObj_plasma_wall_00 != NULL)
	{
		g_pTextureObj_plasma_wall_00->Release();
		g_pTextureObj_plasma_wall_00 = NULL;
	}
	
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffObj_plasma_wall_00 != NULL) 
	{
		g_pVtxBuffObj_plasma_wall_00->Release();
		g_pVtxBuffObj_plasma_wall_00 = NULL;
	}
}

//========================================
// UpdateObj_plasma_wall_00�֐� - �v���Y�}�E�H�[��[00]�̍X�V���� -
//========================================
void UpdateObj_plasma_wall_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffObj_plasma_wall_00->Lock(0, 0, (void**)&pVtx, 0);

	// �v���Y�}�u���b�N[00] �̍��W�ɉ���������/�p�x���擾
	SetPosition();

	for (int nCntObj_plasma_wall_00 = 0; nCntObj_plasma_wall_00 < OBJ_PLASMA_WALL_00_MAX; nCntObj_plasma_wall_00++, pVtx += 4)
	{
		Obj_plasma_wall_00 *pObj = &g_aObj_plasma_wall_00[nCntObj_plasma_wall_00];	// �v���Y�}�E�H�[��[00] �̏��̃|�C���^
		
		{
			float fHeight = pObj->fHeight;	// ����

			{
				int nAnimChangeTime = OBJ_PLASMA_WALL_00_ANIM_CHANGE_TIME;		// �A�j���[�V�����̐؂�ւ�����
				int nPtn			= pObj->nPattern;							// �e�N�X�`���̃p�^�[��No.
				int nPtnMaxX		= OBJ_PLASMA_WALL_00_TEXTURE_X_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(��)
				int nPtnMaxY		= OBJ_PLASMA_WALL_00_TEXTURE_Y_PATTERN_MAX;	// �e�N�X�`���̃p�^�[����(�c)
				D3DXVECTOR3 tex		= pObj->tex;								// �e�N�X�`�����W�̊J�n�ʒu(UV�l)

				// �A�j���[�V�����̃J�E���^�[�����Z
				pObj->nCounterAnim++;

				if (pObj->nCounterAnim >= nAnimChangeTime)
				{// �A�j���[�V�����̃J�E���^�[���A�j���[�V�����ɂ����鎞�ԂɒB�������A
					// �A�j���[�V�����̃J�E���^�[��������
					pObj->nCounterAnim = 0;

					// �p�^�[��No.���X�V
					nPtn = ++nPtn % (nPtnMaxX * nPtnMaxY);
				}

				// V���W���X�V
				tex.y += OBJ_PLASMA_WALL_00_UV_MOVE;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y);
				pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y);
				pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));
				pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, tex.y + (fHeight / OBJ_PLASMA_WALL_00_HEIGHT));

				// �p�^�[��No.��K�p
				pObj->nPattern = nPtn;

				// �e�N�X�`�����W�̊J�n�ʒu(UV�l)��K�p
				pObj->tex = tex;
			}

			{
				D3DXVECTOR3 drawPos = pObj->pos;												// �`��ʒu
				float fWidth		= OBJ_PLASMA_WALL_00_WIDTH;									// ��
				float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
				float fAngle		= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

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

		{
			// �s�����x��ϓ�������
			g_nAlpha += OBJ_PLASMA_WALL_00_ALPHA_CHANGE;

			int nAlpha;										// �s�����x
			int nAlphaMax = OBJ_PLASMA_WALL_00_ALPHA_MAX;	// �s�����x�̏���l
			int nAlphaMin = OBJ_PLASMA_WALL_00_ALPHA_MIN;	// �s�����x�̉����l

			if ((g_nAlpha / (nAlphaMax - nAlphaMin)) % 2 == 0)
			{// �s�����x������ŏ��Z�������ʂ������������ꍇ�A
				// �s�����x������ŏ�]�Z����
				nAlpha = nAlphaMin + (g_nAlpha % (nAlphaMax - nAlphaMin));
			}
			else
			{// �s�����x������ŏ��Z�������ʂ���������ꍇ�A
				// �������s�����x������ŏ�]�Z�������ʂ����Z����
				nAlpha = nAlphaMin + ((nAlphaMax - nAlphaMin) - (g_nAlpha % (nAlphaMax - nAlphaMin)));
			}

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, nAlpha);
		}
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffObj_plasma_wall_00->Unlock();
}

//========================================
// DrawObj_plasma_wall_00�֐� - �v���Y�}�E�H�[��[00]�̕`�揈�� -
//========================================
void DrawObj_plasma_wall_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ ���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffObj_plasma_wall_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureObj_plasma_wall_00);

	for (int nCntObj_plasma_wall_00 = 0; nCntObj_plasma_wall_00 < OBJ_PLASMA_WALL_00_MAX; nCntObj_plasma_wall_00++)
	{
		// �v���Y�}�E�H�[��[00]�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntObj_plasma_wall_00 * 4, 2);
	}
}

//========================================
// SetPosition�֐� - �v���Y�}�u���b�N[00] �̍��W�ɉ������ʒu/����/�p�x���擾 -
//========================================
void SetPosition(void)
{
	Obj_plasma_block_00_square *pObj_plasma_block_00_square = GetObj_plasma_block_00_square();	// �v���Y�}�u���b�N[00] �̏��

	float fTop		= pObj_plasma_block_00_square->fTop;	// ��ӂ̍��W
	float fBottom	= pObj_plasma_block_00_square->fBottom;	// ���ӂ̍��W
	float fLeft		= pObj_plasma_block_00_square->fLeft;	// ���ӂ̍��W
	float fRight	= pObj_plasma_block_00_square->fRight;	// �E�ӂ̍��W

	// ���W���X�V
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_TOP]	.pos = D3DXVECTOR3(fLeft + ((fRight - fLeft) * 0.5f), fTop								, 0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_BOTTOM].pos = D3DXVECTOR3(fLeft + ((fRight - fLeft) * 0.5f), fBottom							, 0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_LEFT]	.pos = D3DXVECTOR3(fLeft							, fTop + ((fBottom - fTop) * 0.5f)	, 0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_RIGHT]	.pos = D3DXVECTOR3(fRight							, fTop + ((fBottom - fTop) * 0.5f)	, 0.0f);

	// �������X�V
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_TOP]	.fHeight = fRight - fLeft;
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_BOTTOM].fHeight = fRight - fLeft;
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_LEFT]	.fHeight = fBottom - fTop;
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_RIGHT]	.fHeight = fBottom - fTop;

	// �������X�V
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_TOP]	.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -	0.5f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_BOTTOM].rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI *	0.5f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_LEFT]	.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI *	0.0f);
	g_aObj_plasma_wall_00[OBJ_PLASMA_WALL_00_RIGHT]	.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI *	1.0f);
}