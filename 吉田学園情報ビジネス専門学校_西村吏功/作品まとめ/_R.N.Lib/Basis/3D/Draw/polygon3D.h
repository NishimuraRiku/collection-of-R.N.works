//========================================
// 
// �|���S��(3D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon3D.h ***
//========================================
#ifndef _POLYGON3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _POLYGON3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// �|���S��(3D)�̍ő吔
#define POLYGON3D_MAX (1024)

//****************************************
// �\���̒�`
//****************************************
// �|���S��(3D)�̐ݒ���\����
typedef struct {
	int nTex        = -1;				// �e�N�X�`���ԍ�
	int nPtn        = 0;				// �p�^�[���ԍ�
	int nPtnX       = 1;				// �p�^�[�����X
	int nPtnY       = 1;				// �p�^�[�����Y
	float fWidth    = 0.0f;				// ��
	float fHeight   = 0.0f;				// ����
	D3DXMATRIX mtx  = INITD3DXMATRIX;	// �}�g���b�N�X
	Color col       = INITCOLOR;		// �F
	bool bZtest     = true;				// Z�e�X�g�t���O
	bool bLighting  = true;				// ���C�e�B���O�t���O
	bool bBillboard = false;			// �r���{�[�h�t���O
	bool bMirror    = false;			// ���E���]�t���O
}Polygon3DSet;

// �|���S��(3D)�̊Ǘ����\����
typedef struct {
	// �|���S��(3D)�̐ݒ���
	Polygon3DSet aPolySet[POLYGON3D_MAX];
	int nPolygonNum = 0;	// �|���S��(3D)�̐�
}Polygon3DControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �|���S��(3D)�̏���������
void InitPolygon3D(void);
// �|���S��(3D)�̏I������
void UninitPolygon3D(void);
// �|���S��(3D)�̕`�揈��
void DrawPolygon3D(void);
//========== *** �ݒ� ***
// �|���S��(3D)�̐ݒ菈��
void SetPolygon3D(
	int nTex        = -1,
	int nPtn        = 0,
	int nPtnX       = 1,
	int nPtnY       = 1,
	float fWidth    = 0.0f,
	float fHeight   = 0.0f,
	D3DXMATRIX mtx  = INITD3DXMATRIX,
	Color col       = INITCOLOR,
	bool bZtest     = true,
	bool bLighting  = true,
	bool bBillboard = false,
	bool bMirror    = false);
//========== *** �擾 ***
// �|���S��(3D)�̊Ǘ������擾
Polygon3DControl *GetPolygon3DControl(void);
//========== *** ���_�ݒ� ***
// ���_���W�̐ݒ菈��(3D)
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight);
// �@���x�N�g���̐ݒ菈��(3D)
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
// ���_�F�̐ݒ菈��(3D)
void SetVertexColor3D(VERTEX_3D *pVtx, Color col);
// �e�N�X�`�����W�̐ݒ菈��(3D)
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos, bool bMirror = false);

#endif