//========================================
// 
// �������֘A�����̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** memory.h ***
//========================================
#ifndef _MEMORY_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MEMORY_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �������m�ۏ���
void GetMemory(void **pPointer, size_t singleSize, int nNum);
// �������Ċm�ۏ���
void ReGetMemory(void **pPointer, size_t singleSize, int nNumOld, int nNumNew);
// �������������
void ReleaseMemory(void **pPointer);

#endif