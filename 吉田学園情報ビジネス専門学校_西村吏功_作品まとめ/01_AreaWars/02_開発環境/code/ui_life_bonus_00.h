//========================================
//  *** ui_life_bonus_00.h ***
//========================================
#ifndef _UI_LIFE_BONUS_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_LIFE_BONUS_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �\���̂̒�` 
//****************************************

// ���C�t�{�[�i�X[00] �\���̂̒�`
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu

	// ��Ԋ֘A
	int		nLifeBonus;		// �\�����郉�C�t�{�[�i�X
	bool	bUse;			// �g�p����Ă��邩�t���O
	int		nLife;			// ����
	int		nCounterBrink;	// �_�ŃJ�E���^�[
	bool	bBrink;			// �_�Ńt���O
}Ui_life_bonus_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���C�t�{�[�i�X[00] �̏����擾
Ui_life_bonus_00 *GetUi_life_bonus_00(void);

// ���C�t�{�[�i�X[00] �̏���������
void InitUi_life_bonus_00(void);

// ���C�t�{�[�i�X[00] �̏I������
void UninitUi_life_bonus_00(void);

// ���C�t�{�[�i�X[00] �̍X�V����
void UpdateUi_life_bonus_00(void);

// ���C�t�{�[�i�X[00] �̐ݒ菈��
void SetUi_life_bonus_00(int nLifeBonus);

#endif