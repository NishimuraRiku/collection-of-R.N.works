//========================================
// 
// ATK:��[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** atk_sword_00.h ***
//========================================
#ifndef _ATK_SWORD_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ATK_SWORD_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "physics.h"
#include "sound.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// ATK:��[00] �̎�ޖ��̏��\����
typedef struct
{
	float		fKnockBack;	// �m�b�N�o�b�N
	char		aModelPath	// ���f���̑��΃p�X
				[TXT_MAX];
	int			nLightType;	// ���̎��
	SOUND_LABEL slashSE;	// �a��SE
	HitTestSet	hitTestSet;	// �����蔻��ݒ���
}Atk_sword_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ATK:��[00] �̏���������
// ATK:��[00] �̏I������
// ATK:��[00] �̍X�V����
// ATK:��[00] �̕`�揈��
void InitAtk_sword_00(void);
void UninitAtk_sword_00(void);
void UpdateAtk_sword_00(void);
void DrawAtk_sword_00(void);

// ATK:��[00] �̓ǂݍ��ݏ���
void LoadAtk_sword_00(void);

// ATK:��[00] �̎�ޖ��̏����擾
Atk_sword_00Type *GetAtk_sword_00Type(void);

// ATK:��[00] �Ƃ̓����蔻�菈��
void CollisionAtk_sword_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

#endif