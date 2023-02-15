//========================================
// 
// OBJ:�X�C�b�`[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_switch_00.h ***
//========================================
#ifndef _OBJ_SWITCH_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_SWITCH_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"
#include "polygon3D.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�X�C�b�`[00] �̃��[�V�����ԍ�
typedef enum
{
	OBJ_SWITCH_00_MOTION_WAIT,	// �ҋ@
	OBJ_SWITCH_00_MOTION_PUSH,	// ����
	OBJ_SWITCH_00_MOTION_MAX,
}OBJ_SWITCH_00_MOTION;

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:�X�C�b�`[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����

	// ���ފ֘A
	int	nType;	// ���

	// ���i�֘A
	Parts3DInfo				partsInfo;	// ���i�Ǘ�
	OBJ_SWITCH_00_MOTION	motion;		// ���[�V����
}Obj_switch_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�X�C�b�`[00] �̏����擾
Obj_switch_00 *GetObj_switch_00(void);

// OBJ:�X�C�b�`[00] �̏���������
// OBJ:�X�C�b�`[00] �̏I������
// OBJ:�X�C�b�`[00] �̍X�V����
// OBJ:�X�C�b�`[00] �̕`�揈��
void InitObj_switch_00(void);
void UninitObj_switch_00(void);
void UpdateObj_switch_00(void);
void DrawObj_switch_00(void);

// OBJ:�X�C�b�`[00] �̓ǂݍ��ݏ���
void LoadObj_switch_00(void);

// OBJ:�X�C�b�`[00] �Ƃ̓����蔻�菈��
int CollisionObj_switch_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:�X�C�b�`[00] �̉�������
void PushObj_switch_00(void);

#endif