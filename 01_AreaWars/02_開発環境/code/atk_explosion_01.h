//========================================
//  *** atk_explosion_01.h ***
//========================================
#ifndef _ATK_EXPLOSION_01_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ATK_EXPLOSION_01_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// ����[00] �̎��
typedef enum
{
	ATK_EXPLOSION_01_TYPE_00 = 0,
	ATK_EXPLOSION_01_TYPE_MAX,
}ATK_EXPLOSION_01_TYPE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitAtk_explosion_01	(void);															// ����[00] �̏���������
void UninitAtk_explosion_01	(void);															// ����[00] �̏I������
void UpdateAtk_explosion_01	(void);															// ����[00] �̍X�V����
void DrawAtk_explosion_01	(void);															// ����[00] �̕`�揈��
void SetAtk_explosion_01	(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ATK_EXPLOSION_01_TYPE type);	// ����[00] �̐ݒ菈��

#endif