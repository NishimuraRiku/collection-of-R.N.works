//========================================
//  *** item_medal_00.h ***
//========================================
#ifndef _ITEM_MEDAL_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ITEM_MEDAL_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************

// ���_��[00] �̍ő吔
#define ITEM_MEDAL_00_MAX	(256)

//****************************************
// �\���̂̒�`
//****************************************

// ���_��[00] �\���̂̒�`
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
}Item_medal_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���_��[00] �̏���������
void InitItem_medal_00(void);

// ���_��[00] �̏I������
void UninitItem_medal_00(void);

// ���_��[00] �̍X�V����
void UpdateItem_medal_00(void);

// ���_��[00] �̕`�揈��
void DrawItem_medal_00(void);

// ���_��[00] �̐ݒ菈��
void SetItem_medal_00(D3DXVECTOR3 pos);

// ���_��[00] �̏����擾
Item_medal_00 *GetItem_medal_00(void);

#endif