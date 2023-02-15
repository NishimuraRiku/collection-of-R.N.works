//========================================
//  *** chr_fighter_00.h ***
//========================================
#ifndef _CHR_FIGHTER_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHR_FIGHTER_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"
#include "atk_explosion_00.h"

//****************************************
// �}�N����`
//****************************************

// �@��[00] �̎�ނ̍ő吔
// �@��[00] �̔��e�ő及����
#define CHR_FIGHTER_00_TYPE_MAX	(80)
#define CHR_FIGHTER_00_BOMB_MAX	(2)

//****************************************
// �񋓌^�̒�`
//****************************************

// �@��[00] �̏��
typedef enum
{
	CHR_FIGHTER_00_STATE_NORMAL = 0,	// �ʏ�
	CHR_FIGHTER_00_STATE_IN_POP,		// �o����
	CHR_FIGHTER_00_STATE_DAMAGE,		// �_���[�W
	CHR_FIGHTER_00_STATE_INVINCIBLE,	// ���G
	CHR_FIGHTER_00_STATE_MAX
}CHR_FIGHTER_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// �@��[00]�\���̂̒�`
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3	pos;			// �ʒu
	D3DXVECTOR3	move;			// �ړ���
	float		fMove;			// �ړ���
	D3DXVECTOR3	rot;			// ����
	D3DXVECTOR3	targetRot;		// �ڕW����
	float		fWidth;			// ��
	float		fHeight;		// ����
	float		fHitTestWidth;	// �����蔻��̕�
	float		fHitTestHeight;	// �����蔻��̍���

	// ���ޏ��
	int		nType;	// ���
	bool	bUse;	// �g�p����Ă��邩�t���O
	
	// ��Ԓl
	int		nLife;						// �̗�
	int		nBomb;						// ���e������
	bool	aState						//
			[CHR_FIGHTER_00_STATE_MAX];	// ��ԃt���O
	int		aCounterState				//
			[CHR_FIGHTER_00_STATE_MAX];	// ��ԊǗ��J�E���^�[
	int		nScore;						// �X�R�A
	int		nPattern;					// �e�N�X�`���̃p�^�[��No.
	int		nCounterAnim;				// �A�j���[�V�����̃J�E���^�[
	int		nShotCounter;				// �e�̔��˃J�E���^�[
	int		nShotTime;					// �e�̔��˂ɂ����鎞��
	int		nInvincibleCounter;			// ���G�J�E���^�[
	int		nCntDamage;					// ��e�J�E���g
}Chr_fighter_00;

// �@��[00] �̎�ޖ��̏��
typedef struct
{
	char			aName			//
					[TXT_MAX];		// ����
	char			aTexturPath		//
					[TXT_MAX];		// �e�N�X�`���̑��΃p�X
	int				nPtnMaxX;		// �p�^�[���̍ő吔(��)
	int				nPtnMaxY;		// �p�^�[���̍ő吔(�c)
	int				nFillStep;		// �ϐF�����̃p�^�[���̒i��
	float			fWidth;			// ��
	float			fHeight;		// ����
	float			fHitTestWidth;	// �����蔻��̕�
	float			fHitTestHeight;	// �����蔻��̍���
	HIT_TEST_TYPE	hitTestType;	// �����蔻��̎��
	int				nLife;			// �̗�
	float			fMove;			// �ړ���
	int				nShotTime;		// �e�̔��˂ɂ����鎞��
}Chr_fighter_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �@��[00] �̏���������
void InitChr_fighter_00(void);

// �@��[00] �̏I������
void UninitChr_fighter_00(void);

// �@��[00] �̍X�V����
void UpdateChr_fighter_00(void);

// �@��[00] �̕`�揈��
void DrawChr_fighter_00(void);

// �@��[00] �̓ǂݍ��ݏ���
void LoadChr_fighter_00(void);

// �@��[00] �̏����擾
Chr_fighter_00 *GetChr_fighter_00(void);

// �@��[00] �̎�ޖ��̏����擾
Chr_fighter_00Type *GetChr_fighter_00Type(void);

// �@��[00] �̐ݒ菈��
void SetChr_fighter_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

// �@��[00] �̔j�󏈗�
void BreakChr_fighter_00(void);

// �@��[00] �̃q�b�g����
void HitChr_fighter_00(int nDamage);

#endif