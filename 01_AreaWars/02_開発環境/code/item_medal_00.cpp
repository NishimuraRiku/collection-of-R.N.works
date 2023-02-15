//========================================
// 
// ���_��[00]�̏���
// Author:���� ����
// 
//========================================
// *** item_medal_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon.h"
#include "sound.h"
#include "chr_fighter_00.h"
#include "item_medal_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include "ui_score_00.h"
#include <stdio.h>
#include <float.h>

//****************************************
// �}�N����`
//****************************************
// ���_��[00] �̃e�N�X�`���̑��΃p�X
// ���_��[00] �̃p�^�[�����(��)
// ���_��[00] �̃p�^�[�����(�c)
// ���_��[00] �̃A�j���[�V�����ɂ����鎞��
// ���_��[00] �̕�
// ���_��[00] �̍���
// ���_��[00] �̓����蔻��̔��a
// ���_��[00] �̒ǔ�����
// ���_��[00] �̒ǔ���
// ���_��[00] �̒ǔ����J�n�������
// ���_��[00] �̎���
// ���_��[00] �̓_�łɂ����鎞��
// ���_��[00] �̓_�ł��J�n�������
// ���_��[00] �̃X�R�A
#define ITEM_MEDAL_00_TEXTURE_PATH			"data\\TEXTURE\\item_medal_000_border.png"
#define ITEM_MEDAL_00_PATTERN_MAX_X			(4)
#define ITEM_MEDAL_00_PATTERN_MAX_Y			(1)
#define ITEM_MEDAL_00_ANIMATION_TIME		(4)
#define ITEM_MEDAL_00_WIDTH					(PIXEL * 16)
#define ITEM_MEDAL_00_HEIGHT				(PIXEL * 16)
#define ITEM_MEDAL_00_HIT_RADIUS			(PIXEL * 8)
#define ITEM_MEDAL_00_TRACKING_DISTANCE		(160.0f)
#define ITEM_MEDAL_00_TRACKING				(8.0f)
#define ITEM_MEDAL_00_TRACKING_START_LIFE	(160)
#define ITEM_MEDAL_00_LIFE					(200)
#define ITEM_MEDAL_00_BRINK_TIME			(2)
#define ITEM_MEDAL_00_BRINK_START_LIFE		(80)
#define ITEM_MEDAL_00_SCORE					(5000)

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �@��[00] �Ƃ̓����蔻�菈��
void CollisionChr_fighter_00(Item_medal_00 *pItem);

// �v���Y�}�E�H�[��[00] �Ƃ̓����蔻�菈��
void CollisionObj_plasma_wall_00(Item_medal_00 *pItem);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureItem_medal_00	//
						= NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem_medal_00	//
						= NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Item_medal_00			g_aItem_medal_00			//
						[ITEM_MEDAL_00_MAX];		// ���_��[00] �̏��

//========== *** ���_��[00] �̏����擾 ***
Item_medal_00 *GetItem_medal_00(void)
{
	return g_aItem_medal_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterItem_medal_00�֐� - ���_��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterItem_medal_00(Item_medal_00 *pItem) 
{
	pItem->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pItem->bUse				= false;							// �g�p����Ă���t���O
	pItem->nLife			= 0;								// ����
	pItem->nCounterBrink	= 0;								// �_�ł̃J�E���^�[
	pItem->bBrink			= false;							// �_�Ńt���O
	pItem->nCounterAnim		= 0;								// �A�j���[�V�����̃J�E���^�[
	pItem->nPattern			= 0;								// �p�^�[��No.
}

//========================================
// TrackingItem_medal_00�֐� - ���_��[00] �̒ǔ����� -
//========================================
void TrackingItem_medal_00(Item_medal_00 *pItem) 
{
	if ((!GetChr_fighter_00()->bUse)
		|| (pItem->nLife > ITEM_MEDAL_00_TRACKING_START_LIFE))
	{// �@��[00] ���g�p����Ă��Ȃ���Ԃ̎��A�������͎������ǔ����J�n��������������Ă������A
		// �������I������
		return;
	}

	Chr_fighter_00	*pChr												//
					= GetChr_fighter_00();								// �@��[00] �̏��̃|�C���^
	float			fDistance											//
					= FindDistance(pItem->pos, pChr->pos);				// �@��[00] �Ƃ̋���
	float			fMove												//
					= ITEM_MEDAL_00_TRACKING								//
					* (1.0f												//
					- (fDistance / ITEM_MEDAL_00_TRACKING_DISTANCE));	// �ړ���
	float			fAngle												//
					= FindAngle(pItem->pos, pChr->pos);					// ����

	if (fDistance > ITEM_MEDAL_00_TRACKING_DISTANCE) 
	{// �������ǔ������������Ă������A
		// �������I������
		return;
	}
	
	// �ʒu���X�V
	pItem->pos.x += sinf(fAngle) * fMove;
	pItem->pos.y += cosf(fAngle) * fMove;
}

//========================================
// CollisionChr_fighter_00�֐� - �@��[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_fighter_00(Item_medal_00 *pItem)
{
	if (!GetChr_fighter_00()->bUse)
	{// �@��[00] ���g�p����Ă��Ȃ���Ԃ̎��A
		// �������I������
		return;
	}

	Chr_fighter_00	*pChr					//
					= GetChr_fighter_00();	// �@��[00] �̏��̃|�C���^

	if (CheckHit(
		pItem->pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		ITEM_MEDAL_00_HIT_RADIUS,
		ITEM_MEDAL_00_HIT_RADIUS,
		HIT_TEST_TYPE_CIRCLE,
		pChr->pos,
		pChr->rot,
		pChr->fHitTestWidth,
		pChr->fHitTestHeight,
		GetChr_fighter_00Type()[pChr->nType].hitTestType))
	{// ���_��[00] ���@��[00] �̏Փ˂������A
		// �{�[�i�XSE[000] ���Đ�
		PlaySound(SOUND_LABEL_SE_BONUS_000);

		// �g�p���Ă��Ȃ���Ԃɂ���
		pItem->bUse = false;

		// �X�R�A�����Z
		pChr->nScore += ITEM_MEDAL_00_SCORE;

		// �X�R�A[00] �̐ݒ菈��
		SetUi_score_00(pChr->pos, ITEM_MEDAL_00_SCORE);
	}
}

//========================================
// CollisionObj_plasma_wall_00�֐� - �v���Y�}�E�H�[��[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionObj_plasma_wall_00(Item_medal_00 *pItem)
{
	if (!pItem->bUse)
	{// ���_��[00] ���g�p����Ă��Ȃ����A
		// �������I������
		return;
	}

	Obj_plasma_wall_00			*pObj								//
								= GetObj_plasma_wall_00();			// �v���Y�}�E�H�[��[00] �̏��
	Obj_plasma_block_00_square	*pSquare							//
								= GetObj_plasma_block_00_square();	// �v���Y�}�u���b�N[00] �̕`���l�p�`�̏��

	for (int nCntObj = 0; nCntObj < OBJ_PLASMA_WALL_00_MAX; nCntObj++, pObj++)
	{
		float fWidth, fHeight;	// ���ƍ���

		if ((int)((pObj->rot.z / D3DX_PI) * 10) % 10 == 0)
		{// �v���Y�}�E�H�[���̌������c�̎��A
			// ���ƍ�����ݒ�
			fWidth = OBJ_PLASMA_WALL_00_WIDTH;
			fHeight = pObj->fHeight;
		}
		else
		{// �v���Y�}�E�H�[���̌��������̎��A
			// ���ƍ�����ݒ�
			fHeight = OBJ_PLASMA_WALL_00_WIDTH;
			fWidth = pObj->fHeight;
		}

		if ((nCntObj == OBJ_PLASMA_WALL_00_TOP) && (pItem->pos.y - ITEM_MEDAL_00_HIT_RADIUS < pSquare->fTop + (fHeight * 0.5f)))
		{// ��̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pItem->pos.y = pSquare->fTop + (fHeight * 0.5f) + ITEM_MEDAL_00_HIT_RADIUS;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_BOTTOM) && (pItem->pos.y + ITEM_MEDAL_00_HIT_RADIUS > pSquare->fBottom - (fHeight * 0.5f)))
		{// ���̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pItem->pos.y = pSquare->fBottom - (fHeight * 0.5f) - ITEM_MEDAL_00_HIT_RADIUS;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_LEFT) && (pItem->pos.x - ITEM_MEDAL_00_HIT_RADIUS < pSquare->fLeft + (fWidth * 0.5f)))
		{// ���̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pItem->pos.x = pSquare->fLeft + (fWidth * 0.5f) + ITEM_MEDAL_00_HIT_RADIUS;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_RIGHT) && (pItem->pos.x + ITEM_MEDAL_00_HIT_RADIUS > pObj->pos.x - (fWidth * 0.5f)))
		{// �E�̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pItem->pos.x = pObj->pos.x - (fWidth * 0.5f) - ITEM_MEDAL_00_HIT_RADIUS;
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitItem_medal_00�֐� - ���_��[00]�̏��������� -
//========================================
void InitItem_medal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, ITEM_MEDAL_00_TEXTURE_PATH, &g_aTextureItem_medal_00);

	for (int nCntItem_medal_00 = 0; nCntItem_medal_00 < ITEM_MEDAL_00_MAX; nCntItem_medal_00++) 
	{
		g_aItem_medal_00[nCntItem_medal_00].bUse	= false;					// �g�p����Ă��邩�t���O ������������
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ITEM_MEDAL_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem_medal_00,
		NULL);

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	Item_medal_00	*pItem				//
					= g_aItem_medal_00;	// ���_��[00] �̏��̃|�C���^

	for (int nCntItem = 0; nCntItem < ITEM_MEDAL_00_MAX; nCntItem++, pVtx += 4, pItem++)
	{
		// �p�����[�^�[�̏���������
		InitParameterItem_medal_00(pItem);

		// ���_���W ��ݒ�
		SetVertexPosition(pVtx,
			pItem->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			true,
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			ANGLE_TYPE_FIXED);

		// rhw�̐ݒ�
		SetRHW(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&pItem->nPattern,
				0,
				ITEM_MEDAL_00_PATTERN_MAX_X
				* ITEM_MEDAL_00_PATTERN_MAX_Y,
				&pItem->nCounterAnim,
				ITEM_MEDAL_00_ANIMATION_TIME,
				ANIM_TYPE_NORMAL),
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			NULL);
	}
	
	// ���_���W ���A�����b�N����
	g_pVtxBuffItem_medal_00->Unlock();
}

//========================================
// UninitItem_medal_00�֐� - ���_��[00]�̏I������ -
//========================================
void UninitItem_medal_00(void)
{
	// �e�N�X�`���̔j��
	if (g_aTextureItem_medal_00 != NULL)
	{
		g_aTextureItem_medal_00->Release();
		g_aTextureItem_medal_00 = NULL;
	}
	
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem_medal_00 != NULL) 
	{
		g_pVtxBuffItem_medal_00->Release();
		g_pVtxBuffItem_medal_00 = NULL;
	}
}

//========================================
// UpdateItem_medal_00�֐� - ���_��[00]�̍X�V���� -
//========================================
void UpdateItem_medal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^
	
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	Item_medal_00	*pItem				//
					= g_aItem_medal_00;	// ���_��[00] �̏��̃|�C���^

	for (int nCntItem_medal_00 = 0; nCntItem_medal_00 < ITEM_MEDAL_00_MAX; nCntItem_medal_00++, pVtx += 4, pItem++)
	{
		if (!pItem->bUse)
		{// ���_��[00]���g�p����Ă����ԂłȂ����A
			// ������܂�Ԃ�
			continue;
		}
		else if (--pItem->nLife <= 0)
		{// ���������Z�������ʐs�������A
			// �g�p���Ă��Ȃ���Ԃɂ���
			pItem->bUse = false;

			continue;
		}
		else if (pItem->nLife <= ITEM_MEDAL_00_BRINK_START_LIFE) 
		{// �������_�ł��J�n��������ȉ��̎��A
			// �_�ŃJ�E���^�[�����Z
			pItem->nCounterBrink++;

			if (pItem->nCounterBrink >= ITEM_MEDAL_00_BRINK_TIME) 
			{// �_�ŃJ�E���^�[���_�łɂ����鎞�ԂɒB�������A
				// �_�ŃJ�E���^�[��������
				pItem->nCounterBrink = 0;

				// �_�Ńt���O��؂�ւ���
				pItem->bBrink ^= 1;
			}
		}

		// �ǔ�����
		TrackingItem_medal_00(pItem);

		// �@��[00] �Ƃ̓����蔻�菈��
		CollisionChr_fighter_00(pItem);

		// �v���Y�}�E�H�[��[00] �Ƃ̓����蔻�菈��
		CollisionObj_plasma_wall_00(pItem);

		// ���_���W ��ݒ�
		SetVertexPosition(pVtx,
			pItem->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			true,
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&pItem->nPattern,
				0,
				(ITEM_MEDAL_00_PATTERN_MAX_X
					* ITEM_MEDAL_00_PATTERN_MAX_Y) - 1,
				&pItem->nCounterAnim,
				ITEM_MEDAL_00_ANIMATION_TIME,
				ANIM_TYPE_NORMAL),
			ITEM_MEDAL_00_PATTERN_MAX_X,
			ITEM_MEDAL_00_PATTERN_MAX_Y,
			NULL);
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffItem_medal_00->Unlock();
}

//========================================
// DrawItem_medal_00�֐� - ���_��[00]�̕`�揈�� -
//========================================
void DrawItem_medal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ ���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem_medal_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureItem_medal_00);

	for (int nCntItem = 0; nCntItem < ITEM_MEDAL_00_MAX; nCntItem++)
	{
		if ((g_aItem_medal_00[nCntItem].bUse)
			&& (!g_aItem_medal_00[nCntItem].bBrink))
		{// ���_��[00]���g�p����Ă��鎞�A�������͓_�Ńt���O���U�̎��A
			// ���_��[00]�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffItem_medal_00->Unlock();
}

//========================================
// SetItem_medal_00�֐� - ���_��[00]�̐ݒ菈�� -
//========================================
void SetItem_medal_00(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffItem_medal_00->Lock(0, 0, (void**)&pVtx, 0);

	Item_medal_00	*pItem				//
					= g_aItem_medal_00;	// ���_��[00] �̏��̃|�C���^

	for (int nCntItem = 0; nCntItem < ITEM_MEDAL_00_MAX; nCntItem++, pVtx += 4, pItem++)
	{
		if (pItem->bUse)
		{// ���_��[00]���g�p����Ă��鎞
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// �p�����[�^�[�̏���������
		InitParameterItem_medal_00(pItem);

		// �ʒu/�p�x ����
		pItem->pos = pos;

		// ���_���W ��ݒ�
		SetVertexPosition(pVtx,
			pItem->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			true,
			ITEM_MEDAL_00_WIDTH,
			ITEM_MEDAL_00_HEIGHT,
			ANGLE_TYPE_FIXED);

		// ������ݒ�
		pItem->nLife = ITEM_MEDAL_00_LIFE;

		// �g�p���Ă����Ԃɂ���
		pItem->bUse = true;

		break;
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffItem_medal_00->Unlock();
}