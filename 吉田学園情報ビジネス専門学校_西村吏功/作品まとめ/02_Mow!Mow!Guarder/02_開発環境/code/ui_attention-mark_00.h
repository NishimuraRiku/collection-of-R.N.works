//========================================
// 
// UI:���Ӄ}�[�N[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_attention-mark_00.h ***
//========================================
#ifndef _UI_ATTENTIONMARK_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_ATTENTIONMARK_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �\���̂̒�`
//****************************************
// UI:���Ӄ}�[�N[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����

	// ��Ԋ֘A
	bool	bUse;			// �g�p����Ă��邩�t���O
	bool	bShow;			// ���t���O
}Ui_attentionMark_00;

// UI:���Ӄ}�[�N[00] �̊Ǘ����\����
typedef struct 
{
	int	nCounterBrink;		// �_�ŃJ�E���^�[
	int	nCounterArrowMove;	// ���ړ��J�E���^�[
}Ui_attentionMark_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���Ӄ}�[�N[00] �̏����擾
// UI:���Ӄ}�[�N[00] �̊Ǘ������擾
Ui_attentionMark_00 *GetUi_attentionMark_00(void);
Ui_attentionMark_00Control *GetUi_attentionMark_00Control(void);

// UI:���Ӄ}�[�N[00] �̏���������
// UI:���Ӄ}�[�N[00] �̏I������
// UI:���Ӄ}�[�N[00] �̍X�V����
// UI:���Ӄ}�[�N[00] �̕`�揈��
void InitUi_attentionMark_00(void);
void UninitUi_attentionMark_00(void);
void UpdateUi_attentionMark_00(void);
void DrawUi_attentionMark_00(void);

// UI:���Ӄ}�[�N[00] �̐ݒ菈��
void SetUi_attentionMark_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif