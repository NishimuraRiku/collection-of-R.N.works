//========================================
//  *** item_bomb_00.h ***
//========================================
#ifndef _ITEM_BOMB_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ITEM_BOMB_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************

// ���e[00] �̍ő吔
#define ITEM_BOMB_00_MAX	(256)

//****************************************
// �\���̂̒�`
//****************************************

// ���e[00] �\���̂̒�`
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu

	// ��Ԓl
	bool	bUse;			// �g�p����Ă��邩�t���O
	int		nLife;			// ����
	int		nCounterBrink;	// �_�ł̃J�E���^�[
	bool	bBrink;			// �_�Ńt���O

	// �e�N�X�`���֘A
	int nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
	int nPattern;		// �p�^�[��No.
}Item_bomb_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���e[00] �̏���������
void InitItem_bomb_00(void);

// ���e[00] �̏I������
void UninitItem_bomb_00(void);

// ���e[00] �̍X�V����
void UpdateItem_bomb_00(void);

// ���e[00] �̕`�揈��
void DrawItem_bomb_00(void);

// ���e[00] �̐ݒ菈��
void SetItem_bomb_00(D3DXVECTOR3 pos);

// ���e[00] �̏����擾
Item_bomb_00 *GetItem_bomb_00(void);

#endif