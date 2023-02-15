//========================================
// 
// ATK:�Ռ�[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** atk_impact_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "atk_impact_00.h"		// ATK:�Ռ�			[00]
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "md_game_00.h"			// MD :�Q�[��		[00]
#include "obj_block_00.h"		// OBJ:�u���b�N		[00]
#include "obj_core_00.h"		// OBJ:�R�A			[00]
#include "obj_discharger_00.h"	// OBJ:���d���u		[00]
#include "obj_mirror_00.h"		// OBJ:�~���[		[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include "obj_turret_00.h"		// OBJ:�^���b�g		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// ATK:�Ռ�[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define ATK_IMPACT_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATTACK\\ATK_IMPACT_00_TYPE_DATA.txt"

// ATK:�Ռ�[00] �̍ő吔
// ATK:�Ռ�[00] �̎�ނ̍ő吔
#define ATK_IMPACT_00_MAX		(128)
#define ATK_IMPACT_00_TYPE_MAX	(8)

// ATK:�Ռ�[00] �̖@���x�N�g��
// ATK:�Ռ�[00] �̑��Έʒu
#define ATK_IMPACT_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define ATK_IMPACT_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 4.0f, 0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************
// ATK:�Ռ�[00] �̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum 
{
	ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00,		// CHR:�G			[00]
	ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00,		// CHR:�v���C���[	[00]
	ATK_IMPACT_00_HITOBJ_OBJ_CORE_00,		// OBJ:�R�A			[00]
	ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:�~���[		[00]
	ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:�u���b�N		[00]
	ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:���d���u		[00]
	ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00,		// OBJ:�^���b�g		[00]
	ATK_IMPACT_00_HITOBJ_MAX,
}ATK_IMPACT_00_HITOBJ;

// ATK:�Ռ�[00] �̑����ԍ�
typedef enum
{
	ATK_IMPACT_00_ELEM_NONE,	// ����
	ATK_IMPACT_00_ELEM_MAX,
}ATK_IMPACT_00_ELEM;

//****************************************
// �\���̂̒�`
//****************************************
// ATK:�Ռ�[00] �̎�ޖ��̏��\����
typedef struct
{
	int					nLife;			// ����
	int					nDamage;		// �_���[�W
	float				fKnockBack;		// �m�b�N�o�b�N
	ATK_IMPACT_00_ELEM	elem;			// ����
	SOUND_LABEL			setSE;			// �ݒ�SE
	HitTestSet			hitTestSet;		// �����蔻��ݒ���
}Atk_impact_00Type;

// ATK:�Ռ�[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����

	// �^���֘A
	HitTestInfo	hitTestInfo;	// �����蔻��̊Ǘ����
	Collision	cmnCollision; 	// ���ʂ̏Փˏ��
	Collision	aObjCollision	// �I�u�W�F�N�g���̏Փˏ��
				[ATK_IMPACT_00_HITOBJ_MAX];

	// ���ފ֘A
	int							nType;		// ���
	ATK_IMPACT_00_PARENTTYPE	parentType;	// �e�̎��

	// ��Ԋ֘A
	int						nLife;			// ����
	bool					bUse;			// �g�p����Ă��邩�t���O
	int						nDamage;		// �_���[�W
	int						nAttackIndex;	// �U������I�u�W�F�N�g�̔ԍ�
	ATK_IMPACT_00_HITOBJ	attackObj;		// �U������I�u�W�F�N�g�̕���
}Atk_impact_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ATK:�e[00] �̃p�����[�^�[�̏���������
void InitParameterAtk_impact_00(Atk_impact_00 *pAtk);

// ATK:�Ռ�[00] �̏Փ˃`�F�b�N����
// ATK:�Ռ�[00] �̏Փˏ��ɉ���������
void CheckCollisionAtk_impact_00(Atk_impact_00 *pAtk, VECTOR vector);
void CollisionProcessAtk_impact_00(Atk_impact_00 *pAtk);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureAtk_impact_00		// ATK:�Ռ�[00] �̃e�N�X�`���ւ̃|�C���^
						[ATK_IMPACT_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_impact_00		// ATK:�Ռ�[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldAtk_impact_00;	// ATK:�Ռ�[00] �̃��[���h�}�g���b�N�X
Atk_impact_00Type		g_aAtk_impact_00Type		// ATK:�Ռ�[00] �̎�ޖ��̏��
						[ATK_IMPACT_00_TYPE_MAX];
Atk_impact_00			g_aAtk_impact_00			// ATK:�Ռ�[00] �̏��
						[ATK_IMPACT_00_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterAtk_impact_00�֐� - ATK:�e[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterAtk_impact_00(Atk_impact_00 *pAtk)
{
	pAtk->pos			= INITD3DXVECTOR3;			// �ʒu
	pAtk->rot			= INITD3DXVECTOR3;			// ����
	pAtk->hitTestInfo	= {};						// �����蔻��̊Ǘ����
	pAtk->cmnCollision	= {};						// ���ʂ̏Փˏ��
	for (int nCntHitObj = 0; nCntHitObj < ATK_IMPACT_00_HITOBJ_MAX; nCntHitObj++)
	{
		pAtk->aObjCollision[nCntHitObj] = {};		// �I�u�W�F�N�g���̏Փˏ��
	}
	pAtk->nType			= 0;						// ���
	pAtk->nLife			= 0;						// ����
	pAtk->bUse			= false;					// �g�p����Ă��邩�t���O
	pAtk->nDamage		= 0;						// �_���[�W
	pAtk->nAttackIndex	= 0;						// �U������I�u�W�F�N�g�̔ԍ�
	pAtk->attackObj		= (ATK_IMPACT_00_HITOBJ)0;	// �U������I�u�W�F�N�g�̕���
}

//========================================
// UpdatePosAtk_impact_00�֐� - ATK:�Ռ�[00] �̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionAtk_impact_00(Atk_impact_00 *pAtk, VECTOR vector)
{
	Atk_impact_00Type	*pType	// ATK:�Ռ�[00] �̎�ޖ��̏��
						= &g_aAtk_impact_00Type[pAtk->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo	myCollInfo =
	{
		&pAtk->pos,
		pAtk->pos,
		NULL,
		&pAtk->rot,
		pAtk->rot,
		g_aAtk_impact_00Type[pAtk->nType].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
	};

	// �Փ˔���(�U���Ώ�)
	switch (pAtk->parentType)
	{
		//========== *** �v���C���[ ***
	case ATK_IMPACT_00_PARENTTYPE_PLAYER:
	{
		int nIndex;	// �ԍ�

		nIndex = CollisionChr_enemy_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// CHR:�G	[00] �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00;
			break;
		}
	}
		break;
		//========== *** �G ***
	case ATK_IMPACT_00_PARENTTYPE_ENEMY:
	{
		int nIndex;	// �ԍ�

		nIndex = /*/ CHR:�v���C���[	[00] /*/CollisionChr_player_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00;
			break;
		}
		nIndex = /*/ OBJ:�u���b�N	[00] /*/CollisionObj_block_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00;
			break;
		}
		nIndex = /*/ OBJ:�^���b�g	[00] /*/CollisionObj_turret_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00;
		}
		nIndex = /*/ OBJ:���d���u	[00] /*/CollisionObj_discharger_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00;
		}
		nIndex = /*/  OBJ:�R�A		[00] /*/CollisionObj_core_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_CORE_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_IMPACT_00_HITOBJ_OBJ_CORE_00;
			break;
		}
		nIndex = /*/  OBJ:�~���[	[00] /*/CollisionObj_mirror_00(vector, &pAtk->aObjCollision[ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00;
			break;
		}
	}
		break;
	}
}

//========================================
// CollisionProcessAtk_impact_00�֐� - ATK:�Ռ�[00] �̏Փˏ��ɉ��������� -
//========================================
void CollisionProcessAtk_impact_00(Atk_impact_00 *pAtk)
{
	Atk_impact_00Type	*pType	// ATK:�Ռ�[00] �̎�ޖ��̏��
						= &g_aAtk_impact_00Type[pAtk->nType];

	if (pAtk->nAttackIndex != -1) 
	{// �U������I�u�W�F�N�g�̔ԍ���-1�Ŗ������A
		// �U������I�u�W�F�N�g�̕��ނɉ������_���[�W����
		switch (pAtk->attackObj)
		{
			case /*/ CHR:�G			[00] /*/ATK_IMPACT_00_HITOBJ_CHR_ENEMY_00:
				DamageChr_enemy_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ CHR:�v���C���[	[00] /*/ATK_IMPACT_00_HITOBJ_CHR_PLAYER_00:
				DamageChr_player_00(pAtk->nDamage);
				KnockBackChr_player_00(
					D3DXVECTOR3(
						0.0f,
						FindAngleLookDown(pAtk->pos, GetChr_player_00()->pos),
						0.0f),
					pType->fKnockBack);
				break;
			case /*/ OBJ:�u���b�N	[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_BLOCK_00:
				DamageObj_block_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:�^���b�g	[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_TURRET_00:
				DamageObj_turret_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:���d���u	[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_DISCHARGER_00:
				DamageObj_discharger_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:�R�A		[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_CORE_00:
				DamageObj_core_00(pAtk->nDamage);
				break;
			case /*/ OBJ:�~���[		[00] /*/ATK_IMPACT_00_HITOBJ_OBJ_MIRROR_00:
				DamageObj_mirror_00(pAtk->nAttackIndex, pAtk->nDamage);
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
// LoadAtk_impact_00�֐� - ATK:�Ռ�[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadAtk_impact_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Atk_impact_00Type	*pAtkType	// ATK:�Ռ�[00] �̎�ޖ��̏��
						= g_aAtk_impact_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(ATK_IMPACT_00_TYPE_DATA_FILE_PATH, "r");

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
			// ATK:�Ռ�[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pAtkType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "LIFE:"))			{ fscanf(pFile, "%d", &pAtkType->nLife); }			// ����
				else if (!strcmp(aDataSearch, "DAMAGE:"))		{ fscanf(pFile, "%d", &pAtkType->nDamage); }		// �_���[�W
				else if (!strcmp(aDataSearch, "KNOCK_BACK:"))	{ fscanf(pFile, "%f", &pAtkType->fKnockBack); }		// �m�b�N�o�b�N
				else if (!strcmp(aDataSearch, "ELEM:"))			{ fscanf(pFile, "%d", &pAtkType->elem); }			// ����
				else if (!strcmp(aDataSearch, "SET_SE:"))		{ fscanf(pFile, "%d", &pAtkType->setSE); }			// �ݒ�SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pAtkType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitAtk_impact_00�֐� - ATK:�Ռ�[00] �̏��������� -
//========================================
void InitAtk_impact_00(void)
{
	Atk_impact_00	*pAtk	// ATK:�e[00] �̏��̃|�C���^
					= g_aAtk_impact_00;

	for (int nCntAtk = 0; nCntAtk < ATK_IMPACT_00_MAX; nCntAtk++, pAtk++)
	{
		// �p�����[�^�[�̏���������
		InitParameterAtk_impact_00(&g_aAtk_impact_00[nCntAtk]);
	}
}

//========================================
// UninitAtk_impact_00�֐� - ATK:�Ռ�[00] �̏I������ -
//========================================
void UninitAtk_impact_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < ATK_IMPACT_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureAtk_impact_00[nCntType] != NULL)
		{
			g_aTextureAtk_impact_00[nCntType]->Release();
			g_aTextureAtk_impact_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAtk_impact_00 != NULL)
	{
		g_pVtxBuffAtk_impact_00->Release();
		g_pVtxBuffAtk_impact_00 = NULL;
	}
}

//========================================
// UpdateAtk_impact_00�֐� - ATK:�Ռ�[00] �̍X�V���� -
//========================================
void UpdateAtk_impact_00(void)
{
	Atk_impact_00	*pAtk	// ATK:�e[00] �̏��̃|�C���^
					= g_aAtk_impact_00;

	for (int nCntAtk = 0; nCntAtk < ATK_IMPACT_00_MAX; nCntAtk++, pAtk++)
	{
		if (!pAtk->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// ������܂�Ԃ�
			continue;
		}
		else if (--pAtk->nLife <= 0)
		{// ���������Z��������0�ȉ��ɂȂ������A
			pAtk->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���
			continue;			// ������܂�Ԃ�
		}

		// ���ʂ̏Փˏ���������
		pAtk->cmnCollision = {};

		// �I�u�W�F�N�g���̏Փˏ���������
		for (int nCntObj = 0; nCntObj < ATK_IMPACT_00_HITOBJ_MAX; nCntObj++)
		{
			pAtk->aObjCollision[nCntObj] = {};
		}

		pAtk->nAttackIndex = -1;	// �U������I�u�W�F�N�g�̔ԍ���������

		// ATK:�Ռ�[00] �̏Փ˃`�F�b�N
		CheckCollisionAtk_impact_00(pAtk, VECTOR_X);
		CheckCollisionAtk_impact_00(pAtk, VECTOR_Z);

		// ATK:�Ռ�[00] �̏Փˏ��ɉ���������
		CollisionProcessAtk_impact_00(pAtk);
	}
}

//========================================
// SetAtk_impact_00�֐� - ATK:�Ռ�[00] �̐ݒ菈�� -
//========================================
void SetAtk_impact_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_IMPACT_00_PARENTTYPE parentType)
{
	if (GetMd_game_00()->state != MD_GAME_00_STATE_NORMAL)
	{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�łȂ����A
		return;	// �������I������
	}

	Atk_impact_00	*pAtk	// ATK:�e[00] �̏��̃|�C���^
					= g_aAtk_impact_00;

	for (int nCntAtk = 0; nCntAtk < ATK_IMPACT_00_MAX; nCntAtk++, pAtk++)
	{
		if (pAtk->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Atk_impact_00Type	*pType	// ATK:�e[00] �̎�ޖ��̏��
							= &g_aAtk_impact_00Type[nType];

		pAtk->pos = pos;				// �ʒu����
		pAtk->rot = rot;				// ��������
		pAtk->nType = nType;			// ��ނ���
		pAtk->parentType = parentType;	// �e�̎�ނ���
		pAtk->nLife = pType->nLife;		// ����
		pAtk->bUse = true;				// �g�p����Ă����Ԃɂ���
		pAtk->nDamage = pType->nDamage;	// �_���[�W��ݒ�

		if (parentType == ATK_IMPACT_00_PARENTTYPE_ENEMY)
		{// �e�̎�ނ��G�̎��A�_���[�W�ɃE�F�[�u�J�E���g��K�p
			pAtk->nDamage *= (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));
		}

		// �ݒ�SE���Đ�
		PlaySound(pType->setSE);

		// �J��Ԃ������𔲂���
		break;
	}
}