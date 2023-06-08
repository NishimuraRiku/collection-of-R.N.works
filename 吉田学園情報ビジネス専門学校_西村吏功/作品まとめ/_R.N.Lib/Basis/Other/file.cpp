//========================================
// 
// �t�@�C���֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** file.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================

//========================================
// OpenFile�֐� - �J���t�@�C����I�����擾 -
// (pFileName:�ǂݍ��܂ꂽ�t�@�C�������i�[���� pInitDir:�����\������f�B���N�g�� pFileType:�I���ł���t�@�C���̎��)
// Author:RIKU NISHIMURA
//========================================
bool OpenFile(char *pFileName, char *pInitDir, char *pFileType)
{
	TCHAR fileName[MAX_PATH];	// �t�@�C�����i�[

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lpstrTitle = "�J��";			// �E�B���h�E�̃^�C�g���w��
	ofn.lStructSize = sizeof(ofn);		// �\���̂̃T�C�Y�w��
	ofn.lpstrFile = fileName;			// �I�����ꂽ�t�@�C�������i�[�����z��w��
	ofn.lpstrFile[0] = '\0';			// �t�@�C����������
	ofn.nMaxFile = sizeof(fileName);	// lpstrFile �z��̍ő�T�C�Y���t�@�C�����i�[�p������̃T�C�Y�Ŏw��
	ofn.lpstrFilter = pFileType;		// �I���ł���t�@�C���̎�ނ��w��
	ofn.nFilterIndex = 1;				// �����I������Ă���t�B���^�̃C���f�b�N�X���w��
	ofn.lpstrFileTitle = NULL;			// �I�����ꂽ�t�@�C���̃^�C�g�����w��(�g�p����Ȃ��ꍇ��NULL)
	ofn.nMaxFileTitle = 0;				// lpstrFileTitle �̍ő�T�C�Y(�g�p����Ȃ��ꍇ��0)
	ofn.lpstrInitialDir = pInitDir;		// �����\������f�B���N�g��(�g�p����Ȃ��ꍇ��NULL)
	// �_�C�A���O�̃I�v�V�������w��
	// OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST �́A�I�����ꂽ�p�X�����݂��邱�ƂƁA�I�����ꂽ�t�@�C�������݂��邱�Ƃ��m�F�������
	// OFN_NOCHANGEDIR �́A�f�B���N�g����ύX���Ȃ��悤�ɂ������
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn))
	{// �J�����t�@�C�������擾�o�������A
		if (!strcmp(fileName, "")) 
		{// �L�����Z�����ꂽ���A
			return false;
		}
		else
		{// �I�����ꂽ���A
			// �t�@�C�����̃|�C���^�ɓǂݍ��񂾃t�@�C��������
			sprintf(pFileName, fileName);
			// ���͏��̃N���A����
			ClearInputInfo();
			return true;
		}
	}
	else {
		// ���͏��̃N���A����
		ClearInputInfo();
		return false;
	}
}

//========================================
// SaveFile�֐� - �ۑ�����t�@�C����I�����擾 -
// �ipFileName:�ǂݍ��܂ꂽ�t�@�C�������i�[���� pInitDir:�����\������f�B���N�g�� pFileType:�I���ł���t�@�C���̊g���q�j
// Author:RIKU NISHIMURA
//========================================
bool SaveFile(char *pFileName, char *pInitDir, char *pExt)
{
	TCHAR fileName[MAX_PATH];	// �t�@�C�����i�[

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lpstrTitle = "���O��t���ĕۑ�";	// �E�B���h�E�̃^�C�g���w��
	ofn.lStructSize = sizeof(ofn);			// �\���̂̃T�C�Y�w��
	ofn.lpstrFile = fileName;				// �I�����ꂽ�t�@�C�������i�[�����z��w��
	ofn.lpstrFile[0] = '\0';				// �t�@�C����������
	ofn.nMaxFile = sizeof(fileName);		// lpstrFile �z��̍ő�T�C�Y���t�@�C�����i�[�p������̃T�C�Y�Ŏw��
	ofn.lpstrFilter = pExt;					// �I���ł���t�@�C���̊g���q���w��
	ofn.nFilterIndex = 1;					// �����I������Ă���t�B���^�̃C���f�b�N�X���w��
	ofn.lpstrFileTitle = NULL;				// �I�����ꂽ�t�@�C���̃^�C�g�����w��(�g�p����Ȃ��ꍇ��NULL)
	ofn.nMaxFileTitle = 0;					// lpstrFileTitle �̍ő�T�C�Y(�g�p����Ȃ��ꍇ��0)
	ofn.lpstrInitialDir = pInitDir;			// �����\������f�B���N�g��(�g�p����Ȃ��ꍇ��NULL)
	// �_�C�A���O�̃I�v�V�������w��
	// OFN_NOCHANGEDIR �́A�f�B���N�g����ύX���Ȃ��悤�ɂ������
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn))
	{// �J�����t�@�C�������擾�o�������A
		// �t�@�C�����̃|�C���^�ɓǂݍ��񂾃t�@�C�����������邪�A
		// �������g���q�ŏI����Ă��Ȃ����A����ɉ����Ċg���q��A��������
		int len = strlen(fileName);
		int extlen = strlen(pExt);
		if (len < extlen || strcmp(fileName + len - extlen, pExt) != 0)
		{
			sprintf(pFileName, "%s%s", fileName, pExt);
		}
		else {
			strcpy(pFileName, fileName);
		}

		// ���͏��̃N���A����
		ClearInputInfo();
		return true;
	}
	else {
		// ���͏��̃N���A����
		ClearInputInfo();
		return false;
	}
}

//========================================
// ConvertToDataPath�֐� - "data"���N�_�Ƃ����p�X������𐶐����� -
// Author:RIKU NISHIMURA
//========================================
char *ConvertToDataPath(const char* path)
{
	static char aDataPath[TXT_MAX] = "";
	const char *pDataDirName = "data\\"; // data�f�B���N�g����

	// �p�X��data�f�B���N�g�����܂܂Ȃ��ꍇ�̓G���[
	if (strstr(path, pDataDirName) == NULL) {
		Message(CreateText("\"%s\" does not contain \"data\\\" directory.\n", path), "Error");
		return NULL;
	}

	// data�f�B���N�g���̒���̕�������R�s�[����
	const char *start = strstr(path, pDataDirName);
	strncpy(aDataPath, start, TXT_MAX);
	aDataPath[TXT_MAX - 1] = '\0'; // NULL�I�[��t��

	return aDataPath;
}

//============================================================
//--------------------| *** �e�L�X�g�ǂݍ��� *** |------------
//============================================================

bool DataSearch(FILE *pFile, char *pDataSearch, char *pEndLabel) 
{
	fscanf_string(pFile, pDataSearch); // ����

	return strcmp(pDataSearch, pEndLabel) != 0;
}

void fscanf_int(FILE *pFile, int *pData) 
{
	fscanf(pFile, "%d", pData);
}

void fscanf_float(FILE *pFile, float *pData) 
{
	fscanf(pFile, "%f", pData);
}

void fscanf_string(FILE *pFile, char *pString) 
{
	fscanf(pFile, "%s", pString);
}

void fscanf_bool(FILE *pFile, bool *pFlag) 
{
	int nTemp;
	fscanf(pFile, "%d", &nTemp);
	*pFlag = (nTemp != 0);
}

void fscanf_D3DXVECTOR3(FILE *pFile, D3DXVECTOR3 *pVec) 
{
	fscanf_float(pFile, &pVec->x);
	fscanf_float(pFile, &pVec->y);
	fscanf_float(pFile, &pVec->z);
}

void fscanf_Color(FILE *pFile, Color *pCol) 
{
	fscanf_int(pFile, &pCol->r);
	fscanf_int(pFile, &pCol->g);
	fscanf_int(pFile, &pCol->b);
	fscanf_int(pFile, &pCol->a);
}