//========================================
// 
// ITM:�R�C��[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** itm_coin_00.h ***
//========================================
#ifndef _ITM_COIN_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ITM_COIN_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ITM:�R�C��[00] �̏���������
// ITM:�R�C��[00] �̏I������
// ITM:�R�C��[00] �̍X�V����
// ITM:�R�C��[00] �̕`�揈��
void InitItm_coin_00(void);
void UninitItm_coin_00(void);
void UpdateItm_coin_00(void);
void DrawItm_coin_00(void);

// ITM:�R�C��[00] �̓ǂݍ��ݏ���
void LoadItm_coin_00(void);

// ITM:�R�C��[00] �̐ݒ菈��
void SetItm_coin_00(D3DXVECTOR3 pos, int nSetNum);

// ITM:�R�C��[00] �Ƃ̓����蔻�菈��
// ITM:�R�C��[00] �̔j�󏈗�
int CollisionItm_coin_00(VECTOR vector, Collision *pItmCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);
void DestroyItm_coin_00(int nIndex);

#endif