//========================================
// 
// OBJ:�R�A[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_core_00.cpp ***
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
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "eff_explosion_00.h"	// EFF:����			[00]
#include "md_game_00.h"			// MD :�Q�[�����	[00]
#include "obj_core_00.h"		// OBJ:�R�A			[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�R�A[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_CORE_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_CORE_00_TYPE_DATA.txt"

// OBJ:�R�A[00] �̎�ނ̐�
#define OBJ_CORE_00_TYPE_NUM	(2)

// OBJ:�R�A[00] �̖��G����
#define OBJ_CORE_00_INVINCIBLE_TIME	(10)

// OBJ:�R�A[00] �̃_���[�W����
#define OBJ_CORE_00_DAMAGE_TIME	(10)

// OBJ:�R�A[00] �̌x�����鋗��
// OBJ:�R�A[00] �̌x���ɂ����鎞��
// OBJ:�R�A[00] �̌x��SE
#define OBJ_CORE_00_WARNING_DISTANCE	(40.0f)
#define OBJ_CORE_00_WARNING_TIME		(40)
#define OBJ_CORE_00_WARNING_SE			(SOUND_LABEL_SE_WARNING_000)

// OBJ:�R�A[00] �̃Q�[�����̎��
#define OBJ_CORE_00_GAME_TYPE		(0)
// OBJ:�R�A[00] �̃`���[�g���A�����̎��
#define OBJ_CORE_00_TUTORIAL_TYPE	(1)

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:�R�A[00] �̎�ޖ��̏��\����
typedef struct
{
	int			nHP;			// HP
	SOUND_LABEL	damageSE;		// �_���[�WSE
	SOUND_LABEL destroySE;		// �j��SE
	int			nExplosionType;	// �����̎��
	HitTestSet	hitTestSet;		// �����蔻��ݒ���
	Parts3DSet	partsSet;		// ���i�ݒ���(3D)
	Motion3DSet motionSet;		// ���[�V�����ݒ���
}Obj_core_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�R�A[00] �̃p�����[�^�[�̏���������
void InitParameterObj_core_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureObj_core_00	// OBJ:�R�A[00] �̃e�N�X�`���ւ̃|�C���^
					[OBJ_CORE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshObj_core_00		// OBJ:�R�A[00] �̃��b�V��(���_���)�ւ̃|�C���^
					[OBJ_CORE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
LPD3DXBUFFER		g_pBuffMatObj_core_00	// OBJ:�R�A[00] �̃}�e���A���ւ̃|�C���^
					= NULL;
DWORD				g_aNumMatObj_core_00	// OBJ:�R�A[00] �̃}�e���A���̐�
					[OBJ_CORE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
D3DXMATRIX			g_aMtxWorldObj_core_00	// OBJ:�R�A[00] �̃��[���h�}�g���b�N�X
					[PARTS_3D_MAX];
Obj_core_00			g_obj_core_00;			// OBJ:�R�A[00] �̏��
Obj_core_00Type		g_aObj_core_00Type		// OBJ:�R�A[00] �̎�ޖ��̏��
					[OBJ_CORE_00_TYPE_NUM];

//========== *** OBJ:�R�A[00] �̏����擾 ***
Obj_core_00 *GetObj_core_00(void) 
{
	return &g_obj_core_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_core_00�֐� - OBJ:�R�A[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_core_00(void) 
{
	Obj_core_00	*pObj	// OBJ:�R�A[00] �̏��̃|�C���^
				= &g_obj_core_00;

	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���݂̈ʒu
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����

	// ���݂̃��[�h�ɉ�����
	if (GetMode() == MODE_GAME_00) 
	{// ��ނ��Q�[�����̂��̂ɐݒ�
		pObj->nType = OBJ_CORE_00_GAME_TYPE;
	}
	else if (GetMode() == MODE_TUTORIAL_00) 
	{// ��ނ��`���[�ƃ��A�����̂��̂ɐݒ�
		pObj->nType = OBJ_CORE_00_TUTORIAL_TYPE;
	}

	pObj->bUse					= true;								// �g�p����Ă��邩�t���O
	pObj->nHP					=									// HP
	pObj->nHPMax				= 0;								// HP�̏��
	pObj->nCounterInvincible	= 0;								// ���G�J�E���^�[
	pObj->nCounterDamage		= 0;								// �_���[�W�J�E���^�[
	pObj->nCounterWarning		= 0;								// �x���J�E���^�[
	pObj->bWarning				= false;							// �x���t���O
	pObj->partsInfo				= {};								// ���i�Ǘ�
	pObj->motion				= OBJ_CORE_00_MOTION_WAIT;			// ���[�V����
}

//========================================
// MotionSetObj_core_00�֐� - OBJ:�R�A[00] �̃��[�V�����ݒ菈�� -
//========================================
void MotionSetObj_core_00(void)
{
	Obj_core_00		*pObj		// OBJ:�R�A[00] �̏��̃|�C���^
					= &g_obj_core_00;
	Obj_core_00Type	*pType		// OBJ:�R�A[00] �̎�ޖ��̏��̃|�C���^
					= &g_aObj_core_00Type[pObj->nType];
	Motion3D		*pMotion	// ���݂̃��[�V�����̏��̃|�C���^
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
// LoadObj_core_00�֐� - OBJ:�R�A[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_core_00(void)
{
	FILE			*pFile;		// �t�@�C���|�C���^
	char			aDataSearch	// �f�[�^�����p
					[TXT_MAX];
	Obj_core_00Type	*pObjType	// OBJ:�R�A[00] �̎�ޖ��̏��
					= g_aObj_core_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_CORE_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:�R�A[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))			{ pObjType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "HIT_POINT:"))		{ fscanf(pFile, "%d", &pObjType->nHP); }			// HP
				else if (!strcmp(aDataSearch, "DAMAGE_SE:"))		{ fscanf(pFile, "%d", &pObjType->damageSE); }		// �_���[�WSE
				else if (!strcmp(aDataSearch, "DESTROY_SE:"))		{ fscanf(pFile, "%d", &pObjType->destroySE); }		// �j��SE
				else if (!strcmp(aDataSearch, "EXPLOSION_TYPE:"))	{ fscanf(pFile, "%d", &pObjType->nExplosionType); }	// �����̎��
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// �����蔻��ݒ���
				else if (!strcmp(aDataSearch, "PARTSSET"))			{ LoadParts3DSet(pFile, &pObjType->partsSet); }		// ���i�ݒ���
				else if (!strcmp(aDataSearch, "MOTIONSET"))			{ LoadMotion3DSet(pFile, &pObjType->motionSet); }	// ���[�V�����ݒ���
			}
		}
	}
}

//========================================
// InitObj_core_00�֐� - OBJ:�R�A[00] �̏��������� -
//========================================
void InitObj_core_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < OBJ_CORE_00_TYPE_NUM; nCntType++)
	{
		Obj_core_00Type	*pType	// OBJ:�R�A[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_core_00Type[nCntType];

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
				&g_pBuffMatObj_core_00,
				NULL,
				&g_aNumMatObj_core_00[nCntType][nCntParts],
				&g_aMeshObj_core_00[nCntType][nCntParts]);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObj_core_00->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_core_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_core_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// �p�����[�^�[�̏���������
	InitParameterObj_core_00();

	Obj_core_00		*pObj	// OBJ:�R�A[00] �̏��̃|�C���^
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// OBJ:�R�A[00] �̎�ޖ��̏��̃|�C���^
					= &g_aObj_core_00Type[pObj->nType];

	// ���i(3D)�̃g�����X�t�H�[����������
	InitParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// HP��������
	pObj->nHP =
	pObj->nHPMax = pType->nHP;
}

//========================================
// UninitObj_core_00�֐� - OBJ:�R�A[00] �̏I������ -
//========================================
void UninitObj_core_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_CORE_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			if (g_aMeshObj_core_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshObj_core_00[nCntType][nCntParts]->Release();
				g_aMeshObj_core_00[nCntType][nCntParts] = NULL;
			}

			// �e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
				if (g_aTextureObj_core_00[nCntType][nCntParts][nCntMat] != NULL)
				{
					g_aTextureObj_core_00[nCntType][nCntParts][nCntMat]->Release();
					g_aTextureObj_core_00[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_core_00 != NULL)
	{
		g_pBuffMatObj_core_00->Release();
		g_pBuffMatObj_core_00 = NULL;
	}
}

//========================================
// UpdateObj_core_00�֐� - OBJ:�R�A[00] �̍X�V���� -
//========================================
void UpdateObj_core_00(void)
{
	Obj_core_00		*pObj	// OBJ:�R�A[00] �̏��̃|�C���^
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
					= &g_aObj_core_00Type[pObj->nType];

	if (pObj->bWarning) 
	{// �x���t���O���^�̎��A
		float fNotRed =	// �ԓx
			1.0f - ((float)Count(&pObj->nCounterWarning, 0, OBJ_CORE_00_WARNING_TIME, NULL, 0, COUNT_TYPE_TURN) /
			(float)OBJ_CORE_00_WARNING_TIME);

		// ��(3D)�̐F��ݒ�
		SetColorLight3D({ 255,(int)(255 * fNotRed),(int)(255 * fNotRed),255 });

		if (pObj->nCounterWarning <= 0) 
		{// �x���J�E���^�[��0�ȉ��̎��A
			pObj->bWarning = false;	// �x���t���O���U�ɂ���
		}
	}
	else if (GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL)
	{// [�x���t���O���U] & MD:�Q�[�����[00] �̏�Ԃ��ʏ�̎��A
		Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^���擾
						= GetChr_enemy_00();

		for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
		{
			if (!pChr->bUse)
			{// �J�E���g��CHR:�G[00] ���g�p����Ă��Ȃ����A
				continue;	// �J��Ԃ�������܂�Ԃ�
			}

			float fDistance = FindDistanceLookDown(pObj->pos, pChr->pos);	// 2�_�̋������擾

			if (fDistance <= OBJ_CORE_00_WARNING_DISTANCE)
			{// 2�_�̋������x�����鋗���ȉ��̎��A
				pObj->bWarning = true;				// �x���t���O��^�ɂ���
				PlaySound(OBJ_CORE_00_WARNING_SE);	// �x��SE���Đ�
				break;
			}
		}
	}

	// ���i(3D)�̃g�����X�t�H�[�����擾
	GetParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// ���[�V�����̐ݒ菈��
	MotionSetObj_core_00();

	// ���[�V�����̍X�V����
	if (UpdateMotion3D(
		pObj->partsInfo.nMotion,
		g_aObj_core_00Type[pObj->nType].motionSet,
		&pObj->partsInfo,
		g_aObj_core_00Type[pObj->nType].partsSet))
	{// ���[�V���������[�v�ɒB�������A
		Motion3D	*pMotion	// ���݂̃��[�V�����̏��̃|�C���^
					= &pType->motionSet.aMotion[pObj->motion];
		if (!pMotion->bLoop)
		{// ���݂̃��[�V�����̃��[�v�t���O���U�̎��A
			pObj->motion = OBJ_CORE_00_MOTION_WAIT;	// ���[�V������ҋ@�ɐݒ�
		}
	}

	if (pObj->nCounterInvincible > 0)
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		pObj->nCounterInvincible--;	// ���G�J�E���^�[�����Z����
	}
	if (pObj->nCounterDamage > 0)
	{// �_���[�W�J�E���^�[��0�������Ă��鎞�A
		pObj->nCounterDamage--;	// �_���[�W�J�E���^�[�����Z����
	}
}

//========================================
// DrawObj_core_00�֐� - OBJ:�R�A[00] �̕`�揈�� -
//========================================
void DrawObj_core_00(void)
{
	Obj_core_00			*pObj				// OBJ:�R�A[00] �̏��̃|�C���^
						= &g_obj_core_00;

	if (!pObj->bUse)
	{// �g�p����Ă���t���O���U�̎��A
		return;	// �������I������
	}

	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX			mtxSelf;			// �{�̂̃}�g���b�N�X

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_core_00->GetBufferPointer();

	bool bLighting =	// ���C�e�B���O�t���O
		(GetMd_game_00()->state == MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM)
		||
		(GetMd_game_00()->state == MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION) ? false : true;
	float fBrightnessTemp;	// ���邳��ۑ�

	if (!bLighting)
	{// ���C�e�B���O�t���O���U�̎��A
		fBrightnessTemp = GetLight3DControl()->fBrightness;	// ���邳��ۑ�
		SetBrightnessLight3D(1.0f);							// ���邳��ݒ�
	}

	// �{�̂̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxSelf);

	// �{�̂̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// �{�̂̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	Obj_core_00Type	*pType	// OBJ:�R�A[00] �̎�ޖ��̏��̃|�C���^
					= &g_aObj_core_00Type[pObj->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// ���i(3D)�̏��̃|�C���^
					= &pObj->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// �e�}�g���b�N�X

		// ���i�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMtxWorldObj_core_00[nCntParts]);

		// ���i�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_core_00[nCntParts], &g_aMtxWorldObj_core_00[nCntParts], &mtxRot);

		// ���i�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_core_00[nCntParts], &g_aMtxWorldObj_core_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
			mtxParent = g_aMtxWorldObj_core_00[pPartsType->nParent];
		}
		else
		{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
			mtxParent = mtxSelf;
		}

		// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(
			&g_aMtxWorldObj_core_00[nCntParts],
			&g_aMtxWorldObj_core_00[nCntParts],
			&mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObj_core_00[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_core_00[pObj->nType][nCntParts]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_CORE_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pObj->nCounterDamage / (float)OBJ_CORE_00_DAMAGE_TIME))),
				255
			});

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_core_00[pObj->nType][nCntParts][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_core_00[pObj->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}

	if (!bLighting)
	{// ���C�e�B���O�t���O���U�̎��A
		SetBrightnessLight3D(fBrightnessTemp);	// ���邳�����ɖ߂�
	}
}

//========================================
// CollisionObj_core_00�֐� - OBJ:�R�A[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionObj_core_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	if (pObjCollision->bHit)
	{// ������t���O���^�̎��A
		return -1;	// �������I������
	}

	Obj_core_00		*pObj	// OBJ:�R�A[00] �̏��̃|�C���^
					= &g_obj_core_00;
	int				nIndex	// OBJ:�R�A[00] �̕Ԃ��̔ԍ�
					= -1;
	Obj_core_00Type	*pType	// OBJ:�R�A[00] �̎�ޖ��̏��̃|�C���^
					= &g_aObj_core_00Type[pObj->nType];

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
// DamageObj_core_00�֐� - OBJ:�R�A[00] �̃_���[�W���� -
//========================================
void DamageObj_core_00(int nDamage)
{
	if ((GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL) || (nDamage == 0))
	{// OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���̎��A
		return;	// �������I������
	}

	Obj_core_00		*pObj	// OBJ:�R�A[00] �̏��̃|�C���^
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// OBJ:�R�A[00] �̎�ޖ��̏��̃|�C���^
					= &g_aObj_core_00Type[pObj->nType];

	if (pObj->nCounterInvincible > 0)
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		return;	// �������I������
	}

	nDamage *= fDamageRand();	// �����␳
	pObj->nHP -= nDamage;		// HP����_���[�W�����Z
	pObj->nCounterDamage 		// �_���[�W�J�E���^�[��ݒ�
		= OBJ_CORE_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:�_���[�W[00] �̐ݒ菈��
		pObj->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);
	PlaySound(pType->damageSE);	// �_���[�WSE���Đ�

	if (pObj->nHP <= 0)
	{// HP��0�ȉ��̎��A
		pObj->nHP = 0;	// HP��0�ɂ���
	}
	else
	{// HP��0�������Ă��鎞�A���G�J�E���^�[��ݒ�
		pObj->nCounterInvincible = OBJ_CORE_00_INVINCIBLE_TIME;
	}
}

//========================================
// DestroyObj_core_00�֐� - OBJ:�R�A[00] �̔j�󏈗� -
//========================================
void DestroyObj_core_00(void) 
{
	Obj_core_00		*pObj	// OBJ:�R�A[00] �̏��̃|�C���^
					= &g_obj_core_00;
	Obj_core_00Type	*pType	// OBJ:�R�A[00] �̎�ޖ��̏��̃|�C���^
					= &g_aObj_core_00Type[pObj->nType];

	pObj->bUse = false;				// �g�p����Ă��Ȃ���Ԃɂ���
	SetEff_explosion_00(			// EFF;����[00] �̐ݒ菈��
		pObj->pos,
		pType->nExplosionType);
	PlaySound(pType->destroySE);	// �j��SE���Đ�
}