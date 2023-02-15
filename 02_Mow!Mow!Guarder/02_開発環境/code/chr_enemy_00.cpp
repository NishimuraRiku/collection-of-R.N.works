//========================================
// 
// CHR:�G[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_enemy_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "md_game_00.h"			// MD :�Q�[�����	[00]
#include "atk_bullet_00.h"		// ATK:�e			[00]
#include "atk_impact_00.h"		// ATK:�Ռ�			[00]
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "itm_coin_00.h"		// ITM:�R�C��		[00]
#include "eff_explosion_00.h"	// EFF:����			[00]
#include "eff_shadow_00.h"		// EFF:�e			[00]
#include "eff_shock-wave_00.h"	// EFF:�Ռ��g		[00]
#include "obj_block_00.h"		// OBJ:�u���b�N		[00]
#include "obj_discharger_00.h"	// OBJ:���d���u		[00]
#include "obj_core_00.h"		// OBJ:�R�A			[00]
#include "obj_mirror_00.h"		// OBJ:�~���[		[00]
#include "obj_turret_00.h"		// OBJ:�^���b�g		[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// CHR:�G[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define CHR_ENEMY_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\CHR_ENEMY_00_TYPE_DATA.txt"

// CHR:�G[00] �̎�ނ̍ő吔
#define CHR_ENEMY_00_TYPE_MAX	(32)

// CHR:�G[00] �̉�]�{��
// CHR:�G[00] �̗͂̌����{��
// CHR:�G[00] �̈ړ��ʂ̌����{��
#define CHR_ENEMY_00_ROT_DIAMETER	(0.25f)
#define CHR_ENEMY_00_FORCE_DAMP		(0.9f)
#define CHR_ENEMY_00_MOVE_DAMP		(0.8f)

// CHR:�G[00] �̏Փˎ��̃m�b�N�o�b�N
#define CHR_ENEMY_00_COLLISION_KNOCK_BACK	(3.0f)

// CHR:�G[00] �̈ړ��J�E���^�[�̉���
// CHR:�G[00] �̈ړ��J�E���^�[�̏��
#define CHR_ENEMY_00_MOVE_COUNTER_MIN	(30)
#define CHR_ENEMY_00_MOVE_COUNTER_MAX	(120)

// CHR:�G[00] �̖��G����
#define CHR_ENEMY_00_INVINCIBLE_TIME	(5)

// CHR:�G[00] �̃_���[�W����
#define CHR_ENEMY_00_DAMAGE_TIME	(20)

// CHR:�G[00] �̖�Ⴢ̎���
// CHR:�G[00] �̖�Ⴢ̌����{��
// CHR:�G[00] �̖��SE
#define CHR_ENEMY_00_PARALYSIS_TIME				(180)
#define CHR_ENEMY_00_PARALYSIS_DECREMENT_RATE	(0.5f)
#define CHR_ENEMY_00_PARALYSIS_SE				(SOUND_LABEL_SE_ELEC_001)

// CHR:�G[00] �̃E�F�[�u�J�E���g����HP�̏㏸�{��
#define CHR_ENEMY_00_WAVE_HP_ADDRATE	(0.1f)

// CHR:�G[00] ��1�x�̐U���ɂ����鎞��
// CHR:�G[00] �̐U���̌����{��
// CHR:�G[00] �̐U���̋����̉���
// CHR:�G[00] �̃_���[�W�ɂ��U���̔{��
#define CHR_ENEMY_00_ONCE_VIBRATION_TIME	(1)
#define CHR_ENEMY_00_VIBRATION_DAMPING		(0.5f)
#define CHR_ENEMY_00_VIBRATION_MIN			(0.01f)
#define CHR_ENEMY_00_DAMAGE_VIBRATION_RATE	(5.0f)

// CHR:�G[00] �̃{�X�ݒ莞�̃J����(3D)�ɂ�����U��
// CHR:�G[00] �̃{�X�ݒ莞�̌��ʉ�
#define CHR_ENEMY_00_BOSS_SET_CAMERA3D_VIBRATION	(10.0f)
#define CHR_ENEMY_00_BOSS_SET_SE					(SOUND_LABEL_SE_FOOTSTEP_002)

// CHR:�G[00] �̃A�s�[�����[�V�����ԍ�
#define CHR_ENEMY_00_APPEAL_MOTION	(0)

//****************************************
// �\���̂̒�`
//****************************************
// CHR:�G[00] �̊Ǘ����\����
typedef struct 
{
	CHR_ENEMY_00_CONTROL_STATE	state;			// ���
	int							nSetBossIdx;	// �ݒ肵���{�X�̔ԍ�
}Chr_enemy_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// CHR:�G[00] �̍s���p�^�[���ݒ���̓ǂݍ��ݏ���
void LoadActPtnSetChr_enemy_00(FILE *pFile, Chr_enemy_00ActPtnSet *pActPtnSet);

// CHR:�G[00] �̃p�����[�^�[�̏���������
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̐ݒ莞�̃p�����[�^�[�̏���������
void InitParameterSetChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̊Ǘ����̃p�����[�^�[�̏���������
void InitParameterChr_enemy_00Control(void);

// CHR:�G[00] �̔ԍ��̌�������
int SearchIndexChr_enemy_00(Chr_enemy_00 *pChr);

// CHR:�G[00] �̍s���ǂݍ��ݏ���
void ActLoadChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̍s������
void ActChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̈ړ�����
void MoveChr_enemy_00(Chr_enemy_00 *pChr, float fMove);
// CHR:�G[00] �̈ʒu�X�V����
void UpdatePosChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̏Փ˃`�F�b�N����
void CheckCollisionChr_enemy_00(Chr_enemy_00 *pChr, VECTOR vector);
// CHR:�G[00] �̏Փˏ��ɉ���������
void CollisionProcessChr_enemy_00(Chr_enemy_00 *pChr);

// CHR:�G[00] �̔j�󏈗�
void DestroyChr_enemy_00(Chr_enemy_00 *pChr);

// CHR:�G[00] �̐U����ݒ�
void SetChr_enemy_00Vibration(Chr_enemy_00 *pChr, float fVibration);

// CHR:�G[00] �̎q�ݒ菈��
void SetChildChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nParentIdx);
// CHR:�G[00] �̎q�̈ړ�����
void ChildMoveChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̎q�̃_���[�W�J�E���^�[�ݒ菈��
void ChildSetDamageCounterChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̎q�̃A�s�[�����[�V�����ݒ菈��
void ShildAppealMotionSetChr_enemy_00(Chr_enemy_00 *pChr);
// CHR:�G[00] �̎q�̔j�󏈗�
void ChildDestoryChr_enemy_00(Chr_enemy_00 *pChr);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureChr_enemy_00	// CHR:�G[00] �̃e�N�X�`���ւ̃|�C���^
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshChr_enemy_00		// CHR:�G[00] �̃��b�V��(���_���)�ւ̃|�C���^
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					= {};
LPD3DXBUFFER		g_aBuffMatChr_enemy_00	// CHR:�G[00] �̃}�e���A���ւ̃|�C���^
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					= {};
DWORD				g_aNumMatChr_enemy_00	// CHR:�G[00] �̃}�e���A���̐�
					[CHR_ENEMY_00_TYPE_MAX]
					[PARTS_3D_MAX]
					= {};
D3DXMATRIX			g_aMtxWorldChr_enemy_00	// CHR:�G[00] �̃��[���h�}�g���b�N�X
					[PARTS_3D_MAX];
Chr_enemy_00		g_aChr_enemy_00			// CHR:�G[00] �̏��
					[CHR_ENEMY_00_MAX];
Chr_enemy_00Control	g_chr_enemy_00Control;	// CHR:�G[00] �̊Ǘ����
Chr_enemy_00Type	g_aChr_enemy_00Type		// CHR:�G[00] �̎�ޖ��̏��
					[CHR_ENEMY_00_TYPE_MAX];
int					g_nChr_enemy_00TypeNum;	// CHR:�G[00] �̎�ސ�

// CHR:�G[00] �̍s���̎�ޖ��̏��\����
const Chr_enemy_00ActType g_aChr_enemy_00ActType[CHR_ENEMY_00_COMMAND_MAX] =
{
	{ "TORTUOUS_MOVE_START"    ,0  },	// �Ȑ܎��ړ��J�n
	{ "TORTUOUS_MOVE_END"      ,0  },	// �Ȑ܎��ړ��I��
	{ "TURN_MOVE_START"        ,2  },	// �����]�����ړ��J�n
	{ "TURN_MOVE_END"          ,0  },	// �����]�����ړ��I��
	{ "FORCE_MOVE_START"       ,1  },	// ���i���ړ��J�n
	{ "FORCE_MOVE_END"         ,0  },	// ���i���ړ��I��
	{ "TURN_TO_TURN_POS_START" ,0  },	// �U������ʒu�ɐU������J�n
	{ "TURN_TO_TURN_POS_END"   ,0  },	// �U������ʒu�ɐU������I��
	{ "TURN_TO_MOVE_ROT_START" ,0  },	// �ړ������ɐU������J�n
	{ "TURN_TO_MOVE_ROT_END"   ,0  },	// �ړ������ɐU������I��
	{ "Z_LOCK"                 ,0  },	// Z���W�����b�N
	{ "Z_UNLOCK"               ,0  },	// Z���W���A�����b�N
	{ "CHILD_SET"              ,7  },	// �q�ݒ�
	{ "CHILD_DEPENDENCY"       ,0  },	// �q�ˑ�
	{ "SPIN_START"             ,3  },	// ��]�J�n
	{ "SPIN_END"               ,0  },	// ��]�I��
	{ "SHOT"                   ,7  },	// �e����
	{ "PLAY_SOUND"             ,1  },	// �T�E���h�Đ�
	{ "MOTION_SET"             ,1  },	// ���[�V�����ݒ�
	{ "AUTO_SHOT_START"        ,3  },	// �����V���b�g�J�n
	{ "AUTO_SHOT_END"          ,0  },	// �����V���b�g�I��
	{ "UNIT_IGNORED_START"     ,0  },	// ���j�b�g�����J�n
	{ "UNIT_IGNORED_END"       ,0  },	// ���j�b�g�����I��
	{ "MOVE_START"             ,3  },	// �ړ��J�n
	{ "MOVE_END"               ,0  },	// �ړ��I��
	{ "ALLY_SET"               ,7  },	// �����ݒ�
	{ "ATK_IMPACT"             ,7  },	// ATK:�Ռ�
	{ "EFF_SHOCKWAVE_00"       ,18 },	// EFF:�Ռ��g[00]
	{ "COLLISION_DAMAGE_CHANGE",1  },	// �Փ˃_���[�W��ύX
	{ "COLLISION_DAMAGE_UNDO"  ,0  },	// �Փ˃_���[�W�����ɖ߂�
	{ "BOSS_DEPENDENCY"        ,0  },	// �{�X�ˑ�
	{ "ALLY_SET_ABSOLUTE"      ,7  },	// �����ݒ�(��Έʒu)
	{ "RECOGNITION_OFF"        ,0  },	// �F��OFF
	{ "TELEPORTATION"          ,3  },	// �u�Ԉړ�
};

//========== *** CHR:�G[00] �̏����擾 ***
Chr_enemy_00 *GetChr_enemy_00(void) 
{
	return g_aChr_enemy_00;
}

//========== *** CHR:�G[00] �̎�ޖ��̏����擾 ***
Chr_enemy_00Type *GetChr_enemy_00Type(void) 
{
	return g_aChr_enemy_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadActPtnSetChr_enemy_00�֐� - CHR:�G[00] �̍s���p�^�[���ݒ���̓ǂݍ��ݏ��� -
//========================================
void LoadActPtnSetChr_enemy_00(FILE *pFile, Chr_enemy_00ActPtnSet *pActPtnSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];
	int		nCntActPtn	// �s���p�^�[���̃J�E���g
			= 0;

	// �s���p�^�[���ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "ACTPTNSET_END")) { break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "ACTPTN"))		{
			Chr_enemy_00ActPtn	*pActPtn	// �s���p�^�[�����̃|�C���^
								= &pActPtnSet->aActPtn[nCntActPtn];

			pActPtn->nActNum = 0;	// �s������������

			// �s���p�^�[�����̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "ACTPTN_END"))	{ nCntActPtn++; break; }						// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "LOOP_TIME:"))	{ fscanf(pFile, "%d", &pActPtn->nLoopTime); }	// ���[�v����
				else if (!strcmp(aDataSearch, "ACT"))			{
					Chr_enemy_00Act	*pAct	// �s�����̃|�C���^
									= pActPtn->aAct;

					// �s�����̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if		(!strcmp(aDataSearch, "ACT_END"))	{ break; }
						else if (!strcmp(aDataSearch, "TIME"))		{
							fscanf(pFile, "%d", &pAct->nTime);	// �s�����鎞��
							fscanf(pFile, "%s", aDataSearch);	// ����

							for (int nCntActType = 0; nCntActType < CHR_ENEMY_00_COMMAND_MAX; nCntActType++) 
							{// �s���̎�ނ�����
								if (strcmp(aDataSearch, g_aChr_enemy_00ActType[nCntActType].aLabelName)) 
								{// �J�E���g�̍s���̎�ނ̃��x�����ƌ�������v���Ȃ��������A
									continue;	// �J��Ԃ�������܂�Ԃ�
								}
								
								pAct->cmd = (CHR_ENEMY_00_COMMAND)nCntActType;	// �s���̎�ނ���

								for (int nCntData = 0; nCntData < g_aChr_enemy_00ActType[nCntActType].nDataNum; nCntData++) 
								{// �l��K�v�������ǂݍ���
									fscanf(pFile, "%f", &pAct->aData[nCntData]);
								}

								break;
							}

							pAct++;				// �s�����̃|�C���^��i�߂�
							pActPtn->nActNum++;	// �s���������Z
						}
					}
				}
			}
		}
	}
}

//========================================
// InitParameterChr_enemy_00�֐� - CHR:�G[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr)
{
	pChr->pos					= INITD3DXVECTOR3;				// ���݂̈ʒu
	pChr->posOld				= INITD3DXVECTOR3;				// �ߋ��̈ʒu
	pChr->force					= INITD3DXVECTOR3;				// ��
	pChr->move					= INITD3DXVECTOR3;				// �ړ���
	pChr->rot					= INITD3DXVECTOR3;				// ���݂̌���
	pChr->rotOld				= INITD3DXVECTOR3;				// �ߋ��̌���
	pChr->moveRot				= INITD3DXVECTOR3;				// �ړ�����
	pChr->pTurnPos				= NULL;							// �U�������̈ʒu�̃|�C���^
	pChr->nType					= 0;							// ���
	pChr->bUse					= false;						// �g�p����Ă���t���O
	pChr->nHP					=								// HP
	pChr->nHPMax				= 0;							// HP�̏��
	pChr->state					= CHR_ENEMY_00_STATE_NORMAL;	// ���
	pChr->nCounterState			= 0;							// ��ԃJ�E���^�[
	pChr->nCounterShot			= 0;							// �V���b�g�J�E���^�[
	pChr->nCounterCollAtk		= 0;							// �ՓˍU���J�E���^�[
	pChr->nCounterInvincible	= 0;							// ���G�J�E���^�[
	pChr->nCounterDamage		= 0;							// �_���[�W�J�E���^�[
	pChr->nAttackIndex			= -1;							// �U������I�u�W�F�N�g�̔ԍ�
	pChr->attackObj				= (CHR_ENEMY_00_HITOBJ)0;		// �U������I�u�W�F�N�g�̕���
	pChr->fVibration			= 0;							// �U���̋���
	pChr->nCounterVib			= 0;							// �U���J�E���^�[
	pChr->vibrationPos			= INITD3DXVECTOR3;				// �U���ʒu
	pChr->nParentIdx			= -1;							// �e�̔ԍ�
	pChr->bAutoShot				= false;						// �����V���b�g�t���O
	pChr->nBulletType			= 0;							// �e�̎��
	pChr->nShotTimeMin			= 0;							// ���ˊԊu�̉���
	pChr->nShotTimeMax			= 0;							// ���ˊԊu�̏��
	pChr->bUnitIgnored			= false;						// ���j�b�g�����t���O
	pChr->bCollDamageChange		= false;						// �Փ˃_���[�W�ύX�t���O
	pChr->nNewCollDamage		= 0;							// �V�K�Փ˃_���[�W
	pChr->bBossDependency		= false;						// �{�X�ˑ��t���O
	pChr->bRecognitionOff		= false;						// �F��OFF�t���O
	pChr->hitTestInfo			= {};							// �����蔻��̊Ǘ����
	pChr->cmnCollision			= {};							// ���ʂ̏Փˏ��
	for (int nCntHitObj			= 0; nCntHitObj < CHR_ENEMY_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};					// �I�u�W�F�N�g���̏Փˏ��
	}
	pChr->actInfo				= {};							// �s���Ǘ����
	pChr->partsInfo				= {};							// ���i�Ǘ�
	pChr->nMotion				= 0;							// ���[�V����
	pChr->nMotionTemp			= 0;							// ���[�V�����ꎞ�ۑ�
}

//========================================
// InitParameterSetChr_enemy_00�֐� - CHR:�G[00] �̐ݒ莞�̃p�����[�^�[�̏��������� -
//========================================
void InitParameterSetChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[pChr->nType];

	pChr->force					= INITD3DXVECTOR3;				// ��
	pChr->move					= INITD3DXVECTOR3;				// �ړ���
	pChr->pTurnPos				= NULL;							// �U�������̈ʒu�̃|�C���^
	pChr->bUse					= true;							// �g�p����Ă���t���O
	pChr->state					= CHR_ENEMY_00_STATE_NORMAL;	// ���
	pChr->nCounterState			= 0;							// ��ԃJ�E���^�[
	pChr->nCounterShot			= 0;							// �V���b�g�J�E���^�[
	pChr->nCounterCollAtk		= 0;							// �ՓˍU���J�E���^�[
	pChr->nCounterInvincible	= 0;							// ���G�J�E���^�[
	pChr->nCounterDamage		= 0;							// �_���[�W�J�E���^�[
	pChr->fVibration			= 0;							// �U���̋���
	pChr->nCounterVib			= 0;							// �U���J�E���^�[
	pChr->vibrationPos			= INITD3DXVECTOR3;				// �U���ʒu
	pChr->bAutoShot				= false;						// �����V���b�g�t���O
	pChr->bUnitIgnored			= false;						// ���j�b�g�����t���O
	pChr->bCollDamageChange		= false;						// �Փ˃_���[�W�ύX�t���O
	pChr->nNewCollDamage		= 0;							// �V�K�Փ˃_���[�W
	pChr->bBossDependency		= false;						// �{�X�ˑ��t���O
	pChr->bRecognitionOff		= false;						// �F��OFF�t���O
	pChr->actInfo				= {};							// �s���Ǘ����
	pChr->partsInfo				= {};							// ���i�Ǘ�
	pChr->nMotion				= 0;							// ���[�V����

	pChr->nHPMax =				// HP�̏����ݒ�
		pType->nHP * (1.0f + (CHR_ENEMY_00_WAVE_HP_ADDRATE * GetObj_stage_00()->nCntWaveMain));
	pChr->nHP = pChr->nHPMax;	// HP�ɍő�HP����
	if (pType->nCollAtkTime != -1) {
		pChr->nCounterCollAtk	// �ՓˍU���̊Ԋu��-1�łȂ����A�ՓˍU���J�E���^�[��ݒ�
			= pType->nCollAtkTime;
	}

	// ���i(3D)�̃g�����X�t�H�[�����擾
	GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);
}

//========================================
// InitParameterChr_enemy_00Control�֐� - CHR:�G[00] �Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterChr_enemy_00Control(void)
{
	Chr_enemy_00Control *pChrCtl	// CHR:�G[00] �̊Ǘ����̃|�C���^
						= &g_chr_enemy_00Control;

	pChrCtl->state			= CHR_ENEMY_00_CONTROL_STATE_DYNAMIC;	// ���
	pChrCtl->nSetBossIdx	= -1;									// �ݒ肵���{�X�̔ԍ�
}

//========================================
// SearchIndexChr_enemy_00�֐� - CHR:�G[00] �̔ԍ��̌������� -
//========================================
int SearchIndexChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00	*pSearchChr	// �����p��CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nIdx = 0; nIdx < CHR_ENEMY_00_MAX; nIdx++, pSearchChr++)
	{
		if (pChr == pSearchChr) 
		{// �����p�̃|�C���^�ƈ�v�������A
			return nIdx;
		}
	}

	return -1;
}

//========================================
// ActLoadChr_enemy_00�֐� - CHR:�G[00] �̍s���ǂݍ��ݏ��� -
//========================================
void ActLoadChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00Type	*pType		// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[pChr->nType];
	Chr_enemy_00ActInfo	*pActInfo	// CHR:�G[00] �̍s���Ǘ����̃|�C���^
						= &pChr->actInfo;
	Chr_enemy_00ActPtn	*pActPtn	// CHR:�G[00] �̍s���p�^�[�����̃|�C���^
						= &pType->actPtnSet.aActPtn[pActInfo->nActPtn];
	Chr_enemy_00Act		*pAct		// CHR:�G[00] �̍s�����̃|�C���^
						= pActPtn->aAct;

	for (int nCntAct = 0; nCntAct < pActPtn->nActNum; nCntAct++, pAct++) 
	{
		if (pActInfo->nCounterAct != pAct->nTime) 
		{// �s���J�E���^�[���J�E���g�̍s�����鎞�ԂɈ�v���Ă��Ȃ����A
			continue;	// ������܂�Ԃ�
		}

		// �s���̎�ނɉ���������
		switch (pAct->cmd)
		{
			case /*/ �Ȑ܎��ړ��J�n /*/CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TORTUOUS_MOVE] = true;	// �Ȑ܎��ړ��t���O��^�ɂ���
				break;
			case /*/ �Ȑ܎��ړ��I�� /*/CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TORTUOUS_MOVE] = false;	// �Ȑ܎��ړ��t���O���U�ɂ���
				break;
			case /*/ �����]�����ړ��J�n /*/CHR_ENEMY_00_COMMAND_TURN_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_MOVE] = true;	// �����]�����ړ��t���O��^�ɂ���
				pActInfo->fRotation			= pAct->aData[0];	// ��]�͂���
				pActInfo->fRandPosRadius	= pAct->aData[1];	// �����ʒu���a����
				break;
			case /*/ �����]�����ړ��I�� /*/CHR_ENEMY_00_COMMAND_TURN_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_MOVE] = false;	// �����]�����ړ��t���O���U�ɂ���
				break;
			case /*/ ���i���ړ��J�n /*/CHR_ENEMY_00_COMMAND_FORCE_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_FORCE_MOVE] = true;	// ���i���ړ��t���O��^�ɂ���
				break;
			case /*/ ���i���ړ��I�� /*/CHR_ENEMY_00_COMMAND_FORCE_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_FORCE_MOVE] = false;	// ���i���ړ��t���O���U�ɂ���
				break;
			case /*/ �U������ʒu�ɐU������J�n /*/CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_TURN_POS] = true;	// �U������ʒu�ɐU������t���O��^�ɂ���
				break;
			case /*/ �U������ʒu�ɐU������I�� /*/CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_TURN_POS] = false;	// �U������ʒu�ɐU������t���O���U�ɂ���
				break;
			case /*/ �ړ������ɐU������J�n /*/CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT] = true;	// �ړ������ɐU������t���O��^�ɂ���
				break;
			case /*/ �ړ������ɐU������I�� /*/CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT] = false;	// �ړ������ɐU������t���O���U�ɂ���
				break;
			case /*/ Z���W�����b�N /*/CHR_ENEMY_00_COMMAND_Z_LOCK:
				pActInfo->aAct[CHR_ENEMY_00_ACT_Z_LOCK] = true;	// Z���W���b�N�t���O��^�ɂ���
				break;
			case /*/ Z���W���A�����b�N /*/CHR_ENEMY_00_COMMAND_Z_UNLOCK:
				pActInfo->aAct[CHR_ENEMY_00_ACT_Z_LOCK] = false;	// Z���W���b�N�t���O���U�ɂ���
				break;
			case /*/ �q�ݒ� /*/CHR_ENEMY_00_COMMAND_CHILD_SET: {
				D3DXVECTOR3 setPos = pChr->pos;	// �ݒ�ʒu

				// �ݒ�ʒu��ݒ�
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				
				// �q�̐ݒ菈��
				SetChildChr_enemy_00(
					setPos,
					pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI),
					pAct->aData[6],
					SearchIndexChr_enemy_00(pChr));

				break;
			}
			case /*/ �q�ˑ� /*/CHR_ENEMY_00_COMMAND_CHILD_DEPENDENCY:
				pActInfo->aAct[CHR_ENEMY_00_ACT_CHILD_DEPENDENCY] = true;	// �q�ˑ��t���O��^�ɂ���
				break;
			case /*/ ��]�J�n /*/CHR_ENEMY_00_COMMAND_SPIN_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_SPIN] = true;	// ��]�t���O��^�ɂ���
				pChr->actInfo.spin.x = pAct->aData[0];
				pChr->actInfo.spin.y = pAct->aData[1];
				pChr->actInfo.spin.z = pAct->aData[2];
				break;
			case /*/ ��]�I�� /*/CHR_ENEMY_00_COMMAND_SPIN_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_SPIN] = false;	// ��]�t���O���U�ɂ���
				break;
			case /*/ �e���� /*/CHR_ENEMY_00_COMMAND_SHOT: {
				D3DXVECTOR3 setPos = pChr->pos;	// �ݒ�ʒu

				// �ݒ�ʒu��ݒ�
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// ATK:�e[00] �̐ݒ菈��
				SetAtk_bullet_00(
					setPos,
					pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI),
					pAct->aData[6],
					ATK_BULLET_00_PARENTTYPE_ENEMY);

				break;
			}
			case /*/ �T�E���h�Đ� /*/CHR_ENEMY_00_COMMAND_PLAY_SOUND:
				PlaySound((SOUND_LABEL)(int)pAct->aData[0]);
				break;
			case /*/ ���[�V�����ݒ� /*/CHR_ENEMY_00_COMMAND_MOTION_SET: {
				Motion3D *pMotion // �V�K�̃��[�V�����̏��̃|�C���^
					= &pType->motionSet.aMotion[(int)pAct->aData[0]];
				if (!pMotion->bLoop)
				{// �V�K�̃��[�V�����̃��[�v�t���O���U�̎��A
					pChr->nMotionTemp = pChr->nMotion;	// ���݂̃��[�V�����ԍ���ۑ�
				}
				pChr->nMotion = pAct->aData[0]; // ���[�V�����ԍ�����
				SetMotion3D( // ���[�V�����ݒ�
					&pChr->partsInfo,
					pChr->nMotion);
				break;
			}
			case /*/ �����V���b�g�J�n /*/CHR_ENEMY_00_COMMAND_AUTO_SHOT_START:
				pChr->nBulletType	= pAct->aData[0];	// �e�̎�ނ���
				pChr->nShotTimeMin	= pAct->aData[1];	// ���ˊԊu�̉�������
				pChr->nShotTimeMax	= pAct->aData[2];	// ���ˊԊu�̏������
				pChr->bAutoShot		= true;				// �����V���b�g�t���O��^�ɂ���

				pChr->nCounterShot		// �V���b�g�J�E���^�[��ݒ�
					= pChr->nShotTimeMin + (rand() % (pChr->nShotTimeMax - pChr->nShotTimeMin));
				break;
			case /*/ �����V���b�g�I�� /*/CHR_ENEMY_00_COMMAND_AUTO_SHOT_END:
				pChr->bAutoShot = false;	// �����V���b�g�t���O���U�ɂ���
				break;
			case /*/ ���j�b�g�����J�n /*/CHR_ENEMY_00_COMMAND_UNIT_IGNORED_START:
				pChr->bUnitIgnored = true;	// ���j�b�g�����t���O��^�ɂ���
				break;
			case /*/ ���j�b�g�����I�� /*/CHR_ENEMY_00_COMMAND_UNIT_IGNORED_END:
				pChr->bUnitIgnored = false;	// ���j�b�g�����t���O���U�ɂ���
				break;
			case /*/ �ړ��J�n /*/CHR_ENEMY_00_COMMAND_MOVE_START:
				pActInfo->aAct[CHR_ENEMY_00_ACT_MOVE] = true;	// �ړ��t���O��^�ɂ���
				pChr->actInfo.move.x = pAct->aData[0];
				pChr->actInfo.move.y = pAct->aData[1];
				pChr->actInfo.move.z = pAct->aData[2];
				break;
			case /*/ �ړ��I�� /*/CHR_ENEMY_00_COMMAND_MOVE_END:
				pActInfo->aAct[CHR_ENEMY_00_ACT_MOVE] = false;	// �ړ��t���O���U�ɂ���
				break;
			case /*/ �����ݒ� /*/CHR_ENEMY_00_COMMAND_ALLY_SET: {
				D3DXVECTOR3 setPos = pChr->pos;	// �ݒ�ʒu
				D3DXVECTOR3 setRot 				// �ݒ����
							= pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI);

				// �ݒ�ʒu��ݒ�
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// �Փ˔���ɕK�v�ȏ��
				CollisionInfo myCollInfo = {
					&setPos,
					pChr->pos,
					NULL,
					&setRot,
					setRot,
					g_aChr_enemy_00Type[(int)pAct->aData[6]].hitTestSet.aHitTest[0] };

				Collision collision = {};	// �Փˏ��

				// OBJ:�X�e�[�W[00] �Ƃ̏Փ˔���
				CollisionObj_stage_00(VECTOR_X, &collision, &collision, myCollInfo);
				CollisionObj_stage_00(VECTOR_Z, &collision, &collision, myCollInfo);

				if (!collision.bHit)
				{// �����I�ɐڐG���Ă��Ȃ����A
					// CHR:�G[00] �̐ݒ菈��
					SetChr_enemy_00(
						setPos,
						setRot,
						pAct->aData[6]);
				}

				break;
			}
			case /*/ ATK:�Ռ� /*/CHR_ENEMY_00_COMMAND_ATK_IMPACT: {
				D3DXVECTOR3 setPos = pChr->pos;	// �ݒ�ʒu
				D3DXVECTOR3 setRot 				// �ݒ����
					= pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI);

				// �ݒ�ʒu��ݒ�
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// ATK:�Ռ�[00] �̐ݒ菈��
				SetAtk_impact_00(
					setPos,
					setRot,
					pAct->aData[6],
					ATK_IMPACT_00_PARENTTYPE_ENEMY);

				break;
			}
			case /*/ EFF:�Ռ��g[00] /*/CHR_ENEMY_00_COMMAND_EFF_SHOCKWAVE_00: {
				D3DXVECTOR3 setPos = pChr->pos;	// �ݒ�ʒu
				D3DXVECTOR3 setRot 				// �ݒ����
					= pChr->rot + D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI);

				// �ݒ�ʒu��ݒ�
				setPos.y += pAct->aData[1];
				setPos.x += (sinf(pChr->rot.y)*pAct->aData[2]) + (sinf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);
				setPos.z += (cosf(pChr->rot.y)*pAct->aData[2]) + (cosf(pChr->rot.y - (D3DX_PI*0.5f))*pAct->aData[0]);

				// EFF:�Ռ��g[00] �̐ݒ菈��
				SetEff_shockWave_00(
					setPos,
					setRot,
					pAct->aData[6],
					pAct->aData[7],
					pAct->aData[8],
					{ (int)pAct->aData[9],(int)pAct->aData[10],(int)pAct->aData[11],(int)pAct->aData[12] },
					{ (int)pAct->aData[13],(int)pAct->aData[14],(int)pAct->aData[15],(int)pAct->aData[16] },
					(int)pAct->aData[17]);

				break;
			}
			case /*/ �Փ˃_���[�W�ύX /*/CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_CHANGE:
				pChr->nNewCollDamage = pAct->aData[0];	// �V�K�Փ˃_���[�W��ݒ�
				pChr->bCollDamageChange = true;			// �Փ˃_���[�W�ύX�t���O��^�ɂ���
				break;
			case /*/ �Փ˃_���[�W�����ɖ߂� /*/CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_UNDO:
				pChr->bCollDamageChange = false;	// �Փ˃_���[�W�ύX�t���O���U�ɂ���
				break;
			case /*/ �{�X�ˑ� /*/CHR_ENEMY_00_COMMAND_BOSS_DEPENDENCY:
				pChr->bBossDependency = true;	// �{�X�ˑ��t���O��^�ɂ���
				break;
			case /*/ �����ݒ�(��Έʒu) /*/CHR_ENEMY_00_COMMAND_ALLY_SET_ABSOLUTE:
				// CHR:�G[00] �̐ݒ菈��
				SetChr_enemy_00(
					D3DXVECTOR3(pAct->aData[0], pAct->aData[1], pAct->aData[2]),
					D3DXVECTOR3(pAct->aData[3] * D3DX_PI, pAct->aData[4] * D3DX_PI, pAct->aData[5] * D3DX_PI),
					pAct->aData[6]);
				break;
			case /*/ �F��OFF /*/CHR_ENEMY_00_COMMAND_RECOGNITION_OFF:
				pChr->bRecognitionOff = true;	// �F��OFF�t���O��^�ɂ���
				break;
			case /*/ �u�Ԉړ� /*/CHR_ENEMY_00_COMMAND_TELEPORTATION:
				// �ʒu��ݒ肷��
				pChr->pos = D3DXVECTOR3(pAct->aData[0], pAct->aData[1], pAct->aData[2]);

				// �q�̈ړ�����
				ChildMoveChr_enemy_00(pChr);

				// �ߋ��̈ʒu��ݒ肷��(�Փ˖h�~)
				pChr->posOld = D3DXVECTOR3(pAct->aData[0], pAct->aData[1], pAct->aData[2]);
				break;
		}
	}

	if (!((pActInfo->nCounterAct == 0) && (pActPtn->nLoopTime == 0)))
	{// (�s���J�E���^�[��0 & ���[�v���Ԃ�0) �łȂ����A
		pActInfo->nCounterAct++;	// �s���J�E���^�[�����Z
		IntLoopControl(				// �s���J�E���^�[�𐧌�
			&pActInfo->nCounterAct,
			pActPtn->nLoopTime + 1,
			0);
	}
}

//========================================
// ActChr_enemy_00�֐� - CHR:�G[00] �̍s������ -
//========================================
void ActChr_enemy_00(Chr_enemy_00 *pChr)
{
	// �s���ǂݍ��ݏ���
	ActLoadChr_enemy_00(pChr);

	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->bAutoShot)
	{// �����V���b�g�t���O���^�̎��A
		if (--pChr->nCounterShot <= 0)
		{// �V���b�g�J�E���^�[�����Z��������0�ȉ��̎��A
			SetAtk_bullet_00(pChr->pos, pChr->rot, pChr->nBulletType, ATK_BULLET_00_PARENTTYPE_ENEMY);		// ATK:�e[00] �̐ݒ菈��
			pChr->nCounterShot = pChr->nShotTimeMin + (rand() % (pChr->nShotTimeMax - pChr->nShotTimeMin));	// �V���b�g�J�E���^�[��ݒ�
		}
	}

	if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk > 0))
	{// �ՓˍU���̊Ԋu��-1�łȂ� & �ՓˍU���J�E���^�[��0�������Ă��鎞�A
		pChr->nCounterCollAtk--;	// �ՓˍU���J�E���^�[�����Z
	}

	Chr_enemy_00ActInfo *pActInfo	// �s���Ǘ����
						= &pChr->actInfo;

	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_MAX; nCntAct++)
	{
		if (!pActInfo->aAct[nCntAct]) 
		{// �J�E���g�̍s���t���O���U�̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// �s���ɉ���������
		switch (nCntAct)
		{
		case /*/ �Ȑ܎��ړ� /*/CHR_ENEMY_00_ACT_TORTUOUS_MOVE: {
			if (--pActInfo->nCounterMove <= 0)
			{// �ړ��J�E���^�[�����Z��������0�ȉ��̎��A
				if ((FindDistance(
					D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
					D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f))
					<= pType->fPlayerSearchDistance)
					&&
					(Lottery(pType->fPlayerAimProbability))
					&&
					(GetChr_player_00()->state != CHR_PLAYER_00_STATE_DOWN))
				{// �v���C���[�Ƃ̋��������G�����̓��� & �v���C���[��_���m���̒��I�ɓ������� & �v���C���[�̏�Ԃ��_�E���łȂ����A
					pChr->moveRot.y = FindAngle(	// �ړ��������v���C���[�܂ł̊p�x�ɐݒ�
						D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
						D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f));
					pChr->pTurnPos = &GetChr_player_00()->pos;	// �U�������̈ʒu�̃|�C���^���v���C���[�̈ʒu�Őݒ�
				}
				else if (Lottery(pType->fCoreAimProbability))
				{// �R�A��_���m���̒��I�ɓ����������A
					pChr->moveRot.y = FindAngle(	// �ړ��������R�A�܂ł̊p�x�ɐݒ�
						D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
						D3DXVECTOR3(GetObj_core_00()->pos.x, GetObj_core_00()->pos.z, 0.0f));
					pChr->pTurnPos = &GetObj_core_00()->pos;	// �U�������̈ʒu�̃|�C���^���R�A�̈ʒu�Őݒ�
				}
				else
				{// ��������Y�����Ȃ����A
					pChr->moveRot.y = -D3DX_PI + fRand(D3DX_PI * 2.0f);	// �ړ������𗐐��Őݒ�
				}

				pActInfo->nCounterMove =	// �ړ��J�E���^�[�𗐐��Őݒ�
					CHR_ENEMY_00_MOVE_COUNTER_MIN + (rand() % (CHR_ENEMY_00_MOVE_COUNTER_MAX - CHR_ENEMY_00_MOVE_COUNTER_MIN));
			}

			// �ړ�����
			MoveChr_enemy_00(pChr, pType->fMoveForce);

			break;
		}
		case /*/ �����]�����ړ� /*/CHR_ENEMY_00_ACT_TURN_MOVE: {
			if (--pActInfo->nCounterMove <= 0)
			{// �ړ��J�E���^�[�����Z��������0�ȉ��̎��A
				if ((FindDistance(
					D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
					D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f))
					<= pType->fPlayerSearchDistance)
					&&
					(Lottery(pType->fPlayerAimProbability))
					&&
					(GetChr_player_00()->state != CHR_PLAYER_00_STATE_DOWN))
				{// �v���C���[�Ƃ̋��������G�����̓��� & �v���C���[��_���m���̒��I�ɓ������� & �v���C���[�̏�Ԃ��_�E���łȂ����A
					pChr->pTurnPos = &GetChr_player_00()->pos;	// �U�������̈ʒu�̃|�C���^���v���C���[�̈ʒu�Őݒ�
				}
				else if (Lottery(pType->fCoreAimProbability))
				{// �R�A��_���m���̒��I�ɓ����������A
					pChr->pTurnPos = &GetObj_core_00()->pos;	// �U�������̈ʒu�̃|�C���^���R�A�̈ʒu�Őݒ�
				}
				else
				{// ��������Y�����Ȃ����A�U�������̈ʒu�𗐐��Őݒ�
					float	fAngle	// �ݒ����
							= -D3DX_PI + fRand(D3DX_PI * 2);
					pChr->pTurnPos =
						&D3DXVECTOR3(
							sinf(fAngle) * pActInfo->fRandPosRadius,
							0.0f,
							cosf(fAngle) * pActInfo->fRandPosRadius);
				}

				pActInfo->nCounterMove =	// �ړ��J�E���^�[�𗐐��Őݒ�
					CHR_ENEMY_00_MOVE_COUNTER_MIN + (rand() % (CHR_ENEMY_00_MOVE_COUNTER_MAX - CHR_ENEMY_00_MOVE_COUNTER_MIN));
			}

			// �����]������
			TurnAroundLookDown(&pChr->moveRot, pChr->pos, *pChr->pTurnPos, pActInfo->fRotation);

			// �ړ�����
			MoveChr_enemy_00(pChr, pType->fMoveForce);
			break;
		}
		case /*/ ���i���ړ� /*/CHR_ENEMY_00_ACT_FORCE_MOVE: {
			break;
		}
		case /*/ �U������ʒu�ɐU����� /*/CHR_ENEMY_00_ACT_TURN_TO_TURN_POS: {
			if (pChr->pTurnPos != NULL)
			{// �U�������̈ʒu�̃|�C���^���ݒ肳��Ă��鎞�A
				// ������U�������̈ʒu�Ɍ����Đ��ڂ���
				pChr->rot.y += (
					AngleDifference(
						pChr->rot.y,
						FindAngle(
							D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
							D3DXVECTOR3(pChr->pTurnPos->x, pChr->pTurnPos->z, 0.0f)))
					* CHR_ENEMY_00_ROT_DIAMETER);
			}
			break;
		}
		case /*/ �ړ������ɐU����� /*/CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT:
			// �������ړ������Ɍ����Đ��ڂ���
			pChr->rot.y += (
				AngleDifference(
					pChr->rot.y,
					pChr->moveRot.y)
				* CHR_ENEMY_00_ROT_DIAMETER);
			break;
		case /*/ ��] /*/CHR_ENEMY_00_ACT_SPIN:
			// �����ɉ�]�ʂ����Z
			pChr->rot += pChr->actInfo.spin;
			break;
		case /*/ �ړ� /*/CHR_ENEMY_00_ACT_MOVE:
			// ���ړ��ʂɈړ��ʂ����Z(�����ɉ�����)
			// X
			pChr->move.x += sinf(pChr->rot.y - (D3DX_PI*0.5f)) * pChr->actInfo.move.x;
			pChr->move.z += cosf(pChr->rot.y - (D3DX_PI*0.5f)) * pChr->actInfo.move.x;
			// Z
			pChr->move.x += sinf(pChr->rot.y) * pChr->actInfo.move.z;
			pChr->move.z += cosf(pChr->rot.y) * pChr->actInfo.move.z;
			// Y
			pChr->move.y += pChr->actInfo.move.y;
			break;
		}
	}

	// �ړ������𐧌�
	RotControl(&pChr->moveRot);
}

//========================================
// MoveChr_enemy_00�֐� - CHR:�G[00] �̈ړ����� -
//========================================
void MoveChr_enemy_00(Chr_enemy_00 *pChr, float fMove)
{
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->state == CHR_ENEMY_00_STATE_PARALYSIS) 
	{// ��Ԃ���Ⴢ̎��A
		fMove *= CHR_ENEMY_00_PARALYSIS_DECREMENT_RATE;	// �ړ��͂��X�V
	}

	// �ړ��ʂ��X�V
	pChr->move.x += sinf(pChr->moveRot.y) * fMove;
	pChr->move.z += cosf(pChr->moveRot.y) * fMove;
}

//========================================
// UpdatePosChr_enemy_00�֐� - CHR:�G[00] �̈ʒu�X�V���� -
//========================================
void UpdatePosChr_enemy_00(Chr_enemy_00 *pChr)
{
	// ���ʂ̏Փˏ���������
	pChr->cmnCollision = {};

	// �I�u�W�F�N�g���̏Փˏ���������
	for (int nCntHitObj = 0; nCntHitObj < CHR_ENEMY_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};
	}

	pChr->nAttackIndex = -1;	// �U������I�u�W�F�N�g�̔ԍ���������

	D3DXVECTOR3 posTemp;	// �ʒu��ۑ�

	pChr->pos.x += pChr->move.x / (1.0f + fabsf(pChr->force.x));	// X���W�Ɉړ��ʂ�K�p
	pChr->move.x *= CHR_ENEMY_00_MOVE_DAMP;							// X�����̈ړ��ʂ�����
	pChr->pos.x += pChr->force.x;									// X���W�ɗ͂�K�p
	pChr->force.x *= CHR_ENEMY_00_FORCE_DAMP;						// X�����̗͂�����
	if (pChr->nParentIdx != -1) { posTemp.x = pChr->pos.x; }		// X���W��ۑ�(�e�����̂�)
	CheckCollisionChr_enemy_00(pChr, VECTOR_X);						// X�����̏Փ˃`�F�b�N����
	if (pChr->nParentIdx != -1) { pChr->pos.x = posTemp.x; }		// X���W��߂�(�e�����̂�)

	if (!pChr->actInfo.aAct[CHR_ENEMY_00_ACT_Z_LOCK])
	{// Z���W���b�N���U�̎��A
		pChr->pos.z += pChr->move.z / (1.0f + fabsf(pChr->force.z));	// Z���W�Ɉړ��ʂ�K�p
		pChr->move.z *= CHR_ENEMY_00_MOVE_DAMP;							// Z�����̈ړ��ʂ�����
		pChr->pos.z += pChr->force.z;									// Z���W�ɗ͂�K�p
		pChr->force.z *= CHR_ENEMY_00_FORCE_DAMP;						// Z�����̗͂�����
	}
	if (pChr->nParentIdx != -1) { posTemp.z = pChr->pos.z; }			// Z���W��ۑ�(�e�����̂�)
	CheckCollisionChr_enemy_00(pChr, VECTOR_Z);							// Z�����̏Փ˃`�F�b�N����
	if (pChr->nParentIdx != -1) { pChr->pos.z = posTemp.z; }			// Z���W��߂�(�e�����̂�)

	// �Փˏ��ɉ���������
	CollisionProcessChr_enemy_00(pChr);

	// �q�̈ړ�����
	ChildMoveChr_enemy_00(pChr);
}

//========================================
// CheckCollisionChr_enemy_00�֐� - CHR:�G[00] �̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionChr_enemy_00(Chr_enemy_00 *pChr, VECTOR vector)
{
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[pChr->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo	myCollInfo =
	{
		&pChr->pos,
		pChr->posOld,
		NULL,
		&pChr->rot,
		pChr->rot,
		pType->hitTestSet.aHitTest[pChr->hitTestInfo.nHitTest]
	};

	// �Փ˔���
	/*/ CHR:�v���C���[	[00] /*/CollisionChr_player_00	(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_CHR_PLAYER_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:�R�A		[00] /*/CollisionObj_core_00	(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_CORE_00]	, &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:�X�e�[�W	[00] /*/CollisionObj_stage_00	(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_STAGE_00]	, &pChr->cmnCollision, myCollInfo);

	// �Փ˔���(���j�b�g)
	if(!pChr->bUnitIgnored)
	{// ���j�b�g�����t���O���U�̎��A
		int nIndex;	// �ԍ�

		nIndex = /*/ OBJ:�u���b�N	[00] /*/CollisionObj_block_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// �ՓˍU���̊Ԋu��-1�łȂ� & �ՓˍU���J�E���^�[��0�ȉ� & �U������I�u�W�F�N�g�ԍ���-1�̎��A
			if (-1 != nIndex) {// OBJ:�u���b�N	[00] �ƐڐG�������A
				pChr->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
				pChr->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
					= CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00;
			}
		}
		nIndex = /*/ OBJ:�^���b�g	[00] /*/CollisionObj_turret_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// �ՓˍU���̊Ԋu��-1�łȂ� & �ՓˍU���J�E���^�[��0�ȉ� & �U������I�u�W�F�N�g�ԍ���-1�̎��A
			if (-1 != nIndex) {// OBJ:�^���b�g	[00] �ƐڐG�������A
				pChr->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
				pChr->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
					= CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00;
			}
		}
		nIndex = /*/ OBJ:���d���u	[00] /*/CollisionObj_discharger_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// �ՓˍU���̊Ԋu��-1�łȂ� & �ՓˍU���J�E���^�[��0�ȉ� & �U������I�u�W�F�N�g�ԍ���-1�̎��A
			if (-1 != nIndex) {// OBJ:���d���u	[00] �ƐڐG�������A
				pChr->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
				pChr->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
					= CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00;
			}
		}
		nIndex = /*/ OBJ:�~���[	[00] /*/CollisionObj_mirror_00(vector, &pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00], &pChr->cmnCollision, myCollInfo);
		if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
		{// �ՓˍU���̊Ԋu��-1�łȂ� & �ՓˍU���J�E���^�[��0�ȉ� & �U������I�u�W�F�N�g�ԍ���-1�̎��A
			if (-1 != nIndex) {// OBJ:�~���[	[00] �ƐڐG�������A
				pChr->nAttackIndex = nIndex;	// �U������I�u�W�F�N�g�̔ԍ���ݒ�
				pChr->attackObj					// �U������I�u�W�F�N�g�̕��ނ�ݒ�
					= CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00;
			}
		}
	}

	if ((pType->nCollAtkTime != -1) && (pChr->nCounterCollAtk <= 0) && (pChr->nAttackIndex == -1))
	{// �ՓˍU���̊Ԋu��-1�łȂ� & �ՓˍU���J�E���^�[��0�ȉ� & �U������I�u�W�F�N�g�̔ԍ���-1�̎��A
		bool	bAttack = true;	// �U���t���O

		if (pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_CHR_PLAYER_00].bHit)
		{// CHR:�v���C���[[00] �Ƃ̓�����t���O���^�̎��A
			int nCollDamage = pChr->bCollDamageChange ? pChr->nNewCollDamage : pType->nCollisionDamage;
			int nDamage	// �_���[�W
				= nCollDamage * (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));

			DamageChr_player_00(nDamage);	// �_���[�W����
			KnockBackChr_player_00(							// �m�b�N�o�b�N����
				D3DXVECTOR3(0.0f,
					FindAngle(
						D3DXVECTOR3(pChr->pos.x, pChr->pos.z, 0.0f),
						D3DXVECTOR3(GetChr_player_00()->pos.x, GetChr_player_00()->pos.z, 0.0f)), 0.0f),
				CHR_ENEMY_00_COLLISION_KNOCK_BACK);
		}
		else if (pChr->aObjCollision[CHR_ENEMY_00_HITOBJ_OBJ_CORE_00].bHit)
		{// OBJ:�R�A[00] �Ƃ̓�����t���O���^�̎��A
			DamageObj_core_00(pType->nCollisionDamage);	// �_���[�W����
		}
		else 
		{// ��������Y�����Ȃ����A
			bAttack = false;	// �U���t���O���U�ɂ���
		}

		if (bAttack) 
		{// �U���t���O���^�̎��A
			pChr->nCounterCollAtk = pType->nCollAtkTime;	// �ՓˍU���J�E���^�[��ݒ�
			pChr->nAttackIndex = -1;						// �U������I�u�W�F�N�g�̔ԍ���������
		}
	}
}

//========================================
// CollisionProcessChr_enemy_00�֐� - CHR:�G[00] �̏Փˏ��ɉ��������� -
//========================================
void CollisionProcessChr_enemy_00(Chr_enemy_00 *pChr)
{
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->nAttackIndex != -1)
	{// �U������I�u�W�F�N�g�̔ԍ���-1�Ŗ������A
		int nCollDamage = pChr->bCollDamageChange ? pChr->nNewCollDamage : pType->nCollisionDamage;
		int nDamage	// �_���[�W
			= nCollDamage * (1.0f + (CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE * GetObj_stage_00()->nCntWaveMain));

		// �U������I�u�W�F�N�g�̕��ނɉ������_���[�W����
		switch (pChr->attackObj)
		{
			case /*/ OBJ:�u���b�N	[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00:
				DamageObj_block_00(pChr->nAttackIndex, nDamage);
				break;
			case /*/ OBJ:�^���b�g	[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00:
				DamageObj_turret_00(pChr->nAttackIndex, nDamage);
				break;
			case /*/ OBJ:�~���[		[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00:
				DamageObj_mirror_00(pChr->nAttackIndex, nDamage);
				break;
			case /*/ OBJ:���d���u	[00] /*/CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00:
				DamageObj_discharger_00(pChr->nAttackIndex, nDamage);
				break;
		}

		pChr->nCounterCollAtk = pType->nCollAtkTime;	// �ՓˍU���J�E���^�[��ݒ�
	}
}

//========================================
// DestroyChr_enemy_00�֐� -CHR:�G[00] �̔j�󏈗� -
//========================================
void DestroyChr_enemy_00(Chr_enemy_00 *pChr) 
{
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[pChr->nType];

	pChr->nHP	= 0;		// HP��0�ɂ���
	pChr->bUse	= false;	// �g�p����Ă��Ȃ���Ԃɂ���

	// EFF:����[00] �̐ݒ菈��
	SetEff_explosion_00(pChr->pos, pType->nExplosionType);

	// ITM:�R�C��[00] �̐ݒ菈��
	SetItm_coin_00(pChr->pos, pType->nDropCoin);

	if ((pChr->nParentIdx == -1) &&
		(pType->rank != CHR_ENEMY_00_RANK_ALLY))
	{// �e�����݂��Ȃ����A
		GetObj_stage_00()->nKill++;	// OBJ:�X�e�[�W[00] �̃L���������Z
	}
	
	// �v���C���[�̃X�R�A�����Z
	GetChr_player_00()->nScore += pType->nScore;

	// �q�̔j�󏈗�
	ChildDestoryChr_enemy_00(pChr);
}

//========================================
// SetChr_enemy_00Vibration�֐� - CHR:�G[00] �̐U����ݒ� -
//========================================
void SetChr_enemy_00Vibration(Chr_enemy_00 *pChr, float fVibration)
{
	// �U���̋�����ݒ�
	pChr->fVibration += fVibration;

	// �U���J�E���^�[��������
	pChr->nCounterVib = 0;
}

//========================================
// SetChildChr_enemy_00�֐� - CHR:�G[00] �̎q�̐ݒ菈�� -
//========================================
void SetChildChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nParentIdx)
{
	Chr_enemy_00		*pChr	// CHR:�G[00] �̏��̃|�C���^
						= g_aChr_enemy_00;
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[nType];

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		pChr->pos =						// �ʒu����
		pChr->posOld = pos;				// �ߋ��̈ʒu��ݒ�
		pChr->rot =						// ��������
		pChr->rotOld = 					// �ߋ��̌�������
		pChr->moveRot = rot;			// �ړ���������
		pChr->nType = nType;			// ��ނ���
		pChr->nParentIdx = nParentIdx;	// �e�̔ԍ�����

		// �ݒ莞�̃p�����[�^�[����������
		InitParameterSetChr_enemy_00(pChr);

		// �s���ǂݍ��ݏ���
		ActLoadChr_enemy_00(pChr);

		// �J��Ԃ������𔲂���
		break;
	}

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
	}
}

//========================================
// ChildMoveChr_enemy_00�֐�- CHR:�G[00] �̎q�̈ړ����� -
//========================================
void ChildMoveChr_enemy_00(Chr_enemy_00 *pChr)
{
	int nParentIdx	// �e�ԍ�
		= SearchIndexChr_enemy_00(pChr);

	// �ʒu/�����̐��ڒl
	D3DXVECTOR3	posTrans = pChr->pos - pChr->posOld;
	D3DXVECTOR3	rotTrans = pChr->rot - pChr->rotOld;

	Chr_enemy_00	*pChildChr	// �q��CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;	

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx))
		{// �g�p����Ă��Ȃ� or �e�ԍ�����v���Ă��Ȃ����A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}
		
		// �ʒu���X�V
		pChildChr->pos += posTrans;

		D3DXVECTOR3 rot	// �e����q�ւ̌���
				= D3DXVECTOR3(
				0.0f,
				FindAngleLookDown(pChr->pos, pChildChr->pos) + rotTrans.y,
				0.0f);
		float	fDistance	// �e�Ǝq�̋���
				= FindDistanceLookDown(pChr->pos, pChildChr->pos);

		// �q�Ɍ����̐��ڒl��K�p
		pChildChr->rot += rotTrans;

		// �ʒu��e�̏��ɖ߂�����A�������p�x�ɋ��������Z����
		pChildChr->pos.x = pChr->pos.x;
		pChildChr->pos.z = pChr->pos.z;
		pChildChr->pos.x += sinf(rot.y) * fDistance;
		pChildChr->pos.z += cosf(rot.y) * fDistance;
	}
}

//========================================
// ChildSetDamageCounterChr_enemy_00�֐�- CHR:�G[00] �̎q�̃_���[�W�J�E���^�[�ݒ菈�� -
//========================================
void ChildSetDamageCounterChr_enemy_00(Chr_enemy_00 *pChr)
{
	int				nParentIdx	// �e�ԍ�
					= SearchIndexChr_enemy_00(pChr);
	Chr_enemy_00	*pChildChr	// �q��CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx)
			&&
			(!((pChr->nParentIdx == nCntChr) && (pChildChr->actInfo.aAct[CHR_ENEMY_00_ACT_CHILD_DEPENDENCY]))))
		{// �g�p����Ă��Ȃ� or �e�ԍ�����v���Ă��Ȃ� or �q�ˑ��̐e�̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		pChildChr->nCounterDamage	// �_���[�W�J�E���^�[��ݒ�
			= CHR_ENEMY_00_DAMAGE_TIME;
	}
}

//========================================
// ShildAppealMotionSetChr_enemy_00�֐�- CHR:�G[00] �̎q�̃A�s�[�����[�V�����ݒ菈�� -
//========================================
void ShildAppealMotionSetChr_enemy_00(Chr_enemy_00 *pChr) 
{
	int				nParentIdx	// �e�ԍ�
					= SearchIndexChr_enemy_00(pChr);
	Chr_enemy_00	*pChildChr	// �q��CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChild = 0; nCntChild < CHR_ENEMY_00_MAX; nCntChild++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx))
		{// �g�p����Ă��Ȃ� or �e�ԍ�����v���Ă��Ȃ� or �q�ˑ��̐e�̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// ���݂̃��[�V�����ԍ���ۑ�
		pChildChr->nMotionTemp = pChildChr->nMotion;

		// ���[�V�������A�s�[���Őݒ�
		pChildChr->nMotion = CHR_ENEMY_00_APPEAL_MOTION;
		SetMotion3D(
			&pChildChr->partsInfo,
			pChildChr->nMotion);

		{
			int				nParentIdx2		// �e�ԍ�
							= SearchIndexChr_enemy_00(pChildChr);
			Chr_enemy_00	*pGrandChildChr	// ����CHR:�G[00] �̏��̃|�C���^
							= g_aChr_enemy_00;

			for (int nCntGrandChild = 0; nCntGrandChild < CHR_ENEMY_00_MAX; nCntGrandChild++, pGrandChildChr++)
			{
				if ((!pGrandChildChr->bUse)
					||
					(pGrandChildChr->nParentIdx != nParentIdx2))
				{// �g�p����Ă��Ȃ� or �e�ԍ�����v���Ă��Ȃ� or ���ˑ��̐e�̎��A
					continue;	// �J��Ԃ�������܂�Ԃ�
				}

				// ���݂̃��[�V�����ԍ���ۑ�
				pGrandChildChr->nMotionTemp = pGrandChildChr->nMotion;

				// ���[�V�������A�s�[���Őݒ�
				pGrandChildChr->nMotion = CHR_ENEMY_00_APPEAL_MOTION;
				SetMotion3D(
					&pGrandChildChr->partsInfo,
					pGrandChildChr->nMotion);
			}
		}
	}
}

//========================================
// ChildDestoryChr_enemy_00�֐�- CHR:�G[00] �̎q�̔j�󏈗� -
//========================================
void ChildDestoryChr_enemy_00(Chr_enemy_00 *pChr)
{
	int				nParentIdx	// �e�ԍ�
					= SearchIndexChr_enemy_00(pChr);
	Chr_enemy_00	*pChildChr	// �q��CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((!pChildChr->bUse)
			||
			(pChildChr->nParentIdx != nParentIdx)
			&&
			(!((pChr->nParentIdx == nCntChr) && (pChildChr->actInfo.aAct[CHR_ENEMY_00_ACT_CHILD_DEPENDENCY]))))
		{// �g�p����Ă��Ȃ� or �e�ԍ�����v���Ă��Ȃ� or �q�ˑ��̐e�̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// �j�󏈗�
		DestroyChr_enemy_00(pChildChr);
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_enemy_00�֐� - CHR:�G[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadChr_enemy_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Chr_enemy_00Type	*pChrType	// CHR:�G[00] �̎�ޖ��̏��
						= g_aChr_enemy_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(CHR_ENEMY_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	g_nChr_enemy_00TypeNum = 0;	// ��ސ���������

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// �ǂݍ��݂��I��
		if (aDataSearch[0] == '#')			{ continue; }				// �܂�Ԃ�

		if (!strcmp(aDataSearch, "TYPE")) {
			g_nChr_enemy_00TypeNum++;	// ��ސ������Z
			// CHR:�G[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))					{ pChrType++; break; }										// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "HIT_POINT:"))				{ fscanf(pFile, "%d", &pChrType->nHP); }					// HP
				else if (!strcmp(aDataSearch, "MOVE_FORCE:"))				{ fscanf(pFile, "%f", &pChrType->fMoveForce); }				// �ړ���
				else if (!strcmp(aDataSearch, "COLLISION_DAMAGE:"))			{ fscanf(pFile, "%d", &pChrType->nCollisionDamage); }		// �Փ˃_���[�W
				else if (!strcmp(aDataSearch, "COLLATK_TIME:"))				{ fscanf(pFile, "%d", &pChrType->nCollAtkTime); }			// �ՓˍU���̊Ԋu
				else if (!strcmp(aDataSearch, "EXPLOSION_TYPE:"))			{ fscanf(pFile, "%d", &pChrType->nExplosionType); }			// �����̎��
				else if (!strcmp(aDataSearch, "SHADOW_RADIUS:"))			{ fscanf(pFile, "%f", &pChrType->fShadowRadius); }			// �e�̔��a
				else if (!strcmp(aDataSearch, "PLAYER_SEARCH_DISTANCE:"))	{ fscanf(pFile, "%f", &pChrType->fPlayerSearchDistance); }	// �v���C���[���G����
				else if (!strcmp(aDataSearch, "PLAYER_AIM_PROBABILITY:"))	{ fscanf(pFile, "%f", &pChrType->fPlayerAimProbability); }	// �v���C���[��_���m��
				else if (!strcmp(aDataSearch, "CORE_AIM_PROBABILITY:"))		{ fscanf(pFile, "%f", &pChrType->fCoreAimProbability); }	// �R�A��_���m��
				else if (!strcmp(aDataSearch, "DROP_COIN:"))				{ fscanf(pFile, "%d", &pChrType->nDropCoin); }				// �h���b�v����R�C����
				else if (!strcmp(aDataSearch, "SCORE:"))					{ fscanf(pFile, "%d", &pChrType->nScore); }					// �X�R�A
				else if (!strcmp(aDataSearch, "RANK:"))						{ fscanf(pFile, "%d", &pChrType->rank); }					// �����N
				else if (!strcmp(aDataSearch, "DAMAGE_SE:"))				{ fscanf(pFile, "%d", &pChrType->damageSE); }				// �_���[�WSE
				else if (!strcmp(aDataSearch, "ACTPTNSET"))					{ LoadActPtnSetChr_enemy_00(pFile, &pChrType->actPtnSet); }	// �s���p�^�[���ݒ���
				else if (!strcmp(aDataSearch, "HITTESTSET"))				{ LoadHitTestSet(pFile, &pChrType->hitTestSet); }			// �����蔻��ݒ���
				else if (!strcmp(aDataSearch, "PARTSSET"))					{ LoadParts3DSet(pFile, &pChrType->partsSet); }				// ���i�ݒ���
				else if (!strcmp(aDataSearch, "MOTIONSET"))					{ LoadMotion3DSet(pFile, &pChrType->motionSet); }			// ���[�V�����ݒ���
			}
		}
	}
}

//========================================
// InitChr_enemy_00�֐� - CHR:�G[00] �̏��������� -
//========================================
void InitChr_enemy_00(void)
{
	// �Ǘ����̃p�����[�^�[�̏�����
	InitParameterChr_enemy_00Control();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < g_nChr_enemy_00TypeNum; nCntType++)
	{
		Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_enemy_00Type[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			Parts3DType *pPartsType	// ���i(3D)�̎�ޖ��̏��\����
						= &pType->partsSet.aPartsType[nCntParts];

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aBuffMatChr_enemy_00[nCntType][nCntParts],
				NULL,
				&g_aNumMatChr_enemy_00[nCntType][nCntParts],
				&g_aMeshChr_enemy_00[nCntType][nCntParts]);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aBuffMatChr_enemy_00[nCntType][nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_enemy_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureChr_enemy_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_enemy_00Type[pChr->nType];

		// �p�����[�^�[�̏���������
		InitParameterChr_enemy_00(pChr);

		// ���i(3D)�̃g�����X�t�H�[����������
		InitParts3DTransform(&pChr->partsInfo, &pType->partsSet);
	}
}

//========================================
// UninitChr_enemy_00�֐� - CHR:�G[00] �̏I������ -
//========================================
void UninitChr_enemy_00(void)
{
	for (int nCntType = 0; nCntType < CHR_ENEMY_00_TYPE_MAX; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// ���b�V���̔j��
			if (g_aMeshChr_enemy_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshChr_enemy_00[nCntType][nCntParts]->Release();
				g_aMeshChr_enemy_00[nCntType][nCntParts] = NULL;
			}

			// �}�e���A���̔j��
			if (g_aBuffMatChr_enemy_00[nCntType][nCntParts] != NULL)
			{
				g_aBuffMatChr_enemy_00[nCntType][nCntParts]->Release();
				g_aBuffMatChr_enemy_00[nCntType][nCntParts] = NULL;
			}
		}
	}
}

//========================================
// UpdateChr_enemy_00�֐� - CHR:�G[00] �̍X�V���� -
//========================================
void UpdateChr_enemy_00(void)
{
	Chr_enemy_00		*pChr		// CHR:�G[00] �̏��̃|�C���^
						= g_aChr_enemy_00;
	Chr_enemy_00Control *pChrCtl	// CHR:�G[00] �̊Ǘ����̃|�C���^
						= &g_chr_enemy_00Control;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse) 
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_enemy_00Type[pChr->nType];

		if (pChrCtl->state == CHR_ENEMY_00_CONTROL_STATE_BOSS_APPEAL) 
		{// �Ǘ���Ԃ��{�X�A�s�[���̎��A
			if (((nCntChr == pChrCtl->nSetBossIdx) || 
				((pChr->nParentIdx == pChrCtl->nSetBossIdx) && (pChr->nParentIdx != -1)) ||
				((g_aChr_enemy_00[pChr->nParentIdx].nParentIdx == pChrCtl->nSetBossIdx) && (pChr->nParentIdx != -1)))
				&&
				(GetMd_game_00()->state != MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET))
			{// �J�E���g���e�ԍ����e�̐e�ԍ����ݒ肵���{�X�̔ԍ��ƈ�v���Ă��� & MD:�Q�[�����[00] �̏�Ԃ��{�X�o�����[�r�[(���_���Z�b�g)�łȂ����A
				// ���[�V�����̍X�V����
				if (UpdateMotion3D(
					pChr->partsInfo.nMotion,
					g_aChr_enemy_00Type[pChr->nType].motionSet,
					&pChr->partsInfo,
					g_aChr_enemy_00Type[pChr->nType].partsSet))
				{// ���[�V���������[�v�ɒB�������A
					pChr->nMotion = pChr->nMotionTemp;	// ���[�V������ۑ��������̂ɐݒ�
					SetMotion3D(	// ���[�V�����ݒ�
						&pChr->partsInfo,
						pChr->nMotion);

					// MD:�Q�[�����[00] �̏�Ԃ��{�X�o�����[�r�[(���_���Z�b�g)�ɐݒ�
					SetMd_game_00State(MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET);
				}
			}

			// ���i(3D)�̃g�����X�t�H�[�����擾
			GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);
		}
		else if (pChrCtl->state != CHR_ENEMY_00_CONTROL_STATE_STATIC)
		{// (�Ǘ���Ԃ��{�X�A�s�[���łȂ�) & �Ǘ���Ԃ��ÓI�Ŗ������A
			if (pChr->state != CHR_ENEMY_00_STATE_NORMAL)
			{// ��Ԃ��ʏ�łȂ����A
				if (--pChr->nCounterState <= 0)
				{// ��ԃJ�E���^�[�����Z��������0�ȉ��̎��A
					pChr->state = CHR_ENEMY_00_STATE_NORMAL;	// ��Ԃ�ʏ�ɐݒ�
				}
			}

			// �s������
			ActChr_enemy_00(pChr);

			// �ʒu�X�V����
			UpdatePosChr_enemy_00(pChr);

			// �����𐧌�
			RotControl(&pChr->rot);
			
			// ���i(3D)�̃g�����X�t�H�[�����擾
			GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);

			// ���[�V�����̍X�V����
			if (UpdateMotion3D(
				pChr->partsInfo.nMotion,
				g_aChr_enemy_00Type[pChr->nType].motionSet,
				&pChr->partsInfo,
				g_aChr_enemy_00Type[pChr->nType].partsSet))
			{// ���[�V���������[�v�ɒB�������A
				Motion3D	*pMotion	// ���݂̃��[�V�����̏��̃|�C���^
							= &pType->motionSet.aMotion[pChr->nMotion];
				if (!pMotion->bLoop)
				{// ���݂̃��[�V�����̃��[�v�t���O���U�̎��A
					pChr->nMotion = pChr->nMotionTemp;	// ���[�V������ۑ��������̂ɐݒ�
					SetMotion3D( // ���[�V�����ݒ�
						&pChr->partsInfo,
						pChr->nMotion);
				}
			}

			if (pChr->fVibration >= CHR_ENEMY_00_VIBRATION_MIN)
			{// �U���̋������U���̋����̉����ȏ�̎��A
				if (--pChr->nCounterVib <= 0)
				{// �U���J�E���^�[�����Z��������0�ȉ��̎��A
					pChr->nCounterVib = CHR_ENEMY_00_ONCE_VIBRATION_TIME;	// �U���J�E���^�[��ݒ�
					float fAngle = -D3DX_PI + fRand(D3DX_PI * 2);			// �����𗐐��Őݒ�
					pChr->vibrationPos.x = sinf(fAngle) * pChr->fVibration;
					pChr->vibrationPos.z = cosf(fAngle) * pChr->fVibration;
					pChr->fVibration *= CHR_ENEMY_00_VIBRATION_DAMPING;	// �U���̋���������
				}
			}
			else
			{// �U���̋������U���̋����̉��������̎��A�U���ʒu��������
				pChr->vibrationPos = INITD3DXVECTOR3;
			}

			if (pChr->nCounterInvincible > 0)
			{// ���G�J�E���^�[��0�������Ă��鎞�A
				pChr->nCounterInvincible--;	// ���G�J�E���^�[�����Z����
			}
			if (pChr->nCounterDamage > 0)
			{// �_���[�W�J�E���^�[��0�������Ă��鎞�A
				pChr->nCounterDamage--;	// �_���[�W�J�E���^�[�����Z����
			}

			// �ߋ��̈ʒu/������ۑ�
			pChr->posOld = pChr->pos;
			pChr->rotOld = pChr->rot;

			if ((pChr->bBossDependency) && (!g_aChr_enemy_00[pChrCtl->nSetBossIdx].bUse)) 
			{// �{�X�ˑ��t���O���^ & �{�X���g�p����Ă��Ȃ���Ԃ̎��A
				DestroyChr_enemy_00(pChr);	// �j�󏈗�
			}
		}
		else 
		{// �Ǘ���Ԃ��ÓI�̎��A
			pChr->nCounterDamage = 0;	// �_���[�W�J�E���^�[������������
		}

		// EFF:�e[00] �̐ݒ菈��
		SetEff_shadow_00(pChr->pos, pType->fShadowRadius);
	}
}

//========================================
// DrawChr_enemy_00�֐� - CHR:�G[00] �̕`�揈�� -
//========================================
void DrawChr_enemy_00(void)
{
	Chr_enemy_00		*pChr				// CHR:�G[00] �̏��̃|�C���^
						= g_aChr_enemy_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX			mtxSelf;			// �{�̂̃}�g���b�N�X

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// �{�̂̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxSelf);

		// �{�̂̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pChr->rot.y, pChr->rot.x, pChr->rot.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);
		{
			D3DXVECTOR3 setPos = pChr->pos + pChr->vibrationPos;

			// �{�̂̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);
		}
		// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

		Chr_enemy_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_enemy_00Type[pChr->nType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			Parts3D		*pParts		// ���i(3D)�̏��̃|�C���^
						= &pChr->partsInfo.aParts[nCntParts];
			Parts3DType *pPartsType	// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
						= &pType->partsSet.aPartsType[nCntParts];
			D3DXMATRIX	mtxParent;	// �e�}�g���b�N�X

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aBuffMatChr_enemy_00[pChr->nType][nCntParts]->GetBufferPointer();

			// ���i�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMtxWorldChr_enemy_00[nCntParts]);

			// ���i�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldChr_enemy_00[nCntParts], &g_aMtxWorldChr_enemy_00[nCntParts], &mtxRot);

			// ���i�̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldChr_enemy_00[nCntParts], &g_aMtxWorldChr_enemy_00[nCntParts], &mtxTrans);

			if (pPartsType->nParent != -1)
			{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
				mtxParent = g_aMtxWorldChr_enemy_00[pPartsType->nParent];
			}
			else
			{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
				mtxParent = mtxSelf;
			}

			// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(
				&g_aMtxWorldChr_enemy_00[nCntParts],
				&g_aMtxWorldChr_enemy_00[nCntParts],
				&mtxParent);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldChr_enemy_00[nCntParts]);

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_enemy_00[pChr->nType][nCntParts]; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
				{
					255,
					(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_ENEMY_00_DAMAGE_TIME))),
					(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_ENEMY_00_DAMAGE_TIME)) * !(pChr->state == CHR_ENEMY_00_STATE_PARALYSIS)),
					255
				});

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aTextureChr_enemy_00[pChr->nType][nCntParts][nCntMat]);

				// �|���S��(�p�[�c)�̕`��
				g_aMeshChr_enemy_00[pChr->nType][nCntParts]->DrawSubset(nCntMat);
			}
		}
	}
}

//========================================
// SetChr_enemy_00�֐� - CHR:�G[00] �̐ݒ菈�� -
//========================================
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	Chr_enemy_00		*pChr	// CHR:�G[00] �̏��̃|�C���^
						= g_aChr_enemy_00;
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[nType];

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		pChr->pos =				// �ʒu����
		pChr->posOld = pos;		// �ߋ��̈ʒu��ݒ�
		pChr->rot =				// ��������
		pChr->rotOld = 			// �ߋ��̌�������
		pChr->moveRot = rot;	// �ړ���������
		pChr->nType = nType;	// ��ނ���
		pChr->nParentIdx = -1;	// �e�̔ԍ���-1�ɐݒ�

		// �ݒ莞�̃p�����[�^�[����������
		InitParameterSetChr_enemy_00(pChr);

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// SetBossChr_enemy_00�֐� - CHR:�G[00] �̃{�X�ݒ菈�� -
//========================================
void SetBossChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	Chr_enemy_00		*pChr		// CHR:�G[00] �̏��̃|�C���^
						= g_aChr_enemy_00;
	Chr_enemy_00Type	*pType		// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[nType];
	Chr_enemy_00Control *pChrCtl	// CHR:�G[00] �̊Ǘ����̃|�C���^
						= &g_chr_enemy_00Control;

	// �ݒ肵���{�X�̔ԍ���������
	pChrCtl->nSetBossIdx = -1;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		pChr->pos =				// �ʒu����
		pChr->posOld = pos;		// �ߋ��̈ʒu��ݒ�
		pChr->rot =				// ��������
		pChr->rotOld = 			// �ߋ��̌�������
		pChr->moveRot = rot;	// �ړ���������
		pChr->nType = nType;	// ��ނ���
		pChr->nParentIdx = -1;	// �e�̔ԍ���-1�ɐݒ�

		// �ݒ莞�̃p�����[�^�[����������
		InitParameterSetChr_enemy_00(pChr);

		// �ݒ肵���{�X�̔ԍ���ۑ�
		pChrCtl->nSetBossIdx = nCntChr;

		// �s���ǂݍ��ݏ���
		ActLoadChr_enemy_00(pChr);

		// ���݂̃��[�V�����ԍ���ۑ�
		pChr->nMotionTemp = pChr->nMotion;

		// ���[�V�������A�s�[���Őݒ�
		pChr->nMotion = CHR_ENEMY_00_APPEAL_MOTION;
		SetMotion3D(
			&pChr->partsInfo,
			pChr->nMotion);

		// �q�̃A�s�[�����[�V�����ݒ�
		ShildAppealMotionSetChr_enemy_00(pChr);

		// �J����(3D)�̐U����ݒ�
		SetCamera3DVibration(CHR_ENEMY_00_BOSS_SET_CAMERA3D_VIBRATION);

		// �ݒ�SE���Đ�
		PlaySound(CHR_ENEMY_00_BOSS_SET_SE);

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// CollisionChr_enemy_00�֐� - CHR:�G[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionChr_enemy_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if (pObjCollision->bHit)
	{// ������t���O���^�̎��A
		return -1;	// �������I������
	}

	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;
	int				nIndex	// CHR:�G[00] �̕Ԃ��̔ԍ�
					= -1;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_enemy_00Type[pChr->nType];

		// �Փ˔���ɕK�v�ȏ��
		CollisionCheck(
			vector,
			pObjCollision,
			myCollInfo,
			{
				&pChr->pos,
				pChr->pos,
				NULL,
				&pChr->rot,
				pChr->rot,
				pType->hitTestSet.aHitTest[pChr->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		if (pObjCollision->bHit)
		{// ������t���O���^�̎��A
			nIndex = nCntChr;	// �Ԃ��̔ԍ���ݒ�
			break;				// �J��Ԃ������𔲂���
		}
	}

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// �ԍ���Ԃ�
	return nIndex;
}

//========================================
// DamageChr_enemy_00�֐� - CHR:�G[00] �̃_���[�W���� -
//========================================
void DamageChr_enemy_00(int nIndex, int nDamage)
{
	Chr_enemy_00		*pChr	// CHR:�G[00] �̏��̃|�C���^
						= &g_aChr_enemy_00[nIndex];
	Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_enemy_00Type[pChr->nType];

	if (pChr->nCounterInvincible > 0) 
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		return;	// �������I������
	}

	nDamage *= fDamageRand();	// �����␳
	SetChr_enemy_00Vibration(	// �U����ݒ�
		pChr,
		((float)nDamage / (float)pChr->nHPMax) * CHR_ENEMY_00_DAMAGE_VIBRATION_RATE);
	pChr->nHP -= nDamage;		// HP����_���[�W�����Z
	pChr->nCounterDamage		// �_���[�W�J�E���^�[��ݒ�
		= CHR_ENEMY_00_DAMAGE_TIME;
	// �q�̃_���[�W�J�E���^�[�ݒ菈��
	ChildSetDamageCounterChr_enemy_00(pChr);
	SetUi_damage_00(			// UI:�_���[�W[00] �̐ݒ菈��
		pChr->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_ENEMY);

	PlaySound(pType->damageSE);	// �_���[�WSE���Đ�

	if (pChr->nHP <= 0) 
	{// HP��0�ȉ��̎��A
		DestroyChr_enemy_00(pChr);	// �j�󏈗�
	}
	else 
	{// HP��0�������Ă��鎞�A���G�J�E���^�[��ݒ�
		pChr->nCounterInvincible = CHR_ENEMY_00_INVINCIBLE_TIME;
	}
}

//========================================
// KnockBackChr_enemy_00�֐� - CHR:�G[00] �̃m�b�N�o�b�N���� -
//========================================
void KnockBackChr_enemy_00(int nIndex, D3DXVECTOR3 rot, float fForce)
{
	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= &g_aChr_enemy_00[nIndex];

	if (pChr->nParentIdx != -1) 
	{// �e�����̎��A�������I������
		return;
	}

	// �͂�������
	pChr->force.x += sinf(rot.y) * fForce;
	pChr->force.z += cosf(rot.y) * fForce;
}

//========================================
// GiveStateChr_enemy_00�֐� - CHR:�G[00] �̏�ԕt�^���� -
//========================================
void GiveStateChr_enemy_00(int nIndex, CHR_ENEMY_00_STATE state) 
{
	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= &g_aChr_enemy_00[nIndex];

	CHR_ENEMY_00_STATE stateTemp = pChr->state;	// ��Ԃ�ۑ�

	pChr->state = state;	// ��Ԃ�ݒ�

	switch (state)
	{
		//========== *** �ʏ� ***
	case CHR_ENEMY_00_STATE_NORMAL: {
		break;
	}
		//========== *** ��� ***
	case CHR_ENEMY_00_STATE_PARALYSIS: {
		pChr->nCounterState = CHR_ENEMY_00_PARALYSIS_TIME;	// ���Ԃ�ݒ�
		if (stateTemp != CHR_ENEMY_00_STATE_PARALYSIS) {
			PlaySound(CHR_ENEMY_00_PARALYSIS_SE);	// ���SE���Đ�
		}
		break;
	}
	}
}

//========================================
// SetChr_enemy_00ControlState�֐� - CHR:�G[00] �̊Ǘ���Ԑݒ菈�� -
//========================================
void SetChr_enemy_00ControlState(CHR_ENEMY_00_CONTROL_STATE state) 
{
	Chr_enemy_00Control *pChrCtl	// CHR:�G[00] �̊Ǘ����̃|�C���^
						= &g_chr_enemy_00Control;

	pChrCtl->state = state;	//	��Ԃ���
}

//========================================
// AllyChr_enemy_00Count�֐� - CHR:�G[00] �̖����J�E���g���� -
//========================================
int AllyChr_enemy_00Count(void) 
{
	int				nAllyCount	// �����J�E���g
					= 0;
	Chr_enemy_00	*pChr		// CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_enemy_00Type[pChr->nType];

		if ((pChr->bUse) && (pType->rank == CHR_ENEMY_00_RANK_ALLY))
		{// �g�p����Ă��� & �����N�������̎��A
			nAllyCount++;	// �����J�E���g�����Z
		}
	}

	return nAllyCount;
}