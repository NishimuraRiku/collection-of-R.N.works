//========================================
// 
// �ݒ�̏����̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** setting.h ***
//========================================
#ifndef _SETTING_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _SETTING_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
// ���ʂ̒i�K
#define VOLUME_STAGE	(20)

//****************************************
// �\���̂̒�`
//****************************************
// �ݒ�̏��\����
typedef struct
{
	int nBGMVolume;		// BGM����
	int nSEVolume;		// SE����
	bool bVibration;	// �U���t���O
	bool bFullScreen;	// �S��ʃt���O
}Setting;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �ݒ�̏����擾
Setting *GetSetting(void);

// �ݒ蒆�t���O���擾
bool *GetSettingNow(void);

// �ݒ�̏���������
// �ݒ�̏I������
// �ݒ�̍X�V����
void InitSetting(void);
void UninitSetting(void);
void UpdateSetting(void);

// �ݒ胁�j���[�̍X�V����
void UpdateSettingMenu(void);

// �ݒ胁�j���[�̐ݒ菈��
void SetSettingMenu(void);

// �ݒ�̓ǂݍ��ݏ���
// �ݒ�̏������ݏ���
void LoadSetting(void);
void SaveSetting(void);

#endif