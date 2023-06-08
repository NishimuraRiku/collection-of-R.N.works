//========================================
// 
// �e�L�X�g(3D)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** text3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �ݒ� *** |------------------------
//============================================================

//========================================
// SetText3D�֐� - �e�L�X�g(3D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetText3D(char *pString, int nFont, TEXT_DISP disp, D3DXMATRIX mtx, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard) {
	assert(pString != NULL);

	// �J�����̌���(�����_���王�_)
	D3DXVECTOR3 cameraRot; {
		D3DXVECTOR3 posV = GetCamera3D()->posV;
		posV.y           = GetCamera3D()->posR.y;
		cameraRot        = FindRot(GetCamera3D()->posR, posV);
	}

	for (int nCntDigit = 0; nCntDigit < (int)strlen(pString); nCntDigit++) {
		if (pString[nCntDigit] == ' ') {
			continue;	// �J�E���g�̕������󔒂̎��A�܂�Ԃ�
		}

		D3DXVECTOR3 setPos = INITD3DXVECTOR3;	// �ݒ�ʒu		
		Font font          = GetFont(nFont);	// �t�H���g�̏��
		float fSpaceRate   = (float)font.nSpaceNum / (float)font.nSpaceDenom;	// �Ԋu�䗦

		// �\���`���ɉ������ݒ�ʒu�̐ݒ�
		switch (disp) {
		case TEXT_DISP_CENTER:
			// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ����̔��������Z
			setPos.y += ((fWidth * fSpaceRate) * strlen(pString)) * 0.5f;
			// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			setPos.y -= ((float)nCntDigit + 0.5f) * (fWidth * fSpaceRate);
			break;
		case TEXT_DISP_LEFT:
			// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			setPos.y -= nCntDigit * (fWidth * fSpaceRate);
			break;
		case TEXT_DISP_RIGHT:
			// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ��������Z
			setPos.y += (fWidth * fSpaceRate) * (strlen(pString) - 1);
			// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			setPos.y -= nCntDigit * (fWidth * fSpaceRate);
			break;
		}

		// ���ʃ}�g���b�N�X
		D3DXMATRIX resultMtx; {
			D3DXVECTOR3 charPos;
			D3DXVec3Cross(&charPos, &setPos, &cameraRot);

			// ��}�g���b�N�X�ƃe�L�X�g�}�g���b�N�X���|�����킹��
			resultMtx = MultiplyMatrix(
				ConvPosRotToMatrix(ConvMatrixToPos(mtx), INITD3DXVECTOR3),
				ConvPosRotToMatrix(charPos, INITD3DXVECTOR3));
		}

		// �|���S��(3D)��ݒ�
		SetPolygon3D(
			font.nTex,
			(int)pString[nCntDigit] - (int)font.cStartChar,
			font.nPtnWidth,
			font.nPtnHeight,
			fWidth,
			fHeight,
			resultMtx,
			col,
			bZtest,
			bLighting,
			bBillboard);
	}
}