//========================================
// 
// ���f���֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** model.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//****************************************
// �O���[�o���ϐ��錾
//****************************************
// ���f���֘A�̃e�N�X�`���ԍ�
static int g_aTextureIdxModel[MODELTYPE_MAX][MATERIAL3D_MAX] = {};
// ���f���֘A�̃��b�V��(���_���)�ւ̃|�C���^
static LPD3DXMESH g_aMeshModel[MODELTYPE_MAX] = {};
// ���f���֘A�̃}�e���A���ւ̃|�C���^
static LPD3DXBUFFER g_pBuffMatModel[MODELTYPE_MAX] = {};
// ���f���֘A�̃}�e���A���̐�
static DWORD g_aNumMatModel[MODELTYPE_MAX] = {};
// ���f���̊Ǘ����
static ModelInfo g_modelSetUpInfo;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitModel�֐� - ���f���̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitModel(void) {

}

//========================================
// UninitModel�֐� - ���f���̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitModel(void) {
	for (int nCntType = 0; nCntType < MODELTYPE_MAX; nCntType++) {
		// ���b�V���̔j��
		if (g_aMeshModel[nCntType] != NULL) {
			g_aMeshModel[nCntType]->Release();
			g_aMeshModel[nCntType] = NULL;
		}

		// �}�e���A���̔j��
		if (g_pBuffMatModel[nCntType] != NULL) {
			g_pBuffMatModel[nCntType]->Release();
			g_pBuffMatModel[nCntType] = NULL;
		}
	}
}

//========================================
// DrawModel�֐� - ���f���̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawModel(void) {
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// ���f���̐ݒ萔���`��
	for (int nCntModel = 0; nCntModel < g_modelSetUpInfo.nModelSetNum; nCntModel++) {
		// ��ޔԍ���ݒ肵�A�͈͊O�Ȃ�܂�Ԃ�
		int nType = g_modelSetUpInfo.aModelSet[nCntModel].nModelIdx;
		if (nType < 0 || nType >= MODELTYPE_MAX) {
			assert(false);
			continue;
		}

		// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_modelSetUpInfo.aModelSet[nCntModel].mtx);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatModel[nType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatModel[nType]; nCntMat++) {
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D, g_modelSetUpInfo.aModelSet[nCntModel].col, g_modelSetUpInfo.aModelSet[nCntModel].bLighting);

			// �e�N�X�`���̐ݒ�
			SetTexture(pDevice, g_aTextureIdxModel[nType][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshModel[nType]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// LoadModel�֐� - ���f���̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
int LoadModel(char *pPath) {
	// ���ɓǂݍ��܂�Ă��郂�f�����m�F
	for (int nCntType = 0; nCntType < g_modelSetUpInfo.nModelTypeNum; nCntType++) {
		if (!strcmp(pPath, g_modelSetUpInfo.aLoadModelPath[nCntType])) 
		{// ���ɓ����p�X�̃��f����ǂݍ���ł������A
			return nCntType;	// ��ޔԍ���Ԃ�
		}
	}

	if (g_modelSetUpInfo.nModelTypeNum >= MODELTYPE_MAX) 
	{// ���Ƀ��f���̎�ސ����ő�ɒB���Ă������A
		assert(false);
		return -1;
	}

	// ���f���̃p�X��ۑ�
	sprintf(g_modelSetUpInfo.aLoadModelPath[g_modelSetUpInfo.nModelTypeNum], "%s", pPath);

	// ��ޔԍ�
	int nType = g_modelSetUpInfo.nModelTypeNum;
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[nType],
		NULL,
		&g_aNumMatModel[nType],
		&g_aMeshModel[nType]);

	if (g_pBuffMatModel[nType] == NULL) 
	{// NULL���������A
		char aMessage[TXT_MAX] = "";
		sprintf(aMessage, "���f���̓ǂݍ��݂Ɏ��s���܂����B\n%s", pPath);
		MessageBox(*GetWindowHandle(), aMessage, "ERROR!", MB_ICONWARNING);
		return -1;
	}
	else {
		// �}�e���A�����ɑ΂���|�C���^���擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatModel[nType]->GetBufferPointer();

		// �}�e���A������u���������A�����ő吔���z���Ă����琧��
		int nNumMat = (int)g_aNumMatModel[nType];
		if (nNumMat >= MATERIAL3D_MAX) {
			assert(false);
			nNumMat = MATERIAL3D_MAX - 1;
			g_aNumMatModel[nType] = (DWORD)nNumMat;
		}

		// �}�e���A�������ǂݍ���
		for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++) {
			assert(nCntMat < MATERIAL3D_MAX);

			if (pMat[nCntMat].pTextureFilename != NULL)
			{// �e�N�X�`���̃t�@�C������NULL�łȂ����A
				// �e�N�X�`����ǂݍ���
				g_aTextureIdxModel[nType][nCntMat] = LoadTexture(pMat[nCntMat].pTextureFilename);
			}
			else
			{// �e�N�X�`���̃t�@�C������NULL�̎��A
				// �e�N�X�`���ԍ���-1�Ŏw��
				g_aTextureIdxModel[nType][nCntMat] = -1;
			}
		}

		// ���f���̐������Z
		g_modelSetUpInfo.nModelTypeNum++;
	}

	// ��ޔԍ���Ԃ�
	return nType;
}

//========================================
// SetModel�֐� - ���f���̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetModel(int nModelIdx, D3DXMATRIX mtx, Color col, bool bLighting) {
	if (nModelIdx < 0 || nModelIdx >= g_modelSetUpInfo.nModelTypeNum) 
	{// ���f���̎�ޔԍ����͈͊O�̎��A
		return;	// �������I��
	}
	else if (g_modelSetUpInfo.nModelSetNum >= MODEL_MAX)
	{// ���f���̐ݒ萔���ő吔�ɒB���Ă��鎞�A
		assert(false);
		return;	// �������I��
	}

	// ���f���̐ݒ������
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].nModelIdx = nModelIdx;
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].mtx = mtx;
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].col = col;
	g_modelSetUpInfo.aModelSet[g_modelSetUpInfo.nModelSetNum].bLighting = bLighting;

	// ���f���̐ݒ萔�����Z
	g_modelSetUpInfo.nModelSetNum++;
}

//========================================
// GetModelInfo�֐� - ���f���̊Ǘ����擾 -
// Author:RIKU NISHIMURA
//========================================
ModelInfo *GetModelInfo(void) {
	return &g_modelSetUpInfo;
}