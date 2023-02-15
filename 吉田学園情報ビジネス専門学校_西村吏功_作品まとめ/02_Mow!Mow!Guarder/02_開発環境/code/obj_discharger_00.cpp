//========================================
// 
// OBJ:���d���u[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_discharger_00.cpp ***
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
#include "obj_discharger_00.h"	// OBJ:���d���u		[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:���d���u[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_DISCHARGER_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_DISCHARGER_00_TYPE_DATA.txt"

// OBJ:���d���u[00] �̍ő吔
// OBJ:���d���u[00] �̎�ނ̍ő吔
#define OBJ_DISCHARGER_00_MAX		(256)
#define OBJ_DISCHARGER_00_TYPE_NUM	(1)

// OBJ:���d���u[00] �̖��G����
#define OBJ_DISCHARGER_00_INVINCIBLE_TIME	(10)

// OBJ:���d���u[00] �̃_���[�W����
#define OBJ_DISCHARGER_00_DAMAGE_TIME	(10)

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:���d���u[00] �̏��\����
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
	int		nCounterShot;		// ���˃J�E���^�[

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
}Obj_discharger_00;

// OBJ:���d���u[00] �̎�ޖ��̏��\����
typedef struct
{
	int			nHP;			// �q�b�g�|�C���g
	int			nBulletType;	// �e�̎��
	int			nShotNum;		// ���ː�
	int			nShotTime;		// ���ˊԊu
	char		aModelPath		// ���f���̑��΃p�X
				[TXT_MAX];
	HitTestSet	hitTestSet;		// �����蔻��ݒ���
}Obj_discharger_00Type;

// OBJ:���d���u[00] �̊Ǘ����\����
typedef struct
{
	OBJ_DISCHARGER_00_CONTROL_STATE state;	// ���
}Obj_discharger_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:���d���u[00] �̃p�����[�^�[�̏���������
// OBJ:���d���u[00] �̊Ǘ����̃p�����[�^�[�̏���������
void InitParameterObj_discharger_00(Obj_discharger_00 *pObj);
void InitParameterObj_discharger_00Control(void);

// OBJ:���d���u[00] �̔j�󏈗�
void DestroyObj_discharger_00(Obj_discharger_00 *pObj);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9			g_aTextureObj_discharger_00		// OBJ:���d���u[00] �̃e�N�X�`���ւ̃|�C���^
							[OBJ_DISCHARGER_00_TYPE_NUM]
							[MATERIAL_3D_MAX]
							= {};
LPD3DXMESH					g_aMeshObj_discharger_00		// OBJ:���d���u[00] �̃��b�V��(���_���)�ւ̃|�C���^
							[OBJ_DISCHARGER_00_TYPE_NUM]
							= {};
LPD3DXBUFFER				g_pBuffMatObj_discharger_00		// OBJ:���d���u[00] �̃}�e���A���ւ̃|�C���^
							= NULL;
DWORD						g_aNumMatObj_discharger_00		// OBJ:���d���u[00] �̃}�e���A���̐�
							[OBJ_DISCHARGER_00_TYPE_NUM]
							= {};
D3DXMATRIX					g_mtxWorldObj_discharger_00;	// OBJ:���d���u[00] �̃��[���h�}�g���b�N�X
Obj_discharger_00			g_aObj_discharger_00			// OBJ:���d���u[00] �̏��
							[OBJ_DISCHARGER_00_MAX];
Obj_discharger_00Control	g_obj_discharger_00Control;		// OBJ:���d���u[00] �̊Ǘ����
Obj_discharger_00Type		g_aObj_discharger_00Type		// OBJ:���d���u[00] �̎�ޖ��̏��
							[OBJ_DISCHARGER_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_discharger_00�֐� - OBJ:���d���u[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_discharger_00(Obj_discharger_00 *pObj)
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
	pObj->nCounterShot			= 0;								// ���˃J�E���^�[
	pObj->hitTestInfo			= {};								// �����蔻��̊Ǘ����
}

//========================================
// InitParameterObj_discharger_00Control�֐� - OBJ:���d���u[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_discharger_00Control(void) 
{
	Obj_discharger_00Control	*pObjCtl 
								= &g_obj_discharger_00Control;

	pObjCtl->state = OBJ_DISCHARGER_00_CONTROL_STATE_DYNAMIC;	// ���
}

//========================================
// DestroyObj_discharger_00�֐� - OBJ:���d���u[00] �̔j�󏈗� -
//========================================
void DestroyObj_discharger_00(Obj_discharger_00 *pObj)
{
	pObj->bUse = false;				// �g�p����Ă��Ȃ���Ԃɂ���
	ItemPriceDownObj_pedestal_00(	// OBJ:���[00] �̏��i(���d���u)�̒l��������
		OBJ_PEDESTAL_00_ITEM_DISCHARGER);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_discharger_00�֐� - OBJ:���d���u[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_discharger_00(void)
{
	FILE					*pFile;		// �t�@�C���|�C���^
	char					aDataSearch	// �f�[�^�����p
							[TXT_MAX];
	Obj_discharger_00Type	*pObjType	// OBJ:���d���u[00] �̎�ޖ��̏��
							= g_aObj_discharger_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_DISCHARGER_00_TYPE_DATA_FILE_PATH, "r");

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
				else if (!strcmp(aDataSearch, "SHOT_NUM:"))		{ fscanf(pFile, "%d", &pObjType->nShotNum); }		// ���ː�
				else if (!strcmp(aDataSearch, "SHOT_TIME:"))	{ fscanf(pFile, "%d", &pObjType->nShotTime); }		// ���ˊԊu
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pObjType->aModelPath); }		// ���f���̑��΃p�X
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitObj_discharger_00�֐� - OBJ:���d���u[00] �̏��������� -
//========================================
void InitObj_discharger_00(void)
{
	// �Ǘ����̏���������
	InitParameterObj_discharger_00Control();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_DISCHARGER_00_TYPE_NUM; nCntType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			g_aObj_discharger_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_discharger_00,
			NULL,
			&g_aNumMatObj_discharger_00[nCntType],
			&g_aMeshObj_discharger_00[nCntType]);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_discharger_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_discharger_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_discharger_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_discharger_00	*pObj	// OBJ:���d���u[00] �̏��̃|�C���^
						= g_aObj_discharger_00;

	for (int nCntObj = 0; nCntObj < OBJ_DISCHARGER_00_MAX; nCntObj++, pObj++)
	{
		// �p�����[�^�[�̏���������
		InitParameterObj_discharger_00(pObj);
	}
}

//========================================
// UninitObj_discharger_00�֐� - OBJ:���d���u[00] �̏I������ -
//========================================
void UninitObj_discharger_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_DISCHARGER_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_discharger_00[nCntType] != NULL)
		{
			g_aMeshObj_discharger_00[nCntType]->Release();
			g_aMeshObj_discharger_00[nCntType] = NULL;
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_discharger_00 != NULL)
	{
		g_pBuffMatObj_discharger_00->Release();
		g_pBuffMatObj_discharger_00 = NULL;
	}
}

//========================================
// UpdateObj_discharger_00�֐� - OBJ:���d���u[00] �̍X�V���� -
//========================================
void UpdateObj_discharger_00(void)
{
	Obj_discharger_00			*pObj		// OBJ:���d���u[00] �̏��̃|�C���^
								= g_aObj_discharger_00;
	Obj_discharger_00Control	*pObjCtl	// OBJ:���d���u[00] �̊Ǘ����̃|�C���^
								= &g_obj_discharger_00Control;

	for (int nCntObj = 0; nCntObj < OBJ_DISCHARGER_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (pObjCtl->state != OBJ_DISCHARGER_00_CONTROL_STATE_STATIC)
		{// �Ǘ���Ԃ��ÓI�łȂ����A
			Obj_discharger_00Type	*pType	// OBJ:���d���u[00] �̎�ޖ��̏��̃|�C���^
									= &g_aObj_discharger_00Type[pObj->nType];

			if (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_RAID)
			{// OBJ:�X�e�[�W[00] �̏�Ԃ��P���̎��A
				if (--pObj->nCounterShot <= 0)
				{// ���˃J�E���^�[�����Z��������0�ȉ��̎��A
					for (int nCntDirection = 0; nCntDirection < pType->nShotNum; nCntDirection++)
					{
						SetAtk_bullet_00(	// ATK:�e[00] �̐ݒ菈��
							pObj->pos,
							D3DXVECTOR3(0.0f, -D3DX_PI + (nCntDirection * ((D3DX_PI * 2) / pType->nShotNum)), 0.0f),
							pType->nBulletType,
							ATK_BULLET_00_PARENTTYPE_PLAYER);
					}

					pObj->nCounterShot = pType->nShotTime;	// ���˃J�E���^�[��ݒ�
				}
			}
			else
			{// OBJ:�X�e�[�W[00] �̏�Ԃ��P���łȂ����A
				pObj->nCounterShot = pType->nShotTime;	// ���˃J�E���^�[��ݒ�
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
			pObj->nCounterDamage = 0;	// �_���[�W�J�E���^�[��ÓI�ɂ���
		}
	}
}

//========================================
// DrawObj_discharger_00�֐� - OBJ:���d���u[00] �̕`�揈�� -
//========================================
void DrawObj_discharger_00(void)
{
	Obj_discharger_00	*pObj				// OBJ:���d���u[00] �̏��̃|�C���^
						= g_aObj_discharger_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_discharger_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_DISCHARGER_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldObj_discharger_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_discharger_00, &g_mtxWorldObj_discharger_00, &mtxRot);
		
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_discharger_00, &g_mtxWorldObj_discharger_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_discharger_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_discharger_00[pObj->nType]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_DISCHARGER_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_DISCHARGER_00_DAMAGE_TIME))),
				255
			});

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_discharger_00[pObj->nType][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_discharger_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_discharger_00�֐� - OBJ:���d���u[00] �̐ݒ菈�� -
//========================================
void SetObj_discharger_00(D3DXVECTOR3 pos)
{
	Obj_discharger_00	*pObj	// OBJ:���d���u[00] �̏��̃|�C���^
						= g_aObj_discharger_00;
	
	for (int nCntObj = 0; nCntObj < OBJ_DISCHARGER_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse) 
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_discharger_00Type	*pType	// OBJ:���d���u[00] �̎�ޖ��̏��̃|�C���^
								= &g_aObj_discharger_00Type[pObj->nType];

		pObj->pos = pos;			// �ʒu����
		pObj->bUse = true;			// �g�p����Ă����Ԃɂ���
		pObj->nHPMax = pType->nHP *	// �ő�HP��ݒ�
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HP��������
		pObj->nCounterShot = 0;		// ���˃J�E���^�[��������

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// CollisionObj_discharger_00�֐� - OBJ:���d���u[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_discharger_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool				bHitTemp	// ������t���O��ۑ�
						= pObjCollision->bHit;
	Obj_discharger_00	*pObj		// OBJ:���d���u[00] �̏��̃|�C���^
						= g_aObj_discharger_00;
	int					nIndex		// CHR:�G[00] �̕Ԃ��̔ԍ�
						= -1;

	for (int nCntObj = 0; nCntObj < OBJ_DISCHARGER_00_MAX; nCntObj++, pObj++)
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
				g_aObj_discharger_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
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
// DamageObj_discharger_00�֐� - OBJ:���d���u[00] �̃_���[�W���� -
//========================================
void DamageObj_discharger_00(int nIndex, int nDamage) 
{
	Obj_discharger_00	*pObj	// CHR:�G[00] �̏��̃|�C���^
						= &g_aObj_discharger_00[nIndex];

	if (pObj->nCounterInvincible > 0)
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		return;	// �������I������
	}

	nDamage *= fDamageRand();	// �����␳
	pObj->nHP -= nDamage;		// �q�b�g�|�C���g����_���[�W�����Z
	pObj->nCounterDamage 		// �_���[�W�J�E���^�[��ݒ�
		= OBJ_DISCHARGER_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:�_���[�W[00] �̐ݒ菈��
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);

	if (pObj->nHP <= 0)
	{// �q�b�g�|�C���g��0�ȉ��̎��A
		DestroyObj_discharger_00(pObj);	// �j�󏈗�
	}
	else
	{// �q�b�g�|�C���g��0�������Ă��鎞�A���G�J�E���^�[��ݒ�
		pObj->nCounterInvincible = OBJ_DISCHARGER_00_INVINCIBLE_TIME;
	}
}

//========================================
// SetObj_discharger_00ControlState�֐� - OBJ:���d���u[00] �̊Ǘ���Ԑݒ菈�� -
//========================================
void SetObj_discharger_00ControlState(OBJ_DISCHARGER_00_CONTROL_STATE state) 
{
	Obj_discharger_00Control	*pObjCtl	// OBJ:���d���u[00] �̊Ǘ����̃|�C���^
								= &g_obj_discharger_00Control;

	pObjCtl->state = state;	// ��Ԃ���
}

//========================================
// HealAllObj_discharger_00�֐� - OBJ:���d���u[00] �̑S�ĉ񕜏��� -
//========================================
void HealAllObj_discharger_00(void)
{
	Obj_discharger_00		*pObj	// OBJ:���d���u[00] �̏��̃|�C���^
							= g_aObj_discharger_00;
	
	for (int nCntObj = 0; nCntObj < OBJ_DISCHARGER_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_discharger_00Type	*pType	// OBJ:���d���u[00] �̎�ޖ��̏��
								= &g_aObj_discharger_00Type[pObj->nType];

		pObj->nHPMax = pType->nHP *	// �ő�HP���X�V
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HP�ɍő�HP����
	}
}