//========================================
// 
// ATK:�Ռ�[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** atk_impact_00.h ***
//========================================
#ifndef _ATK_IMPACT_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ATK_IMPACT_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// ATK:�Ռ�[00] �̐e�̎��
typedef enum
{
	ATK_IMPACT_00_PARENTTYPE_PLAYER,	// �v���C���[
	ATK_IMPACT_00_PARENTTYPE_ENEMY,		// �G
	ATK_IMPACT_00_PARENTTYPE_MAX,
}ATK_IMPACT_00_PARENTTYPE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ATK:�Ռ�[00] �̏���������
// ATK:�Ռ�[00] �̏I������
// ATK:�Ռ�[00] �̍X�V����
void InitAtk_impact_00(void);
void UninitAtk_impact_00(void);
void UpdateAtk_impact_00(void);

// ATK:�Ռ�[00] �̓ǂݍ��ݏ���
void LoadAtk_impact_00(void);

// ATK:�Ռ�[00] �̐ݒ菈��
void SetAtk_impact_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_IMPACT_00_PARENTTYPE parentType);

#endif