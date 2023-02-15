//========================================
// 
// ATK:�e[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** atk_bullet_00.h ***
//========================================
#ifndef _ATK_BULLET_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ATK_BULLET_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// ATK:�e[00] �̐e�̎��
typedef enum
{
	ATK_BULLET_00_PARENTTYPE_PLAYER,	// �v���C���[
	ATK_BULLET_00_PARENTTYPE_ENEMY,		// �G
	ATK_BULLET_00_PARENTTYPE_MAX,
}ATK_BULLET_00_PARENTTYPE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ATK:�e[00] �̏���������
// ATK:�e[00] �̏I������
// ATK:�e[00] �̍X�V����
// ATK:�e[00] �̕`�揈��
void InitAtk_bullet_00(void);
void UninitAtk_bullet_00(void);
void UpdateAtk_bullet_00(void);
void DrawAtk_bullet_00(void);

// ATK:�e[00] �̓ǂݍ��ݏ���
void LoadAtk_bullet_00(void);

// ATK:�e[00] �̐ݒ菈��
void SetAtk_bullet_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_BULLET_00_PARENTTYPE parentType);

#endif