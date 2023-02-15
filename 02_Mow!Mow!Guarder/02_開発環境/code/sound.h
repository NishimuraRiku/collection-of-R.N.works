//========================================
// 
// �T�E���h�̃w�b�_�t�@�C��
// Author : AKIRA TANAKA
// |
// Arrange : RIKU NISHIMURA
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
	SOUND_TYPE_BGM,	// ���y
	SOUND_TYPE_SE,	// ���ʉ�
	SOUND_TYPE_MAX,
}SOUND_TYPE;

// �T�E���h�ꗗ
typedef enum
{
	SOUND_LABEL_BGM_STAGE_000,			// BGM:�X�e�[�W	[000]
	SOUND_LABEL_SE_DAMAGE_000,			// SE :�_���[�W	[000]
	SOUND_LABEL_SE_DAMAGE_001,			// SE :�_���[�W	[001]
	SOUND_LABEL_SE_BREAK_000,			// SE :�j��		[000]
	SOUND_LABEL_SE_SHOT_000,			// SE :�V���b�g	[000]
	SOUND_LABEL_SE_SHOT_001,			// SE :�V���b�g	[001]
	SOUND_LABEL_SE_SHOT_002,			// SE :�V���b�g	[002]
	SOUND_LABEL_SE_EXPLOSION_000,		// SE :����		[000]
	SOUND_LABEL_SE_EXPLOSION_001,		// SE :����		[001]
	SOUND_LABEL_SE_SELECT_000,			// SE :�I��		[001]
	SOUND_LABEL_SE_BUY_000,				// SE :�w��		[000]
	SOUND_LABEL_SE_SET_000,				// SE :�ݒu		[000]
	SOUND_LABEL_SE_GET_000,				// SE :�l��		[000]
	SOUND_LABEL_SE_EXPLOSION_002,		// SE :����		[002]
	SOUND_LABEL_SE_SLASH_000,			// SE :�a��		[000]
	SOUND_LABEL_SE_FOOTSTEP_000,		// SE :����		[000]
	SOUND_LABEL_SE_FOOTSTEP_001,		// SE :����		[001]
	SOUND_LABEL_SE_CRY_000,				// SE :����	[000]
	SOUND_LABEL_SE_CRY_001,				// SE :����	[001]
	SOUND_LABEL_SE_CRY_002,				// SE :����	[002]
	SOUND_LABEL_BGM_STAGE_001,			// BGM:�X�e�[�W	[001]
	SOUND_LABEL_SE_WARNING_000,			// SE :�x��		[000]
	SOUND_LABEL_BGM_STAGE_002,			// BGM:�X�e�[�W	[002]
	SOUND_LABEL_SE_CRY_003,				// SE :����	[003]
	SOUND_LABEL_SE_CRY_004,				// SE :����	[004]
	SOUND_LABEL_SE_DETERMINATION_000,	// SE :����		[000]
	SOUND_LABEL_SE_FOOTSTEP_002,		// SE :����		[002]
	SOUND_LABEL_SE_PUSH_000,			// SE :����		[000]
	SOUND_LABEL_SE_DETERMINATION_001,	// SE :����		[001]
	SOUND_LABEL_BGM_TITLE_000,			// BGM:�^�C�g��	[000]
	SOUND_LABEL_SE_SWITCH_000,			// SE :�؂�ւ�	[000]
	SOUND_LABEL_SE_PAUSE_000,			// SE :�|�[�Y	[000]
	SOUND_LABEL_SE_FOOTSTEP_003,		// SE :����		[003]
	SOUND_LABEL_SE_METAL_000,			// SE :����		[000]
	SOUND_LABEL_SE_IMPACT_000,			// SE :�Ռ�		[000]
	SOUND_LABEL_BGM_STAGE_003,			// BGM:�X�e�[�W	[003]
	SOUND_LABEL_BGM_STAGE_004,			// BGM:�X�e�[�W	[004]
	SOUND_LABEL_BGM_STAGE_005,			// BGM:�X�e�[�W	[005]
	SOUND_LABEL_BGM_CLEAR_000,			// BGM:�N���A	[000]
	SOUND_LABEL_SE_METAL_001,			// SE :����		[001]
	SOUND_LABEL_SE_ELEC_001,			// SE :�d�C		[000]
	SOUND_LABEL_SE_COUNT_000,			// SE :�J�E���g	[000]
	SOUND_LABEL_MAX,
}SOUND_LABEL;

//****************************************
// �\���̂̒�`
//****************************************

// �T�E���h���̍\���̒�`
typedef struct
{
	char		*pFilename;	// �t�@�C����
	int			nCntLoop;	// ���[�v�J�E���g
	float		fVolume;	// ����
	SOUND_TYPE	type;		// �T�E���h�̎��
} SOUNDINFO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �T�E���h�̏����擾
SOUNDINFO *GetSoundInfo(void);

// �T�E���h�̏���������
// �T�E���h�̏I������
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

// �Z�O�����g�Đ�(�Đ����Ȃ��~)
// �Z�O�����g��~(���x���w��)
// �Z�O�����g��~(�S��)
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

// �ݒ艹�ʂ�ݒ�
void SetSettingVolume(SOUND_TYPE type);
// ��ޖ��ɉ��ʕύX
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);
// ���ʕύX
void SoundVolumeChange(SOUND_LABEL label, float fVolume);
// ��ޖ��ɍĐ���~
void StopSoundType(SOUND_TYPE type);

#endif
