//========================================
// 
// OBJ:���d���u[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_discharger_00.h ***
//========================================
#ifndef _OBJ_DISCHARGER_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_DISCHARGER_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:���d���u[00] �̊Ǘ����
typedef enum
{
	OBJ_DISCHARGER_00_CONTROL_STATE_DYNAMIC,	// ���I
	OBJ_DISCHARGER_00_CONTROL_STATE_STATIC,		// �ÓI
	OBJ_DISCHARGER_00_CONTROL_STATE_MAX,
}OBJ_DISCHARGER_00_CONTROL_STATE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:���d���u[00] �̏���������
// OBJ:���d���u[00] �̏I������
// OBJ:���d���u[00] �̍X�V����
// OBJ:���d���u[00] �̕`�揈��
void InitObj_discharger_00(void);
void UninitObj_discharger_00(void);
void UpdateObj_discharger_00(void);
void DrawObj_discharger_00(void);

// OBJ:���d���u[00] �̓ǂݍ��ݏ���
void LoadObj_discharger_00(void);

// OBJ:���d���u[00] �̐ݒ菈��
void SetObj_discharger_00(D3DXVECTOR3 pos);

// OBJ:���d���u[00] �Ƃ̓����蔻�菈��
int CollisionObj_discharger_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:���d���u[00] �̃_���[�W����
// OBJ:���d���u[00] �̑S�ĉ񕜏���
void DamageObj_discharger_00(int nIndex, int nDamage);
void HealAllObj_discharger_00(void);

// OBJ:���d���u[00] �̊Ǘ���Ԑݒ菈��
void SetObj_discharger_00ControlState(OBJ_DISCHARGER_00_CONTROL_STATE state);

#endif