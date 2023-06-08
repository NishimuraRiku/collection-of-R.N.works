//========================================
// 
// ����[01] �̏���
// Author:���� ����
// 
//========================================
// *** atk_explosion_01.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "sound.h"
#include "chr_fighter_00.h"
#include "chr_enemy_00.h"
#include "eff_light_00.h"
#include "eff_particle_00.h"
#include "atk_bullet_00.h"
#include "atk_explosion_01.h"
#include "input.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
#define ATK_EXPLOSION_01_MAX		(256)	// ����[01] �̍ő吔
#define SCALE_DAMPING_MAGNIFICATION	(0.1f)	// �g��ʂ̌����{��

//****************************************
// �񋓌^�̒�`
//****************************************

// �W�I�^�C�v
typedef enum
{
	TARGET_TYPE_ENEMY_ONRY
}TARGET_TYPE;

//****************************************
// �\���̂̒�`
//****************************************

// ����[01] �\���̂̒�`
typedef struct 
{
								//========== *** �ʒu�֘A ***
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	float fWidth;				// ��
	float fHeight;				// ����
	float fHitTestRadius;		// �����蔻��̔��a
	float fScale;				// �g���
								//========== *** ���ޏ�� ***
	ATK_EXPLOSION_01_TYPE type;	// ���
	bool bUse;					// �g�p����Ă��邩�t���O
								//========== *** ��Ԓl ***
	int nLife;					// ����
	int nDamage;				// �_���[�W
	int nPattern;				// �p�^�[��No.
	int nCounterDamage;			// �_���[�W�J�E���^�[
}Atk_explosion_01;

// ����[01] �̎�ޖ��̏��
typedef struct
{
	char					aTexturPath[TXT_MAX];	// �e�N�X�`���̑��΃p�X
	int						nPtnMaxX;				// �p�^�[���̍ő吔(��)
	int						nPtnMaxY;				// �p�^�[���̍ő吔(�c)
	int						nDamage;				// �_���[�W
	int						nDamageTime;			// �_���[�W�̔����Ԋu
	int						nLife;					// ����
	float					fScale;					// �g���
	float					fHitRadiusDiameter;		// �����蔻��̔��a�{��
	SOUND_LABEL				soundSet;				// �ݒ莞�̃T�E���h
	Color					col;					// �F
	EFF_PARTICLE_00_TYPE	ptclType;				// �p�[�e�B�N��[00] �̎��
	int						nPtclNum;				// �p�[�e�B�N���̔�����
	float					fVibration;				// �U���̋���
	TARGET_TYPE				targetType;				// �ڕW�^�C�v
}Atk_explosion_01Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �@��[00] �Ƃ̓����蔻�菈��
void CollisionChr_fighter_00(Atk_explosion_01 *pAtk);

// �G[00] �Ƃ̓����蔻��
void CollisionChr_enemy_00(Atk_explosion_01 *pAtk);

// �e[00] �Ƃ̓����蔻��
void CollisionAtk_bullet_00(Atk_explosion_01 *pAtk);

// ����[01] �̃X�e�[�^�X����ނɉ����Đݒ�
void SetState(Atk_explosion_01 *pAtk);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9 g_aTextureAtk_explosion_01[ATK_EXPLOSION_01_TYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_explosion_01 = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
Atk_explosion_01 g_aAtk_explosion_01[ATK_EXPLOSION_01_MAX];						// ����[01] �̏��

// ����[01] �̎�ނ̏��
Atk_explosion_01Type g_aAtk_explosion_01Type[ATK_EXPLOSION_01_TYPE_MAX] =
{//   �e�N�X�`���̑��΃p�X             , �p�^�[���̍ő�X, Y, �_���[�W, �����Ԋu, ����, �g���, �����蔻��̔��a�{��, SE                           , �F                    , �p�[�e�B�N���̎��           , �p�[�e�B�N���̔����� , �U���̋���, �W�I�^�C�v
	{ "data\\TEXTURE\\eff_ring_002.jpg", 1              , 1, 5       , 0       , 32  , 120.0f, 0.8f                , SOUND_LABEL_SE_EXPLOSION_005 , { 255, 155, 59 , 255 }, EFF_PARTICLE_00_TYPE_NONE    , 0                    , 16.0f     , TARGET_TYPE_ENEMY_ONRY},
};

//========================================
// InitAtk_explosion_01�֐� - ����[01] �̏��������� -
//========================================
void InitAtk_explosion_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntAtk_explosion_01_type = 0; nCntAtk_explosion_01_type < ATK_EXPLOSION_01_TYPE_MAX; nCntAtk_explosion_01_type++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aAtk_explosion_01Type[nCntAtk_explosion_01_type].aTexturPath, &g_aTextureAtk_explosion_01[nCntAtk_explosion_01_type]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ATK_EXPLOSION_01_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_explosion_01,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_explosion_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++, pVtx += 4)
	{
		Atk_explosion_01 *pAtk = &g_aAtk_explosion_01[nCntAtk_explosion_01];	// ����[01] �̏��̃|�C���^

		pAtk->nPattern		= 0;								// �p�^�[��No.������������
		pAtk->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu������������
		pAtk->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����������������
		pAtk->bUse			= false;							// �g�p����Ă��邩�t���O������������
		pAtk->type			= ATK_EXPLOSION_01_TYPE_00;			// ��ނ�����������			

		// �X�e�[�^�X����ނɉ����Đݒ�
		SetState(pAtk);

		{
			D3DXVECTOR3 pos = pAtk->pos;												// �ʒu
			D3DXVECTOR3 rot = pAtk->rot;												// ����
			float fWidth	= pAtk->fWidth;												// ��
			float fHeight	= pAtk->fHeight;											// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
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
			int nPtn		= pAtk->nPattern;								// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxX;	// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxY;	// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffAtk_explosion_01->Unlock();
}

//========================================
// UninitAtk_explosion_01�֐� - ����[01] �̏I������ -
//========================================
void UninitAtk_explosion_01(void)
{
	// �e�N�X�`���̔j��
	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_TYPE_MAX; nCntAtk_explosion_01++)
	{
		if (g_aTextureAtk_explosion_01[nCntAtk_explosion_01] != NULL)
		{
			g_aTextureAtk_explosion_01[nCntAtk_explosion_01]->Release();
			g_aTextureAtk_explosion_01[nCntAtk_explosion_01] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAtk_explosion_01 != NULL) 
	{
		g_pVtxBuffAtk_explosion_01->Release();
		g_pVtxBuffAtk_explosion_01 = NULL;
	}
}

//========================================
// UpdateAtk_explosion_01�֐� - ����[01] �̍X�V���� -
//========================================
void UpdateAtk_explosion_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_explosion_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++, pVtx += 4)
	{
		if (!g_aAtk_explosion_01[nCntAtk_explosion_01].bUse)
		{// ����[01] ���g�p����Ă����ԂłȂ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		Atk_explosion_01 *pAtk = &g_aAtk_explosion_01[nCntAtk_explosion_01];	// ����[01] �̏��̃|�C���^

		// ���������Z
		pAtk->nLife--;

		if (pAtk->nLife <= 0) 
		{// �������s�������A
			// �g�p���Ă��Ȃ���Ԃɂ���
			pAtk->bUse = false;

			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		D3DXVECTOR3 pos		= pAtk->pos;											// �ʒu
		D3DXVECTOR3 rot		= pAtk->rot;											// ����
		int nPtn			= pAtk->nPattern;										// �e�N�X�`���̃p�^�[��No.
		int nPtnMaxX		= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxX;			// �e�N�X�`���̃p�^�[����(��)
		int nPtnMaxY		= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxY;			// �e�N�X�`���̃p�^�[����(�c)

		if (pAtk->nPattern == nPtnMaxX * nPtnMaxY)
		{// ����[01] �̃p�^�[��No.���p�^�[���̏�������������A
			pAtk->bUse = false; // �g�p���Ă��Ȃ���Ԃɂ���
		}

		// ��/�������X�V
		pAtk->fWidth += pAtk->fScale;
		pAtk->fHeight += pAtk->fScale;
		pAtk->fHitTestRadius = pAtk->fWidth*0.5f;

		// �g��ʂ��X�V(����)
		pAtk->fScale += (0.0f - pAtk->fScale) * SCALE_DAMPING_MAGNIFICATION;

		SetEff_light_00(pAtk->pos, pAtk->rot, 0.0f, 
			g_aAtk_explosion_01Type[pAtk->type].col,
			pAtk->fWidth, 
			pAtk->fHeight, 
			g_aAtk_explosion_01Type[pAtk->type].nLife-pAtk->nLife,
			EFF_LIGHT_00_TYPE_01);

		if (++pAtk->nCounterDamage >= g_aAtk_explosion_01Type[pAtk->type].nDamageTime) 
		{// �_���[�W�J�E���^�[���_���[�W�̔����Ԋu�ɒB�������A
			// �_���[�W�J�E���^�[��������
			pAtk->nCounterDamage = 0;

			// �ڕW�^�C�v�ɉ����������蔻�菈��
			switch (g_aAtk_explosion_01Type[pAtk->type].targetType)
			{
				//========== *** �G�̂� ***
			case TARGET_TYPE_ENEMY_ONRY:

				// �G[00]�Ƃ̓����蔻�菈��
				CollisionChr_enemy_00(pAtk);

				break;
			}
		}

		// �e[00] �Ƃ̓����蔻�菈��
		CollisionAtk_bullet_00(pAtk);

		{
			D3DXVECTOR3 drawPos = pos;																			// �`��ʒu
			float fLength	= sqrtf((pAtk->fWidth * pAtk->fWidth) + (pAtk->fHeight * pAtk->fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(pAtk->fWidth, pAtk->fHeight);												// �Ίp���̊p�x

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// ���_���W��ݒ�
			pVtx[0].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		{
			float fAlpha = ((float)pAtk->nLife / (float)g_aAtk_explosion_01Type[pAtk->type].nLife);	// �����x
			Color col = g_aAtk_explosion_01Type[pAtk->type].col;									// �F

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
			pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
			pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
			pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, (int)(255 * fAlpha));
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffAtk_explosion_01->Unlock();
}

//========================================
// DrawAtk_explosion_01�֐� - ����[01] �̕`�揈�� -
//========================================
void DrawAtk_explosion_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_explosion_01, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++)
	{
		if (g_aAtk_explosion_01[nCntAtk_explosion_01].bUse)
		{// �g�p����Ă��鎞�A
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureAtk_explosion_01[g_aAtk_explosion_01[nCntAtk_explosion_01].type]);

			// �@��[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAtk_explosion_01 * 4, 2);
		}
	}

	// �A���t�@�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// Atk_explosion_01�֐� - ����[01] �̐ݒ菈�� -
//========================================
void SetAtk_explosion_01(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ATK_EXPLOSION_01_TYPE type)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_explosion_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_01 = 0; nCntAtk_explosion_01 < ATK_EXPLOSION_01_MAX; nCntAtk_explosion_01++, pVtx += 4)
	{
		if (g_aAtk_explosion_01[nCntAtk_explosion_01].bUse)
		{// ����[01] ���g�p����Ă��鎞�A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		Atk_explosion_01 *pAtk = &g_aAtk_explosion_01[nCntAtk_explosion_01];	// ����[01] �̏��̃|�C���^

		// �U����ݒ�
		SetCameraVibration(g_aAtk_explosion_01Type[type].fVibration);

		// ����[01] �̎�ނ�ݒ肷��
		pAtk->type = type;

		// �X�e�[�^�X����ނɉ����Đݒ�
		SetState(pAtk);

		// ���ʉ����Đ�
		PlaySound(g_aAtk_explosion_01Type[type].soundSet);

		// �ʒu/�p�x����
		pAtk->pos = pos;
		pAtk->rot = rot;

		// ��/����/�����蔻��̔��a��������
		pAtk->fWidth = 0.0f;
		pAtk->fHeight = 0.0f;
		pAtk->fHitTestRadius = 0.0f;

		// �_���[�W�J�E���^�[��������
		pAtk->nCounterDamage = g_aAtk_explosion_01Type[type].nDamageTime;

		{
			D3DXVECTOR3 drawPos = pAtk->pos;												// �`��ʒu
			D3DXVECTOR3 rot		= pAtk->rot;												// ����
			float fWidth		= pAtk->fWidth;												// ��
			float fHeight		= pAtk->fHeight;											// ����
			float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle		= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		pAtk->nPattern		= 0;	// �p�^�[��No.������������

		{
			int nPtn		= pAtk->nPattern;								// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxX;	// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY	= g_aAtk_explosion_01Type[pAtk->type].nPtnMaxY;	// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}

		// �g�p���Ă����Ԃɂ���
		pAtk->bUse = true;

		// �p�[�e�B�N���̐ݒ菈��
		for (int nCntPtcl = 0; nCntPtcl < g_aAtk_explosion_01Type[pAtk->type].nPtclNum; nCntPtcl++) 
		{
			D3DXVECTOR3 setRot;	// �ݒ肷�������ݒ�
			setRot.z = -D3DX_PI + fRand(D3DX_PI * 2);

			// �p�[�e�B�N��[00] ��ݒ�
			SetEff_particle_00(pos, setRot, g_aAtk_explosion_01Type[pAtk->type].ptclType);
		}

		break;
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffAtk_explosion_01->Unlock();
}

//========================================
// SetState�֐� - ����[01] �̃X�e�[�^�X����ނɉ����Đݒ� -
//========================================
void SetState(Atk_explosion_01 *pAtk_explosion_01)
{
	pAtk_explosion_01->nLife	= g_aAtk_explosion_01Type[pAtk_explosion_01->type].nLife;	// ����
	pAtk_explosion_01->nDamage	= g_aAtk_explosion_01Type[pAtk_explosion_01->type].nDamage;	// �_���[�W
	pAtk_explosion_01->fScale	= g_aAtk_explosion_01Type[pAtk_explosion_01->type].fScale;	// �g���
}

//========================================
// CollisionChr_fighter_00�֐� - �@��[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_fighter_00(Atk_explosion_01 *pAtk)
{
	if (!GetChr_fighter_00()->bUse) 
	{// �@��[00] ���g�p����Ă��Ȃ���Ԃ̎��A
		// �������I������
		return;
	}

	if (FindDistance(pAtk->pos, GetChr_fighter_00()->pos) < pAtk->fHitTestRadius + GetChr_fighter_00()->fHitTestWidth)
	{// �e[00] �̍��W���@��[00] �̓����蔻����ɓ��������A
		// �@��[00] �̃q�b�g����
		HitChr_fighter_00(pAtk->nDamage);
	}
}

//========================================
// CollisionChr_enemy_00�֐� - �G[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_enemy_00(Atk_explosion_01 *pAtk)
{
	Chr_enemy_00 *pChr = GetChr_enemy_00();	// �G[00] �̏��̃|�C���^

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if ((!pChr->bUse)
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_POP])
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
			|| (pChr->aState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_GHOST]))
		{// �g�p����Ă��Ȃ���ԁA�������͏o����/������/�����ϐ�/�c���/�c��ݑ҂�/���蔲���̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (FindDistance(pAtk->pos, pChr->pos) < (pAtk->fHitTestRadius * g_aAtk_explosion_01Type[pAtk->type].fHitRadiusDiameter) + pChr->fWidth)
		{// ����[01] ���G[00] �̓����蔻����ɓ��������A
			// �������󂯂��t���O��^�ɂ���
			pChr->bReceivedExplosion = true;

			// �c��ݑ҂��ɂ����鎞�Ԃ�ݒ�
			pChr->nSwellWaitTime = FindDistance(pAtk->pos, pChr->pos) * CHR_ENEMY_00_SWELL_WAIT_TIME_DIAMETER_EXPLOSION;

			// �G[00] �̃q�b�g����
			HitChr_enemy_00(pChr, pAtk->nDamage);
		}
	}
}

//========================================
// CollisionAtk_bullet_00�֐� - �e[00] �Ƃ̓����蔻�菈�� -
//========================================
void CollisionAtk_bullet_00(Atk_explosion_01 *pAtk) 
{
	Atk_bullet_00	*pAtk_b					//
					= GetAtk_bullet_00();	// �e[00] �̏��̃|�C���^

	for (int nCntAtk_b = 0; nCntAtk_b < ATK_BULLET_00_MAX; nCntAtk_b++,pAtk_b++) 
	{
		if (!pAtk_b->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (FindDistance(pAtk->pos, pAtk_b->pos) < pAtk->fHitTestRadius + pAtk_b->fWidth)
		{// ����[01] ���e[00] �̓����蔻����ɓ��������A
			// �e[00] �̔j�󏈗�
			DestructionAtk_bullet_00(pAtk_b);
		}
	}
}