//========================================
// 
// CHR:���f���̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_model.cpp ***
//========================================
#include <stdio.h>
#include "main.h"
// Character
#include "chr_model.h"	// CHR:���f��
// R.N.Lib
#include "../R.N.Lib/RNmain.h"

//****************************************
// �}�N����`
//****************************************
// CHR:���f���̎�ނ̍ő吔
#define CHR_MODEL_TYPE_MAX (64)
// CHR:���f���̏�������
#define CHR_MODEL_INIT_ROT D3DXVECTOR3(0.0f,D3DX_PI,0.0f)
// CHR:���f���̒����_�̑��Έʒu�̈ړ��{��
#define CHR_MODEL_RELATIVE_POS_R_MOVE_DIAMETER (0.25f)

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �X�V ***
// CHR:���f���̃��[�V�����ݒ菈��
void MotionSetChr_model(void);
//========== *** ���o�� ***
// CHR:���f���̃��f���ǂݍ��ݏ���
void LoadModelSetChr_model(void);
//========== *** �擾 ***
// CHR:���f���̃p�����[�^�[�̏����l��Ԃ�
Chr_model InitParameterChr_model(void);
// CHR:���f���̊Ǘ����̃p�����[�^�[�̏����l��Ԃ�
Chr_modelControl InitParameterChr_modelControl(void);

//****************************************
// �O���[�o���錾
//****************************************
// CHR:���f���̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_aTextureChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX][MATERIAL_3D_MAX] = {};
// CHR:���f���̃��b�V��(���_���)�ւ̃|�C���^
LPD3DXMESH g_aMeshChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX] = {};
// CHR:���f���̃}�e���A���ւ̃|�C���^
LPD3DXBUFFER g_aBuffMatChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX] = {};
// CHR:���f���̃}�e���A���̐�
DWORD g_aNumMatChr_model[CHR_MODEL_TYPE_MAX][PARTS_3D_MAX] = {};
// CHR:���f���̃��[���h�}�g���b�N�X
D3DXMATRIX g_aMtxWorldChr_model[PARTS_3D_MAX];
// CHR:���f���̏��
Chr_model g_chr_model;
// CHR:���f���̊Ǘ����
Chr_modelControl g_chr_modelControl;
// CHR:���f���̎�ޖ��̏��
Chr_modelType g_aChr_modelType[CHR_MODEL_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �X�V *** |------------------------
//============================================================
//========================================
// MotionSetChr_model�֐� - CHR:���f���̃��[�V�����ݒ菈�� -
//========================================
void MotionSetChr_model(void)
{
	// CHR:���f���̏��̃|�C���^
	Chr_model *pChr = &g_chr_model;
	// CHR:���f���̎�ޖ��̏��̃|�C���^
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// ���݂̃��[�V�����̏��̃|�C���^
	Motion3D *pMotion = &pType->motionSet.aMotion[pChr->nMotion];

	// ���[�V�����ݒ�
	SetMotion3D(
		&pChr->partsInfo,
		pChr->nMotion);
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================
//========================================
// LoadModelSetChr_model�֐� - CHR:���f���̃��f���ǂݍ��ݏ��� -
//========================================
void LoadModelSetChr_model(void) 
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL*pMat;	// �}�e���A���ւ̃|�C���^

	// CHR:���f���̏��̃|�C���^
	Chr_model *pChr = &g_chr_model;
	// CHR:���f���̎�ޖ��̏��̃|�C���^
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// CHR:���f���̊Ǘ����̃|�C���^
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	for (int nCntType = 0; nCntType < pChrCtl->nTypeNum; nCntType++) 
	{
		// CHR:���f���̎�ޖ��̏��̃|�C���^
		Chr_modelType *pType = &g_aChr_modelType[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			// ���i(3D)�̎�ޖ��̏��\�̃|�C���^
			Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aBuffMatChr_model[nCntType][nCntParts],
				NULL,
				&g_aNumMatChr_model[nCntType][nCntParts],
				&g_aMeshChr_model[nCntType][nCntParts]);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aBuffMatChr_model[nCntType][nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_model[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{// �t�@�C������NULL�łȂ����A
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureChr_model[nCntType][nCntParts][nCntMat]);
				}
				else 
				{// �t�@�C������NULL�̎��A
					// �e�N�X�`����NULL�w��
					g_aTextureChr_model[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================
//========================================
// InitParameterChr_model�֐� - CHR:���f���̃p�����[�^�[�̏����l��Ԃ� -
//========================================
Chr_model InitParameterChr_model(void)
{
	return{
		INITD3DXVECTOR3,			// �ʒu
		CHR_MODEL_INIT_ROT,			// ����
		0,							// ���
		InitParameterParts3DInfo(),	// ���i�Ǘ�
		g_chr_model.nMotion,		// ���[�V����
	};
}

//========================================
// InitParameterChr_modelControl�֐� - CHR:���f���̊Ǘ����̃p�����[�^�[�̏����l��Ԃ� -
//========================================
Chr_modelControl InitParameterChr_modelControl(void)
{
	return{
		1,		// ��ސ�
		false,	// ���f����ǂݍ��߂����t���O
	};
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================
//========================================
// InitChr_model�֐� - CHR:���f���̏��������� -
//========================================
void InitChr_model(void)
{
	// CHR:���f���̏��̃|�C���^
	Chr_model *pChr = &g_chr_model;
	// CHR:���f���̎�ޖ��̏��̃|�C���^
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// CHR:���f���̊Ǘ����̃|�C���^
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	{// �Ǘ����̃p�����[�^�̏�����
		bool bTemp = pChrCtl->bLoadModel;
		*pChrCtl = InitParameterChr_modelControl();
		pChrCtl->bLoadModel ^= bTemp;
	}
	
	// �p�����[�^�[�̏�����
	*pChr = InitParameterChr_model();

	// ���i(3D)�̃g�����X�t�H�[����������
	InitParts3DTransform(&pChr->partsInfo, &pType->partsSet);
}

//========================================
// UninitChr_model�֐� - CHR:���f���̏I������ -
//========================================
void UninitChr_model(void)
{
	for (int nCntType = 0; nCntType < CHR_MODEL_TYPE_MAX; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// ���b�V���̔j��
			if (g_aMeshChr_model[nCntType][nCntParts] != NULL)
			{
				g_aMeshChr_model[nCntType][nCntParts]->Release();
				g_aMeshChr_model[nCntType][nCntParts] = NULL;
			}

			// �}�e���A���̔j��
			if (g_aBuffMatChr_model[nCntType][nCntParts] != NULL)
			{
				g_aBuffMatChr_model[nCntType][nCntParts]->Release();
				g_aBuffMatChr_model[nCntType][nCntParts] = NULL;
			}
		}
	}
}

//========================================
// UpdateChr_model�֐� - CHR:���f���̍X�V���� -
//========================================
void UpdateChr_model(void)
{
	// CHR:���f���̏��̃|�C���^
	Chr_model *pChr = &g_chr_model;
	// CHR:���f���̎�ޖ��̏��̃|�C���^
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];
	// CHR:���f���̊Ǘ����̃|�C���^
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	// ��މ��Z/���Z
	pChr->nType += (int)GetKeyboardTrigger(DIK_2);
	pChr->nType -= (int)GetKeyboardTrigger(DIK_1);
	IntLoopControl(&pChr->nType, pChrCtl->nTypeNum, 0);	// ����

	// ���[�V�����ԍ����Z/���Z
	pChr->nMotion += (int)GetKeyboardTrigger(DIK_4);
	pChr->nMotion -= (int)GetKeyboardTrigger(DIK_3);
	IntLoopControl(&pChr->nMotion, pType->motionSet.nMotionNum, 0);	// ����

	// ���i(3D)�̃g�����X�t�H�[�����擾
	GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);

	// ���[�V�����̐ݒ菈��
	MotionSetChr_model();

	// ���[�V�����̍X�V����
	UpdateMotion3D(
		pChr->partsInfo.nMotion,
		g_aChr_modelType[pChr->nType].motionSet,
		&pChr->partsInfo,
		g_aChr_modelType[pChr->nType].partsSet);

	// �Փ˔���ɕK�v�ȏ����쐬
	CollisionInfo myCollInfo =
	{
		&pChr->pos,						// ���݂̈ʒu�̃|�C���^
		pChr->pos,						// �ߋ��̈ʒu
		NULL,							// �ړ��ʂ̃|�C���^
		&pChr->rot,						// ���݂̌����̃|�C���^
		pChr->rot,						// �ߋ��̌���
		&pChr->partsInfo,				// ���i�Ǘ����
		&pType->partsSet,				// ���i�ݒ���
		&pType->hitTestSet.aHitTest[0],	// �����蔻��̏��
		(COLLCHK_MODE)0,				// �Փ˃`�F�b�N�̃��[�h
	};

	// �����蔻��o�^����
	HitTestSignUp(myCollInfo);
}

//========================================
// DrawChr_model�֐� - CHR:���f���̕`�揈�� -
//========================================
void DrawChr_model(void)
{
	if (!g_chr_modelControl.bLoadModel)
	{
		return;	// ���f�����ǂݍ��߂Ă��Ȃ����A�������I������
	}

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();  
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxSelf;				// �{�̂̃}�g���b�N�X

	// CHR:���f���̏��̃|�C���^
	Chr_model *pChr = &g_chr_model;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �{�̂̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxSelf);

	// �{�̂̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pChr->rot.y, pChr->rot.x, pChr->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// �{�̂̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pChr->pos.x, pChr->pos.y, pChr->pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	// CHR:���f���̎�ޖ��̏��̃|�C���^
	Chr_modelType *pType = &g_aChr_modelType[pChr->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		// ���i(3D)�̏��̃|�C���^
		Parts3D *pParts = &pChr->partsInfo.aParts[nCntParts];		
		// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
		Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX mtxParent;	// �e�}�g���b�N�X

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aBuffMatChr_model[pChr->nType][nCntParts]->GetBufferPointer();

		// ���i�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMtxWorldChr_model[nCntParts]);

		if (pPartsType->nParent == -1)
		{// �e�ԍ���-1�̎��A
			// ���f���̃T�C�Y��ύX
			D3DXMatrixScaling(&g_aMtxWorldChr_model[nCntParts], pType->partsSet.fScale, pType->partsSet.fScale, pType->partsSet.fScale);
		}

		// ���i�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_model[nCntParts], &g_aMtxWorldChr_model[nCntParts], &mtxRot);

		// ���i�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_model[nCntParts], &g_aMtxWorldChr_model[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1) 
		{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
			mtxParent = g_aMtxWorldChr_model[pPartsType->nParent];
		}
		else 
		{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
			mtxParent = mtxSelf;
		}

		// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(
			&g_aMtxWorldChr_model[nCntParts],
			&g_aMtxWorldChr_model[nCntParts],
			&mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldChr_model[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_model[pChr->nType][nCntParts]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureChr_model[pChr->nType][nCntParts][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshChr_model[pChr->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================
//========================================
// LoadChr_model�֐� - CHR:���f���̓ǂݍ��ݏ��� -
//========================================
void LoadChr_model(char *pPath)
{
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch[TXT_MAX];	// �f�[�^�����p

	// CHR:���f���̎�ޖ��̏��
	Chr_modelType *pChrType = g_aChr_modelType;
	// CHR:���f���̊Ǘ����̃|�C���^
	Chr_modelControl *pChrCtl = &g_chr_modelControl;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(pPath, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A�������I������
		return;
	}

	pChrCtl->nTypeNum = 0;		// ��ސ���������
	pChrCtl->bLoadModel = true;	// �ǂݍ��߂����t���O��^�ɂ���

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))
		{// �ǂݍ��݂��I�� 
			fclose(pFile); 
			break;
		}

		if (aDataSearch[0] == '#')
		{// �܂�Ԃ�
			continue;
		}
		
		if (!strcmp(aDataSearch, "TYPE"))
		{// CHR:���f���̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (!strcmp(aDataSearch, "TYPE_END"))
				{// �ǂݍ��݂��I��
					pChrType++; 
					pChrCtl->nTypeNum++; 
					break;
				}
				else if (!strcmp(aDataSearch, "PARTSSET"))
				{// ���i�ݒ���
					LoadParts3DSet(pFile, &pChrType->partsSet);
				}
				else if (!strcmp(aDataSearch, "HITTESTSET"))
				{// �����蔻��ݒ��� 
					LoadHitTestSet(pFile, &pChrType->hitTestSet); 
				}
				else if (!strcmp(aDataSearch, "MOTIONSET"))
				{// ���[�V�����ݒ���
					LoadMotion3DSet(pFile, &pChrType->motionSet);
				}
			}
		}
	}

	// ���f���̓ǂݍ��ݏ���
	LoadModelSetChr_model();
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================
//========================================
// GetChr_model�֐� - CHR:���f���̏��̃|�C���^���擾 -
//========================================
Chr_model *GetChr_model(void)
{
	return &g_chr_model;
}

//========================================
// GetChr_modelControl�֐� - CHR:���f���̊Ǘ����̃|�C���^���擾 -
//========================================
Chr_modelControl *GetChr_modelControl(void)
{
	return &g_chr_modelControl;
}

//========================================
// GetChr_modelType�֐� - CHR:���f���̎�ޖ��̏����擾 -
//========================================
Chr_modelType *GetChr_modelType(void)
{
	return g_aChr_modelType;
}