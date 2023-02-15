//========================================
//  *** ui_score_00.h ***
//========================================
#ifndef _UI_SCORE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_SCORE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
#define UI_SCORE_00_MAX	(32)	// �X�R�A[00] �̍ő吔

//****************************************
// �\���̂̒�` 
//****************************************

// �X�R�A[00]�\���̂̒�`
typedef struct
{
						//========== *** �ʒu�֘A ***
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// ����
	float fWidth;		// ��
	float fHeight;		// ����
						//========== *** ���ޏ�� ***
	bool bUse;			// �g�p����Ă��邩�t���O
						//========== *** ��Ԓl ***
	int nLife;			// ����
	int nScore;			// �X�R�A��
	int nCounterBrink;	// �_�ŃJ�E���^�[
	bool bBrink;		// �_�Ńt���O
}Ui_score_00;

//========== �v���g�^�C�v�錾	==========// 
Ui_score_00 *GetUi_score_00(void);					// �X�R�A[00] �̏����擾
void InitUi_score_00(void);							// �X�R�A[00] �̏���������
void UninitUi_score_00(void);						// �X�R�A[00] �̏I������
void UpdateUi_score_00(void);						// �X�R�A[00] �̍X�V����
void SetUi_score_00(D3DXVECTOR3 pos, int nScore);	// �X�R�A[00] �̐ݒ菈��

#endif