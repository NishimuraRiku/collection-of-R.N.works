//========================================
// 
// �|���S��(3D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon3D.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//****************************************
// �O���[�o���ϐ��錾
//****************************************
// �|���S��(3D)�̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffpolygon3D = NULL;
// �|���S��(3D)�̊Ǘ����
Polygon3DControl g_polygon3DControl;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================

//========================================
// InitPolygon3D�֐� - �|���S��(3D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitPolygon3D(void) {
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * POLYGON3D_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffpolygon3D,
		NULL);
}

//========================================
// UninitPolygon3D�֐� - �|���S��(3D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitPolygon3D(void) {
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffpolygon3D != NULL) {
		g_pVtxBuffpolygon3D->Release();
		g_pVtxBuffpolygon3D = NULL;
	}
}

//========================================
// DrawPolygon3D�֐� - �|���S��(3D)�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawPolygon3D(void) {
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxSelf;		// �{�̃}�g���b�N�X
	D3DXMATRIX mtxView;		// �r���[�}�g���b�N�X�擾�p

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffpolygon3D, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	int aOrderList[POLYGON3D_MAX];	// ���ԃ��X�g

	{// �J�����ʂ܂ł̋����Ń\�[�g
		// �J�����ʂ܂ł̋������v�Z&�i�[
		float aDistance[POLYGON3D_MAX];
		for (int nCntPoly = 0; nCntPoly < g_polygon3DControl.nPolygonNum; nCntPoly++) {
			aDistance[nCntPoly] = FindDistanceToCameraPlane(ConvMatrixToPos(g_polygon3DControl.aPolySet[nCntPoly].mtx));
			aOrderList[nCntPoly] = nCntPoly;
		}

		float fTemp;		// �ꎞ�I�ɒl���i�[
		int nTempCnt = -1;	// �J�E���g���i�[
		for (int nCntCompare1 = 0; nCntCompare1 < g_polygon3DControl.nPolygonNum - 1; nCntCompare1++) {
			// �擪�̗v�f���ŏ��l�Ƃ��ċL��
			fTemp = aDistance[nCntCompare1];

			for (int nCntCompare2 = nCntCompare1 + 1; nCntCompare2 < g_polygon3DControl.nPolygonNum; nCntCompare2++) {
				if (aDistance[nCntCompare2] > fTemp)
				{// �v�f�̒l���ŏ��l�������Ă������A
					// �l���L��
					fTemp = aDistance[nCntCompare2];
					// �J�E���g���L��
					nTempCnt = nCntCompare2;
				}
			}

			if (aDistance[nCntCompare1] < fTemp)
			{// �ŏ��l���擪�̗v�f�̒l��������Ă������A
				if (nTempCnt == -1) {
					assert(false);
					nTempCnt = 0;
				}

				// �l�����ւ���
				aDistance[nTempCnt] = aDistance[nCntCompare1];
				aDistance[nCntCompare1] = fTemp;

				// ���Ԃ����ւ���
				int nTemp = aOrderList[nTempCnt];
				aOrderList[nTempCnt] = aOrderList[nCntCompare1];
				aOrderList[nCntCompare1] = nTemp;
			}
		}
	}

	for (int nCntPoly = 0; nCntPoly < g_polygon3DControl.nPolygonNum; nCntPoly++) {
		int nPoly = aOrderList[nCntPoly];

		// �|���S��(3D)�̐ݒ���
		Polygon3DSet polygon3DSet = g_polygon3DControl.aPolySet[nPoly];

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxSelf);

		if (!polygon3DSet.bZtest)
		{// Z�e�X�g�t���O���U�̎��A
			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}
		else 
		{// Z�e�X�g�t���O���^�̎��A
			// Z�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

		// ���C�e�B���O�̗L��/�����ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, polygon3DSet.bLighting);

		if (polygon3DSet.bBillboard)
		{// �r���{�[�h�t���O���^�̎��A
			// �ʒu�}�g���b�N�X��ݒ�
			D3DXVECTOR3 setPos = ConvMatrixToPos(polygon3DSet.mtx);
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&mtxSelf, NULL, &mtxView);
			mtxSelf._41 = 0.0f;
			mtxSelf._42 = 0.0f;
			mtxSelf._43 = 0.0f;

			// �ʒu�}�g���b�N�X��ݒ�
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);
		}
		else
		{// �r���{�[�h�t���O���U�̎��A
			// �ʒu�ƌ����𔽉f
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &polygon3DSet.mtx);
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

		// �e�N�X�`���̐ݒ�
		SetTexture(pDevice, polygon3DSet.nTex);

		// �|���S��(3D)�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nPoly, 2);
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================
//--------------------| *** �ݒ� *** |------------------------
//============================================================

//========================================
// SetPolygon3D�֐� - �|���S��(3D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetPolygon3D(int nTex, int nPtn, int nPtnX, int nPtnY, float fWidth, float fHeight, D3DXMATRIX mtx, Color col, bool bZtest, bool bLighting, bool bBillboard, bool bMirror) {
	if (g_polygon3DControl.nPolygonNum >= POLYGON3D_MAX)
	{// �|���S��(3D)�̐����ő吔�ɒB���Ă��鎞�A
		assert(false);
		return;	// �������I������
	}

	// �|���S����
	int nPolyNum = g_polygon3DControl.nPolygonNum;

	// �ݒ������
	g_polygon3DControl.aPolySet[nPolyNum].nTex       = nTex; 
	g_polygon3DControl.aPolySet[nPolyNum].nPtn       = nPtn;
	g_polygon3DControl.aPolySet[nPolyNum].nPtnX      = nPtnX;
	g_polygon3DControl.aPolySet[nPolyNum].nPtnY      = nPtnY;
	g_polygon3DControl.aPolySet[nPolyNum].fWidth     = fWidth;
	g_polygon3DControl.aPolySet[nPolyNum].fHeight    = fHeight;
	g_polygon3DControl.aPolySet[nPolyNum].mtx        = mtx;
	g_polygon3DControl.aPolySet[nPolyNum].col        = col;
	g_polygon3DControl.aPolySet[nPolyNum].bZtest     = bZtest;
	g_polygon3DControl.aPolySet[nPolyNum].bLighting  = bLighting;
	g_polygon3DControl.aPolySet[nPolyNum].bBillboard = bBillboard;
	g_polygon3DControl.aPolySet[nPolyNum].bMirror    = bMirror;

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffpolygon3D->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���_���ւ̃|�C���^��i�߂�
		pVtx += g_polygon3DControl.nPolygonNum * 4;

		// ���_���W�̐ݒ�
		SetVertexPos3D(pVtx, fWidth, fHeight);

		// �@���x�N�g���̐ݒ�
		SetNormalLine3D(pVtx, FindNormalVector(ConvMatrixToRot(mtx)));

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, col);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx, nPtn, nPtnX, nPtnY, NULL, bMirror);
	}
	// ���_���W���A�����b�N����
	g_pVtxBuffpolygon3D->Unlock();

	// �|���S��(3D)�̐������Z
	g_polygon3DControl.nPolygonNum++;
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================

//========================================
// GetPolygon3DControl�֐� - �|���S��(3D)�̊Ǘ������擾 -
// Author:RIKU NISHIMURA
//========================================
Polygon3DControl *GetPolygon3DControl(void) {
	return &g_polygon3DControl;
}

//============================================================
//--------------------| *** ���_�ݒ� *** |--------------------
//============================================================

//========================================
// SetVertexPos3D�֐� - ���_���W�̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight) {
	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f),  (fHeight * 0.5f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), -(fHeight * 0.5f), 0.0f);
}

//========================================
// SetNormalLine3D�֐� - �@���x�N�g���̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor) {
	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//========================================
// SetVertexColor3D�֐� - ���_�F�̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor3D(VERTEX_3D *pVtx, Color col) {
	// ���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos3D�֐� - �e�N�X�`�����W�̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos, bool bMirror) {
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

	// ���E���]�t���O�ɉ������e�N�X�`�����W�̐ݒ�
	if (bMirror) {
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x, ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                    , ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                    , (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	}
	else {
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                     , ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x , ( (nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)  + pos.y);
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x                     , (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x , (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	}
}