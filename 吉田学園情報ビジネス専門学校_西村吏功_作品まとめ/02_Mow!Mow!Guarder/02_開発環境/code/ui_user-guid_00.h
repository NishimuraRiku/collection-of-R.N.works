//========================================
// 
// UI:���[�U�[�K�C�h[00] �̃w�b�_�t�@�C��
// Author:���� ����
// 
//========================================
// *** ui_user-guid_00.h ***
//========================================
#ifndef _UI_USERGUID_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_USERGUID_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "input.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:���[�U�[�K�C�h[00] �̏��
typedef enum
{
	UI_USERGUID_00_STATE_POP,		// �o��
	UI_USERGUID_00_STATE_IN_POP,	// �o����
	UI_USERGUID_00_STATE_CLEAR,		// �����Ă���
	UI_USERGUID_00_STATE_IN_CLEAR,	// ������
	UI_USERGUID_00_STATE_SWITCH,	// �؂�ւ�
	UI_USERGUID_00_STATE_MAX,
}UI_USERGUID_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:���[�U�[�K�C�h[00] �̏��\����
typedef struct
{
	ACTIVE_INPUT_TYPE		type;			// ���
	UI_USERGUID_00_STATE	state;			// ���
	int						nCounterState;	// ��ԃJ�E���^�[
	float					fAlpha;			// �S�̂̓����x
	Scale					scale;			// �g��{��
	D3DXVECTOR3				pos;			// �ʒu
}Ui_userGuid_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���[�U�[�K�C�h[00] �̏����擾
Ui_userGuid_00 *GetUi_userGuid_00(void);

// UI:���[�U�[�K�C�h[00] �̏���������
// UI:���[�U�[�K�C�h[00] �̏I������
// UI:���[�U�[�K�C�h[00] �̍X�V����
// UI:���[�U�[�K�C�h[00] �̕`�揈��
void InitUi_userGuid_00(void);
void UninitUi_userGuid_00(void);
void UpdateUi_userGuid_00(void);
void DrawUi_userGuid_00(void);

// UI:���[�U�[�K�C�h[00] �̏�Ԑݒ菈��
// UI:���[�U�[�K�C�h[00] �̐ݒ菈��
void SetStateUi_userGuid_00(UI_USERGUID_00_STATE state);
void SetUi_userGuid_00(D3DXVECTOR3 pos, ACTIVE_INPUT_TYPE type);

#endif