//========================================
// 
// OBJ:�R�A[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_core_00.h ***
//========================================
#ifndef _OBJ_CORE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_CORE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"
#include "polygon3D.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�R�A[00] �̃��[�V�����ԍ�
typedef enum
{
	OBJ_CORE_00_MOTION_WAIT,	// �ҋ@
	OBJ_CORE_00_MOTION_MAX,
}OBJ_CORE_00_MOTION;

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:�R�A[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// ���݂̈ʒu
	D3DXVECTOR3 rot;	// ����
	
	// ���ފ֘A
	int	nType;	// ���

	// ��Ԋ֘A
	bool	bUse;				// �g�p����Ă��邩�t���O
	int		nHP;				// �q�b�g�|�C���g
	int		nHPMax;				// �q�b�g�|�C���g�̏��
	int		nCounterInvincible;	// ���G�J�E���^�[
	int		nCounterDamage;		// �_���[�W�J�E���^�[
	int		nCounterWarning;	// �x���J�E���^�[
	bool	bWarning;			// �x���t���O

	// ���i�֘A
	Parts3DInfo			partsInfo;	// ���i�Ǘ�
	OBJ_CORE_00_MOTION	motion;		// ���[�V����
}Obj_core_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�R�A[00] �̏����擾
Obj_core_00 *GetObj_core_00(void);

// OBJ:�R�A[00] �̏���������
// OBJ:�R�A[00] �̏I������
// OBJ:�R�A[00] �̍X�V����
// OBJ:�R�A[00] �̕`�揈��
void InitObj_core_00(void);
void UninitObj_core_00(void);
void UpdateObj_core_00(void);
void DrawObj_core_00(void);

// OBJ:�R�A[00] �̓ǂݍ��ݏ���
void LoadObj_core_00(void);

// OBJ:�R�A[00] �Ƃ̓����蔻�菈��
int CollisionObj_core_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:�R�A[00] �̃_���[�W����
// OBJ:�R�A[00] �̔j�󏈗�
void DamageObj_core_00(int nDamage);
void DestroyObj_core_00(void);

#endif