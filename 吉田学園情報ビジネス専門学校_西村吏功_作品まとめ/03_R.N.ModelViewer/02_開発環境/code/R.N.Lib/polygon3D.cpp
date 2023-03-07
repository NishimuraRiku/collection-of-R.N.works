//========================================
// 
// �|���S��(3D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon3D.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// �}�N����`
//****************************************
// ��(3D)�̍ő吔
#define LINE3D_MAX (1024)

//****************************************
// �\���̂̒�`
//****************************************
// �|���S��(3D)�֘A�̊Ǘ����\����
typedef struct
{
	int nLineNum;	// �����蔻��̐��̐�
}Polygon3DControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �擾 ***
// �|���S��(3D)�֘A�̊Ǘ����̃p�����[�^�[�����l��Ԃ�
Polygon3DControl InitParameterPolygon3DControl(void);

//****************************************
// �O���[�o���ϐ��錾
//****************************************
// ��(3D)�̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine3D = NULL;
Polygon3DControl g_polygon3DControl;	// �|���S��(3D)�֘A�̊Ǘ����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================
//========================================
// InitParameterPolygon3DControl�֐� - �|���S��(3D)�֘A�̊Ǘ����̃p�����[�^�[�����l��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
Polygon3DControl InitParameterPolygon3DControl(void)
{
	return{
		0,	// ���̐�
	};
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================
//========================================
// InitPolygon3D�֐� - �|���S��(3D)�֘A�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitPolygon3D(void)
{
	// �����֘A�����̊Ǘ����̃|�C���^
	Polygon3DControl pPoly = g_polygon3DControl;

	// �p�����[�^�[�̏�����
	pPoly = InitParameterPolygon3DControl();

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * LINE3D_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine3D,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLine3D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLine = 0; nCntLine < LINE3D_MAX; nCntLine++, pVtx += 2)
	{
		// ���_�J���[��ݒ�
		pVtx[0].col = ColorToD3DCOLOR(INITCOLOR);
		pVtx[1].col = ColorToD3DCOLOR(INITCOLOR);
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffLine3D->Unlock();
}

//========================================
// DrawPolygon3D�֐� - �|���S��(3D)�֘A�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawPolygon3D(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtx;
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// �����_�����O�X�e�[�g��ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLine3D, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntLine = 0; nCntLine < g_polygon3DControl.nLineNum; nCntLine++)
	{
		// �|���S�����̕`��
		pDevice->DrawPrimitive(D3DPT_LINELIST, 2 * nCntLine, 1);
	}

	// �����_�����O�X�e�[�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//========================================
// InitLine3DNum�֐� - ��(3D)�̐��������� -
// Author:RIKU NISHIMURA
//========================================
void InitLine3DNum(void)
{
	// ���̐���������
	g_polygon3DControl.nLineNum = 0;
}

//============================================================
//--------------------| *** �ݒ� *** |------------------------
//============================================================
//========================================
// SetLine3D�֐� - ��(3D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetLine3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, Color col)
{
	// �����֘A�����̊Ǘ����̃|�C���^
	Polygon3DControl *pPoly = &g_polygon3DControl;

	if (pPoly->nLineNum >= LINE3D_MAX)
	{
		return;	// �����蔻��̐��̐����ő吔�ɒB���Ă��鎞�A
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLine3D->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^��i�߂�
	pVtx += pPoly->nLineNum * 2;

	// �ʒu����
	pVtx[0].pos = pos1;
	pVtx[1].pos = pos2;

	// �F����
	pVtx[0].col = ColorToD3DCOLOR(col);
	pVtx[1].col = ColorToD3DCOLOR(col);

	// ���_���W���A�����b�N����
	g_pVtxBuffLine3D->Unlock();

	// �����蔻��̐��̐������Z
	pPoly->nLineNum++;
}

//========================================
// SetVertexPos3D�֐� - ���_���W�̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight)
{
	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
	pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
}

//========================================
// SetNormalLine3D�֐� - �@���x�N�g���̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor) 
{
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
void SetVertexColor3D(VERTEX_3D *pVtx, Color col)
{
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
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
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
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
}

//========================================
// SetMaterial�֐� - �}�e���A���̐ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col)
{
	D3DXCOLOR mtlColTemp = pMat->Diffuse;	// �}�e���A���̐F��ۑ�

	// �}�e���A���̐F��ݒ�
	pMat->Diffuse.r = mtlColTemp.r * ((float)col.r / (float)255);
	pMat->Diffuse.g = mtlColTemp.g * ((float)col.g / (float)255);
	pMat->Diffuse.b = mtlColTemp.b * ((float)col.b / (float)255);
	pMat->Diffuse.a = mtlColTemp.a * ((float)col.a / (float)255);

	// �}�e���A���̐ݒ�
	pDevice->SetMaterial(pMat);

	// �}�e���A���̐F�����ɖ߂�
	pMat->Diffuse = mtlColTemp;
}