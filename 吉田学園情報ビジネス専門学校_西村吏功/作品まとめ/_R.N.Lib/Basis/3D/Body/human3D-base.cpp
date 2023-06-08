//========================================
// 
// �q���[�}��(3D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** human3D-base.cpp ***
//========================================
#include "../Body/human3D-base.h"
#include "../Body/model-setup3D.h"
#include "../../Other/memory.h"
#include "../../Other/file.h"
#include "../../../RNmain.h"

//****************************************
// �O���[�o���ϐ��錾
//****************************************
static Human3DBase **g_ppHuman3DBase;	// �q���[�}��(3D)�x�[�X���
static Human3DBaseControl g_human3DBaseInfo;	// �q���[�}��(3D)�x�[�X�Ǘ����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UninitHuman3DBase�֐� - �q���[�}��(3D)�x�[�X�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitHuman3DBase(void) {
	// ���������
	for (int nCnt = 0; nCnt < g_human3DBaseInfo.nHumanBaseNum; nCnt++) {
		ReleaseMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[nCnt]);	// �Ǎ��σZ�b�g�A�b�v�p�X
		ReleaseMemory((void**)&g_ppHuman3DBase[nCnt]);							// �Z�b�g�A�b�v���
	}
	ReleaseMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath);	// �Ǎ��σZ�b�g�A�b�v�p�X
	ReleaseMemory((void**)&g_ppHuman3DBase);						// �Z�b�g�A�b�v���
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================

//========================================
// LoadHumanBase�֐� - �q���[�}��(3D)�x�[�X�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
int LoadHuman3DBase(char *pHumanBasePath, bool bReload, int nHumanBaseIdx, bool bSetUpLoad) {
	bool bLoadDone = false;	// �ǂݍ��ݍσt���O

	if (nHumanBaseIdx != -1)
	{// �ǂݍ��ރq���[�}��(3D)�x�[�X�ԍ����w�肳��Ă��鎞�A
		nHumanBaseIdx = nHumanBaseIdx;	// �q���[�}��(3D)�x�[�X�ԍ���ݒ�

		// �ǂݍ��ݍσt���O��^�ɂ���
		bLoadDone = true;
	}

	// ���ɓǂݍ��܂�Ă���q���[�}��(3D)�x�[�X���m�F
	for (int nCntType = 0; nCntType < g_human3DBaseInfo.nHumanBaseNum; nCntType++) {
		if (!strcmp(pHumanBasePath, g_human3DBaseInfo.ppLoadHumanBasePath[nCntType]))
		{// ���ɓ����p�X�̃q���[�}��(3D)�x�[�X��ǂݍ���ł������A
			// �ǂݍ��ݍσt���O��^�ɂ���
			bLoadDone = true;

			if (nHumanBaseIdx != -1)
			{// �ǂݍ��ރq���[�}��(3D)�x�[�X�ԍ����w�肳��Ă��鎞�A
				break;
			}
			else if (bReload)
			{// �ēǂݍ��݃t���O���^�̎��A
				nHumanBaseIdx = nCntType;
				break;
			}
			else
			{// �ēǂݍ��݃t���O���U�̎��A
				return nCntType;	// ��ޔԍ���Ԃ�
			}
		}
	}

	if (!bLoadDone)
	{// �ǂݍ��ݍς݂łȂ����A
		// �q���[�}��(3D)�x�[�X�������Z
		g_human3DBaseInfo.nHumanBaseNum++;

		// ������̃������Ċm��
		ReGetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath, sizeof(char*), g_human3DBaseInfo.nHumanBaseNum - 1, g_human3DBaseInfo.nHumanBaseNum);
		GetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[g_human3DBaseInfo.nHumanBaseNum - 1], sizeof(char), TXT_MAX);

		// �q���[�}��(3D)�x�[�X���̃������Ċm��
		ReGetMemory((void**)&g_ppHuman3DBase, sizeof(Human3DBase), g_human3DBaseInfo.nHumanBaseNum - 1, g_human3DBaseInfo.nHumanBaseNum);
	}

	// �q���[�}��(3D)�x�[�X�̃p�X��ۑ�
	sprintf(g_human3DBaseInfo.ppLoadHumanBasePath[nHumanBaseIdx], "%s", pHumanBasePath);

	char aDataSearch[TXT_MAX] = "";	// �f�[�^�����p
	FILE *pFile = fopen(pHumanBasePath, "r");	// ��ޖ��̏��̃f�[�^�t�@�C�����J��

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A�������I������
		assert(false);
		return 0;
	}

	while (DataSearch(pFile, aDataSearch, "END")) {
		Human3DBase *pHumanBase = g_ppHuman3DBase[nHumanBaseIdx];	// �q���[�}��(3D)�x�[�X���̃|�C���^

		if (false) {}
		else if (!strcmp(aDataSearch, "SETUP_PATH:")) {	// ���i�ݒ���
			if (!bSetUpLoad) {
				fscanf_string(pFile, aDataSearch);	// �p�X��ǂݍ���

				// ���f���̃Z�b�g�A�b�v(3D)��ǂݍ���
				pHumanBase->nSetUpNum = LoadModelSetUp(aDataSearch);
			}
		}
		else if (!strcmp(aDataSearch, "EYE_POS:"))     { fscanf_D3DXVECTOR3(pFile, &pHumanBase->eyePos); }	// �ڂ̈ʒu
		else if (!strcmp(aDataSearch, "EYE_ROT:"))     { fscanf_D3DXVECTOR3(pFile, &pHumanBase->eyeRot); }	// �ڂ̌���
		else if (!strcmp(aDataSearch, "EYE_PARENT:"))  { fscanf_int(pFile, &pHumanBase->nEyeParent); }		// �ڂ̐e���i�ԍ�
		else if (!strcmp(aDataSearch, "EYE_SPACE:"))   { fscanf_float(pFile, &pHumanBase->fEyeSpace); }		// �ڂ̊Ԋu
		else if (!strcmp(aDataSearch, "EYE_ROT_DIA:")) { fscanf_float(pFile, &pHumanBase->fEyeRotDia); }	// �ڂ̉�]�{��
	}

	// �f�[�^�t�@�C�������
	fclose(pFile);

	// �q���[�}��(3D)�x�[�X�ԍ���Ԃ�
	return nHumanBaseIdx;
}

//========================================
// SaveHuman3DBase�֐� - �q���[�}��(3D)�x�[�X�������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void SaveHuman3DBase(char *pPath, int nHumanBaseIdx) {
	FILE *pFile = fopen(pPath, "w"); // �t�@�C�����J��

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ�A
		// �������ރq���[�}��(3D)�x�[�X���̃|�C���^
		Human3DBase *pHumanBase = g_ppHuman3DBase[nHumanBaseIdx];
		
		fprintf(pFile, "SETUP_PATH: %s\n", GetModelSetUpControl()->ppLoadModelSetUpPath[pHumanBase->nSetUpNum]);
		fprintf(pFile, "EYE_POS: %f %f %f\n", pHumanBase->eyePos.x, pHumanBase->eyePos.y, pHumanBase->eyePos.z);
		fprintf(pFile, "EYE_ROT: %f %f %f\n", pHumanBase->eyeRot.x, pHumanBase->eyeRot.y, pHumanBase->eyeRot.z);
		fprintf(pFile, "EYE_PARENT: %d\n", pHumanBase->nEyeParent);
		fprintf(pFile, "EYE_SPACE: %f\n", pHumanBase->fEyeSpace);
		fprintf(pFile, "EYE_ROT_DIA: %f\n", pHumanBase->fEyeRotDia);

		fprintf(pFile, "END\n");

		// �t�@�C�������
		fclose(pFile);
	}
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================

//========================================
// GetHuman3DBase�֐� - �q���[�}��(3D)�x�[�X�����擾 -
// Author:RIKU NISHIMURA
//========================================
Human3DBase GetHuman3DBaseInfo(int nSetUp) {
	if (nSetUp == -1 || nSetUp >= g_human3DBaseInfo.nHumanBaseNum)
	{// �Z�b�g�A�b�v�ԍ���-1 or �q���[�}��(3D)�x�[�X���͈̔͂𒴂��Ă������A
		Human3DBase defSetUp = {};
		return defSetUp;
	}

	return *g_ppHuman3DBase[nSetUp];
}

//========================================
// GetHuman3DBase�֐� - �q���[�}��(3D)�x�[�X�����擾(�|�C���^) -
// Author:RIKU NISHIMURA
//========================================
Human3DBase **GetHuman3DBase(void) {
	return g_ppHuman3DBase;
}

//========================================
// GetHuman3DBaseInfo�֐� - �q���[�}��(3D)�x�[�X�Ǘ������擾 -
// Author:RIKU NISHIMURA
//========================================
Human3DBaseControl *GetHuman3DBaseControl(void) {
	return &g_human3DBaseInfo;
}

//============================================================
//--------------------| *** ���̑� *** |----------------------
//============================================================

//========================================
// InitHuman3DBaseMemory�֐� - �q���[�}��(3D)�x�[�X���̃������������� -
// Author:RIKU NISHIMURA
//========================================
void InitHuman3DBaseMemory(int nSetUpNum) {
	// ���݂̃Z�b�g�A�b�v����ۑ�
	int nSetUpNumTemp = g_human3DBaseInfo.nHumanBaseNum;

	// �Z�b�g�A�b�v������
	g_human3DBaseInfo.nHumanBaseNum = nSetUpNum;

	// ������̃��������
	for (int nCnt = 0; nCnt < nSetUpNumTemp; nCnt++) {
		ReleaseMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[nCnt]);
	}

	// ������̃������Ċm��
	ReGetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath, sizeof(char*), nSetUpNumTemp, g_human3DBaseInfo.nHumanBaseNum);
	for (int nCnt = 0; nCnt < g_human3DBaseInfo.nHumanBaseNum; nCnt++) {
		g_human3DBaseInfo.ppLoadHumanBasePath[nCnt] = NULL;
		GetMemory((void**)&g_human3DBaseInfo.ppLoadHumanBasePath[nCnt], sizeof(char), TXT_MAX);
	}

	// ���f���̃Z�b�g�A�b�v(3D)���̃��������
	for (int nCnt = 0; nCnt < nSetUpNumTemp; nCnt++) {
		ReleaseMemory((void**)&g_ppHuman3DBase[nCnt]);
	}

	// ���f���̃Z�b�g�A�b�v(3D)���̃������Ċm��
	ReGetMemory((void**)&g_ppHuman3DBase, sizeof(Human3DBase*), nSetUpNumTemp, g_human3DBaseInfo.nHumanBaseNum);
	for (int nCnt = 0; nCnt < nSetUpNum; nCnt++) {
		g_ppHuman3DBase[nCnt] = NULL;
		GetMemory((void**)&g_ppHuman3DBase[nCnt], sizeof(Human3DBase), 1);
		*g_ppHuman3DBase[nCnt] = {};
	}
}