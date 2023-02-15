//========================================
// 
// OBJ:�~���[[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_mirror_00.h ***
//========================================
#ifndef _OBJ_MIRROR_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_MIRROR_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�~���[[00] �̏���������
// OBJ:�~���[[00] �̏I������
// OBJ:�~���[[00] �̍X�V����
// OBJ:�~���[[00] �̕`�揈��
void InitObj_mirror_00(void);
void UninitObj_mirror_00(void);
void UpdateObj_mirror_00(void);
void DrawObj_mirror_00(void);

// OBJ:�~���[[00] �̓ǂݍ��ݏ���
void LoadObj_mirror_00(void);

// OBJ:�~���[[00] �̐ݒ菈��
void SetObj_mirror_00(D3DXVECTOR3 pos);

// OBJ:�~���[[00] �Ƃ̓����蔻�菈��
int CollisionObj_mirror_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:�~���[[00] �̃_���[�W����
// OBJ:�~���[[00] �̑S�ĉ񕜏���
void DamageObj_mirror_00(int nIndex, int nDamage);
void HealAllObj_mirror_00(void);

#endif