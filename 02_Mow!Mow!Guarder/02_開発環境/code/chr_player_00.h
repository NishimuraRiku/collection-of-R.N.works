//========================================
// 
// CHR:�v���C���[[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_player_00.h ***
//========================================
#ifndef _CHR_PLAYER_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHR_PLAYER_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"
#include "polygon3D.h"
#include "sound.h"

//****************************************
// �}�N����`
//****************************************
// CHR:�v���C���[[00] �̎a���̓��p
// CHR:�v���C���[[00] �̎a���ɂ����鎞��
#define CHR_PLAYER_00_SLASH_INSIDE	(D3DX_PI * 1.5f)
#define CHR_PLAYER_00_SLASH_TIME	(15)

// OBJ:�v���C���[[00] �̌��̍U���̓A�b�v�J�E���g���̏㏸�l
// OBJ:�v���C���[[00] �̒e�̍U���̓A�b�v�J�E���g���̏㏸�l
#define CHR_PLAYER_00_SWORD_ATK_ADDRATE		(0.25f)
#define CHR_PLAYER_00_BULLET_ATK_ADDRATE	(0.1f)

// CHR;�v���C���[[00] ��HP�����\������
// CHR;�v���C���[[00] ��HP�����e�L�X�g�̊g��{��
#define CHR_PLAYER_00_HPRATE_DISPLAY_DISTANCE	(80.0f)
#define CHR_PLAYER_00_HPRATE_TEXT_SCALE			(0.5f)

//****************************************
// �񋓌^�̒�`
//****************************************
// CHR:�v���C���[[00] �̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum
{
	CHR_PLAYER_00_HITOBJ_ITM_COIN_00,		// ITM:�R�C��	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_BLOCK_00,		// OBJ:�u���b�N	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_CORE_00,		// OBJ:�R�A		[00]
	CHR_PLAYER_00_HITOBJ_OBJ_MIRROR_00,		// OBJ:�~���[	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_PEDESTAL_00,	// OBJ:���		[00]
	CHR_PLAYER_00_HITOBJ_OBJ_STAGE_00,		// OBJ:�X�e�[�W	[00]
	CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00,		// OBJ:�X�C�b�`	[00]
	CHR_PLAYER_00_HITOBJ_MAX,
}CHR_PLAYER_00_HITOBJ;

// CHR:�v���C���[[00] �̐ݒ�ʒu�̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum 
{
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_BLOCK_00,		// OBJ:�u���b�N	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_DISCHARGER_00,	// OBJ:���d���u	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_MIRROR_00,		// OBJ:�~���[	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_OBJ_TURRET_00,		// OBJ:�^���b�g	[00]
	CHR_PLAYER_00_SETPOS_HITOBJ_MAX,
}CHR_PLAYER_00_SETPOS_HITOBJ;

// CHR:�v���C���[[00] �̃A�C�e���ԍ�
typedef enum
{
	CHR_PLAYER_00_ITEM_SWORD,		// ��
	CHR_PLAYER_00_ITEM_GUN,			// �e
	CHR_PLAYER_00_ITEM_BLOCK,		// �u���b�N
	CHR_PLAYER_00_ITEM_TURRET,		// �^���b�g
	CHR_PLAYER_00_ITEM_REFLECTOR,	// �V�[���h
	CHR_PLAYER_00_ITEM_DISCHARGER,	// ���d���u
	CHR_PLAYER_00_ITEM_MAX,
}CHR_PLAYER_00_ITEM;

// CHR:�v���C���[[00] �̏��
typedef enum
{
	CHR_PLAYER_00_STATE_NORMAL,	// �ʏ�
	CHR_PLAYER_00_STATE_DOWN,	// �_�E��
	CHR_PLAYER_00_STATE_MAX,
}CHR_PLAYER_00_STATE;

// CHR:�v���C���[[00] �̊Ǘ����
typedef enum 
{
	CHR_PLAYER_00_CONTROL_STATE_DYNAMIC,	// ���I
	CHR_PLAYER_00_CONTROL_STATE_STATIC,		// �ÓI
	CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP,	// ���͒�~
	CHR_PLAYER_00_CONTROL_STATE_MAX,
}CHR_PLAYER_00_CONTROL_STATE;

// CHR:�v���C���[[00] �̃��[�V�����ԍ�
typedef enum
{
	CHR_PLAYER_00_MOTION_WAIT,	// �ҋ@
	CHR_PLAYER_00_MOTION_WALK,	// ���s
	CHR_PLAYER_00_MOTION_SET,	// �ݒ�
	CHR_PLAYER_00_MOTION_SHOT,	// ����
	CHR_PLAYER_00_MOTION_SLASH,	// �a��
	CHR_PLAYER_00_MOTION_DOWN,	// �_�E��
	CHR_PLAYER_00_MOTION_MAX,
}CHR_PLAYER_00_MOTION;

//****************************************
// �\���̂̒�`
//****************************************
// CHR:�v���C���[[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;			// ���݂̈ʒu
	D3DXVECTOR3 posOld;			// �ߋ��̈ʒu(1�t���[���O)
	D3DXVECTOR3 force;			// ��
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 moveRot;		// �ړ�����
	D3DXVECTOR3 targetRot;		// �ڕW����
	D3DXVECTOR3 setPos;			// �ݒu�ʒu
	D3DXVECTOR3 relativePosR;	// �����_�̑��Έʒu
	D3DXVECTOR3 targetRltvPosR;	// �ڕW�̒����_�̑��Έʒu
	
	// ���ފ֘A
	int	nType;	// ���

	// ��Ԋ֘A
	CHR_PLAYER_00_STATE	state;				// ���
	int					nCounterState;		// ��ԃJ�E���^�[
	int					nHP;				// �q�b�g�|�C���g
	int					nHPMax;				// �q�b�g�|�C���g�̏��
	int					nCoin;				// �R�C����
	int					nScore;				// �X�R�A
	int					nSelectItem;		// �I���A�C�e���ԍ�
	bool				bWalk;				// ���s�t���O
	bool				bRotation;			// �����]���t���O
	bool				bSlash;				// �a���t���O
	int					nCounterAttack;		// �U���J�E���^�[
	int					nCounterInvincible;	// ���G�J�E���^�[
	int					nCounterDamage;		// �_���[�W�J�E���^�[
	int					aItemCount			// �A�C�e���J�E���g
						[CHR_PLAYER_00_ITEM_MAX];
	int					nPedestalIndex;		// ����̔ԍ�
	int					nCoinIndex;			// �R�C���̔ԍ�
	int					nBulletAtkUpCount;	// �e�̍U���̓A�b�v�J�E���g
	int					nSwordAtkUpCount;	// ���̍U���̓A�b�v�J�E���g
	int					nRespawnTime;		// �����ɂ����鎞��

	// �^���֘A
	HitTestInfo hitTestInfo;		// �����蔻��̊Ǘ����
	Collision	cmnCollision; 		// ���ʂ̏Փˏ��
	Collision	aObjCollision		// �I�u�W�F�N�g���̏Փˏ��
				[CHR_PLAYER_00_HITOBJ_MAX];
	Collision	setPosCmnCollision; // �ݒ�ʒu�̋��ʂ̏Փˏ��

	// ���i�֘A
	Parts3DInfo				partsInfo;		// ���i�Ǘ�
	CHR_PLAYER_00_MOTION	motion;			// ���[�V����
}Chr_player_00;

// CHR:�v���C���[[00] �̊Ǘ����\����
typedef struct
{
	CHR_PLAYER_00_CONTROL_STATE state;	// ���
}Chr_player_00Control;

// CHR:�v���C���[[00] �̎�ޖ��̏��\����
typedef struct
{
	int			nHP;			// �q�b�g�|�C���g
	float		fMoveForce;		// �ړ���
	int			nSlashDamage;	// �a���_���[�W
	int			nBulletType;	// �e�̎��
	int			nShotTime;		// ���ˊԊu
	float		fShadowRadius;	// �e�̔��a
	SOUND_LABEL	damageSE;		// �_���[�WSE
	HitTestSet	hitTestSet;		// �����蔻��ݒ���
	Parts3DSet	partsSet;		// ���i�ݒ���(3D)
	Motion3DSet motionSet;		// ���[�V�����ݒ���
}Chr_player_00Type;

// CHR:�v���C���[[00] �̃A�C�e�����̏��\����
typedef struct
{
	bool	bSet;	// �ݒu�t���O
	bool	bAim;	// �_���t���O
	bool	bCount;	// �J�E���g�t���O
	char	aName	// ����
			[TXT_MAX];
}Chr_player_00Item;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// CHR:�v���C���[[00] �̏����擾
// CHR:�v���C���[[00] �̊Ǘ������擾
// CHR:�v���C���[[00] �̎�ޖ��̏����擾
// CHR:�v���C���[[00] �̃A�C�e���̏����擾
Chr_player_00 *GetChr_player_00(void);
Chr_player_00Control *GetChr_player_00Control(void);
Chr_player_00Type *GetChr_player_00Type(void);
const Chr_player_00Item *GetChr_player_00Item(void);

// CHR:�v���C���[[00] �̏���������
// CHR:�v���C���[[00] �̏I������
// CHR:�v���C���[[00] �̍X�V����
// CHR:�v���C���[[00] �̕`�揈��
void InitChr_player_00(void);
void UninitChr_player_00(void);
void UpdateChr_player_00(void);
void DrawChr_player_00(void);

// CHR:�v���C���[[00] �̓ǂݍ��ݏ���
void LoadChr_player_00(void);

// CHR:�v���C���[[00] �Ƃ̓����蔻��
int CollisionChr_player_00(VECTOR vector, Collision *pChrCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// CHR:�v���C���[[00] �̃_���[�W����
// CHR:�v���C���[[00] �̉񕜏���
// CHR:�v���C���[[00] �̃m�b�N�o�b�N����
void DamageChr_player_00(int nDamage);
void HealChr_player_00(int nHeal);
void KnockBackChr_player_00(D3DXVECTOR3 rot, float fForce);

// CHR:�v���C���[[00] �̊Ǘ���Ԑݒ菈��
void SetChr_player_00ControlState(CHR_PLAYER_00_CONTROL_STATE state);

#if _DEBUG_COMMENT
// CHR:�v���C���[[00] �̃R�����g�`�揈��
void DrawCommentChr_player_00(void);
#endif

#endif