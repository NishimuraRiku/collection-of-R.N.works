//========================================
// 
// OBJ:�X�e�[�W[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_stage_00.h ***
//========================================
#ifndef _OBJ_STAGE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_STAGE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "obj_pedestal_00.h"	// OBJ:���	[00]

//****************************************
// �}�N����`
//****************************************
// OBJ:�X�e�[�W[00] �̃Q�[�g�̍ő吔
// OBJ:�X�e�[�W[00] �̑���̍ő吔
#define OBJ_STAGE_00_GATE_MAX		(4)
#define OBJ_STAGE_00_PEDESTAL_MAX	(8)

// OBJ:�X�e�[�W[00] �̃E�F�[�u���̍ő吔
// OBJ:�X�e�[�W[00] �̓G�ݒ���̍ő吔
// OBJ:�X�e�[�W[00] �̕i�������̍ő吔
#define OBJ_STAGE_00_WAVE_MAX		(64)
#define OBJ_STAGE_00_ENEMYSET_MAX	(64)
#define OBJ_STAGE_00_ITEMS_MAX		(8)

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�X�e�[�W[00] �̏��
typedef enum 
{
	OBJ_STAGE_00_STATE_NONE,		// ����
	OBJ_STAGE_00_STATE_INTERVAL,	// �C���^�[�o��
	OBJ_STAGE_00_STATE_RAID,		// �P��
	OBJ_STAGE_00_STATE_CLEAR,		// �N���A
	OBJ_STAGE_00_STATE_STOP,		// ��~
}OBJ_STAGE_00_STATE;

// OBJ:�X�e�[�W[00] �̃e�L�X�g�̏��
typedef enum
{
	OBJ_STAGE_00_TEXT_STATE_HIDE,		// �����Ă���
	OBJ_STAGE_00_TEXT_STATE_APPEAR,		// ���ꒆ
	OBJ_STAGE_00_TEXT_STATE_DISAPPEAR,	// ������
	OBJ_STAGE_00_TEXT_STATE_MAX,
}OBJ_STAGE_00_TEXT_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:�X�e�[�W[00] �̃{�X�ݒ���
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
	int			nType;	// ���
}Obj_stage_00SetBoss;

// OBJ:�X�e�[�W[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����

	// ���ޏ��
	int	nType;	// ���

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����

	// ��Ԋ֘A
	int						nCounter;				// �J�E���^�[
	OBJ_STAGE_00_STATE		state;					// ���
	int						nWave;					// �E�F�[�u�ԍ�
	int						nWaveOld;				// �ߋ��̃E�F�[�u�ԍ�
	int						nCntWaveMain;			// �E�F�[�u�J�E���g(���C��)
	int						nCntWaveSub;			// �E�F�[�u�J�E���g(�T�u)
	int						nKill;					// �L����
	OBJ_STAGE_00_TEXT_STATE	textState;				// �e�L�X�g�̏��
	int						nCounterText;			// �e�L�X�g�J�E���^�[
	int						nCounterDiscoloration;	// �ϐF�J�E���^�[
	Color					groundCol;				// �n�ʂ̐F
	Obj_stage_00SetBoss		setBoss;				// �{�X�ݒ���
	int						nTimeBonus;				// �^�C���{�[�i�X
	int						nCounterTimeBonus;		// �^�C���{�[�i�X�J�E���^�[
	int						nCounterTimeBonusText;	// �^�C���{�[�i�X�e�L�X�g�J�E���^�[

	// ���i�֘A
	Parts3DInfo	partsInfo;	// ���i�Ǘ�
}Obj_stage_00;

// OBJ:�X�e�[�W[00] �̃Q�[�g���
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 bossPoint;	// �{�X�ݒ�ʒu
	D3DXVECTOR3 setRot;		// �ݒ����
	float		fWidth;		// ��
	float		fDepth;		// ���s��
}Obj_stage_00Gate;

// OBJ:�X�e�[�W[00] �̃Q�[�g�ݒ���
typedef struct
{
	int					nGateNum;	// �Q�[�g��
	Obj_stage_00Gate	aGate		// �Q�[�g���
						[OBJ_STAGE_00_GATE_MAX];
}Obj_stage_00GateSet;

// OBJ:�X�e�[�W[00] �̒��Ӄ}�[�N���
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
}Obj_stage_00AttentionMark;

// OBJ:�X�e�[�W[00] �̒��Ӄ}�[�N�ݒ���
typedef struct
{
	Obj_stage_00AttentionMark	aAttentionMark	// ���Ӄ}�[�N���
								[OBJ_STAGE_00_GATE_MAX];
}Obj_stage_00AttentionMarkSet;

// OBJ:�X�e�[�W[00] �̑�����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	int			nType;	// ���
}Obj_stage_00Pedestal;

// OBJ:�X�e�[�W[00] �̑���ݒ���
typedef struct
{
	int						nPedestalNum;	// �����
	Obj_stage_00Pedestal	aPedestal		// ������
							[OBJ_STAGE_00_PEDESTAL_MAX];
}Obj_stage_00PedestalSet;

// OBJ:�X�e�[�W[00] �̓G�ݒ���
typedef struct
{
	int nTime;	// ����
	int	nGate;	// �Q�[�g�ԍ�
	int nType;	// ���
}Obj_stage_00EnemySet;

// OBJ:�X�e�[�W[00] �̃E�F�[�u���
typedef struct
{
	int						nIntervalTime;			// �C���^�[�o���^�C��
	int						nEnemyNum;				// �G�̐�
	Obj_stage_00EnemySet	aEnemySet				// �G�ݒ���
							[OBJ_STAGE_00_ENEMYSET_MAX];
	SOUND_LABEL				intervalBGM;			// �C���^�[�o��BGM
	SOUND_LABEL				raidBGM;				// �P��BGM
	Color					intervalGroundColor;	// �C���^�[�o�����̒n�ʂ̐F
	Color					intervalSkyColor;		// �C���^�[�o�����̋�̐F
	Color					raidGroundColor;		// �P�����̒n�ʂ̐F
	Color					raidSkyColor;			// �P�����̋�̐F
	bool					bWarningText;			// ���Ӄe�L�X�g�t���O
	bool					bMainCount;				// ���C���J�E���g�t���O
}Obj_stage_00Wave;

// OBJ:�X�e�[�W[00] �̃E�F�[�u�ݒ���
typedef struct
{
	int					nLoopPoint;	// ���[�v�n�_
	int					nWaveNum;	// �E�F�[�u��
	Obj_stage_00Wave	aWave		// �E�F�[�u���
						[OBJ_STAGE_00_WAVE_MAX];
}Obj_stage_00WaveSet;

// OBJ:�X�e�[�W[00] �̕i�������
typedef struct
{
	OBJ_PEDESTAL_00_ITEM	aItem	// ���i�ԍ�
							[OBJ_STAGE_00_PEDESTAL_MAX];
}Obj_stage_00Items;

// OBJ:�X�e�[�W[00] �̕i�����ݒ���
typedef struct
{
	int					nItemsNum;	// �i������
	Obj_stage_00Items	aItems		// �i�������
						[OBJ_STAGE_00_ITEMS_MAX];
}Obj_stage_00ItemsSet;

// OBJ:�X�e�[�W[00] �̎�ޖ��̏��\����
typedef struct
{
	D3DXVECTOR3						coreSetPos;			// �R�A�ݒ�ʒu
	D3DXVECTOR3						switchSetPos;		// �X�C�b�`�ݒ�ʒu
	Obj_stage_00GateSet				gateSet;			// �Q�[�g�ݒ���
	Obj_stage_00AttentionMarkSet	attentionMarkSet;	// ���Ӄ}�[�N�ݒ���
	Obj_stage_00PedestalSet			pedestalSet;		// ����ݒ���
	Obj_stage_00ItemsSet			itemsSet;			// �i�����ݒ菈��
	Obj_stage_00WaveSet				waveSet;			// �E�F�[�u�ݒ���
	HitTestSet						hitTestSet;			// �����蔻��ݒ���
	Parts3DSet						partsSet;			// ���i�ݒ���(3D)
}Obj_stage_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�X�e�[�W[00] �̏����擾
// OBJ:�X�e�[�W[00] �̎�ޖ��̏����擾
Obj_stage_00 *GetObj_stage_00(void);
Obj_stage_00Type *GetObj_stage_00Type(void);

// OBJ:�X�e�[�W[00] �̏���������
// OBJ:�X�e�[�W[00] �̏I������
// OBJ:�X�e�[�W[00] �̍X�V����
// OBJ:�X�e�[�W[00] �̕`�揈��
void InitObj_stage_00(void);
void UninitObj_stage_00(void);
void UpdateObj_stage_00(void);
void DrawObj_stage_00(void);

// OBJ:�X�e�[�W[00] �̓ǂݍ��ݏ���
void LoadObj_stage_00(void);

// OBJ:�X�e�[�W[00] �Ƃ̓����蔻�菈��
// OBJ:�X�e�[�W[00] �Ƃ̓����蔻�菈��(With�ݒ�ʒu)
void CollisionObj_stage_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);
void CollisionObj_stage_00WithSetPos(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:�X�e�[�W[00] �̏�Ԑݒ菈��
void SetStateObj_stage_00(OBJ_STAGE_00_STATE state);

// OBJ:�X�e�[�W[00] �̃{�X�ݒ菈��
void BossSetObj_stage_00(void);

#endif