//========================================
// 
// �������֘A����
// Author:RIKU NISHIMURA
// 
//========================================
// *** memory.cpp ***
//========================================
#include <assert.h>
// R.N.Lib
#include "../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// GetMemory�֐� - �������m�ۏ��� -
// Author:RIKU NISHIMURA
//========================================
void GetMemory(void **ppPointer, size_t singleSize, int nNum) {
	ReleaseMemory(ppPointer);	// ���������
	assert(singleSize * nNum > 0);
	void *pNew = malloc(singleSize * nNum);	// �m��
	assert(pNew != NULL);	// !�������m�ێ��s
	*ppPointer = pNew;	// �V�����|�C���^�ɓ���ւ���
}

//========================================
// ReGetMemory�֐� - �������Ċm�ۏ��� -
// Author:RIKU NISHIMURA
//========================================
void ReGetMemory(void **ppPointer, size_t singleSize, int nNumOld, int nNumNew) {
	if (nNumNew < nNumOld)
	{// �V�T�C�Y�����ߋ��̃T�C�Y����������Ă��鎞�A
		nNumOld = nNumNew;	// �ߋ��T�C�Y����V�T�C�Y���Ƃ���
	}

	// �V�����T�C�Y��0�ȉ��̎��ANULL������
	if (singleSize * nNumNew <= 0) {
		*ppPointer = NULL;
	}

	// �V�����|�C���^�̃������m��
	void *pNew = NULL;
	GetMemory(&pNew, singleSize, nNumNew);	// �������m��

	if (nNumOld > 0) {
		memcpy(pNew, *ppPointer, singleSize * nNumOld); // �f�[�^���R�s�[
	}
	ReleaseMemory(ppPointer);	// �Â��̈�����
	assert(pNew != NULL);	// !�������m�ێ��s
	*ppPointer = pNew;	// �V�����|�C���^�ɓ���ւ���
}

//========================================
// ReleaseMemory�֐� - ������������� -
// Author:RIKU NISHIMURA
//========================================
void ReleaseMemory(void **ppPointer) {
	if (*ppPointer != NULL) {
		delete[] *ppPointer;	// ���
		*ppPointer = NULL;
	}
}