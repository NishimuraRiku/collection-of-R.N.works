//========================================
// 
// OBJ:�Ŕ�[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_signboard_00.h ***
//========================================
#ifndef _OBJ_SIGNBOARD_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_SIGNBOARD_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�Ŕ�[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3	pos;	// �ʒu
	D3DXVECTOR3	rot;	// ����

	// ���ފ֘A
	int nType;	// ���

	// ��Ԋ֘A
	bool bUse;	// �g�p����Ă��邩�t���O

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
}Obj_signboard_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�Ŕ�[00] �̏��擾����
Obj_signboard_00 *GetObj_signboard_00(void);
// OBJ:�Ŕ�[00] �̏���������
void InitObj_signboard_00(void);
// OBJ:�Ŕ�[00] �̏I������
void UninitObj_signboard_00(void);
// OBJ:�Ŕ�[00] �̍X�V����
void UpdateObj_signboard_00(void);
// OBJ:�Ŕ�[00] �̕`�揈��
void DrawObj_signboard_00(void);
// OBJ:�Ŕ�[00] �̓ǂݍ��ݏ���
void LoadObj_signboard_00(void);
// OBJ:�Ŕ�[00] �̐ݒ菈��
void SetObj_signboard_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
// OBJ:�Ŕ�[00] �Ƃ̓����蔻�菈��
int CollisionObj_signboard_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

#endif