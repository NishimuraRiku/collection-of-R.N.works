//========================================
//  *** ui_bomb_get_00.h ***
//========================================
#ifndef _UI_BOMB_GET_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_BOMB_GET_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �\���̂̒�` 
//****************************************

// ���e�l��[00] �\���̂̒�`
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu

	// ��Ԋ֘A
	bool	bUse;			// �g�p����Ă��邩�t���O
	int		nLife;			// ����
	int		nCounterBrink;	// �_�ŃJ�E���^�[
	bool	bBrink;			// �_�Ńt���O
}Ui_bomb_get_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���e�l��[00] �̏����擾
Ui_bomb_get_00 *GetUi_bomb_get_00(void);

// ���e�l��[00] �̏���������
void InitUi_bomb_get_00(void);

// ���e�l��[00] �̏I������
void UninitUi_bomb_get_00(void);

// ���e�l��[00] �̍X�V����
void UpdateUi_bomb_get_00(void);

// ���e�l��[00] �̐ݒ菈��
void SetUi_bomb_get_00(D3DXVECTOR3 pos);

#endif