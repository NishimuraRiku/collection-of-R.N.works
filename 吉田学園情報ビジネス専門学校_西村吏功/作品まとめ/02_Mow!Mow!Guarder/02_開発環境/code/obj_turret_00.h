//========================================
// 
// OBJ:�^���b�g[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_turret_00.h ***
//========================================
#ifndef _OBJ_TURRET_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_TURRET_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�^���b�g[00] �̊Ǘ����
typedef enum
{
	OBJ_TURRET_00_CONTROL_STATE_DYNAMIC,	// ���I
	OBJ_TURRET_00_CONTROL_STATE_STATIC,		// �ÓI
	OBJ_TURRET_00_CONTROL_STATE_MAX,
}OBJ_TURRET_00_CONTROL_STATE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�^���b�g[00] �̏���������
// OBJ:�^���b�g[00] �̏I������
// OBJ:�^���b�g[00] �̍X�V����
// OBJ:�^���b�g[00] �̕`�揈��
void InitObj_turret_00(void);
void UninitObj_turret_00(void);
void UpdateObj_turret_00(void);
void DrawObj_turret_00(void);

// OBJ:�^���b�g[00] �̓ǂݍ��ݏ���
void LoadObj_turret_00(void);

// OBJ:�^���b�g[00] �̐ݒ菈��
void SetObj_turret_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

// OBJ:�^���b�g[00] �Ƃ̓����蔻�菈��
int CollisionObj_turret_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:�^���b�g[00] �̃_���[�W����
// OBJ:�^���b�g[00] �̑S�ĉ񕜏���
void DamageObj_turret_00(int nIndex, int nDamage);
void HealAllObj_turret_00(void);

// OBJ:�^���b�g[00] �̊Ǘ���Ԑݒ菈��
void SetObj_turret_00ControlState(OBJ_TURRET_00_CONTROL_STATE state);

#endif