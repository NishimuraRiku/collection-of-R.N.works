//========================================
// 
// ���[�V����(3D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** motion3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//****************************************
// �\���̒�`
//****************************************
// ���[�V����(3D)�̊Ǘ����
typedef struct {
	char **ppLoadMotionPath = NULL;	// �ǂݍ��ݍς̃��[�V����(3D)�p�X
	int nMotionNum = 0;	// ���[�V����(3D)�̐�
}Motion3DInfo;

//****************************************
// �O���[�o���ϐ��錾
//****************************************
// ���[�V����(3D)�̊Ǘ����
static Motion3DInfo g_motion3DInfo;
// ���[�V�������
static Motion3D *g_pMotion3D;

//****************************************
// �O���[�o���萔�錾
//****************************************
// ���[�V�����R�}���h�̃��x�����̏��
const Motion3DCommandInfo g_aMotionCommandInfo[MOTION3D_COMMAND_LABEL_MAX] = {
	{ "MOVE",5 },	// �ړ�
	{ "SPIN",5 },	// ��]
	{ "STEP",3 },	// ����
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ���[�V����(3D)�֘A *** |----------
//============================================================

//========================================
// LoadMotion3D�֐� - ���[�V����(3D)�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
int LoadMotion3D(Motion3DLoad motionLoad) {
	char *pMotionPath = motionLoad.pMotionPath;	// ���[�V�����̃p�X
	int nMotionIdx = g_motion3DInfo.nMotionNum;	// ���[�V�����ԍ�
	bool bLoadDone = false;	// �ǂݍ��ݍσt���O

	if (motionLoad.nMotionIdx != -1)
	{// �ǂݍ��ރ��[�V�����ԍ����w�肳��Ă��鎞�A
		nMotionIdx = motionLoad.nMotionIdx;	// ���[�V�����ԍ���ݒ�
		bLoadDone = true;	// �ǂݍ��ݍ�
	}

	// ���ɓǂݍ��܂�Ă��郂�[�V�������m�F
	for (int nCntType = 0; nCntType < g_motion3DInfo.nMotionNum; nCntType++) {
		if (!strcmp(pMotionPath, g_motion3DInfo.ppLoadMotionPath[nCntType]))
		{// ���ɓ����p�X�̃��[�V������ǂݍ���ł������A
			// �ǂݍ��ݍσt���O��^�ɂ���
			bLoadDone = true;

			if (motionLoad.nMotionIdx != -1)
			{// �ǂݍ��ރZ�b�g�A�b�v�ԍ����w�肳��Ă��鎞�A
				break;
			}
			else if (motionLoad.bReload)
			{// �ēǂݍ��݃t���O���^�̎��A
				nMotionIdx = nCntType;
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
		// ���[�V�����������Z
		g_motion3DInfo.nMotionNum++;

		// ������̃_�u���|�C���^�̃������Ċm��
		ReGetMemory((void**)&g_motion3DInfo.ppLoadMotionPath, sizeof(char*), g_motion3DInfo.nMotionNum - 1, g_motion3DInfo.nMotionNum);

		// ������̃|�C���^�̃������m��
		GetMemory((void**)&g_motion3DInfo.ppLoadMotionPath[g_motion3DInfo.nMotionNum - 1], sizeof(char) * TXT_MAX, 1);

		// ���[�V�������̃������Ċm��
		ReGetMemory((void**)g_pMotion3D, sizeof(Motion3D), g_motion3DInfo.nMotionNum - 1, g_motion3DInfo.nMotionNum);
	}

	// ���[�V����(3D)�̃p�X��ۑ�
	sprintf(g_motion3DInfo.ppLoadMotionPath[nMotionIdx], "%s", pMotionPath);

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	FILE *pFile = fopen(pMotionPath, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A�������I������
		assert(false);
		return 0;
	}

	// ���[�V����(3D)���̃|�C���^
	Motion3D *pMotion = &g_pMotion3D[nMotionIdx];
	char aDataSearch[TXT_MAX] = "";	// �f�[�^�����p������
	int nCntParts = 0;	// ���i�̃J�E���g

	// ���[�V�����ݒ���̓ǂݍ��݂��J�n
	while (DataSearch(pFile, aDataSearch, "END")) {
		if (false) {}
		else if (!strcmp(aDataSearch, "LOOP_TIME")) { fscanf_int(pFile, &pMotion->nLoopTime); }	// ���[�v����
		else if (!strcmp(aDataSearch, "LOOP"))      { fscanf_bool(pFile, &pMotion->bLoop); }	// ���[�v���邩�t���O

		if (!strcmp(aDataSearch, "PARTSMOTION")) {
			fscanf_int(pFile, &pMotion->nPartsMotionNum);	// ���i���̃��[�V�����̐��擾

			// ���i�̎�ޖ��̏��̃������m��
			pMotion->pPartsMotion = NULL;
			GetMemory((void**)&pMotion->pPartsMotion, sizeof(PartsMotion3D), pMotion->nPartsMotionNum);

			// ���[�V�������̓ǂݍ��݂��J�n
			while (DataSearch(pFile, aDataSearch, "PARTSMOTION_END")) {
				if (false) {}
				else if (!strcmp(aDataSearch, "PARTS")) {
					// ���i���̃��[�V�����̏��̃|�C���^
					pMotion->pPartsMotion[nCntParts] = {};
					PartsMotion3D *pPartsMotion = &pMotion->pPartsMotion[nCntParts];

					fscanf_int(pFile, &pPartsMotion->nCommandNum);	// �R�}���h���擾

					// �R�}���h���̃������m��
					if (pPartsMotion->nCommandNum > 0) {
						GetMemory((void**)&pPartsMotion->pCommand, sizeof(Motion3DCommand), pPartsMotion->nCommandNum);

						// �R�}���h�̃J�E���g
						int nCntCmd = 0;

						// ���i���̃��[�V�����̏��̓ǂݍ��݂��J�n
						while (DataSearch(pFile, aDataSearch, "PARTS_END")) {
							if (false) {}
							else if (!strcmp(aDataSearch, "TIME")) {
								// �R�}���h�J�n����
								fscanf_int(pFile, &pPartsMotion->pCommand[nCntCmd].nTime);

								// �R�}���h�ԍ�
								int nCmd = 0;
								// �R�}���h���x����ǂݎ��
								fscanf_string(pFile, aDataSearch);

								// ��������Ń��x���̔�����s��
								for (int nCntLabel = 0; nCntLabel < MOTION3D_COMMAND_LABEL_MAX; nCntLabel++) {
									if (!strcmp(aDataSearch, g_aMotionCommandInfo[nCntLabel].pLabelName))
									{// �����񂪃��x�����ƈ�v���Ă��鎞�A
										// ���x����ݒ�
										pPartsMotion->pCommand[nCntCmd].command = (MOTION3D_COMMAND_LABEL)nCntLabel;
										// �R�}���h�ԍ����w��
										nCmd = nCntLabel;
										break;
									}
								}

								// �������̃��������m��
								pPartsMotion->pCommand[nCntCmd].pData = NULL;
								GetMemory((void**)&pPartsMotion->pCommand[nCntCmd].pData, sizeof(float), g_aMotionCommandInfo[nCmd].nDataNum);

								// �����̓ǂݍ��݂������̐����s��
								for (int nCntData = 0; nCntData < g_aMotionCommandInfo[nCmd].nDataNum; nCntData++) {
									fscanf_float(pFile, &pPartsMotion->pCommand[nCntCmd].pData[nCntData]);
								}

								// �R�}���h�̃J�E���g�����Z
								nCntCmd++;
							}
						}
					}

					// ���i�̃J�E���g�����Z
					nCntParts++;
				}
			}
		}
	}

	// �f�[�^�t�@�C�������
	fclose(pFile);

	// ���[�V����(3D)�ԍ���Ԃ�
	return nMotionIdx;
}

//========================================
// SaveMotion3D�֐� - ���[�V����(3D)�������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void SaveMotion3D(char *pPath, int nMotionIdx)
{
	FILE *pFile; // �t�@�C���̃|�C���^

	// �t�@�C�����J��
	pFile = fopen(pPath, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ�A
		// �������ރ��[�V����(3D)���̃|�C���^
		Motion3D *pMotion = &g_pMotion3D[nMotionIdx];

		fprintf(pFile, "LOOP_TIME %d\n", pMotion->nLoopTime);
		fprintf(pFile, "LOOP %d\n", (int)pMotion->bLoop);

		{// ���i(3D)���̃��[�V����������������
			fprintf(pFile, "PARTSMOTION %d\n", pMotion->nPartsMotionNum);

			for (int nCntParts = 0; nCntParts < pMotion->nPartsMotionNum; nCntParts++) {
				// ���i(3D)�ݒ���̃|�C���^
				PartsMotion3D *pPartsMotion = &pMotion->pPartsMotion[nCntParts];

				fprintf(pFile, "	PARTS %d\n", pPartsMotion->nCommandNum);
				for (int nCntCmd = 0; nCntCmd < pPartsMotion->nCommandNum; nCntCmd++) {
					Motion3DCommand cmd = pPartsMotion->pCommand[nCntCmd];
					fprintf(pFile, "		TIME %d ", cmd.nTime);
					switch (cmd.command) {
					case MOTION3D_COMMAND_LABEL_MOVE:
						fprintf(pFile, "MOVE %f %f %f %d %d\n", cmd.pData[0], cmd.pData[1], cmd.pData[2], (int)cmd.pData[3], (int)cmd.pData[4]);
						break;
					case MOTION3D_COMMAND_LABEL_SPIN:
						fprintf(pFile, "SPIN %f %f %f %d %d\n", cmd.pData[0], cmd.pData[1], cmd.pData[2], (int)cmd.pData[3], (int)cmd.pData[4]);
						break;
					case MOTION3D_COMMAND_LABEL_STEP:
						fprintf(pFile, "STEP %d %d %d\n", (int)cmd.pData[0], (int)cmd.pData[1], (int)cmd.pData[2]);
						break;
					default:
						assert(false);
						break;
					}
				}
				fprintf(pFile, "	PARTS_END\n");
			}
			fprintf(pFile, "PARTSMOTION_END\n");
		}

		fprintf(pFile, "END\n");

		// �t�@�C�������
		fclose(pFile);
	}
}

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================

//========================================
// UninitMotion3D�֐� - ���[�V����(3D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitMotion3D(void) {
	for (int nCntPath = 0; nCntPath < g_motion3DInfo.nMotionNum; nCntPath++) {
		// �ǂݍ��ݍσ��[�V����(3D)�p�X�̃|�C���^
		ReleaseMemory((void**)&g_motion3DInfo.ppLoadMotionPath[nCntPath]);
	}
	// �ǂݍ��ݍσ��[�V����(3D)�p�X�̃_�u���|�C���^
	ReleaseMemory((void**)&g_motion3DInfo.ppLoadMotionPath);
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================

//========================================
// GetMotion3D�֐� - ���[�V����(3D)�̏����擾 -
// Author:RIKU NISHIMURA
//========================================
Motion3D *GetMotion3D(void) {
	return g_pMotion3D;
}

//========================================
// GetMotion3DCommandInfo�֐� - ���[�V����(3D)�̃R�}���h���̏����擾 -
// Author:RIKU NISHIMURA
//========================================
Motion3DCommandInfo GetMotion3DCommandInfo(MOTION3D_COMMAND_LABEL cmd) {
	return g_aMotionCommandInfo[cmd];
}

//============================================================
//--------------------| *** ���̑� *** |----------------------
//============================================================

//========================================
// InitMotion3DMemory�֐� - ���[�V����(3D)�̏��̃������������� -
// Author:RIKU NISHIMURA
//========================================
void InitMotion3DMemory(int nMotion) {
	// ���݂̃��[�V��������ۑ�
	int nMotionNumTemp = g_motion3DInfo.nMotionNum;

	// �Z�b�g�A�b�v������
	g_motion3DInfo.nMotionNum = nMotion;

	// ������̃_�u���|�C���^�̃������Ċm��
	ReGetMemory((void**)&g_motion3DInfo.ppLoadMotionPath, sizeof(char*), nMotionNumTemp, g_motion3DInfo.nMotionNum);

	// ������̃|�C���^�̃������m��
	g_motion3DInfo.ppLoadMotionPath[g_motion3DInfo.nMotionNum - 1] = NULL;
	GetMemory((void**)&g_motion3DInfo.ppLoadMotionPath[g_motion3DInfo.nMotionNum - 1], sizeof(char) * TXT_MAX, 1);

	// ���[�V����(3D)�̏��̃������Ċm��
	ReGetMemory((void**)&g_pMotion3D, sizeof(Motion3D), nMotionNumTemp, g_motion3DInfo.nMotionNum);

	// �m�ۂ������̏���S�ď���������
	for (int nCntMotion = 0; nCntMotion < nMotion; nCntMotion++) {
		g_pMotion3D[nCntMotion] = {};
	}
}