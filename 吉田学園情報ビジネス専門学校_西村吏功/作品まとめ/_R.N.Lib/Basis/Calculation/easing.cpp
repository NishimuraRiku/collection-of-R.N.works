//========================================
// 
// �C�[�W���O�����̂܂Ƃ܂�
// Author:RIKU NISHIMURA
// 
//========================================
// *** chunk.cpp ***
//========================================
#include "../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================
// �J�E���g�ɉ��������l��ԏ���
float Easing(const EASE ease, const int nCount, const int nCountMax) {
	float fCount = nCount;	// ���������߂�ו��������ɒu��������

	switch (ease) {
		//----- ���� -----
	case EASE_LINEAR:
		return fCount / nCountMax;

		//----- ���X�ɉ��� -----(�m)
	case EASE_IN:
		fCount /= nCountMax;
		return fCount * fCount;

		//----- ���X�Ɍ��� -----(���m)
	case EASE_OUT:
		fCount /= nCountMax;
		return -fCount * (fCount - 2.0f);

		break;
		//----- �������Č��� -----(�m�����m)
	case EASE_INOUT:
		fCount /= nCountMax / 2.0f;
		if (fCount < 1) {
			return 0.5f * fCount * fCount;
		}

		fCount = fCount - 1.0f;
		return -0.5f * (fCount * (fCount - 2.0f) - 1.0f);
	}

	return 0.0f;
}