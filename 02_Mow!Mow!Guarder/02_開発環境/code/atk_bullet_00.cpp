//========================================
// 
// ATK:�e[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** atk_bullet_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "atk_bullet_00.h"		// ATK:�e			[00]
#include "atk_sword_00.h"		// ATK:��			[00]
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "eff_explosion_00.h"	// EFF:����			[00]
#include "eff_light_00.h"		// EFF:��			[00]
#include "eff_particle_00.h"	// EFF:�p�[�e�B�N��	[00]
#include "eff_shadow_00.h"		// EFF:�e			[00]
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
// ATK:�e[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define ATK_BULLET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATTACK\\ATK_BULLET_00_TYPE_DATA.txt"

// ATK:�e[00] �̍ő吔
// ATK:�e[00] �̎�ނ̍ő吔
#define ATK_BULLET_00_MAX		(128)
#define ATK_BULLET_00_TYPE_MAX	(8)

// ATK:�e[00] �̖@���x�N�g��
// ATK:�e[00] �̑��Έʒu
#define ATK_BULLET_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define ATK_BULLET_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 4.0f, 0.0f))

// ATK:�e[00] �̔���SE
#define ATK_BULLET_00_REFLECTION_SE	(SOUND_LABEL_SE_METAL_001)

//****************************************
// �񋓌^�̒�`
//****************************************
// ATK:�e[00] �̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum 
{
	ATK_BULLET_00_HITOBJ_ATK_SWORD_00,		// ATK:��			[00]
	ATK_BULLET_00_HITOBJ_CHR_ENEMY_00,		// CHR:�G			[00]
	ATK_BULLET_00_HITOBJ_CHR_PLAYER_00,		// CHR:�v���C���[	[00]
	ATK_BULLET_00_HITOBJ_OBJ_CORE_00,		// OBJ:�R�A			[00]
	ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:�~���[		[00]
	ATK_BULLET_00_HITOBJ_OBJ_STAGE_00,		// OBJ:�X�e�[�W		[00]
	ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:�u���b�N		[00]
	ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:���d���u		[00]
	ATK_BULLET_00_HITOBJ_OBJ_TURRET_00,		// OBJ:�^���b�g		[00]
	ATK_BULLET_00_HITOBJ_MAX,
}ATK_BULLET_00_HITOBJ;

// ATK:�e[00] �̑����ԍ�
typedef enum
{
	ATK_BULLET_00_ELEM_NONE,		// ����
	ATK_BULLET_00_ELEM_PARALYSIS,	// ���
	ATK_BULLET_00_ELEM_MAX,
}ATK_BULLET_00_ELEM;

//****************************************
// �\���̂̒�`
//****************************************
// ATK:�e[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// ���݂̈ʒu
	D3DXVECTOR3 posOld;	// �ߋ��̈ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// ����
	
	// �^���֘A
	HitTestInfo	hitTestInfo;	// �����蔻��̊Ǘ����
	Collision	cmnCollision; 	// ���ʂ̏Փˏ��
	Collision	aObjCollision	// �I�u�W�F�N�g���̏Փˏ��
				[ATK_BULLET_00_HITOBJ_MAX];

	// ���ފ֘A
	int							nType;		// ���
	ATK_BULLET_00_PARENTTYPE	parentType;	// �e�̎��

	// �|���S���֘A
	int nPtn;			// �p�^�[��No.
	int nCounterAnim;	// �A�j���[�V�����J�E���^�[

	// ��Ԋ֘A
	int						nLife;			// ����
	bool					bUse;			// �g�p����Ă��邩�t���O
	int						nDamage;		// �_���[�W
	int						nAttackIndex;	// �U������I�u�W�F�N�g�̔ԍ�
	ATK_BULLET_00_HITOBJ	attackObj;		// �U������I�u�W�F�N�g�̕���
}Atk_bullet_00;

// ATK:�e[00] �̎�ޖ��̏��\����
typedef struct
{
	int					nLife;			// ����
	float				fMoveForce;		// �ړ���
	int					nDamage;		// �_���[�W
	ATK_BULLET_00_ELEM	elem;			// ����
	float				fWidth;			// ��
	float				fHeight;		// ����
	char				aTexturePath	// �e�N�X�`���̑��΃p�X
						[TXT_MAX];
	int					nPtnWidth;		// �p�^�[����
	int					nPtnHeight;		// �p�^�[������
	int					nPtnMax;		// �p�^�[�����
	int					nAnimTime;		// �A�j���[�V�����ɂ����鎞��
	float				fShadowRadius;	// �e�̔��a
	int					nLightType;		// ���̎��
	int					nExplosionType;	// �����̎��
	int					nParticleType;	// �p�[�e�B�N���̎��
	SOUND_LABEL			setSE;			// �ݒ�SE
	HitTestSet			hitTestSet;		// �����蔻��ݒ���
}Atk_bullet_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ATK:�e[00] �̃p�����[�^�[�̏���������
void InitParameterAtk_bullet_00(Atk_bullet_00 *pAtk);

// ATK:�e[00] �̔j�󏈗�
void DestroyAtk_bullet_00(Atk_bullet_00 *pAtk);

// ATK:�e[00] �̈ʒu�X�V����
// ATK:�e[00] �̏Փ˃`�F�b�N����
// ATK:�e[00] �̏Փˏ��ɉ���������
void UpdatePosAtk_bullet_00(Atk_bullet_00 *pAtk);
void CheckCollisionAtk_bullet_00(Atk_bullet_00 *pAtk, VECTOR vector);
void CollisionProcessAtk_bullet_00(Atk_bullet_00 *pAtk);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureAtk_bullet_00		// ATK:�e[00] �̃e�N�X�`���ւ̃|�C���^
						[ATK_BULLET_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_bullet_00		// ATK:�e[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldAtk_bullet_00;	// ATK:�e[00] �̃��[���h�}�g���b�N�X
Atk_bullet_00			g_aAtk_bullet_00			// ATK:�e[00] �̏��
						[ATK_BULLET_00_MAX];
Atk_bullet_00Type		g_aAtk_bullet_00Type		// ATK:�e[00] �̎�ޖ��̏��
						[ATK_BULLET_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterAtk_bullet_00�֐� - ATK:�e[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	pAtk->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pAtk->posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ߋ��̈ʒu
	pAtk->move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	pAtk->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pAtk->hitTestInfo	= {};								// �����蔻��̊Ǘ����
	pAtk->cmnCollision	= {};								// ���ʂ̏Փˏ��
	for (int nCntHitObj = 0; nCntHitObj < ATK_BULLET_00_HITOBJ_MAX; nCntHitObj++) 
	{
		pAtk->aObjCollision[nCntHitObj] = {};				// �I�u�W�F�N�g���̏Փˏ��
	}
	pAtk->nType			= 0;								// ���
	pAtk->parentType	= (ATK_BULLET_00_PARENTTYPE)0;		// �e�̎��
	pAtk->nPtn			= 0;								// �p�^�[��No.
	pAtk->nCounterAnim	= 0;								// �A�j���[�V�����J�E���^�[
	pAtk->bUse			= false;							// �g�p����Ă��邩�t���O
	pAtk->nDamage		= 0;								// �_���[�W
	pAtk->nAttackIndex	= 0;								// �U������I�u�W�F�N�g�̔ԍ�
	pAtk->attackObj		= (ATK_BULLET_00_HITOBJ)0;			// �U������I�u�W�F�N�g�̕���
}

//========================================
// DestroyAtk_bullet_00�֐� - ATK:�e[00] �̔j�󏈗� -
//========================================
void DestroyAtk_bullet_00(Atk_bullet_00 *pAtk) 
{
	Atk_bullet_00Type	*pType	// ATK:�e[00] �̎�ޖ��̏��
						= &g_aAtk_bullet_00Type[pAtk->nType];

	pAtk->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���

	// EFF:����[00] �̐ݒ菈��
	SetEff_explosion_00(pAtk->pos, g_aAtk_bullet_00Type[pAtk->nType].nExplosionType);
}

//========================================
// UpdatePosAtk_bullet_00�֐� - ATK:�e[00] �̈ʒu�X�V���� -
//========================================
void UpdatePosAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	Atk_bullet_00Type	*pType	// ATK:�e[00] �̎�ޖ��̏��
						= &g_aAtk_bullet_00Type[pAtk->nType];

	// ���ʂ̏Փˏ���������
	pAtk->cmnCollision = {};

	// �I�u�W�F�N�g���̏Փˏ���������
	for (int nCntObj = 0; nCntObj < ATK_BULLET_00_HITOBJ_MAX; nCntObj++)
	{
		pAtk->aObjCollision[nCntObj] = {};
	}

	pAtk->nAttackIndex = -1;	// �U������I�u�W�F�N�g�̔ԍ���������

	pAtk->pos.x += pAtk->move.x;					// X���W�Ɉړ��ʂ�K�p
	pAtk->move.x = 0;								// X�����̈ړ��ʂ�������
	CheckCollisionAtk_bullet_00(pAtk, VECTOR_X);	// X�����̏Փ˃`�F�b�N����
	pAtk->pos.z += pAtk->move.z;					// Z���W�Ɉړ��ʂ�K�p
	pAtk->move.z = 0;								// Z�����̈ړ��ʂ�������
	CheckCollisionAtk_bullet_00(pAtk, VECTOR_Z);	// Z�����̏Փ˃`�F�b�N����

	// ATK:�e[00] �̏Փˏ��ɉ���������
	CollisionProcessAtk_bullet_00(pAtk);

	SetEff_shadow_00(pAtk->pos, pType->fShadowRadius);	// EFF:�e[00] �̐ݒ菈��
	SetEff_light_00(pAtk->pos, pType->nLightType);		// EFF:��[00] �̐ݒ菈��
}

//========================================
// UpdatePosAtk_bullet_00�֐� - ATK:�e[00] �̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionAtk_bullet_00(Atk_bullet_00 *pAtk, VECTOR vector)
{
	Atk_bullet_00Type	*pType	// ATK:�e[00] �̎�ޖ��̏��
						= &g_aAtk_bullet_00Type[pAtk->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo	myCollInfo =
	{
		&pAtk->pos,
		pAtk->posOld,
		NULL,
		&pAtk->rot,
		pAtk->rot,
		g_aAtk_bullet_00Type[pAtk->nType].hitTestSet.aHitTest[pAtk->hitTestInfo.nHitTest]
	};

	// �Փ˔���(����)
	/*/ OBJ:��			[00] /*/CollisionAtk_sword_00	(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_ATK_SWORD_00]	, &pAtk->cmnCollision, myCollInfo);
	/*/ OBJ:�X�e�[�W	[00] /*/CollisionObj_stage_00	(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_STAGE_00]	, &pAtk->cmnCollision, myCollInfo);

	// �Փ˔���(�񋤒�)
	switch (pAtk->parentType)
	{
		//========== *** �v���C���[ ***
	case ATK_BULLET_00_PARENTTYPE_PLAYER:
	{
		int nIndex;	// �ԍ�

		nIndex = CollisionChr_enemy_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_CHR_ENEMY_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// CHR:�G	[00] �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_BULLET_00_HITOBJ_CHR_ENEMY_00;
			break;
		}
	}
		break;
		//========== *** �G ***
	case ATK_BULLET_00_PARENTTYPE_ENEMY:
	{
		/*/ OBJ:�~���[	[00] /*/CollisionObj_mirror_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00], &pAtk->cmnCollision, myCollInfo);

		int nIndex;	// �ԍ�

		nIndex = /*/ CHR:�v���C���[	[00] /*/CollisionChr_player_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_CHR_PLAYER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_BULLET_00_HITOBJ_CHR_PLAYER_00;
			break;
		}
		nIndex = /*/ OBJ:�u���b�N	[00] /*/CollisionObj_block_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00;
			break;
		}
		nIndex = /*/ OBJ:�^���b�g	[00] /*/CollisionObj_turret_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_TURRET_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_BULLET_00_HITOBJ_OBJ_TURRET_00;
		}
		nIndex = /*/ OBJ:���d���u	[00] /*/CollisionObj_discharger_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00;
		}
		nIndex = /*/  OBJ:�R�A		[00] /*/CollisionObj_core_00(vector, &pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_CORE_00], &pAtk->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// �ƐڐG�������A
			pAtk->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
			pAtk->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
				= ATK_BULLET_00_HITOBJ_OBJ_CORE_00;
			break;
		}
	}
		break;
	}
}

//========================================
// CollisionProcessAtk_bullet_00�֐� - ATK:�e[00] �̏Փˏ��ɉ��������� -
//========================================
void CollisionProcessAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	Atk_bullet_00Type	*pType		// ATK:�e[00] �̎�ޖ��̏��
						= &g_aAtk_bullet_00Type[pAtk->nType];
	bool				bDestroy	// �j��t���O
						= true;

	if (pAtk->nAttackIndex != -1) 
	{// �U������I�u�W�F�N�g�̔ԍ���-1�Ŗ������A
		// �U������I�u�W�F�N�g�̕��ނɉ������_���[�W����
		switch (pAtk->attackObj)
		{
			case /*/ CHR:�G			[00] /*/ATK_BULLET_00_HITOBJ_CHR_ENEMY_00:
				DamageChr_enemy_00(pAtk->nAttackIndex, pAtk->nDamage);
				if (pType->elem == ATK_BULLET_00_ELEM_PARALYSIS) 
				{// ��������Ⴢ̎��A��Ⴢ�t�^
					GiveStateChr_enemy_00(pAtk->nAttackIndex, CHR_ENEMY_00_STATE_PARALYSIS);
				}
				
				break;
			case /*/ CHR:�v���C���[	[00] /*/ATK_BULLET_00_HITOBJ_CHR_PLAYER_00:
				DamageChr_player_00(pAtk->nDamage);
				break;
			case /*/ OBJ:�u���b�N	[00] /*/ATK_BULLET_00_HITOBJ_OBJ_BLOCK_00:
				DamageObj_block_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:�^���b�g	[00] /*/ATK_BULLET_00_HITOBJ_OBJ_TURRET_00:
				DamageObj_turret_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:���d���u	[00] /*/ATK_BULLET_00_HITOBJ_OBJ_DISCHARGER_00:
				DamageObj_discharger_00(pAtk->nAttackIndex, pAtk->nDamage);
				break;
			case /*/ OBJ:�R�A		[00] /*/ATK_BULLET_00_HITOBJ_OBJ_CORE_00:
				DamageObj_core_00(pAtk->nDamage);
				break;
			default /*/ �Y���Ȃ� /*/:
				bDestroy = false;	// �j��t���O���U�ɂ���
				break;
		}
	}
	else
	{// �U������I�u�W�F�N�g�̔ԍ���-1�̎��A
		bDestroy = (pAtk->cmnCollision.bHit | pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_ATK_SWORD_00].bInside) * (!pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00].bHit);
	}

	if (pAtk->aObjCollision[ATK_BULLET_00_HITOBJ_OBJ_MIRROR_00].bHit)
	{// OBJ:�~���[[00] �Ƃ̓�����t���O���^�̎��A
		PlaySound(ATK_BULLET_00_REFLECTION_SE);	// ����SE���Đ�
		pAtk->parentType = ATK_BULLET_00_PARENTTYPE_PLAYER;	// �e�̎�ނ��v���C���[�ɐݒ�
	}

	if (bDestroy)
	{// �j��t���O���^�̎��A
		SetEff_particle_00(pAtk->pos, pType->nParticleType);	// EFF:�p�[�e�B�N��[00] �̐ݒ菈��
		DestroyAtk_bullet_00(pAtk);								// �j�󏈗�
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadAtk_bullet_00�֐� - ATK:�e[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadAtk_bullet_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Atk_bullet_00Type	*pAtkType	// ATK:�e[00] �̎�ޖ��̏��
						= g_aAtk_bullet_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(ATK_BULLET_00_TYPE_DATA_FILE_PATH, "r");

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
			// ATK:�e[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))			{ pAtkType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "LIFE:"))				{ fscanf(pFile, "%d", &pAtkType->nLife); }			// ����
				else if (!strcmp(aDataSearch, "MOVE_FORCE:"))		{ fscanf(pFile, "%f", &pAtkType->fMoveForce); }		// �ړ���
				else if (!strcmp(aDataSearch, "DAMAGE:"))			{ fscanf(pFile, "%d", &pAtkType->nDamage); }		// �_���[�W
				else if (!strcmp(aDataSearch, "ELEM:"))				{ fscanf(pFile, "%d", &pAtkType->elem); }			// ����
				else if (!strcmp(aDataSearch, "WIDTH:"))			{ fscanf(pFile, "%f", &pAtkType->fWidth); }			// ��
				else if (!strcmp(aDataSearch, "HEIGHT:"))			{ fscanf(pFile, "%f", &pAtkType->fHeight); }		// ����
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))		{ fscanf(pFile, "%s", &pAtkType->aTexturePath); }	// �e�N�X�`���̑��΃p�X
				else if (!strcmp(aDataSearch, "PTN_WIDTH:"))		{ fscanf(pFile, "%d", &pAtkType->nPtnWidth); }		// �p�^�[����
				else if (!strcmp(aDataSearch, "PTN_HEIGHT:"))		{ fscanf(pFile, "%d", &pAtkType->nPtnHeight); }		// �p�^�[������
				else if (!strcmp(aDataSearch, "PTN_MAX:"))			{ fscanf(pFile, "%d", &pAtkType->nPtnMax); }		// �p�^�[�����
				else if (!strcmp(aDataSearch, "ANIM_TIME:"))		{ fscanf(pFile, "%d", &pAtkType->nAnimTime); }		// �A�j���[�V�����ɂ����鎞��
				else if (!strcmp(aDataSearch, "SHADOW_RADIUS:"))	{ fscanf(pFile, "%f", &pAtkType->fShadowRadius); }	// �e�̔��a
				else if (!strcmp(aDataSearch, "EXPLOSION_TYPE:"))	{ fscanf(pFile, "%d", &pAtkType->nExplosionType); }	// �����̎��
				else if (!strcmp(aDataSearch, "LIGHT_TYPE:"))		{ fscanf(pFile, "%d", &pAtkType->nLightType); }		// ���̎��
				else if (!strcmp(aDataSearch, "PARTICLE_TYPE:"))	{ fscanf(pFile, "%d", &pAtkType->nParticleType); }	// �p�[�e�B�N���̎��
				else if (!strcmp(aDataSearch, "SET_SE:"))			{ fscanf(pFile, "%d", &pAtkType->setSE); }			// �ݒ�SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pAtkType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitAtk_bullet_00�֐� - ATK:�e[00] �̏��������� -
//========================================
void InitAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntType = 0; nCntType < ATK_BULLET_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aAtk_bullet_00Type[nCntType].aTexturePath, &g_aTextureAtk_bullet_00[nCntType]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * ATK_BULLET_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_bullet_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// ATK:�e[00] �̏��̃|�C���^
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++, pVtx += 4)
	{
		// �p�����[�^�[�̏���������
		InitParameterAtk_bullet_00(&g_aAtk_bullet_00[nCntAtk]);

		// �@���x�N�g���̐ݒ�
		SetNormalLine3D(pVtx, ATK_BULLET_00_NOR);

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx, 0, g_aAtk_bullet_00Type[pAtk->nType].nPtnWidth, g_aAtk_bullet_00Type[pAtk->nType].nPtnHeight, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// UninitAtk_bullet_00�֐� - ATK:�e[00] �̏I������ -
//========================================
void UninitAtk_bullet_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < ATK_BULLET_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureAtk_bullet_00[nCntType] != NULL)
		{
			g_aTextureAtk_bullet_00[nCntType]->Release();
			g_aTextureAtk_bullet_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAtk_bullet_00 != NULL)
	{
		g_pVtxBuffAtk_bullet_00->Release();
		g_pVtxBuffAtk_bullet_00 = NULL;
	}
}

//========================================
// UpdateAtk_bullet_00�֐� - ATK:�e[00] �̍X�V���� -
//========================================
void UpdateAtk_bullet_00(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// ATK:�e[00] �̏��̃|�C���^
					= g_aAtk_bullet_00;
	
	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++, pVtx += 4)
	{
		if (!pAtk->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// ������܂�Ԃ�
			continue;
		}
		else if (--pAtk->nLife <= 0)
		{// ���������Z��������0�ȉ��ɂȂ������A
			DestroyAtk_bullet_00(pAtk);	// �j�󏈗�
			continue;					// ������܂�Ԃ�
		}

		// ���݂̈ʒu���ߋ��̈ʒu�Ƃ��ĕۑ�
		pAtk->posOld = pAtk->pos;

		// �ړ��ʂ������ƈړ��͂ɉ����Đݒ�
		pAtk->move.x = sinf(pAtk->rot.y) * g_aAtk_bullet_00Type[pAtk->nType].fMoveForce;
		pAtk->move.z = cosf(pAtk->rot.y) * g_aAtk_bullet_00Type[pAtk->nType].fMoveForce;

		// �ʒu�X�V����
		UpdatePosAtk_bullet_00(pAtk);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx,
			Count(
				&pAtk->nPtn,
				0,
				g_aAtk_bullet_00Type[pAtk->nType].nPtnMax,
				&pAtk->nCounterAnim,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimTime,
				COUNT_TYPE_NORMAL),
			g_aAtk_bullet_00Type[pAtk->nType].nPtnWidth,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnHeight, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// DrawAtk_bullet_00�֐� - ATK:�e[00] �̕`�揈�� -
//========================================
void DrawAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX			mtxView;	// �r���[�}�g���b�N�X�擾�p
	Atk_bullet_00		*pAtk		// ATK:�e[00] �̏��̃|�C���^
						= g_aAtk_bullet_00;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_bullet_00, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++,pAtk++)
	{
		if (!pAtk->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// ������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldAtk_bullet_00);

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_mtxWorldAtk_bullet_00, NULL, &mtxView);
		g_mtxWorldAtk_bullet_00._41 = 0.0f;
		g_mtxWorldAtk_bullet_00._42 = 0.0f;
		g_mtxWorldAtk_bullet_00._43 = 0.0f;
		{
			D3DXVECTOR3 setPos = pAtk->pos + ATK_BULLET_00_RELATIVE_POS;	// �ݒ�ʒu��ݒ�

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldAtk_bullet_00, &g_mtxWorldAtk_bullet_00, &mtxTrans);
		}
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAtk_bullet_00);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureAtk_bullet_00[pAtk->nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntAtk, 2);
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//========================================
// SetAtk_bullet_00�֐� - ATK:�e[00] �̐ݒ菈�� -
//========================================
void SetAtk_bullet_00(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nType, ATK_BULLET_00_PARENTTYPE parentType)
{
	if (GetMd_game_00()->state != MD_GAME_00_STATE_NORMAL) 
	{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�łȂ����A
		return;	// �������I������
	}

	Atk_bullet_00	*pAtk	// ATK:�e[00] �̏��̃|�C���^
					= g_aAtk_bullet_00;
	VERTEX_3D		*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++, pVtx += 4)
	{
		if (pAtk->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Atk_bullet_00Type	*pType	// ATK:�e[00] �̎�ޖ��̏��
							= &g_aAtk_bullet_00Type[nType];

		pAtk->pos = pos;				// �ʒu����
		pAtk->rot = rot;				// ��������
		pAtk->nType = nType;			// ��ނ���
		pAtk->parentType = parentType;	// �e�̎�ނ���
		pAtk->nPtn = 0;					// �p�^�[��No.��������
		pAtk->nCounterAnim = 0;			// �A�j���[�V�����J�E���^�[��������
		pAtk->nLife = pType->nLife;		// ����
		pAtk->bUse = true;				// �g�p����Ă����Ԃɂ���
		pAtk->nDamage = pType->nDamage;	// �_���[�W��ݒ�

		if (parentType == ATK_BULLET_00_PARENTTYPE_PLAYER) 
		{// �e�̎�ނ��v���C���[�̎��A�_���[�W�ɒe�̍U���̓A�b�v�J�E���g��K�p
			pAtk->nDamage *= (1.0f + (GetChr_player_00()->nBulletAtkUpCount * CHR_PLAYER_00_BULLET_ATK_ADDRATE));
		}
		else if (parentType == ATK_BULLET_00_PARENTTYPE_ENEMY) 
		{// �e�̎�ނ��G�̎��A�_���[�W�ɃE�F�[�u�J�E���g��K�p
			pAtk->nDamage *= (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));
		}

		// ��ނɉ��������_���W�̐ݒ�
		{
			float fWidth	= g_aAtk_bullet_00Type[nType].fWidth;	// ��
			float fHeight	= g_aAtk_bullet_00Type[nType].fHeight;	// ����

			pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
		}

		PlaySound(pType->setSE);	// �ݒ�SE���Đ�

		// �J��Ԃ������𔲂���
		break;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAtk_bullet_00->Unlock();
}