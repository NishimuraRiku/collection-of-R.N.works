//========================================
// 
// OBJ:���[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_pedestal_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "specific.h"
#include "text.h"
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "md_game_00.h"			// MD :�Q�[�����	[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:���[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_PEDESTAL_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_PEDESTAL_00_TYPE_DATA.txt"

// OBJ:���[00] �̎�ނ̐�
#define OBJ_PEDESTAL_00_TYPE_NUM	(1)

// OBJ:���[00] �̔񓞒����̑��ΈʒuY
#define OBJ_PEDESTAL_00_NON_ARRIVAL_RELATIVE_POS_Y	(-6)

// OBJ:���[00] �̒��ڂɂ����鎞��
#define OBJ_PEDESTAL_00_NOTICE_TIME	(10)

// OBJ:���[00] �̒l�i�e�L�X�g�̊J�n���ΈʒuY
// OBJ:���[00] �̒l�i�e�L�X�g�̏I�����ΈʒuY
#define OBJ_PEDESTAL_00_PRICE_TEXT_START_RELATIVE_POS_Y	(4)
#define OBJ_PEDESTAL_00_PRICE_TEXT_END_RELATIVE_POS_Y	(8)

// OBJ:���[00] �̏��i�̒l�グ�J�E���g���̏㏸�l
#define OBJ_PEDESTAL_00_PRICEUP_ADDRATE	(0.1f)

// OBJ:���[00] �̃v���C���[��HP�̉񕜗�
#define OBJ_PEDESTAL_00_PLAYER_HP_HEAL	(50)

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:���[00] �̎�ޖ��̏��\����
typedef struct
{
	char		aModelPath	// ���f���̑��΃p�X
				[TXT_MAX];
	HitTestSet	hitTestSet;	// �����蔻��ݒ���
}Obj_pedestal_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:���[00] �̃p�����[�^�[�̏���������
// OBJ:���[00] �̊Ǘ����̃p�����[�^�[�̏���������
void InitParameterObj_pedestal_00(Obj_pedestal_00 *pObj);
void InitParameterObj_pedestal_00Control(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9			g_aTextureObj_pedestal_00		// OBJ:���[00] �̃e�N�X�`���ւ̃|�C���^
							[OBJ_PEDESTAL_00_TYPE_NUM]
							[MATERIAL_3D_MAX]
							= {};
LPD3DXMESH					g_aMeshObj_pedestal_00			// OBJ:���[00] �̃��b�V��(���_���)�ւ̃|�C���^
							[OBJ_PEDESTAL_00_TYPE_NUM]
							= {};
LPD3DXBUFFER				g_pBuffMatObj_pedestal_00		// OBJ:���[00] �̃}�e���A���ւ̃|�C���^
							= NULL;
DWORD						g_aNumMatObj_pedestal_00		// OBJ:���[00] �̃}�e���A���̐�
							[OBJ_PEDESTAL_00_TYPE_NUM]
							= {};
D3DXMATRIX					g_mtxWorldObj_pedestal_00;		// OBJ:���[00] �̃��[���h�}�g���b�N�X
Obj_pedestal_00				g_aObj_pedestal_00				// OBJ:���[00] �̏��
							[OBJ_PEDESTAL_00_MAX];
Obj_pedestal_00Type			g_aObj_pedestal_00Type			// OBJ:���[00] �̎�ޖ��̏��
							[OBJ_PEDESTAL_00_TYPE_NUM];
Obj_pedestal_00Control		g_obj_pedestal_00Control;		// OBJ:���[00] �̊Ǘ����
Obj_pedestal_00ItemControl	g_aObj_pedestal_00ItemControl	// OBJ:���[00] �̏��i���̊Ǘ����
							[OBJ_PEDESTAL_00_ITEM_MAX];

// OBJ:���[00] �̏��i���
const Obj_pedestal_00Item g_aObj_pedestal_00Item[OBJ_PEDESTAL_00_ITEM_MAX] =
{
	5,	// �u���b�N
	25,	// �^���b�g
	10,	// ���ˑ��u
	20,	// ���d���u
	25,	// �v���C���[��HP����
	30,	// ���j�b�g��HP����
	20,	// ���̍U���̓A�b�v
	25,	// �e�̍U���̓A�b�v
};

//========== *** OBJ:���[00] �̏����擾 ***
Obj_pedestal_00 *GetObj_pedestal_00(void) 
{
	return g_aObj_pedestal_00;
}

//========== *** OBJ:���[00] �̊Ǘ������擾 ***
Obj_pedestal_00Control *GetObj_pedestal_00Control(void)
{
	return &g_obj_pedestal_00Control;
}

//========== *** OBJ:���[00] �̏��i���̏����擾 ***
const Obj_pedestal_00Item *GetObj_pedestal_00Item(void)
{
	return g_aObj_pedestal_00Item;
}

//========== *** OBJ:���[00] �̏��i���̏����擾 ***
Obj_pedestal_00ItemControl *GetObj_pedestal_00ItemControl(void)
{
	return g_aObj_pedestal_00ItemControl;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_pedestal_00�֐� - OBJ:���[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_pedestal_00(Obj_pedestal_00 *pObj)
{
	pObj->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���݂̈ʒu
	pObj->basePos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��ʒu
	pObj->rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pObj->nType				= 0;								// ���
	pObj->bUse				= false;							// �g�p����Ă���t���O
	pObj->nCounterNotice	= 0;								// ���ڃJ�E���^�[
}

//========================================
// InitParameterObj_pedestal_00Control�֐� - OBJ:���[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_pedestal_00Control(void) 
{
	Obj_pedestal_00Control	*pObj	// OBJ:���[00] �̊Ǘ����
							= &g_obj_pedestal_00Control;

	pObj->nCntItems			= 0;	// �i�����J�E���g
	pObj->nCounterArrival	= 0;	// �����J�E���^�[
}

//========================================
// InitParameterObj_pedestal_00ItemControl�֐� - OBJ:���[00] �̏��i���̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_pedestal_00ItemControl(Obj_pedestal_00ItemControl	*pObjItmCtl)
{
	pObjItmCtl->nCntPriceUp = 0;	// �l�グ�J�E���g
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_pedestal_00�֐� - OBJ:���[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_pedestal_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Obj_pedestal_00Type	*pObjType	// OBJ:���[00] �̎�ޖ��̏��
						= g_aObj_pedestal_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_PEDESTAL_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// �ǂݍ��݂��I��
		if (aDataSearch[0] == '#')			{ continue; }				// �܂�Ԃ�

		if (!strcmp(aDataSearch, "TYPE")) {
			// OBJ:���[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pObjType->aModelPath); }		// ���f���̑��΃p�X
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitObj_pedestal_00�֐� - OBJ:���[00] �̏��������� -
//========================================
void InitObj_pedestal_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_PEDESTAL_00_TYPE_NUM; nCntType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			g_aObj_pedestal_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_pedestal_00,
			NULL,
			&g_aNumMatObj_pedestal_00[nCntType],
			&g_aMeshObj_pedestal_00[nCntType]);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_pedestal_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_pedestal_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_pedestal_00[nCntType][nCntMat]);
			}
		}
	}

	// �Ǘ����̏���������
	InitParameterObj_pedestal_00Control();

	Obj_pedestal_00	*pObj	// OBJ:���[00] �̏��̃|�C���^
					= g_aObj_pedestal_00;

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		// �p�����[�^�[�̏���������
		InitParameterObj_pedestal_00(pObj);
	}

	Obj_pedestal_00ItemControl	*pObjItmCtl	// OBJ:���[00] �̏��i���̊Ǘ����
								= g_aObj_pedestal_00ItemControl;

	// ���i���̊Ǘ����̃p�����[�^�[�̏���������
	for (int nCntItm = 0; nCntItm < OBJ_PEDESTAL_00_ITEM_MAX; nCntItm++, pObjItmCtl++)
	{
		InitParameterObj_pedestal_00ItemControl(pObjItmCtl);
	}
}

//========================================
// UninitObj_pedestal_00�֐� - OBJ:���[00] �̏I������ -
//========================================
void UninitObj_pedestal_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_PEDESTAL_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_pedestal_00[nCntType] != NULL)
		{
			g_aMeshObj_pedestal_00[nCntType]->Release();
			g_aMeshObj_pedestal_00[nCntType] = NULL;
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_pedestal_00 != NULL)
	{
		g_pBuffMatObj_pedestal_00->Release();
		g_pBuffMatObj_pedestal_00 = NULL;
	}
}

//========================================
// UpdateObj_pedestal_00�֐� - OBJ:���[00] �̍X�V���� -
//========================================
void UpdateObj_pedestal_00(void)
{
	Obj_pedestal_00Control	*pObjCtl	// OBJ:���[00] �̊Ǘ����̃|�C���^
							= &g_obj_pedestal_00Control;

	if ((GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL) ||
		(GetMd_game_00()->state == MD_GAME_00_STATE_TIPS_WAIT))
	{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�orTIPS�҂��̎��A
		if (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL)
		{// OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���̎��A
			pObjCtl->nCounterArrival++;	// �����J�E���^�[�����Z
		}
		else if (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_RAID)
		{// OBJ:�X�e�[�W[00] �̏�Ԃ��P���̎��A
			pObjCtl->nCounterArrival--;	// �����J�E���^�[�����Z
		}
	}

	// �����J�E���^�[�𐧌�
	IntControl(&pObjCtl->nCounterArrival, OBJ_PEDESTAL_00_ARRIVAL_TIME, 0);

	Obj_pedestal_00		*pObj	// OBJ:���[00] �̏��̃|�C���^
						= g_aObj_pedestal_00;
	Obj_stage_00Items	*pItems	// OBJ:�X�e�[�W[00] �̕i�������̃|�C���^
						= &GetObj_stage_00Type()->itemsSet.aItems[pObjCtl->nCntItems];

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă����ԂłȂ����A
			continue;	// ������܂�Ԃ�
		}

		// Y���W�𓞒��J�E���^�[�ɉ����Đݒ�
		pObj->pos.y = pObj->basePos.y + (OBJ_PEDESTAL_00_NON_ARRIVAL_RELATIVE_POS_Y * (1.0f - ((float)pObjCtl->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME)));

		if (GetChr_player_00()->nPedestalIndex == nCntObj) 
		{// CHR:�v���C���[[00] �̑���̔ԍ����J�E���g�ƈ�v���Ă��鎞�A
			pObj->nCounterNotice++;	// ���ڃJ�E���^�[�����Z
		}
		else 
		{// CHR:�v���C���[[00] �̑���̔ԍ����J�E���g�ƈ�v���Ă��Ȃ����A
			pObj->nCounterNotice--;	// ���ڃJ�E���^�[�����Z
		}

		// ���ڃJ�E���^�[�𐧌�
		IntControl(&pObj->nCounterNotice, OBJ_PEDESTAL_00_NOTICE_TIME, 0);

		{
			char aString[TXT_MAX];
			OBJ_PEDESTAL_00_ITEM itm = pItems->aItem[nCntObj]; // �Y�����鏤�i�ԍ�
			sprintf(aString, "$%d", (int)(g_aObj_pedestal_00Item[pItems->aItem[nCntObj]].nPrice *
				(1.0f + (g_aObj_pedestal_00ItemControl[itm].nCntPriceUp * OBJ_PEDESTAL_00_PRICEUP_ADDRATE))));
			D3DXVECTOR3 setPos = pObj->pos;
			setPos.y +=
				(OBJ_PEDESTAL_00_PRICE_TEXT_START_RELATIVE_POS_Y * (1.0f - ((float)pObj->nCounterNotice / (float)OBJ_PEDESTAL_00_NOTICE_TIME))) +
				(OBJ_PEDESTAL_00_PRICE_TEXT_END_RELATIVE_POS_Y * ((float)pObj->nCounterNotice / (float)OBJ_PEDESTAL_00_NOTICE_TIME));
			SetText3D(
				aString,
				FONT_002,
				DISPLAY_CENTER,
				setPos,
				{ 255,255,255,(int)(255 * ((float)pObj->nCounterNotice / (float)OBJ_PEDESTAL_00_NOTICE_TIME)) },
				1.0f, 1.0f, false, false, true);
		}
	}
}

//========================================
// DrawObj_pedestal_00�֐� - OBJ:���[00] �̕`�揈�� -
//========================================
void DrawObj_pedestal_00(void)
{
	Obj_pedestal_00		*pObj	// OBJ:���[00] �̏��̃|�C���^
						= g_aObj_pedestal_00;
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot,	// �v�Z�p�}�g���b�N�X
						mtxTrans;
	D3DMATERIAL9		matDef;	// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_pedestal_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldObj_pedestal_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_pedestal_00, &g_mtxWorldObj_pedestal_00, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_pedestal_00, &g_mtxWorldObj_pedestal_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_pedestal_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_pedestal_00[pObj->nType]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_pedestal_00[pObj->nType][nCntMat]);
			
			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_pedestal_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_pedestal_00�֐� - OBJ:���[00] �̐ݒ菈�� -
//========================================
void SetObj_pedestal_00(D3DXVECTOR3 pos, int nType)
{
	Obj_pedestal_00		*pObj	// OBJ:���[00] �̏��̃|�C���^
						= g_aObj_pedestal_00;
	Obj_pedestal_00Type	*pType	// OBJ:���[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_pedestal_00Type[nType];

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		pObj->pos = pos;		// �ʒu����
		pObj->basePos = pos;	// ��ʒu����
		pObj->bUse = true;		// �g�p����Ă����Ԃɂ���
		pObj->nType = nType;	// ��ނ���

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// CollisionObj_pedestal_00�֐� - OBJ:���[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_pedestal_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if (pObjCollision->bHit)
	{// ������t���O���^�̎��A
		return -1;	// �������I������
	}

	Obj_pedestal_00	*pObj	// OBJ:���[00] �̏��̃|�C���^
					= g_aObj_pedestal_00;
	int				nIndex	// OBJ:���[00] �̕Ԃ��̔ԍ�
					= -1;

	for (int nCntObj = 0; nCntObj < OBJ_PEDESTAL_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_pedestal_00Type	*pType	// OBJ:���[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_pedestal_00Type[pObj->nType];

		// �Փ˔���ɕK�v�ȏ��
		CollisionCheck(
			vector,
			pObjCollision,
			myCollInfo,
			{
				&pObj->pos,
				pObj->pos,
				NULL,
				&pObj->rot,
				pObj->rot,
				pType->hitTestSet.aHitTest[0]
			},
			COLLCHK_MODE_LOOKDOWN);

		if(pObjCollision->bHit)
		{// ������t���O���^�̎��A
			nIndex = nCntObj;	// �Ԃ��̔ԍ���ݒ�
			break;				// �J��Ԃ������𔲂���
		}
	}

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// �ԍ���Ԃ�
	return nIndex;
}

//========================================
// PurchaseObj_pedestal_00�֐� - OBJ:���[00] �̍w������ -
//========================================
bool PurchaseObj_pedestal_00(void) 
{
	Chr_player_00				*pChr		// CHR:�v���C���[[00] �̏��̃|�C���^���擾
								= GetChr_player_00();
	Obj_pedestal_00				*pObj		// OBJ:���[00] �̏��̃|�C���^
								= g_aObj_pedestal_00;
	Obj_pedestal_00Control		*pObjCtl	// OBJ:���[00] �̊Ǘ����̃|�C���^
								= &g_obj_pedestal_00Control;
	Obj_stage_00Items			*pItems		// OBJ:�X�e�[�W[00] �̕i�������̃|�C���^
								= &GetObj_stage_00Type()->itemsSet.aItems[pObjCtl->nCntItems];
	OBJ_PEDESTAL_00_ITEM		itm			// �Y�����鏤�i�ԍ�
								= pItems->aItem[pChr->nPedestalIndex];

	if (!IntConsumption(
		&pChr->nCoin,
		g_aObj_pedestal_00Item[itm].nPrice *
		(1.0f + (g_aObj_pedestal_00ItemControl[itm].nCntPriceUp * OBJ_PEDESTAL_00_PRICEUP_ADDRATE))))
	{// �R�C������������ʑ���Ȃ��������A
		return false;	// �������I������
	}

	// ���i�̒l�グ����
	ItemPriceUpObj_pedestal_00(itm);

	switch (pItems->aItem[pChr->nPedestalIndex])
	{
	case /*/ �u���b�N             /*/OBJ_PEDESTAL_00_ITEM_BLOCK			:pChr->aItemCount[CHR_PLAYER_00_ITEM_BLOCK]++		; break;
	case /*/ �^���b�g             /*/OBJ_PEDESTAL_00_ITEM_TURRET		:pChr->aItemCount[CHR_PLAYER_00_ITEM_TURRET]++		; break;
	case /*/ ���ˑ��u             /*/OBJ_PEDESTAL_00_ITEM_REFLECTOR		:pChr->aItemCount[CHR_PLAYER_00_ITEM_REFLECTOR]++	; break;
	case /*/ ���d���u             /*/OBJ_PEDESTAL_00_ITEM_DISCHARGER	:pChr->aItemCount[CHR_PLAYER_00_ITEM_DISCHARGER]++	; break;
	case /*/ �v���C���[��HP���� /*/OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS:HealChr_player_00(OBJ_PEDESTAL_00_PLAYER_HP_HEAL)	; break;
	case /*/ ���j�b�g��HP����   /*/OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS	:HealUnitAll()										; break;
	case /*/ ���̍U���̓A�b�v     /*/OBJ_PEDESTAL_00_ITEM_SWORD_ATK_UP	:pChr->nSwordAtkUpCount++							; break;
	case /*/ �e�̍U���̓A�b�v     /*/OBJ_PEDESTAL_00_ITEM_BULLET_ATK_UP	:pChr->nBulletAtkUpCount++							; break;
	}

	return true;
}

//========================================
// ItemsChangeObj_pedestal_00�֐� - OBJ:���[00] �̕i�����ύX���� -
//========================================
void ItemsChangeObj_pedestal_00(void)
{
	Obj_pedestal_00Control	*pObjCtl	// OBJ:���[00] �̊Ǘ����̃|�C���^
							= &g_obj_pedestal_00Control;

	// �i�����ԍ������Z������ŏ�]�Z
	++pObjCtl->nCntItems %= GetObj_stage_00Type()->itemsSet.nItemsNum;
}

//========================================
// ItemPriceUpObj_pedestal_00�֐� - OBJ:���[00] �̏��i�̒l�グ���� -
//========================================
void ItemPriceUpObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm)
{
	Obj_pedestal_00ItemControl	*pObjItmCtl	// OBJ:���[00] �̏��i���̊Ǘ����
								= &g_aObj_pedestal_00ItemControl[itm];

	pObjItmCtl->nCntPriceUp++;	// �Y�����鏤�i�̒l�グ�J�E���g�����Z
}

//========================================
// ItemPriceDownObj_pedestal_00�֐� - OBJ:���[00] �̏��i�̒l�������� -
//========================================
void ItemPriceDownObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm)
{
	Obj_pedestal_00ItemControl	*pObjItmCtl	// OBJ:���[00] �̏��i���̊Ǘ����
								= &g_aObj_pedestal_00ItemControl[itm];

	pObjItmCtl->nCntPriceUp--;	// �Y�����鏤�i�̒l�グ�J�E���g�����Z
}