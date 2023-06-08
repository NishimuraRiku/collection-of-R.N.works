//========================================
// 
// ���i(3D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** parts3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadParts3DSet�֐� - ���i�ݒ���(3D)�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet) {

	// ���i�ݒ����������
	if (pPartsSet->pPartsType != NULL) {
		delete[] pPartsSet->pPartsType;
		pPartsSet->pPartsType = NULL;
	}
	*pPartsSet = {};

	// ���i���ǂݍ���
	fscanf_int(pFile, &pPartsSet->nPartsNum);

	// ���i�̎�ޖ��̏��̃������m��
	GetMemory((void**)&pPartsSet->pPartsType, sizeof(Parts3DType),pPartsSet->nPartsNum);

	char aDataSearch[TXT_MAX] = "";	// �����p������
	int nCntParts = 0;			// ���i�̃J�E���g

	// ���i�ݒ���̓ǂݍ��݂��J�n
	while (DataSearch(pFile, aDataSearch, "PARTSSET_END")) {
		if (false) {}
		else if (!strcmp(aDataSearch, /* �g��{�� */"SCALE")) { fscanf_float(pFile, &pPartsSet->fScale); }
		else if (!strcmp(aDataSearch, /* ���i��� */"PARTS")) {
			// ���i�̎�ޖ��̏��̃|�C���^
			pPartsSet->pPartsType[nCntParts] = {};
			Parts3DType *pPartsType = &pPartsSet->pPartsType[nCntParts];

			// ���i���̓ǂݍ��݂��J�n
			while (DataSearch(pFile, aDataSearch, "PARTS_END")) {
				if (false) {}
				else if (!strcmp(aDataSearch, /* ���i�ԍ�   */"INDEX"))      { fscanf_int(pFile, &pPartsType->nIdx); }
				else if (!strcmp(aDataSearch, /* ���f���p�X */"MODEL_PATH")) {
					fscanf_string(pFile, aDataSearch);	// �p�X�ǂݍ���
														// ���f���ǂݍ���
					pPartsType->nModelIdx = LoadModel(aDataSearch);
				}
				else if (!strcmp(aDataSearch, /* �e���i�ԍ� */"PARENT"))       { fscanf_int(pFile, &pPartsType->nParent); }
				else if (!strcmp(aDataSearch, /* ���Έʒu   */"RELATIVE_POS")) { fscanf_D3DXVECTOR3(pFile, &pPartsType->fixedRelativePos); }
				else if (!strcmp(aDataSearch, /* ���Ίp�x   */"RELATIVE_ROT")) { fscanf_D3DXVECTOR3(pFile, &pPartsType->fixedRelativeRot); }
				else if (!strcmp(aDataSearch, /* �e���i�ԍ� */"LIGHTING"))     { fscanf_bool(pFile, &pPartsType->bLighting); }
			}

			nCntParts++;	// ���i�̃J�E���g�����Z
			assert(nCntParts <= pPartsSet->nPartsNum);
		}
	}
}

//========================================
// SaveParts3DSet�֐� - ���i�ݒ���(3D)�̏������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void SaveParts3DSet(FILE *pFile, Parts3DSet *pPartsSet) {
	fprintf(pFile, "PARTSSET %d\n", pPartsSet->nPartsNum);
	fprintf(pFile, "	SCALE %f\n", pPartsSet->fScale);
	
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++) {
		Parts3DType *pPartsType = &pPartsSet->pPartsType[nCntParts];

		fprintf(pFile, "	PARTS\n");
		fprintf(pFile, "		INDEX %d\n", pPartsType->nIdx);
		fprintf(pFile, "		MODEL_PATH %s\n", GetModelInfo()->aLoadModelPath[pPartsType->nModelIdx]);
		fprintf(pFile, "		PARENT %d\n", pPartsType->nParent);
		fprintf(pFile, "		RELATIVE_POS %f %f %f\n",
				pPartsType->fixedRelativePos.x,
				pPartsType->fixedRelativePos.y,
				pPartsType->fixedRelativePos.z);
		fprintf(pFile, "		RELATIVE_ROT %f %f %f\n",
				pPartsType->fixedRelativeRot.x,
				pPartsType->fixedRelativeRot.y,
				pPartsType->fixedRelativeRot.z);
		fprintf(pFile, "		LIGHTING %d\n", (int)pPartsType->bLighting);
		fprintf(pFile, "	PARTS_END\n");
	}
	fprintf(pFile, "PARTSSET_END\n");
}