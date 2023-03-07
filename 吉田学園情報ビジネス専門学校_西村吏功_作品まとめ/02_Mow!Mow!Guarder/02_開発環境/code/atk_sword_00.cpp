//========================================
// 
// ATK:��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** atk_sword_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "atk_sword_00.h"	// ATK:��			[00]
#include "chr_enemy_00.h"	// CHR:�G			[00]
#include "chr_player_00.h"	// CHR:�v���C���[	[00]
#include "eff_light_00.h"	// EFF:��			[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// ATK:��[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define ATK_SWORD_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATTACK\\ATK_SWORD_00_TYPE_DATA.txt"

// ATK:��[00] �̎�ނ̐�
#define ATK_SWORD_00_TYPE_NUM	(1)

// TK:��[00] �̑��Ίp�x
#define ATK_SWORD_00_RELATIVE_ROT	(D3DXVECTOR3(0.0f,D3DX_PI,0.0f))

// ATK:��[00] �̑��Έʒu
// ATK:��[00] �̃v���C���[����̋���
#define ATK_SWORD_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 2.0f, 0.0f))
#define ATK_SWORD_00_LENGTH_PLAYER	(10.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// ATK:��[00] �̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum
{
	ATK_SWORD_00_HITOBJ_CHR_ENEMY_00,	// CHR:�G	[00]
	ATK_SWORD_00_HITOBJ_MAX,
}ATK_SWORD_00_HITOBJ;

//****************************************
// �\���̂̒�`
//****************************************
// ATK:��[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// ���݂̈ʒu
	D3DXVECTOR3 posOld;	// �ߋ��̈ʒu
	D3DXVECTOR3 rot;	// ����

	// ���ޏ��
	int nType;	// ���

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
	Collision	cmnCollision; 	// ���ʂ̏Փˏ��
	Collision	aObjCollision	// �I�u�W�F�N�g���̏Փˏ��
				[ATK_SWORD_00_HITOBJ_MAX];

	// ��Ԋ֘A
	int					nAttackIndex;	// �U������I�u�W�F�N�g�̔ԍ�
	ATK_SWORD_00_HITOBJ	attackObj;		// �U������I�u�W�F�N�g�̕���
}Atk_sword_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ATK:��[00] �̃p�����[�^�[�̏���������
void InitParameterAtk_sword_00(void);

// ATK:��[00] �̈ʒu�X�V����
// ATK:��[00] �̏Փ˃`�F�b�N����
// ATK:��[00] �̏Փˏ��ɉ���������
void UpdatePosAtk_sword_00(void);
void CheckCollisionAtk_sword_00(VECTOR vector);
void CollisionProcessAtk_sword_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureAtk_sword_00	// ATK:��[00] �̃e�N�X�`���ւ̃|�C���^
					[ATK_SWORD_00_TYPE_NUM]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshAtk_sword_00		// ATK:��[00] �̃��b�V��(���_���)�ւ̃|�C���^
					[ATK_SWORD_00_TYPE_NUM]
					= {};
LPD3DXBUFFER		g_pBuffMatAtk_sword_00	// ATK:��[00] �̃}�e���A���ւ̃|�C���^
					= NULL;
DWORD				g_aNumMatAtk_sword_00	// ATK:��[00] �̃}�e���A���̐�
					[ATK_SWORD_00_TYPE_NUM]
					= {};
D3DXMATRIX			g_mtxWorldAtk_sword_00;	// ATK:��[00] �̃��[���h�}�g���b�N�X
Atk_sword_00		g_atk_sword_00;			// ATK:��[00] �̏��
Atk_sword_00Type	g_aAtk_sword_00Type		// ATK:��[00] �̎�ޖ��̏��
					[ATK_SWORD_00_TYPE_NUM];

//========== *** ATK:��[00] �̎�ޖ��̏����擾 ***
Atk_sword_00Type *GetAtk_sword_00Type(void) 
{
	return g_aAtk_sword_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterAtk_sword_00�֐� - ATK:��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterAtk_sword_00(void) 
{
	Atk_sword_00	*pAtk	// ATK:��[00] �̏��̃|�C���^
					= &g_atk_sword_00;

	pAtk->pos			= INITD3DXVECTOR3;			// ���݂̈ʒu
	pAtk->posOld		= INITD3DXVECTOR3;			// �ߋ��̈ʒu
	pAtk->rot			= INITD3DXVECTOR3;			// ����
	pAtk->nType			= 0;						// ���
	pAtk->hitTestInfo	= {};						// �����蔻��̊Ǘ����
	pAtk->cmnCollision	= {};						// ���ʂ̏Փˏ��
	for (int nCntHitAtk = 0; nCntHitAtk < ATK_SWORD_00_HITOBJ_MAX; nCntHitAtk++) 
	{
		pAtk->aObjCollision[nCntHitAtk]	= {};		// �I�u�W�F�N�g���̏Փˏ��
	}
	pAtk->nAttackIndex	= -1;						// �U������I�u�W�F�N�g�̔ԍ�
	pAtk->attackObj		= (ATK_SWORD_00_HITOBJ)0;	// �U������I�u�W�F�N�g�̕���
}

//========================================
// UpdatePosAtk_sword_00�֐� - ATK:��[00] �̈ʒu�X�V���� -
//========================================
void UpdatePosAtk_sword_00(void)
{
	Atk_sword_00		*pAtk	// ATK:��[00] �̏��̃|�C���^
						= &g_atk_sword_00;
	Atk_sword_00Type	*pType	// ATK:��[00] �̎�ޖ��̏��
						= &g_aAtk_sword_00Type[pAtk->nType];

	// ����
	float fRate = ((float)GetChr_player_00()->nCounterAttack / (float)CHR_PLAYER_00_SLASH_TIME);

	pAtk->rot.y =
		GetChr_player_00()->rot.y - (CHR_PLAYER_00_SLASH_INSIDE * 0.5f)
		+ (CHR_PLAYER_00_SLASH_INSIDE * fRate);
	pAtk->pos = GetChr_player_00()->pos;
	pAtk->pos.x += sinf(pAtk->rot.y) * ATK_SWORD_00_LENGTH_PLAYER;
	pAtk->pos.z += cosf(pAtk->rot.y) * ATK_SWORD_00_LENGTH_PLAYER;

	// ���ʂ̏Փˏ���������
	pAtk->cmnCollision = {};

	// �I�u�W�F�N�g���̏Փˏ���������
	for (int nCntHitObj = 0; nCntHitObj < ATK_SWORD_00_HITOBJ_MAX; nCntHitObj++)
	{
		pAtk->aObjCollision[nCntHitObj] = {};
	}

	pAtk->nAttackIndex = -1;	// �U������I�u�W�F�N�g�̔ԍ���������

	CheckCollisionAtk_sword_00(VECTOR_X);	// X�����̏Փ˃`�F�b�N����
	CheckCollisionAtk_sword_00(VECTOR_Z);	// Z�����̏Փ˃`�F�b�N����

	// ATK:��[00] �̏Փˏ��ɉ���������
	CollisionProcessAtk_sword_00();

	SetEff_light_00(pAtk->pos,		// EFF:��[00] �̐ݒ菈��
		pType->nLightType, fRate);
}

//========================================
// CheckCollisionAtk_sword_00�֐� - ATK:��[00] �̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionAtk_sword_00(VECTOR vector)
{
	Atk_sword_00	*pAtk	// ATK:��[00] �̏��̃|�C���^
					= &g_atk_sword_00;
	
	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo	myCollInfo =
	{
		&pAtk->pos,
		pAtk->posOld,
		NULL,
		&pAtk->rot,
		pAtk->rot,
		g_aAtk_sword_00Type[0].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
	};

	// �Փ˔���(�U���Ώ�)
	{
		int nIndex;	// �ԍ�

		nIndex = CollisionChr_enemy_00(vector, &pAtk->aObjCollision[ATK_SWORD_00_HITOBJ_CHR_ENEMY_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// CHR:�G	[00] �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_SWORD_00_HITOBJ_CHR_ENEMY_00;
		}
	}
}

//========================================
// CollisionProcessAtk_sword_00�֐� - ATK:��[00] �̏Փˏ��ɉ��������� -
//========================================
void CollisionProcessAtk_sword_00(void)
{
	Atk_sword_00		*pAtk	// ATK:��[00] �̏��̃|�C���^
						= &g_atk_sword_00;
	Atk_sword_00Type	*pType	// ATK:��[00] �̎�ޖ��̏��
						= &g_aAtk_sword_00Type[pAtk->nType];

	if (pAtk->nAttackIndex != -1)
	{// �U������I�u�W�F�N�g�̔ԍ���-1�Ŗ������A
		// �U������I�u�W�F�N�g�̕��ނɉ������_���[�W����
		switch (pAtk->attackObj)
		{
  			case /*/ CHR:�G	[00] /*/ATK_SWORD_00_HITOBJ_CHR_ENEMY_00:
				DamageChr_enemy_00(pAtk->nAttackIndex, GetChr_player_00Type()[GetChr_player_00()->nType].nSlashDamage * (1.0f + (GetChr_player_00()->nSwordAtkUpCount * CHR_PLAYER_00_SWORD_ATK_ADDRATE)));
				KnockBackChr_enemy_00(pAtk->nAttackIndex, pAtk->rot,
					pType->fKnockBack * 
					((float)GetChr_player_00Type()[GetChr_player_00()->nType].nSlashDamage / 
					(float)GetChr_enemy_00Type()[GetChr_enemy_00()[pAtk->nAttackIndex].nType].nHP));
				break;
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadAtk_sword_00�֐� - ATK:��[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadAtk_sword_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Atk_sword_00Type	*pAtkType	// ATK:��[00] �̎�ޖ��̏��
						= g_aAtk_sword_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(ATK_SWORD_00_TYPE_DATA_FILE_PATH, "r");

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
			// ATK:��[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pAtkType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "KNOCK_BACK:"))	{ fscanf(pFile, "%f", &pAtkType->fKnockBack); }		// �m�b�N�o�b�N
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pAtkType->aModelPath); }		// ���f���̑��΃p�X
				else if (!strcmp(aDataSearch, "LIGHT_TYPE:"))	{ fscanf(pFile, "%d", &pAtkType->nLightType); }		// ���̎��
				else if (!strcmp(aDataSearch, "SLASH_SE:"))		{ fscanf(pFile, "%d", &pAtkType->slashSE); }		// �a��SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pAtkType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitAtk_sword_00�֐� - ATK:��[00] �̏��������� -
//========================================
void InitAtk_sword_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < ATK_SWORD_00_TYPE_NUM; nCntType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			g_aAtk_sword_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatAtk_sword_00,
			NULL,
			&g_aNumMatAtk_sword_00[nCntType],
			&g_aMeshAtk_sword_00[nCntType]);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatAtk_sword_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatAtk_sword_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureAtk_sword_00[nCntType][nCntMat]);
			}
		}
	}

	// �p�����[�^�[�̏���������
	InitParameterAtk_sword_00();
}

//========================================
// UninitAtk_sword_00�֐� - ATK:��[00] �̏I������ -
//========================================
void UninitAtk_sword_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < ATK_SWORD_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshAtk_sword_00[nCntType] != NULL)
		{
			g_aMeshAtk_sword_00[nCntType]->Release();
			g_aMeshAtk_sword_00[nCntType] = NULL;
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatAtk_sword_00 != NULL)
	{
		g_pBuffMatAtk_sword_00->Release();
		g_pBuffMatAtk_sword_00 = NULL;
	}
}

//========================================
// UpdateAtk_sword_00�֐� - ATK:��[00] �̍X�V���� -
//========================================
void UpdateAtk_sword_00(void)
{
	Atk_sword_00	*pAtk	// ATK:��[00] �̏��̃|�C���^
					= &g_atk_sword_00;

	if (!GetChr_player_00()->bSlash) 
	{// CHR:�v���C���[[00] �̎a���t���O���U�̎��A
		return;	// �������I������
	}

	UpdatePosAtk_sword_00();	// �ʒu�X�V����
	pAtk->posOld = pAtk->pos;	// �ߋ��̈ʒu��ۑ�
}

//========================================
// DrawAtk_sword_00�֐� - ATK:��[00] �̕`�揈�� -
//========================================
void DrawAtk_sword_00(void)
{
	if (!GetChr_player_00()->bSlash)
	{// CHR:�v���C���[[00] �̎a���t���O���U�̎��A
		return;	// �������I������
	}

	Atk_sword_00		*pAtk				// ATK:��[00] �̏��̃|�C���^
						= &g_atk_sword_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldAtk_sword_00);
	
	// �����𔽉f
	{
		D3DXVECTOR3 setRot = pAtk->rot + ATK_SWORD_00_RELATIVE_ROT;	// �ݒ����

		D3DXMatrixRotationYawPitchRoll(&mtxRot, setRot.y, setRot.x, setRot.z);
		D3DXMatrixMultiply(&g_mtxWorldAtk_sword_00, &g_mtxWorldAtk_sword_00, &mtxRot);
	}
	// �ʒu�𔽉f
	{
		D3DXVECTOR3 setPos = pAtk->pos + ATK_SWORD_00_RELATIVE_POS;	// �ݒ�ʒu

		D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
		D3DXMatrixMultiply(&g_mtxWorldAtk_sword_00, &g_mtxWorldAtk_sword_00, &mtxTrans);
	}
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAtk_sword_00);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatAtk_sword_00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aNumMatAtk_sword_00[pAtk->nType]; nCntMat++) 
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureAtk_sword_00[pAtk->nType][nCntMat]);

		// �|���S��(�p�[�c)�̕`��
		g_aMeshAtk_sword_00[pAtk->nType]->DrawSubset(nCntMat);
	}
}

//========================================
// CollisionAtk_sword_00�֐� - ATK:��[00] �Ƃ̓����蔻�菈�� -
//========================================
void CollisionAtk_sword_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool			bHitTemp	// ������t���O��ۑ�
					= pObjCollision->bHit;
	Atk_sword_00	*pAtk		// ATK:��[00] �̏��̃|�C���^
					= &g_atk_sword_00;

	if (!GetChr_player_00()->bSlash)
	{// CHR:�v���C���[[00] �̎a���t���O���U�̎��A
		return;	// �������I������
	}

	// �Փ˔���ɕK�v�ȏ��
	CollisionCheck(
		vector,
		pObjCollision,
		myCollInfo,
		{
			&pAtk->pos,
			pAtk->posOld,
			NULL,
			&pAtk->rot,
			pAtk->rot,
			g_aAtk_sword_00Type[pAtk->nType].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
		},
		COLLCHK_MODE_LOOKDOWN);

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pObjCollision);
}