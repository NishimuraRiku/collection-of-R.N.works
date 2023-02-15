//========================================
// 
// �I�u�W�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** object3D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// �}�N����`
//****************************************
// �I�u�W�F�N�g3D�̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJECT3D_TYPE_DATA_FILE_PATH "data\\OBJECT3D_DATA.txt"
// �I�u�W�F�N�g3D�̎�ނ̍ő吔
#define OBJECT3D_TYPE_MAX (64)
// �I�u�W�F�N�g3D�̒����_�̑��Έʒu�̈ړ��{��
#define OBJECT3D_RELATIVE_POS_R_MOVE_DIAMETER (0.25f)

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �擾 ***
// �I�u�W�F�N�g3D�̃p�����[�^�[�̏����l��Ԃ�
Object3D InitParameterObject3D(void);
//========== *** �X�V ***
// �I�u�W�F�N�g3D�̃��[�V�����ݒ菈��
void MotionSetObject3D(Object3D *pObj3D);

//****************************************
// �O���[�o���ϐ��錾
//****************************************
// �I�u�W�F�N�g3D�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_aTextureObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX][MATERIAL_3D_MAX] = {};
// �I�u�W�F�N�g3D�̃��b�V��(���_���)�ւ̃|�C���^
LPD3DXMESH g_aMeshObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX] = {};
// �I�u�W�F�N�g3D�̃}�e���A���ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX] = {};
// �I�u�W�F�N�g3D�̃}�e���A���̐�
DWORD g_aNumMatObject3D[OBJECT3D_TYPE_MAX][PARTS_3D_MAX] = {};
// �I�u�W�F�N�g3D�̃��[���h�}�g���b�N�X
D3DXMATRIX g_aMtxWorldObject3D[PARTS_3D_MAX];
// �I�u�W�F�N�g3D�̏��
Object3D g_aObject3D[OBJECT3D_MAX];
// �I�u�W�F�N�g3D�̊Ǘ����
Object3DControl g_object3DControl;
// �I�u�W�F�N�g3D�̎�ޖ��̏��
Object3DType g_aObject3DType[OBJECT3D_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================
//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================
//========================================
// InitParameterObject3D�֐� - �I�u�W�F�N�g3D�̃p�����[�^�[�̏����l��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
Object3D InitParameterObject3D(void)
{
	return{
		INITD3DXVECTOR3,	// ���݂̈ʒu
		INITD3DXVECTOR3,	// ����
		0,					// ���
		false,				// �g�p����Ă��邩�t���O
		{},					// ���i�Ǘ�
		0,					// ���[�V����
	};
}

//============================================================
//--------------------| *** �X�V *** |------------------------
//============================================================
//========================================
// MotionSetObject3D�֐� - �I�u�W�F�N�g3D�̃��[�V�����ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void MotionSetObject3D(Object3D *pObj3D)
{
	// �I�u�W�F�N�g3D�̎�ޖ��̏��̃|�C���^
	Object3DType *pType = &g_aObject3DType[pObj3D->nType];
	// ���݂̃��[�V�����̏��̃|�C���^
	Motion3D *pMotion = &pType->motionSet.aMotion[pObj3D->nMotion];

	// ���[�V�����ݒ�
	SetMotion3D(
		&pObj3D->partsInfo,
		pObj3D->nMotion);
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
// InitObject3D�֐� - �I�u�W�F�N�g3D�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitObject3D(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// �}�e���A���ւ̃|�C���^
	D3DXMATERIAL *pMat;
	// �I�u�W�F�N�g3D�̊Ǘ����̃|�C���^
	Object3DControl *pObj3DCtl = &g_object3DControl;

	for (int nCntType = 0; nCntType < pObj3DCtl->nTypeNum; nCntType++)
	{
		// �I�u�W�F�N�g3D�̎�ޖ��̏��̃|�C���^
		Object3DType *pType = &g_aObject3DType[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			// ���i(3D)�̎�ޖ��̏��\����
			Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_pBuffMatObject3D[nCntType][nCntParts],
				NULL,
				&g_aNumMatObject3D[nCntType][nCntParts],
				&g_aMeshObject3D[nCntType][nCntParts]);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObject3D[nCntType][nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObject3D[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObject3D[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// �N���A����
	ClearObject3D();
}

//========================================
// UninitObject3D�֐� - �I�u�W�F�N�g3D�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitObject3D(void)
{
	for (int nCntType = 0; nCntType < OBJECT3D_TYPE_MAX; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// ���b�V���̔j��
			if (g_aMeshObject3D[nCntType][nCntParts] != NULL)
			{
				g_aMeshObject3D[nCntType][nCntParts]->Release();
				g_aMeshObject3D[nCntType][nCntParts] = NULL;
			}

			// �}�e���A���̔j��
			if (g_pBuffMatObject3D[nCntType][nCntParts] != NULL)
			{
				g_pBuffMatObject3D[nCntType][nCntParts]->Release();
				g_pBuffMatObject3D[nCntType][nCntParts] = NULL;
			}
		}
	}
}

//========================================
// UpdateObject3D�֐� - �I�u�W�F�N�g3D�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateObject3D(void)
{
	// �I�u�W�F�N�g3D�̊Ǘ����̃|�C���^
	Object3DControl *pObj3DCtl = &g_object3DControl;
	// �I�u�W�F�N�g3D�̏��̃|�C���^
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		// �I�u�W�F�N�g3D�̎�ޖ��̏��̃|�C���^
		Object3DType *pType = &g_aObject3DType[pObj3D->nType];
	}
}

//========================================
// DrawObject3D�֐� - �I�u�W�F�N�g3D�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawObject3D(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxSelf;				// �{�̂̃}�g���b�N�X

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �I�u�W�F�N�g3D�̏��̃|�C���^
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		if (!pObj3D->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			continue;	// �J��Ԃ�������܂�Ԃ�
		}

		// �{�̂̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxSelf);

		// �{�̂̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj3D->rot.y, pObj3D->rot.x, pObj3D->rot.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

		// �{�̂̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pObj3D->pos.x, pObj3D->pos.y, pObj3D->pos.z);
		D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

		// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

		// �I�u�W�F�N�g3D�̎�ޖ��̏��̃|�C���^
		Object3DType *pType = &g_aObject3DType[pObj3D->nType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			// ���i(3D)�̏��̃|�C���^
			Parts3D *pParts = &pObj3D->partsInfo.aParts[nCntParts];
			// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
			Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];
			D3DXMATRIX mtxParent;	// �e�}�g���b�N�X

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObject3D[pObj3D->nType][nCntParts]->GetBufferPointer();

			// ���i�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMtxWorldObject3D[nCntParts]);

			// ���i�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxRot);

			// ���i�̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
			D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxTrans);

			if (pPartsType->nParent != -1)
			{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
				mtxParent = g_aMtxWorldObject3D[pPartsType->nParent];
			}
			else
			{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
				mtxParent = mtxSelf;
			}

			// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(
				&g_aMtxWorldObject3D[nCntParts],
				&g_aMtxWorldObject3D[nCntParts],
				&mtxParent);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObject3D[nCntParts]);

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObject3D[pObj3D->nType][nCntParts]; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aTextureObject3D[pObj3D->nType][nCntParts][nCntMat]);

				// �|���S��(�p�[�c)�̕`��
				g_aMeshObject3D[pObj3D->nType][nCntParts]->DrawSubset(nCntMat);
			}
		}
	}
}

//============================================================
//--------------------| *** �`�� *** |------------------------
//============================================================
//========================================
// ExcDrawObject3D�֐� - �I�u�W�F�N�g3D�̗�O�`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void ExcDrawObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, Color col)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxSelf;				// �{�̂̃}�g���b�N�X

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �{�̂̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxSelf);

	// �{�̂̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// �{�̂̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	// �I�u�W�F�N�g3D�̎�ޖ��̏��̃|�C���^
	Object3DType *pType = &g_aObject3DType[nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
		Parts3DType *pPartsType = &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// �e�}�g���b�N�X

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObject3D[nType][nCntParts]->GetBufferPointer();

		// ���i�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMtxWorldObject3D[nCntParts]);

		// ���i�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pPartsType->fixedRelativeRot.y, pPartsType->fixedRelativeRot.x, pPartsType->fixedRelativeRot.z);
		D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxRot);

		// ���i�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pPartsType->fixedRelativePos.x, pPartsType->fixedRelativePos.y, pPartsType->fixedRelativePos.z);
		D3DXMatrixMultiply(&g_aMtxWorldObject3D[nCntParts], &g_aMtxWorldObject3D[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
			mtxParent = g_aMtxWorldObject3D[pPartsType->nParent];
		}
		else
		{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
			mtxParent = mtxSelf;
		}

		// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(
			&g_aMtxWorldObject3D[nCntParts],
			&g_aMtxWorldObject3D[nCntParts],
			&mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObject3D[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObject3D[nType][nCntParts]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, col);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObject3D[nType][nCntParts][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObject3D[nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================
//========================================
// LoadObject3D�֐� - �I�u�W�F�N�g3D�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadObject3D(void)
{
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch[TXT_MAX];	// �f�[�^�����p
	// �I�u�W�F�N�g3D�̎�ޖ��̏��
	Object3DType *pObj3DType = g_aObject3DType;
	// �I�u�W�F�N�g3D�̊Ǘ����̃|�C���^
	Object3DControl *pObj3DCtl = &g_object3DControl;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJECT3D_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// ��ސ���������
	pObj3DCtl->nTypeNum = 0;

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
		{
			// �I�u�W�F�N�g3D�̎�ޖ��̏��̓ǂݍ��݃��[�v
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (!strcmp(aDataSearch, "TYPE_END"))
				{ 
					pObj3DType++; pObj3DCtl->nTypeNum++; break; }		// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "PARTSSET"))
				{// ���i�ݒ���
					LoadParts3DSet(pFile, &pObj3DType->partsSet);
				}
				else if (!strcmp(aDataSearch, "MOTIONSET"))
				{// ���[�V�����ݒ���
					LoadMotion3DSet(pFile, &pObj3DType->motionSet);
				}
			}
		}
	}
}

//============================================================
//--------------------| *** �ݒ� *** |------------------------
//============================================================
//========================================
// GetObject3D�֐� - �I�u�W�F�N�g3D�̏����擾 -
// Author:RIKU NISHIMURA
//========================================
Object3D *GetObject3D(void)
{
	return g_aObject3D;
}

//========================================
// GetObject3DControl�֐� - �I�u�W�F�N�g3D�̊Ǘ������擾 -
// Author:RIKU NISHIMURA
//========================================
Object3DControl *GetObject3DControl(void)
{
	return &g_object3DControl;
}

//========================================
// GetObject3DType�֐� - �I�u�W�F�N�g3D�̎�ޖ��̏����擾 -
// Author:RIKU NISHIMURA
//========================================
Object3DType *GetObject3DType(void)
{
	return g_aObject3DType;
}

//============================================================
//--------------------| *** �ݒ� *** |------------------------
//============================================================
//========================================
// SetObject3D�֐� - �I�u�W�F�N�g3D�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// �I�u�W�F�N�g3D�̏��̃|�C���^
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		if (pObj3D->bUse) 
		{
			continue;
		}

		pObj3D->pos = pos;		// �ʒu�̑��
		pObj3D->rot = rot;		// �����̑��
		pObj3D->nType = nType;	// ��ނ̑��
		pObj3D->bUse = true;	// �g�p����Ă����Ԃɂ���

		break;
	}
}

//============================================================
//--------------------| *** ��� *** |------------------------
//============================================================
//========================================
// ClearObject3D�֐� - �I�u�W�F�N�g3D�̃N���A���� -
// Author:RIKU NISHIMURA
//========================================
void ClearObject3D(void) 
{
	// �I�u�W�F�N�g3D�̏��̃|�C���^
	Object3D *pObj3D = g_aObject3D;

	for (int nCntObj3D = 0; nCntObj3D < OBJECT3D_MAX; nCntObj3D++, pObj3D++)
	{
		// �I�u�W�F�N�g3D�̎�ޖ��̏��̃|�C���^
		Object3DType *pType = &g_aObject3DType[pObj3D->nType];

		// �p�����[�^�[�̏���������
		*pObj3D = InitParameterObject3D();

		// ���i(3D)�̃g�����X�t�H�[����������
		InitParts3DTransform(&pObj3D->partsInfo, &pType->partsSet);
	}
}