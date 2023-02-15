//========================================
// *** obj_plasma_block_00.h ***
//========================================
#ifndef _OBJ_PLASMA_BLOCK_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_PLASMA_BLOCK_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
#define OBJ_PLASMA_BLOCK_00_MOVE_TIME	(60)	// �v���Y�}�u���b�N[00] �̈ړ��ɂ����鎞��

//****************************************
// �\���̂̒�`
//****************************************

// �v���Y�}�u���b�N[00] ���`���l�p�`�\���̂̒�`
typedef struct
{
	int nMoveCounter;		// �ړ��̃J�E���^�[

	float fTop;				// ��ӂ̍��W
	float fBottom;			// ���ӂ̍��W
	float fLeft;			// ���ӂ̍��W
	float fRight;			// �E�ӂ̍��W

	float fTargetTop;		// ��ӂ̖ڕW���W
	float fTargetBottom;	// ���ӂ̖ڕW���W
	float fTargetLeft;		// ���ӂ̖ڕW���W
	float fTargetRight;		// �E�ӂ̖ڕW���W

	float fMoveTop;			// ��ӂ̈ړ���
	float fMoveBottom;		// ���ӂ̈ړ���
	float fMoveLeft;		// ���ӂ̈ړ���
	float fMoveRight;		// �E�ӂ̈ړ���

	D3DXVECTOR3 centerPos;	// ���S�ʒu
}Obj_plasma_block_00_square;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �v���Y�}�u���b�N[00] �̏���������
void InitObj_plasma_block_00(void);

// �v���Y�}�u���b�N[00] �̏I������
void UninitObj_plasma_block_00(void);

// �v���Y�}�u���b�N[00] �̍X�V����
void UpdateObj_plasma_block_00(void);

// �v���Y�}�u���b�N[00] ���`���l�p�`�̍X�V����
void UpdateObj_plasma_block_00_square(void);

// �v���Y�}�u���b�N[00] �̕`�揈��
void DrawObj_plasma_block_00(void);

// �v���Y�}�u���b�N[00] ���`���l�p�`�̏����擾
Obj_plasma_block_00_square *GetObj_plasma_block_00_square(void);

// �v���Y�}�u���b�N[00] �̎l�p�`�̈ړ��ʂ�ݒ�
void SetMoveObj_plasma_block_00(void);

#endif