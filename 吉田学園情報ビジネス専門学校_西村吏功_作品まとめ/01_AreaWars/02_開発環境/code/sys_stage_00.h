//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SYS_STAGE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _SYS_STAGE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
#define ENEMY_SET_MAX	(256)	// �G�̔z�u���̍ő�L����

//****************************************
// �\���̂̒�`
//****************************************

// �G�̔z�u���\����
typedef struct 
{
	int			nTime;		// �z�u���鎞��
	int			nType;		// �z�u������
	D3DXVECTOR3 setPos;		// �z�u����ʒu
	D3DXVECTOR3 setRot;		// �z�u����p�x
	bool		bAreaBoss;	// �G���A�{�X�t���O
	bool		bBomb;		// ���e�����t���O
	int			nParentNum;	// �e�ԍ�
	int			nChildNum;	// �q���ԍ�
}EnemySet;

// �X�e�[�W�\����
typedef struct
{
	int			nStageNum;					// �X�e�[�W�ԍ�
	int			nWaveMax;					// �E�F�[�u�̍ő吔
	int			nWaveCount;					// �E�F�[�u�̃J�E���g
	int			nDispWaveCount;				// �\������E�F�[�u�̃J�E���g
	int			nWaveTime;					// �E�F�[�u�̎���
	int			nWaveEndTime;				// �E�F�[�u�̏I������
	bool		bWaveUp;					// �E�F�[�u���𑝂₷���ۂ�
	bool		bWarning;					// �x���t���O
	bool		bSavePoint;					// �Z�[�u�n�_�t���O
	bool		bShowAreaTime;				// �G���A���Ԃ̕\���t���O
	int			nLastWave;					// �ŏI�E�F�[�u
	int			nLastDispWave;				// �ŏI�\���E�F�[�u
	int			nLastScore;					// �ŏI�X�R�A
	int			nLastBomb;					// �ŏI�{���ێ���
	int			nEnemySetNum;				// �G�̔z�u��
	EnemySet	aEnemySet[ENEMY_SET_MAX];	// �G�̔z�u���
	SOUND_LABEL	sound;						// �T�E���h�ԍ�
	SOUND_LABEL soundNow;					// ���ݍĐ����̃T�E���h�ԍ�
}Sys_stage_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �V�X�e��:�X�e�[�W[00] �̏���������
void InitSys_stage_00(int nStageNum);

// �V�X�e��:�X�e�[�W[00] �̏I������
void UninitSys_stage_00(void);

// �V�X�e��:�X�e�[�W[00] �̍X�V����
void UpdateSys_stage_00(void);

// �V�X�e��:�X�e�[�W[00] �̏����擾
Sys_stage_00 *GetSys_stage_00(void);

// �V�X�e��:�X�e�[�W[00] �̃E�F�[�u�̐��ڏ���
void WaveTrendSys_stage_00(void);

// �V�X�e��:�X�e�[�W[00] �̃R���e�B�j���[����
void ContinueSys_stage_00(void);

#endif