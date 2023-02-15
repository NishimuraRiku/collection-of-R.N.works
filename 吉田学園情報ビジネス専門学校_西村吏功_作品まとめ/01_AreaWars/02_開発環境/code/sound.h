//========================================
// 
// �T�E���h�����̃w�b�_�t�@�C��
// Author : AKIRA TANAKA
// |
// Arrange : ���� ����
// 
//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SOUND_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _SOUND_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************

// �T�E���h�̎��
typedef enum 
{
	SOUND_TYPE_BGM = 0,	// ���y
	SOUND_TYPE_SE,		// ���ʉ�
	SOUND_TYPE_MAX,
}SOUND_TYPE;

// �T�E���h�ꗗ
typedef enum
{
	SOUND_LABEL_BGM_TITLE_000 = 0,	// �^�C�g��BGM			[000]
	SOUND_LABEL_BGM_STAGE_000,		// �X�e�[�WBGM			[000]
	SOUND_LABEL_BGM_STAGE_001,		// �X�e�[�WBGM			[001]
	SOUND_LABEL_BGM_BOSS_000,		// �{�XBGM				[000]
	SOUND_LABEL_BGM_BOSS_001,		// �{�XBGM				[001]
	SOUND_LABEL_SE_GAME_CLEAR_000,	// �Q�[���N���ASE		[000]
	SOUND_LABEL_SE_GAME_OVER_000,	// �Q�[���I�[�o�[SE		[000]
	SOUND_LABEL_SE_SELECT_000,		// �I��SE				[000]
	SOUND_LABEL_SE_SELECT_001,		// �I��SE				[001]
	SOUND_LABEL_SE_DECITION_000,	// ����SE				[000]
	SOUND_LABEL_SE_DECITION_001,	// ����SE				[001]
	SOUND_LABEL_SE_DELETION_000,	// �폜SE				[000]
	SOUND_LABEL_SE_PAUSE_START_000,	// �|�[�Y�J�nSE			[000]
	SOUND_LABEL_SE_PAUSE_END_000,	// �|�[�Y�I��SE			[000]
	SOUND_LABEL_SE_COUNT_DOWN_000,	// �J�E���g�_�E��SE		[000]
	SOUND_LABEL_SE_SCORE_COUNT_000,	// �X�R�A�J�E���gSE		[000]
	SOUND_LABEL_SE_TEXT_COUNT_000,	// �e�L�X�g�J�E���gSE	[000]
	SOUND_LABEL_SE_RANKING_SET_000,	// �����L���O�Z�b�gSE	[000]
	SOUND_LABEL_SE_DAMAGE_000,		// �_���[�WSE			[000]
	SOUND_LABEL_SE_DAMAGE_001,		// �_���[�WSE			[001]
	SOUND_LABEL_SE_BONUS_000,		// �{�[�i�XSE			[000]
	SOUND_LABEL_SE_WARNING_000,		// �x��SE				[000]
	SOUND_LABEL_SE_SHOT_000,		// �V���b�gSE			[000]
	SOUND_LABEL_SE_SHOT_001,		// �V���b�gSE			[001]
	SOUND_LABEL_SE_SHOT_002,		// �V���b�gSE			[002]
	SOUND_LABEL_SE_SHOT_003,		// �V���b�gSE			[003]
	SOUND_LABEL_SE_SHOT_004,		// �V���b�gSE			[004]
	SOUND_LABEL_SE_EXPLOSION_000,	// ����SE				[000]
	SOUND_LABEL_SE_EXPLOSION_001,	// ����SE				[001]
	SOUND_LABEL_SE_EXPLOSION_002,	// ����SE				[002]
	SOUND_LABEL_SE_EXPLOSION_003,	// ����SE				[003]
	SOUND_LABEL_SE_EXPLOSION_004,	// ����SE				[004]
	SOUND_LABEL_SE_EXPLOSION_005,	// ����SE				[005]
	SOUND_LABEL_SE_DIFFUSION_000,	// �g�USE				[000]
	SOUND_LABEL_SE_LOCK_ON_000,		// ���b�N�I��SE			[000]
	SOUND_LABEL_SE_OPEN_000,		// �J��SE				[000]
	SOUND_LABEL_SE_METAL_000,		// ����SE				[000]
	SOUND_LABEL_SE_COLLISION_000,	// �Փ�SE				[000]
	SOUND_LABEL_SE_SORTIE_000,		// �o��SE				[000]
	SOUND_LABEL_SE_LANDING_000,		// ���nSE				[000]
	SOUND_LABEL_SE_GET_000,			// �l��SE				[000]
	SOUND_LABEL_MAX
}SOUND_LABEL;

//****************************************
// �\���̂̒�`
//****************************************

// �T�E���h���̍\���̒�`
typedef struct
{
	char		*pFilename;	// �t�@�C����
	char		*pLoadName;	// �ǂݍ��ݖ�
	int			nCntLoop;	// ���[�v�J�E���g
	float		fVolume;	// ����
	SOUND_TYPE	type;		// �T�E���h�̎��
} SOUNDINFO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

// �T�E���h�̏����擾
SOUNDINFO *GetSoundInfo(void);

// �T�E���h�̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadSound(char aString[TXT_MAX], SOUND_LABEL *pLabel);

// �T�E���h�̏���������
HRESULT InitSound(HWND hWnd);

// �T�E���h�̏I������
void UninitSound(void);

// �Z�O�����g�Đ�(�Đ����Ȃ��~)
HRESULT PlaySound(SOUND_LABEL label);

// �Z�O�����g��~(���x���w��)
void StopSound(SOUND_LABEL label);

// �Z�O�����g��~(�S��)
void StopSound(void);

// ��ޖ��ɉ��ʕύX
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);

// �ݒ艹�ʂ�ݒ�
void SetSettingVolume(SOUND_TYPE type);

// ��ޖ��ɍĐ���~
void StopSoundType(SOUND_TYPE type);

#endif
