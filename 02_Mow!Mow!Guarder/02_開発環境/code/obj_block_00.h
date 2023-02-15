//========================================
// 
// OBJ:�u���b�N[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_block_00.h ***
//========================================
#ifndef _OBJ_BLOCK_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_BLOCK_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�u���b�N[00] �̏���������
// OBJ:�u���b�N[00] �̏I������
// OBJ:�u���b�N[00] �̍X�V����
// OBJ:�u���b�N[00] �̕`�揈��
void InitObj_block_00(void);
void UninitObj_block_00(void);
void UpdateObj_block_00(void);
void DrawObj_block_00(void);

// OBJ:�u���b�N[00] �̓ǂݍ��ݏ���
void LoadObj_block_00(void);

// OBJ:�u���b�N[00] �̐ݒ菈��
void SetObj_block_00(D3DXVECTOR3 pos);

// OBJ:�u���b�N[00] �Ƃ̓����蔻�菈��
int CollisionObj_block_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:�u���b�N[00] �̃_���[�W����
// OBJ:�u���b�N[00] �̑S�ĉ񕜏���
void DamageObj_block_00(int nIndex, int nDamage);
void HealAllObj_block_00(void);

#endif