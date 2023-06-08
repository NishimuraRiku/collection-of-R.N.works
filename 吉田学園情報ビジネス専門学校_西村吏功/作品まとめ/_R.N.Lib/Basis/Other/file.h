//========================================
// 
// �t�@�C���֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** file.h ***
//========================================
#ifndef _FILE_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _FILE_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �擾 ***
// �J���t�@�C����I�����擾
bool OpenFile(char *pFileName, char *pInitDir, char *pFileType);
// �ۑ�����t�@�C����I�����擾
bool SaveFile(char *pFileName, char *pInitDir, char *pFileType);
// "data"���N�_�Ƃ����p�X������𐶐�����
char *ConvertToDataPath(const char* path);
//========== *** �e�L�X�g�ǂݍ��� ***
bool DataSearch(FILE *pFile, char *pDataSearch, char *pEndLabel);
void fscanf_int(FILE *pFile, int *pData);
void fscanf_float(FILE *pFile, float *pData);
void fscanf_string(FILE *pFile, char *pString);
void fscanf_bool(FILE *pFile, bool *pFlag);
void fscanf_D3DXVECTOR3(FILE *pFile, D3DXVECTOR3 *pVec);
void fscanf_Color(FILE *pFile, Color *pCol);

#endif