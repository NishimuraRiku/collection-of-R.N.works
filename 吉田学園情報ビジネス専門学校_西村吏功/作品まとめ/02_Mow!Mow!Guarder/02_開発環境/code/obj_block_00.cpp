//========================================
// 
// OBJ:�u���b�N[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_block_00.cpp ***
//========================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "eff_shadow_00.h"		// EFF:�e			[00]
#include "obj_block_00.h"		// OBJ:�u���b�N		[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�u���b�N[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_BLOCK_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_BLOCK_00_TYPE_DATA.txt"

// OBJ:�u���b�N[00] �̍ő吔
// OBJ:�u���b�N[00] �̎�ނ̍ő吔
#define OBJ_BLOCK_00_MAX		(256)
#define OBJ_BLOCK_00_TYPE_NUM	(1)

// OBJ:�u���b�N[00] �̖��G����
#define OBJ_BLOCK_00_INVINCIBLE_TIME	(10)

// OBJ:�u���b�N[00] �̃_���[�W����
#define OBJ_BLOCK_00_DAMAGE_TIME	(10)

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�u���b�N[00] �̏��\����
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3	pos;	// �ʒu
	D3DXVECTOR3	rot;	// ����

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
}Obj_block_00;

// OBJ:�u���b�N[00] �̎�ޖ��̏��\����
typedef struct
{
	int			nHP;		// �q�b�g�|�C���g
	char		aModelPath	// ���f���̑��΃p�X
				[TXT_MAX];
	HitTestSet	hitTestSet;	// �����蔻��ݒ���
}Obj_block_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�u���b�N[00] �̃p�����[�^�[�̏���������
void InitParameterObj_block_00(Obj_block_00 *pObj);

// OBJ:�u���b�N[00] �̔j�󏈗�
void DestroyObj_block_00(Obj_block_00 *pObj);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureObj_block_00	// OBJ:�u���b�N[00] �̃e�N�X�`���ւ̃|�C���^
					[OBJ_BLOCK_00_TYPE_NUM]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshObj_block_00		// OBJ:�u���b�N[00] �̃��b�V��(���_���)�ւ̃|�C���^
					[OBJ_BLOCK_00_TYPE_NUM]
					= {};
LPD3DXBUFFER		g_pBuffMatObj_block_00	// OBJ:�u���b�N[00] �̃}�e���A���ւ̃|�C���^
					= NULL;
DWORD				g_aNumMatObj_block_00	// OBJ:�u���b�N[00] �̃}�e���A���̐�
					[OBJ_BLOCK_00_TYPE_NUM]
					= {};
D3DXMATRIX			g_mtxWorldObj_block_00;	// OBJ:�u���b�N[00] �̃��[���h�}�g���b�N�X
Obj_block_00		g_aObj_block_00			// OBJ:�u���b�N[00] �̏��
					[OBJ_BLOCK_00_MAX];
Obj_block_00Type	g_aObj_block_00Type		// OBJ:�u���b�N[00] �̎�ޖ��̏��
					[OBJ_BLOCK_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_block_00�֐� - OBJ:�u���b�N[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_block_00(Obj_block_00 *pObj)
{
	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pObj->nType					= 0;								// ���
	pObj->bUse					= false;							// �g�p����Ă��邩�t���O
	pObj->nHP					=									// �q�b�g�|�C���g
	pObj->nHPMax				= 0;								// �q�b�g�|�C���g�̏��
	pObj->nCounterInvincible	= 0;								// ���G�J�E���^�[
	pObj->nCounterDamage		= 0;								// �_���[�W�J�E���^�[
	pObj->hitTestInfo			= {};								// �����蔻��̊Ǘ����
}

//========================================
// DestroyObj_block_00�֐� - OBJ:�u���b�N[00] �̔j�󏈗� -
//========================================
void DestroyObj_block_00(Obj_block_00 *pObj)
{
	pObj->bUse = false;				// �g�p����Ă��Ȃ���Ԃɂ���
	ItemPriceDownObj_pedestal_00(	// OBJ:���[00] �̏��i(�u���b�N)�̒l��������
		OBJ_PEDESTAL_00_ITEM_BLOCK);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_block_00�֐� - OBJ:�u���b�N[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_block_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Obj_block_00Type	*pObjType	// OBJ:�u���b�N[00] �̎�ޖ��̏��
						= g_aObj_block_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_BLOCK_00_TYPE_DATA_FILE_PATH, "r");

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
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pObjType->aModelPath); }		// ���f���̑��΃p�X
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitObj_block_00�֐� - OBJ:�u���b�N[00] �̏��������� -
//========================================
void InitObj_block_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
		= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_BLOCK_00_TYPE_NUM; nCntType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			g_aObj_block_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_block_00,
			NULL,
			&g_aNumMatObj_block_00[nCntType],
			&g_aMeshObj_block_00[nCntType]);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_block_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_block_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_block_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_block_00	*pObj	// OBJ:�u���b�N[00] �̏��̃|�C���^
					= g_aObj_block_00;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		// �p�����[�^�[�̏���������
		InitParameterObj_block_00(pObj);
	}
}

//========================================
// UninitObj_block_00�֐� - OBJ:�u���b�N[00] �̏I������ -
//========================================
void UninitObj_block_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_BLOCK_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_block_00[nCntType] != NULL)
		{
			g_aMeshObj_block_00[nCntType]->Release();
			g_aMeshObj_block_00[nCntType] = NULL;
		}

		// �e�N�X�`���̔j��
		for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
			if (g_aTextureObj_block_00[nCntType][nCntMat] != NULL)
			{
				g_aTextureObj_block_00[nCntType][nCntMat]->Release();
				g_aTextureObj_block_00[nCntType][nCntMat] = NULL;
			}
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_block_00 != NULL)
	{
		g_pBuffMatObj_block_00->Release();
		g_pBuffMatObj_block_00 = NULL;
	}
}

//========================================
// UpdateObj_block_00�֐� - OBJ:�u���b�N[00] �̍X�V���� -
//========================================
void UpdateObj_block_00(void)
{
	Obj_block_00	*pObj	// OBJ:�u���b�N[00] �̏��̃|�C���^
					= g_aObj_block_00;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
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
}

//========================================
// DrawObj_block_00�֐� - OBJ:�u���b�N[00] �̕`�揈�� -
//========================================
void DrawObj_block_00(void)
{
	Obj_block_00		*pObj				// OBJ:�u���b�N[00] �̏��̃|�C���^
						= g_aObj_block_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_block_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldObj_block_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_block_00, &g_mtxWorldObj_block_00, &mtxRot);
		
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_block_00, &g_mtxWorldObj_block_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_block_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_block_00[pObj->nType]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_BLOCK_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_BLOCK_00_DAMAGE_TIME))),
				255
			});

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_block_00[pObj->nType][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_block_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_block_00�֐� - OBJ:�u���b�N[00] �̐ݒ菈�� -
//========================================
void SetObj_block_00(D3DXVECTOR3 pos) 
{
	Obj_block_00		*pObj	// OBJ:�u���b�N[00] �̏��̃|�C���^
						= g_aObj_block_00;
	
	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse) 
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_block_00Type	*pType	// OBJ:�u���b�N[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_block_00Type[pObj->nType];

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
// CollisionObj_block_00�֐� - OBJ:�u���b�N[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_block_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool			bHitTemp	// ������t���O��ۑ�
					= pObjCollision->bHit;
	Obj_block_00	*pObj		// OBJ:�u���b�N[00] �̏��̃|�C���^
					= g_aObj_block_00;
	int				nIndex		// CHR:�G[00] �̕Ԃ��̔ԍ�
					= -1;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
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
				g_aObj_block_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
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
// DamageObj_block_00�֐� - OBJ:�u���b�N[00] �̃_���[�W���� -
//========================================
void DamageObj_block_00(int nIndex, int nDamage) 
{
	Obj_block_00	*pObj	// CHR:�G[00] �̏��̃|�C���^
					= &g_aObj_block_00[nIndex];

	if (pObj->nCounterInvincible > 0)
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		return;	// �������I������
	}

	nDamage *= fDamageRand();	// �����␳
	pObj->nHP -= nDamage;		// �q�b�g�|�C���g����_���[�W�����Z
	pObj->nCounterDamage		// �_���[�W�J�E���^�[��ݒ�
		= OBJ_BLOCK_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:�_���[�W[00] �̐ݒ菈��
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);

	if (pObj->nHP <= 0)
	{// �q�b�g�|�C���g��0�ȉ��̎��A
		DestroyObj_block_00(pObj);	// �j�󏈗�
	}
	else
	{// �q�b�g�|�C���g��0�������Ă��鎞�A���G�J�E���^�[��ݒ�
		pObj->nCounterInvincible = OBJ_BLOCK_00_INVINCIBLE_TIME;
	}
}

//========================================
// HealAllObj_block_00�֐� - OBJ:�u���b�N[00] �̑S�ĉ񕜏��� -
//========================================
void HealAllObj_block_00(void)
{
	Obj_block_00	*pObj	// OBJ:�u���b�N[00] �̏��̃|�C���^
					= g_aObj_block_00;

	for (int nCntObj = 0; nCntObj < OBJ_BLOCK_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_block_00Type	*pType	// OBJ:�u���b�N[00] �̎�ޖ��̏��
							= &g_aObj_block_00Type[pObj->nType];

		pObj->nHPMax = pType->nHP *	// �ő�HP���X�V
			(1.0f + (OBJ_PEDESTAL_00_UNIT_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp));
		pObj->nHP = pObj->nHPMax;	// HP�ɍő�HP����
	}
}