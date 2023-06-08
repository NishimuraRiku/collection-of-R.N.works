//========================================
// 
// OBJ:�^���b�g[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_turret_00.cpp ***
//========================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "atk_bullet_00.h"		// ATK:�e			[00]
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "eff_shadow_00.h"		// EFF:�e			[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "obj_turret_00.h"		// OBJ:�^���b�g		[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�^���b�g[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_TURRET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_TURRET_00_TYPE_DATA.txt"

// OBJ:�^���b�g[00] �̍ő吔
// OBJ:�^���b�g[00] �̎�ނ̍ő吔
#define OBJ_TURRET_00_MAX		(256)
#define OBJ_TURRET_00_TYPE_NUM	(1)

// OBJ:�^���b�g[00] �̖��G����
#define OBJ_TURRET_00_INVINCIBLE_TIME	(10)

// OBJ:�^���b�g[00] �̃_���[�W����
#define OBJ_TURRET_00_DAMAGE_TIME	(10)

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:�^���b�g[00] �̏��\����
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3	pos;		// �ʒu
	D3DXVECTOR3	rot;		// ����
	D3DXVECTOR3 targetRot;	// �ڕW����
	D3DXVECTOR3 rotTemp;	// �����̕ۑ�

	// ���ފ֘A
	int nType;	// ���

	// ��Ԋ֘A
	bool	bUse;				// �g�p����Ă��邩�t���O
	int		nHP;				// �q�b�g�|�C���g
	int		nHPMax;				// �q�b�g�|�C���g�̏��
	int		nCounterInvincible;	// ���G�J�E���^�[
	int		nCounterDamage;		// �_���[�W�J�E���^�[
	bool	bAim;				// �_���t���O
	int		nCounterShot;		// ���˃J�E���^�[
	int		nRigorCounter;		// �d���J�E���^�[

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
}Obj_turret_00;

// OBJ:�^���b�g[00] �̎�ޖ��̏��\����
typedef struct
{
	int			nHP;			// �q�b�g�|�C���g
	int			nBulletType;	// �e�̎��
	int			nShotTime;		// ���ˊԊu
	int			nRigorTime;		// �d������
	float		fSearchRange;	// ���G�͈�
	char		aModelPath		// ���f���̑��΃p�X
				[TXT_MAX];
	HitTestSet	hitTestSet;		// �����蔻��ݒ���
}Obj_turret_00Type;

// OBJ:�^���b�g[00] �̊Ǘ����\����
typedef struct
{
	OBJ_TURRET_00_CONTROL_STATE state;	// ���
}Obj_turret_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�^���b�g[00] �̃p�����[�^�[�̏���������
void InitParameterObj_turret_00(Obj_turret_00 *pObj);

// OBJ:�^���b�g[00] �̔j�󏈗�
void DestroyObj_turret_00(Obj_turret_00 *pObj);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_turret_00		// OBJ:�^���b�g[00] �̃e�N�X�`���ւ̃|�C���^
						[OBJ_TURRET_00_TYPE_NUM]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_turret_00		// OBJ:�^���b�g[00] �̃��b�V��(���_���)�ւ̃|�C���^
						[OBJ_TURRET_00_TYPE_NUM]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_turret_00		// OBJ:�^���b�g[00] �̃}�e���A���ւ̃|�C���^
						= NULL;
DWORD					g_aNumMatObj_turret_00		// OBJ:�^���b�g[00] �̃}�e���A���̐�
						[OBJ_TURRET_00_TYPE_NUM]
						= {};
D3DXMATRIX				g_mtxWorldObj_turret_00;	// OBJ:�^���b�g[00] �̃��[���h�}�g���b�N�X
Obj_turret_00			g_aObj_turret_00			// OBJ:�^���b�g[00] �̏��
						[OBJ_TURRET_00_MAX];
Obj_turret_00Control	g_obj_turret_00Control;		// OBJ:�^���b�g[00] �̊Ǘ����
Obj_turret_00Type		g_aObj_turret_00Type		// OBJ:�^���b�g[00] �̎�ޖ��̏��
						[OBJ_TURRET_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_turret_00�֐� - OBJ:�^���b�g[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_turret_00(Obj_turret_00 *pObj)
{
	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pObj->targetRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW����
	pObj->rotTemp				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̕ۑ�
	pObj->nType					= 0;								// ���
	pObj->bUse					= false;							// �g�p����Ă��邩�t���O
	pObj->nHP					=									// �q�b�g�|�C���g
	pObj->nHPMax				= 0;								// �q�b�g�|�C���g�̏��
	pObj->nCounterInvincible	= 0;								// ���G�J�E���^�[
	pObj->nCounterDamage		= 0;								// �_���[�W�J�E���^�[
	pObj->bAim					= false;							// �_���t���O
	pObj->nCounterShot			= 0;								// ���˃J�E���^�[
	pObj->nRigorCounter			= 0;								// �d���J�E���^�[
	pObj->hitTestInfo			= {};								// �����蔻��̊Ǘ����
}

//========================================
// InitParameterControlObj_turret_00�֐� - OBJ:�^���b�g[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterControlObj_turret_00(void) 
{
	Obj_turret_00Control	*pObjCtl	// OBJ:�^���b�g[00] �̊Ǘ����̃|�C���^
							= &g_obj_turret_00Control;

	pObjCtl->state = OBJ_TURRET_00_CONTROL_STATE_DYNAMIC;	// ���
}

//========================================
// DestroyObj_turret_00�֐� - OBJ:�^���b�g[00] �̔j�󏈗� -
//========================================
void DestroyObj_turret_00(Obj_turret_00 *pObj)
{
	pObj->bUse = false;				// �g�p����Ă��Ȃ���Ԃɂ���
	ItemPriceDownObj_pedestal_00(	// OBJ:���[00] �̏��i(�^���b�g)�̒l��������
		OBJ_PEDESTAL_00_ITEM_TURRET);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_turret_00�֐� - OBJ:�^���b�g[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_turret_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Obj_turret_00Type	*pObjType	// OBJ:�^���b�g[00] �̎�ޖ��̏��
						= g_aObj_turret_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_TURRET_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// �ǂݍ��݂��I��
		if (aDataSearch[0] == '#')			{ continue; }				// �܂�Ԃ�

		if (!strcmp(aDataSearch, "TYPE")) {
			// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "HIT_POINT:"))	{ fscanf(pFile, "%d", &pObjType->nHP); }			// �q�b�g�|�C���g
				else if (!strcmp(aDataSearch, "BULLET_TYPE:"))	{ fscanf(pFile, "%d", &pObjType->nBulletType); }	// �e�̎��
				else if (!strcmp(aDataSearch, "SHOT_TIME:"))	{ fscanf(pFile, "%d", &pObjType->nShotTime); }		// ���ˊԊu
				else if (!strcmp(aDataSearch, "RIGOR_TIME:"))	{ fscanf(pFile, "%d", &pObjType->nRigorTime); }		// �d������
				else if (!strcmp(aDataSearch, "SEARCH_RANGE:")) { fscanf(pFile, "%f", &pObjType->fSearchRange); }	// ���G�͈�
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pObjType->aModelPath); }		// ���f���̑��΃p�X
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitObj_turret_00�֐� - OBJ:�^���b�g[00] �̏��������� -
//========================================
void InitObj_turret_00(void)
{
	// �Ǘ����̃p�����[�^�[�̏���������
	InitParameterControlObj_turret_00();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_TURRET_00_TYPE_NUM; nCntType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			g_aObj_turret_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_turret_00,
			NULL,
			&g_aNumMatObj_turret_00[nCntType],
			&g_aMeshObj_turret_00[nCntType]);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_turret_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_turret_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_turret_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_turret_00	*pObj	// OBJ:�^���b�g[00] �̏��̃|�C���^
					= g_aObj_turret_00;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		// �p�����[�^�[�̏���������
		InitParameterObj_turret_00(pObj);
	}
}

//========================================
// UninitObj_turret_00�֐� - OBJ:�^���b�g[00] �̏I������ -
//========================================
void UninitObj_turret_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_TURRET_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_turret_00[nCntType] != NULL)
		{
			g_aMeshObj_turret_00[nCntType]->Release();
			g_aMeshObj_turret_00[nCntType] = NULL;
		}

		// �e�N�X�`���̔j��
		for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
			if (g_aTextureObj_turret_00[nCntType][nCntMat] != NULL)
			{
				g_aTextureObj_turret_00[nCntType][nCntMat]->Release();
				g_aTextureObj_turret_00[nCntType][nCntMat] = NULL;
			}
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_turret_00 != NULL)
	{
		g_pBuffMatObj_turret_00->Release();
		g_pBuffMatObj_turret_00 = NULL;
	}
}

//========================================
// UpdateObj_turret_00�֐� - OBJ:�^���b�g[00] �̍X�V���� -
//========================================
void UpdateObj_turret_00(void)
{
	Obj_turret_00			*pObj		// OBJ:�^���b�g[00] �̏��̃|�C���^
							= g_aObj_turret_00;
	Obj_turret_00Control	*pObjCtl	// OBJ:�^���b�g[00] �̊Ǘ����̃|�C���^
							= &g_obj_turret_00Control;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (pObjCtl->state != OBJ_TURRET_00_CONTROL_STATE_STATIC)
		{// �Ǘ���Ԃ��ÓI�Ŗ������A
			Obj_turret_00Type	*pType	// OBJ:�^���b�g[00] �̎�ޖ��̏��̃|�C���^
				= &g_aObj_turret_00Type[pObj->nType];

			if (!pObj->bAim)
			{// �_���t���O���U�̎��A
				Chr_enemy_00	*pChr = GetChr_enemy_00();	// CHR:�G[00] �̏��̃|�C���^���擾
				float			fNear = 0;					// �ł��߂�����
				D3DXVECTOR3		targetPos;					// �ڕW�ʒu
				bool			bAim = false;				// �_���t���O

				for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
				{
					if ((!pChr->bUse) || (pChr->bRecognitionOff))
					{// �g�p����Ă��Ȃ���� or �F��OFF�t���O���^�̎��A
						continue;	// �J��Ԃ�������܂�Ԃ�
					}

					float fDistance = FindDistanceLookDown(pObj->pos, pChr->pos);	// 2�_�̋������擾

					if ((fDistance < fNear) || (!bAim))
					{// 2�_�̋������ł��߂�������������Ă��� or �_���t���O���U�̎��A
						fNear = fDistance;		// �ł��߂�������ݒ�
						targetPos = pChr->pos;	// �ڕW�ʒu��ݒ�
						bAim = true;			// �_���t���O��^�ɂ���
					}
				}

				if ((fNear <= pType->fSearchRange) && (bAim))
				{// �ł��߂����������G�͈͓� & �_���t���O���^�̎��A
					pObj->rotTemp = pObj->rot;	// �������L��
					pObj->targetRot.y			// �ڕW������ݒ�
						= FindAngleLookDown(pObj->pos, targetPos);
					pObj->bAim = true;			// �_���t���O��^�ɂ���
					pObj->nCounterShot			// ���˃J�E���^�[��ݒ�
						= pType->nShotTime;
				}
			}

			if (pObj->nCounterShot > 0)
			{// ���˃J�E���^�[��0�������Ă��鎞�A
				pObj->nCounterShot--;	// ���˃J�E���^�[�����Z
				pObj->rot.y += (AngleDifference(pObj->rotTemp.y, pObj->targetRot.y) / (float)pType->nShotTime);

				if (pObj->nCounterShot <= 0)
				{// ���˃J�E���^�[��0�ȉ��̎��A
					pObj->nRigorCounter	// �d���J�E���^�[��ݒ�
						= pType->nRigorTime;
					SetAtk_bullet_00(	// ATK:�e[00] �̐ݒ菈��
						pObj->pos,
						pObj->rot,
						pType->nBulletType,
						ATK_BULLET_00_PARENTTYPE_PLAYER);
				}
			}

			if (pObj->nRigorCounter > 0)
			{// �d���J�E���^�[��0�������Ă��鎞�B
				if (--pObj->nRigorCounter <= 0)
				{// �d���J�E���^�[�����Z��������0�ȉ��̎��A
					pObj->bAim = false;	// �_���t���O���U�ɂ���
				}
			}

			if (pObj->nCounterInvincible > 0)
			{// ���G�J�E���^�[��0�������Ă��鎞�A
				pObj->nCounterInvincible--;	// ���G�J�E���^�[�����Z����
			}
			if (pObj->nCounterDamage > 0)
			{// �_���[�W�J�E���^�[��0�������Ă��鎞�A
				pObj->nCounterDamage--;	// �_���[�W�J�E���^�[�����Z����
			}

			{
				float fDitance = FindDistanceLookDown(pObj->pos, GetChr_player_00()->pos);
				if (fDitance <= CHR_PLAYER_00_HPRATE_DISPLAY_DISTANCE)
				{// CHR;�v���C���[[00] �܂ł̋�����HP������\�����鋗���ȉ��̎��AHP�����e�L�X�g��\��
					char aString[TXT_MAX];
					float fHPRate = ((float)pObj->nHP / (float)pObj->nHPMax);
					sprintf(aString, "%d%%", (int)(fHPRate * 100));
					SetText3D(
						aString,
						FONT_002,
						DISPLAY_CENTER,
						pObj->pos,
						{ 255,(int)(255 * fHPRate),(int)(255 * fHPRate),(int)(255 * (1.0f - ((float)fDitance / (float)CHR_PLAYER_00_HPRATE_DISPLAY_DISTANCE))) },
						CHR_PLAYER_00_HPRATE_TEXT_SCALE, CHR_PLAYER_00_HPRATE_TEXT_SCALE, false, false, false);
				}
			}
		}
		else 
		{// �Ǘ���Ԃ��ÓI�̎��A
			pObj->nCounterDamage = 0;	// �_���[�W�J�E���^�[������������
		}
	}
}

//========================================
// DrawObj_turret_00�֐� - OBJ:�^���b�g[00] �̕`�揈�� -
//========================================
void DrawObj_turret_00(void)
{
	Obj_turret_00		*pObj				// OBJ:�^���b�g[00] �̏��̃|�C���^
						= g_aObj_turret_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_turret_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldObj_turret_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_turret_00, &g_mtxWorldObj_turret_00, &mtxRot);
		
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_turret_00, &g_mtxWorldObj_turret_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_turret_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_turret_00[pObj->nType]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_TURRET_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_TURRET_00_DAMAGE_TIME))),
				255
			});

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_turret_00[pObj->nType][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_turret_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_turret_00�֐� - OBJ:�^���b�g[00] �̐ݒ菈�� -
//========================================
void SetObj_turret_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	Obj_turret_00	*pObj	// OBJ:�^���b�g[00] �̏��̃|�C���^
					= g_aObj_turret_00;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse) 
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_turret_00Type	*pType	// OBJ:�^���b�g[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_turret_00Type[pObj->nType];

		pObj->pos = pos;			// �ʒu����
		pObj->rot = rot;			// ��������
		pObj->bUse = true;			// �g�p����Ă����Ԃɂ���
		pObj->nHPMax = pType->nHP *	// �ő�HP��ݒ�
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HP��������
		pObj->bAim = false;			// �_���t���O��������
		pObj->nCounterShot = 0;		// ���˃J�E���^�[��������
		pObj->nRigorCounter = 0;	// �d���J�E���^�[��������

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// CollisionObj_turret_00�֐� - OBJ:�^���b�g[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_turret_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool			bHitTemp	// ������t���O��ۑ�
					= pObjCollision->bHit;
	Obj_turret_00	*pObj		// OBJ:�^���b�g[00] �̏��̃|�C���^
					= g_aObj_turret_00;
	int				nIndex		// CHR:�G[00] �̕Ԃ��̔ԍ�
					= -1;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		// �Փ˔���ɕK�v�ȏ��
		CollisionCheck(
			vector,
			pObjCollision,
			myCollInfo,
			{
				&pObj->pos,
				pObj->pos,
				NULL,
				&pObj->rot,
				pObj->rot,
				g_aObj_turret_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		if ((pObjCollision->bHit) && (!bHitTemp))
		{// ������t���O���^�̎��A
			nIndex = nCntObj;	// �Ԃ��̔ԍ���ݒ�
			bHitTemp = true;	// ������t���O�̕ۑ���^�ɂ���
		}
	}

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// �ԍ���Ԃ�
	return nIndex;
}

//========================================
// DamageObj_turret_00�֐� - OBJ:�^���b�g[00] �̃_���[�W���� -
//========================================
void DamageObj_turret_00(int nIndex, int nDamage) 
{
	Obj_turret_00	*pObj	// CHR:�G[00] �̏��̃|�C���^
					= &g_aObj_turret_00[nIndex];

	if (pObj->nCounterInvincible > 0)
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		return;	// �������I������
	}

	nDamage *= fDamageRand();	// �����␳
	pObj->nHP -= nDamage;		// �q�b�g�|�C���g����_���[�W�����Z
	pObj->nCounterDamage		// �_���[�W�J�E���^�[��ݒ�
		= OBJ_TURRET_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:�_���[�W[00] �̐ݒ菈��
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);

	if (pObj->nHP <= 0)
	{// �q�b�g�|�C���g��0�ȉ��̎��A
		DestroyObj_turret_00(pObj);	// �j�󏈗�
	}
	else
	{// �q�b�g�|�C���g��0�������Ă��鎞�A���G�J�E���^�[��ݒ�
		pObj->nCounterInvincible = OBJ_TURRET_00_INVINCIBLE_TIME;
	}
}

//========================================
// SetObj_turret_00ControlState�֐� - OBJ:�^���b�g[00] �̊Ǘ���Ԑݒ菈�� -
//========================================
void SetObj_turret_00ControlState(OBJ_TURRET_00_CONTROL_STATE state) 
{
	Obj_turret_00Control	*pObjCtl	// OBJ:�^���b�g[00] �̊Ǘ����̃|�C���^
							= &g_obj_turret_00Control;

	pObjCtl->state = state;	// ��Ԃ���
}

//========================================
// HealAllObj_turret_00�֐� - OBJ:�^���b�g[00] �̑S�ĉ񕜏��� -
//========================================
void HealAllObj_turret_00(void)
{
	Obj_turret_00	*pObj	// OBJ:�^���b�g[00] �̏��̃|�C���^
					= g_aObj_turret_00;

	for (int nCntObj = 0; nCntObj < OBJ_TURRET_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_turret_00Type	*pType	// OBJ:�^���b�g[00] �̎�ޖ��̏��
							= &g_aObj_turret_00Type[pObj->nType];

		pObj->nHPMax = pType->nHP *	// �ő�HP���X�V
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HP�ɍő�HP����
	}
}