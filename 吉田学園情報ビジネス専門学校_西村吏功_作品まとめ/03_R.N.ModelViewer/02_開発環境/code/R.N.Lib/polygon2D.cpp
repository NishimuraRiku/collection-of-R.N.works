//========================================
// 
// �|���S��(2D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon2D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================
//========================================
// GetPolygon2DNum�֐� - �|���S��(2D)�̕K�v����Ԃ� -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax)
{
	int nNum = 0;	// �|���S����

	// �|���S�����̍ő吔�����Z
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nNum += pPoly2D->nMax;
	}

	return nNum;
}

//========================================
// GetPolygon2DIndex�֐� - ���݂̃J�E���g�̃|���S��(2D)�̔ԍ���Ԃ� -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount)
{
	int nLine = 0;	// ���C��

	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nLine += pPoly2D->nMax;	// ���C���ɍő吔�����Z

		if (nCount < nLine) 
		{// �J�E���g�����C�������̎��A�J�E���g�̒l��Ԃ�
			return nCntPoly;
		}
	}

	return 0;
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================
//========================================
// LoadPolygon2DTexture�֐� - �|���S��(2D)�̃e�N�X�`���̓ǂݍ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax) 
{
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++, pTextureUi_map_00++)
	{
		D3DXCreateTextureFromFile(pDevice, pPoly2D->aTexturePath, pTextureUi_map_00);
	}
}

//============================================================
//--------------------| *** �ݒ� *** |------------------------
//============================================================
//========================================
// SetVertexPos2D�֐� - ���_���W�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType)
{
	if (bMatchCamera)
	{// �ʒu���J�����ɍ��킹��t���O���^�̎��A
		// �ʒu���J�����ɍ��킹��
		MatchCamera2DPosition(&pos);
	}

	if ((angleType == ANGLE_TYPE_FIXED) || (rot.z == D3DX_PI))
	{// �p�x�̎�ނ��Œ�A�������͊p�x���^��̎��A
		// ���_���W��ݒ�
		pVtx[0].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y + (fHeight * 0.5f);
	}
	else if (rot.z == 0.0f)
	{// �p�x���^���̎��A
		// ���_���W��ݒ�
		pVtx[0].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y - (fHeight * 0.5f);
	}
	else 
	{
		// �Ίp���̒���
		float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;
		// �Ίp���̊p�x
		float fAngle = atan2f(fWidth, fHeight);

		// ���_���W��ݒ�
		pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
		pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
	}
}

//========================================
// SetRHW2D�֐� - rhw�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetRHW2D(VERTEX_2D *pVtx) 
{
	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// SetVertexColor2D�֐� - ���_�F�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor2D(VERTEX_2D *pVtx, Color col)
{
	// ���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos2D�֐� - �e�N�X�`�����W�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
{
	if (nPtnMaxX * nPtnMaxY == 0) 
	{// �p�^�[���̏����0�̎��A
		// �������I������
		return;
	}

	D3DXVECTOR3 pos;	// �ʒu

	if (pPos == NULL) 
	{// �ʒu�̃|�C���^��NULL�̎��A
		// �ʒu��������
		pos = D3DXVECTOR3(0, 0, 0);
	}
	else 
	{// �ʒu�̃|�C���^��NULL�Ŗ������A
		// �ʒu��-�␳���đ��
		pos = -*pPos;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
}

//========================================
// SetTexturePos2DCut�֐� - �e�N�X�`�����W�̐ݒ菈��(2D)(�؂���ver) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos)
{
	pos.x += 0.5f;
	pos.y += 0.5f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[1].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[2].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
	pVtx[3].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
}

//============================================================
//--------------------| *** ���̑� *** |----------------------
//============================================================
//========================================
// MatchPosition�֐� - �`��ʒu����ɍ��킹�� -
// Author:RIKU NISHIMURA
//========================================
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos)
{
	// �ʒu����̈ʒu�ɍ��킹��
	pDrawPos->x = pDrawPos->x - basePos.x + (SCREEN_WIDTH * 0.5f);
	pDrawPos->y = pDrawPos->y - basePos.y + (SCREEN_HEIGHT * 0.5f);
}