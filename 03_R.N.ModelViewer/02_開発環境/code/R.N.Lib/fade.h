//========================================
// 
// �t�F�[�h�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** fade.h ***
//========================================
#ifndef _FADE_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _FADE_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// _R.N.Lib
#include "RNmain.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// �t�F�[�h�̏��
typedef enum 
{
	FADE_NONE = 0,	// �������Ă��Ȃ����
	FADE_IN,		// �t�F�[�h�C�����
	FADE_OUT,		// �t�F�[�h�A�E�g���
	FADE_MAX,
}FADE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �t�F�[�h�̏���������
void InitFade(MODE modeNext);
// �t�F�[�h�̏I������
void UninitFade(void);
// �t�F�[�h�̍X�V����
void UpdateFade(void);
// �t�F�[�h�̕`�揈��
void DrawFade(void);
//========== *** �擾 ***
// �t�F�[�h�̐؂�ւ��擾
bool GetFadeSwap(void);
// �t�F�[�h�C���I���擾
bool GetFadeInEnd(void);
// �t�F�[�h�̏�Ԃ��擾
FADE GetFade(void);
//========== *** �ݒ� ***
// �t�F�[�h�̐ݒ菈��
void SetFade(MODE modeNext);

#endif