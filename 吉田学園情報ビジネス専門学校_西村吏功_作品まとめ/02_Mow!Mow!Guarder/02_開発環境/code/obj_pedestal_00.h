//========================================
// 
// OBJ:���[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_pedestal_00.h ***
//========================================
#ifndef _OBJ_PEDESTAL_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_PEDESTAL_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "physics.h"

//****************************************
// �}�N����`
//****************************************
// OBJ:���[00] �̍ő吔
#define OBJ_PEDESTAL_00_MAX	(64)

// OBJ:���[00] �̓����ɂ����鎞��
// OBJ:���[00] �̃��j�b�g��HP�̏㏸�{��
// OBJ:���[00] �̃v���C���[��HP�̏㏸�{��
#define OBJ_PEDESTAL_00_ARRIVAL_TIME		(60)
#define OBJ_PEDESTAL_00_UNIT_HP_ADDRATE		(0.2f)
#define OBJ_PEDESTAL_00_PLAYER_HP_ADDRATE	(0.2f)

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:���[00] �̏��i�ԍ�
typedef enum 
{
	OBJ_PEDESTAL_00_ITEM_BLOCK,				// �u���b�N
	OBJ_PEDESTAL_00_ITEM_TURRET,			// �^���b�g
	OBJ_PEDESTAL_00_ITEM_REFLECTOR,			// ���ˑ��u
	OBJ_PEDESTAL_00_ITEM_DISCHARGER,		// ���d���u
	OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS,	// �v���C���[��HP����
	OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS,		// ���j�b�g��HP����
	OBJ_PEDESTAL_00_ITEM_SWORD_ATK_UP,		// ���̍U���̓A�b�v
	OBJ_PEDESTAL_00_ITEM_BULLET_ATK_UP,		// �e�̍U���̓A�b�v
	OBJ_PEDESTAL_00_ITEM_MAX,
}OBJ_PEDESTAL_00_ITEM;

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:���[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 basePos;	// ��ʒu
	D3DXVECTOR3 rot;		// ����

	// ���ޏ��
	int nType;	// ���

	// ��Ԋ֘A
	bool	bUse;			// �g�p����Ă��邩�t���O
	int		nCounterNotice;	// ���ڃJ�E���^�[
}Obj_pedestal_00;

// OBJ:���[00] �̊Ǘ����\����
typedef struct
{
	int	nCntItems;			// �i�����J�E���g
	int nCounterArrival;	// �����J�E���^�[
}Obj_pedestal_00Control;

// OBJ:���[00] �̏��i���̏��\����
typedef struct
{
	int	nPrice;	// �l�i
}Obj_pedestal_00Item;

// OBJ:���[00] �̏��i���̊Ǘ����\����
typedef struct
{
	int	nCntPriceUp;	// �l�グ�J�E���g
}Obj_pedestal_00ItemControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:���[00] �̏����擾
// OBJ:���[00] �̊Ǘ������擾
// OBJ:���[00] �̏��i�����擾
Obj_pedestal_00 *GetObj_pedestal_00(void);
Obj_pedestal_00Control *GetObj_pedestal_00Control(void);
const Obj_pedestal_00Item *GetObj_pedestal_00Item(void);
Obj_pedestal_00ItemControl *GetObj_pedestal_00ItemControl(void);

// OBJ:���[00] �̏���������
// OBJ:���[00] �̏I������
// OBJ:���[00] �̍X�V����
// OBJ:���[00] �̕`�揈��
void InitObj_pedestal_00(void);
void UninitObj_pedestal_00(void);
void UpdateObj_pedestal_00(void);
void DrawObj_pedestal_00(void);

// OBJ:���[00] �̓ǂݍ��ݏ���
void LoadObj_pedestal_00(void);

// OBJ:���[00] �̐ݒ菈��
void SetObj_pedestal_00(D3DXVECTOR3 pos, int nType);

// OBJ:���[00] �Ƃ̓����蔻�菈��
int CollisionObj_pedestal_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:���[00] �̍w������
// OBJ:���[00] �̕i�����ύX����
// OBJ:���[00] �̏��i�l�グ����
// OBJ:���[00] �̏��i�l��������
bool PurchaseObj_pedestal_00(void);
void ItemsChangeObj_pedestal_00(void);
void ItemPriceUpObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm);
void ItemPriceDownObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm);

#endif