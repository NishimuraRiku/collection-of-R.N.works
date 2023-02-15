//========================================
// 
// UI:�����L���O�t���[��[00] �̃w�b�_�t�@�C��
// Author:���� ����
// 
//========================================
// *** ui_ranking-frame_00.h ***
//========================================
#ifndef _UI_RANKINGFRAME_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_RANKINGFRAME_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:�����L���O�t���[��[00] �̏��
typedef enum
{
	UI_RANKINGFRAME_00_STATE_POP,		// �o��
	UI_RANKINGFRAME_00_STATE_IN_POP,	// �o����
	UI_RANKINGFRAME_00_STATE_CLEAR,		// �����Ă���
	UI_RANKINGFRAME_00_STATE_IN_CLEAR,	// ������
	UI_RANKINGFRAME_00_STATE_TEXT_SET,	// �����L���O�Z�b�g
	UI_RANKINGFRAME_00_STATE_MAX,
}UI_RANKINGFRAME_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// UI:�����L���O�t���[��[00] �̏��\����
typedef struct
{
	UI_RANKINGFRAME_00_STATE	state;			// ���
	int							nCounterState;	// ��ԃJ�E���^�[
	float						fAlpha;			// �S�̂̓����x
	D3DXVECTOR3					pos;			// �ʒu
	Scale						scale;			// �g��{��
	bool						bNameEntry;		// ���O����
	int							nCountName;		// ���O�J�E���g
	int							nCountChar;		// �����J�E���g
	int							nUpdateRank;	// �X�V����
	int							nCounterBlink;	// �_�ŃJ�E���^�[
}Ui_rankingFrame_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�����L���O�t���[��[00] �̏����擾
Ui_rankingFrame_00 *GetUi_rankingFrame_00(void);

// UI:�����L���O�t���[��[00] �̏���������
// UI:�����L���O�t���[��[00] �̏I������
// UI:�����L���O�t���[��[00] �̍X�V����
// UI:�����L���O�t���[��[00] �̕`�揈��
void InitUi_rankingFrame_00(void);
void UninitUi_rankingFrame_00(void);
void UpdateUi_rankingFrame_00(void);
void DrawUi_rankingFrame_00(void);

// UI:�����L���O�t���[��[00] �̏�Ԑݒ菈��
// UI:�����L���O�t���[��[00] �̐ݒ菈��
void SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE state);
void SetUi_rankingFrame_00(D3DXVECTOR3 pos);

// UI:�����L���O�t���[��[00] �̖��O���͐ݒ菈��
void SetNameEntryUi_rankingFrame_00(int nUpdateRank);

#endif