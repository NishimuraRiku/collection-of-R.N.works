//========================================
// 
// OBJ:�X�C�b�`[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_switch_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "light.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"		// CHR:�G					[00]
#include "eff_explosion_00.h"	// EFF:����					[00]
#include "md_game_00.h"			// MD :�Q�[�����			[00]
#include "md_tutorial_00.h"		// MD :�`���[�g���A�����	[00]
#include "obj_switch_00.h"		// OBJ:�X�C�b�`				[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W				[00]
#include "ui_damage_00.h"		// UI :�_���[�W				[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�X�C�b�`[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_SWITCH_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_SWITCH_00_TYPE_DATA.txt"

// OBJ:�X�C�b�`[00] �̎�ނ̐�
#define OBJ_SWITCH_00_TYPE_NUM	(1)

// OBJ:�X�C�b�`[00] �̓����ɂ����鎞��
// OBJ:�X�C�b�`[00] �̔񓞒����̑��ΈʒuY
#define OBJ_SWITCH_00_ARRIVAL_TIME					(60)
#define OBJ_SWITCH_00_NON_ARRIVAL_RELATIVE_POS_Y	(-6)

// OBJ:�X�C�b�`[00] �̃e�L�X�g�̑��ΈʒuY
// OBJ:�X�C�b�`[00] �̃e�L�X�g�̊g��{��
#define OBJ_SWITCH_00_TEXT_RELATIVE_POS_Y	(5)
#define OBJ_SWITCH_00_TEXT_SCALE			(0.5f)

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:�X�C�b�`[00] �̎�ޖ��̏��\����
typedef struct
{
	HitTestSet	hitTestSet;		// �����蔻��ݒ���
	Parts3DSet	partsSet;		// ���i�ݒ���(3D)
	Motion3DSet motionSet;		// ���[�V�����ݒ���
}Obj_switch_00Type;

// OBJ:�X�C�b�`[00] �̊Ǘ����\����
typedef struct
{
	int nCounterArrival;	// �����J�E���^�[
}Obj_switch_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�X�C�b�`[00] �̃p�����[�^�[�̏���������
// OBJ:�X�C�b�`[00] �̊Ǘ����̃p�����[�^�[�̏���������
void InitParameterObj_switch_00(void);
void InitParameterObj_switch_00Control(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_switch_00		// OBJ:�X�C�b�`[00] �̃e�N�X�`���ւ̃|�C���^
						[OBJ_SWITCH_00_TYPE_NUM]
						[PARTS_3D_MAX]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshObj_switch_00		// OBJ:�X�C�b�`[00] �̃��b�V��(���_���)�ւ̃|�C���^
						[OBJ_SWITCH_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
LPD3DXBUFFER			g_pBuffMatObj_switch_00		// OBJ:�X�C�b�`[00] �̃}�e���A���ւ̃|�C���^
						= NULL;
DWORD					g_aNumMatObj_switch_00		// OBJ:�X�C�b�`[00] �̃}�e���A���̐�
						[OBJ_SWITCH_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
D3DXMATRIX				g_aMtxWorldObj_switch_00	// OBJ:�X�C�b�`[00] �̃��[���h�}�g���b�N�X
						[PARTS_3D_MAX];
Obj_switch_00			g_obj_switch_00;			// OBJ:�X�C�b�`[00] �̏��
Obj_switch_00Control	g_obj_switch_00Control;		// OBJ:�X�C�b�`[00] �̊Ǘ����
Obj_switch_00Type		g_aObj_switch_00Type		// OBJ:�X�C�b�`[00] �̎�ޖ��̏��
						[OBJ_SWITCH_00_TYPE_NUM];

//========== *** OBJ:�X�C�b�`[00] �̏����擾 ***
Obj_switch_00 *GetObj_switch_00(void) 
{
	return &g_obj_switch_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_switch_00(void) 
{
	Obj_switch_00	*pObj	// OBJ:�X�C�b�`[00] �̏��̃|�C���^
					= &g_obj_switch_00;

	pObj->pos		= INITD3DXVECTOR3;				// ���݂̈ʒu
	pObj->rot		= INITD3DXVECTOR3;				// ����
	pObj->nType		= 0;							// ���
	pObj->partsInfo	= {};							// ���i�Ǘ�
	pObj->motion	= OBJ_SWITCH_00_MOTION_WAIT;	// ���[�V����
}

//========================================
// InitParameterObj_switch_00Control�֐� - OBJ:�X�C�b�`[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_switch_00Control(void)
{
	Obj_switch_00Control	*pObjCtl	// OBJ:�X�C�b�`[00] �̏��̃|�C���^
							= &g_obj_switch_00Control;

	pObjCtl->nCounterArrival = 0;	// �����J�E���^�[
}

//========================================
// MotionSetObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̃��[�V�����ݒ菈�� -
//========================================
void MotionSetObj_switch_00(void)
{
	Obj_switch_00		*pObj		// OBJ:�X�C�b�`[00] �̏��̃|�C���^
						= &g_obj_switch_00;
	Obj_switch_00Type	*pType		// OBJ:�X�C�b�`[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_switch_00Type[pObj->nType];
	Motion3D			*pMotion	// ���݂̃��[�V�����̏��̃|�C���^
						= &pType->motionSet.aMotion[pObj->motion];;

	// ���[�V�����ݒ�
	SetMotion3D(
		&pObj->partsInfo,
		pObj->motion);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_switch_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Obj_switch_00Type	*pObjType	// OBJ:�X�C�b�`[00] �̎�ޖ��̏��
						= g_aObj_switch_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_SWITCH_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:�X�C�b�`[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// �����蔻��ݒ���
				else if (!strcmp(aDataSearch, "PARTSSET"))		{ LoadParts3DSet(pFile, &pObjType->partsSet); }		// ���i�ݒ���
				else if (!strcmp(aDataSearch, "MOTIONSET"))		{ LoadMotion3DSet(pFile, &pObjType->motionSet); }	// ���[�V�����ݒ���
			}
		}
	}
}

//========================================
// InitObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̏��������� -
//========================================
void InitObj_switch_00(void)
{
	// �Ǘ����̏�����
	InitParameterObj_switch_00Control();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_SWITCH_00_TYPE_NUM; nCntType++)
	{
		Obj_switch_00Type	*pType	// OBJ:�X�C�b�`[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_switch_00Type[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			Parts3DType *pPartsType	// ���i(3D)�̎�ޖ��̏��\����
						= &pType->partsSet.aPartsType[nCntParts];

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_pBuffMatObj_switch_00,
				NULL,
				&g_aNumMatObj_switch_00[nCntType][nCntParts],
				&g_aMeshObj_switch_00[nCntType][nCntParts]);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObj_switch_00->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_switch_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// �p�����[�^�[�̏���������
	InitParameterObj_switch_00();

	Obj_switch_00		*pObj	// OBJ:�X�C�b�`[00] �̏��̃|�C���^
						= &g_obj_switch_00;
	Obj_switch_00Type	*pType	// OBJ:�X�C�b�`[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_switch_00Type[pObj->nType];

	// ���i(3D)�̃g�����X�t�H�[����������
	InitParts3DTransform(&pObj->partsInfo, &pType->partsSet);
}

//========================================
// UninitObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̏I������ -
//========================================
void UninitObj_switch_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_SWITCH_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			if (g_aMeshObj_switch_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshObj_switch_00[nCntType][nCntParts]->Release();
				g_aMeshObj_switch_00[nCntType][nCntParts] = NULL;
			}

			// �e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
				if (g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat] != NULL)
				{
					g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat]->Release();
					g_aTextureObj_switch_00[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_switch_00 != NULL)
	{
		g_pBuffMatObj_switch_00->Release();
		g_pBuffMatObj_switch_00 = NULL;
	}
}

//========================================
// UpdateObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̍X�V���� -
//========================================
void UpdateObj_switch_00(void)
{
	Obj_switch_00Control	*pObjCtl	// OBJ:���[00] �̊Ǘ����̃|�C���^
							= &g_obj_switch_00Control;

	if (((GetMode() == MODE_GAME_00) &&
		(GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL))
		||
		(GetMode() == MODE_TUTORIAL_00) &&
		(GetMd_tutorial_00()->state == MD_TUTORIAL_00_STATE_NORMAL))
	{// (MD:�Q�[�����[00] �̏�Ԃ��ʏ�orTIPS�҂�)or(�`���[�g���A����ʂ̏�Ԃ��ʏ�)�̎��A
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

	Obj_switch_00		*pObj	// OBJ:�X�C�b�`[00] �̏��̃|�C���^
						= &g_obj_switch_00;
	Obj_switch_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_switch_00Type[pObj->nType];
	{
		float	fRate	// �J�E���^�[�̊���
				= (float)pObjCtl->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME;

		// �e�L�X�g(3D)�̐ݒ菈��
		SetText3D(
			"SKIP",
			FONT_002, DISPLAY_CENTER,
			pObj->pos + D3DXVECTOR3(
				0.0f,
				OBJ_SWITCH_00_TEXT_RELATIVE_POS_Y + (OBJ_SWITCH_00_NON_ARRIVAL_RELATIVE_POS_Y * (1.0f - fRate)),
				0.0f),
				{ 255,255,255,(int)(255 * fRate) },
			OBJ_SWITCH_00_TEXT_SCALE, OBJ_SWITCH_00_TEXT_SCALE, true, false, true);
	}

	// ���i(3D)�̃g�����X�t�H�[�����擾
	GetParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// ���[�V�����̐ݒ菈��
	MotionSetObj_switch_00();

	// ���[�V�����̍X�V����
	if (UpdateMotion3D(
		pObj->partsInfo.nMotion,
		g_aObj_switch_00Type[pObj->nType].motionSet,
		&pObj->partsInfo,
		g_aObj_switch_00Type[pObj->nType].partsSet))
	{// ���[�V���������[�v�ɒB�������A
		Motion3D	*pMotion	// ���݂̃��[�V�����̏��̃|�C���^
					= &pType->motionSet.aMotion[pObj->motion];
		if (!pMotion->bLoop)
		{// ���݂̃��[�V�����̃��[�v�t���O���U�̎��A
			if (pObj->motion == OBJ_SWITCH_00_MOTION_PUSH)
			{// ���[�V�����������̎��A
				// OBJ:�Q�[�����[00] �̏�Ԃ��P���ɐݒ�
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_RAID);
			}
			pObj->motion = OBJ_SWITCH_00_MOTION_WAIT;	// ���[�V������ҋ@�ɐݒ�
		}
	}
}

//========================================
// DrawObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̕`�揈�� -
//========================================
void DrawObj_switch_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX			mtxSelf;			// �{�̂̃}�g���b�N�X

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_switch_00->GetBufferPointer();

	// �{�̂̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxSelf);

	Obj_switch_00	*pObj	// OBJ:�X�C�b�`[00] �̏��̃|�C���^
					= &g_obj_switch_00;

	// �{�̂̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);
	{
		Obj_switch_00Control	*pObjCtl	// OBJ:���[00] �̊Ǘ����̃|�C���^
								= &g_obj_switch_00Control;

		// �{�̂̈ʒu�𔽉f
		D3DXMatrixTranslation(
			&mtxTrans,
			pObj->pos.x,
			pObj->pos.y + (OBJ_SWITCH_00_NON_ARRIVAL_RELATIVE_POS_Y * (1.0f - ((float)pObjCtl->nCounterArrival / (float)OBJ_PEDESTAL_00_ARRIVAL_TIME))),
			pObj->pos.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);
	}
	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	Obj_switch_00Type	*pType	// OBJ:�X�C�b�`[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_switch_00Type[pObj->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// ���i(3D)�̏��̃|�C���^
					= &pObj->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// �e�}�g���b�N�X

		// ���i�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMtxWorldObj_switch_00[nCntParts]);

		// ���i�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_switch_00[nCntParts], &g_aMtxWorldObj_switch_00[nCntParts], &mtxRot);

		// ���i�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_switch_00[nCntParts], &g_aMtxWorldObj_switch_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
			mtxParent = g_aMtxWorldObj_switch_00[pPartsType->nParent];
		}
		else
		{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
			mtxParent = mtxSelf;
		}

		// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(
			&g_aMtxWorldObj_switch_00[nCntParts],
			&g_aMtxWorldObj_switch_00[nCntParts],
			&mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObj_switch_00[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_switch_00[pObj->nType][nCntParts]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_switch_00[pObj->nType][nCntParts][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_switch_00[pObj->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// CollisionObj_switch_00�֐� - OBJ:�X�C�b�`[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_switch_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if ((GetObj_stage_00()->state == OBJ_STAGE_00_STATE_RAID) || (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_CLEAR))
	{// OBJ:�X�e�[�W[00] �̏�Ԃ��P�� or �N���A�̎��A
		return -1;	// �������I������
	}

	Obj_switch_00		*pObj	// OBJ:�X�C�b�`[00] �̏��̃|�C���^
						= &g_obj_switch_00;
	int					nIndex	// OBJ:�X�C�b�`[00] �̕Ԃ��̔ԍ�
						= -1;
	Obj_switch_00Type	*pType	// OBJ:�X�C�b�`[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_switch_00Type[pObj->nType];

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

	if (pObjCollision->bHit)
	{// ������t���O���^�̎��A
		nIndex = 0;	// �Ԃ��̔ԍ���ݒ�
	}

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pObjCollision);

	// �ԍ���Ԃ�
	return nIndex;
}

//========================================
// PushObj_switch_00�֐� - OBJ:�X�C�b�`[00] �̉������� -
//========================================
void PushObj_switch_00(void)
{
	Obj_switch_00	*pObj	// OBJ:�X�C�b�`[00] �̏��̃|�C���^
					= &g_obj_switch_00;

	// ���[�V�����������Őݒ�
	pObj->motion = OBJ_SWITCH_00_MOTION_PUSH;
	SetMotion3D(
		&pObj->partsInfo,
		pObj->motion);	
}