//========================================
// 
// ���f���̃Z�b�g�A�b�v(3D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** model-setup3D.cpp ***
//========================================
#include "model-setup3D.h"
#include "../../Other/memory.h"
#include "../../Other/file.h"
#include "../../Other/window.h"
#include "../../Other/print.h"

//****************************************
// �O���[�o���ϐ��錾
//****************************************
static ModelSetUp3DControl g_modelSetUpInfo;	// ���f���̊Ǘ����
static ModelSetUp3D **g_ppModelSetUp;	// ���f���̃Z�b�g�A�b�v(3D)���

//****************************************
// �O���[�o���萔�錾
//****************************************
// ���f���\����
static const char g_aModelPlatformName[MODELPLATFORM_MAX][TXT_MAX] = {
	"NONE",				// ����
	"BIPEDALISM_00",	// ����2�����s00
	"BIPEDALISM_01",	// ����2�����s01
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UninitModelSetUp�֐� - ���f���̃Z�b�g�A�b�v(3D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitModelSetUp(void) {
	// ���������
	for (int nCntSetUp = 0; nCntSetUp < g_modelSetUpInfo.nModelSetUpNum; nCntSetUp++) {
		// �Z�b�g�A�b�v��� > ���i�ݒ��� > ���i�̎�ޖ��̐ݒ���
		ReleaseMemory((void**)&g_ppModelSetUp[nCntSetUp]->partsSet.pPartsType);

		// �Z�b�g�A�b�v��� > �����蔻��ݒ��� > �����蔻��̏�� >�����蔻��̕��i�̏��
		for (int nCntHitTest = 0; nCntHitTest < g_ppModelSetUp[nCntSetUp]->hitTestSet.nHitTestNum; nCntHitTest++) {
			ReleaseMemory((void**)&g_ppModelSetUp[nCntSetUp]->hitTestSet.pHitTest[nCntHitTest].pHitTestParts);
		}

		// �Z�b�g�A�b�v��� > �����蔻��ݒ��� > �����蔻��̏��
		ReleaseMemory((void**)&g_ppModelSetUp[nCntSetUp]->hitTestSet.pHitTest);

		// �Ǎ��σZ�b�g�A�b�v�p�X
		ReleaseMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[nCntSetUp]);
	}
	// �Ǎ��σZ�b�g�A�b�v�p�X�S��
	ReleaseMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath);

	// �Z�b�g�A�b�v���
	ReleaseMemory((void**)&g_ppModelSetUp);
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================

//========================================
// LoadModelSetUp�֐� - ���f���̃Z�b�g�A�b�v(3D)�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
int LoadModelSetUp(char *pModelSetUpPath, bool bReload, int nSetUpIdx) {
	int nIdx = 0;	// �Z�b�g�A�b�v�ԍ�

	if (nSetUpIdx >= 0 || bReload)
	{// �ǂݍ��ރZ�b�g�A�b�v�ԍ����w�肳��Ă��� or �ēǂݍ��݃t���O���^�̎��A
		// �ԍ����͈͊O�ł����-1��Ԃ��I��
		if (nIdx >= g_modelSetUpInfo.nModelSetUpNum) {
			assert(false);
			return -1;
		}

		// �Z�b�g�A�b�v�ԍ���ݒ�
		nIdx = nSetUpIdx;
	}
	else 
	{// �ǂݍ��ރZ�b�g�A�b�v�ԍ����w�肳��Ă��Ȃ����A
		nIdx = g_modelSetUpInfo.nModelSetUpNum;	// �Z�b�g�A�b�v�ԍ���擪�ԍ��ɂ���
	}

	FILE *pFile;	// �t�@�C���|�C���^

	if (!bReload) 
	{// �ēǂݍ��݂łȂ����A
		// ���ɓǂݍ��܂�Ă��郂�f���Z�b�g�A�b�v���m�F
		for (int nCntType = 0; nCntType < g_modelSetUpInfo.nModelSetUpNum; nCntType++) {
			if (!strcmp(pModelSetUpPath, g_modelSetUpInfo.ppLoadModelSetUpPath[nCntType]))
			{// ���ɓ����p�X�̃��f���Z�b�g�A�b�v��ǂݍ���ł������A
				return nCntType;	// ��ޔԍ���Ԃ�
			}
		}

		// ��ޖ��̏��̃f�[�^�t�@�C�����J��
		pFile = fopen(pModelSetUpPath, "r");
		if (pFile == NULL) {// �t�@�C�����J���Ȃ������ꍇ�A�������I������
			Message(CreateText("���f���̃Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s���܂����B\n\"%s\"", pModelSetUpPath), "ERROR!");
			return -1;
		}

		// �Z�b�g�A�b�v�������Z
		g_modelSetUpInfo.nModelSetUpNum++;

		// ������̃������Ċm��
		ReGetMemory(
			/* �|�C���^ */(void**)&g_modelSetUpInfo.ppLoadModelSetUpPath,
			/* �^�T�C�Y */sizeof(char*),
			/* �ߋ���   */g_modelSetUpInfo.nModelSetUpNum - 1,
			/* ���ݐ�   */g_modelSetUpInfo.nModelSetUpNum);
		// ���݂����|�C���^��NULL������
		g_modelSetUpInfo.ppLoadModelSetUpPath[g_modelSetUpInfo.nModelSetUpNum - 1] = NULL;
		// ���������̃��������m�ۂ���
		GetMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[g_modelSetUpInfo.nModelSetUpNum - 1], sizeof(char), TXT_MAX);

		// ���f���̃Z�b�g�A�b�v(3D)���̃������Ċm��
		ReGetMemory(
			/* �|�C���^ */(void**)&g_ppModelSetUp,
			/* �^�T�C�Y */sizeof(ModelSetUp3D*),
			/* �ߋ���   */g_modelSetUpInfo.nModelSetUpNum - 1,
			/* ���ݐ�   */g_modelSetUpInfo.nModelSetUpNum);
		// ���݂����|�C���^��NULL������
		g_ppModelSetUp[g_modelSetUpInfo.nModelSetUpNum - 1] = NULL;
		// ���f���̃Z�b�g�A�b�v���̃��������m�ۂ���
		GetMemory((void**)&g_ppModelSetUp[g_modelSetUpInfo.nModelSetUpNum - 1], sizeof(ModelSetUp3D), 1);
	}
	else 
	{// �ēǂݍ��݂̎��A
		// ��ޖ��̏��̃f�[�^�t�@�C�����J��
		pFile = fopen(pModelSetUpPath, "r");
		if (pFile == NULL) {// �t�@�C�����J���Ȃ������ꍇ�A�������I������
			Message(CreateText("���f���̃Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s���܂����B\n\"%s\"", pModelSetUpPath), "ERROR!");
			return -1;
		}
	}

	// ���f���̃Z�b�g�A�b�v(3D)�̃p�X��ۑ�
	strcpy(g_modelSetUpInfo.ppLoadModelSetUpPath[nSetUpIdx], pModelSetUpPath);

	// �ǂݍ��݃��[�v
	char aDataSearch[TXT_MAX] = "";
	while (DataSearch(pFile, aDataSearch, "END")) {
		ModelSetUp3D *pSetUp = g_ppModelSetUp[nSetUpIdx];	// ���f���̃Z�b�g�A�b�v(3D)���̃|�C���^

		if (false) {}
		else if (!strcmp(aDataSearch, "PARTSSET"))   { LoadParts3DSet(pFile, &pSetUp->partsSet); }		// ���i�ݒ���
		else if (!strcmp(aDataSearch, "HITTESTSET")) { LoadHitTest3DSet(pFile, &pSetUp->hitTestSet); }	// �����蔻��ݒ��� 
		else if (!strcmp(aDataSearch, "PLATFORM"))
		{// ���f���\��
			fscanf_string(pFile, aDataSearch);

			// ���f���\��������
			pSetUp->platform = MODELPLATFORM_NONE;

			for (int nCntPlatform = 0; nCntPlatform < MODELPLATFORM_MAX; nCntPlatform++) {
				if (!strcmp(aDataSearch, g_aModelPlatformName[nCntPlatform]))
				{// �J�E���g�̃��f���\�����ƈ�v�������A���f���\���ݒ�
					pSetUp->platform = (MODELPLATFORM)nCntPlatform;
					break;
				}
			}
		}
	}

	// �t�@�C�������
	fclose(pFile);

	// ���f���̃Z�b�g�A�b�v(3D)�ԍ���Ԃ�
	return nSetUpIdx;
}

//========================================
// SaveModelSetUp�֐� - ���f���̃Z�b�g�A�b�v(3D)�������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void SaveModelSetUp(char *pPath, int nSetUpIdx) {
	FILE *pFile = fopen(pPath, "w"); // �t�@�C�����J��

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ�A
		// �������ރ��f���̃Z�b�g�A�b�v(3D)���̃|�C���^
		ModelSetUp3D *pSetUp = g_ppModelSetUp[nSetUpIdx];

		// ���i�ݒ���(3D)�̏������ݏ���
		SaveParts3DSet(pFile, &pSetUp->partsSet);

		{// �����蔻��̐ݒ������������
			// �����蔻��̐ݒ���̃|�C���^
			HitTest3DSet *pPartsSet = &g_ppModelSetUp[nSetUpIdx]->hitTestSet;

			fprintf(pFile, "HITTESTSET %d\n", pPartsSet->nHitTestNum);
			for (int nCntHitTest = 0; nCntHitTest < pPartsSet->nHitTestNum; nCntHitTest++) {
				// �����蔻��̏��̃|�C���^
				HitTest3D *pHitTest = &pPartsSet->pHitTest[nCntHitTest];

				fprintf(pFile, "	HITTEST %d\n", pHitTest->nHitTestPartsNum);
				for (int nCntParts = 0; nCntParts < pHitTest->nHitTestPartsNum; nCntParts++) {
					// �����蔻��̕��i���̏��̃|�C���^
					HitTest3DParts *pParts = &pHitTest->pHitTestParts[nCntParts];

					fprintf(pFile, "		HITTEST_PARTS\n");
					fprintf(pFile, "			FORM %d\n", (int)pParts->hitTestForm);
					fprintf(pFile, "			PARENT %d\n", pParts->nParent);
					fprintf(pFile, "			WIDTH %f\n", pParts->fWidth);
					fprintf(pFile, "			HEIGHT %f\n", pParts->fHeight);
					fprintf(pFile, "			DEPTH %f\n", pParts->fDepth);
					fprintf(pFile, "			RELATIVE_POS %f %f %f\n",
						pParts->fixedRelativePos.x,
						pParts->fixedRelativePos.y,
						pParts->fixedRelativePos.z);
					fprintf(pFile, "			RELATIVE_ROT %f %f %f\n",
						pParts->fixedRelativeRot.x,
						pParts->fixedRelativeRot.y,
						pParts->fixedRelativeRot.z);
					fprintf(pFile, "			GHOST");
					for (int nCntDir = 0; nCntDir < HITDIRECTION_MAX; nCntDir++) {
						fprintf(pFile, " %d", (int)pParts->aGhost[nCntDir]);
					}
					fprintf(pFile, "\n");
					fprintf(pFile, "		HITTEST_PARTS_END\n");
				}
				fprintf(pFile, "	HITTEST_END\n");
			}
			fprintf(pFile, "HITTESTSET_END\n");
		}
		fprintf(pFile, "PLATFORM %s\n", g_aModelPlatformName[pSetUp->platform]);
		fprintf(pFile, "END\n");

		// �t�@�C�������
		fclose(pFile);
	}
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================

//========================================
// GetModelSetUp�֐� - ���f���̃Z�b�g�A�b�v(3D)�����擾 -
// Author:RIKU NISHIMURA
//========================================
ModelSetUp3D GetModelSetUpInfo(int nSetUp) {
	if (nSetUp <= -1 || nSetUp >= g_modelSetUpInfo.nModelSetUpNum)
	{// �Z�b�g�A�b�v�ԍ����͈͊O�̎��A�f�t�H���g�̃Z�b�g�A�b�v����Ԃ�
		ModelSetUp3D defSetUp = {};
		return defSetUp;
	}

	return *g_ppModelSetUp[nSetUp];
}

//========================================
// GetModelSetUp�֐� - ���f���̃Z�b�g�A�b�v(3D)�����擾(�|�C���^) -
// Author:RIKU NISHIMURA
//========================================
ModelSetUp3D **GetModelSetUp(void) {
	return g_ppModelSetUp;
}

//========================================
// GetModelSetUpInfo�֐� - ���f���̃Z�b�g�A�b�v(3D)�Ǘ������擾 -
// Author:RIKU NISHIMURA
//========================================
ModelSetUp3DControl *GetModelSetUpControl(void) {
	return &g_modelSetUpInfo;
}

//============================================================
//--------------------| *** ���̑� *** |----------------------
//============================================================

//========================================
// InitModelSetUpMemory�֐� - ���f���̃Z�b�g�A�b�v(3D)���̃������������� -
// Author:RIKU NISHIMURA
//========================================
void InitModelSetUpMemory(int nNum) {
	// ���݂̃Z�b�g�A�b�v����ۑ�
	int nNumTemp = g_modelSetUpInfo.nModelSetUpNum;

	// �Z�b�g�A�b�v������
	g_modelSetUpInfo.nModelSetUpNum = nNum;

	{//========== *** �ǂݍ��ݍς݃��f���Z�b�g�A�b�v�p�X ***
		// ������|�C���^�̃��������
		for (int nCnt = 0; nCnt < nNumTemp; nCnt++) {
			ReleaseMemory((void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[nCnt]);
		}

		// ������_�u���|�C���^�̃������m��
		GetMemory(
			/* �|�C���^ */(void**)&g_modelSetUpInfo.ppLoadModelSetUpPath,
			/* �^�T�C�Y */sizeof(char*),
			/* �m�ې�   */g_modelSetUpInfo.nModelSetUpNum);

		// ������|�C���^�̃������m��(��������)
		for (int nCnt = 0; nCnt < g_modelSetUpInfo.nModelSetUpNum; nCnt++) {
			g_modelSetUpInfo.ppLoadModelSetUpPath[nCnt] = NULL;
			GetMemory(
				/* �|�C���^ */(void**)&g_modelSetUpInfo.ppLoadModelSetUpPath[nCnt],
				/* �^�T�C�Y */sizeof(char),
				/* �m�ې�   */TXT_MAX);
		}
	}

	{//========== *** ���f���̃Z�b�g�A�b�v(3D)��� ***
		// ���f���̃Z�b�g�A�b�v(3D)���̃��������
		for (int nCnt = 0; nCnt < nNumTemp; nCnt++) {
			ReleaseMemory((void**)&g_ppModelSetUp[nCnt]);
		}

		// ���f���̃Z�b�g�A�b�v(3D)���_�u���|�C���^�̃������m��
		GetMemory(
			/* �|�C���^ */(void**)&g_ppModelSetUp,
			/* �^�T�C�Y */sizeof(ModelSetUp3D*),
			/* �m�ې�   */g_modelSetUpInfo.nModelSetUpNum);

		// ���f���̃Z�b�g�A�b�v(3D)���|�C���^�̃������m��
		for (int nCnt = 0; nCnt < nNum; nCnt++) {
			g_ppModelSetUp[nCnt] = NULL;
			GetMemory(
				/* �|�C���^ */(void**)&g_ppModelSetUp[nCnt],
				/* �^�T�C�Y */sizeof(ModelSetUp3D),
				/* �m�ې�   */1);
			*g_ppModelSetUp[nCnt] = {};	// ������
		}
	}
}