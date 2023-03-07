//========================================
// 
// OBJ:�Ŕ�[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_signboard_00.cpp ***
//========================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "obj_signboard_00.h"	// OBJ:�Ŕ�			[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�Ŕ�[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_SIGNBOARD_00_TYPE_DATA_FILE_PATH "data\\GAMEDATA\\OBJECT\\OBJ_SIGNBOARD_00_TYPE_DATA.txt"
// OBJ:�Ŕ�[00] �̍ő吔
#define OBJ_SIGNBOARD_00_MAX (256)
// OBJ:�Ŕ�[00] �̎�ނ̍ő吔
#define OBJ_SIGNBOARD_00_TYPE_NUM (2)
// OBJ:�Ŕ�[00] �̖��G����
#define OBJ_SIGNBOARD_00_INVINCIBLE_TIME (10)
// OBJ:�Ŕ�[00] �̃_���[�W����
#define OBJ_SIGNBOARD_00_DAMAGE_TIME (10)
// OBJ:�Ŕ�[00] �̉�]��
#define OBJ_SIGNBOARD_00_SPIN D3DXVECTOR3(0.0f,0.005f,0.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�Ŕ�[00] �̎�ޖ��̏��\����
typedef struct
{
	char aModelPath[TXT_MAX];	// ���f���̑��΃p�X
	HitTestSet hitTestSet;		// �����蔻��ݒ���
}Obj_signboard_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�Ŕ�[00] �̃p�����[�^�[�̏���������
void InitParameterObj_signboard_00(Obj_signboard_00 *pObj);

// OBJ:�Ŕ�[00] �̔j�󏈗�
void DestroyObj_signboard_00(Obj_signboard_00 *pObj);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_signboard_00	// OBJ:�Ŕ�[00] �̃e�N�X�`���ւ̃|�C���^
						[OBJ_SIGNBOARD_00_TYPE_NUM]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_signboard_00		// OBJ:�Ŕ�[00] �̃��b�V��(���_���)�ւ̃|�C���^
						[OBJ_SIGNBOARD_00_TYPE_NUM]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_signboard_00	// OBJ:�Ŕ�[00] �̃}�e���A���ւ̃|�C���^
						= NULL;
DWORD					g_aNumMatObj_signboard_00	// OBJ:�Ŕ�[00] �̃}�e���A���̐�
						[OBJ_SIGNBOARD_00_TYPE_NUM]
						= {};
D3DXMATRIX				g_mtxWorldObj_signboard_00;	// OBJ:�Ŕ�[00] �̃��[���h�}�g���b�N�X
Obj_signboard_00		g_aObj_signboard_00			// OBJ:�Ŕ�[00] �̏��
						[OBJ_SIGNBOARD_00_MAX];
Obj_signboard_00Type	g_aObj_signboard_00Type		// OBJ:�Ŕ�[00] �̎�ޖ��̏��
						[OBJ_SIGNBOARD_00_TYPE_NUM];

//========== *** OBJ:�Ŕ�[00] �̏��擾���� ***
Obj_signboard_00 *GetObj_signboard_00(void) 
{
	return g_aObj_signboard_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_signboard_00(Obj_signboard_00 *pObj)
{
	pObj->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pObj->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pObj->nType			= 0;								// ���
	pObj->bUse			= false;							// �g�p����Ă��邩�t���O
	pObj->hitTestInfo	= {};								// �����蔻��̊Ǘ����
}

//========================================
// DestroyObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̔j�󏈗� -
//========================================
void DestroyObj_signboard_00(Obj_signboard_00 *pObj)
{
	pObj->bUse = false;				// �g�p����Ă��Ȃ���Ԃɂ���
	ItemPriceDownObj_pedestal_00(	// OBJ:���[00] �̏��i(���ˑ��u)�̒l��������
		OBJ_PEDESTAL_00_ITEM_REFLECTOR);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_signboard_00(void)
{
	FILE					*pFile;		// �t�@�C���|�C���^
	char					aDataSearch	// �f�[�^�����p
							[TXT_MAX];
	Obj_signboard_00Type	*pObjType	// OBJ:�Ŕ�[00] �̎�ޖ��̏��
							= g_aObj_signboard_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_SIGNBOARD_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
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
// InitObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̏��������� -
//========================================
void InitObj_signboard_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_SIGNBOARD_00_TYPE_NUM; nCntType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			g_aObj_signboard_00Type[nCntType].aModelPath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObj_signboard_00,
			NULL,
			&g_aNumMatObj_signboard_00[nCntType],
			&g_aMeshObj_signboard_00[nCntType]);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObj_signboard_00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_signboard_00[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_signboard_00[nCntType][nCntMat]);
			}
		}
	}

	Obj_signboard_00	*pObj	// OBJ:�Ŕ�[00] �̏��̃|�C���^
					= g_aObj_signboard_00;

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++, pObj++)
	{
		// �p�����[�^�[�̏���������
		InitParameterObj_signboard_00(pObj);
	}
}

//========================================
// UninitObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̏I������ -
//========================================
void UninitObj_signboard_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_SIGNBOARD_00_TYPE_NUM; nCntType++)
	{
		if (g_aMeshObj_signboard_00[nCntType] != NULL)
		{
			g_aMeshObj_signboard_00[nCntType]->Release();
			g_aMeshObj_signboard_00[nCntType] = NULL;
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_signboard_00 != NULL)
	{
		g_pBuffMatObj_signboard_00->Release();
		g_pBuffMatObj_signboard_00 = NULL;
	}

	// ����������
	InitObj_signboard_00();
}

//========================================
// UpdateObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̍X�V���� -
//========================================
void UpdateObj_signboard_00(void)
{
	
}

//========================================
// DrawObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̕`�揈�� -
//========================================
void DrawObj_signboard_00(void)
{
	Obj_signboard_00		*pObj				// OBJ:�Ŕ�[00] �̏��̃|�C���^
						= g_aObj_signboard_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_signboard_00->GetBufferPointer();

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++,pObj++)
	{
		if (!pObj->bUse) 
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldObj_signboard_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_signboard_00, &g_mtxWorldObj_signboard_00, &mtxRot);
		
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldObj_signboard_00, &g_mtxWorldObj_signboard_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_signboard_00);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_signboard_00[pObj->nType]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_signboard_00[pObj->nType][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_signboard_00[pObj->nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// SetObj_signboard_00�֐� - OBJ:�Ŕ�[00] �̐ݒ菈�� -
//========================================
void SetObj_signboard_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType)
{
	Obj_signboard_00	*pObj	// OBJ:�Ŕ�[00] �̏��̃|�C���^
						= g_aObj_signboard_00;

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++, pObj++)
	{
		if (pObj->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Obj_signboard_00Type	*pType	// OBJ:�Ŕ�[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_signboard_00Type[pObj->nType];

		pObj->pos = pos;		// �ʒu����
		pObj->rot = rot;		// ��������
		pObj->nType = nType;	// ��ނ���
		pObj->bUse = true;		// �g�p����Ă����Ԃɂ���

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// CollisionObj_signboard_00�֐� - OBJ:�Ŕ�[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_signboard_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool				bHitTemp	// ������t���O��ۑ�
						= pObjCollision->bHit;
	Obj_signboard_00	*pObj		// OBJ:�Ŕ�[00] �̏��̃|�C���^
						= g_aObj_signboard_00;
	int					nIndex		// CHR:�G[00] �̕Ԃ��̔ԍ�
						= -1;

	for (int nCntObj = 0; nCntObj < OBJ_SIGNBOARD_00_MAX; nCntObj++, pObj++)
	{
		if (!pObj->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

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
				g_aObj_signboard_00Type[pObj->nType].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		D3DXVECTOR3 pos = pObj->pos;
		pos.x += sinf(pObj->rot.y) * CHR_PLAYER_00_SIGNBOARD_DECISION_DISTANCE;
		pos.z += cosf(pObj->rot.y) * CHR_PLAYER_00_SIGNBOARD_DECISION_DISTANCE;

		if (FindDistanceLookDown(pos, GetChr_player_00()->pos) <= CHR_PLAYER_00_SIGNBOARD_DISTANCE)
		{// �v���C���[�Ƃ̋��������苗�����̎��A
			if (fabsf(AngleDifference(myCollInfo.pRot->y, pObj->rot.y)) >= D3DX_PI * 0.5f)
			{// �p�x����90�x�ȏ�(���ʂ������Ă���)
				nIndex = nCntObj;
			}
		}
	}

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// �ԍ���Ԃ�
	return nIndex;
}