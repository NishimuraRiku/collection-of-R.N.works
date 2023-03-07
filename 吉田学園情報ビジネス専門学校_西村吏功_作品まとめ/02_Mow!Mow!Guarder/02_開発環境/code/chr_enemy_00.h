//========================================
// 
// CHR:�G[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_enemy_00.h ***
//========================================
#ifndef _CHR_ENEMY_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHR_ENEMY_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"
#include "polygon3D.h"
#include "sound.h"

//****************************************
// �}�N����`
//****************************************
// CHR:�G[00] �̍ő吔
#define CHR_ENEMY_00_MAX	(64)

// CHR:�G[00] �̍s���p�^�[���̍ő吔
// CHR:�G[00] �̍s�����̍ő�L����
// CHR:�G[00] �̍s���̎�ޖ��̓ǂݍ��߂�ėp�l�̍ő吔
#define CHR_ENEMY_00_ACTPTN_MAX			(8)
#define CHR_ENEMY_00_ACT_MEMORY_MAX		(128)
#define CHR_ENEMY_00_COMMAND_DATA_MAX	(32)

// CHR:�G[00] �̃E�F�[�u�J�E���g���̃_���[�W�̏㏸�{��
#define CHR_ENEMY_00_WAVE_DAMAGE_ADDRATE	(0.05f)

//****************************************
// �񋓌^�̒�`
//****************************************
// CHR:�G[00] �̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum
{
	CHR_ENEMY_00_HITOBJ_CHR_PLAYER_00,		// CHR:�v���C���[	[00]
	CHR_ENEMY_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:�u���b�N		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_CORE_00,		// OBJ:�R�A			[00]
	CHR_ENEMY_00_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:���d���u		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:�~���[		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_TURRET_00,		// OBJ:�^���b�g		[00]
	CHR_ENEMY_00_HITOBJ_OBJ_STAGE_00,		// OBJ:�X�e�[�W		[00]
	CHR_ENEMY_00_HITOBJ_MAX,
}CHR_ENEMY_00_HITOBJ;

// CHR:�G[00] �̏��
typedef enum
{
	CHR_ENEMY_00_STATE_NORMAL,		// �ʏ�
	CHR_ENEMY_00_STATE_PARALYSIS,	// ���
	CHR_ENEMY_00_STATE_MAX,
}CHR_ENEMY_00_STATE;

// CHR:�G[00] �̊Ǘ����
typedef enum
{
	CHR_ENEMY_00_CONTROL_STATE_DYNAMIC,		// ���I
	CHR_ENEMY_00_CONTROL_STATE_STATIC,		// �ÓI
	CHR_ENEMY_00_CONTROL_STATE_BOSS_APPEAL,	// �{�X�A�s�[��
	CHR_ENEMY_00_CONTROL_STATE_MAX,
}CHR_ENEMY_00_CONTROL_STATE;

// CHR:�G[00] �̃R�}���h
typedef enum
{
	CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_START,			// �Ȑ܎��ړ��J�n
	CHR_ENEMY_00_COMMAND_TORTUOUS_MOVE_END,				// �Ȑ܎��ړ��I��
	CHR_ENEMY_00_COMMAND_TURN_MOVE_START,				// �����]�����ړ��J�n
	CHR_ENEMY_00_COMMAND_TURN_MOVE_END,					// �����]�����ړ��I��
	CHR_ENEMY_00_COMMAND_FORCE_MOVE_START,				// ���i���ړ��J�n
	CHR_ENEMY_00_COMMAND_FORCE_MOVE_END,				// ���i���ړ��I��
	CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_START,		// �U������ʒu�ɐU������J�n
	CHR_ENEMY_00_COMMAND_TURN_TO_TURN_POS_END,			// �U������ʒu�ɐU������I��
	CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_START,		// �ړ������ɐU������J�n
	CHR_ENEMY_00_COMMAND_TURN_TO_MOVE_ROT_END,			// �ړ������ɐU������I��
	CHR_ENEMY_00_COMMAND_Z_LOCK,						// Z���W�����b�N
	CHR_ENEMY_00_COMMAND_Z_UNLOCK,						// Z���W���A�����b�N
	CHR_ENEMY_00_COMMAND_CHILD_SET,						// �q�ݒ�
	CHR_ENEMY_00_COMMAND_CHILD_DEPENDENCY,				// �q�ˑ�
	CHR_ENEMY_00_COMMAND_SPIN_START,					// ��]�J�n
	CHR_ENEMY_00_COMMAND_SPIN_END,						// ��]�I��
	CHR_ENEMY_00_COMMAND_SHOT,							// �e����
	CHR_ENEMY_00_COMMAND_PLAY_SOUND,					// �T�E���h�Đ�
	CHR_ENEMY_00_COMMAND_MOTION_SET,					// ���[�V�����ݒ�
	CHR_ENEMY_00_COMMAND_AUTO_SHOT_START,				// �����V���b�g�J�n
	CHR_ENEMY_00_COMMAND_AUTO_SHOT_END,					// �����V���b�g�I��
	CHR_ENEMY_00_COMMAND_UNIT_IGNORED_START,			// ���j�b�g�����J�n
	CHR_ENEMY_00_COMMAND_UNIT_IGNORED_END,				// ���j�b�g�����I��
	CHR_ENEMY_00_COMMAND_MOVE_START,					// �ړ��J�n
	CHR_ENEMY_00_COMMAND_MOVE_END,						// �ړ��I��
	CHR_ENEMY_00_COMMAND_ALLY_SET,						// �����ݒ�
	CHR_ENEMY_00_COMMAND_ATK_IMPACT,					// ATK:�Ռ�
	CHR_ENEMY_00_COMMAND_EFF_SHOCKWAVE_00,				// EFF:�Ռ��g[00]
	CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_CHANGE,		// �Փ˃_���[�W�ύX
	CHR_ENEMY_00_COMMAND_COLLISION_DAMAGE_UNDO,			// �Փ˃_���[�W�����ɖ߂�
	CHR_ENEMY_00_COMMAND_BOSS_DEPENDENCY,				// �{�X�ˑ�
	CHR_ENEMY_00_COMMAND_ALLY_SET_ABSOLUTE,				// �����ݒ�(��Έʒu)
	CHR_ENEMY_00_COMMAND_RECOGNITION_OFF,				// �F��OFF
	CHR_ENEMY_00_COMMAND_TELEPORTATION,					// �u�Ԉړ�
	CHR_ENEMY_00_COMMAND_REFLECTION_START,				// ���ˊJ�n
	CHR_ENEMY_00_COMMAND_REFLECTION_END,				// ���ˏI��
	CHR_ENEMY_00_COMMAND_CHANGE_ACT_BY_HP,				// �c��HP�ɉ����čs���p�^�[����ύX
	CHR_ENEMY_00_COMMAND_RELEASE_PARENT,				// �e����
	CHR_ENEMY_00_COMMAND_CHANGE_POS,					// ���W�ύX
	CHR_ENEMY_00_COMMAND_DESTRUCTION,					// ���Ȕj��
	CHR_ENEMY_00_COMMAND_COUNT_SET,						// �J�E���g�ݒ�
	CHR_ENEMY_00_COMMAND_PARENT_COUNT_UPDATE_ON_DEATH,	// ���񂾎��ɐe�̃J�E���g�l��ϓ�������
	CHR_ENEMY_00_COMMAND_CHANGE_ACT_BY_COUNT,			// �J�E���g�ɉ����čs���p�^�[����ύX
	CHR_ENEMY_00_COMMAND_EFF_EXPLOSION_00,				// EFF:����[00]
	CHR_ENEMY_00_COMMAND_MAX,
}CHR_ENEMY_00_COMMAND;

// CHR:�G[00] �̍s��
typedef enum
{
	CHR_ENEMY_00_ACT_TORTUOUS_MOVE,		// �Ȑ܎��ړ�
	CHR_ENEMY_00_ACT_TURN_MOVE,			// �����]�����ړ�
	CHR_ENEMY_00_ACT_FORCE_MOVE,		// ���i���ړ�
	CHR_ENEMY_00_ACT_TURN_TO_TURN_POS,	// �U������ʒu�ɐU�����
	CHR_ENEMY_00_ACT_TURN_TO_MOVE_ROT,	// �ړ������ɐU�����
	CHR_ENEMY_00_ACT_Z_LOCK,			// Z���W���b�N
	CHR_ENEMY_00_ACT_CHILD_DEPENDENCY,	// �q�ˑ�
	CHR_ENEMY_00_ACT_SPIN,				// ��]
	CHR_ENEMY_00_ACT_MOVE,				// �ړ�
	CHR_ENEMY_00_ACT_MAX,
}CHR_ENEMY_00_ACT;

// CHR:�G[00] �̃����N
typedef enum
{
	CHR_ENEMY_00_RANK_LOWER,	// ���
	CHR_ENEMY_00_RANK_BOSS,		// �{�X
	CHR_ENEMY_00_RANK_ALLY,		// ����
	CHR_ENEMY_00_RANK_MAX
}CHR_ENEMY_00_RANK;

//****************************************
// �\���̂̒�`
//****************************************
// CHR:�G[00] �̍s���Ǘ����\����
typedef struct 
{
	int			nActPtn;		// �s���p�^�[���ԍ�
	bool		aAct			// �s���t���O
				[CHR_ENEMY_00_ACT_MAX];
	int			nCounterAct		// �s���J�E���^�[
				= -1;
	int			nCounterMove;	// �ړ��J�E���^�[
	float		fRotation;		// ��]��
	float		fRandPosRadius;	// �����ʒu���a
	D3DXVECTOR3 spin;			// ��]��
	D3DXVECTOR3 move;			// �ړ���
}Chr_enemy_00ActInfo;

// CHR:�G[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 posOld;		// �ߋ��̈ʒu(1�t���[���O)
	D3DXVECTOR3 force;		// ��
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ���݂̌���
	D3DXVECTOR3 rotOld;		// �ߋ��̈ʒu(1�t���[���O)
	D3DXVECTOR3 moveRot;	// �ړ�����
	D3DXVECTOR3 *pTurnPos;	// �U�������̈ʒu�̃|�C���^

	// ���ޏ��
	int nType;	// ���

	// ��Ԋ֘A
	bool				bUse;				// �g�p����Ă��邩�t���O
	int					nHP;				// �q�b�g�|�C���g
	int					nHPMax;				// �q�b�g�|�C���g�̏��
	CHR_ENEMY_00_STATE	state;				// ���
	int					nCounterState;		// ��ԃJ�E���^�[
	int					nCounterShot;		// �V���b�g�J�E���^�[
	int					nCounterCollAtk;	// �ՓˍU���J�E���^�[
	int					nCounterInvincible;	// ���G�J�E���^�[
	int					nCounterDamage;		// �_���[�W�J�E���^�[
	int					nAttackIndex;		// �U������I�u�W�F�N�g�̔ԍ�
	CHR_ENEMY_00_HITOBJ	attackObj;			// �U������I�u�W�F�N�g�̕���
	float				fVibration;			// �U���̋���
	int					nCounterVib;		// �U���J�E���^�[
	D3DXVECTOR3			vibrationPos;		// �U���ʒu
	int					nParentIdx;			// �e�̔ԍ�
	int					nParentIdxOld;		// �ߋ��̐e�̔ԍ�
	bool				bAutoShot;			// �����V���b�g�t���O
	int					nBulletType;		// �e�̎��
	int					nShotTimeMin;		// ���ˊԊu�̉���
	int					nShotTimeMax;		// ���ˊԊu�̏��
	bool				bUnitIgnored;		// ���j�b�g�����t���O
	bool				bCollDamageChange;	// �Փ˃_���[�W�ύX�t���O
	int					nNewCollDamage;		// �V�K�Փ˃_���[�W
	bool				bBossDependency;	// �{�X�ˑ��t���O
	bool				bRecognitionOff;	// �F��OFF�t���O
	bool				bReflection;		// ���˃t���O
	int					nChangeActHP;		// �s���p�^�[����ύX����HP
	int					nCount;				// �J�E���g
	int			nParentCountUpdateOnDeath;	// ���񂾎��̐e�̃J�E���g�l��ϓ��l
	int					nChangeActCount;	// �s���p�^�[����ύX����J�E���g
	int					nChangeAct;			// �ύX����s���p�^�[��

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
	Collision	cmnCollision; 	// ���ʂ̏Փˏ��
	Collision	aObjCollision	// �I�u�W�F�N�g���̏Փˏ��
				[CHR_ENEMY_00_HITOBJ_MAX];

	// AI�֘A
	Chr_enemy_00ActInfo actInfo;	// �s���Ǘ����

	// ���i�֘A
	Parts3DInfo	partsInfo;		// ���i�Ǘ�
	int			nMotion;		// ���[�V����
	int			nMotionTemp;	// ���[�V�����ꎞ�ۑ�
}Chr_enemy_00;

// CHR:�G[00] �̍s���̎�ޖ��̏��\����
typedef struct
{
	char	aLabelName	// ���x����
			[TXT_MAX];
	int		nDataNum;	// �l�̐�
}Chr_enemy_00ActType;

// CHR:�G[00] �̍s�����\����
typedef struct
{
	int						nTime;	// �s�����鎞��
	CHR_ENEMY_00_COMMAND	cmd;	// �R�}���h
	float					aData	// �ėp�l
							[CHR_ENEMY_00_COMMAND_DATA_MAX];
}Chr_enemy_00Act;

// CHR:�G[00] �̍s���p�^�[�����\����
typedef struct
{
	int				nLoopTime;	// ���[�v����
	int				nActNum;	// �s����
	Chr_enemy_00Act	aAct		// �s�����
					[CHR_ENEMY_00_ACT_MEMORY_MAX];
}Chr_enemy_00ActPtn;

// CHR:�G[00] �̍s���p�^�[���ݒ���\����
typedef struct
{
	Chr_enemy_00ActPtn	aActPtn	// �s���p�^�[�����
						[CHR_ENEMY_00_ACTPTN_MAX];
}Chr_enemy_00ActPtnSet;

// CHR:�G[00] �̎�ޖ��̏��\����
typedef struct
{
	int						nHP;					// HP
	float					fMoveForce;				// �ړ���
	int						nCollisionDamage;		// �Փ˃_���[�W
	int						nCollAtkTime;			// �ՓˍU���̊Ԋu
	int						nExplosionType;			// �����̎��
	float					fShadowRadius;			// �e�̔��a
	float					fPlayerSearchDistance;	// �v���C���[���G����
	float					fPlayerAimProbability;	// �v���C���[��_���m��
	float					fCoreAimProbability;	// �R�A��_���m��
	int						nDropCoin;				// �h���b�v����R�C����
	int						nScore;					// �X�R�A
	CHR_ENEMY_00_RANK		rank;					// �����N
	SOUND_LABEL				damageSE;				// �_���[�WSE
	Chr_enemy_00ActPtnSet	actPtnSet;				// �s���p�^�[���ݒ���
	HitTestSet				hitTestSet;				// �����蔻��ݒ���
	Parts3DSet				partsSet;				// ���i�ݒ���(3D)
	Motion3DSet				motionSet;				// ���[�V�����ݒ���
}Chr_enemy_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// CHR:�G[00] �̏����擾
Chr_enemy_00 *GetChr_enemy_00(void);
// CHR:�G[00] �̎�ޖ��̏����擾
Chr_enemy_00Type *GetChr_enemy_00Type(void);

// CHR:�G[00] �̏���������
void InitChr_enemy_00(void);
// CHR:�G[00] �̏I������
void UninitChr_enemy_00(void);
// CHR:�G[00] �̍X�V����
void UpdateChr_enemy_00(void);
// CHR:�G[00] �̕`�揈��
void DrawChr_enemy_00(void);

// CHR:�G[00] �̓ǂݍ��ݏ���
void LoadChr_enemy_00(void);
// CHR:�G[00] �̐ݒ菈��
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
// CHR:�G[00] �̃{�X�ݒ菈��
void SetBossChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
// CHR:�G[00] �Ƃ̓����蔻�菈��
int CollisionChr_enemy_00(VECTOR vector, Collision *pChrCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);
// CHR:�G[00] �̃_���[�W����
void DamageChr_enemy_00(int nIndex, int nDamage);
// CHR:�G[00] �̃m�b�N�o�b�N����
void KnockBackChr_enemy_00(int nIndex, D3DXVECTOR3 rot, float fForce);
// CHR:�G[00] �̏�ԕt�^����
void GiveStateChr_enemy_00(int nIndex, CHR_ENEMY_00_STATE state);
// CHR:�G[00] �̊Ǘ���Ԑݒ菈��
void SetChr_enemy_00ControlState(CHR_ENEMY_00_CONTROL_STATE state);
// CHR:�G[00] �̖����J�E���g����
int AllyChr_enemy_00Count(void);

#endif