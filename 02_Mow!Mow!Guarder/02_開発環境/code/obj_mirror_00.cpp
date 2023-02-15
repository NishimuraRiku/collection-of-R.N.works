//========================================
// 
// OBJ:�~���[[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_mirror_00.cpp ***
//========================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "obj_mirror_00.h"		// OBJ:�~���[		[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�~���[[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_MIRROR_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_MIRROR_00_TYPE_DATA.txt"

// OBJ:�~���[[00] �̍ő吔
// OBJ:�~���[[00] �̎�ނ̍ő吔
#define OBJ_MIRROR_00_MAX		(256)
#define OBJ_MIRROR_00_TYPE_NUM	(1)

// OBJ:�~���[[00] �̖��G����
#define OBJ_MIRROR_00_INVINCIBLE_TIME	(10)

// OBJ:�~���[[00] �̃_���[�W����
#define OBJ_MIRROR_00_DAMAGE_TIME	(10)

// OBJ:�~���[[00] �̉�]��
#define OBJ_MIRROR_00_SPIN	D3DXVECTOR3(0.0f,0.005f,0.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�~���[[00] �̏��\����
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3	pos;	// �ʒu

	// ���ފ֘A
	int nType;	// ���

	// ��Ԋ֘A
	bool	bUse;				// �g�p����Ă��邩�t���O
	int		nHP;				// �q�b�g�|�C���g
	int		nHPMax;				// �q�b�g�|�C���g�̏��
	int		nCounterInvincible;	// ���G�J�E���^�[
	int		nCounterDamage;		// �_���[�W�J�E���^�[

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
}Obj_mirror_00;

// OBJ:�~���[[00] �̊Ǘ����\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3	rot;	// ���݂̌���
	D3DXVECTOR3	rotOld;	// �ߋ��̌���
}Obj_mirror_00Control;

// OBJ:�~���[[00] �̎�ޖ��̏��\����
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
}Obj_mirror_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�~���[[00] �̃p�����[�^�[�̏���������
void InitParameterObj_mirror_00(Obj_mirror_00 *pObj);

// OBJ:�~���[[00] �̔j�󏈗�
void DestroyObj_mirror_00(Obj_mirror_00 *pObj);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_mirror_00		// OBJ:�~���[[00] �̃e�N�X�`���ւ̃|�C���^
						[OBJ_MIRROR_00_TYPE_NUM]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_mirror_00		// OBJ:�~���[[00] �̃��b�V��(���_���)�ւ̃|�C���^
						[OBJ_MIRROR_00_TYPE_NUM]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_mirror_00		// OBJ:�~���[[00] �̃}�e���A���ւ̃|�C���^
						= NULL;
DWORD					g_aNumMatObj_mirror_00		// OBJ:�~���[[00] �̃}�e���A���̐�
						[OBJ_MIRROR_00_TYPE_NUM]
						= {};
D3DXMATRIX				g_mtxWorldObj_mirror_00;	// OBJ:�~���[[00] �̃��[���h�}�g���b�N�X
Obj_mirror_00			g_aObj_mirror_00			// OBJ:�~���[[00] �̏��
						[OBJ_MIRROR_00_MAX];
Obj_mirror_00Control	g_obj_mirror_00Control;		// OBJ:�~���[[00] �̊Ǘ����
Obj_mirror_00Type		g_aObj_mirror_00Type		// OBJ:�~���[[00] �̎�ޖ��̏��
						[OBJ_MIRROR_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_mirror_00�֐� - OBJ:�~���[[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_mirror_00(Obj_mirror_00 *pObj)
{
	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pObj->nType					= 0;								// ���
	pObj->bUse					= false;							// �g�p����Ă��邩�t���O
	pObj->nHP					=									// �q�b�g�|�C���g
	pObj->nHPMax				= 0;								// �q�b�g�|�C���g�̏��
	pObj->nCounterInvincible	= 0;								// ���G�J�E���^�[
	pObj->nCounterDamage		= 0;								// �_���[�W�J�E���^�[
	pObj->hitTestInfo			= {};								// �����蔻��̊Ǘ����
}

//========================================
// InitParameterObj_mirror_00Control�֐� - OBJ:�~���[[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_mirror_00Control(void)
{
	Obj_mirror_00Control	*pObjCtl	// OBJ:�~���[[00] �̊Ǘ����̃|�C���^ 
							= &g_obj_mirror_00Control;

	pObjCtl->rot	= INITD3DXVECTOR3;	// ���݂̌���
	pObjCtl->rotOld = INITD3DXVECTOR3;	// �ߋ��̌���
}

//========================================
// DestroyObj_mirror_00�֐� - OBJ:�~���[[00] �̔j�󏈗� -
//========================================
void DestroyObj_mirror_00(Obj_mirror_00 *pObj)
{
	pObj->bUse = false;				// �g�p����Ă��Ȃ���Ԃɂ���
	ItemPriceDownObj_pedestal_00(	// OBJ:���[00] �̏��i(���ˑ��u)�̒l��������
		OBJ_PEDESTAL_00_ITEM_REFLECTOR);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_mirror_00�֐� - OBJ:�~���[[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_mirror_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Obj_mirror_00Type	*pObjType	// OBJ:�~���[[00] �̎�ޖ��̏��
						= g_aObj_mirror_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_MIRROR_00_TYPE_DATA_FILE_PATH, "r");

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
// InitObj_mirror_00�֐� - OBJ:�~���[[00] �̏��������� -
//========================================
void InitObj_mirror_00(void)
{
	// �Ǘ����̏���������
	InitParameterObj_mirror_00Control();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_MIRROR_00_TYPE_NUM; nCntType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			g_aObj_mirror_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_mirror_00,
			NULL,
			&g_aNumMatObj_mirror_00[nCntType],
			&g_aMeshObj_mirror_00[nCntType]);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_mirror_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_mirror_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_mirror_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_mirror_00	*pObj	// OBJ:�~���[[00] �̏��̃|�C���^
					= g_aObj_mirror_00;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		// �p�����[�^�[�̏���������
		InitParameterObj_mirror_00(pObj);
	}
}

//========================================
// UninitObj_mirror_00�֐� - OBJ:�~���[[00] �̏I������ -
//========================================
void UninitObj_mirror_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_MIRROR_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_mirror_00[nCntType] != NULL)
		{
			g_aMeshObj_mirror_00[nCntType]->Release();
			g_aMeshObj_mirror_00[nCntType] = NULL;
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_mirror_00 != NULL)
	{
		g_pBuffMatObj_mirror_00->Release();
		g_pBuffMatObj_mirror_00 = NULL;
	}
}

//========================================
// UpdateObj_mirror_00�֐� - OBJ:�~���[[00] �̍X�V���� -
//========================================
void UpdateObj_mirror_00(void)
{
	Obj_mirror_00			*pObj		// OBJ:�~���[[00] �̏��̃|�C���^
							= g_aObj_mirror_00;
	Obj_mirror_00Control	*pObjCtl	// OBJ:�~���[[00] �̊Ǘ����̃|�C���^ 
							= &g_obj_mirror_00Control;

	pObjCtl->rotOld = pObjCtl->rot;		// �ߋ��̌�����ۑ�
	pObjCtl->rot += OBJ_MIRROR_00_SPIN;	// ��]��������
	RotControl(&pObjCtl->rot);			// �����𐧌�

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		Obj_mirror_00Type	*pType	// OBJ:�~���[[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_mirror_00Type[pObj->nType];

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
}

//========================================
// DrawObj_mirror_00�֐� - OBJ:�~���[[00] �̕`�揈�� -
//========================================
void DrawObj_mirror_00(void)
{
	Obj_mirror_00		*pObj				// OBJ:�~���[[00] �̏��̃|�C���^
						= g_aObj_mirror_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_mirror_00->GetBufferPointer();

	Obj_mirror_00Control	*pObjCtl	// OBJ:�~���[[00] �̊Ǘ����̃|�C���^ 
							= &g_obj_mirror_00Control;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldObj_mirror_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObjCtl->rot.y, pObjCtl->rot.x, pObjCtl->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_mirror_00, &g_mtxWorldObj_mirror_00, &mtxRot);
		
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_mirror_00, &g_mtxWorldObj_mirror_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_mirror_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_mirror_00[pObj->nType]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, 
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_MIRROR_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_MIRROR_00_DAMAGE_TIME))),
				255
			});

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_mirror_00[pObj->nType][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_mirror_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_mirror_00�֐� - OBJ:�~���[[00] �̐ݒ菈�� -
//========================================
void SetObj_mirror_00(D3DXVECTOR3 pos)
{
	Obj_mirror_00	*pObj	// OBJ:�~���[[00] �̏��̃|�C���^
					= g_aObj_mirror_00;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_mirror_00Type	*pType	// OBJ:�~���[[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_mirror_00Type[pObj->nType];

		pObj->pos = pos;			// �ʒu����
		pObj->bUse = true;			// �g�p����Ă����Ԃɂ���
		pObj->nHPMax = pType->nHP *	// �ő�HP��ݒ�
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HP��������

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// CollisionObj_mirror_00�֐� - OBJ:�~���[[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_mirror_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool					bHitTemp	// ������t���O��ۑ�
							= pObjCollision->bHit;
	Obj_mirror_00			*pObj		// OBJ:�~���[[00] �̏��̃|�C���^
							= g_aObj_mirror_00;
	Obj_mirror_00Control	*pObjCtl	// OBJ:�~���[[00] �̊Ǘ����̃|�C���^ 
							= &g_obj_mirror_00Control;
	int						nIndex		// CHR:�G[00] �̕Ԃ��̔ԍ�
							= -1;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
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
				&pObjCtl->rot,
				pObjCtl->rotOld,
				g_aObj_mirror_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
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
// DamageObj_mirror_00�֐� - OBJ:�~���[[00] �̃_���[�W���� -
//========================================
void DamageObj_mirror_00(int nIndex, int nDamage) 
{
	Obj_mirror_00	*pObj	// CHR:�G[00] �̏��̃|�C���^
					= &g_aObj_mirror_00[nIndex];

	if (pObj->nCounterInvincible > 0)
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		return;	// �������I������
	}

	nDamage *= fDamageRand();	// �����␳
	pObj->nHP -= nDamage;		// �q�b�g�|�C���g����_���[�W�����Z
	pObj->nCounterDamage		// �_���[�W�J�E���^�[��ݒ�
		= OBJ_MIRROR_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:�_���[�W[00] �̐ݒ菈��
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);

	if (pObj->nHP <= 0)
	{// �q�b�g�|�C���g��0�ȉ��̎��A
		DestroyObj_mirror_00(pObj);	// �j�󏈗�
	}
	else
	{// �q�b�g�|�C���g��0�������Ă��鎞�A���G�J�E���^�[��ݒ�
		pObj->nCounterInvincible = OBJ_MIRROR_00_INVINCIBLE_TIME;
	}
}

//========================================
// HealAllObj_mirror_00�֐� - OBJ:���ˑ��u[00] �̑S�ĉ񕜏��� -
//========================================
void HealAllObj_mirror_00(void)
{
	Obj_mirror_00	*pObj	// OBJ:���ˑ��u[00] �̏��̃|�C���^
					= g_aObj_mirror_00;

	for (int nCntObj = 0; nCntObj < OBJ_MIRROR_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_mirror_00Type	*pType	// OBJ:���ˑ��u[00] �̎�ޖ��̏��
							= &g_aObj_mirror_00Type[pObj->nType];

		pObj->nHPMax = pType->nHP *	// �ő�HP���X�V
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HP�ɍő�HP����
	}
}