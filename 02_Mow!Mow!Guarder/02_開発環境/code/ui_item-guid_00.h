//========================================
// 
// UI:���i�K�C�h[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_item-guid_00.h ***
//========================================
#ifndef _UI_ITEMGUID_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_ITEMGUID_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
// UI:���i�K�C�h[00] �̓����ɂ����鎞��
#define UI_ITEMGUID_00_ARRIVAL_TIME	(10)

// UI:���i�K�C�h[00] �̔񓞒����̑��ΈʒuY
#define UI_ITEMGUID_00_NON_ARRIVAL_RELATIVE_POS_Y	(-10)

//****************************************
// �\���̂̒�`
//****************************************
// UI:���i�K�C�h[00] �̏��\����
typedef struct
{
	int nTex;				// �e�N�X�`���ԍ�
	int nCounterArrival;	// �����J�E���^�[
}Ui_itemGuid_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���i�K�C�h[00] �̏��擾����
Ui_itemGuid_00 *GetUi_itemGuid_00(void);

// UI:���i�K�C�h[00] �̏���������
// UI:���i�K�C�h[00] �̏I������
// UI:���i�K�C�h[00] �̍X�V����
// UI:���i�K�C�h[00] �̕`�揈��
void InitUi_itemGuid_00(void);
void UninitUi_itemGuid_00(void);
void UpdateUi_itemGuid_00(void);
void DrawUi_itemGuid_00(void);

#endif